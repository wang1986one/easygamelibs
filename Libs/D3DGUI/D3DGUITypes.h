/****************************************************************************/
/*                                                                          */
/*      �ļ���:    D3DGUITypes.h                                            */
/*      ��������:  2009��07��06��                                           */
/*      ����:      Sagasarate                                               */
/*                                                                          */
/*      ��������Ȩ��Sagasarate(sagasarate@sina.com)����                     */
/*      ����Խ������������κ���ҵ�ͷ���ҵ������������                      */
/*      ���뱣���˰�Ȩ����                                                  */
/*                                                                          */
/****************************************************************************/
#pragma once

namespace D3DGUI{


struct FLOAT_POINT
{
	FLOAT x;
	FLOAT y;
	FLOAT_POINT()
	{
		x=0;
		y=0;
	}
	FLOAT_POINT(FLOAT px,FLOAT py)
	{
		x=px;
		y=py;
	}
};

//2DCellʹ�õĶ����ʽ
//struct CELLVERTEX { D3DXVECTOR4 p;   DWORD color;     FLOAT tu, tv; };
//
////2DCellʹ�õĶ����ʽ�����
//#define D3DFVF_CELLVERTEX (D3DFVF_XYZRHW|D3DFVF_DIFFUSE|D3DFVF_TEX1)
//
////����2DCell����ĺ���
//inline CELLVERTEX InitCellVertex( const D3DXVECTOR4& p, DWORD color,
//                                      FLOAT tu, FLOAT tv )
//{
//    CELLVERTEX v;   v.p = p;   v.color = color;   v.tu = tu;   v.tv = tv;
//    return v;
//}

enum WM_D3DGUI_MESSAGES
{
	WM_D3DGUI_START=WM_USER+0x1000,
	WM_D3DGUI_WND_LOADED,
	WM_D3DGUI_CHILD_LOADED,
	WM_D3DGUI_VISIBLE_CHANGE,
	WM_D3DGUI_SYSTEM_MSG_END=WM_USER+0x1100,
	WM_D3DGUI_RESIZING,
	WM_D3DGUI_RESIZED,
	WM_D3DGUI_GET_FOCUS,
	WM_D3DGUI_LOST_FOCUS,
	WM_D3DGUI_MOUSE_ENTER,
	WM_D3DGUI_MOUSE_LEAVE,
	WM_D3DGUI_BUTTON_DOWN,
	WM_D3DGUI_BUTTON_UP,
	WM_D3DGUI_BUTTON_CLICK,
	WM_D3DGUI_EDIT_TEXT_CHANGED,
	WM_D3DGUI_SCROLL_BAR_SCROLL,
	WM_D3DGUI_SIMPLE_LIST_LCLICK,
	WM_D3DGUI_SIMPLE_LIST_LDBCLICK,
	WM_D3DGUI_SIMPLE_LIST_SELCHANGE,
	WM_D3DGUI_COMBO_SELCHANGE,
	
};

//#define D3DGUI_OBJECT_SEC	0x01000000
//#define OBJECT_TYPE_SEC(TypeID)	(TypeID>>24)
//
//enum D3DGUI_OBJECT_TYPES
//{
//	GUI_OBJ_NONE=0,
//	GUI_OBJ_WND=D3DGUI_OBJECT_SEC|0x01,
//	GUI_OBJ_BUTTON=D3DGUI_OBJECT_SEC|0x02,
//	GUI_OBJ_EDIT=D3DGUI_OBJECT_SEC|0x03,
//	GUI_OBJ_SIMPLELIST=D3DGUI_OBJECT_SEC|0x04,
//	GUI_OBJ_COMBO=D3DGUI_OBJECT_SEC|0x05,
//	GUI_OBJ_SCROLLBAR=D3DGUI_OBJECT_SEC|0x06,
//};


}