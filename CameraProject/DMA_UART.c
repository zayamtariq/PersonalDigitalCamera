#include "DMA_UART.h"

// The control table used by the uDMA controller.  This table must be aligned to a 1024 byte boundary.
// each channel has source,destination,control,pad (pad word is ignored)
uint32_t ucControlTable[256] __attribute__ ((aligned(1024)));

// UART4RX uses uDMA channel 18, encoding 2
#define CH18    (18*4)
#define BIT18 0x00040000  

void DMA_UART_Init() { 
	int i; 
	volatile uint32_t delay; 
	for(i=0; i<256; i++){
    ucControlTable[i] = 0;
  }
	// enable uDMA clock using RCGCDMA register 
	SYSCTL_RCGCDMA_R |= 0x01; 
	delay = SYSCTL_RCGCDMA_R; // allow time to finish 
	// enable uDMA controller, setting MASTEREN bit of DMACFG register
	UDMA_CFG_R |= 0x01; 
	// program location of channel control table w DMACTLBASE register
	UDMA_CTLBASE_R = (uint32_t)ucControlTable;
	UDMA_CHMAP2_R &= 0x00000200; // setting enc. 2 for channel 18 
	// configure channel attributes for this specific DMA 
	UDMA_PRIOCLR_R = BIT18;     // default, not high priority
  UDMA_ALTCLR_R = BIT18;      // use primary control
  UDMA_USEBURSTCLR_R = BIT18; // responds to both burst and single requests
  UDMA_REQMASKCLR_R = BIT18;  // allow the µDMA controller to recognize requests for this channel
	// Timer5A_Init(period);
}

// ************DMA_Transfer*****************
// Called to transfer bytes from source to destination
// The source address is fixed, destination address incremented each byte
// Inputs:  source is a pointer to the 32-bit I/O, least significant byte to read
//          destination is a pointer to a place to put the copy
//          count is the number of bytes to transfer (max is 1024 words)
// Outputs: none
// This routine does not wait for completion
void DMA_UART_Transfer(volatile uint32_t *source, uint8_t *destination, uint32_t count){ 
  ucControlTable[CH18]   = (uint32_t)source;                 // first and last address
  ucControlTable[CH18+1] = (uint32_t)destination+count-1;    // last address
  ucControlTable[CH18+2] = 0x0C000001+((count-1)<<4);             // DMA Channel Control Word (DMACHCTL)
/* DMACHCTL          Bits    Value Description
   DSTINC            31:30   0     8-bit destination address increment
   DSTSIZE           29:28   0     8-bit destination data size
   SRCINC            27:26   3     no source address increment
   SRCSIZE           25:24   0     8-bit source data size
   reserved          23:18   0     Reserved  
   ARBSIZE           17:14   0     Arbitrates after 1 transfer
   XFERSIZE          13:4  count-1 Transfer count items
   NXTUSEBURST       3       0      N/A for this transfer type
   XFERMODE          2:0     1      Use basic transfer mode
  */
  UDMA_ENASET_R = BIT18;  // µDMA Channel 18 is enabled.
  // bit 8 in UDMA_ENASET_R become clear when done
  // bits 2:0 ucControlTable[CH8+2] become clear when done
}

// ************DMA_Status*****************
// Can be used to check the status of a previous request
// Inputs:  none
// Outputs: true if still active, false if complete
// This routine does not wait for completion
uint32_t DMA_UART_Status(void){ 
  return (UDMA_ENASET_R&BIT18);  // µDMA Channel 18 enable bit is high if active
}

void DMA_UART_Enable() { 
	DMA_UART_Init();
	UART4_DMACTL_R |= 0x01; // enables to receive dma (i wonder if this should only be on when reading from image??) -> probs not necessary  
}

void DMA_UART_Disable() { 
	UART4_DMACTL_R &= 0xFFFFFFFFE; // disable last bit 
}
