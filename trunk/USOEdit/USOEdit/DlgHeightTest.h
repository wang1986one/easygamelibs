/****************************************************************************/
/*                                                                          */
/*      �ļ���:    DlgHeightTest.h                                          */
/*      ��������:  2011��10��07��                                           */
/*      ����:      Sagasarate                                               */
/*                                                                          */
/*      �������Ȩ��Sagasarate(sagasarate@sina.com)����                     */
/*      ����Խ�����������κ���ҵ�ͷ���ҵ�����������                      */
/*      ���뱣���˰�Ȩ����                                                  */
/*                                                                          */
/****************************************************************************/
#pragma once


// CDlgHeightTest �Ի���

class CDlgHeightTest : public CDialog
{
	DECLARE_DYNAMIC(CDlgHeightTest)
protected:
	CD3DViewPort *		m_pViewPort;
public:
	CDlgHeightTest(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CDlgHeightTest();

	void SetView(CD3DViewPort * pViewPort)
	{
		m_pViewPort=pViewPort;
	}

// �Ի�������
	enum { IDD = IDD_DIALOG_HEIGHT_TEST };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	float m_PosX;
	float m_PosZ;
	float m_PosY;
	float m_Height;
	afx_msg void OnBnClickedDoTest();
};
