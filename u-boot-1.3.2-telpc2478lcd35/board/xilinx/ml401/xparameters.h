/*
 * (C) Copyright 2007 Michal Simek
 *
 * Michal  SIMEK <monstr@monstr.eu>
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
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston,
 * MA 02111-1307 USA
 *
 * CAUTION: This file is automatically generated by libgen.
 * Version: Xilinx EDK 8.2.02 EDK_Im_Sp2.4
 */

/* System Clock Frequency */
#define XILINX_CLOCK_FREQ	100000000

/* Microblaze is microblaze_0 */
#define XILINX_USE_MSR_INSTR	1
#define XILINX_FSL_NUMBER	3

/* Interrupt controller is opb_intc_0 */
#define XILINX_INTC_BASEADDR	0x41200000
#define XILINX_INTC_NUM_INTR_INPUTS	6

/* Timer pheriphery is opb_timer_1 */
#define XILINX_TIMER_BASEADDR	0x41c00000
#define XILINX_TIMER_IRQ	0

/* Uart pheriphery is RS232_Uart */
#define XILINX_UART_BASEADDR	0x40600000
#define XILINX_UART_BAUDRATE	115200

/* IIC pheriphery is IIC_EEPROM */
#define XILINX_IIC_0_BASEADDR	0x40800000
#define XILINX_IIC_0_FREQ	100000
#define XILINX_IIC_0_BIT	0

/* GPIO is LEDs_4Bit*/
#define XILINX_GPIO_BASEADDR	0x40000000

/* Flash Memory is FLASH_2Mx32 */
#define XILINX_FLASH_START	0x2c000000
#define XILINX_FLASH_SIZE	0x00800000

/* Main Memory is DDR_SDRAM_64Mx32 */
#define XILINX_RAM_START	0x28000000
#define XILINX_RAM_SIZE	0x04000000

/* Sysace Controller is SysACE_CompactFlash */
#define XILINX_SYSACE_BASEADDR	0x41800000
#define XILINX_SYSACE_HIGHADDR	0x4180ffff
#define XILINX_SYSACE_MEM_WIDTH	16

/* Ethernet controller is Ethernet_MAC */
#define XPAR_XEMAC_NUM_INSTANCES	1
#define XPAR_OPB_ETHERNET_0_DEVICE_ID	0
#define XPAR_OPB_ETHERNET_0_BASEADDR	0x40c00000
#define XPAR_OPB_ETHERNET_0_HIGHADDR	0x40c0ffff
#define XPAR_OPB_ETHERNET_0_DMA_PRESENT	1
#define XPAR_OPB_ETHERNET_0_ERR_COUNT_EXIST	1
#define XPAR_OPB_ETHERNET_0_MII_EXIST	1