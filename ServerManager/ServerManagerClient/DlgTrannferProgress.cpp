/****************************************************************************/
/*                                                                          */
/*      文件名:    DlgTrannferProgress.cpp                                  */
/*      创建日期:  2010年02月09日                                           */
/*      作者:      Sagasarate                                               */
/*                                                                          */
/*      本软件版权归Sagasarate(sagasarate@sina.com)所有                     */
/*      你可以将本软件用于任何商业和非商业软件开发，但                      */
/*      必须保留此版权声明                                                  */
/*                                                                          */
/****************************************************************************/
// DlgTrannferProgress.cpp : 实现文件
//

#include "stdafx.h"




// CDlgTrannferProgress 对话框

IMPLEMENT_DYNAMIC(CDlgTrannferProgress, CDialog)

CDlgTrannferProgress::CDlgTrannferProgress(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgTrannferProgress::IDD, pParent)
	, m_TransferRate(_T(""))
{

}

CDlgTrannferProgress::~CDlgTrannferProgress()
{
}

void CDlgTrannferProgress::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_TRANSFER_PROGRESS, m_pgTransfer);
	DDX_Text(pDX, IDC_STATIC_TRANSFER_RATE, m_TransferRate);
}


BEGIN_MESSAGE_MAP(CDlgTrannferProgress, CDialog)
END_MESSAGE_MAP()


// CDlgTrannferProgress 消息处理程序
void CDlgTrannferProgress::InitProgress(int TotalSize)
{
	

	m_pgTransfer.SetRange32(0,TotalSize);
	m_pgTransfer.SetPos(0);
	m_TransferRate="0kb/S";
	UpdateData(false);

	m_TansferTimer.SaveTime();
}
void CDlgTrannferProgress::UpdateProgress(int FinishSize)
{
	m_pgTransfer.SetPos(FinishSize);
	float Rate=(float)FinishSize*1000.0f/1024.0f/m_TansferTimer.GetPastTime();
	m_TransferRate.Format("%fkb/S",Rate);
	UpdateData(false);
}	

