#include "mmcsd_proto.h"
#include "platform.h"
#include "edma_event.h"
#include "soc_AM335x.h"
#include "interrupt.h"
#include "uartStdio.h"
#include "hs_mmcsd.h"
#include "string.h"
#include "delay.h"
#include "cache.h"
#include "edma.h"
#include "mmu.h"
#include "debug.h"
#include "ff.h"
#include "dcan.h"
#include "ff_ext.h"
#ifdef MMCSD_PERF
#include "perf.h"
#endif
#include "gpio_v2.h"
#include "utf8.h"
#include "lib_gui.h"
#include "pf_lcd.h"
#include "pf_usbmsc.h"
#include "pf_platform_cfg.h"
#include "../include/cpld.h"
#include "pf_tsc.h"
#include "pf_uart.h"
#include "uart_irda_cir.h"
#include "module.h"
#include "pf_key_touchpad.h"
#include "lib_gui.h"
#include "dmtimer.h"
#include "pf_dmtimer.h"
#include "algorithm.h"
/******************************************************************************
**                      INTERNAL MACRO DEFINITIONS
*******************************************************************************/


/* Global data pointers */
#define HSMMCSD_DATA_SIZE              512


#define SIZEOFARRYA                     2*1024

#define DCAN_IN_CLK                       (24000000u)

#define RX8025_ADDR                     0x32
#define EEPROM_ADDR                     0x50

/* SD card info structure */
mmcsdCardInfo card0, card1;

/* SD Controller info structure */
extern mmcsdCtrlInfo mmcsdctr[2];

FATFS fs[2];


/******************************************************************************
**                      VARIABLE DEFINITIONS
*******************************************************************************/

#ifdef __IAR_SYSTEMS_ICC__
#pragma data_alignment=SOC_CACHELINE_SIZE
unsigned char data[HSMMCSD_DATA_SIZE];

#elif defined(__TMS470__)
#pragma DATA_ALIGN(data, SOC_CACHELINE_SIZE);
unsigned char data[HSMMCSD_DATA_SIZE];

#elif defined(gcc)
unsigned char data[HSMMCSD_DATA_SIZE]
__attribute__((aligned(SOC_CACHELINE_SIZE))) = { 0 };
#else
#error "Unsupported Compiler. \r\n"
#endif


/******************************************************************************
**                          FUNCTION DEFINITIONS
*******************************************************************************/



volatile unsigned int ack = 0;

static void canrcvhandler(unsigned int index, CAN_FRAME *frame) {
   ack = 1;
}


unsigned char bufmmc1[512*1],bufmmc2[512*1];

char buffer[1024];
unsigned int fontloaded = 0;

#define FONT_LOAD_ADDR        0x80000000+64*1024*1024

void drawText() {
   GUI_SetBkColor(C_TRANSPARENT);
   GUI_SetColor(C_WHITE);
   GUI_SetFont(fonts[2]);
   drawString("杭州欢迎你", 30, 30);
   drawString("杭州はあなたを歓迎する", 30, 60);
   drawString("Welcome to Hangzhou", 30, 90);
   drawString("Hangzhou Hoşgeldiniz", 30, 120);
   drawString("1111111", 30, 150);
   GUI_SetFont(fonts[3]);
   drawString("杭州欢迎你", 30, 200);
   drawString("杭州はあなたを歓迎する", 30, 250);
   drawString("Welcome to Hangzhou", 30, 300);
   drawString("Hangzhou Hoşgeldiniz", 30, 350);
   drawString("1111111", 30, 400);
}


volatile unsigned short cpldbuffer[100];
extern void hmishow(void);
extern void probUdisk(void);



int main(void) {
   volatile unsigned int initFlg = 1;
   platformInit();
   static char  buf[512];
   static char  fmmirrorbuf[512];
   for(int i=0;i < sizeof buf; i++){
      buf[i] = i;
   }


////////////////////////////////////////////
//dmtimerInitForMatch(MODULE_ID_TIMER3,0xffffffff-0x7ffffff,(0xffffffff-0x7ffffff/2),DMTIMER_FLAG_INTENABLE_MATCH);

//////////////////////////////////////////////////////
/*   spiFmromInit(fmmirrorbuf, sizeof fmmirrorbuf);
   spiFmromRead(0, buf, 1);
   spiFmromWren();
   delay(500);
   spiFmromWrite(0, buf, sizeof buf); 
   delay(500);
   spiFmromRead(0, buf, sizeof buf);
   while (1);*/
   moduleEnable(MODULE_ID_GPIO0);
   LCDRasterStart();    //lcd
   LCDBackLightON();
   MMCSDP_CtrlInfoInit(&mmcsdctr[0], MODULE_ID_MMCSD0, 48000000, MMCSD_BUSWIDTH_4BIT, 0,
                       &card0, NULL, NULL, NULL);
   MMCSDP_CtrlInit(&mmcsdctr[0]);
   MMCSDP_CardInit(&mmcsdctr[0], MMCSD_CARD_AUTO);
   TouchCalibrate(0);
   static FATFS inandfs;
   f_mount(0, &inandfs);
   initFont(FONT_LOAD_ADDR);
   drawText();
}

//GPIODirModeSet(SOC_GPIO_0_REGS,30,GPIO_DIR_OUTPUT );
//while (1){
// GPIOPinWrite(SOC_GPIO_0_REGS,30,1);
// delay(10000);
// GPIOPinWrite(SOC_GPIO_0_REGS,30,0);
// delay(10000);
//}
// uartInit(MODULE_ID_UART1, 115200,8 ,UART_PARITY_NONE,UART_FRAME_NUM_STB_1
//          ,UART_INT_RHR_CTI,8,8);

/*
   while (1) {      
      udiskread();
      drawText();
   }

   MMCSDP_CtrlInfoInit(& mmcsdctr[0], MODULE_ID_MMCSD0, 48000000, MMCSD_BUSWIDTH_4BIT, 0,
                       &card0, NULL,NULL, NULL); 
   MMCSDP_CtrlInit(& mmcsdctr[0]);
   MMCSDP_CardInit(mmcsdctr,MMCSD_CARD_AUTO);
  
 
     
   while(1);
   
   
     
   LCDRasterStart();	//lcd 
   MMCSDP_CtrlInfoInit(& mmcsdctr[0], SOC_MMCHS_0_REGS, 24000000, MMCSD_BUSWIDTH_4BIT, 0,
                       &card0, NULL,NULL, NULL);  
  
   MMCSDP_CardInit(mmcsdctr,MMCSD_CARD_AUTO);
   

   for (int i=0;i<sizeof(bufmmc1)/4;i++) {
      bufmmc1[i]=0x55555555;
   }
   for (int i=0;i<sizeof(bufmmc2)/4;i++) {
      bufmmc2[i]=0;
   }
   
   
   MMCSDP_Write(mmcsdctr,bufmmc1,0,1024);
   MMCSDP_Read(mmcsdctr,bufmmc2,0,1024);
   for(int i=0;i<512*1024/4;i++){
     if(bufmmc2[i]!=bufmmc1[1])
        while(1);
   }
   
   while(1); 
     
}
*/

