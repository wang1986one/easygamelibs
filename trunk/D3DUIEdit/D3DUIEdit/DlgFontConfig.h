/****************************************************************************/
/*                                                                          */
/*      �ļ���:    DlgFontConfig.h                                          */
/*      ��������:  2011��10��07��                                           */
/*      ����:      Sagasarate                                               */
/*                                                                          */
/*      �������Ȩ��Sagasarate(sagasarate@sina.com)����                     */
/*      ����Խ�����������κ���ҵ�ͷ���ҵ�����������                      */
/*      ���뱣���˰�Ȩ����                                                  */
/*                                                                          */
/****************************************************************************/
#pragma once
#include "Resource.h"
#include "afxwin.h"

// CDlgFontConfig �Ի���

class CDlgFontConfig : public CPropertyConfigDialog
{
	DECLARE_DYNAMIC(CDlgFontConfig)

public:
	CDlgFontConfig(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CDlgFontConfig();

// �Ի�������
	enum { IDD = IDD_DIALOG_FONT };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
protected:
	CD3DWnd *		m_pGUIObject;
	LOGFONT			m_Font;
	CString			m_FontString;
	int				m_CharSpace;
	int				m_LineSpace;
	DWORD			m_FontColor;
	int				m_ShadowWidth;
	DWORD			m_ShadowColor;
	BOOL			m_IsAlignLeft;
	BOOL			m_IsAlignCenter;
	BOOL			m_IsAlignRight;
	BOOL			m_IsAlignTop;
	BOOL			m_IsAlignVCenter;
	BOOL			m_IsAlignBottom;
	BOOL			m_IsWorkBreak;
	BOOL			m_ShadowLeft;
	BOOL			m_ShadowTop;
	BOOL			m_ShadowRight;
	BOOL			m_ShadowBottom;
	BOOL			m_ShadowTopLeft;
	BOOL			m_ShadowTopRight;
	BOOL			m_ShadowBottomLeft;
	BOOL			m_ShadowBottomRight;
	float			m_FontScale;

public:
	virtual BOOL OnInitDialog();

	CString LogFontToString(LOGFONT& Font);

	void SetGUIObject(CD3DWnd *	pGUIObject)
	{
		m_pGUIObject=pGUIObject;
	}
	afx_msg void OnBnClickedButtonSelectFont();
	afx_msg void OnBnClickedButtonSelectFontColor();
	afx_msg void OnBnClickedButtonSelectShadowColor();	
	afx_msg void OnBnClickedApply();
protected:
	virtual void OnOK();
public:	
	
	
	
	
};
