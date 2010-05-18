/****************************************************************************/
/*                                                                          */
/*      �ļ���:    StaticObject.h                                           */
/*      ��������:  2010��02��09��                                           */
/*      ����:      Sagasarate                                               */
/*                                                                          */
/*      �������Ȩ��Sagasarate(sagasarate@sina.com)����                     */
/*      ����Խ�����������κ���ҵ�ͷ���ҵ�����������                      */
/*      ���뱣���˰�Ȩ����                                                  */
/*                                                                          */
/****************************************************************************/
#pragma once




template <class T>
class CStaticObject
{
public:
	static T * GetInstance()
	{
		return &m_Instance;
	}
protected:
	static T m_Instance;
};
template <class T>
T  CStaticObject<T>::m_Instance;



template <class T,LPCTSTR EnvVarName>
class CStaticObject2
{
public:
	static T * GetInstance()
	{
		if( m_pInstance == NULL )
		{
			CAutoLock Lock(m_InstanceCriticalSection);
			if( m_pInstance == NULL )
			{
				char ThisEnvVarName[260];
				sprintf_s(ThisEnvVarName,260,"%s_%u",
					EnvVarName,GetCurProcessID());
				char InstanceAddrStr[32];
				InstanceAddrStr[0]=0;			
				GetEnvVar(ThisEnvVarName,InstanceAddrStr,32);
				m_pInstance=(T *)_atoi64(InstanceAddrStr);
				if(m_pInstance==NULL)
				{
					m_pInstance = new T;
					sprintf_s(InstanceAddrStr,32,"%llu",(LONGLONG)m_pInstance);
					InstanceAddrStr[31]=0;
					SetEnvVar(ThisEnvVarName,InstanceAddrStr);
					//atexit(ReleaseInstance);
				}		
			}
		}
		return m_pInstance;
	}
	static void ReleaseInstance()
	{
		CAutoLock Lock(m_InstanceCriticalSection);
		char ThisEnvVarName[260];
		sprintf_s(ThisEnvVarName,260,"%s_%u",
			EnvVarName,GetCurProcessID());
		char InstanceAddrStr[32];
		InstanceAddrStr[0]=0;
		GetEnvVar(ThisEnvVarName,InstanceAddrStr,32);
		m_pInstance=(T *)_atoi64(InstanceAddrStr);
		if(m_pInstance)
		{
			SAFE_DELETE(m_pInstance)			
			SetEnvVar(ThisEnvVarName,"0");			
		}
		m_pInstance=NULL;		
	}
protected:
	

	static CEasyCriticalSection	m_InstanceCriticalSection;
	static T * m_pInstance;	
};

template <class T,LPCTSTR EnvVarName>
CEasyCriticalSection CStaticObject2<T,EnvVarName>::m_InstanceCriticalSection;

template <class T,LPCTSTR EnvVarName>
T * CStaticObject2<T,EnvVarName>::m_pInstance=NULL;





template <class T>
class CStaticObject3
{
public:
	static T * GetInstance()
	{
		static T s_Instance;
		return &s_Instance;
	}
};



