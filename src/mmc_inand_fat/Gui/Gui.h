#ifndef	_Gui_H_
#define 	_Gui_H_

#define ASC_Head 			32		//����ַ���ӦASC����׵�ַ
#define ASC_Num			16		//������ASC�����ƫ��λ��

#define LCD_SIGNAL_Fan		0				// 1��ʾ��ƽ�źŷ�(0x0000=��,0xffff=��)(������Ϊ1)

#define LCD_FilterColor		0xfc1f			//��ʾͼƬʱָ���Ĺ���ɫ(��ͼ��������Ͻǵ���ɫ)
#define LCD_FilterNo			0xffffffff		//��ʾͼƬʱ�����й���

#define C_Back				C_Gray

//�����������ɫ���Ͷ���(�н���Ч��)--------
#define MAX_FormTitle		3				//�����������ʽ���Ϊ3
#define FORM_Red			0				//���������Ϊ��ɫ
#define FORM_Blue		1				//���������Ϊ��ɫ
#define FORM_Gray		2				//���������Ϊ��ɫ

//�������ı���ɫ����(�н���Ч��)------------------
#define BTN_Gray			0				
#define BTN_Yellow		1

#define FONT16			16				//����Ϊ16*16����	
#define FONT24			20				//����Ϊ20*20����	

#define FRAME_Hight		24				//�ı���ĸ߶�

#define TITLE_H16		24				//������Ϊ16��������ĸ߶�
#define TITLE_H24		30				//������Ϊ24��������ĸ߶�
#define TITLE_H35		32				//�������߶�Ϊ35������


#define PTEXT	(const char *)
#define GET_ScrCenterX(width)	((LCD_XSize-width)>>1)	//��ʾ����Ļ���ĵ�Xֵ
#define GET_ScrCenterY(height)	((LCD_YSize-height)>>1)	//��ʾ����Ļ���ĵ�Yֵ

//����ͼ�겿���ṹ----------------
typedef __packed struct _S_GuiIcon{
	const char *Text;		//ͼ������ʾ���ı�ָ��
	uint16 *Bmp;			//ICONͼ����׵�ַָ��
	uint16 X;				//ͼ�겿������ʼX����(����ʾ����)
	uint16 Y;				//ͼ�겿������ʼY����(����ʾ����)
	uint16 Width;			//ͼ�겿���Ŀ��(����ʾ��Ŀ��)
	uint16 Height;			//ͼ�겿���ĸ߶�(����ʾ��ĸ߶�)
	uint16 BmpW;			//ͼƬ�Ŀ��
	uint16 BmpH;			//ͼƬ�ĸ߶�
	uint16 Sel;			//��Ϊ0��ʾ������ڱ�ͼ����
	uint16 TextColor;		//��ʾ�ı���ɫ
	uint16 RectColor;		//ѡ�п����ɫ
	uint16 BackColor;		//����ɫ
}S_GuiIcon;

//ͼ����ʾ���궨λ�ṹ--------
typedef struct _S_GuiXY{
	uint16 StartX;			//��ʼ��X����
	uint16 StartY;			//��ʼ��Y����
	uint16 RowSpace;			//�����������м��
	uint16 ColSpace;			//�����������м��
	uint16 RowIcons;			//ÿ����ʾ��ͼ����
	uint16 MaxIcons;			//�ܵ�ͼ����

	uint16 BtnStartX;			//������ť����ʼX����
	uint16 BtnPartWidth;		//������ť�������
	uint16 BtnWidth;			//������ťʵ�ʿ��
}S_GuiXY;

//��ʾ���ݱ��ݼ��ָ��ṹ--------------
typedef __packed struct _S_LcdMem{
	uint16 *Mem[10];			//ÿ���Դ�ĵ�ַָ��(���10��)
	uint32 Point;				//��ǰָ��Ĳ����Դ��(��1��ʼ)
	uint32 Size[10];			//ÿ���Դ�ռ�õ�����
	uint32 X[10];				//��ʼX����
	uint32 Y[10];				//��ʼY����
	uint32 Width[10];			//���ؿ��
	uint32 Height[10];		//���ظ߶�
}S_LcdMem;

//������Ϣ�ṹ����------------------------------
typedef __packed struct _S_Form{
	uint16 X;					//��ʼ��X����
	uint16 Y;					//��ʼ��Y����
	uint16 Width;				//������
	uint16 Height;			//����߶�
	const char *TitleText;		//�������ı�	
	uint16 TitleFont;			//�������ı�����������
	uint16 TitleColor;			//��������ɫ����(�̶�Ϊ����)
	uint16 TitleHeight;		//�������߶�	
	uint16 TextColor;			//������������ɫ
	uint16 BackColor;			//����ɫ(�������ı���ɫ)
	uint16 FillBack;			//��Ϊ0��ʾ��䱳��ɫ
}S_Form;

extern const uint8 Asc8x16[];
extern const uint8 Asc10x20[];
extern const uint16 FormTitleColor[3][24];
extern S_LcdMem LcdMem;
extern uint32 Lang;
extern uint8 Hz_Lib[];							
extern uint8 Hz_Lib20[];						

void ClearScreen(uint8 dat,uint8 layer);
void Lcd_Init(void);

uint32 KeyIconMove(S_GuiXY *guixy,uint32 k);
void LcdMemInit(void);
void LcdMemPush(uint32 x,uint32 y,uint32 width,uint32 height);
void LcdMemPop(void);

void Dis_Rect(uint16 x,uint16 y,uint16 width,uint16 heigh,uint16 color);
void Dis_RectFill(uint16 x,uint16 y,uint16 width,uint16 heigh,uint16 color);
void Dis_RectSome(uint32 x,uint32 y,uint32 width,uint32 heigh,uint32 line,uint32 color);
void Dis_CircleRect(int16 x,int16 y,uint32 width,uint32 height,uint32 r,uint16 color);
void Dis_CircleRectFill(int16 x,int16 y,uint32 width,uint32 height,uint32 r,uint16 color);
void Dis_CircleRectFillShade(int16 x,int16 y,uint32 width,uint32 height,uint32 r,uint16 color1,uint16 color2);
void Dis_Circle(uint32 x,uint32 y,uint32 r,uint16 color);
void Dis_CircleFill(uint32 x,uint32 y,uint32 r,uint16 color);
void Dis_Ellispse(int x,int y,int a,int b,uint16 color);
void Dis_EllispseFill(int x,int y,int a,int b,uint16 color);

void Dis_IconSelect(S_GuiIcon *icon);

extern void Dis_Picture(uint32 x,uint32 y,uint32 width,uint32 height,uint16 *bmp);
extern void Dis_PictureFilter(uint32 x,uint32 y,uint32 width,uint32 height,uint16 *bmp,uint32 filter);

extern void Dis_8x16(uint8 num,uint16 x,uint16 y,uint16 color_f,uint16 color_b);
extern void Dis_Asc12(uint8 num,uint16 x,uint16 y,uint16 color_f,uint16 color_b);
extern void Dis_Asc(uint8 num,uint16 x,uint16 y,uint16 color_f,uint16 color_b);
extern void Dis_Hex(uint8 num,uint16 x,uint16 y,uint16 color_f,uint16 color_b);

extern void Dis_Strings(const char *hz,uint16 x,uint16 y,uint16 font,uint16 color_f,uint16 color_b);
extern void Dis_StringsAligen(const char *hz,uint32 aligen,uint16 x,uint16 y,uint32 width,uint16 font,uint16 color_f,uint16 color_b);
extern void Dis_NumFilterTitle(uint16 dat,uint16 x,uint16 y,uint8 weisu,uint16 font,uint16 color_f,S_Form *form);
extern void Dis_Chinese(uint16 hz,uint16 x,uint16 y,uint16 color_f,uint16 color_b);
extern void Dis_StringsFilterTitle(const char *hz,uint16 x,uint16 y,uint32 font,uint16 color_f,S_Form *form);
extern void Dis_StringsLogo(const char *hz,uint16 x,uint16 y,uint16 color_f);
extern void Dis_StringsFilter(const char *hz,uint16 x,uint16 y,uint16 font,uint16 color_f);
extern void Dis_StringsAligenFilter(const char *hz,uint32 aligen,uint16 x,uint16 y,uint32 width,uint16 font,uint16 color_f);

extern void Dis_NumHide(uint16 dat,uint16 x,uint16 y,uint8 weisu,uint16 font,uint16 color_f,uint16 color_b);
extern void Dis_NumAll(uint16 dat,uint16 x,uint16 y,uint8 weisu,uint16 font,uint16 color_f,uint16 color_b);
extern void Dis_NumAllLogo(uint16 dat,uint16 x,uint16 y,uint8 weisu,uint16 color_f);
void Dis_NumFloat(uint32 dat,uint16 x,uint16 y,uint8 weisu,uint8 xiaosu,uint16 font,uint16 color_f,uint16 color_b);
void Dis_NumPoint5(uint32 dat,uint32 five,uint16 x,uint16 y,uint8 weisu,uint16 font,uint16 color_f,uint16 color_b);
extern void Dis_NumFuHao(uint16 dat,uint8 zf,uint16 x,uint16 y,uint16 font,uint16 color_f,uint16 color_b);
void Dis_NumInt(int16 dat,uint16 x,uint16 y,uint8 weisu,uint16 font,uint16 color_f,uint16 color_b);
extern void Dis_WindowsForm(S_Form *form);
extern void Dis_WindowsFormLeft(S_Form *form);
extern void Dis_TextFrame(uint16 x,uint16 y,uint16 width,uint16 height,uint16 color);
extern void Dis_BoxUp(uint16 x,uint16 y,uint16 width,uint16 height,uint16 color);
extern void Dis_BoxDown(uint16 x,uint16 y,uint16 width,uint16 height,uint16 color);
extern void Dis_FrameUp(uint16 x,uint16 y,uint16 width,uint16 height,uint16 color);
extern void Dis_FrameDown(uint16 x,uint16 y,uint16 width,uint16 height,uint16 color);
extern void Dis_HintBox(const char *title,const char *text,uint32 push,uint16 color_f,uint16 color_b);
extern void Dis_PictureFilterBottom(uint32 x,uint32 y,uint32 width,uint32 height,uint16 *bmp,uint32 filter);
extern void Dis_HintForm(const char *title,const char *text,uint16 color_f,uint16 color_b);
extern uint32 Dis_InputDialog(uint32 max,uint32 dat,const char *text,uint16 color_b);
uint32 Dis_InputStrDialog(char *str,uint32 len,const char *text,uint16 color_b);
extern uint32 Dis_SureDialog(const char *title,const char *text,uint16 back_color);
extern uint32 Dis_SelectDialog(const char *text[][LANG_MAX],uint32 item,uint16 inx,uint16 iny,uint16 inw,uint16 inh,uint32 k,uint32 b_color);
extern void Dis_PercentSlider(uint32 dat,uint32 tol,uint32 x,uint32 y,uint32 height,uint32 width,uint32 color_f,uint32 color_b);

extern void Dis_SpeedInfo(const char *title,uint32 dat,uint32 tol,uint32 width,uint32 color);
//extern void Dis_TouchBottom(const char *hz,uint32 x,uint32 y,uint32 width,uint32 height,uint32 color,uint32 btncolor,uint16 key,uint16 num,uint16 font);

uint32 Dis_PassWord(uint32 mima);
uint32 Dis_SuperPassWord(uint8 *code);

void Get_IconFormPlace(S_Form *form,S_GuiXY *guixy,S_GuiIcon *icon,uint32 ext_h);
void Dis_IconRef(S_GuiXY *guixy,S_GuiIcon *icon,const char *text[][LANG_MAX],uint16 *bmp,uint32 k,uint32 tch_k);
void Dis_FileName(uint8 *fdt,uint32 ext,uint16 x,uint16 y,uint16 color_f,uint16 color_b);
void Dis_FileTime(uint8 *fdt,uint16 x,uint16 y,uint16 color_f,uint16 color_b);
void File_SortByFloder(uint32 tol,uint8 *file_list);
uint32 Dis_FileSelectOne(const char *title,uint8 *file_fdt,uint32 device);
uint32 Dis_FileSelectMany(const char *title,uint8 *file_fdt,uint8 *sel_list);

#endif

