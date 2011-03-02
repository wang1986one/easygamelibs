/****************************************************************************/
/*                                                                          */
/*      �ļ���:    LinkComboBox.cpp                                         */
/*      ��������:  2009��07��06��                                           */
/*      ����:      Sagasarate                                               */
/*                                                                          */
/*      ��������Ȩ��Sagasarate(sagasarate@sina.com)����                     */
/*      ����Խ������������κ���ҵ�ͷ���ҵ������������                      */
/*      ���뱣���˰�Ȩ����                                                  */
/*                                                                          */
/****************************************************************************/
#include "stdafx.h"
#include "LinkComboBox.h"
#include "Inputer.h"

namespace PropertyGrid
{
IMPLEMENT_DYNAMIC(CLinkComboBox, CComboBox)
CLinkComboBox::CLinkComboBox()
{
}

CLinkComboBox::~CLinkComboBox()
{
}


BEGIN_MESSAGE_MAP(CLinkComboBox, CComboBox)
	ON_WM_KILLFOCUS()	
END_MESSAGE_MAP()


void CLinkComboBox::OnKillFocus(CWnd* pNewWnd)
{
	//CLinkComboBox::OnKillFocus(pNewWnd);
	if(IsWindowVisible())
		((CInputer *)GetParent())->LostInputFocus();
	
	//ShowWindow(SW_HIDE);
	

	// TODO: �ڴ˴�������Ϣ�����������
}


// CLinkComboBox ��Ϣ��������
}