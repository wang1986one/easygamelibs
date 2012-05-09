/****************************************************************************/
/*                                                                          */
/*      �ļ���:    DlgTextEditor.h                                          */
/*      ��������:  2011��10��07��                                           */
/*      ����:      Sagasarate                                               */
/*                                                                          */
/*      �������Ȩ��Sagasarate(sagasarate@sina.com)����                     */
/*      ����Խ�����������κ���ҵ�ͷ���ҵ�����������                      */
/*      ���뱣���˰�Ȩ����                                                  */
/*                                                                          */
/****************************************************************************/
#pragma once


// CDlgTextEditor �Ի���

class CDlgTextEditor : public CDialog
{
	DECLARE_DYNAMIC(CDlgTextEditor)

public:
	CDlgTextEditor(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CDlgTextEditor();

// �Ի�������
	enum { IDD = IDD_DIALOG_TEXT_EDITOR };

protected:
	CString m_Text;

	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	void SetText(LPCTSTR szText)
	{
		m_Text=szText;
	}
	LPCTSTR GetText()
	{
		return m_Text;
	}
	virtual BOOL OnInitDialog();
protected:
	virtual void OnOK();
	virtual void OnCancel();
};
