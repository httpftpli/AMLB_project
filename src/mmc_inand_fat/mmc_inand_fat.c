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
#include "gui.h"
#include "pf_lcd.h"
#include "pf_usbmsc.h"
#include "pf_platform_cfg.h"
#include "font.h"
#include "../include/cpld.h"
#include "pf_tsc.h"
#include "event.h"
#include "pf_uart.h"
#include "uart_irda_cir.h"
#include "module.h"

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
mmcsdCtrlInfo mmcsdctr[2];

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
                    __attribute__ ((aligned (SOC_CACHELINE_SIZE)))= {0};
#else
#error "Unsupported Compiler. \r\n"
#endif


/******************************************************************************
**                          FUNCTION DEFINITIONS
*******************************************************************************/



volatile unsigned int ack = 0;

static void canrcvhandler(unsigned int index,CAN_FRAME *frame){
      ack = 1;
}


unsigned char bufmmc1[512*1],bufmmc2[512*1];

char buffer[1024];
unsigned int fontloaded = 0;

#define FONT_LOAD_ADDR        0x80000000+128*1024*1024


void udiskread(){
   static unsigned int error=0;
   int r;
   if((g_usbMscState == USBMSC_DEVICE_READY)&&(fontloaded == 0)&&(error == 0)){
     r = initFont(L"2:/",FONT_LOAD_ADDR);
     if (r!=0) {
        error = 1;
        return;
     }
     fontloaded = 1;   
   }
}


void drawText(){
   static unsigned int textdrawed = 0;
   if ((textdrawed == 0)&&(fontloaded == 0)) {
      textdrawed = 1;
      Dis_DrawText("杭州欢迎你",30,30,C_White,C_TRANSPARENT);
      Dis_DrawText("杭州はあなたを歓迎する",30,60,C_White,C_TRANSPARENT);
      Dis_DrawText("Welcome to Hangzhou",30,90,C_White,C_TRANSPARENT);
      Dis_DrawText("Hangzhou Hoşgeldiniz",30,120,C_White,C_TRANSPARENT);
      Dis_DrawText("1111111",30,150,C_White,C_TRANSPARENT);
   }
}



volatile unsigned short cpldbuffer[100];
extern void hmishow(void);

int main(void)
{
   volatile unsigned int initFlg = 1;
   unsigned int status;
   platformInit();
   moduleEnable(MODULE_ID_GPIO0);
   UARTStdioInit();
   DCANInit(MODULE_ID_DCAN0,CAN_MODE_NORMAL,1000000);
   LCDRasterStart();	//lcd 
   initMessageRing();
   TouchScreenInit();
   tsEnalbe();
   TouchCalibrate();
   hmishow();
   atomicClear(&touched);
   unsigned int timertick = TimerTickGet();
   int num = 0;
   while (1) {
      guiExec();
   }
  
   while(1);
  
      static const unsigned char calIcon[] = { 0x01, 0x80, 0x01, 0x80, 0x01, 0x80, 0x01, 0x80,
      0x01, 0x80, 0x01, 0x80, 0x01, 0x80, 0xFF, 0xFF,
      0xFF, 0xFF, 0x01, 0x80, 0x01, 0x80, 0x01, 0x80, 0x01,
      0x80, 0x01, 0x80, 0x01, 0x80, 0x01, 0x80 };
   Dis_Clear(C_Black);
   MSG msg;
   clearMessageRing();
   while (1) {
   while(!GetMessage(&msg));
   Dis_DrawMask(calIcon, msg.xpt - 8,  msg.ypt - 8, 16, 16, C_White, C_TRANSPARENT); 
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
   while(1){
     Dis_Clear(C_Red);
     delay(10000);
     Dis_Clear(C_Green);
     delay(10000);
     Dis_Clear(C_Blue);
     delay(10000);
     Dis_Clear(C_White);
     delay(10000);
   }
    while(1);

   while (1) {
      usbMscProcess();
      udiskread();
      drawText();
   }

   MMCSDP_CtrlInfoInit(& mmcsdctr[0], MODULE_ID_MMCSD0, 48000000, MMCSD_BUSWIDTH_4BIT, 0,
                       &card0, NULL,NULL, NULL); 
   MMCSDP_CtrlInit(& mmcsdctr[0]);
   MMCSDP_CardInit(mmcsdctr,MMCSD_CARD_AUTO);
  
   DWORD plist[] = {256*1024, -1UL, 0, 0};  /* Divide drive into two partitions */
   BYTE work[_MAX_SS];
   unsigned int beginLBA = 64*1024;
   f_fdiskEx(0,beginLBA, plist, work);  /* Divide physical drive 0 */
   FATFS fs1,fs2;
   unsigned char buf[512];
   f_mount(3, &fs1);
   f_mkfs(3,0,0);

     
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








/*void mainLoop(0){
      LCDSwapContex();  
      Dis_Clear(C_Red);
      LCDSwapFb(); 
      
      LCDSwapContex();
      Dis_Clear(C_Green);
      LCDSwapFb();  
}*/

