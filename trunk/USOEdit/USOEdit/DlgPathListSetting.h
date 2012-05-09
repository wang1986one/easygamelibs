/****************************************************************************/
/*                                                                          */
/*      �ļ���:    DlgPathListSetting.h                                     */
/*      ��������:  2011��10��07��                                           */
/*      ����:      Sagasarate                                               */
/*                                                                          */
/*      �������Ȩ��Sagasarate(sagasarate@sina.com)����                     */
/*      ����Խ�����������κ���ҵ�ͷ���ҵ�����������                      */
/*      ���뱣���˰�Ȩ����                                                  */
/*                                                                          */
/****************************************************************************/
/****************************************************************************/
/*                                                                          */
/*      �ļ���:    DlgPathListSetting.h                                     */
/*      ��������:  2009��07��06��                                           */
/*      ����:      Sagasarate                                               */
/*                                                                          */
/*      �������Ȩ��Sagasarate(sagasarate@sina.com)����                     */
/*      ����Խ�����������κ���ҵ�ͷ���ҵ�����������                      */
/*      ���뱣���˰�Ȩ����                                                  */
/*                                                                          */
/****************************************************************************/
#pragma once
#include "afxwin.h"


// CDlgPathListSetting �Ի���

class CDlgPathListSetting : public CDialog
{
	DECLARE_DYNAMIC(CDlgPathListSetting)

public:
	CDlgPathListSetting(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CDlgPathListSetting();

// �Ի�������
	enum { IDD = IDD_SET_PATH_LIST };
	enum WORK_MODE
	{
		WORK_MODE_PATH_LIST,
		WORK_MODE_FILE_LIST,
	};

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
	CString m_PathListText;
	
public:
	CEasyArray<CEasyString> m_PathList;
	int m_WorkMode;
	CString m_Title;	
	afx_msg void OnBnClickedAdd();
	afx_msg void OnBnClickedCancel();
	virtual BOOL OnInitDialog();
	afx_msg void OnEditCut();
	afx_msg void OnEditCopy();
	afx_msg void OnEditPaste();
	CEdit m_edPathListText;
	afx_msg void OnEditUndo();
protected:
	virtual void OnOK();
};
