/****************************************************************************/
/*                                                                          */
/*      �ļ���:    EpollThread.h                                            */
/*      ��������:  2009��07��06��                                           */
/*      ����:      Sagasarate                                               */
/*                                                                          */
/*      �������Ȩ��Sagasarate(sagasarate@sina.com)����                     */
/*      ����Խ�����������κ���ҵ�ͷ���ҵ�����������                      */
/*      ���뱣���˰�Ȩ����                                                  */
/*                                                                          */
/****************************************************************************/
#pragma once


class CEpollThread :
	public CEasyThread
{
protected:
	UINT			m_hEpoll;	
	
	DECLARE_CLASS_INFO_STATIC(CEpollThread)
public:
	CEpollThread(void);
	virtual ~CEpollThread(void);

	void SetEpollHandle(UINT hEpoll);
protected:	
	virtual BOOL OnStart();
	virtual BOOL OnRun();
	virtual void OnTerminate();
};

inline void CEpollThread::SetEpollHandle(UINT hEpoll)
{
	m_hEpoll=hEpoll;
}