/****************************************************************************/
/*                                                                          */
/*      �ļ���:    EasyThreadWin.h                                          */
/*      ��������:  2009��07��06��                                           */
/*      ����:      Sagasarate                                               */
/*                                                                          */
/*      �������Ȩ��Sagasarate(sagasarate@sina.com)����                     */
/*      ����Խ�����������κ���ҵ�ͷ���ҵ�����������                      */
/*      ���뱣���˰�Ȩ����                                                  */
/*                                                                          */
/****************************************************************************/
#pragma once
#include "nameobject.h"

#define DEFAULT_THREAD_TERMINATE_TIME		60000
#define DEFAULT_THREAD_STARTUP_TIME			60000

enum THREAD_STATUS
{
	THREAD_STATUS_TERMINATED,
	THREAD_STATUS_STARTING,
	THREAD_STATUS_SUSPENDED,
	THREAD_STATUS_WORKING,
	THREAD_STATUS_ENDING,
};

class CEasyThread :
	public CNameObject
{
protected:
	UINT			m_ThreadID;	
	HANDLE			m_hThread;		
	volatile UINT	m_Status;
	volatile BOOL	m_WantTerminate;

	


	DECLARE_CLASS_INFO_STATIC(CEasyThread)
public:
	CEasyThread();
	virtual ~CEasyThread();

	///�����߳�
	BOOL Start(BOOL IsSuspended = FALSE,DWORD StartWaitTime=DEFAULT_THREAD_STARTUP_TIME);

	///�����߳�
	BOOL Resume();

	/// �����߳�
	BOOL Suspend();

	///��ֹ�߳�
	void Terminate();

	//ǿ�ƽ���
	void ForceTerminate();
	void SafeTerminate(DWORD Milliseconds=DEFAULT_THREAD_TERMINATE_TIME);

	//�߳��Ƿ񱻽�����
	BOOL IsTerminated();

	//�߳��Ƿ񱻹���
	BOOL IsSuspend();

	//�߳��Ƿ�������
	BOOL IsWorking();

	///����߳�״̬
	int GetStatus();

	///�ȴ��߳̿�ʼ����
	BOOL WaitForWorking(DWORD Milliseconds=INFINITE);

	///�ȴ��߳��˳�
	BOOL WaitForTerminate(DWORD Milliseconds=INFINITE);


	HANDLE GetThreadHandle();
	UINT GetThreadID();

protected:
	virtual BOOL OnStart();
	virtual void Execute();
	virtual BOOL OnRun();
	virtual void OnTerminate();


	///�߳�����
	static UINT __stdcall ThreadProc(LPVOID pParam);
};

inline BOOL CEasyThread::IsTerminated()
{
	return m_Status==THREAD_STATUS_TERMINATED;
}
inline BOOL CEasyThread::IsSuspend()
{
	return m_Status==THREAD_STATUS_SUSPENDED;
}
inline BOOL CEasyThread::IsWorking()
{
	return m_Status==THREAD_STATUS_WORKING;
}
inline int CEasyThread::GetStatus()
{
	return m_Status;
}

inline HANDLE CEasyThread::GetThreadHandle()
{
	return m_hThread;
}

inline UINT CEasyThread::GetThreadID()
{
	return m_ThreadID;
}
