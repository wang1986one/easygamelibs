/****************************************************************************/
/*                                                                          */
/*      �ļ���:    DlgEditBox.h                                             */
/*      ��������:  2010��02��09��                                           */
/*      ����:      Sagasarate                                               */
/*                                                                          */
/*      ��������Ȩ��Sagasarate(sagasarate@sina.com)����                     */
/*      ����Խ������������κ���ҵ�ͷ���ҵ������������                      */
/*      ���뱣���˰�Ȩ����                                                  */
/*                                                                          */
/****************************************************************************/
#pragma once


// CDlgEditBox �Ի���

class CDlgEditBox : public CDialog
{
	DECLARE_DYNAMIC(CDlgEditBox)

public:
	CDlgEditBox(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CDlgEditBox();

// �Ի�������
	enum { IDD = IDD_DIALOG_EDIT_BOX };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	CString m_Text;
	virtual BOOL OnInitDialog();
};