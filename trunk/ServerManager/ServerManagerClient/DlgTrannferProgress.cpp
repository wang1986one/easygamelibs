/****************************************************************************/
/*                                                                          */
/*      �ļ���:    DlgTrannferProgress.cpp                                  */
/*      ��������:  2010��02��09��                                           */
/*      ����:      Sagasarate                                               */
/*                                                                          */
/*      �������Ȩ��Sagasarate(sagasarate@sina.com)����                     */
/*      ����Խ�����������κ���ҵ�ͷ���ҵ�����������                      */
/*      ���뱣���˰�Ȩ����                                                  */
/*                                                                          */
/****************************************************************************/
// DlgTrannferProgress.cpp : ʵ���ļ�
//

#include "stdafx.h"




// CDlgTrannferProgress �Ի���

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


// CDlgTrannferProgress ��Ϣ�������
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

