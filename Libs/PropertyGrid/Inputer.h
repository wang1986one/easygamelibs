#pragma once
#include "LinkEdit.h"
#include "LinkComboBox.h"
#include "PropertyConfigDialog.h"


#define ID_FUCTION_BUTTON	3762
// CInputer
namespace PropertyGrid
{
class CInputer : public CWnd
{
	DECLARE_DYNAMIC(CInputer)

public:
	CInputer();
	virtual ~CInputer();

protected:
	DECLARE_MESSAGE_MAP()

	afx_msg void OnBnClicked();

protected:
	CLinkEdit				m_Edit;
	CLinkComboBox			m_ComboBox;
	CSpinButtonCtrl			m_Spin;
	CButton					m_Button;

	int						m_Type;
	CPropertyConfigDialog *	m_FuctionDialog;


public:
	virtual BOOL Create(CWnd* pParentWnd,CFont * pFont);

	void StartInput(int Type,CString& Define,CString& DefaultValue,CPropertyConfigDialog * pFuctionDialog);

	void LostInputFocus(bool IsCheckMouse=true);

	void EndInput(bool IsChanged);
	

	CString GetInputText();
};

}
