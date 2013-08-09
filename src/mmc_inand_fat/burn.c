#include "ff.h"
#include "ff_ext.h"
#include "type.h"
#include "pf_usbmsc.h"
#include "mmath.h"
#include "string.h"

typedef struct __fileinfo{
  char filename[13];
} MYFILEINFO;

FRESULT scan_files(TCHAR *path, MYFILEINFO *fileinfolist, unsigned int *nfiles)
{
 
#if _USE_LFN!=0
#error Cannot use LFN feature without valid code page.
#endif
   FRESULT res;
   FILINFO fno;
   DIR dir;
   res = f_opendir(&dir, path);                       /* Open the directory */
   if (res != FR_OK){
      *nfiles = 0;
      return res;
   }
   int i = 0;
   res = f_readdir(&dir, NULL); //rewind reading index of the directory object 
   for (; ; ){     
      res = f_readdir(&dir, &fno);
      if (fno.fattrib & AM_DIR)continue;
      if (res != FR_OK || fno.fname[0] == 0 || *nfiles==i){
         break;
      }
      memcpy(fileinfolist[i].filename,fno.fname,sizeof(fno.fname));
      i++;
   }
   *nfiles = i;
   return res;
}




BOOL idiskFdisk(){
   int r;
   DWORD plist[] = {256*1024, -1UL, 0, 0};  /* Divide drive into two partitions */
   BYTE work[_MAX_SS];
   unsigned int beginLBA = 64*1024;
   r =  f_fdiskEx(0,beginLBA, plist, work);  /* Divide physical drive 0 */ 
   if (FR_OK!=r) {
      return FALSE;
   }
   return TRUE;
}

BOOL idiskFormat(){
   FATFS fs1;
   int r; 
   unsigned char buf[512];
   f_mount(0, &fs1);
   r = f_mkfs(0,0,0);
   if (FR_OK!=r) {
      return FALSE;
   }
   return TRUE;
}
