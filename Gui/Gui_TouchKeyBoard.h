#ifndef __Gui_TouchKeyBoard_H
#define __Gui_TouchKeyBoard_H

#define C_ButtonUp1			0xF7BE			//������ť����״̬���ϰ벿����ɫ
#define C_ButtonUp2			0xE75D			//������ť����״̬���°벿����ɫ
#define C_ButtonDown			0x63D0			//������ť��ס״̬�µ���ɫ
#define C_ButtonYinYing		0				//������ť��Ӱ����ɫ
#define C_ButtonBack			0x1EA			//������ť�����ı���ɫ
#define C_ButtonSide			0x21C4			//�������̱߿����ɫ

#define MAX_TKeyBoard		6				//ģ�ⴥ�����̵ĸ���

#define TKEY_BoardStand		1				//��׼���봥������	
#define TKEY_BoardStandMove	2				//��׼���봥������(�����Ҽ�)
#define TKEY_BoardPara		3				//�������봥������(�����Ƽ�)
#define TKEY_BoardParaSet	4				//�������봥������(����ǰֵ�趨��)
#define TKEY_BoardAsc		5				//���봥������(���ַ�A~F)
#define TKEY_BoardTest		6				//���Դ�������(���Զ�����)

void Dis_TouchButton(const char *hz,uint32 x,uint32 y,uint32 width,uint32 height,uint16 key,uint16 num,uint32 color,uint16 font);
void Dis_TouchKeyBoard(uint32 hint);
void TouchKeyBoardClose(void);
void Dis_TouchButtonFlash(void);
void Dis_IconUp(uint32 x,uint32 y,uint32 width,uint32 height,uint16 *bmp);
void Dis_IconDown(uint32 x,uint32 y,uint32 width,uint32 height,uint16 *bmp);
void Dis_TouchIconRef(void);

#endif

