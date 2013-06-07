/********************************************************************
功能:				U盘管理
修改时间:		2013-3-7
********************************************************************/
#include "Head.h"

//BMP图标文件名定义----------------
const char BmpTypes[][8] = {
   'B', 'M', 'P', '_', 'F', 'U', 'N', 'H',            //功能图标
   'B', 'M', 'P', '_', 'K', 'E', 'Y', ' ',                //触摸键输入窗体
   'B', 'M', 'P', '_', 'S', 'T', 'A', 'R',            //开机显示图标

   'I', 'C', 'O', 'N', 'U', 'S', 'B', ' ',                //U盘管理菜单图标
   'I', 'C', 'O', 'N', 'J', 'I', 'Q', 'I',
   'I', 'C', 'O', 'N', 'M', 'E', 'M', ' ',
   'I', 'C', 'O', 'N', 'A', 'D', 'D', ' ',
   'I', 'C', 'O', 'N', 'W', 'O', 'R', 'K',
   'I', 'C', 'O', 'N', 'F', 'I', 'L', 'E',
   'I', 'C', 'O', 'N', 'W', 'E', 'H', 'U',
   'I', 'C', 'O', 'N', 'G', 'Y', 'D', ' ',
};

const char *HZ_BmpFileErr[][LANG_MAX] = { "图片文件不符合要求!", "Bmp file check error!" };
const char *Hz_UsbMang[][LANG_MAX] = { "文件管理", "U-Disk" };
const char *Hz_UsbFile[][LANG_MAX] = { "U盘花型文件", "U-Disk file" };
const char *Hz_MemFile[][LANG_MAX] = { "内存花型文件", "Mem Files" };
const char *Hz_ItemInfo[][LANG_MAX] = {
   "总文件数:", "Files:",
   "动作行数:", "Cnt Rows:",
   "花型宽度:", "Width:",
   "花型高度:", "Height:",
   "修改时间:", "EditDate:",
};

const char *Hz_ItemCopy[][LANG_MAX] = {
   "F1:文件导入到内存", "F1:Load to Mem",
   "F2:文件导出到U盘", "F2:Save to U-Disk",
   "F3:从内存导入参数", "F3:Para from mem",
   "F4:从U盘导入参数", "F4:Para from UDisk",
   "F5:删除文件", "F5:Delete file",
   "F6:选择内存花样", "F6:Select file",
};
const char *Hz_ItemDel[][LANG_MAX] = {
   "是否要删除U盘中选定的花型?", "Delete file in the U-Disk?",
   "是否要删除内存中选定的花型?", "Delete file in the Memory?",
};
const char *Hz_FileSeach[][LANG_MAX] = { "正在搜索文件...", "Searching..." };
const char *Hz_FileProHint[][LANG_MAX] = { "按功能键F1~6选择相应操作", "Press Key F1~F6" };
const char *Hz_ItemCopyErr[][LANG_MAX] = {
   "文件长度超出范围!", "Length out rang!",
   "花型宽度超出范围!", "Width out rang!",
};
const char *HZ_HuaXingSend[][LANG_MAX] = {
   "花型载入完成!", "File load ok!", 
   "花型载入失败!", "Load false!",
};

const char *Hz_ParaCopySel[][LANG_MAX] = { "请选择源文件号: ", "Sel source num:" };
const char *Hz_WrkFileNone[][LANG_MAX] = { "工作参数文件不存在!", "No work para file!" };
const char *HZ_ItemWrkErr[][LANG_MAX] = {
   "工作参数读取错误!", "Work file read error!",
   "工作参数写入错误!", "Work file write error!",
};
const char *HZ_ItemFileErr[][LANG_MAX] = {
   "文件读取错误!", "Read file error!",
   "文件打开错误!", "Open file error!",
   "CNT文件校验错误!", "CNT check error!",
   "PAT文件校验错误!", "PAT check error!",
   "文件写入错误!", "File write error!",
};
const char *HZ_FileWriteErr[][LANG_MAX] = { "文件写入错误!", "Write error!" };
const char *HZ_SameFileErr[][LANG_MAX] = { "源文件与目标文件相同!", "Sourse file error!" };

const char *HZ_OperateEnd[][LANG_MAX] = { "操作结束!", "End of operation!" };

//===================================================================
//功能 : 将CNT和PAT的所有文件按名字进行配对排列
//*file_list: PAT文件目录项信息
//===================================================================
void File_SortSameName(uint8 *cnt_list, uint32 cnt_tol, uint8 *pat_list, uint32 pat_tol, uint8 *file_list) {
   uint32 cnt_count, pat_count, tol_count;
   uint32 i, j, m;
   uint32 big;
   uint32 totle;
   //将CNT和PAT文件按文件名配对排列----------
   totle = cnt_tol + pat_tol;
   if (cnt_tol >= pat_tol) big = cnt_tol;
   else big = pat_tol;
   tol_count = 0;
   cnt_count = 0;
   pat_count = 0;
   for (i = 0; i < big; i++) {
      //CNT文件多于PAT文件个数
      if (big == cnt_tol) {
         for (m = 0; m < 32; m++) {
            file_list[tol_count * 32 + m] = cnt_list[cnt_count * 32 + m];
         }
         tol_count++;
         //查找与.CNT文件名相同的.PAT文件
         for (j = 0; j < pat_tol; j++) {
            for (m = 0; m < 8; m++) {
               if (cnt_list[cnt_count * 32 + m] != pat_list[j * 32 + m]) break;
            }
            if (m >= 8) break;     //文件名相同
         }
         if (j < pat_tol) {
            for (m = 0; m < 32; m++) {
               file_list[tol_count * 32 + m] = pat_list[j * 32 + m];
            }
            pat_list[j * 32] = ' ';
            tol_count++;
            pat_count++;
         }
         cnt_count++;
      } else {
         for (m = 0; m < 32; m++) {
            file_list[tol_count * 32 + m] = pat_list[pat_count * 32 + m];
         }
         tol_count++;
         for (j = 0; j < cnt_tol; j++) {
            for (m = 0; m < 8; m++) {
               if (cnt_list[pat_count * 32 + m] != cnt_list[j * 32 + m]) break;
            }
            if (m >= 8) break;
         }
         if (j < cnt_tol) {
            for (m = 0; m < 32; m++) {
               file_list[tol_count * 32 + m] = cnt_list[j * 32 + m];
            }
            cnt_list[j * 32] = ' ';
            tol_count++;
            cnt_count++;
         }
         pat_count++;
      }
   }

   if (tol_count < totle) {
      cnt_count = 0;
      pat_count = 0;
      if (big == cnt_tol) {
         for (i = 0; i < pat_tol; i++) {
            if (pat_list[i * 32] != ' ') {
               for (m = 0; m < 32; m++) {
                  file_list[tol_count * 32 + m] = pat_list[i * 32 + m];
               }
               tol_count++;
               pat_count++;
            }
         }
      } else {
         for (i = 0; i < cnt_tol; i++) {
            if (cnt_list[i * 32] != ' ') {
               for (m = 0; m < 32; m++) {
                  file_list[tol_count * 32 + m] = cnt_list[i * 32 + m];
               }
               tol_count++;
               cnt_count++;
            }
         }
      }
   }
}

//===================================================================
//功能:			获取指定的文件是否存在
//参数:
//		fdt:			文件名信息
//		type:		文件类型,0=CNT,1=PAT,2=PRM,3=WRK,4=SPA
//		device:		设备号
//返回: 			0表示当前文件存在
//影响: 			fdt的第8~10个字节(扩展名)
//修改时间:		2013-3-5
//===================================================================
uint32 Get_FileExist(uint8 *fdt, uint32 type, uint32 device) {
   uint32 i;
   uint32 flag;
   uint8 ext_name[] = { 'C', 'N', 'T', 'P', 'A', 'T', 'P', 'R', 'M', 'W', 'R', 'K', 'S', 'P', 'A', 'S', 'A', 'Z', 'F', 'A', 'C' };
   CloseFile(device);
   i = type * 3;
   *(fdt + 8) = ext_name[i++];
   *(fdt + 9) = ext_name[i++];
   *(fdt + 10) = ext_name[i++];
   flag = OpenFileFloder(fdt, device);
   CloseFile(device);
   return flag;
}

//===================================================================
//功能:			将指定文件名的.WRK文件(4KB)写入到设备
//入口参数:
//		fdt:			指定的文件名
//		device:		指定的设备:  0=SD, 1=USB
//修改时间:		2013-4-10
//===================================================================
uint32 Write_WrkToFlash(uint8 *fdt, uint32 device) {
   uint32 flag;
   uint8 dat[4096];
   CloseFile(device);
   Fdt_InitDir(fdt);
   fdt[8] = 'W';
   fdt[9] = 'R';
   fdt[10] = 'K';
   flag = OpenFileFloder(fdt, device);
   if ((flag == 0) && (MyFileInfo[device].LengthInByte == 4096)) {
      flag = ReadFile(4096, dat, device);
      if (flag == 0) {
         Fm_WriteBytes(ADDR_ParaGongZuo, dat, 4096);
         Para_WriteBagIDAll();
         //若是复位成功状态则强制要求复位-----
         if (SysFlag.ResetOk > 0) {
            SysFlag.QiZenDianRst = 0x55;      //要求强制复位
         }
      } else {
         Dis_HintForm(HZ_TiSi[0][Lang], HZ_ItemWrkErr[0][Lang], C_Black, C_Back);
      }
   }
   CloseFile(device);
   return (flag);
}

//===================================================================
//功能: 将当前工作参数以WRK文件形式写入到SD卡
//===================================================================
uint32 Write_WrkToSD(void) {
   uint32 i;
   uint8 dat[4096];            // 2k字的工作参数
   uint8 file_fdt[32];
   uint32 flag;
//	BEEP_OFF;
   Fm_ReadBytes(ADDR_ParaGongZuo, dat, 4096);
   for (i = 0; i < 8; i++) file_fdt[i] = CntFileFdt[i];
   file_fdt[8] = 'W';
   file_fdt[9] = 'R';
   file_fdt[10] = 'K';
   flag = Get_FileExist(file_fdt, FILE_WRK, NAND);
   //工作文件已存在-----
   if (flag == 0) {
      flag = DeleteFileFloder(file_fdt, NAND);
   }

   flag = CreateFile(file_fdt, 4096, NAND);        // 4K字节
   if (flag == 0) {
      flag = WriteFile(4096, dat, NAND);
      if (flag > 0) {
         SetFilePoint(0, NAND);
         flag = WriteFile(4096, dat, NAND);
      }
   }
   if (flag > 0) {
      Dis_HintForm(HZ_TiSi[0][Lang], HZ_ItemWrkErr[1][Lang], C_Black, C_Back);
   }
   CloseFile(NAND);
   return (flag);
}

//===================================================================
//功能: 			将从U盘中导入的CNT文件按固定格式初始化
//入口参数:
//		dat:			CNT文件指针
//		len:			CNT文件长度
//		use_crc:		不为0表示使用了CRC校验方式
//说明:			在写每个文件前先判断内存中是否有相同文件存在
//               			若文件已存在,则先删除再写文件
//               			纱嘴纱入动作标志清0处理
//修改时间:		2010-11-26
//===================================================================
void CntFile_LoadMenInit(uint8 *dat, uint32 len, uint32 use_crc) {
   const char MingDe[6] = { 'M', 'i', 'n', 'g', 'D', 'e' };
   uint32 i;
   uint32 init;            //0表示已经被初始化过
   uint32 m;
   uint8 byte;
   uint8 sazui_tingfangzu = 1;
//uint32 crc;
   init = 0;
   for (i = 0; i < 6; i++) {
      if (*(dat + i) != MingDe[i]) {
         init = 1;
         break;
      }
   }
   if ((dat[0] == 'M') && (dat[1] == 'D')) {
      init = 0;
   }

   //若CNT文件未被初始化----------------
   if (init > 0) {
      //判断是否是HQ打版系统生成的文件------
      if (((dat[0] == 'H') && (dat[1] == 'Q')) || ((dat[0] == 'C') && (dat[1] == 'X'))) {
         //按两个系统度目处理-----------
         dat[0] = 'M';
         dat[1] = 'D';
      } else {
         for (i = 0; i < 6; i++) {
            *(dat + i) = MingDe[i];
         }
      }

      sazui_tingfangzu = 1;
      len = len / CNT_P;
      //每一页的初始化----
      for (i = 1; i <= len; i++) {
         m = i * CNT_P;
         //纱嘴停放组(若为0默认为上次的数值)----
         byte = *(dat + m + CNT_SaZuoTingFang);
         if (byte == 0) {
            *(dat + m + CNT_SaZuoTingFang) = sazui_tingfangzu;
         } else {
            sazui_tingfangzu = byte;
         }

         //系统1纱嘴标志强制为0x80----
         if ((*(dat + m + CNT_S1_SaZui + 1) > 0) && (*(dat + m + CNT_S1_SaZui) != 0x81)) {
            *(dat + m + CNT_S1_SaZui) = 0x80;
         }
         //系统2纱嘴标志强制为0x80----
         if ((*(dat + m + CNT_S2_SaZui + 1) > 0) && (*(dat + m + CNT_S2_SaZui) != 0x81)) {
            *(dat + m + CNT_S2_SaZui) = 0x80;
         }

//			//速度为0处理
//			byte=*(dat+m+CNT_SuDu);
//			if(byte==0) *(dat+m+CNT_SuDu)=sudu;
//			else sudu=byte;
//			//副罗拉为0处理
//			byte=*(dat+m+CNT_FuJuanBu);
//			if(byte==0) *(dat+m+CNT_FuJuanBu)=fuluola;
//			else fuluola=byte;
//停车设置默认为不停----
         *(dat + m + CNT_TingCe) = 0;
         //循环信息清0-----
         *(dat + m + CNT_LoopStart) = 0;
         *(dat + m + CNT_LoopStart + 1) = 0;
         *(dat + m + CNT_LoopNum) = 0;
         *(dat + m + CNT_LoopNum + 1) = 0;
         *(dat + m + CNT_SaRu) = 0;                //纱入标志清0
         *(dat + m + CNT_SaRuYarn) = 0;            //纱入的纱嘴清0
//			crc=Get_CRC16(dat+m,CNT_CheckNum);
//			*(dat+m+CNT_CRCL)=crc%256;
//			*(dat+m+CNT_CRCH)=crc/256;
      }
   }
}

//===================================================================
//功能:			将当前花型的所有文件从U盘输入内存(.CNT, .PAT, .PRM, .WRK)
//入口参数:
//		fdt:			文件目录项信息
//返回:			0表示正确,其它表示错误
//说明:			在写每个文件前先判断内存中是否有相同文件存在
//              			若文件已存在,则先删除再写文件
//修改时间:		2013-3-22
//===================================================================
uint32 FileAll_WriteMem(uint8 *fdt) {
   const char *Hz_OverWriteWrk[][LANG_MAX] = {
      "内存花型参数文件已存在,是否要覆盖?", "WRK file exist,over write it?",
   };
   uint32 i, j;
   uint32 addr;
   uint32 flag;
   uint8 file_fdt[32];
   uint32 tol_bytes;
   uint32 count;
   uint8 byte;
   uint32 file_error;              // 1表示读取错误,2表示写入错误
   uint8 dat[NAND_PageB];
   uint8 buf[4096];
   uint32 use_crc;             //不为0表示使用了校验方式
   uint32 wrk_usb;             //不为0表示U盘中不存在工作参数文件
   uint32 same_file;               //不为0表示拷入的花型与当前的花型相同
   uint8 *mem;
   same_file = 0;
   wrk_usb = 0;
   use_crc = 0;
   file_error = 0;

   mem = MemApplication(mem, MAX_FileSize);
   if (mem == 0) {
      return (1);
   }

   //若内存当前花型与复制花型相同则改变ID号----
   if (SysFlag.FileOk > 0) {
      for (i = 0; i < 8; i++) {
         if (*(fdt + i) != CntFileFdt[i]) break;
      }
      if (i >= 8) same_file = 1;
   }
   CloseFile(USB);
   CloseFile(NAND);
   while (1) {
      //读取U盘中的CNT文件--------------
      *(fdt + 8) = 'C';
      *(fdt + 9) = 'N';
      *(fdt + 10) = 'T';
      flag = Get_FileExist(fdt, FILE_CNT, NAND);
      if (flag == 0) {
         flag = DeleteFileFloder(fdt, NAND);
      }
      flag = OpenFileFloder(fdt, USB);
      for (i = 0; i < 32; i++) {
         file_fdt[i] = MyFileInfo[USB].FileNameDat[i];
      }
      tol_bytes = MyFileInfo[USB].LengthInByte;
      count = tol_bytes / NAND_PageB;
      if (tol_bytes % NAND_PageB) count++;
      for (i = 0; i < count; i++) {
         addr = i * NAND_PageB;
         flag = ReadFile(NAND_PageB, mem + addr, USB);
         if (flag > 0) i = count;                         //保证消除进度框
         Dis_SpeedInfo(HZ_ItemCntWR[1][Lang], i + 1, count, 220, C_Gray);
      }
      CloseFile(USB);

      if (flag > 0) {
         file_error = 1;
         break;
      }

/*
        //判断CNT文件是否使用了CRC校验方式----
        use_crc=GetCntCheck(mem);
        if(use_crc>0)
        {
            error_hang=CntCheckCRC(mem,tol_bytes);	//CRC校验处理
            if(error_hang>0)
            {
                file_error=3;
                break;
            }
        }
*/
      //将CNT文件初始化处理------------------
      CntFile_LoadMenInit(mem, tol_bytes, use_crc);

      //将CNT文件写入到内存(SD卡)中--------
      flag = CreateFile(file_fdt, tol_bytes, NAND);
      if (flag == 0) {
         for (i = 0; i < count; i++) {
            for (j = 0; j < NAND_PageB; j++) {
               dat[j] = mem[i * NAND_PageB + j];
            }
            flag = WriteFile(NAND_PageB, dat, NAND);
            if (flag > 0) {
               flag = SetFilePoint(i * NAND_PageB, NAND);
               flag = ModifyFileDat(NAND_PageB, dat, NAND);
            }
            if (flag > 0) i = count;                         //保证消除进度框
            Dis_SpeedInfo(HZ_ItemCntWR[0][Lang], i + 1, count, 220, C_Gray);
         }
      }
      CloseFile(NAND);
      if (flag > 0) {
         file_error = 5;
         break;
      }

      //读取U盘中的PAT文件----------------------------------
      *(fdt + 8) = 'P';
      *(fdt + 9) = 'A';
      *(fdt + 10) = 'T';
      flag = Get_FileExist(fdt, FILE_PAT, NAND);
      if (flag == 0) {
         flag = DeleteFileFloder(fdt, NAND);
      }
      flag = OpenFileFloder(fdt, USB);
      for (i = 0; i < 32; i++) {
         file_fdt[i] = MyFileInfo[USB].FileNameDat[i];
      }
      tol_bytes = MyFileInfo[USB].LengthInByte;
      count = tol_bytes / NAND_PageB;
      if (tol_bytes % NAND_PageB) count++;
      for (i = 0; i < count; i++) {
         addr = i * NAND_PageB;
         flag = ReadFile(NAND_PageB, mem + addr, USB);
         if (flag > 0) i = count;             //保证消除进度框
         Dis_SpeedInfo(HZ_ItemPatWR[1][Lang], i + 1, count, 220, C_Gray);
      }
      CloseFile(USB);
      if (flag > 0) {
         file_error = 1;
         break;
      }
/*
        //判断PAT文件是否使用了CRC校验方式----
        use_crc=GetPatCheck(mem);
        if(use_crc>0)
        {
            error_hang=PatCheckCRC(mem,tol_bytes);	//CRC校验处理
            if(error_hang>0)
            {
                file_error=4;
                break;
            }
        }		
*/
      //将PAT文件写入到内存(SD卡)中-----------------------
      flag = CreateFile(file_fdt, tol_bytes, NAND);
      if (flag == 0) {
         for (i = 0; i < count; i++) {
            for (j = 0; j < NAND_PageB; j++) {
               dat[j] = mem[i * NAND_PageB + j];
            }
            flag = WriteFile(NAND_PageB, dat, NAND);
            if (flag > 0) {
               flag = SetFilePoint(i * NAND_PageB, NAND);
               flag = ModifyFileDat(NAND_PageB, dat, NAND);
            }
            if (flag > 0) i = count;                         //保证消除进度框
            Dis_SpeedInfo(HZ_ItemPatWR[0][Lang], i + 1, count, 220, C_Gray);
         }
      }
      CloseFile(NAND);
      if (flag > 0) {
         file_error = 5;
         break;
      }

      //====================================================================
      //读取SD卡中的PRM文件(固定为512B)---------------------------------
      flag = Get_FileExist(fdt, FILE_PRM, NAND);
      if (flag == 0) {
         flag = DeleteFileFloder(fdt, NAND);
      }
      flag = OpenFileFloder(fdt, USB);
      //若PRM 文件存在----------------
      if (flag == 0) {
         for (i = 0; i < 32; i++) {
            file_fdt[i] = MyFileInfo[USB].FileNameDat[i];
         }
         tol_bytes = MyFileInfo[USB].LengthInByte;
         if (tol_bytes == 512) {
            flag = ReadFile(512, dat, USB);
            MyFileInfo[USB].FileOpen = 0;
            //将PRM文件写入到内存(SD卡)中--------
            flag = CreateFile(file_fdt, 512, NAND);
            if (flag == 0) {
               flag = WriteFile(512, dat, NAND);
            }
         }
      }
      CloseFile(USB);
      CloseFile(NAND);

      //====================================================================
      //读取SD卡中的SAZ文件(固定为512B)---------------------------------
      flag = Get_FileExist(fdt, FILE_SAZ, NAND);
      if (flag == 0) {
         flag = DeleteFileFloder(fdt, NAND);
      }
      flag = OpenFileFloder(fdt, USB);
      //若U盘中的SAZ文件存在----------------
      if (flag == 0) {
         for (i = 0; i < 32; i++) {
            file_fdt[i] = MyFileInfo[USB].FileNameDat[i];
         }
         tol_bytes = MyFileInfo[USB].LengthInByte;
         if (tol_bytes == 512) {
            flag = ReadFile(512, dat, USB);
            if (flag > 0) {
               flag = SetFilePoint(0, USB);
               flag = ReadFile(512, dat, USB);
            }
            MyFileInfo[USB].FileOpen = 0;
            //将SAZ文件写入到内存(SD卡)中--------
            flag = CreateFile(file_fdt, 512, NAND);
            if (flag == 0) {
               flag = WriteFile(512, dat, NAND);
            }
         }
      }
      CloseFile(USB);
      CloseFile(NAND);

      //读取U盘中的WRK文件---------------------------------
      *(fdt + 8) = 'W';
      *(fdt + 9) = 'R';
      *(fdt + 10) = 'K';
      flag = OpenFileFloder(fdt, USB);
      //若U盘中的WRK 文件存在----
      if (flag == 0) {
         flag = Get_FileExist(fdt, FILE_WRK, NAND);
         if (flag == 0) {
            flag = Dis_SureDialog(HZ_TiSi[0][Lang], Hz_OverWriteWrk[0][Lang], C_Back);
            if (flag == DO_Cancel) flag = 0x55aa;
            if (flag != 0x55aa) {
               flag = DeleteFileFloder(fdt, NAND);
            }
         }
         if (flag != 0x55aa) {
            for (i = 0; i < 32; i++) {
               file_fdt[i] = MyFileInfo[USB].FileNameDat[i];
            }
            tol_bytes = MyFileInfo[USB].LengthInByte;
            if (tol_bytes == 4096) {
               flag = ReadFile(4096, buf, USB);
               MyFileInfo[USB].FileOpen = 0;
               //若工作文件读取错误处理-------------
               if (flag > 0) {
                  Dis_HintForm(HZ_TiSi[0][Lang], HZ_ItemWrkErr[0][Lang], C_Black, C_Back);
               } else {
                  wrk_usb = 1;
                  //将WRK文件写入到内存(SD卡)中--------
                  flag = CreateFile(file_fdt, tol_bytes, NAND);
                  if (flag == 0) {
                     flag = WriteFile(4096, buf, NAND);
                  }
                  if (flag > 0) {
                     Dis_HintForm(HZ_TiSi[0][Lang], HZ_ItemWrkErr[1][Lang], C_Black, C_Back);
                  }
               }
            }
         }
      }
      break;
   }

   CloseFile(USB);
   CloseFile(NAND);
   //若内存当前花型与复制花型相同则改变ID号----
   if (same_file > 0) {
      if (wrk_usb == 0) {
         flag = Write_WrkToSD();
      }
      SysFlag.FileOk = 0;
      Sys.ErrorCode = ERR_MemNoFile;
      byte = Fm_Read1Byte(ADDR_FM_FileID);
      Fm_Write1Byte(ADDR_FM_FileID, byte + 1);
      Fm_Write1Byte(ADDR_FM_FileOK, 0);
      //清复位成功标志------
      SysFlag.ResetOk = 0;
      Fm_Write1Byte(ADDR_FM_RunReset, 0);
      Sys.RefRunItem = 1;   //刷新运行界面底部功能图标及其它
   }

   if (file_error > 0) {
      Dis_HintForm(HZ_CuoWu[0][Lang], HZ_ItemFileErr[file_error - 1][Lang], C_Black, C_Back);
   }

   free(mem);
   return (flag);
}

//===================================================================
//功能:			将当前花型的所有文件输出到U盘(.CNT, .PAT, .PRM, .WRK)
//返回: 			0表示正确,其它表示错误
//说明: 			在写每个文件前先判断内存中是否有相同文件存在
//             			若文件已存在,则先删除再写文件
//					文件写入到U盘的根目录中
//修改时间:		2011-5-7
//===================================================================
uint32 FileAll_WriteUsb(uint8 *fdt) {
   uint32 i, j;
   uint32 m;
   uint32 flag;
   uint32 addr;
   uint8 file_fdt[32];
   uint32 tol_bytes;
   uint32 count;
   uint32 file_error;              // 1表示读取错误,2表示写入错误
   uint8 *mem;
   uint8 dat[BASE_Byte];
   uint8 buf[4096];
   mem = MemApplication(mem, MAX_FileSize);
   if (mem == 0) {
      return (1);
   }

   file_error = 0;
   while (1) {
      CloseFile(USB);
      CloseFile(NAND);
      //读取SD卡中的CNT文件--------------
      *(fdt + 8) = 'C';
      *(fdt + 9) = 'N';
      *(fdt + 10) = 'T';
      Fdt_InitDir(fdt);
      flag = Get_FileExist(fdt, FILE_CNT, USB);
      if (flag == 0) {
         flag = DeleteFileFloder(fdt, USB);
      }
      flag = OpenFileFloder(fdt, NAND);
      for (i = 0; i < 32; i++) {
         file_fdt[i] = MyFileInfo[NAND].FileNameDat[i];
      }
      tol_bytes = MyFileInfo[NAND].LengthInByte;
      count = tol_bytes / NAND_PageB;
      if (tol_bytes % NAND_PageB) count++;
      for (i = 0; i < count; i++) {
         addr = i * NAND_PageB;
         flag = ReadFile(NAND_PageB, mem + addr, NAND);
         if (flag > 0) i = count;
         Dis_SpeedInfo(HZ_ItemCntWR[1][Lang], i + 1, count, 220, C_Gray);
         if (flag > 0) {
            file_error = 1;
            break;
         }
      }
      if (file_error > 0) {
         break;
      }

      CloseFile(USB);
      CloseFile(NAND);
      //将CNT文件写入到U盘中------------------------
      flag = CreateFile(file_fdt, tol_bytes, USB);
      if (flag == 0) {
         for (i = 0; i < count; i++) {
            for (j = 0; j < NAND_PageB; j++) {
               dat[j] = mem[i * NAND_PageB + j];
            }
            flag = WriteFile(NAND_PageB, dat, USB);
            if (flag > 0) {
               flag = SetFilePoint(i * NAND_PageB, USB);
               flag = ModifyFileDat(NAND_PageB, dat, USB);
            }
            if (flag > 0) m = count;
            else m = i;
            Dis_SpeedInfo(HZ_ItemCntWR[0][Lang], m + 1, count, 220, C_Gray);
            if (flag > 0) {
               file_error = 2;
               break;
            }
         }
      }
      if (file_error > 0) {
         break;
      }

      CloseFile(USB);
      CloseFile(NAND);
      //读取SD卡中的PAT文件----------------------
      *(fdt + 8) = 'P';
      *(fdt + 9) = 'A';
      *(fdt + 10) = 'T';
      flag = Get_FileExist(fdt, FILE_PAT, USB);
      if (flag == 0) {
         flag = DeleteFileFloder(fdt, USB);
      }
      flag = OpenFileFloder(fdt, NAND);
      for (i = 0; i < 32; i++) {
         file_fdt[i] = MyFileInfo[NAND].FileNameDat[i];
      }
      tol_bytes = MyFileInfo[NAND].LengthInByte;
      count = tol_bytes / NAND_PageB;
      if (tol_bytes % NAND_PageB) count++;
      for (i = 0; i < count; i++) {
         addr = i * NAND_PageB;
         flag = ReadFile(NAND_PageB, mem + addr, NAND);
         if (flag > 0) i = count;
         Dis_SpeedInfo(HZ_ItemPatWR[1][Lang], i + 1, count, 220, C_Gray);
         if (flag > 0) {
            file_error = 1;
            break;
         }
      }
      if (file_error > 0) {
         break;
      }

      CloseFile(USB);
      CloseFile(NAND);
      //将PAT文件写入到U盘中--------
      flag = CreateFile(file_fdt, tol_bytes, USB);
      if (flag == 0) {
         for (i = 0; i < count; i++) {
            for (j = 0; j < NAND_PageB; j++) {
               dat[j] = mem[i * NAND_PageB + j];
            }
            flag = WriteFile(NAND_PageB, dat, USB);
            if (flag > 0) {
               flag = SetFilePoint(i * NAND_PageB, USB);
               flag = ModifyFileDat(NAND_PageB, dat, USB);
            }
            if (flag > 0) m = count;
            else m = i;
            Dis_SpeedInfo(HZ_ItemPatWR[0][Lang], m + 1, count, 220, C_Gray);
            if (flag > 0) {
               file_error = 2;
               break;
            }
         }
      }
      if (file_error > 0) {
         break;
      }

      CloseFile(USB);
      CloseFile(NAND);
      //============================================================
      //读取SD卡中的PRM文件---------------------------------
      *(fdt + 8) = 'P';
      *(fdt + 9) = 'R';
      *(fdt + 10) = 'M';
      flag = Get_FileExist(fdt, FILE_PRM, USB);
      if (flag == 0) {
         flag = DeleteFileFloder(fdt, USB);
      }
      flag = OpenFileFloder(fdt, NAND);
      //若PRM 文件存在----
      if (flag == 0) {
         for (i = 0; i < 32; i++) {
            file_fdt[i] = MyFileInfo[NAND].FileNameDat[i];
         }
         tol_bytes = MyFileInfo[NAND].LengthInByte;
         if (tol_bytes == 512) {
            flag = ReadFile(512, dat, NAND);
            if (flag > 0) {
               flag = SetFilePoint(0, NAND);
               flag = ReadFile(512, dat, NAND);
            }
            //将PRM文件写入到内存(SD卡)中--------
            flag = CreateFile(file_fdt, 512, USB);
            if (flag == 0) {
               flag = WriteFile(512, dat, USB);
            }
         }
      }

      CloseFile(USB);
      CloseFile(NAND);
      //============================================================
      //判断U盘中的SAZ文件是否已存在---------------------
      flag = Get_FileExist(fdt, FILE_SAZ, USB);
      if (flag == 0) {
         flag = DeleteFileFloder(fdt, USB);
      }
      flag = OpenFileFloder(fdt, NAND);
      //若SAZ文件存在----
      if (flag == 0) {
         for (i = 0; i < 32; i++) {
            file_fdt[i] = MyFileInfo[NAND].FileNameDat[i];
         }
         tol_bytes = MyFileInfo[NAND].LengthInByte;
         if (tol_bytes == 512) {
            flag = ReadFile(512, dat, NAND);
            if (flag > 0) {
               flag = SetFilePoint(0, NAND);
               flag = ReadFile(512, dat, NAND);
            }
            //将SAZ文件写入到内存(SD卡)中--------
            flag = CreateFile(file_fdt, 512, USB);
            if (flag == 0) {
               flag = WriteFile(512, dat, USB);
            }
         }
      }

      CloseFile(USB);
      CloseFile(NAND);
      //读取SD卡中的WRK文件---------------------------------
      *(fdt + 8) = 'W';
      *(fdt + 9) = 'R';
      *(fdt + 10) = 'K';
      flag = OpenFileFloder(fdt, NAND);
      //若SD卡中的WRK 文件存在----
      if (flag == 0) {
         flag = Get_FileExist(fdt, FILE_WRK, USB);
         if (flag == 0) {
            flag = DeleteFileFloder(fdt, USB);
         }
         for (i = 0; i < 32; i++) {
            file_fdt[i] = MyFileInfo[NAND].FileNameDat[i];
         }
         tol_bytes = MyFileInfo[NAND].LengthInByte;
         if (tol_bytes == 4096) {
            flag = ReadFile(4096, buf, NAND);
            MyFileInfo[NAND].FileOpen = 0;
            //工作参数文件读取错误处理----------
            if (flag > 0) {
               Dis_HintForm(HZ_TiSi[0][Lang], HZ_ItemWrkErr[0][Lang], C_Black, C_Back);
            } else {
               //将WRK文件写入到内存(SD卡)中--------
               flag = CreateFile(file_fdt, 4096, USB);
               if (flag == 0) {
                  flag = WriteFile(4096, buf, USB);
               }
               if (flag > 0) {
                  Dis_HintForm(HZ_TiSi[0][Lang], HZ_ItemWrkErr[1][Lang], C_Black, C_Back);
               }
            }
         }
      }
      break;
   }

   MyFileInfo[NAND].FileOpen = 0;
   MyFileInfo[USB].FileOpen = 0;
   if (file_error == 1) {
      Dis_HintForm(HZ_CuoWu[0][Lang], HZ_FileOptHint[FILE_ReadErr][Lang], C_Black, C_Back);
   } else if (file_error == 2) {
      Dis_HintForm(HZ_CuoWu[0][Lang], HZ_FileWriteErr[0][Lang], C_Black, C_Back);
   }
   free(mem);
   return (flag);
}


//===================================================================
//功能:			得到内存中所有有效花型文件的目录项列表信息
//入口参数:
//		file_list:		取得所有的有效花型文件的目录项信息
//		device: 		设备类型
//返回 : 			得到的文件个数
//修改时间:		2011-5-7
//===================================================================
uint32 Get_FullFiles(uint8 *file_list, uint32 device) {
   uint32 i, j, m, n;
   uint8 file_fdt[32];
   uint32 file_tol;
   uint32 cnt_tol;
   uint32 pat_tol;
   uint8 cnt_list[MAX_Files][32];
   uint8 pat_list[MAX_Files][32];
   file_fdt[0] = 'C';
   file_fdt[1] = 'N';
   file_fdt[2] = 'T';
   if (device == NAND) m = MAX_MemFiles;
   else m = MAX_Files;
   cnt_tol = ListFileNameFloder(file_fdt, m, cnt_list[0], device);
   file_fdt[0] = 'P';
   file_fdt[1] = 'A';
   file_fdt[2] = 'T';
   pat_tol = ListFileNameFloder(file_fdt, m, pat_list[0], device);

   //得到完整花型文件FDT列表-----------------
   file_tol = 0;
   for (i = 0; i < pat_tol; i++) {
      for (j = 0; j < cnt_tol; j++) {
         for (m = 0; m < (8 + 1); m++) {
            if (pat_list[i][m] != cnt_list[j][m]) break;
            *(file_list + file_tol * 32 + m) = pat_list[i][m];
            //文件名相同处理
            if (m == 7) {
               //取得目录项的其它信息-----
               for (n = 8; n < 32; n++) {
                  *(file_list + file_tol * 32 + n) = pat_list[i][n];
               }
               file_tol++;
               j = 10000;
               break;
            }
         }
      }
   }
   return (file_tol);
}

//===================================================================
//功能: 删除内存指定花型处理
//*fdt     : 指定的花型名称
//===================================================================
uint32 File_DeleteHuaXin(uint8 *fdt, uint32 device) {
   uint32 i;
   uint32 flag;
   uint8 buf[8];
   flag = Get_FileExist(fdt, FILE_CNT, device);
   if (flag == 0) DeleteFileFloder(fdt, device);
   flag = Get_FileExist(fdt, FILE_PAT, device);
   if (flag == 0) DeleteFileFloder(fdt, device);
   flag = Get_FileExist(fdt, FILE_PRM, device);
   if (flag == 0) DeleteFileFloder(fdt, device);
   flag = Get_FileExist(fdt, FILE_WRK, device);
   if (flag == 0) DeleteFileFloder(fdt, device);
   flag = Get_FileExist(fdt, FILE_SAZ, device);
   if (flag == 0) DeleteFileFloder(fdt, device);
   Fm_ReadBytes(ADDR_FM_FileName, buf, 8);
   //判断删除的花型是不是当前花型----------
   if (device == NAND) {
      for (i = 0; i < 8; i++) {
         if (buf[i] != *(fdt + i)) {
            i = 0xffffffff;
            break;
         }
      }
      if (i != 0xffffffff) {
         SysFlag.FileOk = 0;
         Sys.ErrorCode = ERR_MemNoFile;
         for (i = 0; i < 8; i++) buf[i] = 0;
         Fm_Write1Byte(ADDR_FM_FileOK, 0);
         Fm_WriteBytes(ADDR_FM_FileName, buf, 8);
      }
   }
   return (flag);
}

//===================================================================
//功能: 将指定的色代号与PAT页数据进行比较,判断是否有起针
//返回: 0表示无出针
//===================================================================
uint32 PatSel_Out(uint16 sedaihao, uint16 pat_num) {
   uint32 i, j;
   uint32 sehao;
   uint16 m, n;
   uint32 addr;
   uint32 sel_out;
   sel_out = 0;
   addr = PatFile.PageByte * (uint32)pat_num;      //当前行PAT数据的首地址
   for (i = 0; i < 16; i++) {
      if (sedaihao > 0) {
         //判断当前色代号在PAT数据中是否有对应值--
         sehao = sedaihao & (1 << i);
         if (sehao > 0) {
            sehao = i;
            for (j = 0; j < PatFile.PageByte; j++) {
               n = PatFileDat[addr + j];
               m = (n & 0xf0) >> 4;          //得到PAT数据的高4位
               n &= 0xf;                 //得到PAT数据的低4位
               if (((sehao & m) > 0) || ((sehao & n) > 0)) {
                  sel_out = 1;
                  break;
               }
            }
         }

         sedaihao &= ~(1 << i);

         //若已对应则不再继续查找-------
         if (sel_out > 0) {
            break;
         }
      } else {
         break;
      }
   }
   return (sel_out);
}

//===================================================================
//功能: 判断两个色代号中是否有相同的位变量
//返回: 0表示正确,1表示错误(相同)
//===================================================================
uint32 Compare_SeDaiHao(uint16 front, uint16 back) {
   uint16 m, n;
   m = front ^ back;
   n = m & front;
   if (n != front) return (1);
   else return (0);
}

//===================================================================
//功能: 			对花型文件进行校验处理
//返回: 			0表示正确,其它表示错误
//修改时间:		2011-4-9
//===================================================================
uint32 CntFile_Check(void) {
   const char *Hz_ItemFileChk[][LANG_MAX] = {
      "正在检查花型数据......", "Checking file...",
      "色代号指向的PAT数据异常! ", "Select color to PAT error! ",
      "针对针时前后板同时起针! ", "Move bed with neddle error! ",
   };
   uint32 i, m;
   uint32 addr;
   uint16 s1_f_pat, s1_b_pat, s2_f_pat, s2_b_pat;
   uint16 s1_f_acolor, s1_f_hcolor, s1_b_acolor, s1_b_hcolor;
   uint16 s2_f_acolor, s2_f_hcolor, s2_b_acolor, s2_b_hcolor;
   uint32 error;
   uint32 sel_out;
   uint32 yaocuang;
   char str[50];
   uint8 s1_sz_flag, s2_sz_flag;
   uint8 s1_sazui, s2_sazui;
   error = 0;
   Dis_HintBox(HZ_TiSi[0][Lang], Hz_ItemFileChk[0][Lang], 1, C_Black, C_Back);
   for (i = 1; i < CntFile.ZongHangSu; i++) {
      addr = i * CNT_P;
      s1_f_pat = (uint16)CntFileDat[addr + CNT_S1Q_Pat] + (uint16)CntFileDat[addr + CNT_S1Q_Pat + 1] * 256;
      s1_b_pat = (uint16)CntFileDat[addr + CNT_S1H_Pat] + (uint16)CntFileDat[addr + CNT_S1H_Pat + 1] * 256;
      s2_f_pat = (uint16)CntFileDat[addr + CNT_S2Q_Pat] + (uint16)CntFileDat[addr + CNT_S2Q_Pat + 1] * 256;
      s2_b_pat = (uint16)CntFileDat[addr + CNT_S2H_Pat] + (uint16)CntFileDat[addr + CNT_S2H_Pat + 1] * 256;
      s1_f_acolor = (uint16)CntFileDat[addr + CNT_S1Q_AColor] + (uint16)CntFileDat[addr + CNT_S1Q_AColor + 1] * 256;
      s1_f_hcolor = (uint16)CntFileDat[addr + CNT_S1Q_HColor] + (uint16)CntFileDat[addr + CNT_S1Q_HColor + 1] * 256;
      s1_b_acolor = (uint16)CntFileDat[addr + CNT_S1H_AColor] + (uint16)CntFileDat[addr + CNT_S1H_AColor + 1] * 256;
      s1_b_hcolor = (uint16)CntFileDat[addr + CNT_S1H_HColor] + (uint16)CntFileDat[addr + CNT_S1H_HColor + 1] * 256;

      s2_f_acolor = (uint16)CntFileDat[addr + CNT_S2Q_AColor] + (uint16)CntFileDat[addr + CNT_S2Q_AColor + 1] * 256;
      s2_f_hcolor = (uint16)CntFileDat[addr + CNT_S2Q_HColor] + (uint16)CntFileDat[addr + CNT_S2Q_HColor + 1] * 256;
      s2_b_acolor = (uint16)CntFileDat[addr + CNT_S2H_AColor] + (uint16)CntFileDat[addr + CNT_S2H_AColor + 1] * 256;
      s2_b_hcolor = (uint16)CntFileDat[addr + CNT_S2H_HColor] + (uint16)CntFileDat[addr + CNT_S2H_HColor + 1] * 256;

      //读取纱嘴号------------------------------
      s1_sz_flag = CntFileDat[addr + CNT_S1_SaZui];
      s1_sazui = CntFileDat[addr + CNT_S1_SaZui + 1];
      s2_sz_flag = CntFileDat[addr + CNT_S2_SaZui];
      s2_sazui = CntFileDat[addr + CNT_S2_SaZui + 1];
      //有正常纱嘴值时才判断选针状况-----
      if (((s1_sz_flag == 0x80) && (s1_sazui > 0)) || ((s2_sz_flag == 0x80) && (s2_sazui > 0))) {
         sel_out = 0;
         //判断1系统前板A,H位有没有出针---
         if (s1_f_pat > 0) {
            sel_out = PatSel_Out(s1_f_acolor, s1_f_pat);
            if (sel_out == 0) {
               sel_out = PatSel_Out(s1_f_hcolor, s1_f_pat);
            }
         }
         //判断1系统后板A,H位有没有出针---
         if ((s1_b_pat > 0) && (sel_out == 0)) {
            sel_out = PatSel_Out(s1_b_acolor, s1_b_pat);
            if (sel_out == 0) {
               sel_out = PatSel_Out(s1_b_hcolor, s1_b_pat);
            }
         }
         //---------------------------------------------------------
         //判断2系统前板A,H位有没有出针---
         if ((s2_f_pat > 0) && (sel_out == 0)) {
            sel_out = PatSel_Out(s2_f_acolor, s2_f_pat);
            if (sel_out == 0) {
               sel_out = PatSel_Out(s2_f_hcolor, s2_f_pat);
            }
         }
         //判断2系统后板A,H位有没有出针---
         if ((s2_b_pat > 0) && (sel_out == 0)) {
            sel_out = PatSel_Out(s2_b_acolor, s2_b_pat);
            if (sel_out == 0) {
               sel_out = PatSel_Out(s2_b_hcolor, s2_b_pat);
            }
         }

         if ((s1_f_pat == 0) && (s1_b_pat == 0) && (s2_f_pat == 0) && (s2_b_pat == 0)) {
            sel_out = 1;
         }
         if (sel_out == 0) {
            error = 1;
            break;
         }
      }

      //判断针对针时是否存在打针现象--------
      yaocuang = CntFileDat[addr + CNT_YaoCuangDir];
      if ((yaocuang == 0x81) || (yaocuang == 0xc1)) {
         if ((s1_f_pat == s1_b_pat) && (s1_f_pat > 0)) {
            m = Compare_SeDaiHao(s1_f_acolor, s1_b_acolor);
            if (m > 0) {
               error = 2;
               break;
            }
         }
         if ((s2_f_pat == s2_b_pat) && (s2_f_pat > 0)) {
            m = Compare_SeDaiHao(s2_f_acolor, s2_b_acolor);
            if (m > 0) {
               error = 2;
               break;
            }
         }
      }
   }

   LcdMemPop();            //消除处理提示框

   if (error > 0) {
      Get_StringNum(str, i, Hz_ItemFileChk[error][Lang]);
      Dis_HintForm(HZ_CuoWu[0][Lang], str, C_Black, C_Back);
   }

   return (error);
}

//===================================================================
//功能:			文件管理
//修改时间:		2013-4-18
//===================================================================
void File_UsbProcess(void) {
   const char *Hz_MemNumErr[][LANG_MAX] = { "内存花型已满,请及时删除!", "Memory is full!" };
   const char *Hz_CntErr[][LANG_MAX] = { "动作文件存在异常,请重新导入!", "CNT file data err,reload it!" };
   const char *Hz_PatErr[][LANG_MAX] = {
      "花型宽度异常!", "File width error!",
      "花型高度异常!", "File height error!",
   };
   const char *Hz_Dir[][LANG_MAX] = { "目录", "Dir " };
   const char *Hz_DirError[][LANG_MAX] = { "不能操作文件夹!", "Can't operate floder!" };
#define MAX_NUM		20					//每页显示的项目条数
#define U_X			10					//U盘窗体的起始x坐标
#define M_X			(LCD_XSize-C_W-U_X)	//内存窗体的起始x坐标
#define C_W			260					//文件窗体的宽度
#define C_H			510					//文件窗体的高度
#define F_H			410					//文件区的高度

#define KEYT_Usb		100					//激活U盘窗口
#define KEYT_MEM	101					//激活内存窗口
#define KEYT_Num	110					//触摸的文件号

   uint16 text_xy[][2] = { 8, 470, 140, 470,  8, 490, 140, 490, 8, 510 };
   uint32 i, j;
   uint32 m, n;
   uint8 usb_list[MAX_Files][32];
   uint8 mem_list[MAX_Files][32];
   uint8 fdt[32];
   uint8 yuan_fdt[32], mubiao_fdt[32];
   uint32 yuan_device, mubiao_device;
   uint8 buf[BASE_Byte];
   uint8 dat[4096];
   uint8 now_fdt[8];
   uint32 usb_files, mem_files;
   uint32 flag;
   __VOL uint32 flag2;                     //发送系统状态回码
   uint32 read_usb = 1, read_mem = 1;           //读取设备中的文件
   uint32 ref_usb, ref_mem;                 // 1=只刷新窗体,2=只刷新数据,3=全部刷新
   uint32 active = 0;                            // 0=U盘管理框被激活,
   uint32 x, y;
   uint32 k_mem = 0, k_usb = 0;                 //当前光标在显示框中的位置
   uint32 kt_mem = 0, kt_usb = 0;                   //顶部显示的起始位置
   uint32 color;
   uint32 color_b;
   uint32 usb_hangsu = 0, usb_kuandu = 0, usb_gaodu = 0;
   uint32 mem_hangsu = 0, mem_kuandu = 0, mem_gaodu = 0;
   uint32 do_copy = 0;
   uint32 zongzensu;
   uint32 count;
   uint32 temp;
   uint32 read_file_err;                       //不为0表示只执行 变量复位,不显示花型载入信息
   uint32 file_send_err;                       // 0表示发送到下位机成功
   uint32 pat_s1q, pat_s1h, pat_s2q, pat_s2h;     //CNT文件中的PAT行号
   uint32 file_same;                           //不为0表示操作的花型与当前花型相同
   S_Form form;
   S_Form form_m;
   S_Form form_u;
   Touch_KeyPush();
   read_file_err = 0;
   Sys.LcdRef = REF_Item;
   Sys.RefLoop = 0;
   usb_files = 0;
   mem_files = 0;
   temp = Fm_Read1Byte(ADDR_FM_FileOK);
   if (temp == 0x55) {
      Fm_ReadBytes(ADDR_FM_FileName, now_fdt, 8);
   } else {
      for (i = 0; i < 8; i++) now_fdt[i] = 0;
   }
   zongzensu = Fm_Read1Word(ADDR_ParaZongZenSu);
   //窗体信息数据初始化----------
   form.Width = LCD_XSize;
   form.Height = LCD_YSize;
   form.Y = 0;
   form.X = 0;
   form.TitleText = Hz_UsbMang[0][Lang];
   form.TitleColor = FORM_Blue;
   form.TitleHeight = TITLE_H24;
   form.TitleFont = FONT24;
   form.BackColor = C_Back;
   form.FillBack = 1;

   LcdMemPush(form.X, form.Y, form.Width, form.Height);
   Dis_WindowsForm(&form);
   Touch_KeyEn(0, 0, LCD_XSize, 22, KEY_ESC, 6);

   flag = UsbApplication(0);
   if (flag > 0) active = 1;

   flag = EnumMassDevSys(NAND);
   //显示选择窗体下面的花型信息------
   for (i = 0; i < 5; i++) {
      Dis_Strings(Hz_ItemInfo[i][Lang], U_X + text_xy[i][0], text_xy[i][1], FONT16, C_Black, C_Back);
      Dis_Strings(Hz_ItemInfo[i][Lang], M_X + text_xy[i][0], text_xy[i][1], FONT16, C_Black, C_Back);
   }
   Dis_StringsAligen(Hz_FileProHint[0][Lang], ALIGEN_MIDDLE, 0, LCD_YSize - 35, LCD_XSize, FONT16, C_Qing, C_Back);

   //直接点击窗体将切换活动窗口区-------
   //注意:触摸键的优先级应放到最后
   Touch_KeyEn(U_X, 30, C_W - 40, C_H - 50, KEYT_Usb, 30);
   Touch_KeyEn(M_X, 30, C_W - 40, C_H - 50, KEYT_MEM, 31);

   TchVal.KeyNum[0] = 12 + 20;

   FatDir.SubDir[USB] = 0;
   while (1) {
      Send_DspReply();        //发送中断中置位的命令回码

      //读取有效文件信息-----------------------------
      if ((read_usb > 0) || (read_mem > 0)) {
         ref_usb = 3;
         ref_mem = 3;
         Dis_HintBox(HZ_TiSi[0][Lang], Hz_FileSeach[0][Lang], 1, C_Black, C_Back);
         //获取U盘中的有效花型----
         if (read_usb > 0) {
            read_usb = 0;
            usb_files = 0;
            if (Sys.UsbConnect > 0) {
               usb_files = Get_FullFiles(usb_list[0], USB);
               File_SortByFloder(usb_files, usb_list[0]);
            }
         }
         //获取内存中的有效花型----
         if (read_mem > 0) {
            read_mem = 0;
            mem_files = Get_FullFiles(mem_list[0], NAND);
            File_SortByFloder(mem_files, mem_list[0]);

            //查找当前使用文件的位置--------------------
            for (i = 0; i < mem_files; i++) {
               for (m = 0; m < 8; m++) {
                  if (mem_list[i][m] != now_fdt[m]) break;
               }
               //计算当前内存使用文件的位置
               if (m >= 8) {
                  kt_mem = (i / MAX_NUM) * MAX_NUM;
                  k_mem = i % MAX_NUM;
                  break;
               }
            }

         }

         LcdMemPop();            //消除搜索提示框
                                 //显示6个功能钮-------------
         for (i = 0; i < 6; i++) {
            x = GET_ScrCenterX(150);
            y = 100 + i * 60;
            Dis_TouchButton(Hz_ItemCopy[i][Lang], x, y, 140, 45, KEY_F1 + i, i, C_Black, FONT16);
         }
      }

      //刷新U盘管理框窗体--------------------------------------------
      //显示比例条----
      if (ref_usb > 0) {
         if (ref_usb == 3) {
            Dis_RectFill(U_X + 4, 54, C_W - 8, F_H, C_Black);
         }
         x = U_X + C_W - 6 - 20;
         if (active == 0) {
            Dis_PercentSlider(kt_usb + k_usb + 1, usb_files, x, 54, F_H, 20, C_Green, C_Black);
            Touch_KeyEn(x, 54, 20, 100, KEY_PU, 7);
            Touch_KeyEn(x, 54 + F_H - 100, 20, 100, KEY_PD, 8);
         } else {
            Dis_PercentSlider(kt_usb + k_usb + 1, usb_files, x, 54, F_H, 20, C_Gray1, C_Black);
         }
      }
      //刷新U盘花型文件窗体---------------
      if ((ref_usb == 1) || (ref_usb == 3)) {
         if (ref_usb == 1) ref_usb = 0;
         if (active == 0) {
            form_u.TitleColor = FORM_Red;
         } else {
            form_u.TitleColor = FORM_Gray;
         }
         //窗体信息数据初始化----------
         form_u.Width = C_W;
         form_u.Height = C_H;
         form_u.Y = 30;
         form_u.X = U_X;
         form_u.TitleText = Hz_UsbFile[0][Lang];
         form_u.TitleHeight = TITLE_H16;
         form_u.TitleFont = FONT16;
         form_u.BackColor = C_Back;
         form_u.FillBack = 0;
         Dis_WindowsForm(&form_u);

      }
      //刷新U盘管理框内部数据-----
      if ((ref_usb == 2) || (ref_usb == 3)) {
         ref_usb = 0;
         x = U_X + 20;
         for (i = 0; i < MAX_NUM; i++) {
            y = 60 + i * 20;
            if (active == 0) {
               Touch_KeyEn(x, y, C_W - 80, 16, KEYT_Num + i, 10 + i);
            }
            if (i == k_usb) {
               color = C_Red;
               if (active == 0) color_b = C_Yellow;
               else color_b = C_Gray1;
            } else {
               color = C_White;
               color_b = C_Black;
            }
            if ((i + kt_usb + 1) <= usb_files) {
               if ((usb_list[i + kt_usb][FAT_Properties] == FAT_Dir) && (usb_list[i + kt_usb][0] != '.')) {
                  Dis_Strings(Hz_Dir[0][Lang], x, y, FONT16, C_Yellow, C_Black);
               } else {
                  Dis_Strings("    ", x, y, FONT16, C_Black, C_Black);
               }
               Dis_NumAll(i + kt_usb + 1, x + 40, y, 3, FONT16, C_White, C_Black);
               Dis_Strings("-", x + 64, y, FONT16, C_White, C_Black);
               if ((usb_list[i + kt_usb][FAT_Properties] == FAT_Dir) && (usb_list[i + kt_usb][0] == '.')) {
                  Dis_Strings(Hz_FatDir[1][Lang], x + 80, y, FONT16, color, color_b);
                  Dis_RectFill(x + 80 + 8 * 6, y, 16, 16, color_b);
               } else {
                  Dis_FileName(usb_list[i + kt_usb], 0, x + 80, y, color, color_b);
               }
            } else {
               Dis_RectFill(x, y, 160, 16, C_Black);
               if (active == 0) {
                  TouchKeyDis(10 + i);
               }
            }
         }

         if ((usb_files > 0) && (usb_list[kt_usb + k_usb][FAT_Properties] != FAT_Dir)) {
            m = kt_usb + k_usb;
            //获取U盘中的CNT文件长度----
            usb_list[m][8] = 'C';
            usb_list[m][9] = 'N';
            usb_list[m][10] = 'T';
            //传递当前子目录的簇号--------
            if (FatDir.SubDir[USB] == 0x55) {
               usb_list[m][26] = usb_list[0][26];
               usb_list[m][27] = usb_list[0][27];
               usb_list[m][20] = usb_list[0][20];
               usb_list[m][21] = usb_list[0][21];
            } else {
               usb_list[m][26] = 0;
               usb_list[m][27] = 0;
               usb_list[m][20] = 0;
               usb_list[m][21] = 0;
            }
            flag = OpenFileFloder(usb_list[m], USB);
            CloseFile(USB);
            usb_hangsu = MyFileInfo[USB].LengthInByte / 128;
            if (usb_hangsu > 0) usb_hangsu--;
            //获取U盘中的PAT信息-----
            usb_list[m][8] = 'P';
            usb_list[m][9] = 'A';
            usb_list[m][10] = 'T';
            flag = OpenFileFloder(usb_list[m], USB);
            flag = ReadFile(512, buf, USB);
            CloseFile(USB);
            usb_kuandu = buf[0] + buf[1] * 256;
            usb_gaodu = buf[2] + buf[3] * 256;
            //获取修改时间---------
            Dis_FileTime((uint8 *)(MyFileInfo[USB].FileNameDat), U_X + text_xy[4][0] + 72, text_xy[4][1], C_Blue, C_Back);
         } else {
            usb_kuandu = 0;
            usb_gaodu = 0;
            usb_hangsu = 0;
            Dis_RectFill(U_X + text_xy[4][0] + 72, text_xy[4][1], 160, 16, C_Back);
         }

         Dis_NumHide(usb_files, U_X + text_xy[0][0] + 72, text_xy[0][1], 3, FONT16, C_Blue, C_Back);
         Dis_NumHide(usb_hangsu, U_X + text_xy[1][0] + 72, text_xy[1][1], 5, FONT16, C_Blue, C_Back);
         Dis_NumHide(usb_kuandu, U_X + text_xy[2][0] + 72, text_xy[2][1], 4, FONT16, C_Blue, C_Back);
         Dis_NumHide(usb_gaodu, U_X + text_xy[3][0] + 72, text_xy[3][1], 5, FONT16, C_Blue, C_Back);
      }

      //刷新内存管理框窗体--------------------------------------------
      //显示比例条----
      if (ref_mem > 0) {
         if (ref_mem == 3) {
            Dis_RectFill(M_X + 4, 54, C_W - 8, F_H, C_Black);
         }
         x = M_X + C_W - 6 - 20;
         if (active > 0) {
            Dis_PercentSlider(kt_mem + k_mem + 1, mem_files, x, 54, F_H, 20, C_Green, C_Black);
            Touch_KeyEn(x, 54, 20, 100, KEY_PU, 7);
            Touch_KeyEn(x, 54 + F_H - 100, 20, 100, KEY_PD, 8);
         } else {
            Dis_PercentSlider(kt_mem + k_mem + 1, mem_files, x, 54, F_H, 20, C_Gray1, C_Black);
         }
      }
      //刷新内存花型文件窗体-----------
      if ((ref_mem == 1) || (ref_mem == 3)) {
         if (ref_mem == 1) ref_mem = 0;
         if (active > 0) {
            form_m.TitleColor = FORM_Red;
         } else {
            form_m.TitleColor = FORM_Gray;
         }
         form_m.Width = C_W;
         form_m.Height = C_H;
         form_m.Y = 30;
         form_m.X = M_X;
         form_m.TitleText = Hz_MemFile[0][Lang];
         form_m.TitleHeight = TITLE_H16;
         form_m.TitleFont = FONT16;
         form_m.BackColor = C_Back;
         form_m.FillBack = 0;
         Dis_WindowsForm(&form_m);
      }
      //刷新内存管理框内部数据-------------
      if ((ref_mem == 2) || (ref_mem == 3)) {
         ref_mem = 0;
         x = M_X + 20;
         for (i = 0; i < MAX_NUM; i++) {
            y = 60 + i * 20;
            if (active == 1) {
               Touch_KeyEn(x, y, C_W - 50, 16, KEYT_Num + i, 10 + i);
            }
            if (i == k_mem) {
               color = C_Red;
               if (active == 1) color_b = C_Yellow;
               else color_b = C_Gray1;
            } else {
               color = C_White;
               color_b = C_Black;
            }
            if ((i + kt_mem + 1) <= mem_files) {
//					Dis_Strings("(",x,y,FONT16,C_Yellow,C_Black);
//					Dis_Hex(i+1,x+8,y,C_Yellow,C_Black);
//					Dis_Strings(")",x+16,y,FONT16,C_Yellow,C_Black);
               Dis_NumAll(i + kt_mem + 1, x + 40, y, 3, FONT16, C_White, C_Black);
               Dis_Strings("-", x + 64, y, FONT16, C_White, C_Black);
               Dis_FileName(mem_list[i + kt_mem], 0, x + 75, y, color, color_b);
            } else {
               Dis_RectFill(x, y, 160, 16, C_Black);
               if (active == 1) {
                  TouchKeyDis(10 + i);
               }
            }
         }

         if (mem_files > 0) {
            m = kt_mem + k_mem;
            //获取内存中的CNT文件长度----
            mem_list[m][8] = 'C';
            mem_list[m][9] = 'N';
            mem_list[m][10] = 'T';
            flag = OpenFileFloder(mem_list[m], NAND);
            CloseFile(NAND);
            mem_hangsu = MyFileInfo[NAND].LengthInByte / 128;
            if (mem_hangsu > 0) mem_hangsu--;
            //获取U盘中的PAT信息-----
            mem_list[m][8] = 'P';
            mem_list[m][9] = 'A';
            mem_list[m][10] = 'T';
            flag = OpenFileFloder(mem_list[m], NAND);
            flag = ReadFile(NAND_PageB, buf, NAND);
            CloseFile(NAND);
            mem_kuandu = buf[0] + buf[1] * 256;
            mem_gaodu = buf[2] + buf[3] * 256;
            //获取修改时间---------
            Dis_FileTime((uint8 *)(MyFileInfo[NAND].FileNameDat), M_X + text_xy[4][0] + 72, text_xy[4][1], C_Blue, C_Back);
         } else {
            mem_kuandu = 0;
            mem_gaodu = 0;
            mem_hangsu = 0;
            Dis_RectFill(M_X + text_xy[4][0] + 72, text_xy[4][1], 160, 16, C_Back);
         }

         Dis_NumHide(mem_files, M_X + text_xy[0][0] + 72, text_xy[0][1], 3, FONT16, C_Blue, C_Back);
         Dis_NumHide(mem_hangsu, M_X + text_xy[1][0] + 72, text_xy[1][1], 5, FONT16, C_Blue, C_Back);
         Dis_NumHide(mem_kuandu, M_X + text_xy[2][0] + 72, text_xy[2][1], 4, FONT16, C_Blue, C_Back);
         Dis_NumHide(mem_gaodu, M_X + text_xy[3][0] + 72, text_xy[3][1], 5, FONT16, C_Blue, C_Back);
      }

      Read_Key();

      Dis_TouchButtonFlash();

      //滑动状态下的左右键不处理------
      if (TchVal.Down == TCH_KeySlide) {
         if ((Key.Value == KEY_LEFT) || (Key.Value == KEY_RIGHT)) {
            Key.Value = KEY_No;
         }
      }

      //数字键选择当前文件-------------------------
      if (((Key.Value > 0) && (Key.Value <= KEY_F)) || ((Key.Value >= KEYT_Num) && (Key.Value < KEYT_Num + 20))) {
         if (Key.Value >= KEYT_Num) m = Key.Value - KEYT_Num;
         else m = Key.Value - 1;

         //在U盘窗口中处理----------------
         if (active == 0) {
            if ((kt_usb + m) < usb_files) {
               //在文件夹上双击当确定键处理----
               if ((TchVal.Down == TCH_KeyDown) && (k_usb == m) && (usb_list[kt_usb + k_usb][FAT_Properties] == FAT_Dir)) {
                  Key.Value = KEY_OK;
               } else {
                  k_usb = m;
                  ref_usb = 2;
               }
            }
         } else {
            if ((kt_mem + m) < mem_files) {
               k_mem = m;
               ref_mem = 2;
            }
         }
      } else if (Key.Value == KEY_DOWN) {
         //U盘管理框在激活状态------------
         if (active == 0) {
            ref_usb = 2;
            k_usb++;
            if (k_usb >= MAX_NUM) {
               k_usb = MAX_NUM - 1;
               if ((kt_usb + k_usb + 1) < usb_files) {
                  kt_usb++;
               }
            } else if ((k_usb + kt_usb + 1) > usb_files) {
               k_usb--;
            }
         } else {
            ref_mem = 2;
            k_mem++;
            if (k_mem >= MAX_NUM) {
               k_mem = MAX_NUM - 1;
               if ((kt_mem + k_mem + 1) < mem_files) {
                  kt_mem++;
               }
            } else if ((k_mem + kt_mem + 1) > mem_files) {
               k_mem--;
            }
         }
      } else if (Key.Value == KEY_UP) {
         //U盘管理框在激活状态------------
         if (active == 0) {
            ref_usb = 2;
            if (k_usb > 0) {
               k_usb--;
            } else {
               if (kt_usb > 0) kt_usb--;
            }
         } else {
            ref_mem = 2;
            if (k_mem > 0) {
               k_mem--;
            } else {
               if (kt_mem > 0) kt_mem--;
            }
         }
      }
      //下翻一页--------------------------------------------------
      else if (Key.Value == KEY_PD) {
         //U盘管理框在激活状态------------
         if (active == 0) {
            ref_usb = 2;
            if (usb_files > 0) {
               kt_usb += MAX_NUM;
               //翻页成功后判断当前光标位置是否超出---
               if (kt_usb < usb_files) {
                  if ((k_usb + kt_usb + 1) >= usb_files) {
                     k_usb = usb_files - kt_usb - 1;
                  }
               }
               //若不成功则将光标放到最后一行位置-----
               else {
                  kt_usb -= MAX_NUM;
                  k_usb = usb_files - kt_usb - 1;
               }
            }
         } else {
            ref_mem = 2;
            if (mem_files > 0) {
               kt_mem += MAX_NUM;
               //翻页成功后判断当前光标位置是否超出---
               if (kt_mem < mem_files) {
                  if ((k_mem + kt_mem + 1) >= mem_files) {
                     k_mem = mem_files - kt_mem - 1;
                  }
               }
               //若不成功则将光标放到最后一行位置-----
               else {
                  kt_mem -= MAX_NUM;
                  k_mem = mem_files - kt_mem - 1;
               }
            }
         }
      }
      //上翻一页---------------------------
      else if (Key.Value == KEY_PU) {
         //U盘管理框在激活状态------------
         if (active == 0) {
            ref_usb = 2;
            if (kt_usb >= MAX_NUM) {
               kt_usb -= MAX_NUM;
            } else {
               kt_usb = 0;
               k_usb = 0;
            }
         } else {
            ref_mem = 2;
            if (kt_mem >= MAX_NUM) {
               kt_mem -= MAX_NUM;
            } else {
               kt_mem = 0;
               k_mem = 0;
            }
         }
      }
      //激活U盘窗口----------
      else if (Key.Value == KEYT_Usb) {
         if (Sys.UsbConnect > 0) {
            if (active != 0) {
               active = 0;
               ref_usb = 3;
               ref_mem = 3;
            }
         } else {
            Key.Value = KEY_USB;
         }
      }
      //激活内存窗口----
      else if ((Key.Value == KEYT_MEM) && (active != 1)) {
         active = 1;
         ref_usb = 3;
         ref_mem = 3;
      }
      //在两个管理框间切换------------
      else if (Key.Value == KEY_RIGHT) {
         if (active == 0) active = 1;
         else active = 0;
         if (Sys.UsbConnect == 0) active = 1;
         ref_mem = 3;
         ref_usb = 3;
      }
      //在两个管理框间切换------------
      else if (Key.Value == KEY_LEFT) {
         if (active == 0) active = 1;
         else active = 0;
         if (Sys.UsbConnect == 0) active = 1;
         ref_mem = 3;
         ref_usb = 3;
      }

      //在文件夹上按确定处理--------
      if ((Key.Value == KEY_OK) && (active == 0) && (usb_list[kt_usb + k_usb][FAT_Properties] == FAT_Dir)) {
         Key.Value = KEY_No;
         m = kt_usb + k_usb;
         //返回上级目录处理---------
         if (usb_list[m][0] == '.') {
            read_usb = 1;
            FatDir.CurrentDir[26] = FatDir.ParentDir[26];
            FatDir.CurrentDir[27] = FatDir.ParentDir[27];
            FatDir.CurrentDir[20] = FatDir.ParentDir[20];
            FatDir.CurrentDir[21] = FatDir.ParentDir[21];
            if ((FatDir.CurrentDir[26] == 0) && (FatDir.CurrentDir[27] == 0) && (FatDir.CurrentDir[20] == 0) && (FatDir.CurrentDir[21] == 0)) {
               FatDir.SubDir[USB] = 0;
            }
            k_usb = 0;                    //光标移到第1项
            kt_usb = 0;
            ref_usb = 2;
         }
         //进入文件夹-----------
         else {
            read_usb = 1;
            FatDir.SubDir[USB] = 0x55;
            FatDir.CurrentDir[26] = usb_list[m][26];
            FatDir.CurrentDir[27] = usb_list[m][27];
            FatDir.CurrentDir[20] = usb_list[m][20];
            FatDir.CurrentDir[21] = usb_list[m][21];
            k_usb = 0;                    //光标移到第1项
            kt_usb = 0;
            ref_usb = 2;
         }
      }

      //将U盘中选定花型输入到内存中-----------------
      if (((Key.Value == KEY_F1) || (Key.Value == KEY_OK)) && (Sys.UsbConnect > 0) && (active == 0) && (usb_files > 0) && (usb_list[kt_usb + k_usb][FAT_Properties] != FAT_Dir)) {
         //若内存花型已达到最大数量，则提示--
         if (mem_files >= MAX_MemFiles) {
            Dis_HintForm(HZ_CuoWu[0][Lang], Hz_MemNumErr[0][Lang], C_Black, C_Back);
         } else {
            m = kt_usb + k_usb;
            //判断CNT文件是否超长-----
            usb_list[m][8] = 'C';
            usb_list[m][9] = 'N';
            usb_list[m][10] = 'T';
            //传递当前子目录的簇号--------
            if (FatDir.SubDir[USB] == 0x55) {
               usb_list[m][26] = usb_list[0][26];
               usb_list[m][27] = usb_list[0][27];
               usb_list[m][20] = usb_list[0][20];
               usb_list[m][21] = usb_list[0][21];
            } else {
               usb_list[m][26] = 0;
               usb_list[m][27] = 0;
               usb_list[m][20] = 0;
               usb_list[m][21] = 0;
            }
            flag = OpenFileFloder(usb_list[m], USB);
            CloseFile(USB);
            n = MyFileInfo[USB].LengthInByte;
            //判断PAT文件是否超长-----
            usb_list[m][8] = 'P';
            usb_list[m][9] = 'A';
            usb_list[m][10] = 'T';
            flag = OpenFileFloder(usb_list[m], USB);
            CloseFile(USB);
            if (MyFileInfo[USB].LengthInByte > n) n = MyFileInfo[USB].LengthInByte;
            //判断源文件是否超长-----------------
            if (n > MAX_FileSize) {
               Dis_HintForm(HZ_CuoWu[0][Lang], Hz_ItemCopyErr[0][Lang], C_Black, C_Back);
            } else {
               //判断内存当前花型与操作的花型是否相同----
               file_same = 0;
               if (SysFlag.FileOk > 0) {
                  for (i = 0; i < 8; i++) {
                     if (usb_list[m][i] != CntFileFdt[i]) break;
                  }
                  if (i >= 8) file_same = 1;
               }

               //若文件不同或在停车状态下允许操作-----
               if ((file_same == 0) || (SysFlag.MotoStop > 0)) {
                  //若文件相同则不允许运行------------
                  if (file_same > 0) {
                     flag2 = Send_SysStatus(SYS_Free);
                  }
                  //判断SD卡中文件是否存在----------------
                  flag = Get_FileExist(usb_list[m], FILE_CNT, NAND);
                  if (flag == 0) {
                     flag = Dis_SureDialog(HZ_FileOptHint[FILE_Exist][Lang], HZ_FileOptHint[FILE_ExitOverwrite][Lang], C_Back);
                     if (flag != DO_Cancel) {
                        flag = 1;
                     } else flag = 0;
                  }
                  //将U盘当前文件复制到内存中-----
                  if (flag > 0) {
                     //传递当前子目录的簇号--------
                     if (FatDir.SubDir[USB] == 0x55) {
                        usb_list[m][26] = usb_list[0][26];
                        usb_list[m][27] = usb_list[0][27];
                        usb_list[m][20] = usb_list[0][20];
                        usb_list[m][21] = usb_list[0][21];
                     } else {
                        usb_list[m][26] = 0;
                        usb_list[m][27] = 0;
                        usb_list[m][20] = 0;
                        usb_list[m][21] = 0;
                     }
                     flag = FileAll_WriteMem(usb_list[m]);
                     read_mem = 1;
                  }

                  if (file_same > 0) {
                     flag2 = Sys_StatusChang();
                  }
               }
               //若文件相同且在运行状态处理-----
               else {
                  Dis_HintForm(HZ_TiSi[0][Lang], HZ_OperateErr[0][Lang], C_Black, C_Back);   //提示机器正在运行
               }
            }
         }
      }
      //将内存选定花型输入到U盘中----
      else if ((Key.Value == KEY_F2) && (Sys.UsbConnect > 0) && (active == 1) && (mem_files > 0)) {
         for (i = 0; i < 8; i++) {
            if (CntFileFdt[i] != mem_list[kt_mem + k_mem][i]) break;
         }
         //若选中的内存花型为当前花型,保存工作文件--
         if (i >= 8) {
            flag = Write_WrkToSD();
         }
         //判断U盘中文件是否存在----------------
         Fdt_InitDir(mem_list[kt_mem + k_mem]);
         flag = Get_FileExist(mem_list[kt_mem + k_mem], FILE_CNT, USB);
         if (flag == 0) {
            flag = Dis_SureDialog(HZ_FileOptHint[FILE_Exist][Lang], HZ_FileOptHint[FILE_ExitOverwrite][Lang], C_Back);
            if (flag != DO_Cancel) {
               flag = 1;
            } else flag = 0;
         }
         //将内存中当前文件复制到U盘中-----
         if (flag > 0) {
            flag = FileAll_WriteUsb(mem_list[kt_mem + k_mem]);
            read_usb = 1;
         }
      }
      //从内存复制工作参数到当前光标花样-----------------------------
      else if ((Key.Value == KEY_F3) && (mem_files > 0)) {
         //工作参数文件(.WRK)从内存复制到U盘----------
         if (active == 0) {
            for (i = 0; i < 32; i++) mubiao_fdt[i] = usb_list[kt_usb + k_usb][i];
            yuan_fdt[8] = 'W';
            yuan_fdt[9] = 'R';
            yuan_fdt[10] = 'K';
            yuan_device = NAND;
            mubiao_fdt[8] = 'W';
            mubiao_fdt[9] = 'R';
            mubiao_fdt[10] = 'K';
            mubiao_device = USB;
            do_copy = 1;
         }
         //工作参数文件(.WRK)从内存复制到内存----------------------
         else {
            for (i = 0; i < 32; i++) mubiao_fdt[i] = mem_list[kt_mem + k_mem][i];
            yuan_fdt[8] = 'W';
            yuan_fdt[9] = 'R';
            yuan_fdt[10] = 'K';
            yuan_device = NAND;
            mubiao_fdt[8] = 'W';
            mubiao_fdt[9] = 'R';
            mubiao_fdt[10] = 'K';
            mubiao_device = NAND;
            do_copy = 1;
         }
      }
      //从U盘复制工作参数到当前光标花样-----------------------------
      else if ((Key.Value == KEY_F4) && (usb_files > 0)) {
         //从U盘复制工作参数文件(.WRK)到U盘----------
         if (active == 0) {
            for (i = 0; i < 32; i++) mubiao_fdt[i] = usb_list[kt_usb + k_usb][i];
            yuan_fdt[8] = 'W';
            yuan_fdt[9] = 'R';
            yuan_fdt[10] = 'K';
            yuan_device = USB;
            mubiao_fdt[8] = 'W';
            mubiao_fdt[9] = 'R';
            mubiao_fdt[10] = 'K';
            mubiao_device = USB;
            do_copy = 1;
         }
         //从U盘复制工作参数文件(.WRK)到内存----------
         else {
            for (i = 0; i < 32; i++) mubiao_fdt[i] = mem_list[kt_mem + k_mem][i];
            yuan_fdt[8] = 'W';
            yuan_fdt[9] = 'R';
            yuan_fdt[10] = 'K';
            yuan_device = USB;
            mubiao_fdt[8] = 'W';
            mubiao_fdt[9] = 'R';
            mubiao_fdt[10] = 'K';
            mubiao_device = NAND;
            do_copy = 1;
         }
      }
      //按"F5"键删除选定的文件------------------------
      else if (Key.Value == KEY_F5) {
         if (active == 0) {
            m = USB;
            for (i = 0; i < 32; i++) {
               fdt[i] = usb_list[kt_usb + k_usb][i];
            }
         } else {
            m = NAND;
            for (i = 0; i < 32; i++) {
               fdt[i] = mem_list[kt_mem + k_mem][i];
            }
         }

         while (1) {
            //判断当前是否是在文件夹上----------------------
            if ((m == USB) && (fdt[FAT_Properties] == FAT_Dir)) {
               Dis_HintForm(HZ_TiSi[0][Lang], HZ_FileOptHint[FILE_DelFloderErr][Lang], C_Black, C_Back);
               break;
            }

            //判断内存当前花型与操作的花型是否相同----
            file_same = 0;
            if ((SysFlag.FileOk > 0) && (m == NAND)) {
               for (i = 0; i < 8; i++) {
                  if (fdt[i] != CntFileFdt[i]) break;
               }
               if (i >= 8) file_same = 1;
            }

            //若文件不同或在停车状态下允许操作-----
            if ((file_same == 0) || (SysFlag.MotoStop > 0)) {
               if (file_same > 0) {
                  flag2 = Send_SysStatus(SYS_Free);
               }
               flag = Dis_SureDialog(HZ_TiSi[0][Lang], Hz_ItemDel[active][Lang], C_Back);
               //删除文件--------------
               if (flag != DO_Cancel) {
                  if (active == 0) read_usb = 1;
                  else read_mem = 1;
                  flag = File_DeleteHuaXin(fdt, m);
                  Dis_HintForm(HZ_TiSi[0][Lang], HZ_FileOptHint[FILE_DelOk][Lang], C_Black, C_Back);
               }
               if (file_same > 0) {
                  flag2 = Sys_StatusChang();
               }
            }
            //若文件相同且在运行状态处理-----
            else {
               Dis_HintForm(HZ_TiSi[0][Lang], HZ_OperateErr[0][Lang], C_Black, C_Back);   //提示机器正在运行
            }
            break;
         }
      }
      //选择内存花样并将当前花型发送到下位机---------------------
      else if (((Key.Value == KEY_F6) || (Key.Value == KEY_OK)) && (active == 1) && (mem_files > 0)) {
         //运行状态下禁止操作---------------
         if (SysFlag.MotoStop > 0) {
            flag2 = Send_SysStatus(SYS_Free);

            for (i = 0; i < 8; i++) {
               fdt[i] = mem_list[kt_mem + k_mem][i];
            }
            //选择内存花型处理--------------------------------------------
            //若原先有花型,则将当前的工作参数保存到文件----
            if (SysFlag.FileOk > 0) {
               flag = Write_WrkToSD();
            }
            read_file_err = 0;


            x = GET_ScrCenterX(240);
            //判断花型宽度是否异常-----------
            if ((mem_kuandu > zongzensu) || (mem_kuandu == 0)) {
               Dis_HintForm(HZ_CuoWu[0][Lang], Hz_PatErr[0][Lang], C_Black, C_Back);
            }
            //判断花型高度是否异常-----------
            else if ((mem_gaodu > MAX_CntRow) || (mem_gaodu == 0)) {
               Dis_HintForm(HZ_CuoWu[0][Lang], Hz_PatErr[1][Lang], C_Black, C_Back);
            } else {
               while (1) {
                  file_send_err = 0;
                  //判断当前花型与选择的花型是否相同---------
                  flag = 0;
                  for (i = 0; i < 8; i++) {
                     if (now_fdt[i] != fdt[i]) {
                        flag = 1;
                        break;
                     }
                  }
                  //判断下位机的文件是否相同---------
                  if (flag == 0) {
                     flag = Send_CNTInfo(0);
                     if (flag == 0) {
                        //与下位机文件相同 则给出提示信息----
                        if (NetRec.Hint == 0) {
                           flag = 0;
                        } else {
                           flag = 1;
                        }
                     } else {
                        file_send_err = 1;        //显示	花型载入失败
                        break;
                     }
                  }
                  //相同处理--------
                  if (flag == 0) {
                     flag = Dis_SureDialog(HZ_TiSi[0][Lang], HZ_FileOptHint[FILE_ExitOverwrite][Lang], C_Back);
                     if (flag == DO_Cancel) {
                        file_send_err = 0x55;
                        break;
                     }
                  }

                  RunVal.SazuiKunBangSu = 0;
                  //复位循环----
                  RunVal.F2_Loop = 0;
                  Fm_Write1Byte(ADDR_FM_FirstLoop, RunVal.F2_Loop);

                  CloseFile(NAND);
                  //读取CNT文件内容------------------------------------
                  fdt[8] = 'C';
                  fdt[9] = 'N';
                  fdt[10] = 'T';
                  flag = OpenFileFloder(fdt, NAND);
                  CntFile.Size = MyFileInfo[NAND].LengthInByte;
                  count = CntFile.Size / NAND_PageB;
                  if (CntFile.Size % NAND_PageB) count++;
                  for (i = 0; i < count; i++) {
                     flag = ReadFile(NAND_PageB, buf, NAND);
                     if (flag > 0) {
                        flag = SetFilePoint(i * NAND_PageB, NAND);
                        flag = ReadFile(NAND_PageB, buf, NAND);
                     }
                     if (flag > 0) m = count;
                     else m = i;
                     Dis_SpeedInfo(HZ_ItemCntWR[1][Lang], m + 1, count, 220, C_Gray);
                     if (flag > 0) {
                        Dis_HintForm(HZ_CuoWu[0][Lang], HZ_FileOptHint[FILE_ReadErr][Lang], C_Black, C_Back);
                        read_file_err = 1;
                        break;
                     }
                     for (j = 0; j < NAND_PageB; j++) {
                        CntFileDat[i * NAND_PageB + j] = buf[j];
                     }
                  }
                  CloseFile(NAND);
                  if (read_file_err > 0) break;

                  for (i = 0; i < 32; i++) CntFileFdt[i] = MyFileInfo[NAND].FileNameDat[i];

                  //CntFile.UseCrc=GetCntCheck(CntFileDat);
                  /*					
                                      CntFile.UseCrc=0;
                                      if(CntFile.UseCrc>0)
                                      {
                                          flag=CntCheckCRC(CntFileDat,CntFile.Size);
                                          if(flag>0)
                                          {
                                              Dis_HintForm(*(HZ_CuoWu+Lang),*(HZ_ItemFileErr+4+Lang),C_Black,C_Back);
                                              break;
                                          }
                                      }
                  */
                  //判断是否是使用两个系统度目-----
                  CntFile.DuMu_Two = 0;
                  if ((CntFileDat[0] == 'M') && (CntFileDat[1] == 'D')) {
                     CntFile.DuMu_Two = 0x55;
                  }

                  //获取花型的总行数并校验数据--------------------
                  CntFile.ZongHangSu = 0;
                  flag = 0;
                  for (i = 1; i < MAX_CntRow; i++) {
                     m = i * 128;
                     pat_s1q = CntFileDat[m + CNT_S1Q_Pat + 1];
                     pat_s1q *= 256;
                     pat_s1q += CntFileDat[m + CNT_S1Q_Pat];
                     pat_s1h = CntFileDat[m + CNT_S1H_Pat + 1];
                     pat_s1h *= 256;
                     pat_s1h += CntFileDat[m + CNT_S1H_Pat];
                     if ((pat_s1q > mem_gaodu) || (pat_s1h > mem_gaodu)) {
                        flag = 1;
                     } else {
                        pat_s2q = CntFileDat[m + CNT_S2Q_Pat + 1];
                        pat_s2q *= 256;
                        pat_s2q += CntFileDat[m + CNT_S2Q_Pat];
                        pat_s2h = CntFileDat[m + CNT_S2H_Pat + 1];
                        pat_s2h *= 256;
                        pat_s2h += CntFileDat[m + CNT_S2H_Pat];
                        if ((pat_s2q > mem_gaodu) || (pat_s2h > mem_gaodu)) {
                           flag = 1;
                        }
                     }

                     //判断结束标志---------
                     if (CntFileDat[m + CNT_End] == 1) {
                        CntFile.ZongHangSu = i;
                        break;
                     }
                     if (i > mem_hangsu) {
                        flag = 1;
                        break;
                     }
                  }
                  if (flag > 0) {
                     Dis_HintForm(HZ_CuoWu[0][Lang], Hz_CntErr[0][Lang], C_Black, C_Back);
                     read_file_err = 1;
                     break;
                  }
                  //若有PRM文件,则将循环数据写入CNT文件并删除PRM文件-------
                  flag = PrmFile_WriteCnt();

                  //读取PAT文件内容---------------------------------------
                  fdt[8] = 'P';
                  fdt[9] = 'A';
                  fdt[10] = 'T';
                  OpenFileFloder(fdt, NAND);
                  PatFile.Size = MyFileInfo[NAND].LengthInByte;
                  count = PatFile.Size / NAND_PageB;
                  if (PatFile.Size % NAND_PageB) count++;
                  for (i = 0; i < count; i++) {
                     flag = ReadFile(NAND_PageB, buf, NAND);
                     if (flag > 0) {
                        flag = SetFilePoint(i * NAND_PageB, NAND);
                        flag = ReadFile(NAND_PageB, buf, NAND);
                     }
                     if (flag > 0) m = count;
                     else m = i;
                     Dis_SpeedInfo(HZ_ItemPatWR[1][Lang], m + 1, count, 220, C_Gray);
                     if (flag > 0) {
                        Dis_HintForm(HZ_CuoWu[0][Lang], HZ_FileOptHint[FILE_ReadErr][Lang], C_Black, C_Back);
                        read_file_err = 1;
                        break;
                     }
                     for (j = 0; j < NAND_PageB; j++) {
                        PatFileDat[i * NAND_PageB + j] = buf[j];
                     }
                  }
                  CloseFile(NAND);
                  if (read_file_err > 0) break;

                  for (i = 0; i < 32; i++) PatFileFdt[i] = MyFileInfo[NAND].FileNameDat[i];
                  PatFile.Width = PatFileDat[0] + PatFileDat[1] * 256;
                  PatFile.Height = PatFileDat[2] + PatFileDat[3] * 256;
                  /*
                                      PatFile.UseCrc=GetPatCheck(PatFileDat);
                                      PatFile.CrcLen=PatFileDat[PatFile.Size-1]*256+PatFileDat[PatFile.Size-2];
                                      PatFile.DatLen=PatFile.Size-PatFile.CrcLen-2;
                                      if(PatFile.DatLen>PatFile.Size) PatFile.DatLen=0;
                                      PatFile.CrcStart=PatFile.DatLen;
                                      if(PatFile.UseCrc>0)
                                      {
                                          flag=PatCheckCRC(PatFileDat,PatFile.Size);
                                          if(flag>0)
                                          {
                                              Dis_HintForm(*(HZ_CuoWu+Lang),*(HZ_ItemFileErr+6+Lang),C_Black,C_Back);
                                              break;
                                          }
                                      }
                  */

                  //对花型文件进行校验处理-----------------------------
                  PatFile.PageByte = (PatFile.Width >> 1) + (PatFile.Width & 0x1);
                  flag = CntFile_Check();
                  if (flag > 0) {
                     read_file_err = 1;
                     break;
                  }
/*					
                        //向下位机发送文件信息-----------------------------
                        flag=Send_CntFile(0);
                        if(flag>0)
                        {
                            file_send_err=1;
                            break;
                        }
                        DelayMs(300);
                        flag=Send_PatFile(0);
                        if(flag>0)
                        {
                            file_send_err=1;
                            break;
                        }	

                        PeiDui_Process();					//处理主板查询显示器的配置文件
*/
                  //读取WRK文件内容---------------------------------------
                  flag = Get_FileExist(fdt, FILE_WRK, NAND);
                  if (flag == 0) {
                     flag = Write_WrkToFlash(fdt, NAND);
                     if (flag == 0) {
                        Para_WriteBagIDAll();
                        Send_Para();                //防止主板计算起针点时按老的参数计算
                     }
                  }
                  //若工作参数文件不存在则写入日志---
                  else {
                     Recod_AddNew(MESS_NoWrkPara);       //增加一条日志记录
                  }

                  //获取.SAZ纱嘴捆绑文件数据---------------
                  Get_SazFileData(fdt);
                  flag = Send_SazFile();

                  for (i = 0; i < 8; i++) now_fdt[i] = fdt[i]; //更新当前花型名称
                  SysFlag.FileOk = 1;
                  Sys.ErrorCode = 0;
                  Fm_Write1Byte(ADDR_FM_FileOK, 0x55);
                  Fm_WriteBytes(ADDR_FM_FileName, CntFileFdt, 8);
                  //清复位成功标志------
                  SysFlag.ResetOk = 0;
                  Fm_Write1Byte(ADDR_FM_RunReset, 0);
                  RunVal.NowBz_Hour = 0;
                  RunVal.NowBz_Minute = 0;
                  RunVal.NowBz_Second = 0;
                  RunVal.NowTc_Hour = 0;
                  RunVal.NowTc_Minute = 0;
                  RunVal.NowTc_Second = 0;
                  //运行信息初始化------------------
                  RunVal.CntHangHao = 1;
                  RunVal.GetCntInfo = 1;
                  Sys.RefRunItem = 1;   //刷新运行界面底部功能图标及其它
                  Sys.LcdRefAll = 1;
                  break;
               }

               //花型读取错误处理---------------
               if ((read_file_err > 0) || (file_send_err == 1)) {
                  SysFlag.FileOk = 0;
                  Sys.ErrorCode = ERR_MemNoFile;
                  Fm_Write1Byte(ADDR_FM_FileOK, 0);
                  //清复位成功标志------
                  SysFlag.ResetOk = 0;
                  Fm_Write1Byte(ADDR_FM_RunReset, 0);
                  RunVal.NowBz_Hour = 0;
                  RunVal.NowBz_Minute = 0;
                  RunVal.NowBz_Second = 0;
                  RunVal.NowTc_Hour = 0;
                  RunVal.NowTc_Minute = 0;
                  RunVal.NowTc_Second = 0;
                  //运行信息初始化------------------
                  RunVal.CntHangHao = 1;
                  RunVal.GetCntInfo = 1;
                  Sys.RefRunItem = 1;   //刷新运行界面底部功能图标及其它
               }

               //小于2显示提示信息-----
               if ((file_send_err < 2) && (read_file_err == 0)) {
                  Dis_HintForm(HZ_TiSi[0][Lang], HZ_HuaXingSend[file_send_err][Lang], C_Black, C_Back);
               }
            }

            flag2 = Sys_StatusChang();
         } else {
            Dis_HintForm(HZ_TiSi[0][Lang], HZ_OperateErr[0][Lang], C_Black, C_Back);   //提示机器正在运行
         }
      }
      //检测U盘-----------------------------------------------
      else if (Key.Value == KEY_USB) {
         if (Sys.UsbConnect == 0) {
            flag = UsbApplication(1);
            if (Sys.UsbConnect > 0) {
               read_usb = 1;
               FatDir.SubDir[USB] = 0;
            }
         }
      }
      if (Key.Value == KEY_ESC) {
         break;
      }

      //参数文件复制时判断目标是不是文件夹-------
      if ((do_copy > 0) && (mubiao_device == USB)) {
         if (mubiao_fdt[FAT_Properties] == FAT_Dir) {
            do_copy = 0;
            Dis_HintForm(HZ_CuoWu[0][Lang], Hz_DirError[0][Lang], C_Black, C_Back);
         }
      }

      //将源文件参数复制到目标文件(文件名与目标文件相同)--------
      if (do_copy > 0) {
         do_copy = 0;
         if (SysFlag.MotoStop > 0) {
            flag2 = Send_SysStatus(SYS_Free);

            if (yuan_device == NAND) n = mem_files;
            else n = usb_files;
            m = Dis_InputDialog(n, 1, Hz_ParaCopySel[0][Lang], C_Back);

            //判断源文件是否是文件夹--------------------
            if ((yuan_device == USB) && (usb_list[m - 1][FAT_Properties] == FAT_Dir)) {
               Dis_HintForm(HZ_CuoWu[0][Lang], Hz_DirError[0][Lang], C_Black, C_Back);
               flag = 1;
            }
            //判断源文件是否存在工作参数文件---------------------
            else if ((m > 0) && (m <= n)) {
               if (yuan_device == NAND) {
                  for (i = 0; i < 8; i++) {
                     yuan_fdt[i] = mem_list[m - 1][i];
                  }
               } else {
                  for (i = 0; i < 8; i++) {
                     yuan_fdt[i] = usb_list[m - 1][i];
                  }
               }

               //若源文件是当前工艺，则将Flash中的数据重新写成WRK文件----
               //防止当前工艺工作参数修改后直接导入到其它文件时修改的数据不对
               if (SysFlag.FileOk > 0) {
                  for (i = 0; i < 8; i++) {
                     if (yuan_fdt[i] != CntFileFdt[i]) {
                        break;
                     }
                  }
                  //若源文件为当前工艺-----
                  if (i >= 8) {
                     flag = Write_WrkToSD();
                  }
               }
               //读取源文件内容-----
               if (yuan_device == USB) {
                  //传递当前子目录的簇号--------
                  if (FatDir.SubDir[USB] == 0x55) {
                     yuan_fdt[26] = usb_list[0][26];
                     yuan_fdt[27] = usb_list[0][27];
                     yuan_fdt[20] = usb_list[0][20];
                     yuan_fdt[21] = usb_list[0][21];
                  } else {
                     yuan_fdt[26] = 0;
                     yuan_fdt[27] = 0;
                     yuan_fdt[20] = 0;
                     yuan_fdt[21] = 0;
                  }
               }
               flag = Get_FileExist(yuan_fdt, FILE_WRK, yuan_device);
               if (flag > 0) {
                  Dis_HintForm(HZ_TiSi[0][Lang], Hz_WrkFileNone[0][Lang], C_Black, C_Back);
               }
               //判断源文件和目标文件是否为同一个文件------------------
               else if (yuan_device == mubiao_device) {
                  if (yuan_device == NAND) {
                     if (m == (kt_mem + k_mem + 1)) {
                        flag = 1;
                     }
                  } else if (yuan_device == USB) {
                     if (m == (kt_usb + k_usb + 1)) {
                        flag = 1;
                     }
                  }
                  if (flag > 0) {
                     Dis_HintForm(HZ_CuoWu[0][Lang], HZ_SameFileErr[0][Lang], C_Black, C_Back);
                  }
               }
            } else flag = 1;

            //若输入数值有效处理----------------------------------
            if (flag == 0) {
               //判断目标花型的参数文件是否存在----
               Fdt_InitDir(mubiao_fdt);
               flag = Get_FileExist(mubiao_fdt, FILE_WRK, mubiao_device);
               if (flag == 0) {
                  flag = Dis_SureDialog(HZ_FileOptHint[FILE_Exist][Lang], HZ_FileOptHint[FILE_ExitOverwrite][Lang], C_Back);
                  if (flag == DO_Cancel) flag = 0;
                  else {
                     flag = DeleteFileFloder(mubiao_fdt, mubiao_device);
                     flag = 1;
                  }
               }
               if (flag > 0) {
                  //读取源文件内容-----
                  if (yuan_device == USB) {
                     //传递当前子目录的簇号--------
                     if (FatDir.SubDir[USB] == 0x55) {
                        yuan_fdt[26] = usb_list[0][26];
                        yuan_fdt[27] = usb_list[0][27];
                        yuan_fdt[20] = usb_list[0][20];
                        yuan_fdt[21] = usb_list[0][21];
                     } else {
                        yuan_fdt[26] = 0;
                        yuan_fdt[27] = 0;
                        yuan_fdt[20] = 0;
                        yuan_fdt[21] = 0;
                     }
                  }
                  flag = OpenFileFloder(yuan_fdt, yuan_device);
                  flag = ReadFile(4096, dat, yuan_device);
                  CloseFile(yuan_device);
                  //生成目标文件----
                  flag = CreateFile(mubiao_fdt, 4096, mubiao_device);
                  if (flag == 0) {
                     flag = WriteFile(4096, dat, mubiao_device);
                  }
                  if (flag > 0) flag = 1;
                  CloseFile(mubiao_device);
                  Dis_HintForm(HZ_TiSi[0][Lang], HZ_GzFileOk[flag][Lang], C_Black, C_Gray);
                  //若工作参数导入到当前使用的花样则写入Flash
                  if (mubiao_device == NAND) {
                     //判断目标文件是否为当前使用的花样--------
                     for (i = 0; i < 8; i++) {
                        if (mubiao_fdt[i] != now_fdt[i]) break;
                     }
                     //参数文件写入Flash
                     if (i >= 8) {
                        Write_WrkToFlash(now_fdt, NAND);
                        if (flag == 0) {
                           Para_WriteBagIDAll();
                        }
                     }
                  }
               }
            }

            flag2 = Sys_StatusChang();
         } else {
            Dis_HintForm(HZ_TiSi[0][Lang], HZ_OperateErr[0][Lang], C_Black, C_Back);   //提示机器正在运行
         }
      }
   }

   DelayMs(20);
   LcdMemPop();
   Touch_KeyPop();
   Key.Value = KEY_No;
}

//===================================================================
//功能: 			从U盘载入字库文件
//说明:
//修改时间:		2013-4-23
//===================================================================
void File_LoadFont(void) {
   const char *Hz_UsbListCnt[][LANG_MAX] = { "字库文件输入到内存", "Load font lib file" };
   const char *Hz_FontLibErr[][LANG_MAX] = { "字库文件不符合要求!", "Font file check error!" };
   uint32 i;
   uint8 file_fdt[32];
   uint8 file_list[MAX_Files][32];
   uint32 file_tol;                            //选中的文件数
   uint32 count;
   uint32 num;
   uint32 flag;
   uint32 file_len;
   uint32 addr;
   file_fdt[0] = 'L';
   file_fdt[1] = 'I';
   file_fdt[2] = 'B';
   file_tol = Dis_FileSelectMany(Hz_UsbListCnt[0][Lang], file_fdt, file_list[0]);
   //没有选择任何文件退出
   if (file_tol == 0) {
      return;
   }

   num = 0;
   while (1) {
      CloseFile(USB);
      file_len = file_list[num][28] + file_list[num][29] * 0x100 + file_list[num][30] * 0x10000 + file_list[num][31] * 0x1000000;

      while (1) {
         //文件长度不符合要求----------
         if ((file_len != (SIZE_LibFont16 * 2)) && (file_len != (SIZE_LibFont20 * 2))) {
            Dis_HintForm(HZ_TiSi[0][Lang], Hz_FontLibErr[0][Lang], C_Black, C_Back);
            break;
         }

         flag = OpenFileFloder(file_list[num], USB);
         if (flag > 0) {
            Dis_HintForm(HZ_CuoWu[0][Lang], HZ_FileOptHint[FILE_OpenErr][Lang], C_Black, C_Back);
            break;
         }

         count = file_len / NAND_PageB;
         if (file_len % NAND_PageB) count++;

         //字库为16*16点阵------
         if (file_len == (SIZE_LibFont16 * 2)) {
            addr = ADDR_LibFont16;
            for (i = 0; i < count; i++) {
               flag = ReadFile(NAND_PageB, Hz_Lib + i * NAND_PageB, USB);
               if (flag > 0) i = count + 1;
               Dis_SpeedInfo(HZ_ItemFileOpt[0][Lang], i + 1, count, 300, C_Back);
            }
            if (flag > 0) break;

            for (i = 0; i < 4; i++) {
               LibFlash_BlockErase(ADDR_LibFont16 + i * (1024 * 32));
            }
            for (i = 0; i < 64; i++) {
               addr = i * (NOR_SectorWords * 2);
               LibFlash_Write(ADDR_LibFont16 + i * NOR_SectorWords, (uint16 *)(Hz_Lib + addr), NOR_SectorWords);
               Dis_SpeedInfo(HZ_ItemFileOpt[1][Lang], i + 1, 64, 300, C_Back);
            }
            Fm_WriteUseBackup(ADDR_FM_Font, 0x55);
            Sys.LcdRefAll = 1;
         }
         //字库为20*20点阵-----
         else if (file_len == (SIZE_LibFont20 * 2)) {
            for (i = 0; i < count; i++) {
               flag = ReadFile(NAND_PageB, Hz_Lib20 + i * NAND_PageB, USB);
               if (flag > 0) i = count + 1;
               Dis_SpeedInfo(HZ_ItemFileOpt[0][Lang], i + 1, count, 300, C_Back);
            }
            if (flag > 0) break;

            for (i = 0; i < 10; i++) {
               LibFlash_BlockErase(ADDR_LibFont20 + i * (1024 * 32));
            }
            for (i = 0; i < 104; i++) {
               addr = i * (NOR_SectorWords * 2);
               LibFlash_Write(ADDR_LibFont20 + i * NOR_SectorWords, (uint16 *)(Hz_Lib20 + addr), NOR_SectorWords);
               Dis_SpeedInfo(HZ_ItemFileOpt[1][Lang], i + 1, 64, 300, C_Back);
            }
            Sys.LcdRefAll = 1;
         }
         break;
      }

      num++;
      if (num >= file_tol) break;
   }

   Dis_HintForm(HZ_TiSi[0][Lang], HZ_OperateEnd[0][Lang], C_Black, C_Back);
   CloseFile(USB);
   Key.Value = KEY_No;
}

//===================================================================
//功能: 			调入位图文件(小图片)
//参数:
//返回:
//说明:			图片格式为大端模式(高位在前)
//             			图片在内部的文件名固定
//修改时间:		2013-1-9
//===================================================================
void File_LoadBmp(void) {
   const char *Hz_UsbListCnt[][LANG_MAX] = { "图片文件输入到内存", "Load image file" };
   const char *Hz_LoadIcon[][LANG_MAX] = { "正在载入图标文件...", "Load icon..." };
   enum {MEM_SIZE = 1024 * 800};
   uint32 i, j;
   uint32 m, n, c;
   uint8 file_list[MAX_Files][32];
   uint8 file_fdt[32];
   uint32 file_tol;
   uint32 count;
   uint32 flag;
   uint32 bmp_type;
   uint32 file_len;
   uint32 addr;
   uint8 *mem;
   file_fdt[0] = 'B';
   file_fdt[1] = 'M';
   file_fdt[2] = 'P';
   file_tol = Dis_FileSelectMany(Hz_UsbListCnt[0][Lang], file_fdt, file_list[0]);
   //没有选择任何文件退出
   if (file_tol == 0) {
      return;
   }

   mem = MemApplication(mem, MEM_SIZE);
   if (mem == 0) {
      return;
   }

   count = 0;
   CloseFile(USB);
   flag = EnumMassDevSys(NAND_LIB);
   while (1) {
      //判断BMP文件的图片类型
      file_len = file_list[count][28] + file_list[count][29] * 0x100 + file_list[count][30] * 0x10000 + file_list[count][31] * 0x1000000;
      bmp_type = 0xff;
      //判断当前图片的类型---------------
      for (j = 0; j < MAX_BmpType; j++) {
         for (i = 0; i < 8; i++) {
            if (file_list[count][i] != BmpTypes[j][i]) {
               break;
            }
         }
         if (i >= 8) {
            bmp_type = j;
            if (bmp_type >= BMP_Icon) {
               bmp_type = BMP_Icon;
            }
            break;
         }
      }

      while (1) {
         //图片是有效的功能(40*40)位图--------------
         if (bmp_type == BMP_TypeFunc) {
            flag = OpenFileFloder(file_list[count], USB);
            if (flag > 0) break;
            file_len = MyFileInfo[USB].LengthInByte;
            if (file_len != (SIZE_LibBmpFunc * 2)) break;
            m = file_len / NAND_PageB;
            if (file_len % NAND_PageB) m++;

            for (i = 0; i < m; i++) {
               n = i * NAND_PageB;
               flag = ReadFile(NAND_PageB, mem + n, USB);
               if (flag > 0) i = m;
               Dis_SpeedInfo(HZ_ItemFileOpt[0][Lang], i + 1, m, 240, C_Back);
            }
            CloseFile(USB);
            if (flag > 0) break;

            for (i = 0; i < SIZE_LibBmpFunc; i++) {
               BmpFunc[i] = mem[i * 2] + (mem[i * 2 + 1] << 8);
            }
            //以块(32KW)为单位擦除-----------
            n = SIZE_LibBmpFunc / (1024 * 32);
            if (SIZE_LibBmpFunc % (1024 * 32)) n++;
            addr = ADDR_LibBmpFunc;
            for (i = 0; i < n; i++) {
               LibFlash_BlockErase(addr);
               addr += (1024 * 32);
            }
            //
            n = SIZE_LibBmpFunc / NOR_SectorWords;
            if (SIZE_LibBmpFunc % NOR_SectorWords) n++;
            addr = ADDR_LibBmpFunc;
            for (i = 0; i < n; i++) {
               if ((i + 1) == n) c = SIZE_LibBmpFunc - i * NOR_SectorWords;
               else c = NOR_SectorWords;
               LibFlash_Write(addr, BmpFunc + i * NOR_SectorWords, c);
               Dis_SpeedInfo(HZ_ItemFileOpt[1][Lang], i + 1, n, 240, C_Back);
               addr += NOR_SectorWords;
            }
            Sys.LcdRefAll = 1;
         }
         //图标式界面-----------
         else if (bmp_type == BMP_Icon) {
            Dis_HintBox(HZ_TiSi[0][Lang], Hz_LoadIcon[0][Lang], 1, C_Black, C_Back);
            flag = OpenFileFloder(file_list[count], NAND_LIB);
            if (flag == 0) {
               CloseFile(NAND_LIB);
               flag = DeleteFileFloder(file_list[count], NAND_LIB);
            }
            flag = OpenFileFloder(file_list[count], USB);
            flag = ReadFile(file_len, mem, USB);

            flag = CreateFile(file_list[count], file_len, NAND_LIB);
            if (flag == 0) {
               flag = WriteFile(file_len, mem, NAND_LIB);
            }
            CloseFile(NAND_LIB);
            CloseFile(USB);
            LcdMemPop();

            IconFlag.LoadOk[ICON_Usb] = 0;
            IconFlag.LoadOk[ICON_JiQi] = 0;
            IconFlag.LoadOk[ICON_Mem] = 0;
            IconFlag.LoadOk[ICON_ParaAdd] = 0;
            IconFlag.LoadOk[ICON_ParaFile] = 0;
            IconFlag.LoadOk[ICON_Work] = 0;
            IconFlag.LoadOk[ICON_WeiHu] = 0;
         }
         //图片文件不符合要求------------------------
         else {
            Dis_HintForm(HZ_TiSi[0][Lang], HZ_BmpFileErr[0][Lang], C_Black, C_Back);
         }
         break;
      }

      count++;
      if (count >= file_tol) break;
   }

   Dis_HintForm(HZ_TiSi[0][Lang], HZ_OperateEnd[0][Lang], C_Black, C_Back);
   CloseFile(USB);
   Key.Value = KEY_No;
   free(mem);
}

//===================================================================
//功能:			从U盘中导入LOGO图片
//说明:			图片存储按大端模式(高位在前)
//修改时间:		2013-4-17
//===================================================================
void File_LoadPic(void) {
   const char *HZ_UsbListPic[][LANG_MAX] = { "厂标图片输入到内存", "Load logo picture" };
   enum {MEM_SIZE = 1024 * 800};
   uint32 i, j;
   uint8 file_fdt[32];
   uint8 file_list[MAX_Files][32];
   uint32 file_tol;                            //选中的文件数
   uint32 count;
   uint32 num;
   uint32 flag;
   uint32 m, n;
   uint8 buf[BASE_Byte];
   uint16 temp[NOR_SectorWords];
   uint32 file_len;
   uint32 addr;
   uint8 *mem;
   file_fdt[0] = 'P';
   file_fdt[1] = 'I';
   file_fdt[2] = 'C';
   file_tol = Dis_FileSelectMany(HZ_UsbListPic[0][Lang], file_fdt, file_list[0]);
   //没有选择任何文件退出
   if (file_tol == 0) {
      return;
   }

   mem = MemApplication(mem, MEM_SIZE);
   if (mem == 0) {
      return;
   }

   num = 0;
   while (1) {
      CloseFile(USB);
      file_len = file_list[num][28] + file_list[num][29] * 0x100 + file_list[num][30] * 0x10000 + file_list[num][31] * 0x1000000;

      while (1) {
         //厂标图片处理(小端模式)-------------------
         if (file_len == SIZE_LibBmpLogo * 2) {
            flag = OpenFileFloder(file_list[num], USB);
            if (flag > 0) {
               Dis_HintForm(HZ_CuoWu[0][Lang], HZ_FileOptHint[FILE_OpenErr][Lang], C_Black, C_Back);
               break;
            }

            if (Sys.DisMainLogo == 1) Sys.LcdRefAll = 1;
            //计算要擦除的块数(32K字一块)---
            m = SIZE_LibBmpLogo / (32 * 1024);
            if (SIZE_LibBmpLogo % (32 * 1024)) m++;
            addr = ADDR_LibBmpLogo;
            for (i = 0; i < m; i++) {
               LibFlash_BlockErase(addr);
               addr += 32 * 1024;
            }

            count = file_len / BASE_Byte;
            if (file_len % BASE_Byte) count++;
            //每次读取-----------------
            for (i = 0; i < count; i++) {
               flag = ReadFile(NAND_PageB, buf, USB);
               if (flag > 0) i = count;
               //将图片数据存入厂标缓存---------
               for (j = 0; j < NAND_PageB / 2; j++) {
                  temp[j] = buf[j * 2] + buf[j * 2 + 1] * 256;
                  m = i * (NAND_PageB / 2) + j;
                  n = m % LCD_XSize;
                  m = m / LCD_XSize;
#if(LCD_SIGNAL_Fan==0)
                  Lcd_Logo[m][n] = temp[j];
#else
                  Lcd_Logo[m][n] = ~temp[j];
#endif
                  //若到最后一个数据则强制退出-----
                  if ((m >= (LOGO_PicH - 1)) && (n >= (LOGO_PicW - 1))) {
                     break;
                  }
               }
               addr = ADDR_LibBmpLogo + (NOR_SectorWords / 2) * i;
               LibFlash_Write(addr, temp, NOR_SectorWords / 2);
               Dis_SpeedInfo(HZ_JinDu[0][Lang], i + 1, count, 220, C_Gray);
            }
         }
         //部底图片处理(小端模式)-----------------------
         else if (file_len == SIZE_LibBottomLogo * 2) {
            flag = OpenFileFloder(file_list[num], USB);
            if (flag > 0) {
               Dis_HintForm(HZ_CuoWu[0][Lang], HZ_FileOptHint[FILE_OpenErr][Lang], C_Black, C_Back);
               break;
            }

            Sys.LcdRefAll = 1;
            //计算要擦除的块数(32K字一块)---
            m = SIZE_LibBottomLogo / (32 * 1024);
            if (SIZE_LibBottomLogo % (32 * 1024)) m++;
            addr = ADDR_LibBottomLogo;
            for (i = 0; i < m; i++) {
               LibFlash_BlockErase(addr);
               addr += 32 * 1024;
            }

            count = file_len / BASE_Byte;
            if (file_len % BASE_Byte) count++;
            //每次读取-----------------
            for (i = 0; i < count; i++) {
               flag = ReadFile(NAND_PageB, buf, USB);
               if (flag > 0) i = count;
               addr = ADDR_LibBottomLogo + (NAND_PageB / 2) * i;
               for (j = 0; j < NAND_PageB / 2; j++) {
                  temp[j] = buf[j * 2] + buf[j * 2 + 1] * 256;
                  m = i * (NAND_PageB / 2) + j;
                  n = m % PIC_BottomW;
                  m = m / PIC_BottomW;
#if(LCD_SIGNAL_Fan==0)
                  Lcd_BottomLogo[m][n] = temp[j];
#else
                  Lcd_BottomLogo[m][n] = ~temp[j];
#endif
                  //若到最后一个数据则强制退出-----
                  if ((m >= (PIC_BottomH - 1)) && (n >= (PIC_BottomW - 1))) {
                     break;
                  }
               }
               LibFlash_Write(addr, temp, NOR_SectorWords / 2);
               Dis_SpeedInfo(HZ_JinDu[0][Lang], i + 1, count, 220, C_Gray);
            }
         } else {
            Dis_HintForm(HZ_CuoWu[0][Lang], HZ_BmpFileErr[0][Lang], C_Black, C_Back);
            break;
         }

         break;
      }

      num++;
      if (num >= file_tol) break;
   }

   Dis_HintForm(HZ_TiSi[0][Lang], HZ_OperateEnd[0][Lang], C_Black, C_Back);
   CloseFile(USB);
   Key.Value = KEY_No;
   free(mem);
}

//===================================================================
//功能:			从U盘中载入帮助文件
//参数:
//返回:
//说明:			帮助文件为标准txt文件
//修改时间:		2013-1-9
//===================================================================
void File_LoadHelp(void) {
   const char *Hz_UsbListCnt[][LANG_MAX] = { "帮助文件输入到内存", "Load image file" };
   enum {MEM_SIZE = 1024 * 200};
   char type[3] = { 'H', 'E', 'L', };
   uint32 type_ok;
   uint32 i;
   uint8 file_list[50][32];
   uint8 file_fdt[32];
   uint32 file_tol;
   uint32 count;
   uint32 flag;
   uint32 file_len;
   uint32 err;
   uint8 *mem;
   flag = UsbApplication(1);
   if (flag > 0) return;

   file_fdt[0] = 'T';
   file_fdt[1] = 'X';
   file_fdt[2] = 'T';
   file_tol = Dis_FileSelectMany(Hz_UsbListCnt[0][Lang], file_fdt, file_list[0]);
   //没有选择任何文件退出
   if (file_tol == 0) {
      return;
   }

   mem = MemApplication(mem, MEM_SIZE);
   if (mem == 0) {
      return;
   }

   count = 0;
   CloseFile(USB);
   flag = EnumMassDevSys(NAND_LIB);
   while (1) {
      //判断当前图片的类型---------------
      for (i = 0; i < 3; i++) {
         if (file_list[count][i] != type[i]) {
            break;
         }
      }
      if (i >= 3) type_ok = 1;
      else type_ok = 0;

      err = 0;

      while (1) {
         if (type_ok == 0) {
            err = FILE_TypeErr;
         }
         //文件格式正确处理----------------
         else {
            CloseFile(USB);
            CloseFile(NAND_LIB);

            flag = OpenFileFloder(file_list[count], NAND_LIB);
            if (flag == 0) {
               CloseFile(NAND_LIB);
               flag = DeleteFileFloder(file_list[count], NAND_LIB);
            }
            flag = OpenFileFloder(file_list[count], USB);
            file_len = MyFileInfo[USB].LengthInByte;
            //文件大小超出范围-----
            if (file_len > MEM_SIZE) {
               err = FILE_SizeErr;
               break;
            }
            flag = ReadFile(file_len, mem, USB);

            flag = CreateFile(file_list[count], file_len, NAND_LIB);
            if (flag == 0) {
               flag = WriteFile(file_len, mem, NAND_LIB);
            }
         }
         break;
      }

      //错误提示----------------------
      if (err > 0) {
         Dis_HintForm(HZ_TiSi[0][Lang], HZ_FileOptHint[err][Lang], C_Black, C_Back);
      }

      count++;
      if (count >= file_tol) break;
   }

   Dis_HintForm(HZ_TiSi[0][Lang], HZ_OperateEnd[0][Lang], C_Black, C_Back);
   CloseFile(USB);
   CloseFile(NAND_LIB);
   Key.Value = KEY_No;
   free(mem);
}

uint32 Send_DspProgramDat(uint32 bag, uint8 *dat) {
   uint32 i;
   uint32 flag;
   TxBuf[4] = bag / 256;
   TxBuf[5] = bag % 256;
   for (i = 6; i < 512 + 6; i++) TxBuf[i] = *dat++;
   flag = Send_WaitResp(CMD_DspProgramDat, LEN_DspProgramDat, 0x55, RESP_500MS);
   return (flag);
}

//===================================================================
//功能:			DSP程序升级发送数据处理
//说明:			修改进入BOOT命令
//修改时间:		2012-6-7
//===================================================================
uint32 Do_DspUpdata(uint8 *dat, uint32 file_len) {
   const char *Hz_ItemDspErr[][LANG_MAX] = {
      "主板未进入升级状态!", "Not in boot status!",
      "主板程序擦除异常!", "Program earse error!",
      "通讯错误,主板无回码!", "Communication error!",
      "主板编程错误!", "Program error!",
   };
   const char *Hz_DspEarse[][LANG_MAX] = { "擦除", "Earse" };
   const char *Hz_DspProgramErr[][LANG_MAX] = { "编程错误!", "Program error!" };
   const char *Hz_DspProgramOk[][LANG_MAX] = { "编程结束!", "Program ok!" };
   uint32 i, j;
   uint32 m;
   uint32 flag;
   uint32 step;
   uint32 send;
   uint32 error;
   uint32 count;
   uint32 boot_stick;
   uint32 sure_tick;
   uint32 go_boot_error;                   //表示新命令错误
   error = 0;
   step = 1;
   send = 1;
   boot_stick = 0;
   go_boot_error = 0;

   while (1) {
      Read_Key();
      if (Key.Value == KEY_ESC) {
         break;
      }

      if (step == 3) {
         if (Get_SysTick(&sure_tick, 40)) {
            TxBuf[4] = 0x55;
            Net_SendCmd(CMD_DspRunStatus, LEN_DspRunStatus);
         }

         if (Get_SysTick(&boot_stick, 2000)) {
            step = 0;
            Dis_HintForm(HZ_CuoWu[0][Lang], Hz_ItemDspErr[0][Lang], C_Black, C_Back);
            break;
         }
      }

      if (NetRec.Cmd_Full > 0) {
         //表示DSP已进入BOOT程序------
         if ((NetRec.Command == CMD_DspRunStatus) && (NetRec.Cmd_Dat[0] == 0x55)) {
            step = 4;
            DelayMs(10);
         }
         //DSP发送擦除信息----
         else if (NetRec.Command == CMD_DspEarseStatus) {
            step = 5;
            i = NetRec.Cmd_Dat[0];
            if (i == 0xaa) {
               Dis_HintForm(HZ_CuoWu[0][Lang], Hz_ItemDspErr[1][Lang], C_Black, C_Back);
               SysFlag.DspUpdataErr = 0x55;
               break;
            } else {
               if (i >= 100) {
                  step = 6;
                  DelayMs(100);
               }
            }
            Dis_SpeedInfo(Hz_DspEarse[0][Lang], i, 100, 240, C_Back);
         }
         NetRec.Cmd_Full = 0;
      }

      if (send > 0) {
         send = 0;
         if (step == 1) TxBuf[4] = 0x55;
         else if (step == 2) TxBuf[4] = 0xaa;
         if (go_boot_error == 0) {
            flag = Send_WaitResp(CMD_DspToBoot, LEN_DspToBoot, 0, RESP_100MS * 3);
            if (flag > 0) {
               go_boot_error = 1;
            }
         }
         //若新的升级命令无效则用老的升级命令----
         if (go_boot_error > 0) {
            flag = Send_WaitResp(CMD_DspToBootOld, LEN_DspToBoot, 0, RESP_100MS * 3);
         }

         if (flag == 0) {
            if (step == 1) {
               step = 2;
               DelayMs(10);
               send = 1;
            } else {
               step = 3;
               boot_stick = Sys.Count5Ms;
               send = 0;
            }
         } else {
            error = 1;
            DelayMs(1);
            Dis_HintForm(HZ_CuoWu[0][Lang], Hz_ItemDspErr[2][Lang], C_Black, C_Back);
            break;
         }
      }

      //发送擦除DSP用户区FLASH指令-----
      if (step == 4) {
         DelayMs(500);
         step = 5;
         flag = Send_WaitResp(CMD_DspEarseSector, LEN_DspEarseSector, 0, RESP_2S);
         if ((flag > 0) || (NetRec.Hint == 0xaa)) {
            step = 0;
            BEEP_ON;
            error = 1;
            DelayMs(200);
            BEEP_OFF;
            Dis_HintForm(HZ_CuoWu[0][Lang], Hz_DspProgramErr[0][Lang], C_Black, C_Back);
            SysFlag.DspUpdataErr = 0x55;
            break;
         }
      }

      if (step == 6) {
         //主板升级后机头需重新复位运行------
         //清复位成功标志------
         SysFlag.ResetOk = 0;
         Fm_Write1Byte(ADDR_FM_RunReset, 0);
         //复位循环----
         RunVal.F2_Loop = 0;
         Fm_Write1Byte(ADDR_FM_FirstLoop, 0);

         step = 0;
         count = file_len / 512;
         if (file_len % 512) count++;
         for (i = 0; i < count; i++) {
            if ((i + 1) == count) {
               m = file_len % 512;
               if (m > 0) {
                  for (j = m; j < 512; j++) {
                     dat[j] = 0;
                  }
               }
            }
            flag = Send_DspProgramDat(i + 1, dat + i * 512);
            if (flag > 0) {
               i = count;
            }
            Dis_SpeedInfo(HZ_JinDu[0][Lang], i + 1, count, 280, C_Back);
            DelayMs(10);
         }
         if (flag == 0) {
            TxBuf[4] = 0x55;
            flag = Send_WaitResp(CMD_DspProgramEnd, LEN_DspProgramEnd, 0, RESP_500MS);
            Recod_AddNew(MESS_DspUpdate);       //写入记录表
            Dis_HintForm(HZ_TiSi[0][Lang], Hz_DspProgramOk[0][Lang], C_Black, C_Back);
            error = 0;
            SysFlag.DspUpdataErr = 0;
            break;
         } else {
            if (flag == 1) {
               Dis_HintForm(HZ_CuoWu[0][Lang], HZ_TongXunErr[0][Lang], C_Black, C_Back);
            } else {
               Dis_HintForm(HZ_CuoWu[0][Lang], Hz_DspProgramErr[0][Lang], C_Black, C_Back);
            }
            SysFlag.DspUpdataErr = 0x55;
            error = 1;
            break;
         }
      }
   }
   return (error);
}

//===================================================================
//功能: 			获取主板升级成老程序的升级密码
//参数:
//		code:		配置文件中的升级密码
//		config:		配置文件
//返回:			0表示成功
//说明:
//修改时间:		2012-8-13
//===================================================================
uint32 Get_DspUpdataOldCode(uint8 *code, uint8 *config) {
#define ADDR_CodeFlag			58			//有效标志的地址
#define ADDR_CodeStart			59			//密码区的起始地址
#define ADDR_CodeHint			65			//密码提示的地址
   uint32 i;
   uint32 flag;
   if (config[ADDR_CodeFlag] != 0x55) {
      return (1);
   }

   //检查密码是否符合要求-----
   flag = 0;
   for (i = 0; i < 6; i++) {
      code[i] = config[ADDR_CodeStart + i];
      if (code[i] > 0xf) {
         flag = 1;
         break;
      }
   }

   code[6] = config[ADDR_CodeHint];

   return (flag);
}

//===================================================================
//功能: 			主板程序升级处理
//说明:			根据升级文件信息分别处理
//					若升级成老的主板程序需要输入配置文件中的密码
//修改时间:		2012-8-13
//===================================================================
void File_DspProgramUpdata(void) {
   enum {UPDATA_FlagAddr = 0x22};
   const char *Hz_DspUpdata[][LANG_MAX] = { "【主板程序升级】", "Program Updata" };
   const char *Hz_DspUpdataFileErr[][LANG_MAX] = { "主板升级文件不符合要求!", "Updata file error!" };
   uint32 i, j;
   uint8 file_fdt[32];
   uint8 dat[512];
   uint32 count;
   uint32 flag;
   uint32 file_len;
   uint32 error;
   uint8 code[7];
   uint8 config[CONFIG1_FileLen];      //主板的配置文件数据
   uint8 *mem;
   if (LockVal[0].Use == 0x55) {
      flag = Dis_SuperPassWord(UpdataCode);
   } else {
      flag = Dis_PassWord(SYS_Code);
   }
   if (flag != 1) {
      return;
   }
   flag = UsbApplication(1);
   if (flag > 0) return;

   file_fdt[0] = 'M';
   file_fdt[1] = 'I';
   file_fdt[2] = 'N';
   flag = Dis_FileSelectOne(Hz_DspUpdata[0][Lang], file_fdt, USB);
   //没有选择任何文件退出
   if (flag == DO_Cancel) {
      return;
   }

   mem = MemApplication(mem, 1024 * 300);
   if (mem == 0) {
      return;
   }

   count = 0;
   error = 0;
   CloseFile(USB);
   while (1) {
      flag = OpenFileFloder(file_fdt, USB);
      if (flag > 0) {
         error = 1;
         Dis_HintForm(HZ_CuoWu[0][Lang], HZ_FileOptHint[FILE_OpenErr][Lang], C_Black, C_Back);
         break;
      }
      file_len = MyFileInfo[USB].LengthInByte;

      count = file_len / 512;
      if (file_len % 512) count++;
      for (i = 0; i < count; i++) {
         flag = ReadFile(512, dat, USB);
         if (flag > 0) {
            flag = SetFilePoint(i * 512, USB);
            flag = ReadFile(512, dat, USB);
         }
         if (flag > 0) i = count;
         //将数据进行解密----
         for (j = 0; j < 256; j++) {
            mem[i * 512 + j] = dat[j] ^ ProgramTable[j];
            mem[i * 512 + j + 256] = dat[j + 256] ^ ProgramTable[j];
         }
         Dis_SpeedInfo(HZ_ItemFileOpt[0][Lang], i + 1, count, 260, C_Back);
      }

      if (flag > 0) {
         error = 1;
         Dis_HintForm(HZ_CuoWu[0][Lang], HZ_FileOptHint[FILE_ReadErr][Lang], C_Black, C_Back);
      }
      break;
   }
   CloseFile(USB);

   //升级文件不是新程序处理-----
   if ((mem[UPDATA_FlagAddr] != '4') || (mem[UPDATA_FlagAddr + 1] != 'D')) {
      error = 1;

      //判断配置文件是否正确------
      if (Sys.MatchError != 0x55) {
         //获取主板配置文件信息-----
         flag = PeiDui_GetDspConfig(config);
         if (flag == 0) {
            flag = Get_DspUpdataOldCode(code, config);
            if (flag == 0) {
               error = 2;
            }
         }
      }
   }

   //不允许升级成老程序-----
   if (error == 1) {
      Dis_HintForm(HZ_CuoWu[0][Lang], Hz_DspUpdataFileErr[0][Lang], C_Black, C_Back);
   }
   //需要输入密码才能升级成老程序-----
   else if (error == 2) {
      flag = Dis_SuperPassWord(code);
      if (flag > 0) {
         error = 0;
      }
   }

   //文件读取正确处理-----------------------------
   if (error == 0) {
      flag = Do_DspUpdata(mem, file_len);
   }

   free(mem);
}

//===================================================================
//功能: 			显示器升级处理
//说明:
//修改时间:		2010-11-4
//===================================================================
void ArmProgramUpdate(void) {
   typedef void (*func)(void);
   uint32 i;
   uint32 flag;
   flag = Send_SysStatus(SYS_Free);
   if (LockVal[0].Use == 0x55) {
      flag = Dis_SuperPassWord(UpdataCode);
   } else {
      flag = Dis_PassWord(SYS_Code);
   }

   //若密码正确则进入BOOT程序-------
   if (flag == 1) {
      IrqDisable();
      LPC_LCD->CTRL = 0;
      LCD_BackOff;
      SysFlag.LcdBackOff = 0x55;            //置背景灭标志
      BEEP_OFF;
      ADDR_FM_Boot_Flag = 0X55AA;
      DelayMs(1500);
      SCB->VTOR = 0x00000000 & 0x3FFFFF80;

      asm("mov sp,#0x0");
      asm("ldr sp,[sp,#0]");
      register func user_code_entry;
      user_code_entry = *((func *)0x0 + 1);
      user_code_entry();
      for (i = 0; i < 1000000; i++) ;
      IrqEnable();
   }

   flag = Sys_StatusChang();
}

//===================================================================
//功能:			发送背包升级文件数据处理
//说明:			每包的编程数据为64个字节
//					若是最后一包则标志位置0x55
//修改时间:		2011-8-8
//===================================================================
uint32 Send_BagProgramDat(uint32 bags, uint32 bag, uint8 *dat) {
   uint32 i;
   uint32 flag;
   TxBuf[4] = bag / 256;
   TxBuf[5] = bag % 256;
   TxBuf[6] = 0;
   if (bag == bags) TxBuf[6] = 0x55;        //若是最后一包则标志位
   for (i = 7; i < (LEN_BagFrameLen + 7); i++) TxBuf[i] = *dat++;
   flag = Send_WaitResp(CMD_BagProgramDat, LEN_BagProgramDat, 0x55, RESP_1S);
   return (flag);
}

//===================================================================
//功能:			背包程序升级发送数据处理
//参数:
//		dat:			升级文件数据指针
//		file_len:		升级文件长度
//返回:
//说明:
//修改时间:		2013-1-9
//===================================================================
uint32 Do_BagUpdata(uint8 *dat, uint32 file_len) {
   const char *Hz_ItemBagErr[][LANG_MAX] = {
      "背包未进入升级状态!", "Not in boot status!",
      "背包程序擦除异常!", "Program earse error!",
      "通讯错误,主板无回码!", "Communication error!",
      "背包编程错误!", "Program error!",
   };
   const char *Hz_DspEarse[][LANG_MAX] = { "擦除", "Earse" };
   const char *Hz_DspProgramErr[][LANG_MAX] = { "编程错误!", "Program error!" };
   const char *Hz_DspProgramOk[][LANG_MAX] = { "编程结束!", "Program ok!" };
   uint32 i, j;
   uint32 m;
   uint32 flag;
   uint32 step;
   uint32 send;
   uint32 error;
   uint32 count;
   uint32 boot_stick;
   uint32 sure_tick;
   error = 0;
   step = 1;
   send = 1;
   boot_stick = 0;
   while (1) {
      Read_Key();
      if (Key.Value == KEY_ESC) {
         break;
      }

      if (step == 3) {
         if (Get_SysTick(&sure_tick, 40)) {
            TxBuf[4] = 0x55;
            Net_SendCmd(CMD_BagRunStatus, LEN_BagRunStatus);
         }

         if (Get_SysTick(&boot_stick, 2000)) {
            step = 0;
            Dis_HintForm(HZ_CuoWu[0][Lang], Hz_ItemBagErr[0][Lang], C_Black, C_Back);
            break;
         }
      }

      if (NetRec.Cmd_Full > 0) {
         //表示背包已进入BOOT程序------
         if ((NetRec.Command == CMD_BagRunStatus) && (NetRec.Cmd_Dat[0] == 0x55)) {
            step = 4;
            DelayMs(10);
         }
         //DSP发送擦除信息----
         else if (NetRec.Command == CMD_BagEarseStatus) {
            step = 5;
            i = NetRec.Cmd_Dat[0];
            if (i == 0xaa) {
               Dis_HintForm(HZ_CuoWu[0][Lang], Hz_ItemBagErr[1][Lang], C_Black, C_Back);
               break;
            } else {
               if (i >= 100) {
                  step = 6;
                  DelayMs(100);
               }
            }
            Dis_SpeedInfo(Hz_DspEarse[0][Lang], i, 100, 240, C_Back);
         }
         NetRec.Cmd_Full = 0;
      }

      if (send > 0) {
         send = 0;
         if (step == 1) {
            TxBuf[4] = 0x55;
            flag = Send_WaitResp(CMD_BagToBoot, LEN_BagToBoot, 0, RESP_2S);
         } else if (step == 2) {
            flag = 0;
         }

         if (flag == 0) {
            if (step == 1) {
               step = 2;
               DelayMs(100);
               send = 1;
            } else {
               step = 0;
               step = 3;
               boot_stick = Sys.Count5Ms;
               send = 0;
            }
         } else {
            error = 1;
            DelayMs(1);
            Dis_HintForm(HZ_CuoWu[0][Lang], Hz_ItemBagErr[2][Lang], C_Black, C_Back);
            break;
         }
      }

      //发送擦除DSP用户区FLASH指令-----
      if (step == 4) {
         DelayMs(500);
         step = 5;
         flag = Send_WaitResp(CMD_BagEarseSector, LEN_BagEarseSector, 0, RESP_2S);
         if ((flag > 0) || (NetRec.Hint == 0xaa)) {
            step = 0;
            BEEP_ON;
            error = 1;
            DelayMs(200);
            BEEP_OFF;
            Dis_HintForm(HZ_CuoWu[0][Lang], Hz_DspProgramErr[0][Lang], C_Black, C_Back);
            break;
         }
      }

      if (step == 6) {
         step = 0;
         //计算发送的总包数----
         count = file_len / LEN_BagFrameLen;
         if (file_len % LEN_BagFrameLen) count++;
         for (i = 0; i < count; i++) {
            if ((i + 1) == count) {
               m = file_len % LEN_BagFrameLen;
               if (m > 0) {
                  for (j = m; j < LEN_BagFrameLen; j++) {
                     dat[i * LEN_BagFrameLen + j] = 0;
                  }
               }
            }
            flag = Send_BagProgramDat(count, i + 1, dat + i * LEN_BagFrameLen);
            if (flag > 0) {
               i = count;
            }
            Dis_SpeedInfo(HZ_JinDu[0][Lang], i + 1, count, 280, C_Back);
            DelayMs(10);
         }
         if (flag == 0) {
            TxBuf[4] = 0x55;
            flag = Send_WaitResp(CMD_BagProgramEnd, LEN_BagProgramEnd, 0, RESP_500MS);
            Dis_HintForm(HZ_TiSi[0][Lang], Hz_DspProgramOk[0][Lang], C_Black, C_Back);
            error = 0;
            Recod_AddNew(MESS_BagUpdate);       //写入记录表
            break;
         } else {
            if (flag == 1) {
               Dis_HintForm(HZ_CuoWu[0][Lang], HZ_TongXunErr[0][Lang], C_Black, C_Back);
            } else {
               Dis_HintForm(HZ_CuoWu[0][Lang], Hz_DspProgramErr[0][Lang], C_Black, C_Back);
            }
            error = 1;
            break;
         }
      }
   }
   return (error);
}

//===================================================================
//功能: 			背包程序升级处理
//修改时间:		2011-7-28
//===================================================================
void File_BagProgramUpdata(void) {
   const char *Hz_BagUpdata[][LANG_MAX] = { "【背包程序升级】", "Head Program Updata" };
   uint32 i;
   uint8 file_fdt[32];
   uint32 count;
   uint32 flag;
   uint32 file_len;
   uint32 error;
   uint8 *mem;
   flag = Dis_PassWord(SYS_Code);

   if (flag != 1) {
      return;
   }
   flag = UsbApplication(1);
   if (flag > 0) return;

   file_fdt[0] = 'B';
   file_fdt[1] = 'S';
   file_fdt[2] = 'G';
   flag = Dis_FileSelectOne(Hz_BagUpdata[0][Lang], file_fdt, USB);
   //没有选择任何文件退出
   if (flag == DO_Cancel) {
      return;
   }

   mem = MemApplication(mem, 1024 * 300);
   if (mem == 0) {
      return;
   }

   count = 0;
   error = 0;
   CloseFile(USB);
   while (1) {
      flag = OpenFileFloder(file_fdt, USB);
      if (flag > 0) {
         error = 1;
         Dis_HintForm(HZ_CuoWu[0][Lang], HZ_FileOptHint[FILE_OpenErr][Lang], C_Black, C_Back);
         break;
      }
      file_len = MyFileInfo[USB].LengthInByte;

      count = file_len / 512;
      if (file_len % 512) count++;
      for (i = 0; i < count; i++) {
         flag = ReadFile(512, mem + i * 512, USB);
         if (flag > 0) {
            flag = SetFilePoint(i * 512, USB);
            flag = ReadFile(512, mem + i * 512, USB);
         }
         if (flag > 0) i = count;
         Dis_SpeedInfo(HZ_ItemFileOpt[0][Lang], i + 1, count, 260, C_Back);
      }

      if (flag > 0) {
         error = 1;
         Dis_HintForm(HZ_CuoWu[0][Lang], HZ_FileOptHint[FILE_ReadErr][Lang], C_Black, C_Back);
      }
      break;
   }
   CloseFile(USB);

   //文件读取正确处理-----------------------------
   if (error == 0) {
      flag = Do_BagUpdata(mem, file_len);
   }

   free(mem);
}

//===================================================================
//功能: 从加解密文件中获取相关信息
//dat     :  加解密文件数据的首地址
//jm       : 返回计算后的相关信息
//===================================================================
void Get_LockInfo(uint8 *dat, S_LockVal *jm) {
   uint32 i;
   uint8 buf[16];
   for (i = 0; i < 16; i++) {
      buf[i] = *(dat + 32 + i) ^ *(dat + 48 + i);
   }
   jm->Days = buf[3] + (uint16)buf[4] * 256;
   jm->ID = buf[7] + (uint32)buf[8] * 0x100 + (uint32)buf[9] * 0x10000 + (uint32)buf[10] * 0x1000000;
   jm->Year = buf[11] + (uint16)buf[12] * 256;
   jm->Month = buf[13];
   jm->Day = buf[15];
   if (jm->Year == 0) jm->Use = 0;
   else jm->Use = 0x55;
}

const char LockFile_Head[8] = { 'K', 'E', 'Y', ':', '1', '0', '0', '1' };
//===================================================================
//功能: 检查密鈅文件是否有效(256字节)
//说明: 若年月日不符合规范则返回0
//返回: 0表示无效文件,其它表示加密类型
//===================================================================
uint32 Check_LockFile(uint8 *dat) {
   uint32 i;
   uint32 type;
   type = 1;
   for (i = 0; i < 8; i++) {
      if (dat[i] != LockFile_Head[i]) {
         type = 0;
         break;
      }
   }

   if (type == 0) {
      return (0);
   }
   Get_LockInfo(dat, &LockVal[1]);
   if (((LockVal[1].Year > 0) && (LockVal[1].Year < 2009)) || (LockVal[1].Year > 2050)) type = 0;
   else if (LockVal[1].Month > 12) type = 0;
   else if (LockVal[1].Day > 31) type = 0;
   return (type);
}

//===================================================================
//功能:			从铁电存储器中读取时间锁文件
//参数:
//		dat:			时间锁文件数据指针
//返回: 			0表示文件无效,其它表示有效
//说明:
//修改时间:		2012-5-2
//===================================================================
uint32 Read_LockFileFromFram(uint8 *dat) {
   uint32 i;
   uint32 flag;
   flag = 0;
   //读取时间锁文件数据------------------
   for (i = 0; i < 2; i++) {
      Fm_ReadBytes(ADDR_FM_TimeLock, dat, 256);
      flag = Check_LockFile(dat);
      if (flag > 0) {
         flag = 1;
         break;
      }
   }
   //若存储区文件无效，则读备份区文件-----
   if (flag == 0) {
      for (i = 0; i < 2; i++) {
         Fm_ReadBytes(ADDR_FM_TimeLockF, dat, 256);
         flag = Check_LockFile(dat);
         if (flag > 0) {
            flag = 1;
            break;
         }
      }
   }
   return (flag);
}

//===================================================================
//功能: 			查看版本
//参数:
//返回:
//说明: 			显示配置文件信息
//修改时间:		2013-4-22
//===================================================================
void LockFile_SureTime(void) {
   enum {ASCW = 10,FONT = FONT24};
   const char *Hz_SureTime[][LANG_MAX] =
   {
      "请确认系统时间", "Check System Time",
      "系统时间", "System Time",
      "C:修改时间", "C:Edit Time",
      "确定返回", "Enter",
   };
   uint32 i;
   uint32 x, y;
   uint32 temp;
   uint16 edit_year = 0;
   uint8 edit_month = 0;
   uint8 edit_date = 0;
   uint8 edit_hour = 0;
   uint8 edit_minute = 0;
   uint32 color_b;
   uint32 k = 0;
   uint32 ref_time;
   uint32 ref_tick;
   uint32 edit;                        //不为0表示在修改时间状态
   volatile uint32 flag;
   uint32 kt[] = { KEY_C, KEY_OK };
   S_Form form;
   S_GuiXY guixy;
   //若已设置了分期付款则不允许修改时间--------
   if (LockVal[0].Use == 0x55) {
      return;
   }

   edit = 0;
   ref_time = 1;

   form.Width = 350;
   form.Height = 150;
   form.X = GET_ScrCenterX(form.Width);
   form.Y = GET_ScrCenterY(form.Height);
   form.TitleText = Hz_SureTime[0][Lang];
   form.TitleColor = FORM_Red;
   form.TitleHeight = TITLE_H24;
   form.TitleFont = FONT24;
   form.BackColor = C_Back;
   form.FillBack = 1;

   Touch_KeyPush();
   LcdMemPush(form.X, form.Y, form.Width, form.Height);
   Dis_WindowsForm(&form);
   Touch_KeyEn(form.X, form.Y, form.Width, form.TitleHeight, KEY_ESC, 2);
   TchVal.KeyNum[0] = 3;

   x = form.X + 15;
   y = form.Y + form.TitleHeight + 30;
   Dis_Strings(Hz_SureTime[1][Lang], x, y, FONT, C_Black, form.BackColor);

   guixy.BtnStartX = form.X + 20;
   guixy.BtnWidth = ASCW * 12;
   guixy.BtnPartWidth = GET_TchBtnPartW(form.Width, 20, guixy.BtnWidth, 2);
   x = guixy.BtnStartX;
   y = form.Y + form.Height - 50;
   for (i = 0; i < 2; i++) {
      Dis_TouchButton(Hz_SureTime[2 + i][Lang], x, y, guixy.BtnWidth, 35, kt[i], i, C_Black, FONT);
      x += guixy.BtnPartWidth;
   }

   while (1) {
      Send_DspReply();        //发送中断中置位的命令回码

      if (ref_time > 0) {
         ref_time = 0;
         //当前时间刷新--------------------------
         GetArmTime();

         x = form.X + 15 + ASCW * 12;
         y = form.Y + form.TitleHeight + 30;
         color_b = C_Yellow;

         //显示年
         Dis_Strings("20", x, y, FONT, C_Black, form.BackColor);
         if (edit > 0) temp = edit_year;
         else temp = Time.Year % 100;
         if ((k == 0) && (edit > 0)) Dis_NumAll(temp, x + ASCW * 2, y, 2, FONT, C_Black, color_b);
         else Dis_NumAll(temp, x + ASCW * 2, y, 2, FONT, C_Black, form.BackColor);
         Dis_Strings("-", x + ASCW * 4, y, FONT, C_Black, form.BackColor);
         //显示月
         if (edit > 0) temp = edit_month;
         else temp = Time.Month;
         if ((k == 1) && (edit > 0)) Dis_NumAll(temp, x + ASCW * 5, y, 2, FONT, C_Black, color_b);
         else Dis_NumAll(temp, x + ASCW * 5, y, 2, FONT, C_Black, form.BackColor);
         Dis_Strings("-", x + ASCW * 7, y, FONT, C_Black, form.BackColor);
         //日
         if (edit > 0) temp = edit_date;
         else temp = Time.Date;
         if ((k == 2) && (edit > 0)) Dis_NumAll(temp, x + ASCW * 8, y, 2, FONT, C_Black, color_b);
         else Dis_NumAll(temp, x + ASCW * 8, y, 2, FONT, C_Black, form.BackColor);

         x += ASCW * 11;
         //时
         if (edit > 0) temp = edit_hour;
         else temp = Time.Hour;
         if ((k == 3) && (edit > 0)) Dis_NumAll(temp, x, y, 2, FONT, C_Black, color_b);
         else Dis_NumAll(temp, x, y, 2, FONT, C_Black, form.BackColor);
         Dis_Strings(":", x + ASCW * 2, y, FONT, C_Black, form.BackColor);
         //分
         if (edit > 0) temp = edit_minute;
         else temp = Time.Minute;
         if ((k == 4) && (edit > 0)) Dis_NumAll(temp, x + ASCW * 3, y, 2, FONT, C_Black, color_b);
         else Dis_NumAll(temp, x + ASCW * 3, y, 2, FONT, C_Black, form.BackColor);
         Dis_Strings(":", x + ASCW * 5, y, FONT, C_Black, form.BackColor);
         //秒
         Dis_NumAll(Time.Second, x + ASCW * 6, y, 2, FONT, C_Black, form.BackColor);
      }

      if (Get_SysTick(&ref_tick, 40)) {
         ref_time = 1;
      }

      Read_Key();

      Dis_TouchButtonFlash();         //刷新模拟触摸键盘的变化状态

      if (k == 0) edit_year = Data_Input(49, 0, edit_year);
      else if (k == 1) edit_month = Data_Input(12, 1, edit_month);
      else if (k == 2) edit_date = Data_Input(31, 1, edit_date);
      else if (k == 3) edit_hour = Data_Input(23, 0, edit_hour);
      else if (k == 4) edit_minute = Data_Input(59, 0, edit_minute);

      if (edit > 0) {
         if (Key.Value == KEY_RIGHT) {
            k++;
            if (k >= 5) k = 0;
            ref_time = 1;
         } else if (Key.Value == KEY_LEFT) {
            if (k > 0) k--;
            else k = 4;
            ref_time = 1;
         }
      } else {
         //修改系统时间------------
         if (Key.Value == KEY_C) {
            edit = 1;
            edit_year = Time.Year % 100;        //年份的最高两位固定为20xx
            edit_month = Time.Month;
            edit_date = Time.Date;
            edit_hour = Time.Hour;
            edit_minute = Time.Minute;
            color_b = C_Yellow;

            TchVal.RefX = x;
            TchVal.RefY = y;
            TchVal.RefWidth = 100;
            TchVal.RefHeight = 20;
            Dis_TouchKeyBoard(TKEY_BoardStandMove);
         }
      }

      if (Key.Value == KEY_OK) {
         Sys.LcdRef = 1;
         if (edit > 0) {
            //将时间写入RTC芯片----
            WriteSysTime(edit_year + 2000, edit_month, edit_date, edit_hour, edit_minute, 0);
            Key.Value = KEY_No;
            TouchKeyBoardClose();
            Recod_AddNew(MESS_EditTime);        //写入记录表
            break;
         } else {
            Key.Value = KEY_ESC;
         }
      }

      if (Key.Value == KEY_ESC) {
         Key.Value = KEY_No;
         TouchKeyBoardClose();
         if (edit == 0) {
            break;
         } else {
            edit = 0;
         }
      }
   }

   Key.Value = KEY_No;
   Touch_KeyPop();
   LcdMemPop();
}

//===================================================================
//功能:			从U盘中读入密鈅文件(256字节)
//说明: 			增加了载入修改时间文件
//修改时间:		2012-11-20
//===================================================================
void File_LoadKey(void) {
   const char *Hz_KeyManage[][LANG_MAX] = { "【密匙管理】", "Key Manague" };
   uint8 file_fdt[32];
   uint32 flag;
   uint8 buf[256];
   uint32 status;          //操作状态1表示成功(取消加密)2表示成功(有加密),3表示失败
   uint32 jm_type;         //加密方式
   char new_hz[50];
   uint16 crc, crc1;
   //检测显示器程序与配置文件是否匹配,若不匹配则不能进行时间锁操作-----
   flag = PeiDui_Check();
   if (flag > 0) {
      return;
   }

   LockFile_SureTime();

   status = 0;
   file_fdt[0] = 'K';
   file_fdt[1] = 'E';
   file_fdt[2] = 'Y';
   flag = Dis_FileSelectOne(Hz_KeyManage[0][Lang], file_fdt, USB);
   //没有选择任何文件退出
   if (flag == DO_Cancel) {
      return;
   }

   CloseFile(USB);
   while (1) {
      flag = OpenFileFloder(file_fdt, USB);
      if ((flag > 0) || (MyFileInfo[USB].LengthInByte != 256)) {
         Dis_HintForm(HZ_CuoWu[0][Lang], HZ_FileOptHint[FILE_OpenErr][Lang], C_Black, C_Back);
         break;
      }

      flag = ReadFile(256, buf, USB);
      //读取加解密文件正确处理------------
      if (flag == 0) {
         //计算CRC校验是否正确-----
         crc = Get_CRC16(buf, LOCK_CRC_LEN);
         crc1 = buf[254];
         crc1 *= 256;
         crc1 += buf[255];
         //校验正确处理-----------
         if (crc == crc1) {
            jm_type = Check_LockFile(buf);

//若使用了加密狗则判断标志位-----
#ifdef DOG_USE
            if ((buf[8] != 'M') || (buf[9] != 'D')) jm_type = 0;
#endif
            //若不是时间锁文件则判断是否是修改时间文件-----
            if (jm_type == 0) {
               flag = TimeFile_Process(buf);
               //若为修改时间文件则退出-----
               if (flag == 0) {
                  status = 0;               //不提示时间锁的状态提示
                  break;
               }
            }

            //若加密类型为'KEY:1001'
            if (jm_type == 1) {
               //若之前为时间锁禁止状态---------
               if (LockVal[0].Use != 0x55) {
                  Get_LockInfo(buf, &LockVal[1]);
                  //若为解锁文件则禁止操作-----
                  if (LockVal[1].Year == 0) {
                     status = 3;
                  } else {
                     Fm_WriteBytes(ADDR_FM_TimeLock, buf, 256);
                     DelayMs(50);
                     Fm_WriteBytes(ADDR_FM_TimeLockF, buf, 256);
                     Get_LockInfo(buf, &LockVal[0]);
                     if (LockVal[0].Use == 0x55) status = 2;
                     else status = 1;
                  }
               }
               //若之前为时间锁有效状态处理-----------
               else {
                  Get_LockInfo(buf, &LockVal[1]);
                  //判断当前加密文件的时间是否比之前的小
                  flag = 0;
                  while (1) {
                     if (LockVal[1].ID != LockVal[0].ID) {flag = 1; break;}
                     if (LockVal[1].Year == 0) break;
                     if (LockVal[1].Year > LockVal[0].Year) break;
                     if (LockVal[1].Year < LockVal[0].Year) {flag = 1; break;}
                     if (LockVal[1].Month > LockVal[0].Month) break;
                     if (LockVal[1].Month < LockVal[0].Month) {flag = 1; break;}
                     if (LockVal[1].Day > LockVal[0].Day) break;
                     flag = 1;
                     break;
                  }
                  //比之前的大
                  if (flag == 0) {
                     Fm_WriteBytes(ADDR_FM_TimeLock, buf, 256);
                     DelayMs(50);
                     Fm_WriteBytes(ADDR_FM_TimeLockF, buf, 256);
                     Get_LockInfo(buf, &LockVal[0]);
                     if (LockVal[0].Use == 0x55) status = 2;
                     else status = 1;
                     SysFlag.DisableUpdata = 0;
                     Recod_AddNew(MESS_LoadKey);
                     Sys.TimeStop = 0;
                     Sys.TimeError = 0;
                  } else {
                     status = 3;
                  }
               }

               if ((status == 1) || (status == 2)) {
                  LockTimeProcess(0);         //重新判断是否到期
                  flag = Sys_StatusChang(); //保证解锁后进入运行状态
                                            //若时间锁文件导入成功则向下位机发送时间锁信息-----
                  Get_TimeLockInfo(TxBuf + 4);
                  flag = Send_WaitResp(CMD_WriteLockInfo, LEN_WriteLockInfo, 0, RESP_500MS);
                  PeiDui_Process();
               }
            }	else {
               status = 3;
            }
         } else {
            status = 3;
         }
      } else {
         Dis_HintForm(HZ_CuoWu[0][Lang], HZ_FileOptHint[FILE_ReadErr][Lang], C_Black, C_Back);
      }
      break;
   }

   if (status > 0) {
      //显示到期时间------
      if (status == 2) {
         Get_StringNum(new_hz, LockVal[0].Year, HZ_ItemKeyOpt[status - 1][Lang]);
         Get_StringString(new_hz, new_hz, "-");
         Get_StringNum(new_hz, LockVal[0].Month, new_hz);
         Get_StringString(new_hz, new_hz, "-");
         Get_StringNum(new_hz, LockVal[0].Day, new_hz);
      } else {
         Get_StringString(new_hz, (char *)(HZ_ItemKeyOpt[status - 1][Lang]), " ");
      }
      Dis_HintForm(HZ_TiSi[0][Lang], (const char *)(new_hz), C_Black, C_Back);
   }
   CloseFile(USB);
}

//===================================================================
//功能: 			查看版本
//参数:
//返回:
//说明: 			显示配置文件信息
//修改时间:		2013-3-7
//===================================================================
void VersionView(void) {
   enum {MAX = 3,ASCW = 10};
   const char *Hz_VerView[][LANG_MAX] = {
      "版本信息", "Version",
      "显示器:", "Monito:",
      "主板:", "Main:",
      "背包板:", "Bag:",
   };
   uint32 i;
   uint32 x, y;
   uint32 flag;
   uint32 dsp_len;
   uint32 bag_len;
   char dsp_ver[MAX_Version + 1];                //主板版本字符
   char bag_ver[MAX_Version + 1];                //背包板版本字符
   uint8 config[CONFIG1_FileLen];
   uint8 code[7];
   S_Form form;
   S_GuiXY guixy;
   form.Width = 320;
   form.Height = 250;
   form.X = (LCD_XSize - form.Width) >> 1;
   form.Y = (LCD_YSize - form.Height) >> 1;
   form.TitleText = Hz_VerView[0][Lang];
   form.TitleColor = FORM_Red;
   form.TitleHeight = TITLE_H35;
   form.TitleFont = FONT24;
   form.BackColor = C_Black;
   form.FillBack = 1;

   guixy.StartX = form.X + 20;
   guixy.StartY = form.Y + form.TitleHeight + 20;
   guixy.RowSpace = 32;

   LcdMemPush(form.X, form.Y, form.Width, form.Height);
   Touch_KeyPush();
   Dis_WindowsForm(&form);
   Touch_KeyEn(form.X, form.Y, form.Width, form.Height, KEY_ESC, 0);
   TchVal.KeyNum[0] = 1;

   x = guixy.StartX;
   y = guixy.StartY;
   for (i = 0; i < MAX; i++) {
      Dis_Strings(Hz_VerView[i + 1][Lang], x, y, FONT24, C_Blue, form.BackColor);
      y += guixy.RowSpace * 2;
   }

   x += 100;
   y = guixy.StartY;
   Dis_Strings(HZ_Version, x, y, FONT24, C_Yellow, form.BackColor);

   y += guixy.RowSpace;
   //显示配置文件信息-----
   while (1) {
      flag = Config1File_GetData(config);
      if (flag > 0) {
         break;
      }

      //将配置文件数据解密-----
      for (i = 0; i < CONFIG1_FileLen; i++) {
         config[i] ^= ProgramTable[i];
      }

      //判断厂名长度是否正确-----
      if ((config[1] == 0) || (config[1] > 30)) {
         break;
      }

      config[config[1] + 2] = 0;
      Dis_Strings(PTEXT(&config[2]), x, y, FONT24, C_Yellow, form.BackColor);
      break;
   }

   for (i = 0; i < (MAX_Version + 1); i++) {
      dsp_ver[i] = 0;
      bag_ver[i] = 0;
   }
   flag = Send_WaitResp(CMD_GetDspVer, LEN_GetDspVer, 0, RESP_1S);
   while (flag == 0) {
      dsp_len = NetRec.Cmd_Dat[0];
      if ((dsp_len > MAX_Version) || (dsp_len == 0)) break;      //若主板版本字符超长则认为所有版本无效
      bag_len = NetRec.Cmd_Dat[dsp_len + 1];
      //处理主板版本信息---------------------
      for (i = 0; i < dsp_len; i++) {
         dsp_ver[i] = NetRec.Cmd_Dat[1 + i];
      }
      //处理背包板版本信息-----------------
      if ((bag_len >= 3) && (bag_len <= MAX_Version)) {
         for (i = 0; i < bag_len; i++) {
            bag_ver[i] = NetRec.Cmd_Dat[dsp_len + 2 + i];
         }
      }
      //显示主板版本信息-----
      y += guixy.RowSpace;
      Dis_Strings(PTEXT(dsp_ver), x, y, FONT24, C_Yellow, form.BackColor);
      //获取主板配置文件信息-----
      flag = PeiDui_GetDspConfig(config);
      y += guixy.RowSpace;
      //显示主板配置文件厂名-----
      while (flag == 0) {
         //判断厂名长度是否正确-----
         if ((config[1] == 0) || (config[1] > 30)) {
            break;
         }

         config[config[1] + 2] = 0;
         Dis_Strings(PTEXT(&config[2]), x, y, FONT24, C_Yellow, form.BackColor);

         //显示主板升级成老程序的密码提示-----
         flag = Get_DspUpdataOldCode(code, config);
         if (flag == 0) {
            Dis_Strings("-", x + ASCW * (config[1] + 1), y, FONT24, C_Yellow, form.BackColor);
            Dis_NumHide(code[6], x + ASCW * (config[1] + 2), y, 3, FONT24, C_Yellow, form.BackColor);
         }
         break;
      }

      y += guixy.RowSpace;
      Dis_Strings(PTEXT(bag_ver), x, y, FONT24, C_Yellow, form.BackColor);
      break;
   }

   while (1) {
      Read_Key();
      if (Key.Value == KEY_ESC) {
         break;
      }
   }

   Touch_KeyPop();
   LcdMemPop();
   Key.Value = KEY_No;
}

//===================================================================
//功能:			U盘文件管理菜单
//修改时间:		2013-4-22
//===================================================================
void Menu_UDisk(void) {
   const char *Hz_FileSelect[][LANG_MAX] = { "U盘管理", "U-Disk" };
   const char *Hz_NotSetLock[][LANG_MAX] = { "到期时间未设置!", "Not set lock!" };
   const char *Hz_IconUsb[][LANG_MAX] = {
      "文件管理", "File Menu",
      "载入字库", "Load Font",
      "载入图标", "Load Icon",
      "载入厂标", "Load Logo",

      "载入帮助", "Load Help",
      "主板升级", "Dsp Update",
      "显示器升级", "Dis Update",
      "背包升级", "Head Updat",

      "密匙管理", "Secrity",
      "版本信息", "Version",
      "帮助文档", "View Help",
      "退出", "Exit",
   };
   const char *Hz_UpdataErr[][LANG_MAX] = { "即将到期,禁止升级程序!", "Program updata disable!" };
   enum {
      K_File = 0,K_Hz, K_Icon, K_Logo,
      K_LoadHelp, K_DspUpdate, K_ArmUpdate, K_BagUpdate,
      K_Code, K_Ver, K_Help, K_Exit
   };
   uint32 k;
   __VOL uint32 flag;
   uint32 updata;              //为0x55表示不管运行状态强制进入升级
   uint32 key_func;
   char new_hz[50];
   S_GuiXY guixy;
   S_GuiIcon icon;
   S_Form form;
   key_func = 0;
   updata = 0;
   form.TitleText = Hz_FileSelect[0][Lang];
   form.TitleColor = FORM_Blue;
   form.TitleHeight = TITLE_H24;
   form.TitleFont = FONT24;
   form.BackColor = C_Black;
   form.FillBack = 1;

   guixy.MaxIcons = 12;
   guixy.RowIcons = 4;
   guixy.ColSpace = ICON_Col;
   guixy.RowSpace = ICON_Row;

   icon.Width = ICON_PartW;
   icon.Height = ICON_PartH;
   icon.BmpW = ICON_W;
   icon.BmpH = ICON_H;
   icon.RectColor = C_Blue;
   icon.BackColor = form.BackColor;

   Get_IconFormPlace(&form, &guixy, &icon, 0);

   LoadIcon(ICON_Usb, IconUsb);
   LcdMemPush(form.X, form.Y, form.Width, form.Height);
   Touch_KeyPush();
   Sys.LcdRef = REF_Item;
   Sys.DatChang = 0;
   k = 0;
   Dis_WindowsForm(&form);
   Touch_KeyEn(form.X, form.Y, form.Width, form.TitleHeight, KEY_ESC, guixy.MaxIcons);

   TchVal.KeyNum[0] = guixy.MaxIcons + 1;

   while (Sys.TaskCode == TASK_FileManage) {
      Send_DspReply();        //发送中断中置位的命令回码

      Read_Key();

      Dis_TouchIconRef();                 //刷新图标的当前触摸状态

      k = KeyIconMove(&guixy, k);

      if (Sys.LcdRef > 0) {
         Sys.LcdRef = 0;
         Dis_IconRef(&guixy, &icon, Hz_IconUsb, IconUsb, k, 0);
      }

      if (TchVal.Down == TCH_KeySlide) {
         if (Key.Value == KEY_PU) {
            //光标在图标上滑动作当特殊处理------
            if ((k == K_ArmUpdate) || (k == K_Code) || (k == K_Ver)) {
               Key.Value = KEY_POP;
            }
            //输入主板配置文件-----
            else if (k == K_File) {
               key_func = 0x55;
               Key.Value = 2;
            }
         } else if (Key.Value == KEY_PD) {
            //输入显示器配置文件-----
            if (k == K_File) {
               key_func = 0x55;
               Key.Value = 1;
            }
         }
      }

      if (key_func == 0x55) {
         if ((Key.Value == KEY_1) || (Key.Value == KEY_2)) {
            key_func = 0;
            flag = Send_SysStatus(SYS_Free);
            ConfigFile_Set(Key.Value);
            flag = Sys_StatusChang();
         } else if (Key.Value != KEY_No) {
            key_func = 0;
         }
      }

      if (Key.Value == KEY_POP) {
         //在升级图标上按手指键忽略运行状态----
         if ((k == K_ArmUpdate) || (k == K_DspUpdate) || (k == K_Ver)) {
            updata = 0x55;
         } else if (k == K_File) {
            Key.Value = KEY_No;
            key_func = 0x55;
         }
         //在密匙管理图标上按手指键查看时间锁设置----
         else if (k == K_Code) {
            if (LockVal[0].Use == 0x55) {
               Get_StringNum(new_hz, LockVal[0].Year, HZ_ItemKeyOpt[1][Lang]);
               Get_StringString(new_hz, new_hz, "-");
               Get_StringNum(new_hz, LockVal[0].Month, new_hz);
               Get_StringString(new_hz, new_hz, "-");
               Get_StringNum(new_hz, LockVal[0].Day, new_hz);
               Get_StringString(new_hz, new_hz, (char *)(Hz_BianHao[0][Lang]));
               Get_StringNum(new_hz, LockVal[0].ID, new_hz);
               Dis_HintForm(HZ_TiSi[0][Lang], (const char *)(new_hz), C_Black, C_Back);
            } else {
               Dis_HintForm(HZ_TiSi[0][Lang], Hz_NotSetLock[0][Lang], C_Black, C_Back);
            }
         }
      }

      if (Key.Value == KEY_OK) {
         //需插上U盘才能进入--------------
         if (((k >= K_Hz) && (k <= K_LoadHelp)) || (k == K_Code)) {
            flag = UsbApplication(1);
         }

         //U盘已连接-----------------------------
         if (Sys.UsbConnect > 0) {
            if (k == K_Hz) {
               File_LoadFont();
            } else if (k == K_Icon) {
               File_LoadBmp();
            } else if (k == K_Logo) {
               File_LoadPic();
            } else if (k == K_LoadHelp) {
               File_LoadHelp();
            } else if (k == K_Code) {
               File_LoadKey();
            }
         }

         if (k == K_File) {
            File_UsbProcess();
         }
         //主板升级处理----
         else if (k == K_DspUpdate) {
            if ((SysFlag.MotoStop > 0) || (updata == 0x55)) {
               flag = Send_SysStatus(SYS_Free);
               File_DspProgramUpdata();
               flag = Sys_StatusChang();
            } else {
               Dis_HintForm(HZ_TiSi[0][Lang], HZ_OperateErr[0][Lang], C_Black, C_Back);   //提示机器正在运行
            }
         }
         //启动显示器自升级程序---------------------------
         else if (k == K_ArmUpdate) {
            //时间锁未到提示时间处理-----
            if (SysFlag.DisableUpdata != 0x55) {
               if ((SysFlag.MotoStop > 0) || (updata == 0x55)) {
                  ArmProgramUpdate();
               } else {
                  Dis_HintForm(HZ_TiSi[0][Lang], HZ_OperateErr[0][Lang], C_Black, C_Back);   //提示机器正在运行
               }
            }
            //时间锁到不允许升级显示器----
            else {
               Dis_HintForm(HZ_TiSi[0][Lang], Hz_UpdataErr[0][Lang], C_Black, C_Back);
            }
         }
         //背包升级处理----
         else if (k == K_BagUpdate) {
            if (SysFlag.MotoStop > 0) {
               flag = Send_SysStatus(SYS_Free);
               File_BagProgramUpdata();
               flag = Sys_StatusChang();
            } else {
               Dis_HintForm(HZ_TiSi[0][Lang], HZ_OperateErr[0][Lang], C_Black, C_Back);   //提示机器正在运行
            }
         }
         //查看版本信息-------------
         else if (k == K_Ver) {
            if ((SysFlag.MotoStop > 0) || (updata == 0x55)) {
               flag = Send_SysStatus(SYS_Free);
               VersionView();
               flag = Sys_StatusChang();
            } else {
               Dis_HintForm(HZ_TiSi[0][Lang], HZ_OperateErr[0][Lang], C_Black, C_Back);   //提示机器正在运行
            }
         }		else if (k == K_Help) {
            Dis_HelpFile(HELP_UDisk);
         }			else if (k == K_Exit) {
            Key.Value = KEY_ESC;
         }
      }

      if (Key.Value == KEY_ESC) {
         break;
      }
   }

   //若内存无花型则报警-------
   if (SysFlag.FileOk == 0) {
      Sys.ErrorCode = ERR_MemNoFile;
      while (1) {
         Read_Key();
         Error_Process(0);
         if (Sys.ErrorCode == 0) {
            break;
         }
      }
   }

   Touch_KeyPop();
   LcdMemPop();
   Sys.TaskCode = TASK_MainMenu;
   Key.Value = KEY_No;
}





