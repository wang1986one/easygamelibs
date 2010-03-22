#include "StdAfx.h"

CMainConfig::CMainConfig(void)
{
	FUNCTION_BEGIN;
	m_MaxClient=10000;
	m_ProcessInfoFetchTime=5000;
	m_MaxProcessCount=128;
	m_FileCacheSize=32;
	m_FileCacheQueryQueueSize=128;
	FUNCTION_END;
}

CMainConfig::~CMainConfig(void)
{
}

bool CMainConfig::LoadConfig(LPCTSTR FileName)
{
	FUNCTION_BEGIN;

	xml_parser Parser;

	if(Parser.parse_file(FileName,pug::parse_trim_attribute))
	{
		xml_node Config=Parser.document();

		if(Config.moveto_child("Main"))
		{

			xml_node ClientListen=Config;
			if(ClientListen.moveto_child("ClientListen"))
			{
				if(ClientListen.has_attribute("IP"))
					m_ListenAddress.SetIP(ClientListen.attribute("IP").getvalue().c_str());

				if(ClientListen.has_attribute("Port"))
					m_ListenAddress.SetPort((long)ClientListen.attribute("Port"));

				if(ClientListen.has_attribute("MaxClient"))
					m_MaxClient=(long)ClientListen.attribute("MaxClient");

			}			
			xml_node ProcessMonitor=Config;
			if(ProcessMonitor.moveto_child("ProcessMonitor"))
			{
				if(ProcessMonitor.has_attribute("ProcessInfoFetchTime"))
					m_ProcessInfoFetchTime=(long)ProcessMonitor.attribute("ProcessInfoFetchTime");
				if(ProcessMonitor.has_attribute("MaxProcessCount"))
					m_MaxProcessCount=(long)ProcessMonitor.attribute("MaxProcessCount");
			}
			xml_node FileCache=Config;
			if(FileCache.moveto_child("FileCache"))
			{
				if(FileCache.has_attribute("CacheSize"))
					m_FileCacheSize=(long)FileCache.attribute("CacheSize");
				if(FileCache.has_attribute("QueryQueueSize"))
					m_FileCacheQueryQueueSize=(long)FileCache.attribute("QueryQueueSize");


			}

			xml_node ServiceList=Config;
			if(ServiceList.moveto_child("ServiceList"))
			{
				for(UINT i=0;i<ServiceList.children();i++)
				{
					xml_node Service=ServiceList.child(i);
					if(_stricmp(Service.name(),"Service")==0)
					{
						SERVICE_INFO ServiceInfo;
						if(Service.has_attribute("Name"))
						{
							strncpy_0(ServiceInfo.Name,MAX_PATH,Service.attribute("Name").getvalue().c_str(),MAX_PATH);
						}
						if(Service.has_attribute("ImageFilePath"))
						{
							strncpy_0(ServiceInfo.ImageFilePath,MAX_PATH,Service.attribute("ImageFilePath").getvalue().c_str(),MAX_PATH);
						}
						if(Service.has_attribute("WorkDir"))
						{
							strncpy_0(ServiceInfo.WorkDir,MAX_PATH,Service.attribute("WorkDir").getvalue().c_str(),MAX_PATH);
						}
						if(Service.has_attribute("StartupParam"))
						{
							strncpy_0(ServiceInfo.StartupParam,MAX_PATH,Service.attribute("StartupParam").getvalue().c_str(),MAX_PATH);
						}					

						m_ServiceInfoList.Add(ServiceInfo);
					}
				}
			}
			
		}
		else
		{
			Log("配置文件[%s]不合法",FileName);
			return false;
		}
	}
	else
	{
		Log("无法打开配置文件[%s]",FileName);
		return false;
	}

	return true;
	FUNCTION_END;
	return false;
}


