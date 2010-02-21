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
	

	// TODO: 在此处添加消息处理程序代码
}


// CLinkComboBox 消息处理程序
}