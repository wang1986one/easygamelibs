/****************************************************************************/
/*                                                                          */
/*      �ļ���:    DlgFXEditor.h                                            */
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
/*      �ļ���:    DlgFXEditor.h                                            */
/*      ��������:  2009��07��06��                                           */
/*      ����:      Sagasarate                                               */
/*                                                                          */
/*      �������Ȩ��Sagasarate(sagasarate@sina.com)����                     */
/*      ����Խ�����������κ���ҵ�ͷ���ҵ�����������                      */
/*      ���뱣���˰�Ȩ����                                                  */
/*                                                                          */
/****************************************************************************/
#pragma once


// CDlgFXEditor �Ի���

class CDlgFXEditor : public CPropertyConfigDialog
{
	DECLARE_DYNAMIC(CDlgFXEditor)

public:
	CDlgFXEditor(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CDlgFXEditor();

// �Ի�������
	enum { IDD = IDD_DIALOG_FX_EDIT };

	CString m_FxContent;
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
protected:
	virtual void OnOK();
};
