//fdisk 

    DWORD plist[] = {100, 0, 0, 0};  /* Divide drive into two partitions */
    BYTE work[_MAX_SS];
    f_fdisk(0, plist, work);  /* Divide physical drive 0 */
    f_mount(0, &Fatfs);
    f_mkfs(0, 0, 0);          /* Create an FAT volume on the logical drive 0. 2nd argument is ignored. */
    f_mount(0, 0);

//format partition
    FATFS fs;
    f_mount(0,  &fs);
    status = f_mkfs(0,0,4*512);
    if (status == 0) {
       mdDebug("driver 1 format success \r\n");
    }else{
       mdDebug("driver 1 format fail \r\n");
    }
//copy fist sector
    status = MMCSDP_CardInit(&mmcsdctr[0], MMCSD_CARD_AUTO);
    if (status == 0) {
       return  0;
    }
    status = MMCSDP_CardInit(&mmcsdctr[1], MMCSD_CARD_AUTO);
    if (status == 0) {
       return  0;
    }
    unsigned char buf[512];
    status = MMCSDP_Read(&mmcsdctr[0], buf, 0, 1);
    if (status == 0) {
       return  0;
    }
    status = MMCSDP_Write(&mmcsdctr[1],buf,0,1);
    if (status == 0) {
       return  0;
    }
    return  0;


//copy file 
    FATFS fs1,fs2;
    unsigned char buf[512];
    f_mount(0, &fs1);
    f_mount(3, &fs2);

   f_copy("3:/MLO", "0:/MLO", buf, sizeof buf);


    I2CInit(SOC_I2C_1_REGS,400000,0,0);
   //Rx8025Init();
   unsigned char h,m,s;
   Rx8025GetTime(&h,&m,&s);


   CAN_FRAME canFrame;
   canFrame.id = 0x512<<18;
   canFrame.xtd = 0;
   canFrame.data[0] = 0;
   canFrame.data[1] = 0x04;
   canFrame.dlc = 8;
   canFrame.dir = 0;
   CANRegistRcvedHandler(canrcvhandler);
   CANInit(SOC_DCAN_0_REGS,CAN_MODE_NORMAL, DCAN_IN_CLK,1000000);
   while (1) {
      CANSend_noblock(SOC_DCAN_0_REGS,&canFrame);
      while (ack == 0);
      ack = 0;
   }


   QEPInit(SOC_EQEP_2_REGS,100*1000*1000,QEP_MODE_QUAD);
   QEPSetPosFactor(1);
   QEPSetPosCompare(SOC_EQEP_2_REGS,3200);
   QEPVelocityDetectStart(SOC_EQEP_2_REGS,5*1000*1000);  


   TimerTickConfigure();
   TimerTickStart();


   unsigned char a[100] = "上海福建有震感";
   WCHAR b[128],c;
   c = strLen_UTF8(a);
   UTF8toUCS2_string(a, b, 100);


   MMCSDP_CtrlInfoInit(& mmcsdctr[0], SOC_MMCHS_0_REGS, 96000000, 24000000, MMCSD_BUSWIDTH_4BIT, 0,
                       &card0, HSMMCSDXferSetup,
                       HSMMCSDCmdStatusGet, HSMMCSDXferStatusGet);
   MMCSDP_CtrlInfoInit(& mmcsdctr[1], SOC_MMCHS_1_REGS, 96000000, 24000000, MMCSD_BUSWIDTH_4BIT, 0,
                       &card1, HSMMCSDXferSetup,
                       HSMMCSDCmdStatusGet, HSMMCSDXferStatusGet);
   MMCSDP_CtrlInit(& mmcsdctr[0]);
   MMCSDP_CtrlInit(& mmcsdctr[1]);

   while (1) {
    usbMscProcess();
    if(USBMSC_DEVICE_READY == g_usbMscState) {
      FIL file;
      unsigned char res = f_open(&file,"2:/YS_SinkT.ARA",FA_OPEN_EXISTING |FA_READ);
    }
   return 0;


//mmc test
   MMCSDP_CtrlInfoInit(& mmcsdctr[0], SOC_MMCHS_0_REGS, 96000000, 24000000, MMCSD_BUSWIDTH_4BIT, 0,
                       &card0, HSMMCSDXferSetup,
                       HSMMCSDCmdStatusGet, HSMMCSDXferStatusGet);
   MMCSDP_CtrlInit(& mmcsdctr[0]);
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



   //mmc test finish

//DDR TEST
      unsigned int *p = (unsigned int *)(0x80000000 + 32 * 1024 * 1024);

   for (int i = 0; i < (256 * 1024 * 1024 - 32 * 1024 * 1024) / 4; i++) {
      p[i] = 0x55555555;
      if (p[i] != 0x55555555)
          while (1);
   }
   for (int i = 0; i < (256 * 1024 * 1024 - 32 * 1024 * 1024) / 4; i++) {
      p[i] = 0xaaaaaaaa;
      if (p[i] != 0xaaaaaaaa)
          while (1);
   }

   //DDR TEST FINISH



   FRESULT f_fdiskEx (
	BYTE pdrv,			/* Physical drive number */
    unsigned int *beginLBA,
	const DWORD szt[],	/* Pointer to the size table for each partitions */
	void* work			/* Pointer to the working buffer */
)
{
	UINT i, n, sz_cyl, tot_cyl, b_cyl, e_cyl, p_cyl;
	BYTE s_hd, e_hd, *p, *buf = (BYTE*)work;
	DSTATUS stat;
	DWORD sz_disk, sz_part, s_part;


	stat = disk_initialize(pdrv);
	if (stat & STA_NOINIT) return FR_NOT_READY;
	if (stat & STA_PROTECT) return FR_WRITE_PROTECTED;
	if (disk_ioctl(pdrv, GET_SECTOR_COUNT, &sz_disk)) return FR_DISK_ERR;

	/* Determine CHS in the table regardless of the drive geometry */
	for (n = 16; n < 256 && sz_disk / n / 63 > 1024; n *= 2) ;
	if (n == 256) n--;
	e_hd = n - 1;
	sz_cyl = 63 * n;
	tot_cyl = sz_disk / sz_cyl;

	/* Create partition table */
	mem_set(buf, 0, _MAX_SS);
	p = buf + MBR_Table; b_cyl = *beginLBA/sz_cyl+1;*beginLBA = b_cyl*sz_cyl;
	for (i = 0; i < 4; i++, p += SZ_PTE) {
        if (szt[i]==-1UL) {
           i= 4; 
           p_cyl = tot_cyl- b_cyl;
        }else{
           p_cyl = (szt[i] <= 100) ? (DWORD)(tot_cyl-b_cyl )* szt[i] / 100 : szt[i] / sz_cyl;
        }
		if (!p_cyl) continue;
		s_part = (DWORD)sz_cyl * b_cyl;
		sz_part = (DWORD)sz_cyl * p_cyl;
		if (b_cyl == 0) {	/* Exclude first track of cylinder 0 */
			s_hd = 1;
			s_part += 63; sz_part -= 63;
		} else {
			s_hd = 0;
		}
		e_cyl = b_cyl + p_cyl - 1;
		if (e_cyl >= tot_cyl) return FR_INVALID_PARAMETER;

		/* Set partition table */
		p[1] = s_hd;						/* Start head */
		p[2] = (BYTE)((b_cyl >> 2) + 1);	/* Start sector */
		p[3] = (BYTE)b_cyl;					/* Start cylinder */
		p[4] = 0x06;						/* System type (temporary setting) */
		p[5] = e_hd;						/* End head */
		p[6] = (BYTE)((e_cyl >> 2) + 63);	/* End sector */
		p[7] = (BYTE)e_cyl;					/* End cylinder */
		ST_DWORD(p + 8, s_part);			/* Start sector in LBA */
		ST_DWORD(p + 12, sz_part);			/* Partition size */

		/* Next partition */
		b_cyl += p_cyl;
	}
	ST_WORD(p, 0xAA55);

	/* Write it to the MBR */
	return (disk_write(pdrv, buf, 0, 1) || disk_ioctl(pdrv, CTRL_SYNC, 0)) ? FR_DISK_ERR : FR_OK;
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


     __asm volatile ("DMB");