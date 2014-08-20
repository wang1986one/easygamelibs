/****************************************************************************/
/*                                                                          */
/*      �ļ���:    ServerAppWin.h                                           */
/*      ��������:  2009��07��06��                                           */
/*      ����:      Sagasarate                                               */
/*                                                                          */
/*      �������Ȩ��Sagasarate(sagasarate@sina.com)����                     */
/*      ����Խ�����������κ���ҵ�ͷ���ҵ�����������                      */
/*      ���뱣���˰�Ȩ����                                                  */
/*                                                                          */
/****************************************************************************/
#pragma once



class CServerApp :
	public CWinApp,public CNTService
{
protected:
	IBaseServer *		m_pServer;
public:
	CServerApp(void);
	~CServerApp(void);

	virtual BOOL InitInstance();

	virtual int ExitInstance();

	virtual BOOL OnIdle(LONG lCount);

	virtual void Run(DWORD argc, LPTSTR * argv);
	
	virtual void Stop();
public:

	virtual int Run();
	virtual BOOL OnStartUp();
	virtual void OnShutDown();

protected:
	void GetProgamVersion();
	//DECLARE_MESSAGE_MAP()
	
};
