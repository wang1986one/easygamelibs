/****************************************************************************/
/*                                                                          */
/*      �ļ���:    MainApp.h                                                */
/*      ��������:  2010��02��09��                                           */
/*      ����:      Sagasarate                                               */
/*                                                                          */
/*      �������Ȩ��Sagasarate(sagasarate@sina.com)����                     */
/*      ����Խ�����������κ���ҵ�ͷ���ҵ�����������                      */
/*      ���뱣���˰�Ȩ����                                                  */
/*                                                                          */
/****************************************************************************/
// DBProxy.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once


// CMainApp:
// �йش����ʵ�֣������ DBProxy.cpp
//

class CMainApp : public CServerApp
{
protected:
	
public:
	CMainApp();

// ��д
	public:
	//virtual BOOL InitInstance();
	//virtual int ExitInstance();

	virtual BOOL OnStartUp();
	virtual void OnShutDown();

// ʵ��

	
};

//extern CMainApp theApp;
