/****************************************************************************/
/*                                                                          */
/*      �ļ���:    PropertyConfigDialog.cpp                                 */
/*      ��������:  2009��07��06��                                           */
/*      ����:      Sagasarate                                               */
/*                                                                          */
/*      �������Ȩ��Sagasarate(sagasarate@sina.com)����                     */
/*      ����Խ�����������κ���ҵ�ͷ���ҵ�����������                      */
/*      ���뱣���˰�Ȩ����                                                  */
/*                                                                          */
/****************************************************************************/
// PropertyConfigDialog.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "PropertyConfigDialog.h"

namespace PropertyGrid
{
// CPropertyConfigDialog �Ի���

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


// CPropertyConfigDialog ��Ϣ�������
}