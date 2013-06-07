#include "gui.h"
#include "mmath.h"
#include "pf_timertick.h"
#include "pf_tsc.h"

#define MAX_BUTTON_CAPTION   30
#define MAX_LABEL_CAPTION    100

#define ID_SET        1
#define ID_BURN       2
#define ID_UFDISK     3
#define ID_IFDISK     4
#define ID_UFORMAT    5
#define ID_IFORMAT    6
#define ID_BURNPRO    7
#define ID_BURNFONT   8
#define ID_UPPRO      9
#define ID_DOWNPRO    10
#define ID_UPFONT     11
#define ID_DOWNFONT   12
#define ID_TC         13




typedef void BUTTON_CLICK_HANDLER(void *button,unsigned int stat);

typedef struct __button{
   unsigned short x,y,width,height;
   unsigned short pushed:1;
   unsigned short statChanged:1;
   unsigned short checkable:1;
   unsigned short haveFrame:1;
   unsigned short enable:1;
   unsigned short winId;
   unsigned int   group; 
   TEXTCHAR *caption; 
   BUTTON_CLICK_HANDLER *handler;  
}BUTTON;


typedef struct __label{
   unsigned short x,y,width,height;
   unsigned int haveFrame:1;
   unsigned int statChanged:1;
   TEXTCHAR *caption;
}LABEL;



BUTTON *buttonList[50];
unsigned int buttonindext = 0;
LABEL *labelList[20];
unsigned int labelindex = 0;

BUTTON *buttonGroup1[50];
BUTTON *buttonGroup2[50];


void registButton(BUTTON *button){
   buttonList[buttonindext++] = button;
}

void buttonRegistHandler(BUTTON *button,BUTTON_CLICK_HANDLER handler){
   button->handler = handler;
}


void registLabel(LABEL *label){
   labelList[labelindex++] = label;
}


const LABEL titleLabel = {
   .x = 0,.y = 0,.width=800,.height=20,.haveFrame=1,.caption = "mingde   bootloader",
};

const LABEL pageLabel = {
   .x = 0,.y = 20,.width=800,.height=560,.haveFrame=1,
};

const LABEL statLabel = {
  .x = 0,.y = 580,.width=60,.height=20,.haveFrame=1,.haveFrame=1,.caption = "status"
};

LABEL statLabel1 = {
   .x = 60,.y = 580,.width=740,.height=20,.haveFrame=1,
};
 

const LABEL frametitles[] = {
   [0]={.x = 240,.y = 35,.width=220,.height=20,.haveFrame=1,.haveFrame=1,.caption = "idisk"},
   [1]={.x = 460,.y = 35,.width=220,.height=20,.haveFrame=1,.haveFrame=1,.caption = "udisk"},  
};

const LABEL frames[] = {
   [0]={.x = 240,.y = 55,.width=220,.height=480,.haveFrame=1,},
   [1]={.x = 460,.y = 55,.width=220,.height=480,.haveFrame=1,},
};

BUTTON burnpagebuttons[] = {
   [0]={.x = 690,.y = 150,.width=100,.height=40,.winId=ID_UFDISK,.caption = "ts calibrat",},
   [1]={.x = 690,.y = 250,.width=100,.height=40,.winId=ID_UFORMAT,.caption = "idisk fdisk",},
   [2]={.x = 690,.y = 300,.width=100,.height=40,.winId=ID_IFORMAT,.caption = "idisk format",},
   [3]={.x = 690,.y = 350,.width=100,.height=40,.winId=ID_BURNPRO,.caption = "burn program",},
   [4]={.x = 690,.y = 400,.width=100,.height=40,.winId=ID_BURNFONT,.caption = "burn font",},
   [5]={.x = 250,.y = 540,.width=80,.height=35,.winId=ID_UPPRO,.caption = "up",},
   [6]={.x = 360,.y = 540,.width=80,.height=35,.winId=ID_DOWNPRO,.caption = "down",},
   [7]={.x = 480,.y = 540,.width=80,.height=35,.winId=ID_UPFONT,.caption = "up",},
   [8]={.x = 590,.y = 540,.width=80,.height=35,.winId=ID_DOWNFONT,.caption = "down",},
};

BUTTON inandButtons[] = {
   [0]={.x = 240,.y = 55,.width=220,.height=40,.winId=ID_UFDISK,.checkable=1},
   [1]={.x = 240,.y = 95,.width=220,.height=40,.winId=ID_UFORMAT,.checkable=1},
   [2]={.x = 240,.y = 135,.width=220,.height=40,.winId=ID_IFORMAT,.checkable=1},
   [3]={.x = 240,.y = 175,.width=220,.height=40,.winId=ID_BURNPRO,.checkable=1},
   [4]={.x = 240,.y = 215,.width=220,.height=40,.winId=ID_BURNFONT,.checkable=1},
   [5]={.x = 240,.y = 255,.width=220,.height=40,.winId=ID_UPPRO,.checkable=1},
   [6]={.x = 240,.y = 295,.width=220,.height=40,.winId=ID_DOWNPRO,.checkable=1},
   [7]={.x = 240,.y = 335,.width=220,.height=40,.winId=ID_UPFONT,.checkable=1},
   [8]={.x = 240,.y = 375,.width=220,.height=40,.winId=ID_DOWNFONT,.checkable=1},
   [9]={.x = 240,.y = 415,.width=220,.height=40,.winId=ID_DOWNFONT,.checkable=1},
   [10]={.x = 240,.y = 455,.width=220,.height=40,.winId=ID_DOWNFONT,.checkable=1},
   [11]={.x = 240,.y = 495,.width=220,.height=40,.winId=ID_DOWNFONT,.checkable=1},
};
BUTTON udiskButtons[] = {
   [0]={.x = 460,.y = 55,.width=220,.height=40,.winId=ID_UFDISK,.checkable=1},
   [1]={.x = 460,.y = 95,.width=220,.height=40,.winId=ID_UFORMAT,.checkable=1},
   [2]={.x = 460,.y = 135,.width=220,.height=40,.winId=ID_IFORMAT,.checkable=1},
   [3]={.x = 460,.y = 175,.width=220,.height=40,.winId=ID_BURNPRO,.checkable=1},
   [4]={.x = 460,.y = 215,.width=220,.height=40,.winId=ID_BURNFONT,.checkable=1},
   [5]={.x = 460,.y = 255,.width=220,.height=40,.winId=ID_UPPRO,.checkable=1},
   [6]={.x = 460,.y = 295,.width=220,.height=40,.winId=ID_DOWNPRO,.checkable=1},
   [7]={.x = 460,.y = 335,.width=220,.height=40,.winId=ID_UPFONT,.checkable=1},
   [8]={.x = 460,.y = 375,.width=220,.height=40,.winId=ID_DOWNFONT,.checkable=1},
   [9]={.x = 460,.y = 415,.width=220,.height=40,.winId=ID_DOWNFONT,.checkable=1},
   [10]={.x = 460,.y = 455,.width=220,.height=40,.winId=ID_DOWNFONT,.checkable=1},
   [11]={.x = 460,.y = 495,.width=220,.height=40,.winId=ID_DOWNFONT,.checkable=1},
};

const BUTTON setbuttons[] = {
   [0]={.x = 100,.y = 600,.width=50,.height=20,.caption = "touchpad calibrat",},
};


void buttonRedraw(const BUTTON *button, unsigned int force) {
   unsigned int forcetemp = force || button->statChanged;
   if (forcetemp) {
      if (!button->pushed) {
         if (button->haveFrame == 1) {
            Dis_HLine(button->x, button->y, button->width, C_White);
            Dis_HLine(button->x, button->y + button->height - 1, button->width, C_White);
            Dis_VLine(button->x, button->y, button->height, C_White);
            Dis_VLine(button->x + button->width - 1, button->y, button->height, C_White);
         }
         Dis_RectFill(button->x + 1, button->y + 1, button->width - 2, button->height - 2, C_Blue);
         if (button->caption != NULL) {
            Dis_StringAligen1(button->caption, ALIGEN_MIDDLE, button->x, button->y, button->width, button->height, 0, C_White, C_TRANSPARENT);
         }
      } else {
         if (button->haveFrame == 1) {
            Dis_HLine(button->x, button->y, button->width, C_Blue);
            Dis_HLine(button->x, button->y + button->height - 1, button->width, C_Blue);
            Dis_VLine(button->x, button->y, button->height, C_Blue);
            Dis_VLine(button->x + button->width - 1, button->y, button->height, C_Blue);
         }
         Dis_RectFill(button->x + 1, button->y + 1, button->width - 2, button->height - 2, C_White);
         if (button->caption != NULL) {
            Dis_StringAligen1(button->caption, ALIGEN_MIDDLE, button->x, button->y, button->width, button->height, 0, C_Blue, C_TRANSPARENT);
         }
      }
   }
}

void buttonShow(const BUTTON *button){
   buttonRedraw(button,1);
};

void buttonSetCaption(BUTTON *button,TEXTCHAR *caption){
   button->caption = caption;
   button->statChanged = 1;
}

void labelSetCaption(LABEL *label,TEXTCHAR *caption){
   label->caption = caption;
   label->statChanged = 1;
}

void labelRedraw(const LABEL *label, unsigned int force) {
   unsigned int forcetemp = force || label->statChanged;
   if (forcetemp) {
      Dis_RectFill(label->x, label->y, label->width, label->height, C_Blue);
      if (label->haveFrame) {
         Dis_HLine(label->x, label->y, label->width, C_White);
         Dis_HLine(label->x, label->y + label->height - 1, label->width, C_White);
         Dis_VLine(label->x, label->y, label->height, C_White);
         Dis_VLine(label->x + label->width - 1, label->y, label->height, C_White);
      }
      if (label->caption != NULL) {
         Dis_StringAligen1(label->caption, ALIGEN_MIDDLE, label->x, label->y, label->width, label->height, 0, C_White, C_TRANSPARENT);
      }
   }
}

void guiRedraw(void){
   for (int i=0;(i<NARRAY(labelList))&&(labelList[i]!=NULL);i++) {
      labelRedraw(labelList[i],0);
      labelList[i]->statChanged = 0;
   }
   for (int i=0;(i<50)&&(buttonList[i]!=NULL);i++) {
      buttonRedraw(buttonList[i],0);
      buttonList[i]->statChanged = 0;
   }
}

static unsigned int touch2buttonindex() {
   for (int i = 0; (i < NARRAY(buttonList)) && (buttonList[i] != NULL); i++) {
      if ((ts.x > buttonList[i]->x) && (ts.x < buttonList[i]->x + buttonList[i]->width) &&
          (ts.y > buttonList[i]->y) && (ts.y < buttonList[i]->y + buttonList[i]->height)) {
         return i;
      }
   }
   return -1;
}

unsigned short guiExec(void) {
#define STAT_NO         0
#define STAT_PUSHDOWN   1
#define STAT_FROZE      2
   static int stat = STAT_NO;
   static int timemark;
   static unsigned int button;
   unsigned int buttonnow;
   switch (stat) {
   case STAT_NO:
      if (atomicTestClear(&touched)) {
         buttonnow = touch2buttonindex();
         if (buttonnow != -1) {
            stat = STAT_PUSHDOWN;
            timemark = TimerTickGet();
            if (0 == buttonList[buttonnow]->checkable) {              
               buttonList[buttonnow]->pushed = 1;
               buttonList[buttonnow]->statChanged = 1;
               button = buttonnow;
               guiRedraw();
            } else {
               buttonList[buttonnow]->pushed = ! buttonList[buttonnow]->pushed;
               buttonList[buttonnow]->statChanged = 1;
               guiRedraw();
               button = buttonnow;
            }
            if (buttonList[buttonnow]->handler != NULL) buttonList[buttonnow]->handler(buttonList[buttonnow],buttonList[buttonnow]->pushed);
            return buttonList[buttonnow]->winId;
         }
      }
      break;
   case STAT_PUSHDOWN:
      if ((!atomicTestClear(&touched)) && (TimerTickGet() - timemark > 150)) {
         if (0 == buttonList[button]->checkable) {
            buttonList[button]->pushed = 0;
            buttonList[button]->statChanged = 1;
            guiRedraw();
         }
         stat = STAT_FROZE;
         timemark = TimerTickGet();
      }
      break;  
case STAT_FROZE:
      if (TimerTickGet() - timemark > 50) {
         stat = STAT_NO; 
      }
      break;
   default:
      break;
   }
   return 0;
}


void labelShow(const LABEL *label) {
   labelRedraw(label,1);
};


TEXTCHAR buff[100];

static void tsbuttonhandler(void * button,unsigned int stat){
   static unsigned int xxxx = 0;
   sprintf(buff,"xxxx: %d",xxxx++);
   labelSetCaption(&statLabel1,buff);
}


unsigned int ifileindex=-1,ifilegroupindex=0;



static void idiskbuttonhandler(void *button, unsigned int stat) {
   BUTTON *b = (BUTTON *)button;
   BUTTON **grop;
   static TEXTCHAR buff[100];
   if (stat == 1) {
      grop = (BUTTON **)(b->group);
      for (int i = 0;; i++) {
         if (grop[i] == NULL) {
            break;
         }
         if (grop[i] != b) {
            grop[i]->pushed = 0;
            grop[i]->statChanged = 1;
            ifileindex = i;
         }
      }
      guiRedraw();
   }else{
     ifileindex = -1;
   }
   sprintf(buff,"ifile   %d",ifileindex);
   labelSetCaption(&statLabel1,buff);
}


unsigned int ufileindex=0xfffffff,ufilegroupindex=0;

static void udiskbuttonhandler(void *button, unsigned int stat) {
   BUTTON *b = (BUTTON *)button;
   BUTTON **grop;
   static TEXTCHAR buff[100];
   if (stat == 1) {
      grop = (BUTTON **)(b->group);
      for (int i = 0;; i++) {
         if (grop[i] == NULL) {
            break;
         }
         if (grop[i] != b) {
            grop[i]->pushed = 0;
            grop[i]->statChanged = 1;
            ufileindex = i;
         }
      }
      guiRedraw();
   }else{
      ufileindex = -1;
   }
   sprintf(buff,"ufile   %d",ifileindex);
   labelSetCaption(&statLabel1,buff);
   
}



void hmishow(){
   buttonRegistHandler(burnpagebuttons,tsbuttonhandler);
   labelShow(&titleLabel);
   Dis_HLine(0,500,800,C_White);
   labelShow(&pageLabel);
   labelShow(&statLabel);
   labelShow(&statLabel1);
   registLabel(&statLabel1);
   for (int i=0;i<NARRAY(frametitles);i++) {
      labelShow(frametitles+i);
   }   
   for (int i=0;i<NARRAY(frames);i++) {
      labelShow(frames+i);
   } 
   for (int i=0;i<NARRAY(burnpagebuttons);i++) {
      burnpagebuttons[i].haveFrame = 1;
      buttonShow(burnpagebuttons+i);
      registButton(burnpagebuttons+i);
   }
   for (int i=0;i<NARRAY(inandButtons);i++) {
      buttonShow(inandButtons+i);
      buttonRegistHandler(inandButtons+i,idiskbuttonhandler);
      registButton(inandButtons+i);
      inandButtons[i].group = (unsigned int)buttonGroup1;
      buttonGroup1[i] = inandButtons+i;
   }
   for (int i=0;i<NARRAY(udiskButtons);i++) {
      buttonShow(udiskButtons+i);
      buttonRegistHandler(udiskButtons+i,udiskbuttonhandler);
      registButton(udiskButtons+i);
      udiskButtons[i].group = (unsigned int)buttonGroup2;
      buttonGroup2[i] = udiskButtons+i;
   }
}


