#pragma once


// CSystemConsoleFrame ���

class CSystemConsoleFrame : public CMDIChildWnd
{
	DECLARE_DYNCREATE(CSystemConsoleFrame)
protected:
	CSystemConsoleFrame();           // ��̬������ʹ�õ��ܱ����Ĺ��캯��
	virtual ~CSystemConsoleFrame();

protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnClose();
};

