/****************************************************************************/
/*                                                                          */
/*      文件名:    SystemConfig.h                                           */
/*      创建日期:  2010年02月09日                                           */
/*      作者:      Sagasarate                                               */
/*                                                                          */
/*      本软件版权归Sagasarate(sagasarate@sina.com)所有                     */
/*      你可以将本软件用于任何商业和非商业软件开发，但                      */
/*      必须保留此版权声明                                                  */
/*                                                                          */
/****************************************************************************/
#pragma once

class CSystemConfig :
	public CStaticObject<CSystemConfig>
{
protected:
	UINT			m_MainThreadProcessLimit;
	CIPAddress		m_UDPControlAddress;
	bool			m_LogServerObjectUse;
	UINT			m_LogLevel;
	UINT			m_GuardThreadKeepAliveTime;
	UINT			m_GuardThreadKeepAliveCount;
public:
	CSystemConfig(void);
	~CSystemConfig(void);

	bool LoadConfig(LPCTSTR ConfigFileName);

	UINT GetMainThreadProcessLimit()
	{
		return m_MainThreadProcessLimit;
	}
	CIPAddress& GetUDPControlAddress()
	{
		return m_UDPControlAddress;
	}

	bool IsLogServerObjectUse()
	{
		return m_LogServerObjectUse;
	}
	UINT GetLogLevel()
	{
		return m_LogLevel;
	}
	UINT GetGuardThreadKeepAliveTime()
	{
		return m_GuardThreadKeepAliveTime;
	}
	UINT GetGuardThreadKeepAliveCount()
	{
		return m_GuardThreadKeepAliveCount;
	}

};
