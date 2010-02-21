#pragma once

#include "ExceptionParser.h"




#define CHECK_HRESULT(hr) \
	if(FAILED(hr))\
{\
	PrintSystemLog(0xff00ff,"HResult Error:<%s><%d>[%x]",__FILE__,__LINE__,hr);\
	return false;\
}

inline CEasyString GetModulePath(HMODULE hModule)
{
	CEasyString ModulePath;
	ModulePath.Resize(MAX_PATH);
	GetModuleFileName(hModule,ModulePath,MAX_PATH);
	ModulePath.TrimBuffer();
	int Pos=ModulePath.ReverseFind('\\');
	if(Pos>=0)
	{
		ModulePath.Resize(Pos+1);
	}
	return ModulePath;
}

inline CEasyString MakeFullPath(LPCTSTR Path)
{
	CEasyString FilePath;
	FilePath.Resize(MAX_PATH);
	_tfullpath(FilePath,Path,MAX_PATH);
	FilePath.TrimBuffer();
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
	CEasyString Drv,Dir;
	Drv.Resize(8);
	Dir.Resize(MAX_PATH);
	_tsplitpath_s(Path,Drv,8,Dir,MAX_PATH,NULL,0,NULL,0);
	Drv.TrimBuffer();
	Dir.TrimBuffer();
	Drv+=Dir;
	return Drv;
}

inline CEasyString GetPathFileName(LPCTSTR Path)
{
	CEasyString FileName,ExtName;

	FileName.Resize(MAX_PATH);
	ExtName.Resize(8);
	_splitpath_s(Path,NULL,0,NULL,0,FileName,MAX_PATH,ExtName,8);
	FileName.TrimBuffer();
	ExtName.TrimBuffer();
	FileName+=ExtName;
	return FileName;
}






inline void DoSleep(UINT nMilliseconds)
{
	Sleep(nMilliseconds);
}


inline bool CreateDirEx(LPCTSTR szDirName)
{
	int Ret=SHCreateDirectoryEx(NULL,szDirName,NULL);
	if(Ret==ERROR_SUCCESS||Ret==ERROR_ALREADY_EXISTS||Ret==ERROR_FILE_EXISTS)
		return true;
	else
		return false;
}

inline UINT GetCurProcessID()
{
	return GetCurrentProcessId();
}