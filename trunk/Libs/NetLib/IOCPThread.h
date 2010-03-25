/****************************************************************************/
/*                                                                          */
/*      �ļ���:    IOCPThread.h                                             */
/*      ��������:  2009��07��06��                                           */
/*      ����:      Sagasarate                                               */
/*                                                                          */
/*      �������Ȩ��Sagasarate(sagasarate@sina.com)����                     */
/*      ����Խ�����������κ���ҵ�ͷ���ҵ�����������                      */
/*      ���뱣���˰�Ȩ����                                                  */
/*                                                                          */
/****************************************************************************/
#pragma once


class CIOCPThread :
	public CEasyThread
{
protected:
	HANDLE			m_hIOCP;	
	
	DECLARE_CLASS_INFO_STATIC(CIOCPThread)
public:
	CIOCPThread(void);
	virtual ~CIOCPThread(void);

	void SetIOCPHandle(HANDLE hIOCP);
protected:	
	virtual BOOL OnStart();
	virtual BOOL OnRun();
	virtual void OnTerminate();
};

inline void CIOCPThread::SetIOCPHandle(HANDLE hIOCP)
{
	m_hIOCP=hIOCP;
}