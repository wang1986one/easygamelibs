// PropertyConfigDialog.cpp : 实现文件
//

#include "stdafx.h"
#include "PropertyConfigDialog.h"

namespace PropertyGrid
{
// CPropertyConfigDialog 对话框

IMPLEMENT_DYNAMIC(CPropertyConfigDialog, CDialog)
CPropertyConfigDialog::CPropertyConfigDialog(UINT nIDTemplate, CWnd* pParentWnd)
	: CDialog(nIDTemplate, pParentWnd)
{
	m_IsModual=true;
}

CPropertyConfigDialog::~CPropertyConfigDialog()
{
}

void CPropertyConfigDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CPropertyConfigDialog, CDialog)
END_MESSAGE_MAP()


// CPropertyConfigDialog 消息处理程序
}