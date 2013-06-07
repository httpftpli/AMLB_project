/********************************************************************
����:				TFT��ʾGUI�����ļ�
�޸�ʱ��:		2013-3-15
********************************************************************/
#include "Head.h"
#include "Asc8x16_dot.h"

uint32 Lang;									//����ѡ�� 0=����,1=Ӣ��
uint8 Hz_Lib[1024*256];							//���ֿ�16*16����(ʵ��ռ��256K�ֽ�)
uint8 Hz_Lib20[1024*448];						//���ֿ�20*20����(ʵ��ռ��448K�ֽ�)
S_LcdMem LcdMem;

const char *HZ_SuRuMiMa[][LANG_MAX]={"����������:"," Password:"};
const char *Hz_InputDialog[][LANG_MAX]={"����Ի���","Input Dialog"};

//===================================================================
//����: 			��ʾ�����ڴ��ʼ��
//˵��: 			
//�޸�ʱ��:		2010-10-29
//===================================================================
void LcdMemInit(void)
{
uint32 i;
	LcdMem.Point=0;
	for(i=0;i<10;i++)
	{
		LcdMem.X[i]=0;
		LcdMem.Y[i]=0;
		LcdMem.Width[i]=0;
		LcdMem.Height[i]=0;
		LcdMem.Size[i]=0;
		LcdMem.Mem[i]=(void *)0;
	}
}

//===================================================================
//����: 			����һ�鱸���õ��Դ�
//��ڲ���:		
//		width:		���		
//		height:		�߶�
//˵��: 			����ڴ������Ϊ10��	

//�޸�ʱ��:		2013-3-6
//===================================================================
uint16 *LcdAppMem(uint32 x,uint32 y,uint32 width,uint32 height)
{
uint32 size;
uint32 addr;
uint32 tol;
	//��������Դ���������-----
	if(LcdMem.Point>=10)
	{
		return((void *)0);
	}

	size=(width*height)<<1;		//���뱸���Դ�Ĵ�С(�ֽ���)
	tol=0;
	addr=(uint32)LcdMemory;
	//������һ����Ϣ���㵱ǰ�����ľ�����ʼ��ַ----
	if(LcdMem.Point>0)
	{
		addr=(uint32)LcdMem.Mem[LcdMem.Point-1]+LcdMem.Size[LcdMem.Point-1];
	}

	tol=addr+size;					
	//�ж�������Դ���Ƿ񳬳���Χ------
	if(tol>(uint32)(LcdMemory+LCD_MAX_DisRam))
	{
		return((void *)0);
	}
	LcdMem.Point++;
	LcdMem.Mem[LcdMem.Point-1]=(uint16 *)(addr);
	LcdMem.X[LcdMem.Point-1]=x;
	LcdMem.Y[LcdMem.Point-1]=y;
	LcdMem.Width[LcdMem.Point-1]=width;
	LcdMem.Height[LcdMem.Point-1]=height;
	LcdMem.Size[LcdMem.Point-1]=size;
	return(LcdMem.Mem[LcdMem.Point-1]);
}

//===================================================================
//����: 			�ͷ��ϴα����õ��Դ�
//˵��: 			����ڴ������Ϊ10��	
//�޸�ʱ��:		2010-10-29
//===================================================================
void LcdFreeMem(void)
{
	if(LcdMem.Point>0)
	{
		LcdMem.Point--;
		LcdMem.Size[LcdMem.Point]=0;
		LcdMem.Mem[LcdMem.Point]=(void *)0;
	}
}

//===================================================================
//����: 			��ָ������ʾ�������ݴ��뱸���Դ�
//����:			
//		x,y:			ָ����ʾ�������ʼx,y����
//		width,height:	��ʾ����Ŀ�Ⱥ͸߶�
//˵��: 			
//�޸�ʱ��:		2013-3-15
//===================================================================
void LcdMemPush(uint32 x,uint32 y,uint32 width,uint32 height)
{
uint32 i;
uint16 *p;
	p=LcdAppMem(x,y,width,height);
	if(p==NULL) 
	{
		return;
	}
	for(i=0;i<height;i++)
	{
		memcpy((uint8 *)p,(uint8 *)&LcdBuffer[y+i][x],sizeof(uint16)*width);
		p+=width;
	}
}

//===================================================================
//����: 			�ӱ����Դ��лָ�֮ǰ����ʾ����
//˵��: 			
//�޸�ʱ��:		2013-3-15
//===================================================================
void LcdMemPop(void)
{
uint32 i;
uint32 x,y,width,height;
uint16 *p;
	if(LcdMem.Point==0) return;

	x=LcdMem.X[LcdMem.Point-1];
	y=LcdMem.Y[LcdMem.Point-1];
	width=LcdMem.Width[LcdMem.Point-1];
	height=LcdMem.Height[LcdMem.Point-1];
	p=LcdMem.Mem[LcdMem.Point-1];
	for(i=0;i<height;i++)
	{
		memcpy((uint8 *)&LcdBuffer[y+i][x],(uint8 *)p,sizeof(uint16)*width);
		p+=width;
	}
	LcdFreeMem();
}

//===================================================================
//����:			�����ο���
//����:
//		x:			��ʼ������
//		y:			��ʼ������
//		width:		���ο�Ķȶ�
//		heigh:		���ο�ĸ߶�
//		color:		��ɫ
//����:
//˵��:
//�޸�ʱ��:		2012-10-17
//===================================================================
void Dis_Rect(uint16 x,uint16 y,uint16 width,uint16 heigh,uint16 color)
{
	Dis_HLine(x,y,width,color);
	Dis_HLine(x,y+heigh-1,width,color);
	Dis_VLine(x,y,heigh,color);
	Dis_VLine(x+width-1,y,heigh,color);           
}   

//===================================================================
//����:			�����ο���,�߿���趨
//����:
//		x:			��ʼ������
//		y:			��ʼ������
//		width:		���ο�Ķȶ�
//		heigh:		���ο�ĸ߶�
//		line: 			�ߵ�����
//		color:		��ɫ
//����:
//˵��:
//�޸�ʱ��:		2012-10-17
//===================================================================
void Dis_RectSome(uint32 x,uint32 y,uint32 width,uint32 heigh,uint32 line,uint32 color)
{
uint32 i;
	for(i=0;i<line;i++)
	{
		Dis_HLine(x+i,y+i,width-i*2,color);				//����ĺ���
		Dis_HLine(x+i,y+heigh-1-i,width-i*2,color);		//����ĺ���
		Dis_VLine(x+i,y+i,heigh-i*2,color);				//��ߵ�����
		Dis_VLine(x+i+width-1-i*2,y+i,heigh-i*2,color);	//�ұߵ�����
	}
}   

//===================================================================
//����:			�����ľ��κ���
//����:
//		x:			��ʼX����
//		y:			��ʼY����
//		width:		���εĶȶ�
//		heigh:		���εĸ߶�
//		color:		��ɫ
//����:
//˵��:
//�޸�ʱ��:		2013-3-15
//===================================================================
void Dis_RectFill(uint16 x,uint16 y,uint16 width,uint16 heigh,uint16 color)
{
uint32 i;    
uint16 dat[LCD_XSize];
	for(i=0;i<width;i++) dat[i]=color;
	for(i=0;i<heigh;i++) 
	{
		memcpy((uint8 *)(&LcdBuffer[SECTION_Y(x, y)][SECTION_X(x, y)]),(uint8 *)dat,(sizeof(uint16))*width);
		y++;
	}  
}  

//=============================================================
//����:			������Բ�Ǿ���
//����: 
//		x :			Բ�Ǿ��ε���ʼx����
//		y :			Բ�Ǿ��ε���ʼy����
//		width:		Բ�Ǿ��εĿ��
//		height:		Բ�Ǿ��εĸ߶�
//		r :			Բ�ǵİ뾶
//		color:		��ɫ
//����: 
//˵��:
//�޸�ʱ��:		2012-10-17
//=============================================================
void Dis_CircleRect(int16 x,int16 y,uint32 width,uint32 height,uint32 r,uint16 color)
{
int a,b;
int di;
uint16 qua2_y0,qua3_y0;
uint16 qua3_x0,qua4_x0;
	x=x+r;
	y=y+r;
	width=width-2*r-1;
	height=height-2*r-1;
	a=0;
	b=r;
	di=3-2*r;
	while(a<=b)
	{
		//��һ����
		LCD_SetPixel(x-a,y-b,color);    
		LCD_SetPixel(x-b,y-a,color);             //3

		//�ڶ�����        
		qua2_y0=y+height;
		LCD_SetPixel(x-a,qua2_y0+b,color);
		LCD_SetPixel(x-b,qua2_y0+a,color);
		
		//��������        
		qua3_y0=y+height;
		qua3_x0=x+width;
		LCD_SetPixel(qua3_x0+a,qua3_y0+b,color);           
		LCD_SetPixel(qua3_x0+b,qua3_y0+a,color);             //4

		//��������
		qua4_x0=x+width;
		LCD_SetPixel(qua4_x0+a,y-b,color);               
		LCD_SetPixel(qua4_x0+b,y-a,color);        

		a++;
		if(di<0) 
		{
			di+=4*a+6;
		}
		else
		{
			di+=10+4*(a-b);
			b--;
		}
	}

	Dis_VLine(x-r,y,height,color);	
	Dis_VLine(x+r+width,y,height,color);
	Dis_HLine(x,y-r,width,color);
	Dis_HLine(x,y+height+r,width,color);
}

//=============================================================
//����:			��ʵ��Բ�Ǿ���
//����: 
//		x :			Բ�Ǿ��ε���ʼx����
//		y :			Բ�Ǿ��ε���ʼy����
//		width:		Բ�Ǿ��εĿ��
//		height:		Բ�Ǿ��εĸ߶�
//		r :			Բ�ǵİ뾶
//		color:		��ɫ
//����: 
//˵��:
//�޸�ʱ��:		2012-10-17
//=============================================================
void Dis_CircleRectFill(int16 x,int16 y,uint32 width,uint32 height,uint32 r,uint16 color)
{
int a,b;
int di;
uint32 qua2_y0,qua3_y0;
uint16 qua3_x0,qua4_x0;   
	x=x+r;
	y=y+r;
	width=width-2*r-1;
	height=height-2*r-1;
	a=0;
	b=r;
	di=3-2*r;
	while(a<=b)
	{
		//��һ����
		LCD_SetPixel(x-a,y-b,color);      
		LCD_SetPixel(x-b,y-a,color);             //3

		//�ڶ�����        
		qua2_y0=y+height;
		LCD_SetPixel(x-a,qua2_y0+b,color);     
		LCD_SetPixel(x-b,qua2_y0+a,color);            

		//��������        
		qua3_y0=y+height;
		qua3_x0=x+width;
		LCD_SetPixel(qua3_x0+a,qua3_y0+b,color);           
		LCD_SetPixel(qua3_x0+b,qua3_y0+a,color);             //4

		//��������
		qua4_x0=x+width;
		LCD_SetPixel(qua4_x0+a,y-b,color);                    
		LCD_SetPixel(qua4_x0+b,y-a,color);           

		Dis_HLine(x-a,y-b,(qua4_x0+a)-(x-a),color);
		Dis_HLine(x-b,y-a,(qua4_x0+b)-(x-b),color);

		Dis_HLine(x-a,qua2_y0+b,(qua3_x0+a)-(x-a),color);
		Dis_HLine(x-b,qua2_y0+a,(qua4_x0+b)-(x-b),color);

		a++;
		if(di<0) 
		{
			di+=4*a+6;
		}
		else
		{
			di+=10+4*(a-b);
			b--;
		}
	}

	Dis_VLine(x-r,y,height,color);
	Dis_VLine(x+r+width,y,height,color);
	Dis_HLine(x,y-r,width,color);
	Dis_HLine(x,y+height+r,width,color);
	Dis_RectFill(x-r+1,y,width+2*r-1,height,color);
}

//=============================================================
//����:			���н���Ч����ʵ��Բ�Ǿ���
//����: 
//		x :			Բ�Ǿ��ε���ʼx����
//		y :			Բ�Ǿ��ε���ʼy����
//		width:		Բ�Ǿ��εĿ��
//		height:		Բ�Ǿ��εĸ߶�
//		r :			Բ�ǵİ뾶
//		color:		��ɫ
//����: 
//˵��:
//�޸�ʱ��:		2012-10-17
//=============================================================
void Dis_CircleRectFillShade(int16 x,int16 y,uint32 width,uint32 height,uint32 r,uint16 color1,uint16 color2)
{
int a,b;
int di;
uint32 qua2_y0,qua3_y0;
uint16 qua3_x0,qua4_x0;   
	x=x+r;
	y=y+r;
	width=width-2*r-1;
	height=height-2*r-1;
	a=0;
	b=r;
	di=3-2*r;
	while(a<=b)
	{
		//��һ����
		LCD_SetPixel(x-a,y-b,color1);      
		LCD_SetPixel(x-b,y-a,color1);             //3

		//�ڶ�����        
		qua2_y0=y+height;
		LCD_SetPixel(x-a,qua2_y0+b,color1);     
		LCD_SetPixel(x-b,qua2_y0+a,color1);            

		//��������        
		qua3_y0=y+height;
		qua3_x0=x+width;
		LCD_SetPixel(qua3_x0+a,qua3_y0+b,color2);           
		LCD_SetPixel(qua3_x0+b,qua3_y0+a,color2);             //4

		//��������
		qua4_x0=x+width;
		LCD_SetPixel(qua4_x0+a,y-b,color2);                    
		LCD_SetPixel(qua4_x0+b,y-a,color2);           

		Dis_HLine(x-a,y-b,(qua4_x0+a)-(x-a),color1);
		Dis_HLine(x-b,y-a,(qua4_x0+b)-(x-b),color1);

		Dis_HLine(x-a,qua2_y0+b,(qua3_x0+a)-(x-a),color2);
		Dis_HLine(x-b,qua2_y0+a,(qua4_x0+b)-(x-b),color2);

		a++;
		if(di<0) 
		{
			di+=4*a+6;
		}
		else
		{
			di+=10+4*(a-b);
			b--;
		}
	}

	Dis_VLine(x-r,y,height,color1);
	Dis_VLine(x+r+width,y,height,color2);
	Dis_HLine(x,y-r,width,color1);
	Dis_HLine(x,y+height+r,width,color2);
//	Dis_RectFill(x-r+1,y,width+2*r-1,height,color1);

	Dis_RectFill(x-r+1,y,width+2*r-1,height/2,color1);
	Dis_RectFill(x-r+1,y+height/2,width+2*r-1,height/2+1,color2);
}

//=============================================================
//����:			������Բ
//����: 
//		x :			Բ�ĵ�x����
//		y :			Բ�ĵ�y����
//		r :			Բ�İ뾶
//		color:		��ɫ
//����: 
//˵��:
//�޸�ʱ��:		2012-10-10
//=============================================================
void Dis_Circle(uint32 x,uint32 y,uint32 r,uint16 color)
{
int dx,dy;
	if(r==0) return;
	dx=r;
	dy=1;
	
	while(dy<=dx)
	{
		LCD_SetPixel(x+dx,y+dy,color);
		LCD_SetPixel(x-dx,y+dy,color);
		LCD_SetPixel(x+dx,y-dy,color);
		LCD_SetPixel(x-dx,y-dy,color);
		if(dx!=dy)
		{
			LCD_SetPixel(x+dy,y+dx,color);
			LCD_SetPixel(x+dy,y-dx,color);
			LCD_SetPixel(x-dy,y+dx,color);
			LCD_SetPixel(x-dy,y-dx,color);
		}
		dy++;
		if((r*r-dy*dy)<=(dx-1)*dx)
		{
			dx--;
		}
	}
	LCD_SetPixel(x + r, y, color);
	LCD_SetPixel(x - r, y, color);
	LCD_SetPixel(x, y + r, color);
	LCD_SetPixel(x, y - r, color);
}

//=============================================================
//����:			��ʵ��Բ
//����: 
//		x :			Բ�ĵ�x����
//		y :			Բ�ĵ�y����
//		r :			Բ�İ뾶
//		color:		��ɫ
//����: 
//˵��:
//�޸�ʱ��:		2012-10-10
//=============================================================
void Dis_CircleFill(uint32 x,uint32 y,uint32 r,uint16 color)
{
int dx,dy,ref;
int Temp;
	if(r==0) return;
	dx=r;
	dy=1;
	ref=(707*r)/1000; 				// �ο�ֵΪ0.707r

	while(dy<=dx)
	{
		if(dx>ref)
		{
			Dis_Line(x + ref + 1, y + dy, x + dx, y + dy, color);
		}
		
		if(x>=(ref+1)&&(dx>ref))
		{
			if(x < dx)
				Temp = 0;
			else
				Temp = x - dx;
			Dis_Line(Temp, y + dy, x - ref - 1, y + dy, color);
		}

		if((y>=dy)&&(dx>ref))
			Dis_Line(x+ref + 1, y - dy, x + dx, y - dy, color);
		if((x>=(ref+1))&&(y>=dy)&&(dx>ref))
		{
			if(x<dx)
				Temp = 0;
			else
				Temp = x - dx;
			Dis_Line(Temp, y - dy, x - ref - 1, y - dy, color);
		}

		if((dy!=dx)||(dx==ref))
		{
			if(y<dx)
				Temp = 0;
			else
				Temp = y - dx;
			Dis_Line(x + dy, Temp, x + dy, y + dx, color);
		}

		if((x>=dy)&&((dy!= dx)||(dy==ref)))
		{
			if(y < dx)
				Temp = 0;
			else
				Temp = y - dx;
			Dis_Line(x - dy, Temp, x - dy, y + dx, color);
		} 
		dy++;
		if((r*r-dy*dy)<=(dx-1)*dx)
			dx--;
	}

			if(y < dx)
				Temp = 0;
			else
				Temp = y - dx;
			Dis_Line(x-dy,Temp,x-dy,y+dx,color);

	Dis_Line(x+ref+1,y,x+r,y,color);					//����Ҳ�ĺ���

	if(x>=(ref+1))
	{
		if(x<r)
			Temp = 0;
		else
			Temp = x - r;
		Dis_Line(Temp, y, x - ref - 1, y, color);		//������ĺ���
	}

	if(y< r)
		Temp = 0;
	else
		Temp=y-r;
	Dis_Line(x, Temp, x, y + r, color);				//����м������
}

//=============================================================
//����:			��������Բ
//����: 
//		x:			��Բ�ĵ�x����
//		y:			��Բ�ĵ�y����
//		a:			��Բ�Ŀ��
//		b:			��Բ�ĸ߶�
//		color:		��ɫ
//����: 
//˵��:
//�޸�ʱ��:		2012-10-10
//=============================================================
void Dis_Ellispse(int x,int y,int a,int b,uint16 color) 
{
int xx,yy;
float d1,d2;
	xx=0;
	yy=b;
       d1=b*b+a*a*(-b+0.25);   
       LCD_SetPixel(x+xx,y+yy,color);
       LCD_SetPixel(x-xx,y-yy,color);
       LCD_SetPixel(x-xx,y+yy,color);
       LCD_SetPixel(x+xx,y-yy,color);

       while((b*b*(xx+1))<(a*a*(yy-0.5)))
       {
              if(d1<=0)
              {
			d1+=b*b*(2*xx+3);
			xx++;
              }
              else
              {
			d1+=b*b*(2*xx+3)+a*a*(-2*yy+2);
			xx++;
			yy--;
              }
	       LCD_SetPixel(x+xx,y+yy,color);
	       LCD_SetPixel(x-xx,y-yy,color);
	       LCD_SetPixel(x-xx,y+yy,color);
	       LCD_SetPixel(x+xx,y-yy,color);
       }

       d2=b*b*(xx+0.5)*(xx+0.5)+a*a*(yy-1)*(yy-1)-a*a*b*b;

       while(yy>0)
       {
              if(d2<=0)
              {
			d2+=b*b*(2*xx+2)+a*a*(-2*yy+3);
			xx++;
			yy--;
              }
              else
              {
			d2+=a*a*(-2*yy+3);
			yy--;
              }

	       LCD_SetPixel(x+xx,y+yy,color);
	       LCD_SetPixel(x-xx,y-yy,color);
	       LCD_SetPixel(x-xx,y+yy,color);
	       LCD_SetPixel(x+xx,y-yy,color);
       }
}

//=============================================================
//����:			��ʵ����Բ
//����: 
//		x:			��Բ�ĵ�x����
//		y:			��Բ�ĵ�y����
//		a:			��Բ�Ŀ��
//		b:			��Բ�ĸ߶�
//����: 
//˵��:
//�޸�ʱ��:		2012-10-10
//=============================================================
void Dis_EllispseFill(int x,int y,int a,int b,uint16 color) 
{
int xx,yy;
float d1,d2;
	xx=0;
	yy=b;
       d1=b*b+a*a*(-b+0.25);   //�б�ʽ
       LCD_SetPixel(x+xx,y+yy,color);
       LCD_SetPixel(x-xx,y-yy,color);
       LCD_SetPixel(x-xx,y+yy,color);
       LCD_SetPixel(x+xx,y-yy,color);

       while((b*b*(xx+1))<(a*a*(yy-0.5)))
       {
              if(d1<=0)
              {
			d1+=b*b*(2*xx+3);
			xx++;
              }
              else
              {
			d1+=b*b*(2*xx+3)+a*a*(-2*yy+2);
			Dis_Line(x-xx,y-yy,x+xx,y-yy,color);
			Dis_Line(x-xx,y+yy,x+xx,y+yy,color);
			xx++;
			yy--;
              }
       }

	Dis_Line(x-xx,y-yy,x+xx,y-yy,color);
	Dis_Line(x-xx,y+yy,x+xx,y+yy,color);


       d2=b*b*(xx+0.5)*(xx+0.5)+a*a*(yy-1)*(yy-1)-a*a*b*b;

       while(yy>0)
       {
              if(d2<=0)
              {
			d2+=b*b*(2*xx+2)+a*a*(-2*yy+3);
			xx++;
			yy--;
              }
              else
              {
			d2+=a*a*(-2*yy+3);
			yy--;
              }

		Dis_Line(x-xx,y-yy,x+xx,y-yy,color);
		Dis_Line(x-xx,y+yy,x+xx,y+yy,color);
       }
}

//===================================================================
//����:			��ʾһ��ͼƬ
//����:
//		x,y:			��ʼ����
//		width:		ͼƬ�Ŀ��
//		height:		ͼƬ�ĸ߶�
//		bmp:		ͼƬ����ָ��
//�޸�ʱ��:		2013-4-17
//===================================================================
void Dis_Picture(uint32 x,uint32 y,uint32 width,uint32 height,uint16 *bmp)
{
uint32 i;
	for(i=y;i<height+y;i++) 
	{
		memcpy((uint8 *)(&LcdBuffer[SECTION_Y(x, i)][SECTION_X(x, i)]),(uint8 *)bmp,width*2);
		bmp+=width;
	}
}

//===================================================================
//����: ��ʾһ��ͼƬ(����ָ������ɫ)
//x         : ��ʼ������
//y         : ��ʼ������
//width   : ͼƬ�Ŀ��
//height  : ͼƬ�ĸ߶�
//*bmp   : ͼƬ����ָ��
//===================================================================
void Dis_PictureFilter(uint32 x,uint32 y,uint32 width,uint32 height,uint16 *bmp,uint32 filter)
{
uint32 i,j;
uint16 color;
	for (i=y;i<height+y;i++) 
	{
		for (j=x;j<width+x;j++) 
		{
			color=*bmp;
		       	if(color!=filter)
		       	{
			       	LCD_SetPixel(j,i,color);
		       	}
			bmp++;
 	   	}
	}
}

//===================================================================
//����: ��ʾһ��ͼƬ(����ָ������ɫ,����ɫΪָ��ͼƬ)
//x         : ��ʼ������
//y         : ��ʼ������
//width   : ͼƬ�Ŀ��
//height  : ͼƬ�ĸ߶�
//*bmp   : ͼƬ����ָ��
//===================================================================
void Dis_PictureFilterBottom(uint32 x,uint32 y,uint32 width,uint32 height,uint16 *bmp,uint32 filter)
{
uint32 i,j;
uint16 color;
uint16 colorb;
	for (i=y;i<height+y;i++) 
	{
		for (j=x;j<width+x;j++) 
		{
			color=*bmp;
		       	if(color!=filter)
		       	{
			       	LCD_SetPixel(j,i,color);
		       	}
			else
			{
				colorb=Lcd_BottomLogo[i-LOGO_BotmY][j];
				LCD_SetPixel(j,i,colorb);
			}
			bmp++;
 	   	}
	}
}

//===================================================================
//����: ��ʾ����ַ�
//x         : ��ʼ������
//y         : ��ʼ������
//color_f  : ��ʾ��ǰ��ɫ
//color_b : ��ʾ��ǰ��ɫ
//===================================================================
void Dis_8x16(uint8 num,uint16 x,uint16 y,uint16 color_f,uint16 color_b)
{
uint32 i,j;
uint32 addr;
	addr=num*16;
	for(i=0;i<16;i++)
	{
		for(j=0;j<8;j++)
		{
			if(Asc8x16[addr+i]&(0x80>>j))
			{
				LCD_SetPixel(x+j,y+i,color_f);
			}
			else
			{
				LCD_SetPixel(x+j,y+i,color_b);
			}
		}
	}
}

//===================================================================
//����: ��ʾ����ַ�
//x         : ��ʼ������
//y         : ��ʼ������
//color_f  : ��ʾ��ǰ��ɫ
//color_b : ��ʾ��ǰ��ɫ
//===================================================================
void Dis_8x16Logo(uint8 num,uint16 x,uint16 y,uint16 color_f)
{
uint32 i,j;
uint32 addr;
	addr=num*16;
	for(i=0;i<16;i++)
	{
		for(j=0;j<8;j++)
		{
			if(Asc8x16[addr+i]&(0x80>>j))
			{
				LCD_SetPixel(x+j,y+i,color_f);
			}
			else
			{
				LcdBuffer[y+i][x+j]=Lcd_BottomLogo[y-LOGO_BotmY+i][x-BOTTOM_LogoX+j];
			}
		}
	}
}

//===================================================================
//����: ��ʾ8*16�����ASC�ַ�(ASC���0��ʼ)
//x         : ��ʼ������
//y         : ��ʼ������
//color_f  : ��ʾ��ǰ��ɫ
//color_b : ��ʾ��ǰ��ɫ
//===================================================================
void Dis_Asc(uint8 num,uint16 x,uint16 y,uint16 color_f,uint16 color_b)
{
	if(num<32) return;
	num-=32;
	Dis_8x16(num,x,y,color_f,color_b);
}

//===================================================================
//�����ľ��κ���
//x:		��ʼ������
//y:		��ʼ������
//width:	���εĶȶ�
//heigh:	���εĸ߶�
//color:	�ߵ���ɫ
//===================================================================
void Dis_SuZi(uint8 num,uint16 x,uint16 y,uint16 font,uint16 color_f,uint16 color_b)
{
#define ASC_Num		16		//������ASC���е�λ��
uint32 i,j;
uint32 addr;
	if(font==FONT24) 
	{
		addr=(num+ASC_Num)*40;
		for(i=0;i<20;i++)
		{
			for(j=0;j<8;j++)
			{
				if(Asc10x20[addr+i*2]&(0x80>>j))
				{
					LCD_SetPixel(x+j,y+i,color_f);
				}
				else
				{
					LCD_SetPixel(x+j,y+i,color_b);
				}
			}
			for(j=0;j<2;j++)
			{
				if(Asc10x20[addr+i*2+1]&(0x80>>j))
				{
					LCD_SetPixel(x+j+8,y+i,color_f);
				}
				else
				{
					LCD_SetPixel(x+j+8,y+i,color_b);
				}
			}			
		}				
	}
	else 
	{
		addr=(num+ASC_Num)*16;
		for(i=0;i<16;i++)
		{
			for(j=0;j<8;j++)
			{
				if(Asc8x16[addr+i]&(0x80>>j))
				{
					LCD_SetPixel(x+j,y+i,color_f);
				}
				else
				{
					LCD_SetPixel(x+j,y+i,color_b);
				}
			}
		}		
	}
}

//===================================================================
//�����ľ��κ���
//x:		��ʼ������
//y:		��ʼ������
//width:	���εĶȶ�
//heigh:	���εĸ߶�
//color:	�ߵ���ɫ
//===================================================================
void Dis_SuZiLogo(uint8 num,uint16 x,uint16 y,uint16 color_f)
{
#define ASC_Num		16		//������ASC���е�λ��
uint32 i,j;
uint32 addr;
	addr=(num+ASC_Num)*16;
	for(i=0;i<16;i++)
	{
		for(j=0;j<8;j++)
		{
			if(Asc8x16[addr+i]&(0x80>>j))
			{
				LCD_SetPixel(x+j,y+i,color_f);
			}
			else
			{
				LcdBuffer[y+i][x+j]=Lcd_BottomLogo[y-LOGO_BotmY+i][x-BOTTOM_LogoX+j];
			}
		}
	}
}

//===================================================================
//����: ��ʾʮ�����Ƶ���ֵ(0~F)
//===================================================================
void Dis_Hex(uint8 num,uint16 x,uint16 y,uint16 color_f,uint16 color_b)
{
	if(num<=9) Dis_SuZi(num,x,y,FONT16,color_f,color_b);
	else if(num<=15) Dis_Asc('A'+num-10,x,y,color_f,color_b);
	else Dis_Asc('.',x,y,color_f,color_b);
}

//===================================================================
//����:			��ʾ16*16�����ַ�
//����:
//		num:		ȫ���ַ���λ��
//		x,y:			��ʾλ�õ�x,y����
//		color_f:		��ʾ��ǰ��ɫ
//		color_b:		��ʾ�ı���ɫ
//����:
//˵��:
//�޸�ʱ��:		2013-3-27
//===================================================================
void Dis_16x16(uint32 num,uint16 x,uint16 y,uint16 color_f,uint16 color_b)
{
uint32 i,j;
uint8 dat;
uint8 dot[32];
uint32 addr;
	addr=num*32;
	memcpy(dot,Hz_Lib+addr,32);
	for(i=0;i<16;i++)
	{
		dat=dot[i*2];
		for(j=0;j<8;j++)
		{
			if(dat&(0x80>>j))
			{
				LCD_SetPixel(x+j,y+i,color_f);
			}
			else
			{
				LCD_SetPixel(x+j,y+i,color_b);
			}
		}

		dat=dot[i*2+1];
		for(j=0;j<8;j++)
		{
			if(dat&(0x80>>j))
			{
				LCD_SetPixel(x+8+j,y+i,color_f);
			}
			else
			{
				LCD_SetPixel(x+8+j,y+i,color_b);
			}
		}
	}
}

//===================================================================
//�����ľ��κ���
//x:		��ʼ������
//y:		��ʼ������
//width:	���εĶȶ�
//heigh:	���εĸ߶�
//color:	�ߵ���ɫ
//===================================================================
void Dis_16x16Logo(uint32 num,uint16 x,uint16 y,uint16 color_f)
{
uint32 i,j;
uint8 dat;
uint8 dot[32];
uint32 addr;
	addr=num*32;
	memcpy(dot,Hz_Lib+addr,32);
	for(i=0;i<16;i++)
	{
		dat=dot[i*2];
		for(j=0;j<8;j++)
		{
			if(dat&(0x80>>j))
			{
				LCD_SetPixel(x+j,y+i,color_f);
			}
			else
			{
				LcdBuffer[y+i][x+j]=Lcd_BottomLogo[y-LOGO_BotmY+i][x-BOTTOM_LogoX+j];
			}
		}

		dat=dot[i*2+1];
		for(j=0;j<8;j++)
		{
			if(dat&(0x80>>j))
			{
				LCD_SetPixel(x+8+j,y+i,color_f);
			}
			else
			{
				LcdBuffer[y+i][x+8+j]=Lcd_BottomLogo[y-LOGO_BotmY+i][x-BOTTOM_LogoX+8+j];
			}
		}
	}
}

//===================================================================
//����:			��ʾ����ַ�(10*20����)
//����:
//		num:
//		x,y:			��ʾ����ʼ����
//		color_f:		��ʾ��ǰ��ɫ
//		color_b:		��ʾ�ı���ɫ
//����:
//˵��:
//�޸�ʱ��:		2013-4-16
//===================================================================
void Dis_10x20(uint8 num,uint16 x,uint16 y,uint16 color_f,uint16 color_b)
{
uint32 i,j;
uint32 addr;
	addr=num*40;
	for(i=0;i<20;i++)
	{
		for(j=0;j<8;j++)
		{
			if(Asc10x20[addr+i*2]&(0x80>>j))
			{
				LCD_SetPixel(x+j,y+i,color_f);
			}
			else
			{
				LCD_SetPixel(x+j,y+i,color_b);
			}
		}

		for(j=0;j<2;j++)
		{
			if(Asc10x20[addr+i*2+1]&(0x80>>j))
			{
				LCD_SetPixel(x+j+8,y+i,color_f);
			}
			else
			{
				LCD_SetPixel(x+j+8,y+i,color_b);
			}
		}		
	}
}

//===================================================================
//����:			��ʾ����ַ�(10*20),��ˢ����ɫ
//����:
//		num:
//		x,y:			��ʾ����ʼ����
//		color_f:		��ʾ��ǰ��ɫ
//		color_b:		��ʾ�ı���ɫ
//����:
//˵��:
//�޸�ʱ��:		2013-4-16
//===================================================================
void Dis_10x20Filter(uint8 num,uint16 x,uint16 y,uint16 color_f)
{
uint32 i,j;
uint32 addr;
	addr=num*40;
	for(i=0;i<20;i++)
	{
		for(j=0;j<8;j++)
		{
			if(Asc10x20[addr+i*2]&(0x80>>j))
			{
				LCD_SetPixel(x+j,y+i,color_f);
			}
		}

		for(j=0;j<2;j++)
		{
			if(Asc10x20[addr+i*2+1]&(0x80>>j))
			{
				LCD_SetPixel(x+j+8,y+i,color_f);
			}
		}		
	}
}

//===================================================================
//����:			��ʾ20*20�����ַ�
//����:
//		num:		ȫ���ַ���λ��
//		x,y:			��ʾλ�õ�x,y����
//		color_f:		��ʾ��ǰ��ɫ
//		color_b:		��ʾ�ı���ɫ
//����:
//˵��:
//�޸�ʱ��:		2013-4-16
//===================================================================
void Dis_20x20(uint32 num,uint16 x,uint16 y,uint16 color_f,uint16 color_b)
{
uint32 i,j;
uint8 dat;
uint8 dot[60];
uint32 addr;
	addr=num*60;
	memcpy(dot,Hz_Lib20+addr,60);
	for(i=0;i<20;i++)
	{
		dat=dot[i*3];
		for(j=0;j<8;j++)
		{
			if(dat&(0x80>>j))
			{
				LCD_SetPixel(x+j,y+i,color_f);
			}
			else
			{
				LCD_SetPixel(x+j,y+i,color_b);
			}
		}

		dat=dot[i*3+1];
		for(j=0;j<8;j++)
		{
			if(dat&(0x80>>j))
			{
				LCD_SetPixel(x+8+j,y+i,color_f);
			}
			else
			{
				LCD_SetPixel(x+8+j,y+i,color_b);
			}
		}

		dat=dot[i*3+2];
		for(j=0;j<4;j++)
		{
			if(dat&(0x80>>j))
			{
				LCD_SetPixel(x+16+j,y+i,color_f);
			}
			else
			{
				LCD_SetPixel(x+16+j,y+i,color_b);
			}
		}		
	}
}

//===================================================================
//����:			��ʾ20*20�����ַ�(��ˢ����ɫ)
//����:
//		num:		ȫ���ַ���λ��
//		x,y:			��ʾλ�õ�x,y����
//		color_f:		��ʾ��ǰ��ɫ
//����:
//˵��:
//�޸�ʱ��:		2013-4-16
//===================================================================
void Dis_20x20Filter(uint32 num,uint16 x,uint16 y,uint16 color_f)
{
uint32 i,j;
uint8 dat;
uint8 dot[60];
uint32 addr;
	addr=num*60;
	memcpy(dot,Hz_Lib20+addr,60);
	for(i=0;i<20;i++)
	{
		dat=dot[i*3];
		for(j=0;j<8;j++)
		{
			if(dat&(0x80>>j))
			{
				LCD_SetPixel(x+j,y+i,color_f);
			}
		}

		dat=dot[i*3+1];
		for(j=0;j<8;j++)
		{
			if(dat&(0x80>>j))
			{
				LCD_SetPixel(x+8+j,y+i,color_f);
			}
		}

		dat=dot[i*3+2];
		for(j=0;j<4;j++)
		{
			if(dat&(0x80>>j))
			{
				LCD_SetPixel(x+16+j,y+i,color_f);
			}
		}		
	}
}

//===================================================================
//����:			��ʾ����ַ�,����ˢ�ɵ�ǰ���������
//��ڲ���:		
//		x:			��ʼX����
//		y:			��ʼ������
//		color_f:		�ַ�����ɫ
//		form:		��ǰ����ṹָ��
//˵��:			
//�޸�ʱ��:		2010-11-16
//===================================================================
void Dis_8x16FilterTitle(uint8 num,uint16 x,uint16 y,uint16 color_f,S_Form *form)
{
uint32 i,j;
uint32 m;
uint32 addr;
uint16 titlecolor;
	if(((x+8)>LCD_XSize)||((y+16)>LCD_YSize)) return;

	addr=num*16;
	for(i=0;i<16;i++)
	{
		//ȡ����ǰY����Ĵ������������ɫ--------------
		m=y+i-form->Y;		//��ǰ������ڴ�����ʼY�����ֵ	
		//�������߶ȴ���24ʱ�ı���ɫ����------
		if(form->TitleHeight>24)
		{
			if(m<12)
			{
				titlecolor=FormTitleColor[form->TitleColor][m];
			}
			else if((form->TitleHeight-m)>=12)
			{
				titlecolor=FormTitleColor[form->TitleColor][12];
			}
			else
			{
				titlecolor=FormTitleColor[form->TitleColor][24-(form->TitleHeight-m)];
			}
		}
		else
		{
			titlecolor=FormTitleColor[form->TitleColor][m];
		}		
		
		for(j=0;j<8;j++)
		{
			if(Asc8x16[addr+i]&(0x80>>j))
			{
				LCD_SetPixel(x+j,y+i,color_f);
			}
			else
			{
				LCD_SetPixel(x+j,y+i,titlecolor);
			}
		}
	}
}

//===================================================================
//����:			��ʾ����ַ�(10*20),��ˢ����ɫ
//����:
//		num:
//		x,y:			��ʾ����ʼ����
//		color_f:		��ʾ��ǰ��ɫ
//		color_b:		��ʾ�ı���ɫ
//����:
//˵��:
//�޸�ʱ��:		2013-4-16
//===================================================================
void Dis_10x20FilterTitle(uint8 num,uint16 x,uint16 y,uint16 color_f,S_Form *form)
{
uint32 i,j;
uint32 addr;
uint32 m;
uint16 titlecolor;
	addr=num*40;
	for(i=0;i<20;i++)
	{
		//ȡ����ǰY����Ĵ������������ɫ--------------
		m=y+i-form->Y;		//��ǰ������ڴ�����ʼY�����ֵ	
		//�������߶ȴ���24ʱ�ı���ɫ����------
		if(form->TitleHeight>24)
		{
			if(m<12)
			{
				titlecolor=FormTitleColor[form->TitleColor][m];
			}
			else if((form->TitleHeight-m)>=12)
			{
				titlecolor=FormTitleColor[form->TitleColor][12];
			}
			else
			{
				titlecolor=FormTitleColor[form->TitleColor][24-(form->TitleHeight-m)];
			}
		}
		else
		{
			titlecolor=FormTitleColor[form->TitleColor][m];
		}		
	
		for(j=0;j<8;j++)
		{
			if(Asc10x20[addr+i*2]&(0x80>>j))
			{
				LCD_SetPixel(x+j,y+i,color_f);
			}
			else
			{
				LCD_SetPixel(x+j,y+i,titlecolor);
			}
		}

		for(j=0;j<2;j++)
		{
			if(Asc10x20[addr+i*2+1]&(0x80>>j))
			{
				LCD_SetPixel(x+j+8,y+i,color_f);
			}
			else
			{
				LCD_SetPixel(x+j+8,y+i,titlecolor);
			}
		}		
	}
}


//===================================================================
//����:			��ʾȫ���ַ�,����ˢ�ɵ�ǰ���������
//		x:			��ʼ������
//		y:			��ʼ������
//		width:		���εĶȶ�
//		heigh:		���εĸ߶�
//		color:		�ߵ���ɫ
//�޸�ʱ��:		2010-11-16
//===================================================================
void Dis_16x16FilterTitle(uint32 num,uint16 x,uint16 y,uint16 color_f,S_Form *form)
{
uint32 i,j;
uint32 m;
uint8 dat;
uint8 dot[32];
uint32 addr;
uint16 titlecolor;
	if(((x+16)>LCD_XSize)||((y+16)>LCD_YSize)) return;

	addr=num*32;
	memcpy(dot,Hz_Lib+addr,32);
	
	for(i=0;i<16;i++)
	{
		//ȡ����ǰY����Ĵ������������ɫ--------------
		m=y+i-form->Y;		//��ǰ������ڴ�����ʼY�����ֵ	
		//�������߶ȴ���24ʱ�ı���ɫ����------
		if(form->TitleHeight>24)
		{
			if(m<12)
			{
				titlecolor=FormTitleColor[form->TitleColor][m];
			}
			else if((form->TitleHeight-m)>=12)
			{
				titlecolor=FormTitleColor[form->TitleColor][12];
			}
			else
			{
				titlecolor=FormTitleColor[form->TitleColor][24-(form->TitleHeight-m)];
			}
		}
		else
		{
			titlecolor=FormTitleColor[form->TitleColor][m];
		}
		
		dat=dot[i*2];
		for(j=0;j<8;j++)
		{
			if(dat&(0x80>>j))
			{
				LCD_SetPixel(x+j,y+i,color_f);
			}
			else
			{
				LCD_SetPixel(x+j,y+i,titlecolor);
			}
		}

		dat=dot[i*2+1];
		for(j=0;j<8;j++)
		{
			if(dat&(0x80>>j))
			{
				LCD_SetPixel(x+8+j,y+i,color_f);
			}
			else
			{
				LCD_SetPixel(x+8+j,y+i,titlecolor);
			}
		}
	}
}

//===================================================================
//����:			��ʾ24*24�����ַ�(����ɫ�ñ�������ɫ���)
//����:
//		num:		ȫ���ַ���λ��
//		x,y:			��ʾλ�õ�x,y����
//		color_f:		��ʾ��ǰ��ɫ
//		form:		�������ݽṹָ��
//����:
//˵��:
//�޸�ʱ��:		2013-4-16
//===================================================================
void Dis_20x20FilterTitle(uint32 num,uint16 x,uint16 y,uint16 color_f,S_Form *form)
{
uint32 i,j;
uint8 dat;
uint8 dot[60];
uint32 addr;
uint32 m;
uint16 titlecolor;
	addr=num*60;
	memcpy(dot,Hz_Lib20+addr,60);
	for(i=0;i<20;i++)
	{
		//ȡ����ǰY����Ĵ������������ɫ--------------
		m=y+i-form->Y;		//��ǰ������ڴ�����ʼY�����ֵ	
		//�������߶ȴ���24ʱ�ı���ɫ����------
		if(form->TitleHeight>20)
		{
			if(m<12)
			{
				titlecolor=FormTitleColor[form->TitleColor][m];
			}
			else if((form->TitleHeight-m)>=12)
			{
				titlecolor=FormTitleColor[form->TitleColor][12];
			}
			else
			{
				titlecolor=FormTitleColor[form->TitleColor][24-(form->TitleHeight-m)];
			}
		}
		else
		{
			titlecolor=FormTitleColor[form->TitleColor][m];
		}
	
		dat=dot[i*3];
		for(j=0;j<8;j++)
		{
			if(dat&(0x80>>j))
			{
				LCD_SetPixel(x+j,y+i,color_f);
			}
			else
			{
				LCD_SetPixel(x+j,y+i,titlecolor);
			}
		}

		dat=dot[i*3+1];
		for(j=0;j<8;j++)
		{
			if(dat&(0x80>>j))
			{
				LCD_SetPixel(x+8+j,y+i,color_f);
			}
			else
			{
				LCD_SetPixel(x+8+j,y+i,titlecolor);
			}
		}

		dat=dot[i*3+2];
		for(j=0;j<4;j++)
		{
			if(dat&(0x80>>j))
			{
				LCD_SetPixel(x+16+j,y+i,color_f);
			}
			else
			{
				LCD_SetPixel(x+16+j,y+i,titlecolor);
			}
		}		
	}
}

//===================================================================
//����:			��ʾȫ���ַ�
//x:		��ʼ������
//y:		��ʼ������
//width:	���εĶȶ�
//heigh:	���εĸ߶�
//color:	�ߵ���ɫ
//�޸�ʱ��:		2010-11-16
//===================================================================
void Dis_StringsFilterTitle(const char *hz,uint16 x,uint16 y,uint32 font,uint16 color_f,S_Form *form)
{
uint8 dat;
uint32 qu_ma,wei_ma;
uint32 hz_addr;
uint32 half_step,full_step;
void (*call_full)(uint32 num, uint16 x, uint16 y, uint16 color_f, S_Form *form);
void (*call_half)(uint8 num, uint16 x, uint16 y, uint16 color_f, S_Form *form);
	if(font==FONT24) 
	{
		call_full=Dis_20x20FilterTitle;
		call_half=Dis_10x20FilterTitle;
		full_step=FONT24;
		half_step=FONT24>>1;
	}
	else
	{
		call_full=Dis_16x16FilterTitle;
		call_half=Dis_8x16FilterTitle;
		full_step=FONT16;
		half_step=FONT16>>1;
	}
	while(1)
	{
		dat=*hz;
		if(dat==0) break;		//���ִ�����
		//�Ǻ��ִ���----
		if(dat>=0x80)
		{
			qu_ma=dat;
			hz++;
			wei_ma=*hz;
			if(wei_ma==0) break;
			if((qu_ma>0xa0)&&(wei_ma>0xa0))
			{
				qu_ma-=0xa0;					
				wei_ma-=0xa0;
				hz_addr=(qu_ma-1)*94+wei_ma-1;
				(*call_full)(hz_addr,x,y,color_f,form);
			}
			x+=full_step;
		}
		//ASC����ʾ����----
		else
		{
			if(dat<32) return;
			dat-=ASC_Head;
			(*call_half)(dat,x,y,color_f,form);
			x+=half_step;
		}
		hz++;
	}
}

void Dis_NumFilterTitle(uint16 dat,uint16 x,uint16 y,uint8 weisu,uint16 font,uint16 color_f,S_Form *form)
{
enum {NUM_Addr=16};		//�������ֿ��е���ʼ�±�
uint8 i;
uint8 youxiao;
uint8 buf[5];
uint8 num;
uint32 step;
void (*call_sub)(uint8 num, uint16 x, uint16 y, uint16 color_f, S_Form * form);
	if(font==FONT24) 
	{	
		step=FONT24>>1;
		call_sub=Dis_10x20FilterTitle;
	}
	else 
	{
		step=8;
		call_sub=Dis_8x16FilterTitle;
	}
	
	num=0;
	switch(weisu)
	{
		case 1:
			if(dat>=10) i=0;
			else i=(uint8)dat;
			(*call_sub)(i+NUM_Addr,x,y,color_f,form);
			break;
		case 2:
			if(dat>=100) dat=0;
			buf[0]=dat/10;
			buf[1]=dat%10;
			for(i=0;i<2;i++)
			{
				if((buf[i]>0)||(i==1))
				{
					(*call_sub)(buf[i]+NUM_Addr,x,y,color_f,form);
					x+=step;
					num++;
				}
			}
			if(num!=2)
			{
				Dis_StringsFilterTitle(" ",x,y,font,color_f,form);
			}
			break;
		case 3:
			youxiao=0;
			if(dat>=1000) dat=0;
			buf[0]=dat/100;
			dat%=100;
			buf[1]=dat/10;
			buf[2]=dat%10;
			for(i=0;i<3;i++)
			{
				if((buf[i]>0)||(i==2)||(youxiao>0))
				{
					(*call_sub)(buf[i]+NUM_Addr,x,y,color_f,form);
					x+=step;
					youxiao=1;
					num++;
				}
			}
			if(num!=3)
			{
				for(i=num;i<3;i++)
				{
					Dis_StringsFilterTitle(" ",x,y,font,color_f,form);
					x+=step;
				}
			}
			break;
		case 4:
			youxiao=0;
			if(dat>=10000) dat=0;
			buf[0]=dat/1000;
			dat%=1000;
			buf[1]=dat/100;
			dat%=100;
			buf[2]=dat/10;
			buf[3]=dat%10;
			for(i=0;i<4;i++)
			{
				if((buf[i]>0)||(i==3)||(youxiao>0))
				{
					(*call_sub)(buf[i]+NUM_Addr,x,y,color_f,form);
					x+=step;
					youxiao=1;
					num++;
				}
			}
			if(num!=4)
			{
				for(i=num;i<4;i++)
				{
					Dis_StringsFilterTitle(" ",x,y,font,color_f,form);
					x+=step;
				}
			}
			break;
		case 5:
			youxiao=0;
			buf[0]=dat/10000;
			dat%=10000;
			buf[1]=dat/1000;
			dat%=1000;
			buf[2]=dat/100;
			dat%=100;
			buf[3]=dat/10;
			buf[4]=dat%10;
			for(i=0;i<5;i++)
			{
				if((buf[i]>0)||(i==4)||(youxiao>0))
				{
					(*call_sub)(buf[i]+NUM_Addr,x,y,color_f,form);
					x+=step;
					youxiao=1;
					num++;
				}
			}
			if(num!=5)
			{
				for(i=num;i<5;i++)
				{
					Dis_StringsFilterTitle(" ",x,y,font,color_f,form);
					x+=step;
				}
			}
			break;
		default:
			break;
	}
}

//===================================================================
//����:			��ʾASC�ַ�(ASC���0��ʼ)
//����:
//		x:			��ʼ������
//		y:			��ʼ������
//		font:			�����С
//		color_f:		��ʾ��ǰ��ɫ
//		color_b:		��ʾ��ǰ��ɫ
//�޸�ʱ��:		2011-11-30
//===================================================================
void Dis_Char(uint8 num,uint16 x,uint16 y,uint32 font,uint16 color_f,uint16 color_b)
{
	if(num<32) return;
	num-=32;
	if(font==FONT24)
	{
		Dis_10x20(num,x,y,color_f,color_b);
	}
	else
	{
		Dis_8x16(num,x,y,color_f,color_b);
	}
}

//===================================================================
//����:			��ʾ�ַ���
//����:
//		hz:			�ַ���ָ��
//		x:			��ʼx����
//		y:			��ʼy����
//		font:			��������С
//		color_f:		�ַ���ǰ��ɫ
//		color_b:		�ַ�������ɫ
//�޸�ʱ��:		2010-11-10
//===================================================================
void Dis_Strings(const char *hz,uint16 x,uint16 y,uint16 font,uint16 color_f,uint16 color_b)
{
uint8 dat;
uint32 qu_ma,wei_ma;
uint32 hz_addr;
uint32 full_step;				//ȫ��ռ�õ����ؿ��
uint32 half_step;				//���ռ�õ����ؿ��	
void (*call_full)(uint32 num, uint16 x, uint16 y, uint16 color_f, uint16 color_b);
void (*call_half)(uint8 num, uint16 x, uint16 y, uint16 color_f, uint16 color_b);
	//����Ϊ24������----------
	if(font==FONT24) 
	{
		call_full=Dis_20x20;
		call_half=Dis_10x20;
		full_step=FONT24;
		half_step=FONT24>>1;
	}
	else
	{
		call_full=Dis_16x16;
		call_half=Dis_8x16;
		full_step=FONT16;
		half_step=FONT16>>1;
	}

	while(1)
	{
		dat=*hz;
		if(dat==0) break;		//���ִ�����
		//�Ǻ��ִ���----
		if(dat>=0x80)
		{
			qu_ma=dat-0xa0;
			hz++;
			wei_ma=*hz;
			if(wei_ma==0) break;
			wei_ma-=0xa0;
			hz_addr=(qu_ma-1)*94+wei_ma-1;
			call_full(hz_addr,x,y,color_f,color_b);
			x+=full_step;
		}
		//ASC����ʾ����----
		else
		{
			if(dat<32) return;
			dat-=ASC_Head;
			call_half(dat,x,y,color_f,color_b);
			x+=half_step;
		}
		hz++;
	}
}

//===================================================================
//zadd=iAddrZK+32*(((uint32)(zm)-0xa0-1)*94+(uint32)(bm)-0xa0-1);break;
//�����ľ��κ���
//x:		��ʼ������
//y:		��ʼ������
//width:	���εĶȶ�
//heigh:	���εĸ߶�
//color:	�ߵ���ɫ
//===================================================================
void Dis_StringsLogo(const char *hz,uint16 x,uint16 y,uint16 color_f)
{
uint8 dat;
uint32 qu_ma,wei_ma;
uint32 hz_addr;
	while(1)
	{
		dat=*hz;
		if(dat==0) break;		//���ִ�����
		//�Ǻ��ִ���----
		if(dat>=0x80)
		{
			qu_ma=dat;
			hz++;
			wei_ma=*hz;
			if(wei_ma==0) break;
			if((qu_ma>0xa0)&&(wei_ma>0xa0))
			{
				qu_ma-=0xa0;					
				wei_ma-=0xa0;
				hz_addr=(qu_ma-1)*94+wei_ma-1;
				Dis_16x16Logo(hz_addr,x,y,color_f);
			}
			x+=16;
		}
		//ASC����ʾ����----
		else
		{
			if(dat<32) return;
			dat-=ASC_Head;
			Dis_8x16Logo(dat,x,y,color_f);
			x+=8;
		}
		hz++;
	}
}

//===================================================================
//zadd=iAddrZK+32*(((uint32)(zm)-0xa0-1)*94+(uint32)(bm)-0xa0-1);break;
//����: ��ʾ�ַ���
//*hz      : Ҫ��ʾ���ַ���ָ��
//aligen   : ��ʾ�Ķ��뷽ʽ,0=�����,1=������ʾ,2=�Ҷ���
//x         : ��ʾ��������ʼ������
//y		 : ��ʾ��������ʼ������
//width   : ��ʾ�������ܿ��
//color_f  : ������ɫ
//color_b : ����ɫ
//===================================================================
void Dis_StringsAligen(const char *hz,uint32 aligen,uint16 x,uint16 y,uint32 width,uint16 font,uint16 color_f,uint16 color_b)
{
uint8 dat;
uint32 tol;					//����ַ����ܸ���
uint32 i,m;
uint32 step;					//����ַ�ռ�õ����ؿ��
	//����Ϊ24������----------
	if(font==FONT24) step=FONT24>>1;
	else step=8;
	tol=0;
	//�������������ʾ���������ʾ����ʼx����----
	if(aligen!=ALIGEN_LEFT)
	{
		for(i=0;i<80;i++)
		{
			dat=*(hz+i);
			//��Ч���ַ�----
			if(dat>=32) tol++;
			else 
			{
				//������ʾ����---------
				if(aligen==ALIGEN_MIDDLE)
				{
					m=width-tol*step;
					x=x+m/2;
				}
				//�Ҷ�����ʾ����----
				else
				{
					x=x+(width-tol*step);
				}
				break;
			}
		}
	}
	Dis_Strings(hz,x,y,font,color_f,color_b);
}

//===================================================================
//���� : ��ʾ8*16�������ַ�(��ˢ�±���ɫ)
//x         : ��ʼ������
//y         : ��ʼ������
//color_f  : ��ʾ��ǰ��ɫ
//color_b : ��ʾ��ǰ��ɫ
//===================================================================
void Dis_8x16Filter(uint8 num,uint16 x,uint16 y,uint16 color_f)
{
uint32 i,j;
uint32 addr;
	addr=num*16;
	for(i=0;i<16;i++)
	{
		for(j=0;j<8;j++)
		{
			if(Asc8x16[addr+i]&(0x80>>j))
			{
				LCD_SetPixel(x+j,y+i,color_f);
			}
		}
	}
}

//===================================================================
//���� : ��ʾ16*16����ȫ���ַ�(��ˢ�±���ɫ)
//x:		��ʼ������
//y:		��ʼ������
//width:	���εĶȶ�
//heigh:	���εĸ߶�
//color:	�ߵ���ɫ
//===================================================================
void Dis_16x16Filter(uint32 num,uint16 x,uint16 y,uint16 color_f)
{
uint32 i,j;
uint8 dat;
uint8 dot[32];
uint32 addr;
	addr=num*32;
	memcpy(dot,Hz_Lib+addr,32);
	for(i=0;i<16;i++)
	{
		dat=dot[i*2];
		for(j=0;j<8;j++)
		{
			if(dat&(0x80>>j))
			{
				LCD_SetPixel(x+j,y+i,color_f);
			}
		}

		dat=dot[i*2+1];
		for(j=0;j<8;j++)
		{
			if(dat&(0x80>>j))
			{
				LCD_SetPixel(x+8+j,y+i,color_f);
			}
		}
	}
}

//===================================================================
//���� : ��ʾ�ַ���(��ˢ�±���ɫ)
//zadd=iAddrZK+32*(((uint32)(zm)-0xa0-1)*94+(uint32)(bm)-0xa0-1)
//*hz       : �ַ���ָ��
//x          : ��ʼ������
//y          : ��ʼ������
//color_f  : ��ʾ��ǰ��ɫ
//color:	�ߵ���ɫ
//===================================================================
void Dis_StringsFilter(const char *hz,uint16 x,uint16 y,uint16 font,uint16 color_f)
{
uint8 dat;
uint32 qu_ma,wei_ma;
uint32 hz_addr;
uint32 full_step;				//ȫ��ռ�õ����ؿ��
uint32 half_step;				//���ռ�õ����ؿ��	
void (*call_full)(uint32 num, uint16 x, uint16 y, uint16 color_f);
void (*call_half)(uint8 num, uint16 x, uint16 y, uint16 color_f);
	//����Ϊ24������----------
	if(font==FONT24) 
	{
		call_full=Dis_20x20Filter;
		call_half=Dis_10x20Filter;
		full_step=FONT24;
		half_step=FONT24>>1;
	}
	else
	{
		call_full=Dis_16x16Filter;
		call_half=Dis_8x16Filter;
		full_step=FONT16;
		half_step=FONT16>>1;
	}

	while(1)
	{
		dat=*hz;
		if(dat==0) break;		//���ִ�����
		//�Ǻ��ִ���----
		if(dat>=0x80)
		{
			qu_ma=dat-0xa0;
			hz++;
			wei_ma=*hz;
			if(wei_ma==0) break;
			wei_ma-=0xa0;
			hz_addr=(qu_ma-1)*94+wei_ma-1;
			(*call_full)(hz_addr,x,y,color_f);
			x+=full_step;
		}
		//ASC����ʾ����----
		else
		{
			if(dat<32) return;
			dat-=ASC_Head;
			(*call_half)(dat,x,y,color_f);
			x+=half_step;
		}
		hz++;
	}
}

//===================================================================
//zadd=iAddrZK+32*(((uint32)(zm)-0xa0-1)*94+(uint32)(bm)-0xa0-1);break;
//����: ��ʾ�ַ���
//*hz      : Ҫ��ʾ���ַ���ָ��
//aligen   : ��ʾ�Ķ��뷽ʽ,0=�����,1=������ʾ,2=�Ҷ���
//x         : ��ʾ��������ʼ������
//y		 : ��ʾ��������ʼ������
//width   : ��ʾ�������ܿ��
//color_f  : ������ɫ
//color_b : ����ɫ
//�޸�ʱ��:		2010-11-11
//===================================================================
void Dis_StringsAligenFilter(const char *hz,uint32 aligen,uint16 x,uint16 y,uint32 width,uint16 font,uint16 color_f)
{
uint8 dat;
uint32 qu_ma,wei_ma;
uint32 hz_addr;
uint32 tol;					//����ַ����ܸ���
uint32 i,m;
uint32 full_step;				//ȫ��ռ�õ����ؿ��
uint32 half_step;				//���ռ�õ����ؿ��	
void (*call_full)(uint32 num, uint16 x, uint16 y, uint16 color_f);
void (*call_half)(uint8 num, uint16 x, uint16 y, uint16 color_f);
	//����Ϊ24������----------
	if(font==FONT24) 
	{
		call_full=Dis_20x20Filter;
		call_half=Dis_10x20Filter;
		full_step=FONT24;
		half_step=FONT24>>1;
	}
	else
	{
		call_full=Dis_16x16Filter;
		call_half=Dis_8x16Filter;
		full_step=FONT16;
		half_step=FONT16>>1;
	}

	tol=0;
	//�������������ʾ���������ʾ����ʼx����----
	if(aligen!=ALIGEN_LEFT)
	{
		for(i=0;i<80;i++)
		{
			dat=*(hz+i);
			//��Ч���ַ�----
			if(dat>=32) tol++;
			else 
			{
				//������ʾ����---------
				if(aligen==ALIGEN_MIDDLE)
				{
					m=width-tol*half_step;
					x=x+m/2;
				}
				//�Ҷ�����ʾ����----
				else
				{
					x=x+(width-tol*half_step);
				}
				break;
			}
		}
	}
	
	while(1)
	{
		dat=*hz;
		if(dat==0) break;		//���ִ�����
		//�Ǻ��ִ���----
		if(dat>=0x80)
		{
			qu_ma=dat;
			hz++;
			wei_ma=*hz;
			if(wei_ma==0) break;
			if((qu_ma>0xa0)&&(wei_ma>0xa0))
			{
				qu_ma-=0xa0;					
				wei_ma-=0xa0;
				hz_addr=(qu_ma-1)*94+wei_ma-1;
				(*call_full)(hz_addr,x,y,color_f);
			}
			x+=full_step;
		}
		//ASC����ʾ����----
		else
		{
			if(dat<32) return;
			dat-=ASC_Head;
			(*call_half)(dat,x,y,color_f);
			x+=half_step;
		}
		hz++;
	}
}

//===================================================================
//zadd=iAddrZK+32*(((uint32)(zm)-0xa0-1)*94+(uint32)(bm)-0xa0-1);break;
//�����ľ��κ���
//x:		��ʼ������
//y:		��ʼ������
//width:	���εĶȶ�
//heigh:	���εĸ߶�
//color:	�ߵ���ɫ
//===================================================================
void Dis_Chinese(uint16 hz,uint16 x,uint16 y,uint16 color_f,uint16 color_b)
{
uint32 qu_ma,wei_ma;
uint32 hz_addr;
	qu_ma=hz>>8;
	wei_ma=hz&0xff;
	if((qu_ma<0xa0)||(wei_ma<0xa0)) return;
	qu_ma-=0xa0;
	wei_ma-=0xa0;
	hz_addr=(qu_ma-1)*94+wei_ma-1;
	Dis_16x16(hz_addr,x,y,color_f,color_b);
}

//===================================================================
//����:			��ʾһ����ֵ
//��ʾһ����ֵ,�����,��λ0�Զ�����
//weisu:  ��ֵ�����λ��
//stt:     0Ϊ������ʾ,1Ϊ������ʾ
//�޸�ʱ��:		2010-11-20
//===================================================================
void Dis_NumHide(uint16 dat,uint16 x,uint16 y,uint8 weisu,uint16 font,uint16 color_f,uint16 color_b)
{
uint8 i;
uint8 youxiao;
uint8 buf[5];
uint8 num;
uint32 width;
	if(font==FONT24) width=FONT24>>1;
	else width=8;
	num=0;
	switch(weisu)
	{
		case 1:
			if(dat>=10) i=0;
			else i=(uint8)dat;
			Dis_SuZi(i,x,y,font,color_f,color_b);
			break;
		case 2:
			if(dat>=100) dat=0;
			buf[0]=dat/10;
			buf[1]=dat%10;
			for(i=0;i<2;i++)
			{
				if((buf[i]>0)||(i==1))
				{
					Dis_SuZi(buf[i],x,y,font,color_f,color_b);
					x+=width;
					num++;
				}
			}
			if(num!=2)
			{
				Dis_Strings(" ",x,y,font,color_b,color_b);
			}
			break;
		case 3:
			youxiao=0;
			if(dat>=1000) dat=0;
			buf[0]=dat/100;
			dat%=100;
			buf[1]=dat/10;
			buf[2]=dat%10;
			for(i=0;i<3;i++)
			{
				if((buf[i]>0)||(i==2)||(youxiao>0))
				{
					Dis_SuZi(buf[i],x,y,font,color_f,color_b);
					x+=width;
					youxiao=1;
					num++;
				}
			}
			if(num!=3)
			{
				for(i=num;i<3;i++)
				{
					Dis_Strings(" ",x,y,font,color_b,color_b);
					x+=width;
				}
			}
			break;
		case 4:
			youxiao=0;
			if(dat>=10000) dat=0;
			buf[0]=dat/1000;
			dat%=1000;
			buf[1]=dat/100;
			dat%=100;
			buf[2]=dat/10;
			buf[3]=dat%10;
			for(i=0;i<4;i++)
			{
				if((buf[i]>0)||(i==3)||(youxiao>0))
				{
					Dis_SuZi(buf[i],x,y,font,color_f,color_b);
					x+=width;
					youxiao=1;
					num++;
				}
			}
			if(num!=4)
			{
				for(i=num;i<4;i++)
				{
					Dis_Strings(" ",x,y,font,color_b,color_b);
					x+=width;
				}
			}
			break;
		case 5:
			youxiao=0;
			buf[0]=dat/10000;
			dat%=10000;
			buf[1]=dat/1000;
			dat%=1000;
			buf[2]=dat/100;
			dat%=100;
			buf[3]=dat/10;
			buf[4]=dat%10;
			for(i=0;i<5;i++)
			{
				if((buf[i]>0)||(i==4)||(youxiao>0))
				{
					Dis_SuZi(buf[i],x,y,font,color_f,color_b);
					x+=width;
					youxiao=1;
					num++;
				}
			}
			if(num!=5)
			{
				for(i=num;i<5;i++)
				{
					Dis_Strings(" ",x,y,font,color_b,color_b);
					x+=width;
				}
			}
			break;
		default:
			break;
	}
}

void Dis_NumAll(uint16 dat,uint16 x,uint16 y,uint8 weisu,uint16 font,uint16 color_f,uint16 color_b)
{
uint32 i;
uint8 wei[5];
uint16 step;				//ÿ���ַ�ռ�õ����ص�
	if(font==FONT24) step=FONT24>>1;
	else step=8;
	wei[4]=0;
	wei[3]=0;
	wei[2]=0;
	wei[1]=0;
	if(dat>=10000) 
	{
		wei[4]=dat/10000;
		dat%=10000;
	}
	if(dat>=1000)
	{
		wei[3]=dat/1000;
		dat%=1000;
	}
	if(dat>=100)
	{
		wei[2]=dat/100;
		dat%=100;
	}
	if(dat>=10) wei[1]=dat/10;
	wei[0]=dat%10;
	for(i=weisu;i>0;i--)
	{
		if(wei[i-1]>9) wei[i-1]=0;
		Dis_SuZi(wei[i-1],x,y,font,color_f,color_b);
		x+=step;
	}
}

//===================================================================
//����: ��ʾ��ֵ(������),����ɫΪLOGOͼƬ
//===================================================================
void Dis_NumAllLogo(uint16 dat,uint16 x,uint16 y,uint8 weisu,uint16 color_f)
{
uint32 i;
uint8 wei[5];
	wei[4]=0;
	wei[3]=0;
	wei[2]=0;
	wei[1]=0;
	if(dat>=10000) 
	{
		wei[4]=dat/10000;
		dat%=10000;
	}
	if(dat>=1000)
	{
		wei[3]=dat/1000;
		dat%=1000;
	}
	if(dat>=100)
	{
		wei[2]=dat/100;
		dat%=100;
	}
	if(dat>=10) wei[1]=dat/10;
	wei[0]=dat%10;
	for(i=weisu;i>0;i--)
	{
		if(wei[i-1]>9) wei[i-1]=0;
		Dis_SuZiLogo(wei[i-1],x,y,color_f);
		x+=8;
	}
}

//===================================================================
//����: ��ʾ��С��������(���2λС��)
//dat      : ��С�����ֳ���10��N�η���ת��������������
//weisu   : ԭ���������ֵ�λ��
//xiaosu  : ԭ��С�����ֵ�λ��
//===================================================================
void Dis_NumFloat(uint32 dat,uint16 x,uint16 y,uint8 weisu,uint8 xiaosu,uint16 font,uint16 color_f,uint16 color_b)
{
uint32 m;
uint32 zensu;
uint32 step;
	if(font==FONT24) step=FONT24>>1;
	else step=8;
	if(xiaosu==1) m=10;
	else if(xiaosu==2) m=100;
	else return;
	zensu=dat/m;
	Dis_NumHide(zensu,x,y,weisu,font,color_f,color_b);
	if(xiaosu==1) Dis_Strings("  ",x+weisu*step,y,font,color_f,color_b);
	else Dis_Strings("   ",x+weisu*step,y,font,color_f,color_b);
	if(zensu<10) x+=step;
	else if(zensu<100) x+=step*2;
	else if(zensu<1000) x+=step*3;
	Dis_Strings(".",x,y,font,color_f,color_b);
	x+=step;
	zensu=dat%m;
	Dis_NumAll(zensu,x,y,xiaosu,font,color_f,color_b);
}

//===================================================================
//����:			��ʾ��С��������(�̶�Ϊ.0��.5)
//����:			
//		dat:			��������
//		five:			��Ϊ0��ʾ��0.5
//		weisu:		ԭ���������ֵ�λ��
//		x,y:			��ʾ����ʼx,y����
//		color_f:		������ɫ
//		color_b:		����ɫ
//˵��:			�������Ϊ5λ��
//�޸�ʱ��:		2011-11-28
//===================================================================
void Dis_NumPoint5(uint32 dat,uint32 five,uint16 x,uint16 y,uint8 weisu,uint16 font,uint16 color_f,uint16 color_b)
{
uint32 i;
uint32 m;
uint32 step;
uint32 num;				//������ʵ��λ��
	if(font==FONT24) step=FONT24>>1;
	else step=8;
	if(dat<10) num=1;
	else if(dat<100) num=2;
	else if(dat<1000) num=3;
	else if(dat<10000) num=4;
	else num=5;
	if(num>weisu) num=weisu;
	Dis_NumHide(dat,x,y,weisu,font,color_f,color_b);
	if(five>0)
	{
		Dis_Strings(".5",x+num*step,y,font,color_f,color_b);
	}
	else 
	{
		Dis_Strings(".0",x+num*step,y,font,color_f,color_b);
	}
	m=weisu-num;
	for(i=0;i<m;i++)
	{
		Dis_Strings(" ",x+(num+i+2)*step,y,font,color_f,color_b);
	}
}

//===================================================================
//����: ��ʾ16λ�з������ķ���
//dat      : ����(ȡ����ֵ)
//zf        : �з�������������־0=��,1=��
//�޸�ʱ��:		2010-11-11
//===================================================================
void Dis_NumFuHao(uint16 dat,uint8 zf,uint16 x,uint16 y,uint16 font,uint16 color_f,uint16 color_b)
{
	if(dat==0) 
	{
		Dis_Strings(" ",x,y,font,color_f,color_b);
	}
	else if(zf==0) 
	{
		Dis_Strings("+",x,y,font,color_f,color_b);
	}
	else 
	{
		Dis_Strings("-",x,y,font,color_f,color_b);
	}
}

//===================================================================
//����:			��ʾһ���з�����ֵ
//����:
//		dat:			16λ�з�����
//		zf:			��Ϊ0��ʾ����(��ֵΪ0ʱ��Ч)
//		x,y:			��ʾ����ʼx,y����
//		weisu:		��ֵ�����λ��
//		font:			��������
//		color_f:		������ɫ
//		color_b:		���屳��ɫ
//˵��:			��ʾһ����ֵ,�����,��λ0�Զ�����
//�޸�ʱ��:		2011-11-28
//===================================================================
void Dis_NumInt(int16 dat,uint16 x,uint16 y,uint8 weisu,uint16 font,uint16 color_f,uint16 color_b)
{
uint16 m;
uint32 width;
	if(font==FONT24) width=FONT24>>1;
	else width=8;
	//����0����-----
	if(dat==0) 
	{
		m=dat;
		Dis_NumHide(m,x,y,weisu,font,color_f,color_b);
		Dis_Strings(" ",x+weisu*width,y,font,color_f,color_b);
	}
	//����0����-----
	else if(dat>0)
	{
		m=dat;
		Dis_Strings("+",x,y,font,color_f,color_b);
		Dis_NumHide(m,x+width,y,weisu,font,color_f,color_b);		
	}
	//С��0����-----
	else
	{
		m=0-dat;
		Dis_Strings("-",x,y,font,color_f,color_b);
		Dis_NumHide(m,x+width,y,weisu,font,color_f,color_b);		
	}
}

//===================================================================
//����: ��ʾһ������������Windows����(��������ɫ�м���Ч��)
//             ��������������ɫΪ��ɫ
//x         : ��ʼ������
//y         : ��ʼ������
//width   : ����Ŀ��
//heigh   : ����ĸ߶�
//ref_back: ��Ϊ0��ʾҪˢ�´����ڵı���ɫ
//head_color: �������ı���ɫ��ʽ0~2
//˵��: �����������߶�24 (�Ҳ���ʾX��ʾ���������ر�)
//		   ���������־��ж���
//�޸�ʱ��:		2010-11-10
//===================================================================
void Dis_WindowsForm(S_Form *form)
{
uint32 TOP_H;			//�����������ĸ߶�
uint16 x,y,w,h;
uint16 color;
uint32 i,len;			
uint32 c_x;
uint32 asc_dot;
	x=form->X;
	y=form->Y;
	w=form->Width;
	h=form->Height;
	if(form->TitleColor>MAX_FormTitle) form->TitleColor=FORM_Blue;
	if(form->TitleFont!=FONT24) form->TitleFont=FONT16;
	if((form->TitleHeight<TITLE_H16)||(form->TitleHeight>(TITLE_H24*2)))
	{
		form->TitleHeight=TITLE_H16;
		form->TitleFont=FONT16;
	}

	TOP_H=form->TitleHeight;	
	//����������ɫ-----
	for(i=0;i<TOP_H;i++)
	{
		//�������߶ȴ���24ʱ�ı���ɫ����------
		if(TOP_H>24)
		{
			if(i<12)
			{
				color=FormTitleColor[form->TitleColor][i];
			}
			else if((TOP_H-i)>=12)
			{
				color=FormTitleColor[form->TitleColor][12];
			}
			else
			{
				color=FormTitleColor[form->TitleColor][24-(TOP_H-i)];
			}
		}
		else
		{
			color=FormTitleColor[form->TitleColor][i];
		}
		Dis_HLine(x+1,y+i,w-2,color);
	}

	c_x=x+w-20-4;
	if(form->TitleColor==0) len=BMP_CloseBlue;
	else len=BMP_CloseRed;
	Dis_PictureFilter(c_x,y+((TOP_H-20)>>1),20,20,BmpFunc+len,LCD_FilterColor);		//��ʾ�ر�ͼƬ

	//�����������
	Dis_VLine(x,y,h,FormTitleColor[form->TitleColor][23]);
	Dis_VLine(x+1,y+1,h-2,FormTitleColor[form->TitleColor][12]);
	//�ұ���������
	Dis_VLine(x+w-2,y+1,h-1,FormTitleColor[form->TitleColor][23]);
	Dis_VLine(x+w-1,y,h,FormTitleColor[form->TitleColor][23]);
	//�ײ���������(��ɫ)
	Dis_HLine(x+2,y+h-2,w-4,FormTitleColor[form->TitleColor][12]);
	Dis_HLine(x,y+h-1,w,FormTitleColor[form->TitleColor][23]);

	len=0;
	asc_dot=(form->TitleFont>>1);
	for(i=0;i<100;i++)
	{
		if(*(form->TitleText+i)!=0) len+=asc_dot;
		else break;
	}
	i=w/2-len/2;
	len=x+i;
	Dis_StringsFilter(form->TitleText,len,y+(TOP_H-form->TitleFont)/2,form->TitleFont,C_White);
	//��䴰���ڵı���ɫ-----
	if(form->FillBack>0)
	{
		Dis_RectFill(x+2,y+TOP_H,w-4,h-TOP_H-2,form->BackColor);
	}
}

//===================================================================
//����:			��ʾһ������������Windows����
//��ڲ���:		
//		form:		����ṹָ��
//˵��: 			���������ֿ������
//�޸�ʱ��:		2010-11-23
//===================================================================
void Dis_WindowsFormLeft(S_Form *form)
{
uint32 TOP_H;			//�����������ĸ߶�
uint16 x,y,width,height;
uint32 i;		
uint16 color;
uint32 addr;
	if(form->TitleColor>MAX_FormTitle) form->TitleColor=FORM_Blue;
	x=form->X;
	y=form->Y;
	width=form->Width;
	height=form->Height;
	TOP_H=form->TitleHeight;

	//����������ɫ-----
	for(i=0;i<TOP_H;i++)
	{
		//�������߶ȴ���24ʱ�ı���ɫ����------
		if(TOP_H>24)
		{
			if(i<12)
			{
				color=FormTitleColor[form->TitleColor][i];
			}
			else if((TOP_H-i)>=12)
			{
				color=FormTitleColor[form->TitleColor][12];
			}
			else
			{
				color=FormTitleColor[form->TitleColor][24-(TOP_H-i)];
			}
		}
		else
		{
			color=FormTitleColor[form->TitleColor][i];
		}
		Dis_HLine(x+1,y+i,width-2,color);
	}
	
	//��ʾ�ر�ͼ��-----
	if(form->TitleColor==FORM_Red) addr=BMP_CloseBlue;
	else addr=BMP_CloseRed;
	x=form->X+width-20-4;
	y=form->Y+((form->TitleHeight-20)>>1);
	Dis_PictureFilter(x,y,20,20,BmpFunc+addr,LCD_FilterColor);		//��ʾ�ر�ͼƬ

	x=form->X;
	y=form->Y;
	//�����������
	Dis_VLine(x,y,height,FormTitleColor[form->TitleColor][23]);
	Dis_VLine(x+1,y+1,height-2,FormTitleColor[form->TitleColor][12]);
	//�ұ���������
	Dis_VLine(x+form->Width-2,y+1,form->Height-1,FormTitleColor[form->TitleColor][23]);
	Dis_VLine(x+form->Width-1,y,form->Height,FormTitleColor[form->TitleColor][23]);
	//�ײ���������(��ɫ)
	Dis_HLine(x+2,y+form->Height-2,form->Width-4,FormTitleColor[form->TitleColor][12]);
	Dis_HLine(x,y+form->Height-1,form->Width,FormTitleColor[form->TitleColor][23]);
	//��ʾ�������ı�---------
	Dis_StringsFilter(form->TitleText,x+6,form->Y+((TOP_H-form->TitleFont)>>1),form->TitleFont,C_White);
	if(form->FillBack>0)
	{
		Dis_RectFill(x+2,y+form->TitleHeight,form->Width-4,form->Height-form->TitleHeight-2,form->BackColor);	//ˢ�´����ڵı���ɫ
	}
}

//===================================================================
//����:			��ʾһ���ı���
//�޸�ʱ��:		2010-11-11
//===================================================================
void Dis_TextFrame(uint16 x,uint16 y,uint16 width,uint16 height,uint16 color)
{
	//�ı���
	Dis_RectFill(x+2,y+2,width-4,height-4,color);
/*
	//������������(��ɫ)
	Dis_HLine(x,y,width,C_Black);
	Dis_HLine(x,y+1,width-1,C_Black);
	//�����������(��ɫ)
	Dis_VLine(x,y,height,C_Black);
	Dis_VLine(x+1,y,height-1,C_Black);	
*/
	Dis_RectSome(x,y,width,height,2,C_Blue);
}

//===================================================================
//����: ��ʾһ����͹�Ŀ���(ˢ�¿����ڵ���ɫ)
//color    : ����ĵ�ɫ
//===================================================================
void Dis_BoxUp(uint16 x,uint16 y,uint16 width,uint16 height,uint16 color)
{
	//��ɫ
	Dis_RectFill(x,y,width,height,color);
	//������������(��ɫ)
	Dis_HLine(x,y,width,C_White);
	Dis_HLine(x,y+1,width-1,C_White);
	//�ײ���������(��ɫ)
	Dis_HLine(x+2,y+height-2,width-2,C_Black);
	Dis_HLine(x+1,y+height-1,width-1,C_Black);
	//�����������(��ɫ)
	Dis_VLine(x,y,height,C_White);
	Dis_VLine(x+1,y,height-1,C_White);
	//�ұ���������(��ɫ)
	Dis_VLine(x+width-2,y+2,height-2,C_Black);
	Dis_VLine(x+width-1,y+1,height-1,C_Black);
}

//===================================================================
//����: ��ʾһ���°��Ŀ���(ˢ�¿����ڵ���ɫ)
//color    : ����ĵ�ɫ
//===================================================================
void Dis_BoxDown(uint16 x,uint16 y,uint16 width,uint16 height,uint16 color)
{
	//��ɫ
	Dis_RectFill(x,y,width,height,color);
	//������������(��ɫ)
	Dis_HLine(x,y,width,C_Black);
	Dis_HLine(x,y+1,width-1,C_Black);
	//�ײ���������(��ɫ)
	Dis_HLine(x+2,y+height-2,width-2,C_White);
	Dis_HLine(x+1,y+height-1,width-1,C_White);
	//�����������(��ɫ)
	Dis_VLine(x,y,height,C_Black);
	Dis_VLine(x+1,y,height-1,C_Black);
	//�ұ���������(��ɫ)
	Dis_VLine(x+width-2,y+2,height-2,C_White);
	Dis_VLine(x+width-1,y+1,height-1,C_White);
}

//===================================================================
//����: ��ʾһ����͹�Ŀ���(��ˢ�¿����ڵ���ɫ)
//color    : ��ɫ�ߵ���ɫ
//===================================================================
void Dis_FrameUp(uint16 x,uint16 y,uint16 width,uint16 height,uint16 color)
{
	//������������(��ɫ)
	Dis_HLine(x,y,width,C_White);
	Dis_HLine(x,y+1,width-1,C_White);
	//�ײ���������(��ɫ)
	Dis_HLine(x+2,y+height-2,width-2,color);
	Dis_HLine(x+1,y+height-1,width-1,color);
	//�����������(��ɫ)
	Dis_VLine(x,y,height,C_White);
	Dis_VLine(x+1,y,height-1,C_White);
	//�ұ���������(��ɫ)
	Dis_VLine(x+width-2,y+2,height-2,color);
	Dis_VLine(x+width-1,y+1,height-1,color);
}

//===================================================================
//����: ��ʾһ���°��Ŀ���(��ˢ�¿����ڵ���ɫ)
//color    : ��ɫ�ߵ���ɫ
//===================================================================
void Dis_FrameDown(uint16 x,uint16 y,uint16 width,uint16 height,uint16 color)
{
	//������������(��ɫ)
	Dis_HLine(x,y,width,color);
	Dis_HLine(x,y+1,width-1,color);
	//�ײ���������(��ɫ)
	Dis_HLine(x+2,y+height-2,width-2,C_White);
	Dis_HLine(x+1,y+height-1,width-1,C_White);
	//�����������(��ɫ)
	Dis_VLine(x,y,height,color);
	Dis_VLine(x+1,y,height-1,color);
	//�ұ���������(��ɫ)
	Dis_VLine(x+width-2,y+2,height-2,C_White);
	Dis_VLine(x+width-1,y+1,height-1,C_White);
}

//===================================================================
//����  : 			��ʾһ��Windows���͵���ʾ��(������������)
//��ڲ���:
//		title:		�������ϵ��ı���Ϣ
//		text:		��ʾ��Ϣ�ı�������(������ʾ)
//		push:		��Ϊ0��ʾҪ����������ı�������ջ����ʾ
//		color_f:		��ʾ�ı�����ɫ
//		color_b:		����ı���ɫ
//˵��:			�����Զ�������ʾ
//					����24��������
//ע��:			��push��Ϊ0ʱ,���ñ�����������Ӧ���Դ��ջ����
//�޸�ʱ��:		2010-12-1
//===================================================================
void Dis_HintBox(const char *title,const char *text,uint32 push,uint16 color_f,uint16 color_b)
{
uint32 i;
uint32 x,y;
uint32 len;
S_Form form;
	form.Height=70;
	form.TitleHeight=TITLE_H24;
	form.TitleFont=FONT24;
	form.Y=GET_ScrCenterY(form.Height);
	form.TitleText=PTEXT(title);
	form.TitleColor=FORM_Red;
	form.BackColor=color_b;
	form.FillBack=1;
	//�����ַ����ĳ���(����ֵ)------
	len=0;
	for(i=0;i<100;i++)
	{
		if(*(text+i)!=0) len+=12;
		else break;
	}
	//�����ַ�����ȷ��������----
	form.Width=len+60;		//���߸���60�����ص�
	form.X=GET_ScrCenterX(form.Width);
	//Ҫ���Դ���ջ����-------------
	if(push>0) 
	{
		LcdMemPush(form.X,form.Y,form.Width,form.Height);
	}
	Dis_WindowsFormLeft(&form);

	x=(form.Width-len)>>1;
	x=form.X+x;
	y=form.Y+form.TitleHeight+((form.Height-form.TitleHeight-FONT24)>>1);
	Dis_Strings(text,x,y,FONT24,color_f,form.BackColor);	//��ʾ��ʾ�ı�
}

//===================================================================
//����  : 			��ʾһ��Windows���͵���ʾ��(����������˳�)
//��ڲ���:
//		title:		�������ϵ��ı���Ϣ
//		text:		��ʾ��Ϣ�ı�������(������ʾ)
//		color_f:		��ʾ�ı�����ɫ
//		color_b:		����ı���ɫ
//˵��  : 			�Զ�װ�غͻָ�����ǰ����ʾ��Ϣ
//					�����Զ�������ʾ
//           			������ʾ�ı��ĳ����Զ��������
//					��ʾ����̶�Ϊ24����
//�޸�ʱ��:		2013-3-7
//===================================================================
void Dis_HintForm(const char *title,const char *text,uint16 color_f,uint16 color_b)
{
uint16 x,y;
uint32 i;
uint32 len;
uint32 tchnum;							//���ݵ��ܴ�������
S_TchKey tch_backup;						//���ݵĴ�������Ϣ
S_Form form;
	form.Height=70;
	form.TitleHeight=TITLE_H24;
	form.TitleFont=FONT24;
	form.Y=GET_ScrCenterY(form.Height);
	form.TitleText=PTEXT(title);
	form.TitleColor=FORM_Red;
	form.BackColor=color_b;
	form.FillBack=1;
	//�����ַ����ĳ���(����ֵ)------
	len=0;
	for(i=0;i<100;i++)
	{
		if(*(text+i)!=0) len+=FONT24>>1;
		else break;
	}
	//�����ַ�����ȷ��������----
	form.Width=len+60;		//���߸���30�����ص�
	form.X=GET_ScrCenterX(form.Width);
	LcdMemPush(form.X,form.Y,form.Width,form.Height);

	Dis_WindowsFormLeft(&form);	
	x=(form.Width-len)>>1;
	x=form.X+x;
	y=form.Y+form.TitleHeight+((form.Height-form.TitleHeight-FONT24)>>1);
	Dis_Strings(text,x,y,FONT24,color_f,color_b);	//��ʾ��ʾ�ı�
	//�����1����������Ϣ-----
	tch_backup=TchKey[0][0];
	tchnum=TchVal.KeyNum[0];
	Touch_KeyEn(form.X,form.Y,form.Width,form.Height,KEY_Tch,0);
	TchVal.KeyNum[0]=1;
	WaitAnyKey();
	//�ָ�֮ǰ�ĵ�1����������Ϣ----
	TchKey[0][0]=tch_backup;
	TchVal.KeyNum[0]=tchnum;
	LcdMemPop();
}

//===================================================================
//����:			��ʾһ��Windows���͵������
//��ڲ���:
//		max:			������������ֵ
//		dat:			Ĭ�ϵĳ�ʼ��ֵ
//		text:			��ʾ��Ϣ�ı�������
//		color_b:		����ı���ɫ
//����:			��������ֵ,��ȡ���򷵻�DO_Cancel
//˵��:			����24��������
//					�����Զ�����,�����ȸ��������Զ����
//�޸�ʱ��:		2011-3-14
//===================================================================
uint32 Dis_InputDialog(uint32 max,uint32 dat,const char *text,uint16 color_b)
{
uint32 i;
uint16 x,y;
uint32 len;
uint32 ret;
uint32 weisu;		
uint32 dat_len;
uint32 old_chang;				//���ڻָ���ǰ��Sys.DatChang����
S_Form form;
	if(dat>max) dat=0;
	old_chang=Sys.DatChang;
	//�����ַ����ĳ���------
	len=0;
	for(i=0;i<100;i++)
	{
		if(*(text+i)!=0) len+=FONT24>>1;
		else break;
	}
	//�ж�����ֵ�����λ��-----
	if(max<10) weisu=1;
	else if(max<100) weisu=2;
	else if(max<1000) weisu=3;
	else if(max<10000) weisu=4;
	else weisu=5;
	dat_len=weisu*FONT24>>1;
	
	form.Width=len+dat_len+50;		//�Զ����㴰����
	if(form.Width<160) form.Width=160;
	form.Height=80;
	form.X=GET_ScrCenterX(form.Width);
	form.Y=GET_ScrCenterY(form.Height);
	form.TitleText=Hz_InputDialog[0][Lang];
	form.TitleHeight=TITLE_H24;
	form.TitleFont=FONT24;
	form.TitleColor=FORM_Blue;
	form.BackColor=color_b;
	form.FillBack=1;

	LcdMemPush(form.X,form.Y,form.Width,form.Height);
	Dis_WindowsFormLeft(&form);

	//��ʾ��ʾ�ַ���-----------
	x=form.X+10;
	y=form.Y+form.TitleHeight+((form.Height-form.TitleHeight-FONT24)>>1);
	Dis_Strings(text,x,y,FONT24,C_Black,form.BackColor);
	x=x+len+10;
	Dis_TextFrame(x,y-2,dat_len+20,FRAME_Hight,C_White);
	Sys.LcdRef=1;
	Key.ReInput=1;
	TchVal.RefX=x;
	TchVal.RefY=y-2;
	TchVal.RefWidth=dat_len+20;
	TchVal.RefHeight=FONT24;
	Dis_TouchKeyBoard(TKEY_BoardStand);
	
	while(1)
	{
		Send_DspReply();		//�����ж�����λ���������		
	
		if(Sys.LcdRef>0)
		{
			Sys.LcdRef=0;
			Dis_NumAll(dat,x+10,y,weisu,FONT24,C_Red,C_White);
		}
	
		Read_Key();

		Dis_TouchButtonFlash();			//ˢ��ģ�ⴥ�����̵ı仯״̬		

		dat=Data_Input(max,0,dat);
		
		if(Key.Value==KEY_ESC)
		{
			ret=DO_Cancel;
			break;
		}
		else if(Key.Value==KEY_OK)
		{
			ret=dat;
			break;
		}
	}
	
	TouchKeyBoardClose();
	Key.Value=KEY_No;
	LcdMemPop();
	Sys.DatChang=old_chang;
	return(ret);
}

//===================================================================
//����:			��ʾһ��Windows���͵��ַ��������
//����:
//		dat:			������ַ�ָ��
//		len:			�������������ַ�����
//		text:			��ʾ��Ϣ�ı�������
//		color_b:		����ı���ɫ
//����:			��ȡ���򷵻�DO_Cancel
//˵��:			����24��������
//					�����Զ�����,�����ȸ��������Զ����
//�޸�ʱ��:		2011-11-30
//===================================================================
uint32 Dis_InputStrDialog(char *str,uint32 len,const char *text,uint16 color_b)
{
enum{ASCW=FONT24>>1};
uint32 i;
uint32 k;
uint16 x,y;
uint32 text_len;
char string[20];
uint16 c_b;
uint32 ret;
S_Form form;
	k=0;
	memcpy(string,str,len);
	//�����ַ����ĳ���------
	text_len=0;
	for(i=0;i<100;i++)
	{
		if(*(text+i)!=0) text_len+=ASCW;
		else break;
	}
	
	form.Width=len*ASCW+text_len+50;			//�Զ����㴰����
	if(form.Width<160) form.Width=160;
	form.Height=80;
	form.X=GET_ScrCenterX(form.Width);
	form.Y=GET_ScrCenterY(form.Height)-60;		//��֤ģ����������·�
	form.TitleText=Hz_InputDialog[0][Lang];
	form.TitleHeight=TITLE_H24;
	form.TitleFont=FONT24;
	form.TitleColor=FORM_Blue;
	form.BackColor=color_b;
	form.FillBack=1;

	LcdMemPush(form.X,form.Y,form.Width,form.Height);
	Dis_WindowsFormLeft(&form);

	//��ʾ��ʾ�ַ���-----------
	x=form.X+10;
	y=form.Y+form.TitleHeight+((form.Height-form.TitleHeight-FONT24)>>1);
	Dis_Strings(text,x,y,FONT24,C_Black,form.BackColor);
	x=x+text_len+10;
	Dis_TextFrame(x,y-2,len*ASCW+20,FRAME_Hight,C_White);
	Sys.LcdRef=1;
	Key.ReInput=1;
	TchVal.RefX=x;
	TchVal.RefY=y-2;
	TchVal.RefWidth=len*ASCW+20;
	TchVal.RefHeight=FONT24;
	Dis_TouchKeyBoard(TKEY_BoardAsc);
	
	while(1)
	{
		Send_DspReply();		//�����ж�����λ���������		
	
		if(Sys.LcdRef>0)
		{
			Sys.LcdRef=0;
			for(i=0;i<len;i++)
			{
				if(i==k) c_b=C_Black;
				else c_b=C_White;
				Dis_Char(string[i],x+10+i*ASCW,y,FONT24,C_Red,c_b);
			}
		}
	
		Read_Key();

		Dis_TouchButtonFlash();			//ˢ��ģ�ⴥ�����̵ı仯״̬		

		if(Key.Value<=KEY_9)
		{
			Sys.LcdRef=REF_Item;
			string[k]=Key.Value+'0';
			k++;
			if(k>=len) k=0;
		}	
		else if((Key.Value>=KEY_A)&&(Key.Value<=KEY_F))
		{
			Sys.LcdRef=REF_Item;
			string[k]=Key.Value-KEY_A+'A';
			k++;
			if(k>=len) k=0;			
		}
		else if(Key.Value==KEY_QUK)
		{	
			Sys.LcdRef=REF_Item;
			string[k]='0';
			if(k>0)
			{
				k--;
			}
		}
		else if(Key.Value==KEY_RIGHT)
		{
			if((k+1)<len)
			{
				Sys.LcdRef=REF_Item;
				k++;
			}
		}
		else if(Key.Value==KEY_LEFT)
		{
			if(k>0)
			{
				Sys.LcdRef=REF_Item;
				k--;
			}
		}

		if(Key.Value==KEY_OK)
		{
			memcpy(str,string,len);
			ret=0;
			break;
		}		
		else if(Key.Value==KEY_ESC)
		{
			ret=DO_Cancel;
			break;
		}
	}
	
	TouchKeyBoardClose();
	Key.Value=KEY_No;
	LcdMemPop();
	return(ret);
}

//===================================================================
//����: 			��ʾһ��Windows���͵�ȷ�Ͽ�
//��ڲ���:		
//		title:		�������ϵ�������Ϣ
//		text:		��ʾ��Ϣ�ı�������
//����: 			0xffffffff��ʾȡ��(DO_Cancel),������ʾȷ��
//˵��: 			�˳�ʱ�Զ��ָ������帲�ǵ���ʾ��Ϣ
//					����Ϊ24����
//�޸�ʱ��:		2010-11-29
//===================================================================
uint32 Dis_SureDialog(const char *title,const char *text,uint16 back_color)
{
uint32 i;
uint32 x,y;
uint32 ret;
uint32 len;
uint32 btnw;
S_Form form;
	Touch_KeyPush();
	TchVal.KeyNum[0]=2;
	//������ʾ�ı��ĳ���(�԰����)-----
	len=0;
	for(i=0;i<100;i++)
	{
		if(*(text+i)!=0) len+=FONT24>>1;
		else break;
	}	
	form.Width=len+30;
	if(form.Width<150) form.Width=150;
	btnw=(form.Width-30*2)>>1;
	form.Height=120;		
	form.X=GET_ScrCenterX(form.Width);
	form.Y=GET_ScrCenterY(form.Height);
	form.TitleText=title;
	form.TitleColor=FORM_Red;
	form.TitleFont=FONT24;
	form.TitleHeight=TITLE_H24;
	form.BackColor=back_color;
	form.FillBack=1;	
	LcdMemPush(form.X,form.Y,form.Width,form.Height);
	Dis_WindowsForm(&form);

	x=(form.Width-len)>>1;
	x+=form.X;
	y=form.Y+form.TitleHeight+10;
	Dis_Strings(text,x,y,FONT24,C_Black,form.BackColor);

	//��ʾ����ѡ���-------------------------------
	x=form.X+15;
	y=form.Y+form.Height-50;
	Dis_TouchButton(HZ_ItemSiFo[0][Lang],x,y,btnw,35,KEY_ESC,0,C_Black,FONT24);

	x=form.X+form.Width-15-btnw;
	Dis_TouchButton(HZ_ItemSiFo[1][Lang],x,y,btnw,35,KEY_OK,1,C_Black,FONT24);

	Key.Value=KEY_No;

	while(1)
	{
		Send_DspReply();		//�����ж�����λ���������		
	
		Read_Key();

		Dis_TouchButtonFlash();
		
		if(Key.Value==KEY_OK)
		{
			ret=Key.Value;
			break;
		}
		else if(Key.Value==KEY_ESC)
		{
			ret=DO_Cancel;
			break;
		}
	}
	
	Touch_KeyPop();
	LcdMemPop();
	Key.Value=KEY_No;
	return(ret);
}

//===================================================================
//����: 			�Զ��������ŵ���ʾλ��
//��ڲ���:
//		inx/y/w/h:	������X,Y���꼰��ȸ߶�
//		width: 		�����������Ŀ��
//		height: 		�����������ĸ߶�
//		first: 		0��ʾ������ʾ������,1��ʾ������ʾ���ұ�
//�޸�ʱ��:		2010-11-15
//===================================================================
void Get_AutoXY(uint16 inx,uint16 iny,uint16 inw,uint16 inh,uint32 width,uint32 height,uint32 first,uint16 *rx,uint16 *ry)
{
enum {
	JIANGE_X=5,
	JIANGE_Y=6,
	JIANGE_Side=20		//������ϵ���С����ֵ
};
uint32 x,y;
uint32 start_x,start_y;
uint32 xy_ok;					//�õ���ȷ��λ��
uint32 count;
	xy_ok=0;
	count=2;
	while(count--)
	{
		//�������ұ���ʾ��λ�ô���------------
		if(first==FIRST_Right)
		{
			x=inx+inw+JIANGE_X+width+JIANGE_Side;
			y=iny+height+JIANGE_Side;
			//�ж��ܷ���ʾ���ұ�-------
			if(x<LCD_XSize)
			{
				start_x=inx+inw+JIANGE_X;
				//��ʾ������----
				if(y<LCD_YSize)
				{
					start_y=iny;
				}
				//��ʾ������----
				else
				{
					start_y=LCD_YSize-height-JIANGE_Side;
				}
				xy_ok=1;
				break;
			}
		}
		
		//������������ʾ��λ�ô���------------
		if(first==FIRST_Down)
		{
			//�ж��ܷ���ʾ���·�---------------
			x=inx+width+JIANGE_X+JIANGE_Side;
			y=iny+inh+height+JIANGE_Y+JIANGE_Side;
			//�ж�������ܷ���ʾ���·�----------
			if(y<LCD_YSize)
			{
				start_y=iny+inh+JIANGE_Y;
				//�ж�������ܷ���ʾ���ҷ�----------
				if(x<LCD_XSize)
				{
					start_x=inx+JIANGE_X;
				}
				else
				{
					start_x=LCD_XSize-width-JIANGE_Side;
				}
				xy_ok=1;
				break;
			}
			
			//�ж��ܷ���ʾ���Ϸ�----
			x=inx+width+JIANGE_X+JIANGE_Side;
			y=height+JIANGE_Y+JIANGE_Side;
			if(y<iny)
			{
				start_y=iny-height-JIANGE_Y;
				//�ж�������ܷ���ʾ���ҷ�----------
				if(x<LCD_XSize)
				{
					start_x=inx+JIANGE_X;
				}
				else
				{
					start_x=LCD_XSize-width-JIANGE_Side;
				}
				xy_ok=1;
				break;
			}
		}

		if(first==FIRST_Down) first=FIRST_Right;
		else first=FIRST_Down;
	}

	//��ʾ����ߴ���------------------------------
	if(xy_ok==0)
	{
		start_x=inx-width-JIANGE_X;
		y=iny+height+JIANGE_Y;
		if(y<LCD_YSize)
		{
			start_y=iny;
		}
		else 
		{
			start_y=LCD_YSize-height-JIANGE_Y;
		}
	}
	
	*rx=start_x;
	*ry=start_y;
}

//===================================================================
//����:			��ʾһ��Windows���͵�ѡ���
//����:
//		text:			ѡ������ʾ��Ϣ�ı�������
//		item:		ѡ��������
//		inx/y/w/h:	��������ʼ���꼰��ȸ߶�
//		k:			��ʼ��ѡ��ֵ
//����: 			0xffffffff��ʾȡ��,������ʾѡ������(��0��ʼ)
//˵��: 			�˳�ʱ�Զ��ָ������帲�ǵ���ʾ��Ϣ
//					�Զ����㴰����
//�޸�ʱ��:		2013-3-7
//===================================================================
uint32 Dis_SelectDialog(const char *text[][LANG_MAX],uint32 item,uint16 inx,uint16 iny,uint16 inw,uint16 inh,uint32 k,uint32 b_color)
{
enum {ASCW=FONT24>>1};
const char *Hz_Selcet[][LANG_MAX]={"��ѡ��","Select",};
uint32 i;
uint32 x,y;
uint32 color;
uint16 rx,ry;
uint32 ret;
uint32 step;
uint32 buttonw;				//��Ŀ���Ŀ��
uint32 ref;
uint32 ref_count;
uint32 kk;
uint32 old;
uint32 len;
char *p;
uint32 asc_num;				//���������ַ�����(���)
S_Form form;
	if((item==0)||(item>9)) return(DO_Cancel);
	step=ASCW;
	old=k;
	Touch_KeyPush();

	len=0;
	//���������ַ�����-----
	for(i=0;i<item;i++)
	{
		p=(char *)text[i][Lang];
		asc_num=0;
		while(1)
		{
			if(*p==0) break;
			asc_num++;
			if(asc_num>20) 
			{
				break;
			}
			p++;
		}

		if(asc_num>len)
		{
			len=asc_num;
		}
	}
	
	//������Ϣ���ݳ�ʼ��----------
	buttonw=len*step+step;
	form.Width=buttonw+30;
	if(form.Width<130) 
	{
		form.Width=130;
		buttonw=form.Width-30;
	}
	form.Height=TITLE_H24+32*item+15;		//����ĸ߶�
	Get_AutoXY(inx,iny,inw,inh,form.Width,form.Height,FIRST_Right,&rx,&ry);
	form.Y=ry;
	form.X=rx;
	form.TitleText=Hz_Selcet[0][Lang];
	form.TitleColor=FORM_Red;
	form.TitleHeight=TITLE_H24;
	form.TitleFont=FONT24;
	form.BackColor=b_color;
	form.FillBack=1;	
	LcdMemPush(form.X,form.Y,form.Width,form.Height);	
	Dis_WindowsForm(&form);
	i=(form.Width-buttonw)>>1;
	x=form.X+i;
	Touch_KeyEn(form.X,form.Y,form.Width,form.TitleHeight,KEY_ESC,item);
	Key.Value=KEY_No;
	ref=REF_Item;
	TchVal.KeyNum[0]=item+1;	

	while(1)
	{
		Send_DspReply();		//�����ж�����λ���������		
	
		if(ref>0)
		{
			if(ref==REF_Now) ref_count=2;
			else ref_count=1;
			for(i=0;i<item;i++)
			{
				if(ref==REF_Now)
				{
					ref_count--;
					if(ref_count==1) i=kk;
					else i=k;
				}
				
				y=form.Y+form.TitleHeight+8+i*32;	
				if(i==k) color=C_Red;
				else color=C_Black;
				Dis_TouchButton(text[i][Lang],x,y,buttonw,30,i+1,i,color,FONT24);
				if(ref==REF_Now)
				{
					if(ref_count==0) break;
					else i=0;
				}
			}
			ref=0;
			kk=k;
		}
		
		Read_Key();

		Dis_TouchButtonFlash();

		if(TchVal.Down==TCH_KeySlide) Key.Value=KEY_No;			//��������Ч
		
		if((Key.Value>0)&&(Key.Value<=item))
		{
			ret=Key.Value-1;
			break;
		}
		else if(Key.Value==KEY_DOWN)
		{
			ref=REF_Now;
			k++;
			if(k>=item) k=0;
		}
		else if(Key.Value==KEY_UP)
		{
			ref=REF_Now;
			if(k>0) k--;
			else k=item-1;
		}
		else if(Key.Value==KEY_OK)
		{
			ret=k;
			break;
		}
		else if(Key.Value==KEY_ESC)
		{
			ret=DO_Cancel;
			break;
		}
	}

	if(ret!=DO_Cancel)
	{
		//���ݱ��޸Ĵ���------
		if(ret!=old)
		{
			Sys.LcdRef=REF_Now;				
			Sys.DatChang=1;			//��ʾ�ı���ѡ��
		}
	}
	Touch_KeyPop();
	LcdMemPop();
	Key.Value=KEY_No;
	return(ret);	
}

//===================================================================
//����: ��ʾһ��������
//dat      : ��ǰ����ֵ(��1��ʼ)
//tol       : ����ֵ
//x         : ��ʼ��x����
//y         : ��ʼ��y����
//height  : �������ĸ߶�
//color_f : ���������ɫ
//color_b : ��Ϣ��ı���ɫ
//˵��: ������ռ����ʾ���Ϊ13
//===================================================================
void Dis_PercentSlider(uint32 dat,uint32 tol,uint32 x,uint32 y,uint32 height,uint32 width,uint32 color_f,uint32 color_b)
{
#define SLIDER_H 	30	//15
uint32 percent;
uint32 m,n;
	width=width-1;
	if(dat>0) dat--;
	if(tol>0) 
	{
		tol--;
		percent=dat*100/tol;
	}
	else percent=0;
	if(percent>100) percent=100;
	Dis_VLine(x,y,height,C_Gray1);
	m=(height-SLIDER_H)*percent/100;
	Dis_BoxUp(x+1,y+m,width,SLIDER_H,color_f);
	//��们�����ϲ��ı���ɫ
	if(m>0)
	{
		Dis_RectFill(x+1,y,width,m,color_b);		
	}
	//��们�����²��ı���ɫ
	n=m+SLIDER_H;
	if(n<height)
	{	
		Dis_RectFill(x+1,y+n,width,height-n,color_b);
	}
}

//===================================================================
//����:			��ʾ������Ϣ(���ȿ���趨)
//��ڲ���:
//		title:			�������ַ���ָ��
//		dat:			��ǰ����Ľ�����ֵ(��1��ʼ)
//		tol:			�ܵĴ�����ֵ
//		width:		����Ŀ��
//		color:		����ı���ɫ
//˵��:			����߶ȹ̶�Ϊ90
//�޸�ʱ��:		2010-11-23
//===================================================================
void Dis_SpeedInfo(const char *title,uint32 dat,uint32 tol,uint32 width,uint32 color)
{
enum {BAR_H=28};
uint32 i;
uint32 percent;
uint32 m;
static uint32 ago_percent=0;		//֮ǰ�İٷֱ�ֵ
static uint32 ago_width=0;			//֮ǰ�Ŀ��
static uint32 now_width=0;			//��ǰ�Ŀ��
S_Form form;
	form.Width=width;
	form.Height=90;				//���ȴ���߶ȹ̶�Ϊ90
	form.X=GET_ScrCenterX(form.Width);		//�����X����̶���ʾ���м�
	form.Y=GET_ScrCenterY(form.Height);		//�����Y����̶����м�
	while(1)
	{
		if(dat>tol) break;
		percent=dat*100/tol;
		//��һ����ʾʱˢ����������----------	
		if(SysFlag.SpeedDis==0)
		{
			SysFlag.SpeedDis=1;
			ago_percent=0;
			ago_width=0;			
			//����ʾ������ǰ�Ļ��渴��
			LcdMemPush(form.X,form.Y,form.Width,form.Height);
			form.TitleText=PTEXT(title);
			form.TitleColor=FORM_Red;
			form.TitleHeight=TITLE_H16;
			form.TitleFont=FONT16;
			form.BackColor=color;
			form.FillBack=1;
			Dis_WindowsFormLeft(&form);
			//�������ĵ�ɫ---------------
			Dis_BoxDown(form.X+10,form.Y+50,form.Width-20,BAR_H,C_Gray1);
			Dis_Strings("%",form.X+form.Width/2+10,form.Y+30,FONT16,C_Blue,color);
		}

		if(percent==ago_percent) break;	//���ٷֱȲ�����ˢ����ʾ(��Լʱ��)
		now_width=(form.Width-20-4)*percent/100;
		m=now_width-ago_width;
		for(i=0;i<24;i++)
		{
			Dis_HLine(form.X+12+ago_width,form.Y+52+i,m,FormTitleColor[FORM_Blue][i]);
		}
		Dis_NumHide(percent,form.X+form.Width/2-16,form.Y+30,3,FONT16,C_Blue,color);
		ago_percent=percent;
		ago_width=now_width;		
		break;
	}
	
	if(dat>=tol)
	{
		if(dat==tol) DelayMs(400);
		if(SysFlag.SpeedDis>0)
		{
			LcdMemPop();
		}
		SysFlag.SpeedDis=0;
	}
}

//===================================================================
//����: 			�Զ�����ͼ�괰����ʾ�����ĵĿ�Ⱥ���ʼ����
//��ڲ���:		
//		icon:		ͼ�겿���ṹָ��
//˵��:			����ռ������ʱ��ѭ�������������
//�޸�ʱ��:		2010-11-3	
//===================================================================
void Get_IconFormPlace(S_Form *form,S_GuiXY *guixy,S_GuiIcon *icon,uint32 ext_h)
{
uint32 i;
uint32 row;
uint32 w,h;
uint32 m;
	w=guixy->RowIcons*guixy->ColSpace+50;
	//����ͼ���ռ�õ�����(�������5��)--------
	row=0;
	m=guixy->MaxIcons;
	for(i=0;i<5;i++)
	{
		if(m>=guixy->RowIcons)
		{
			row++;
			m-=guixy->RowIcons;
		}
		else
		{	
			if(m>0) row++;		//����һ�а�һ����
			break;
		}
	}
	h=guixy->RowSpace*row+form->TitleHeight+40+ext_h;
	form->Width=w;
	form->Height=h;
	form->X=GET_ScrCenterX(form->Width);
	form->Y=GET_ScrCenterY(form->Height);
	guixy->StartX=form->X+25;
	guixy->StartY=form->Y+form->TitleHeight+20;
}

//===================================================================
//����:			��ʾһ��ͼ�겿��(������ʾ�ı�ѡ���)
//�޸�ʱ��:		2013-3-7
//===================================================================
void Dis_IconSelect(S_GuiIcon *icon)
{
uint32 x,y,width,height;
uint32 bmp_w,bmp_h;
uint32 bmp_x,bmp_y;
uint32 m;
	x=icon->X;
	y=icon->Y;
	width=icon->Width;
	height=icon->Height;
	bmp_w=icon->BmpW;
	bmp_h=icon->BmpH;
	if((x>=LCD_XSize)||(y>=LCD_YSize)) return;
	if((x+width)>LCD_XSize)
	{
		width=LCD_XSize-x;
	}
	if((y+height)>LCD_YSize)
	{
		height=LCD_YSize-y;
	}

	//����ͼƬ����ʾλ��----------------
	bmp_x=x+((width-bmp_w)>>1);
	bmp_y=y+5;
	//��ʾͼ��--------------------
	Dis_IconUp(bmp_x,bmp_y,bmp_w,bmp_h,icon->Bmp);

	//��ʾ��ʾ�ı�-------------------
	m=y+bmp_h+(height-bmp_h)/2-8;
	Dis_StringsAligen(icon->Text,ALIGEN_MIDDLE,x,m,width,FONT16,icon->TextColor,icon->BackColor);

	//������ڵ�ǰͼ��--------
	if(icon->Sel>0)
	{
		Dis_RectSome(x,y,width,height,2,icon->RectColor);
	}
	else
	{
		Dis_RectSome(x,y,width,height,2,icon->BackColor);
	}	
}

//===================================================================
//����: 			�Ӵ洢���ж�ȡ��Ļ������Ϣ
//��ڲ���:		
//		guixy:
//		icon:			ͼ�겿���ṹָ��
//		text:			ͼ���ı��Ķ�ά�ַ���ָ������
//		bmp:
//		k:			��ǰ�Ĺ��λ��
//˵��:			
//�޸�ʱ��:		2013-3-7
//===================================================================
void Dis_IconRef(S_GuiXY *guixy,S_GuiIcon *icon,const char *text[][LANG_MAX],uint16 *bmp,uint32 k,uint32 tch_k)
{
uint32 i;
uint16 x,y;
	x=guixy->StartX;
	y=guixy->StartY;
	for(i=0;i<guixy->MaxIcons;i++)
	{
		if((i%guixy->RowIcons)==0)
		{
			x=guixy->StartX;
			if(i>0) y+=guixy->RowSpace;
		}
		else
		{
			x+=guixy->ColSpace;
		}
		
		if(k==i)
		{
			icon->Sel=1;
			icon->TextColor=C_Red;
		}
		else
		{
			icon->Sel=0;
			icon->TextColor=C_White;
		}
		icon->X=x+((guixy->ColSpace-icon->Width)>>1);
		icon->Y=y;
		icon->Bmp=(uint16 *)(bmp+icon->BmpW*icon->BmpH*i);
		icon->Text=text[i][Lang];
		Touch_KeyEn(icon->X+((icon->Width-icon->BmpW)>>1),y+5,icon->BmpW,icon->BmpH,KEY_Tch,tch_k+i);
		TchKey[0][tch_k+i].Flash=1;					//������ʾ����״̬�仯
		TchKey[0][tch_k+i].Bmp=(uint16 *)icon->Bmp;

		Dis_IconSelect(icon);		
	}			
}

//===================================================================
//����: 			ͼ��˵��е�λ���ƶ�������
//��ڲ���:		
//		guixy:		�˵�������ṹָ��	
//		k:			�˵���Ĺ��λ����
//����:			�����Ĳ˵�����λ����
//˵��: 			���ͼ���ֱ�ӽ���˵�
//�޸�ʱ��:		2013-3-7
//===================================================================
uint32 KeyIconMove(S_GuiXY *guixy,uint32 k)
{
uint32 m,n;
	//��λ���������´���------------------
	if(TchVal.Down==TCH_KeyDown)
	{
		if(Key.Value==KEY_Tch)
		{
			TchVal.Down=0;
			k=TchVal.Item[0];
			Sys.LcdRef=REF_Now;
			Key.Value=KEY_OK;
		}
	}

	if(TchVal.Down==TCH_KeySlide) return(k);			//�������Ļ�����Ч

	//���Ƽ����´���-------------------
	if(Key.Value==KEY_RIGHT)
	{
		Sys.LcdRef=REF_Now;
		k++;
		if(k>=guixy->MaxIcons)
		{
			k=0;
		}
	}
	//���Ƽ����´���-------------------
	else if(Key.Value==KEY_LEFT)
	{
		Sys.LcdRef=REF_Now;
		if(k>0) k--;
		else k=guixy->MaxIcons-1;
	}
	//���Ƽ����´���-------------------
	else if(Key.Value==KEY_DOWN)
	{
		Sys.LcdRef=REF_Now;
		k+=guixy->RowIcons;
		if(k>=guixy->MaxIcons)
		{
			k=k%guixy->RowIcons;
		}
	}
	//���Ƽ����´���-------------------
	else if(Key.Value==KEY_UP)
	{
		Sys.LcdRef=REF_Now;
		if(k>=guixy->RowIcons)
		{
			k-=guixy->RowIcons;
		}
		else
		{
			m=guixy->MaxIcons/guixy->RowIcons;
			n=guixy->MaxIcons%guixy->RowIcons;
			if(n>0) m++;
			if(m>0) m--;
			m*=guixy->RowIcons;			//���һ�е���ֵ
			n=k%guixy->RowIcons;
			m=m+n;
			while(1)
			{
				if(m>=guixy->MaxIcons)
				{
					if(m>=guixy->RowIcons)
					{
						m-=guixy->RowIcons;
					}
					else
					{
						break;
					}
				}
				else
				{
					k=m;
					break;
				}
			}
		}
	}
	return(k);
}

//===================================================================
//����:			��������봦��(ֻ��������)
//����:			
//		mima:		����ֵ
//����: 			1��ʾ����������ȷ
//˵��:			����ֵ���Ϊ9999 
//�޸�ʱ��:		2013-4-18
//===================================================================
uint32 Dis_PassWord(uint32 mima)
{
enum {ASCW=FONT24>>1,FONT=FONT24};
uint8 code[4];
uint8 dat[5]={0,0,0,0,0};
uint32 len;
uint32 i;
uint32 x,y;
uint32 code_ok=0;
S_Form form;
	len=0;
	code[0]=mima/1000;
	code[1]=(mima%1000)/100;
	code[2]=(mima%100)/10;
	code[3]=mima%10;
	if(Sys.TouchEn==0x55)
	{
		form.Y=(LCD_YSize>>1)-60;
	}
	else
	{
		form.Y=(LCD_YSize>>1)-30;
	}
	form.Width=240;
	form.Height=60;
	form.X=GET_ScrCenterX(form.Width);
	LcdMemPush(form.X,form.Y,form.Width,form.Height);
	Dis_RectFill(form.X,form.Y,form.Width,form.Height,C_Black);
	Dis_Rect(form.X,form.Y,form.Width,form.Height,C_Yellow);
	Dis_Rect(form.X+2,form.Y+2,form.Width-4,form.Height-4,C_Yellow);
	Dis_Strings(HZ_SuRuMiMa[0][Lang],form.X+20,form.Y+20,FONT,C_Red,C_Black);

	TchVal.RefX=form.X+form.Width-60;
	TchVal.RefY=form.Y+16;
	TchVal.RefWidth=70;
	TchVal.RefHeight=30;
	Dis_TouchKeyBoard(TKEY_BoardAsc);

	while(1)
	{
		Send_DspReply();		//�����ж�����λ���������	
	
		Read_Key();

		Dis_TouchButtonFlash();			//ˢ��ģ�ⴥ�����̵ı仯״̬		

		if(Sys.LcdRef>0)
		{
			Sys.LcdRef=0;
			x=form.X+20+ASCW*12;
			y=form.Y+20;
			for(i=0;i<5;i++)
			{
				if(i<len) Dis_Strings("*",x+i*ASCW,y,FONT24,C_White,C_Black);
				else Dis_Strings(" ",x+i*ASCW,y,FONT,C_Black,C_Black);
			}
		}

		if(Key.Value<=KEY_9)
		{
			if(len<5)
			{
				len++;
				Sys.LcdRef=1;
				dat[len-1]=Key.Value;
			}
		}
		//ɾ��һ����ֵ---------
		else if(Key.Value==KEY_QUK)
		{
			if(len>0)
			{
				Sys.LcdRef=1;
				dat[len-1]=0;
				len--;
			}
		}

		if(Key.Value==KEY_OK)
		{
			if(len!=4) Key.Value=KEY_ESC;
			else
			{
				for(i=0;i<4;i++)
				{
					if(dat[i]!=code[i]) 
					{
						Key.Value=KEY_ESC;
						break;
					}
				}
				if(i>=4) 
				{
					code_ok=1;
					break;
				}
			}
		}
		
		if(Key.Value==KEY_ESC)
		{
			code_ok=0;
			break;
		}
	}

	TouchKeyBoardClose();
	LcdMemPop();
	Key.Value=KEY_No;
	return(code_ok);
}

//===================================================================
//����:			�����������봦��(6λ����,����Ϊ0~F)
//����:
//		code:		�����ַ�����ָ��
//		x:			��������ʼX����
//		y:			��������ʼY����
//����:			1��ʾ����������ȷ
//˵��:			
//�޸�ʱ��:		2013-4-18
//===================================================================
uint32 Dis_SuperPassWord(uint8 *code)
{
enum {ASCW=FONT24>>1,FONT=FONT24};
uint8 dat[6]={0,0,0,0,0,0};
uint32 len;
uint32 i;
uint32 x;
uint32 code_ok=0;
S_Form form;
	Touch_KeyPush();
	len=0;
	if(Sys.TouchEn==0x55)
	{
		form.Y=(LCD_YSize>>1)-60;
	}
	else
	{
		form.Y=(LCD_YSize>>1)-30;
	}
	form.Width=250;
	form.Height=60;
	form.X=GET_ScrCenterX(form.Width);
	LcdMemPush(form.X,form.Y,form.Width,form.Height);
	Dis_RectFill(form.X,form.Y,form.Width,form.Height,C_Black);
	Dis_Rect(form.X,form.Y,form.Width,form.Height,C_Yellow);
	Dis_Rect(form.X+2,form.Y+2,form.Width-4,form.Height-4,C_Yellow);
	Dis_Strings(HZ_SuRuMiMa[0][Lang],form.X+20,form.Y+20,FONT,C_Red,C_Black);	
	TchVal.RefX=form.X+form.Width-60;
	TchVal.RefY=form.Y+20;
	TchVal.RefWidth=70;
	TchVal.RefHeight=30;
	Dis_TouchKeyBoard(TKEY_BoardAsc);

	while(1)
	{
		Send_DspReply();		//�����ж�����λ���������	
	
		Read_Key();

		Dis_TouchButtonFlash();			//ˢ��ģ�ⴥ�����̵ı仯״̬		

		if(Sys.LcdRef>0)
		{
			Sys.LcdRef=0;
			x=form.X+20+ASCW*12;
			for(i=0;i<6;i++)
			{
				if(i<len) Dis_Strings("#",x+i*ASCW,form.Y+20,FONT,C_White,C_Black);
				else Dis_Strings(" ",x+i*ASCW,form.Y+20,FONT,C_Black,C_Black);
			}
		}

		if(Key.Value<=KEY_F)
		{
			if(len<6)
			{
				len++;
				Sys.LcdRef=1;
				dat[len-1]=Key.Value;
			}
		}
		//ɾ��һ����ֵ---------
		else if(Key.Value==KEY_QUK)
		{
			if(len>0)
			{
				Sys.LcdRef=1;
				dat[len-1]=0;
				len--;
			}
		}

		if(Key.Value==KEY_OK)
		{
			if(len!=6) Key.Value=KEY_ESC;
			else
			{
				for(i=0;i<6;i++)
				{
					if(dat[i]!=code[i]) 
					{
						Key.Value=KEY_ESC;
						break;
					}
				}
				if(i>=6) 
				{
					code_ok=1;
					break;
				}
			}
		}
		
		if(Key.Value==KEY_ESC)
		{
			code_ok=0;
			break;
		}
	}

	TouchKeyBoardClose();
	LcdMemPop();
	Touch_KeyPop();
	Key.Value=KEY_No;
	return(code_ok);
}

//===================================================================
//���� : ��ʾ�ļ���(����չ��)
//fdt        : 32�ֽڵ��ļ�Ŀ¼����Ϣ
//ext        : 0��ʾ����ʾ��չ��,1��ʾ��ʾ��չ��
//x          : ��ʼ��x����
//y          : ��ʼ��y����
//color_f  : ���ֵ���ɫ
//color_b : ���ֵı���ɫ
//===================================================================
void Dis_FileName(uint8 *fdt,uint32 ext,uint16 x,uint16 y,uint16 color_f,uint16 color_b)
{
uint32 i;
uint32 hz_low;
uint32 hz;
uint32 dat;
uint32 dis_len=0;
	//�ļ���
	hz_low=0;
	for(i=0;i<8;i++)
	{
		dat=*(fdt+i);
		if(dat==0x20) break;
		else if(dat<0xa0)
		{
			Dis_Asc(dat,x,y,color_f,color_b);
			dis_len+=8;
		}
		else
		{
			if(hz_low==0) 
			{
				hz=dat;
				hz*=256;
				hz_low=1;
			}
			else
			{
				hz+=dat;
				hz_low=0;
				Dis_Chinese(hz,x-8,y,color_f,color_b);
				dis_len+=16;
			}
		}
		x+=8;
	}
	//����ʾ��չ��---
	if(ext==0) 
	{
		if(dis_len<64)
		{
			Dis_RectFill(x,y,64-dis_len,16,color_b);
		}
		return;				
	}
	//��չ��--------------------------------------
	Dis_Strings(".",x,y,FONT16,color_f,color_b);
	dis_len+=8;
	x+=8;
	for(i=0;i<3;i++)
	{
		dat=*(fdt+8+i);
		if(dat==0x20) break;
		Dis_Asc(dat,x,y,color_f,color_b);
		dis_len+=8;
		x+=8;
	}

	if(dis_len<96)
	{
		Dis_RectFill(x,y,96-dis_len,16,color_b);
	}
}

//===================================================================
//���� : ��ʾ�ļ��Ĵ�С
//fdt        : 32�ֽڵ��ļ�Ŀ¼����Ϣ
//x          : ��ʼ��x����
//y          : ��ʼ��y����
//color_f  : ���ֵ���ɫ
//color_b : ���ֵı���ɫ
//===================================================================
void Dis_FileSize(uint8 *fdt,uint16 x,uint16 y,uint16 color_f,uint16 color_b)
{
uint32 i;
uint32 bytes;
uint32 wei[7];
uint32 cusu[7]={1000000,100000,10000,1000,100,10,1};
uint32 start_wei=0;
uint32 dis_len=0;
	bytes=*(fdt+0x1c)+*(fdt+0x1d)*0x100+*(fdt+0x1e)*0x10000+*(fdt+0x1f)*0x10000000;
	if(bytes>9999999) bytes=9999999;
	for(i=0;i<7;i++)
	{
		wei[i]=bytes/cusu[i];
		bytes=bytes%cusu[i];
	}
	start_wei=0;
	for(i=0;i<7;i++)
	{
		if((wei[i]>0)||(start_wei>0)||(i==6))
		{
			start_wei=1;
			Dis_NumHide(wei[i],x,y,1,FONT16,color_f,color_b);
			dis_len+=8;
			x+=8;
		}
	}

	if(dis_len<56)
	{
		Dis_RectFill(x,y,56-dis_len,16,color_b);
	}
}

//===================================================================
//���� : ��ʾ�ļ��Ĵ�С
//fdt        : 32�ֽڵ��ļ�Ŀ¼����Ϣ
//x          : ��ʼ��x����
//y          : ��ʼ��y����
//color_f  : ���ֵ���ɫ
//color_b : ���ֵı���ɫ
//===================================================================
void Dis_FileTime(uint8 *fdt,uint16 x,uint16 y,uint16 color_f,uint16 color_b)
{
	Get_FileTime(fdt,&FatTime);
	Dis_NumAll(FatTime.Year,x,y,4,FONT16,color_f,color_b);
	Dis_Strings("-",x+32,y,FONT16,color_f,color_b);
	Dis_NumAll(FatTime.Month,x+40,y,2,FONT16,color_f,color_b);
	Dis_Strings("-",x+56,y,FONT16,color_f,color_b);
	Dis_NumAll(FatTime.Data,x+64,y,2,FONT16,color_f,color_b);
	Dis_RectFill(x+80,y,10,16,color_b);
	x+=90;
	Dis_NumAll(FatTime.Hour,x,y,2,FONT16,color_f,color_b);
	Dis_Strings(":",x+16,y,FONT16,color_f,color_b);
	Dis_NumAll(FatTime.Minute,x+24,y,2,FONT16,color_f,color_b);
	Dis_Strings(":",x+40,y,FONT16,color_f,color_b);
	Dis_NumAll(FatTime.Second,x+48,y,2,FONT16,color_f,color_b);
}

//===================================================================
//����:			��ʾ�ļ���Ϣ(�ļ���,��չ��,��С,�޸�ʱ��)
//��ڲ���:	
//		fdt: 			�ļ�Ŀ¼������ָ��(32�ֽ�)	
//		ext: 			��ʾ�Ƿ�Ҫ��ʾ��չ��,��Ϊ0��ʾ
//		x,y:			��ʾλ�õ�x,y����ֵ
//		color_f:		��ʾ�ַ���ǰ��ɫ
//		color_b:		��ʾ�ַ��ı���ɫ
//˵��:			֧���ļ���
//�޸�ʱ��:		2011-4-28
//===================================================================
void Dis_FileInfoFlord(uint8 *fdt,uint32 ext,uint16 x,uint16 y,uint16 color_f,uint16 color_b)
{
	//��ǰĿ¼�����Ŀ¼����-----------------
	if(fdt[FAT_Properties]!=FAT_Dir)
	{
		Dis_FileName(fdt,ext,x,y,color_f,color_b);
		if(ext>0) 
		{
			Dis_RectFill(x+12*8,y,115-12*8,16,color_b);
			x+=115;
		}
		else 
		{
			Dis_RectFill(x+8*8,y,70-8*8,16,color_b);
			x+=70;
		}
		Dis_FileSize(fdt,x,y,color_f,color_b);
		Dis_RectFill(x+8*7,y,80-8*7,16,color_b);
		Dis_FileTime(fdt,x+80,y,color_f,color_b);		
	}
	//���ļ��д���----------------------
	else
	{
		//��ǰ����Ŀ¼�´���-----
		if(fdt[0]=='.')
		{
			Dis_RectFill(x,y,115+80+8*20,16, C_Black);
			Dis_Strings(Hz_FatDir[1][Lang],x,y,FONT16,color_f,color_b);
		}
		else
		{
			Dis_RectFill(x,y,115+80+8*20,16, C_Black);
			Dis_FileName(fdt,0,x,y,color_f,color_b);
			x+=115;
			Dis_Strings(Hz_FatDir[0][Lang],x,y,FONT16,color_f,C_Black);
//			color_b=C_Black;
//			Dis_RectFill(x+8*7,y,80-8*7,16,color_b);
//			Dis_FileTime(fdt,x+80,y,color_f,color_b);					
		}
	}
}

//===================================================================
//����:			���ļ��б��������(����ʾ�ļ���)
//��ڲ���:
//		tol:			�ļ��б��е����ļ���
//		file_list:		�ļ��б��Ŀ¼����Ϣ(�����������б�)
//˵��:			
//�޸�ʱ��:		2011-5-7
//===================================================================
void File_SortByFloder(uint32 tol,uint8 *file_list)
{
uint32 i,j;
uint32 m;
uint8 fdt[MAX_Files][32];
uint32 floders;					//�б��е��ļ��и���
uint32 files;					//�б��е��ļ�����	
uint32 floder_count;
uint32 file_count;
	if(tol<=1) return;			//������2���ļ���ֱ�ӷ���

	floders=0;
	files=0;
	//��ȡ�ļ��к��ļ��ĸ���-------
	m=FAT_Properties;
	for(i=0;i<tol;i++)
	{
		if(file_list[m]==FAT_Dir)
		{
			floders++;
		}
		else
		{
			files++;
		}
		m+=32;
	}

	floder_count=0;
	file_count=floders;

	//��ԭʼ�ļ��б�����-------
	m=0;
	for(i=0;i<tol;i++)
	{
		if(file_list[m+FAT_Properties]==FAT_Dir)
		{
			for(j=0;j<32;j++)
			{
				fdt[floder_count][j]=file_list[m+j];
			}
			floder_count++;
		}
		else
		{
			for(j=0;j<32;j++)
			{
				fdt[file_count][j]=file_list[m+j];
			}
			file_count++;
		}
		m+=32;
	}
	
	//���������б���-----
	m=0;
	for(i=0;i<tol;i++)
	{
		for(j=0;j<32;j++)
		{
			file_list[m+j]=fdt[i][j];
		}
		m+=32;
	}
}

//===================================================================
//����:			���ļ�(ָ����չ��)�б����ѡ��һ���ļ�
//����:
//		title:			��������ʾ���ı�(������ʾ)
//		file_fdt:		���ݽ�������չ��(ǰ3�ֽ�),���ص�ǰѡ�е��ļ���Ϣ
//		device:		�豸���� 0=SD��,1=U��,2=NandFlash
//����:			0xffffffff��ʾȡ������
//�޸�ʱ��:		2013-3-7
//===================================================================
uint32 Dis_FileSelectOne(const char *title,uint8 *file_fdt,uint32 device)
{
enum {MAX_Num=10,BOXH=220,SLIDW=25};
uint32 k=0;						//��ǰ����ڱ�ҳ�����
uint32 k_start=0;					//�������ϵ����
uint32 i;
uint32 m;
uint32 x,y;
uint8 file_list[32*MAX_Files];
uint32 file_tol;
uint32 color_f,color_b;
uint32 flag;
uint32 ret;
uint32 ref_list;					// 1��ʾ���¶�ȡ�ļ���Ϣ
uint8 tkey[]={KEY_CE,KEY_OK};
S_Form form;
S_GuiXY guixy;
	Touch_KeyPush();
	ref_list=1;
	file_tol=0;
	TchVal.Item[0]=k;

	form.Width=420;
	form.Height=320;
	form.X=GET_ScrCenterX(form.Width);
	form.Y=GET_ScrCenterY(form.Height);
	form.TitleText=title;
	form.TitleColor=FORM_Red;
	form.TitleHeight=TITLE_H24;
	form.TitleFont=FONT24;
	form.BackColor=C_Back;
	form.FillBack=1;

	guixy.StartX=form.X+20;
	guixy.StartY=form.Y+form.TitleHeight+20;

	Sys.LcdRef=REF_Item;
	Sys.DatChang=0;
	LcdMemPush(form.X,form.Y,form.Width,form.Height);
	Dis_WindowsForm(&form);
	x=form.X+10;y=form.Y+form.Height-40;	
	Dis_Strings(HZ_FileTol[0][Lang],x,y,FONT16,C_Black,C_Back);
	Touch_KeyEn(form.X,form.Y,form.Width,form.TitleHeight,KEY_ESC,10);

	x=form.X+180;
	y=form.Y+form.Height-50;
	for(i=1;i<3;i++)
	{
		Dis_TouchButton(Hz_SelDel[i][Lang],x,y,100,40,tkey[i-1],13+i-1,C_Black,FONT24);
		x+=120;
	}
	TchVal.KeyNum[0]=MAX_Num+5;

	FatDir.SubDir[device]=0;			//��ʾ�ڸ�Ŀ¼��
	while(1)
	{
		Send_DspReply();		//�����ж�����λ���������	
		
		if(ref_list>0)
		{
			ref_list=0;
			Dis_BoxDown(guixy.StartX-10,guixy.StartY-10,form.Width-20,BOXH,C_Black);
			if(device==NAND) i=MAX_MemFiles;
			else i=MAX_Files;
			file_tol=ListFileNameFloder(file_fdt,i,file_list,device);	
			File_SortByFloder(file_tol,file_list);
			Dis_NumHide(file_tol,form.X+10+60,form.Y+form.Height-40,4,FONT16,C_Red,C_Back);
		}
	
		if(Sys.LcdRef>0)
		{
			x=form.X+form.Width-10-SLIDW-2;
			Dis_PercentSlider(k+k_start+1,file_tol,x,guixy.StartY-10+2,BOXH-4,SLIDW,C_Green,C_Gray1);
			Touch_KeyEn(x,guixy.StartY-10+2,SLIDW,60,KEY_PU,11);				//�Ϸ���
			Touch_KeyEn(x,guixy.StartY-10+BOXH-60,SLIDW,60,KEY_PD,12);		//�·���

			x=guixy.StartX;
			y=guixy.StartY;
			for(i=0;i<10;i++)
			{
				if(k==i) 
				{	
					color_f=C_Red;
					color_b=C_Yellow;
				}
				else 
				{
					color_f=C_White;
					color_b=C_Black;
				}

				m=k_start+i;
				if(m>=file_tol)
				{
					Dis_RectFill(x,y,form.Width-60,16,C_Black);
					TouchKeyDis(i);
				}
				else
				{
					Dis_FileInfoFlord(&file_list[(i+k_start)*32],1,x,y,color_f,color_b);
					Touch_KeyEn(x,y,320,16,KEY_Tch,i);
				}
				y+=20;
			}
			Sys.k=k;
			Sys.LcdRef=0;
		}

		Read_Key();

		Dis_TouchButtonFlash();
		
		//��Ч�Ĵ����㰴�´���(���)---------
		if((TchVal.Down==TCH_KeyDown)&&(Key.Value==KEY_Tch))
		{
			TchVal.Down=0;
			if(k!=TchVal.Item[0])
			{
				k=TchVal.Item[0];
				Sys.LcdRef=REF_Now;
			}
			//Ŀ¼��˫����ȷ��������---------
			else
			{
				m=k_start+k;
				if((m+1)<=file_tol)
				{
					m*=32;
					if(file_list[m+FAT_Properties]==FAT_Dir)
					{
						Key.Value=KEY_OK;
					}
				}
			}
		}

		//����һ��-----------------
		if(Key.Value==KEY_DOWN)
		{
			if((k<(MAX_Num-1))&&((k_start+k)<(file_tol-1)))
			{	
				Sys.LcdRef=REF_Item;			
				k++;
			}
			else if((k_start+MAX_Num)<file_tol)
			{
				Sys.LcdRef=REF_Item;	
				k_start++;
			}
		}
		//����һ��-----------------
		else if(Key.Value==KEY_UP)
		{
			if(k>0) 
			{
				Sys.LcdRef=REF_Item;			
				k--;
			}
			else if(k_start>0)
			{
				Sys.LcdRef=REF_Item;
				k_start--;
			}
		}
		//�·�һҳ----------------
		else if(Key.Value==KEY_PD)
		{
			if(file_tol>(k_start+MAX_Num))
			{
				Sys.LcdRef=REF_Item;
				k_start+=MAX_Num;
				if(k>=(file_tol-k_start))
				{
					k=file_tol-k_start-1;		//��굽���һ��
				}
			}
		}
		//�Ϸ�һҳ----------------
		else if(Key.Value==KEY_PU)
		{
			if(k_start>=MAX_Num)
			{
				Sys.LcdRef=REF_Item;
				k_start-=MAX_Num;
			}
			else if(k_start>0)
			{
				Sys.LcdRef=REF_Item;
				k_start=0;
			}
		}	
		else if((Key.Value==KEY_CE)&&(file_tol>0))
		{
			//�жϵ�ǰ�ļ��Ƿ����ļ���----------
			m=(k+k_start)*32;
			if(file_list[m+FAT_Properties]==FAT_Dir)
			{
				Dis_HintForm(HZ_TiSi[0][Lang],HZ_FileOptHint[FILE_DelFloderErr][Lang],C_Black,C_Back);				
			}
			else
			{
				flag=Dis_SureDialog(HZ_TiSi[0][Lang],HZ_SureDelFile[0][Lang],C_Back);
				//��ȷ������ɾ��
				if(flag!=DO_Cancel)
				{
					if(FatDir.SubDir[device]!=0x55)
					{
						file_list[m+26]=0;
						file_list[m+27]=0;
						file_list[m+20]=0;
						file_list[m+21]=0;
					}
					flag=DeleteFileFloder(&file_list[m],device);
					if(flag==0)
					{
						Dis_HintForm(HZ_TiSi[0][Lang],HZ_FileOptHint[FILE_DelOk][Lang],C_Black,C_Back);
						//����ǰ�������һ���ϴ���
						if((k+1)==file_tol)
						{
							if(k>0) k-=1;
						}
					}
					ref_list=1;
					Sys.LcdRef=REF_Now;
				}
			}
		}
		else if(Key.Value==KEY_OK)
		{
			m=(k+k_start)*32;
			//�ļ��д���------------------------------
			if(file_list[m+FAT_Properties]==FAT_Dir)
			{	
				//���ص��ϼ�Ŀ¼����------------------
				if(file_list[m]=='.')
				{
					ref_list=1;
					FatDir.CurrentDir[26]=FatDir.ParentDir[26];
					FatDir.CurrentDir[27]=FatDir.ParentDir[27];
					FatDir.CurrentDir[20]=FatDir.ParentDir[20];
					FatDir.CurrentDir[21]=FatDir.ParentDir[21];					
					if((FatDir.CurrentDir[26]==0)&&(FatDir.CurrentDir[27]==0)&&(FatDir.CurrentDir[20]==0)&&(FatDir.CurrentDir[21]==0))
					{
						FatDir.SubDir[device]=0;
					}
					k=0;					//����Ƶ���1��
					k_start=0;
					Sys.LcdRef=REF_Item;
				}
				//�����ļ���-----------
				else
				{
					ref_list=1;
					FatDir.SubDir[device]=0x55;
					FatDir.CurrentDir[26]=file_list[m+26];
					FatDir.CurrentDir[27]=file_list[m+27];
					FatDir.CurrentDir[20]=file_list[m+20];
					FatDir.CurrentDir[21]=file_list[m+21];
					k=0;					//����Ƶ���1��
					k_start=0;
					Sys.LcdRef=REF_Item;
				}
			}
			else
			{
				if(file_tol==0) ret=DO_Cancel;
				else
				{
					ret=0;
					for(i=0;i<32;i++)
					{
						file_fdt[i]=file_list[m+i];
					}
					//���ݵ�ǰ��Ŀ¼�Ĵغ�--------
					if(FatDir.SubDir[device]==0x55)
					{
						file_fdt[26]=file_list[26];
						file_fdt[27]=file_list[27];
						file_fdt[20]=file_list[20];
						file_fdt[21]=file_list[21];
					}
					else
					{
						file_fdt[26]=0;
						file_fdt[27]=0;
						file_fdt[20]=0;
						file_fdt[21]=0;
					}
				}
				break;
			}
		}
		if(Key.Value==KEY_ESC)
		{
			ret=DO_Cancel;
			break;
		}
	}
	Key.Value=KEY_No;
	Touch_KeyPop();
	LcdMemPop();
	return(ret);
}

//===================================================================
//����:			���ļ�(ָ����չ��)�б����ѡ��һ�������ļ�
//��ڲ���:
//		title:			��������ʾ���ı�ָ��(������ʾ)
//		file_fdt:		���ݽ�������չ��ָ��(ǰ3�ֽ�)
//		sel_list:		ѡ�е��ļ�Ŀ¼��Ϣָ��
//˵��:			�������˵���ȫ��ѡ��
//����:			���صĸ���,0��ʾû��ѡ��
//�޸�ʱ��:		2013-4-18
//===================================================================
uint32 Dis_FileSelectMany(const char *title,uint8 *file_fdt,uint8 *sel_list)
{
enum {MAX_Num=10,BOX_H=220,SLID_W=20};
uint32 k=0;						//��ǰ����ڱ�ҳ�����
uint32 k_start=0;					//�������ϵ����
uint32 i,j;
uint32 m;
uint32 x,y;
uint8 file_list[32*MAX_Files];			//����Ҫ����ļ��б�
uint8 file_sel[MAX_Files];			// 1��ʾ�ļ���ѡ��
uint32 file_tol;
uint32 sel_tol;						//ѡ�е��ļ���
uint32 color_f,color_b;
uint32 flag;
uint32 ref_list;					// 1��ʾ���¶�ȡ�ļ���Ϣ
uint8 tkey[]={KEY_POP,KEY_CE,KEY_OK};
S_Form form;
S_GuiXY guixy;
	Touch_KeyPush();
	ref_list=1;
	sel_tol=0;
	form.Width=420;
	form.Height=320;
	form.X=GET_ScrCenterX(form.Width);
	form.Y=GET_ScrCenterY(form.Height);
	form.TitleText=title;
	form.TitleColor=FORM_Red;
	form.TitleHeight=TITLE_H24;
	form.TitleFont=FONT24;
	form.BackColor=C_Back;
	form.FillBack=1;

	guixy.StartX=form.X+32;
	guixy.StartY=form.Y+form.TitleHeight+20;
	guixy.RowSpace=20;

	TchVal.Item[0]=k;
	Sys.LcdRef=REF_Item;
	Sys.DatChang=0;
	for(i=0;i<MAX_Files;i++) file_sel[i]=0;
	LcdMemPush(form.X,form.Y,form.Width,form.Height);
	Dis_WindowsForm(&form);
	Touch_KeyEn(form.X,form.Y,form.Width,form.TitleHeight,KEY_ESC,10);
	Dis_Strings(HZ_FileTol[0][Lang],form.X+10,form.Y+form.Height-40,FONT16,C_Black,C_Back);

	//�ײ����ܼ�-----------------
	x=form.X+130;
	y=form.Y+form.Height-50;
	for(i=0;i<3;i++)
	{
		Dis_TouchButton(Hz_SelDel[i][Lang],x,y,85,40,tkey[i],13+i,C_Black,FONT24);
		x+=90;
	}
	
	TchVal.KeyNum[0]=MAX_Num+6;
	FatDir.SubDir[USB]=0;			//��ʾ�ڸ�Ŀ¼��
	
	while(1)
	{
		Send_DspReply();		//�����ж�����λ���������	
		
		if(ref_list>0)
		{
			ref_list=0;
			Dis_BoxDown(form.X+6,guixy.StartY-7,form.Width-12,BOX_H,C_Black);
			file_tol=ListFileNameFloder(file_fdt,MAX_Files,file_list,USB);	
			File_SortByFloder(file_tol,file_list);
			Dis_NumHide(file_tol,form.X+10+60,form.Y+form.Height-40,4,FONT16,C_Red,C_Back);
		}
	
		if(Sys.LcdRef>0)
		{
			x=form.X+form.Width-SLID_W-6-2;
			y=guixy.StartY-5;
			Dis_PercentSlider(k+k_start+1,file_tol,x,y,BOX_H-4,20,C_Green,C_Black);
			Touch_KeyEn(x,y+2,SLID_W,60,KEY_PU,11);			//�Ϸ���
			Touch_KeyEn(x,y+BOX_H-60-4,20,60,KEY_PD,12);		//�·���
			x=guixy.StartX;
			y=guixy.StartY;
			for(i=0;i<10;i++)
			{
				if(k==i) 
				{
					color_f=C_Red;
					color_b=C_Yellow;
				}
				else 
				{
					color_f=C_White;
					color_b=C_Black;
				}
				
				m=k_start+i;
				if(m>=file_tol) 
				{
					file_sel[m]=0;	
					Dis_RectFill(form.X+9,y-1,form.Width-40,18,C_Black);
					TouchKeyDis(i);
				}
				else
				{
					//�ļ���ǰ����ʾѡ���----
					if(file_list[m*32+FAT_Properties]==FAT_Dir)
					{
						file_sel[m]=0;			//Ŀ¼��ܱ�ѡ��
						Dis_Strings("  ",form.X+10,y,FONT16,C_Black,C_Black);
						Dis_Rect(form.X+9,y-1,18,18,C_Black);				
					}
					else
					{
						if(file_sel[m]==1) Dis_Strings("��",form.X+10,y,FONT16,C_Green,C_Black);
						else Dis_Strings("  ",form.X+10,y,FONT16,C_Black,C_Black);
						Dis_Rect(form.X+9,y-1,18,18,C_Gray1);				
					}
					Dis_FileInfoFlord(&file_list[m*32],1,x,y,color_f,color_b);
					Touch_KeyEn(form.X+5,y,330,16,KEY_Tch,i);
				}
				y+=20;
			}
			Sys.k=k;
			Sys.LcdRef=0;
		}

		Read_Key();

		Dis_TouchButtonFlash();

		//��Ч�Ĵ����㰴�´���(���)---------
		if((TchVal.Down==TCH_KeyDown)&&(Key.Value==KEY_Tch))
		{
			TchVal.Down=0;
			if(k!=TchVal.Item[0])
			{
				k=TchVal.Item[0];
				Sys.LcdRef=REF_Now;
			}
			//Ŀ¼��˫����ȷ��������---------
			else
			{
				m=k_start+k;
				if((m+1)<=file_tol)
				{
					m*=32;
					if(file_list[m+FAT_Properties]==FAT_Dir)
					{
						Key.Value=KEY_OK;
					}
					//����Ŀ¼���Ի���ѡ�������----
					else
					{
						Key.Value=KEY_SEL;
					}
				}
			}						
		}

		//����һ��-----------------
		if(Key.Value==KEY_DOWN)
		{
			if((k<(MAX_Num-1))&&((k_start+k)<(file_tol-1)))
			{	
				Sys.LcdRef=REF_Item;			
				k++;
			}
			else if((k_start+MAX_Num)<file_tol)
			{
				Sys.LcdRef=REF_Item;	
				k_start++;
			}
		}
		//����һ��-----------------
		else if(Key.Value==KEY_UP)
		{
			if(k>0) 
			{
				Sys.LcdRef=REF_Item;			
				k--;
			}
			else if(k_start>0)
			{
				Sys.LcdRef=REF_Item;
				k_start--;
			}
		}
		//�·�һҳ----------------
		else if(Key.Value==KEY_PD)
		{
			if(file_tol>(k_start+MAX_Num))
			{
				Sys.LcdRef=REF_Item;
				k_start+=MAX_Num;
				if(k>=(file_tol-k_start))
				{
					k=file_tol-k_start-1;		//��굽���һ��
				}
			}
		}
		//�Ϸ�һҳ----------------
		else if(Key.Value==KEY_PU)
		{
			if(k_start>=MAX_Num)
			{
				Sys.LcdRef=REF_Item;
				k_start-=MAX_Num;
			}
			else if(k_start>0)
			{
				Sys.LcdRef=REF_Item;
				k_start=0;
			}
		}				
		//ѡ���-------------------
		else if(Key.Value==KEY_SEL)
		{
			Sys.LcdRef=REF_Now;
			i=k+k_start;
			//�������ļ�������------
			if(file_list[i*32+FAT_Properties]!=FAT_Dir)
			{
				if(file_sel[i]==1) file_sel[i]=0;
				else file_sel[i]=1;
			}
		}
		//ѡ��ȫ���ļ�----------
		else if(Key.Value==KEY_POP)
		{
			Sys.LcdRef=REF_Item;
			i=k+k_start;
			//�����Ŀ¼�ϲ���ȫѡ---------
			if(file_list[i*32+FAT_Properties]!=FAT_Dir)
			{
				for(i=0;i<file_tol;i++)
				{
					//�������ļ�����ѡ��------
					if(file_list[i*32+FAT_Properties]!=FAT_Dir)
					{
						file_sel[i]=1;
					}
				}
			}
		}
		else if((Key.Value==KEY_CE)&&(file_tol>0))
		{
			//�жϵ�ǰ�ļ��Ƿ����ļ���----------
			m=(k+k_start)*32;
			if(file_list[m+FAT_Properties]==FAT_Dir)
			{
				Dis_HintForm(HZ_TiSi[0][Lang],HZ_FileOptHint[FILE_DelFloderErr][Lang],C_Black,C_Back);				
			}
			else
			{
				flag=Dis_SureDialog(HZ_TiSi[0][Lang],HZ_SureDelFile[0][Lang],C_Back);
				//��ȷ������ɾ��
				if(flag!=DO_Cancel)
				{
					if(FatDir.SubDir[USB]!=0x55)
					{
						file_list[m+26]=0;
						file_list[m+27]=0;
						file_list[m+20]=0;
						file_list[m+21]=0;
					}				
					flag=DeleteFileFloder(&file_list[m],USB);
					if(flag==0)
					{
						Dis_HintForm(HZ_TiSi[0][Lang],HZ_FileOptHint[FILE_DelOk][Lang],C_Black,C_Back);
						//����ǰ�������һ���ϴ���
						if((k+1)==file_tol)
						{
							if(k>0) k-=1;
						}
					}
					ref_list=1;
					Sys.LcdRef=REF_Now;
				}
			}
		}
		else if(Key.Value==KEY_OK)
		{
			m=(k+k_start)*32;
			//�ļ��д���------------------------------
			if(file_list[m+FAT_Properties]==FAT_Dir)
			{	
				for(i=0;i<MAX_Files;i++) file_sel[i]=0;
				//���ص��ϼ�Ŀ¼����------------------
				if(file_list[m]=='.')
				{
					ref_list=1;
					FatDir.CurrentDir[26]=FatDir.ParentDir[26];
					FatDir.CurrentDir[27]=FatDir.ParentDir[27];
					FatDir.CurrentDir[20]=FatDir.ParentDir[20];
					FatDir.CurrentDir[21]=FatDir.ParentDir[21];					
					if((FatDir.CurrentDir[26]==0)&&(FatDir.CurrentDir[27]==0)&&(FatDir.CurrentDir[20]==0)&&(FatDir.CurrentDir[21]==0))
					{
						FatDir.SubDir[USB]=0;
					}
					k=0;					//����Ƶ���1��
					k_start=0;
					Sys.LcdRef=REF_Item;
				}
				//�����ļ���-----------
				else
				{
					ref_list=1;
					FatDir.SubDir[USB]=0x55;
					FatDir.CurrentDir[26]=file_list[m+26];
					FatDir.CurrentDir[27]=file_list[m+27];
					FatDir.CurrentDir[20]=file_list[m+20];
					FatDir.CurrentDir[21]=file_list[m+21];
					k=0;					//����Ƶ���1��
					k_start=0;
					Sys.LcdRef=REF_Item;
				}
			}
			else
			{
				if(file_tol>0)
				{
					sel_tol=0;
					for(i=0;i<file_tol;i++)
					{
						m=sel_tol*32;
						if(file_sel[i]==1)
						{
							for(j=0;j<32;j++)
							{
								sel_list[m+j]=file_list[i*32+j];
							}
							//���ݵ�ǰ��Ŀ¼�Ĵغ�--------
							if(FatDir.SubDir[USB]==0x55)
							{
								sel_list[m+26]=file_list[26];
								sel_list[m+27]=file_list[27];
								sel_list[m+20]=file_list[20];
								sel_list[m+21]=file_list[21];
							}
							else
							{
								sel_list[m+26]=0;
								sel_list[m+27]=0;
								sel_list[m+20]=0;
								sel_list[m+21]=0;
							}
							sel_tol++;
						}
					}
				}
				break;
			}
		}
		if(Key.Value==KEY_ESC)
		{
			sel_tol=0;
			break;
		}
	}
	Key.Value=KEY_No;
	Touch_KeyPop();
	LcdMemPop();
	return(sel_tol);
}


