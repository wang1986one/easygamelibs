/****************************************************************************/
/*                                                                          */
/*      �ļ���:    SystemConfig.cpp                                         */
/*      ��������:  2010��02��09��                                           */
/*      ����:      Sagasarate                                               */
/*                                                                          */
/*      �������Ȩ��Sagasarate(sagasarate@sina.com)����                     */
/*      ����Խ�����������κ���ҵ�ͷ���ҵ�����������                      */
/*      ���뱣���˰�Ȩ����                                                  */
/*                                                                          */
/****************************************************************************/
#include "StdAfx.h"
#include "SystemConfig.h"

CSystemConfig::CSystemConfig(void)
{
	m_MainThreadProcessLimit=DEFAULT_PROCESS_LIMIT;
	m_UDPControlAddress.SetPort(0);
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

			xml_node MainThread=Config;
			if(MainThread.moveto_child("MainThread"))
			{
				if(MainThread.has_attribute("ProcessLimit"))
					m_MainThreadProcessLimit=(long)MainThread.attribute("ProcessLimit");

			}
			
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


			PrintImportantLog(0,"ϵͳ����������");
			PrintImportantLog(0,"���̴߳�������:%u",m_MainThreadProcessLimit);
			PrintImportantLog(0,"UDP���ƽӿ�:%s:%u",m_UDPControlAddress.GetIPString(),m_UDPControlAddress.GetPort());
			PrintImportantLog(0,"�Ƿ��¼OverLapped����ʹ��״̬:%s",m_LogServerObjectUse?"��":"��");
			PrintImportantLog(0,"Log�������:%u",m_LogLevel);
			PrintImportantLog(0,"���߳������ж�ʱ��:%u",m_GuardThreadKeepAliveTime);
			PrintImportantLog(0,"���߳������ж�����:%u",m_GuardThreadKeepAliveCount);

		}
		else
		{
			PrintImportantLog(0,"�����ļ�[%s]���Ϸ�",ConfigFileName);
			return false;
		}

	}
	else
	{
		PrintImportantLog(0,"�޷��������ļ�[%s]",ConfigFileName);
		return false;
	}
	return true;
}