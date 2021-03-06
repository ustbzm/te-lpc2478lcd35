/*
 * (C) Copyright 2003
 * Author : Hamid Ikdoumi (Atmel)
 *
 * See file CREDITS for list of people who contributed to this
 * project.
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation; either version 2 of
 * the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston,
 * MA 02111-1307 USA
 */

#include <at91_net.h>
#include <net.h>
#include <dm9161.h>

/* for test purposes only, should be moved to e.g. lan.c */

#define MICREL_ID_1 0x0022
#define MICREL_ID_2 0x1619

#ifdef CONFIG_DRIVER_ETHER

#if defined(CONFIG_CMD_NET)

/*
 * Name:
 *	dm9161a_IsPhyConnected
 * Description:
 *	Reads the 2 PHY ID registers
 * Arguments:
 *	p_mac - pointer to AT91S_EMAC struct
 * Return value:
 *	TRUE - if id read successfully
 *	FALSE- if error
 */
static unsigned int dm9161a_IsPhyConnected (AT91PS_EMAC p_mac)
{
	unsigned short Id1, Id2;

	at91_EmacEnableMDIO (p_mac);
	at91_EmacReadPhy (p_mac, SAM9260EK_PHY_ADDRESS, DM9161_PHYID1, &Id1);
	at91_EmacReadPhy (p_mac, SAM9260EK_PHY_ADDRESS, DM9161_PHYID2, &Id2);
	at91_EmacDisableMDIO (p_mac);
	
	/*printf(" Id1 0x%04x\n", Id1);
	printf(" Id1 0x%04x\n", Id2);*/
	

	if ((Id1 == (DM9161_PHYID1_OUI >> 6)) &&
	    ((Id2 >> 10) == (DM9161_PHYID1_OUI & DM9161_LSB_MASK))) {
	  printf("DM9161A PHY Detected\n\r");
	  return TRUE;
	}
	
	if ((Id1 == MICREL_ID_1) && (Id2 == MICREL_ID_2)) {
		printf("KS8721 PHY Detected\n\r");
		return TRUE;
	}
	
	return FALSE;
}

/*
 * Name:
 *	dm9161a_GetLinkSpeed
 * Description:
 *	Link parallel detection status of MAC is checked and set in the
 *	MAC configuration registers
 * Arguments:
 *	p_mac - pointer to MAC
 * Return value:
 *	TRUE - if link status set succesfully
 *	FALSE - if link status not set
 */
static uchar dm9161a_GetLinkSpeed (AT91PS_EMAC p_mac)
{
	unsigned short stat1, stat2;

	if (!at91_EmacReadPhy (p_mac, SAM9260EK_PHY_ADDRESS, DM9161_BMSR, &stat1))
		return FALSE;

	if (!(stat1 & DM9161_LINK_STATUS))	/* link status up? */
		return FALSE;

	if (!at91_EmacReadPhy (p_mac, SAM9260EK_PHY_ADDRESS, DM9161_DSCSR, &stat2))
		return FALSE;
	
	/*printf("made it to the link speed selection with stat1: 0x%04x, stat2: 0x%04x\n", stat1, stat2);*/

	/*if ((stat1 & DM9161_100BASE_TX_FD) && (stat2 & DM9161_100FDX)) {*/
	if (stat1 & DM9161_100BASE_TX_FD) {
		/*set Emac for 100BaseTX and Full Duplex  */
		p_mac->EMAC_NCFGR |= AT91C_EMAC_SPD | AT91C_EMAC_FD;
		return TRUE;
	}

	/*if ((stat1 & DM9161_10BASE_T_FD) && (stat2 & DM9161_10FDX)) {*/
	if (stat1 & DM9161_10BASE_T_FD) {
		/*set MII for 10BaseT and Full Duplex  */
		p_mac->EMAC_NCFGR = (p_mac->EMAC_NCFGR &
				~(AT91C_EMAC_SPD | AT91C_EMAC_FD))
				| AT91C_EMAC_FD;
		return TRUE;
	}

	/*if ((stat1 & DM9161_100BASE_TX_HD) && (stat2 & DM9161_100HDX)) {*/
	if (stat1 & DM9161_100BASE_TX_HD) {
		/*set MII for 100BaseTX and Half Duplex  */
		p_mac->EMAC_NCFGR = (p_mac->EMAC_NCFGR &
				~(AT91C_EMAC_SPD | AT91C_EMAC_FD))
				| AT91C_EMAC_SPD;
		return TRUE;
	}

	/*if ((stat1 & DM9161_10BASE_T_HD) && (stat2 & DM9161_10HDX)) {*/
	if (stat1 & DM9161_10BASE_T_HD) {
		/*set MII for 10BaseT and Half Duplex  */
		p_mac->EMAC_NCFGR &= ~(AT91C_EMAC_SPD | AT91C_EMAC_FD);
		return TRUE;
	}

	return FALSE;
}


/*
 * Name:
 *	dm9161a_ResetPhy
 * Description:
 *	Resets PHY
 * Arguments:
 *	p_mac - pointer to struct AT91S_EMAC
 * Return value:
 *	TRUE - if link status set succesfully
 *	FALSE - if link status not set
 */
static uchar dm9161a_ResetPhy(AT91PS_EMAC p_mac)
{
        unsigned short bmcr;
	int timeout = 10;
	uchar ret = TRUE;
	unsigned short Id1;
	unsigned int i;
	
	/*for (i=0; i<32;i++)
	{
		at91_EmacEnableMDIO (p_mac);
		at91_EmacReadPhy (p_mac, i, DM9161_PHYID1, &Id1);
		at91_EmacDisableMDIO (p_mac);
		printf(" adress %d : 0x%04x\n", i, Id1);
	}*/
	
	


	bmcr = DM9161_RESET;
	at91_EmacEnableMDIO (p_mac);
	at91_EmacWritePhy (p_mac, SAM9260EK_PHY_ADDRESS, DM9161_BMCR, &bmcr);

	do {
	  at91_EmacReadPhy (p_mac, SAM9260EK_PHY_ADDRESS, DM9161_BMCR, &bmcr);
	  at91_EmacReadPhy (p_mac, SAM9260EK_PHY_ADDRESS, DM9161_BMCR, &bmcr);
	  timeout--;
	} while ((bmcr & DM9161_RESET) && timeout);

	at91_EmacDisableMDIO (p_mac);

	if (!timeout)
	  ret = FALSE;

	return ret;
}


/*
 * Name:
 *	dm9161a_InitPhy
 * Description:
 *	MAC starts checking its link by using parallel detection and
 *	Autonegotiation and the same is set in the MAC configuration registers
 * Arguments:
 *	p_mac - pointer to struct AT91S_EMAC
 * Return value:
 *	TRUE - if link status set succesfully
 *	FALSE - if link status not set
 */
static uchar dm9161a_InitPhy (AT91PS_EMAC p_mac)
{
	unsigned int tick = 0;
	unsigned short IntValue;
	uchar ret = TRUE;

	at91_EmacEnableMDIO (p_mac);

	do {
		/* Link status is latched, so read twice to get current value */
		at91_EmacReadPhy(p_mac, SAM9260EK_PHY_ADDRESS, DM9161_BMSR, &IntValue);
		at91_EmacReadPhy(p_mac, SAM9260EK_PHY_ADDRESS, DM9161_BMSR, &IntValue);
	
		tick++;
	} while (!(IntValue & DM9161_LINK_STATUS) && (tick < AT91C_ETH_TIMEOUT));

	if (!(IntValue & DM9161_LINK_STATUS)){
		at91_EmacDisableMDIO (p_mac);
		printf ("No link\n\r");
		ret = FALSE;
		return ret;
	}

	if (!dm9161a_GetLinkSpeed (p_mac)) {
		/* Try another time */
		ret = dm9161a_GetLinkSpeed (p_mac);
	}

	/* Disable PHY Interrupts */
	at91_EmacReadPhy (p_mac, SAM9260EK_PHY_ADDRESS, DM9161_MDINTR, &IntValue);
	/* clear FDX, SPD, Link, INTR masks */
	IntValue &= ~(DM9161_FDX_MASK | DM9161_SPD_MASK |
		      DM9161_LINK_MASK | DM9161_INTR_MASK);
	at91_EmacWritePhy (p_mac, SAM9260EK_PHY_ADDRESS, DM9161_MDINTR, &IntValue);
	at91_EmacDisableMDIO (p_mac);

	return (ret);
}


/*
 * Name:
 *	dm9161a_AutoNegotiate
 * Description:
 *	MAC Autonegotiates with the partner status of same is set in the
 *	MAC configuration registers
 * Arguments:
 *	dev - pointer to struct net_device
 * Return value:
 *	TRUE - if link status set successfully
 *	FALSE - if link status not set
 */
static uchar dm9161a_AutoNegotiate (AT91PS_EMAC p_mac, int *status)
{
	unsigned short value;
	unsigned short PhyAnar;
	unsigned short PhyAnalpar;

	/* Set dm9161 control register */
	if (!at91_EmacReadPhy (p_mac, SAM9260EK_PHY_ADDRESS, DM9161_BMCR, &value))
		return FALSE;
	value &= ~DM9161_AUTONEG;	/* remove autonegotiation enable */
	value |= DM9161_ISOLATE;	/* Electrically isolate PHY */
	if (!at91_EmacWritePhy (p_mac, SAM9260EK_PHY_ADDRESS, DM9161_BMCR, &value))
		return FALSE;

	/* Set the Auto_negotiation Advertisement Register */
	/* MII advertising for Next page, 100BaseTxFD and HD, 10BaseTFD and HD, IEEE 802.3 */
	PhyAnar = DM9161_NP | DM9161_TX_FDX | DM9161_TX_HDX |
		  DM9161_10_FDX | DM9161_10_HDX | DM9161_AN_IEEE_802_3;
	if (!at91_EmacWritePhy (p_mac, SAM9260EK_PHY_ADDRESS, DM9161_ANAR, &PhyAnar))
		return FALSE;

	/* Read the Control Register     */
	if (!at91_EmacReadPhy (p_mac, SAM9260EK_PHY_ADDRESS, DM9161_BMCR, &value))
		return FALSE;

	value |= DM9161_SPEED_SELECT | DM9161_AUTONEG | DM9161_DUPLEX_MODE;
	if (!at91_EmacWritePhy (p_mac, SAM9260EK_PHY_ADDRESS, DM9161_BMCR, &value))
		return FALSE;
	/* Restart Auto_negotiation  */
	value |= DM9161_RESTART_AUTONEG;
	if (!at91_EmacWritePhy (p_mac, SAM9260EK_PHY_ADDRESS, DM9161_BMCR, &value))
		return FALSE;

	/*check AutoNegotiate complete */
	udelay (10000);
	at91_EmacReadPhy (p_mac, SAM9260EK_PHY_ADDRESS, DM9161_BMSR, &value);
	if (!(value & DM9161_AUTONEG_COMP))
		return FALSE;

	/* Get the AutoNeg Link partner base page */
	if (!at91_EmacReadPhy (p_mac, SAM9260EK_PHY_ADDRESS, DM9161_ANLPAR, &PhyAnalpar))
		return FALSE;

	if ((PhyAnar & DM9161_TX_FDX) && (PhyAnalpar & DM9161_TX_FDX)) {
		/*set MII for 100BaseTX and Full Duplex  */
		p_mac->EMAC_NCFGR |= AT91C_EMAC_SPD | AT91C_EMAC_FD;
		return TRUE;
	}

	if ((PhyAnar & DM9161_10_FDX) && (PhyAnalpar & DM9161_10_FDX)) {
		/*set MII for 10BaseT and Full Duplex  */
		p_mac->EMAC_NCFGR = (p_mac->EMAC_NCFGR &
				~(AT91C_EMAC_SPD | AT91C_EMAC_FD))
				| AT91C_EMAC_FD;
		return TRUE;
	}
	return FALSE;
}


/*
 * Name:
 *	at91_GetPhyInterface
 * Description:
 *	Initialise the interface functions to the PHY
 * Arguments:
 *	None
 * Return value:
 *	None
 */
void at91_GetPhyInterface(AT91PS_PhyOps p_phyops)
{
	p_phyops->Init           = dm9161a_InitPhy;
	p_phyops->IsPhyConnected = dm9161a_IsPhyConnected;
	p_phyops->GetLinkSpeed   = dm9161a_GetLinkSpeed;
	p_phyops->AutoNegotiate  = dm9161a_AutoNegotiate;
	p_phyops->Reset          = dm9161a_ResetPhy;
}

#endif	/* CONFIG_CMD_NET */

#endif	/* CONFIG_DRIVER_ETHER */

