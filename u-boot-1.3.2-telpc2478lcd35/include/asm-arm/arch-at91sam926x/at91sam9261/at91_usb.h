/*  ----------------------------------------------------------------------------
 *          ATMEL Microcontroller Software Support  -  ROUSSET  -
 *  ----------------------------------------------------------------------------
 *  DISCLAIMER:  THIS SOFTWARE IS PROVIDED BY ATMEL "AS IS" AND ANY EXPRESS OR
 *  IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
 *  MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NON-INFRINGEMENT ARE
 *  DISCLAIMED. IN NO EVENT SHALL ATMEL BE LIABLE FOR ANY DIRECT, INDIRECT,
 *  INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
 *  LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA,
 *  OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
 *  LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
 *  NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
 *  EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *  ----------------------------------------------------------------------------
 * File Name           : AT91SAM9261.h
 * Object              : AT91SAM9261 definitions
 * Generated           : AT91 SW Application Group  03/30/2005 (17:05:06)
 * 
 *  ----------------------------------------------------------------------------
*/
#ifndef AT91SAM9261_USB_H
#define AT91SAM9261_USB_H

/* ***************************************************************************** */
/*              SOFTWARE API DEFINITION  FOR USB Host Interface */
/* ***************************************************************************** */
typedef struct _AT91S_UHP {
	AT91_REG	 UHP_HcRevision; 	/* Revision */
	AT91_REG	 UHP_HcControl; 	/* Operating modes for the Host Controller */
	AT91_REG	 UHP_HcCommandStatus; 	/* Command & status Register */
	AT91_REG	 UHP_HcInterruptStatus; 	/* Interrupt Status Register */
	AT91_REG	 UHP_HcInterruptEnable; 	/* Interrupt Enable Register */
	AT91_REG	 UHP_HcInterruptDisable; 	/* Interrupt Disable Register */
	AT91_REG	 UHP_HcHCCA; 	/* Pointer to the Host Controller Communication Area */
	AT91_REG	 UHP_HcPeriodCurrentED; 	/* Current Isochronous or Interrupt Endpoint Descriptor */
	AT91_REG	 UHP_HcControlHeadED; 	/* First Endpoint Descriptor of the Control list */
	AT91_REG	 UHP_HcControlCurrentED; 	/* Endpoint Control and Status Register */
	AT91_REG	 UHP_HcBulkHeadED; 	/* First endpoint register of the Bulk list */
	AT91_REG	 UHP_HcBulkCurrentED; 	/* Current endpoint of the Bulk list */
	AT91_REG	 UHP_HcBulkDoneHead; 	/* Last completed transfer descriptor */
	AT91_REG	 UHP_HcFmInterval; 	/* Bit time between 2 consecutive SOFs */
	AT91_REG	 UHP_HcFmRemaining; 	/* Bit time remaining in the current Frame */
	AT91_REG	 UHP_HcFmNumber; 	/* Frame number */
	AT91_REG	 UHP_HcPeriodicStart; 	/* Periodic Start */
	AT91_REG	 UHP_HcLSThreshold; 	/* LS Threshold */
	AT91_REG	 UHP_HcRhDescriptorA; 	/* Root Hub characteristics A */
	AT91_REG	 UHP_HcRhDescriptorB; 	/* Root Hub characteristics B */
	AT91_REG	 UHP_HcRhStatus; 	/* Root Hub Status register */
	AT91_REG	 UHP_HcRhPortStatus[2]; 	/* Root Hub Port Status Register */
} AT91S_UHP, *AT91PS_UHP;

#endif


