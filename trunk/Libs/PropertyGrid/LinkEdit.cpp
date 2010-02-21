#include "stdafx.h"
#include "LinkEdit.h"
#include "Inputer.h"


namespace PropertyGrid
{
IMPLEMENT_DYNAMIC(CLinkEdit, CEdit)
CLinkEdit::CLinkEdit()
{
	m_EnableInput=true;	
}

CLinkEdit::~CLinkEdit()
{
}


BEGIN_MESSAGE_MAP(CLinkEdit, CEdit)
	ON_WM_KILLFOCUS()		
	ON_WM_CHAR()
	ON_WM_SETFOCUS()	
	ON_WM_KEYDOWN()	
END_MESSAGE_MAP()


// CLinkEdit ��Ϣ�������
void CLinkEdit::OnKillFocus(CWnd* pNewWnd)
{
	CEdit::OnKillFocus(pNewWnd);	
	// TODO: �ڴ˴������Ϣ����������	
	//HideCaret();
	if(IsWindowVisible())
		((CInputer *)GetParent())->LostInputFocus();
	//ShowWindow(SW_HIDE);
	//DisableSpin();
}

void CLinkEdit::OnChar(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ

	if(m_EnableInput)
		CEdit::OnChar(nChar, nRepCnt, nFlags);

	if(nChar==VK_RETURN)
	{
		((CInputer *)GetParent())->LostInputFocus(false);
	}
}

void CLinkEdit::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ

	if(m_EnableInput)
		CEdit::OnKeyDown(nChar, nRepCnt, nFlags);
}


void CLinkEdit::OnSetFocus(CWnd* pOldWnd)
{
	CEdit::OnSetFocus(pOldWnd);

	// TODO: �ڴ˴������Ϣ����������
}


}


