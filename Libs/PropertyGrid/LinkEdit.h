#pragma once




namespace PropertyGrid
{
class CLinkEdit : public CEdit
{
	DECLARE_DYNAMIC(CLinkEdit)
protected:
	bool					m_EnableInput;
	
	
public:
	CLinkEdit();
	virtual ~CLinkEdit();	
	

	void EnableInput(bool IsEnable)
	{
		m_EnableInput=IsEnable;
	}

protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnKillFocus(CWnd* pNewWnd);		
	afx_msg void OnChar(UINT nChar, UINT nRepCnt, UINT nFlags);	
	afx_msg void OnSetFocus(CWnd* pOldWnd);
	
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);

};

}