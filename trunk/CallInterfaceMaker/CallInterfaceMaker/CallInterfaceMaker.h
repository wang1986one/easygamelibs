/****************************************************************************/
/*                                                                          */
/*      �ļ���:    CallInterfaceMaker.h                                     */
/*      ��������:  2011��10��07��                                           */
/*      ����:      Sagasarate                                               */
/*                                                                          */
/*      �������Ȩ��Sagasarate(sagasarate@sina.com)����                     */
/*      ����Խ�����������κ���ҵ�ͷ���ҵ�����������                      */
/*      ���뱣���˰�Ȩ����                                                  */
/*                                                                          */
/****************************************************************************/
// CallInterfaceMaker.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������


// CCallInterfaceMakerApp:
// �йش����ʵ�֣������ CallInterfaceMaker.cpp
//

class CCallInterfaceMakerApp : public CWinApp
{
protected:
	CCallInterfaceMakerDlg	*m_pMainDlg;	
public:
	CCallInterfaceMakerApp();

	
	CCallInterfaceMakerDlg * GetMainDlg()
	{
		return m_pMainDlg;
	}

// ��д
	public:
	virtual BOOL InitInstance();

// ʵ��

	DECLARE_MESSAGE_MAP()
};

extern CCallInterfaceMakerApp theApp;

inline CCallInterfaceMakerDlg * GetMainDlg()
{
	return theApp.GetMainDlg();
}