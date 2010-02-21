#include "StdAfx.h"
#include "SystemConfig.h"

CSystemConfig::CSystemConfig(void)
{
	m_UDPControlAddress.SetPort(9600);
	m_LogServerObjectUse=TRUE;
#ifdef _DEBUG
	m_LogLevel=ILogPrinter::LOG_LEVEL_NORMAL|ILogPrinter::LOG_LEVEL_DEBUG;
#else
	m_LogLevel=ILogPrinter::LOG_LEVEL_NORMAL;
#endif
	m_GuardThreadKeepAliveTime=20*1000;
	m_GuardThreadKeepAliveCount=5;
}

CSystemConfig::~CSystemConfig(void)
{
}

bool CSystemConfig::LoadConfig(LPCTSTR ConfigFileName)
{
	xml_parser Parser;

	if(Parser.parse_file(ConfigFileName,pug::parse_trim_attribute))
	{
		xml_node Config=Parser.document();
		if(Config.moveto_child("System"))
		{
			xml_node UDPControlAddress=Config;
			if(UDPControlAddress.moveto_child("UDPControlAddress"))
			{
				if(UDPControlAddress.has_attribute("IP"))
					m_UDPControlAddress.SetIP(UDPControlAddress.attribute("IP").getvalue().c_str());

				if(UDPControlAddress.has_attribute("Port"))
					m_UDPControlAddress.SetPort((WORD)((long)UDPControlAddress.attribute("Port")));

			}

			xml_node LogConfig=Config;
			if(LogConfig.moveto_child("LogConfig"))
			{
				if(LogConfig.has_attribute("LogServerObjectUse"))
					m_LogServerObjectUse=(bool)LogConfig.attribute("LogServerObjectUse");

				if(LogConfig.has_attribute("LogLevel"))
					m_LogLevel=(long)LogConfig.attribute("LogLevel");
			}

			xml_node GuardThread=Config;
			if(GuardThread.moveto_child("GuardThread"))
			{
				if(GuardThread.has_attribute("KeepAliveTime"))
					m_GuardThreadKeepAliveTime=(long)GuardThread.attribute("KeepAliveTime");

				if(GuardThread.has_attribute("KeepAliveCount"))
					m_GuardThreadKeepAliveCount=(long)GuardThread.attribute("KeepAliveCount");
			}
		}
		else
		{
			PrintImportantLog(0,"配置文件[%s]不合法",ConfigFileName);
			return false;
		}

	}
	else
	{
		PrintImportantLog(0,"无法打开配置文件[%s]",ConfigFileName);
		return false;
	}
	return true;
}