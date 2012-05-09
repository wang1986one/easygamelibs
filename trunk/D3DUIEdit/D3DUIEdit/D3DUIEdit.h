/****************************************************************************/
/*                                                                          */
/*      �ļ���:    D3DUIEdit.h                                              */
/*      ��������:  2011��10��07��                                           */
/*      ����:      Sagasarate                                               */
/*                                                                          */
/*      �������Ȩ��Sagasarate(sagasarate@sina.com)����                     */
/*      ����Խ�����������κ���ҵ�ͷ���ҵ�����������                      */
/*      ���뱣���˰�Ȩ����                                                  */
/*                                                                          */
/****************************************************************************/
// D3DUIEdit.h : D3DUIEdit Ӧ�ó������ͷ�ļ�
//
#pragma once

#ifndef __AFXWIN_H__
	#error �ڰ������� PCH �Ĵ��ļ�֮ǰ������stdafx.h�� 
#endif

#include "resource.h"       // ������


// CD3DUIEditApp:
// �йش����ʵ�֣������ D3DUIEdit.cpp
//
class CD3DViewPort;

class CD3DUIEditApp : public CWinApp
{
protected:
	CD3DViewPort * m_pD3DViewPort;
public:
	CD3DUIEditApp();


// ��д
public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();

	void SetD3DViewPort(CD3DViewPort * pD3DViewPort)
	{
		m_pD3DViewPort=pD3DViewPort;
	}

// ʵ��
	afx_msg void OnAppAbout();
	DECLARE_MESSAGE_MAP()
	virtual BOOL OnIdle(LONG lCount);
	afx_msg void OnFileOpen();
protected:
	void CheckSystemFile();
};

extern CD3DUIEditApp theApp;
