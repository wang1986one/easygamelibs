/****************************************************************************/
/*                                                                          */
/*      �ļ���:    ToolsLinux.h                                             */
/*      ��������:  2009��07��06��                                           */
/*      ����:      Sagasarate                                               */
/*                                                                          */
/*      �������Ȩ��Sagasarate(sagasarate@sina.com)����                     */
/*      ����Խ�����������κ���ҵ�ͷ���ҵ�����������                      */
/*      ���뱣���˰�Ȩ����                                                  */
/*                                                                          */
/****************************************************************************/
#pragma once




inline CEasyString GetModulePath(HMODULE hModule)
{
	CEasyString ModulePath;

	char   exePath[NAME_MAX+1];
	char   fullPath[MAX_PATH+1];
	sprintf(exePath,"/proc/%d/exe",getpid());
	int Len=readlink(exePath,fullPath,MAX_PATH);
	if(Len>0)
	{
	    fullPath[Len]=0;
		ModulePath=fullPath;
		ModulePath.TrimBuffer();
		int Pos=ModulePath.ReverseFind('/');
		if(Pos>=0)
		{
			ModulePath.Resize(Pos+1);
		}
	}
	return ModulePath;
}

inline CEasyString GetModuleFilePath(HMODULE hModule)
{
	CEasyString ModulePath;

	char   exePath[NAME_MAX+1];
	char   fullPath[MAX_PATH+1];
	sprintf(exePath,"/proc/%d/exe",getpid());
	int Len=readlink(exePath,fullPath,MAX_PATH);
	if(Len>0)
	{
		fullPath[Len]=0;
		ModulePath=fullPath;
		ModulePath.TrimBuffer();		
	}
	return ModulePath;
}

inline CEasyString MakeFullPath(LPCTSTR Path)
{
	TCHAR FilePath[MAX_PATH];
	realpath(Path,FilePath);
	return FilePath;
}

inline CEasyString MakeModuleFullPath(HMODULE hModule,LPCTSTR Path)
{
	CEasyString FilePath=Path;
	if(FilePath.Find(':')<0)
	{
		CEasyString ModulePath=GetModulePath(hModule);
		FilePath=MakeFullPath(ModulePath+FilePath);
	}
	return FilePath;
}



inline CEasyString GetPathDirectory(LPCTSTR Path)
{
	CEasyString Dir=Path;
	int Pos=Dir.ReverseFind('/');
	Dir.Resize(Pos+1,true);
	return Dir;
}

inline CEasyString GetPathFileName(LPCTSTR Path)
{
	CEasyString FileName=Path;
	int Pos=FileName.ReverseFind('/');
	return FileName.Right(FileName.GetLength()-Pos-1);
}

inline void DoSleep(UINT nMilliseconds)
{
	usleep(nMilliseconds*1000);
}

inline int GetSystemCPUCount()
{
	FILE * pFile=NULL;
	int CPUCount=0;

	pFile=fopen("/proc/cpuinfo","r");
	if(pFile)
	{
		char LineBuffer[1024];
		while(fgets(LineBuffer,1023,pFile))
		{
			if(_strnicmp(LineBuffer,"processor",9)==0)
			{
				CPUCount++;
			}
		}

	}
	if(CPUCount<=0)
		CPUCount++;
	return CPUCount;
}

inline bool CreateDirEx(LPCTSTR szDirName)
{
	CEasyString DirName=MakeFullPath(szDirName);

	for(UINT i=1; i<DirName.GetLength(); i++)
	{
		if(DirName[i]=='/')
		{
			DirName[i]=0;
			if(access(DirName,F_OK)!=0)
			{
				if(mkdir(DirName,S_IRWXU)==-1)
				{
					PrintImportantLog(0,"mkdir failed:%d",errno);
					return false;
				}
				else
				{
					return true;
				}
			}
			DirName[i]='/';
		}
	}
	if(access(DirName,F_OK)!=0)
	{
		if(mkdir(DirName,S_IRWXU)==-1)
		{
			PrintImportantLog(0,"mkdir failed:%d",errno);
			return false;
		}
		else
		{
			return true;
		}
	}
	return true;
}
