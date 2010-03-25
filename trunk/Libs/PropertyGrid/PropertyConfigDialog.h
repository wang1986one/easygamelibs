/****************************************************************************/
/*                                                                          */
/*      �ļ���:    PropertyConfigDialog.h                                   */
/*      ��������:  2009��07��06��                                           */
/*      ����:      Sagasarate                                               */
/*                                                                          */
/*      �������Ȩ��Sagasarate(sagasarate@sina.com)����                     */
/*      ����Խ�����������κ���ҵ�ͷ���ҵ�����������                      */
/*      ���뱣���˰�Ȩ����                                                  */
/*                                                                          */
/****************************************************************************/
#pragma once


// CPropertyConfigDialog �Ի���
namespace PropertyGrid
{
class CPropertyConfigDialog : public CDialog
{
	DECLARE_DYNAMIC(CPropertyConfigDialog)

public:
	CPropertyConfigDialog(UINT nIDTemplate, CWnd* pParentWnd = NULL);   // ��׼���캯��
	virtual ~CPropertyConfigDialog();

// �Ի�������
	bool IsModual()
	{
		return m_IsModual;
	}

protected:
	bool	m_IsModual;
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
};
}