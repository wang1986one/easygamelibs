/****************************************************************************/
/*                                                                          */
/*      �ļ���:    DlgFXSelect.h                                            */
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
/*      �ļ���:    DlgFXSelect.h                                            */
/*      ��������:  2009��07��06��                                           */
/*      ����:      Sagasarate                                               */
/*                                                                          */
/*      �������Ȩ��Sagasarate(sagasarate@sina.com)����                     */
/*      ����Խ�����������κ���ҵ�ͷ���ҵ�����������                      */
/*      ���뱣���˰�Ȩ����                                                  */
/*                                                                          */
/****************************************************************************/
#pragma once
#include "afxcmn.h"
#include <vector>

// CDlgFXSelect �Ի���
typedef std::vector<CD3DFX *> CFXPtrArray;

class CDlgFXSelect : public CPropertyConfigDialog
{
	DECLARE_DYNAMIC(CDlgFXSelect)

public:
	CDlgFXSelect(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CDlgFXSelect();

// �Ի�������
	enum { IDD = IDD_FX_SELECT };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
	CListCtrl	m_lvFXList;
	CFXPtrArray m_FXPtrArray;
	CD3DFX *	m_SelectedFX;
	virtual BOOL OnInitDialog();
};
