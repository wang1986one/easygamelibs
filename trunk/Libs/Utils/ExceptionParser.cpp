/****************************************************************************/
/*                                                                          */
/*      文件名:    ExceptionParser.cpp                                      */
/*      创建日期:  2010年02月09日                                           */
/*      作者:      Sagasarate                                               */
/*                                                                          */
/*      本软件版权归Sagasarate(sagasarate@sina.com)所有                     */
/*      你可以将本软件用于任何商业和非商业软件开发，但                      */
/*      必须保留此版权声明                                                  */
/*                                                                          */
/****************************************************************************/
#include "StdAfx.h"
#include ".\exceptionparser.h"
#include <signal.h>

#ifndef _M_IX86

#error "The following code only works for x86!"

#endif



void DisableSetUnhandledExceptionFilter()

{

	void *addr = (void*)GetProcAddress(LoadLibrary(_T("kernel32.dll")),"SetUnhandledExceptionFilter");

	if (addr) 

	{

		unsigned char code[16];

		int size = 0;

		//xor eax,eax;

		code[size++] = 0x33;

		code[size++] = 0xC0;

		//ret 4

		code[size++] = 0xC2;

		code[size++] = 0x04;

		code[size++] = 0x00;



		DWORD dwOldFlag, dwTempFlag;

		if(VirtualProtect(addr, size, PAGE_READWRITE, &dwOldFlag))
		{
			if(WriteProcessMemory(GetCurrentProcess(), addr, code, size, NULL))
			{
				PrintImportantLog(0xff,"屏蔽SetUnhandledExceptionFilter成功");
			}
			VirtualProtect(addr, size, dwOldFlag, &dwTempFlag);
		}

		

	}

}



LONG CExceptionParser::ExceptionHander(LPEXCEPTION_POINTERS pException)
{
	CExceptionParser::GetInstance()->ParseException(pException);
	return 0;
}

void CExceptionParser::ExceptionTranslator(UINT Code,LPEXCEPTION_POINTERS pException)
{
	CExceptionParser::GetInstance()->ParseException(pException);	
}


CExceptionParser::CExceptionParser(void)
{
	m_hProcess=NULL;
	m_ExceptionCount=0;
}

CExceptionParser::~CExceptionParser(void)
{
	if(m_hProcess)
		SymCleanup(m_hProcess);
}

void CExceptionParser::Init(UINT Flag)
{
	m_Flag=Flag;

	if(m_Flag&EXCEPTION_SET_DEFAULT_HANDLER)
	{
		SetUnhandledExceptionFilter((LPTOP_LEVEL_EXCEPTION_FILTER)ExceptionHander);

		

		_set_abort_behavior(0, _WRITE_ABORT_MSG | _CALL_REPORTFAULT);
		signal(SIGABRT,SignalHandler);
		_set_invalid_parameter_handler(InvalidParameterHandler);

	}
	if(m_Flag&EXCEPTION_USE_API_HOOK)
	{
		DisableSetUnhandledExceptionFilter();
	}
	if(m_Flag&EXCEPTION_SET_TRANSLATOR)
		_set_se_translator(ExceptionTranslator);
}

void CExceptionParser::ParseException(LPEXCEPTION_POINTERS pException)
{
	CAutoLock Lock(m_CriticalSection);

	if(m_ExceptionCount>0)
	{
		PrintImportantLog(0xff,"发生多次异常捕捉%d",m_ExceptionCount);
		return;
	}
	m_ExceptionCount++;

	SymInit();

	WriteDump(pException);


	CEasyString ModulePath=GetModuleFilePath(NULL);
	CEasyString ExceptionLogFileName;
	CEasyTime CurTime;


	CurTime.FetchLocalTime();

	ExceptionLogFileName.Format("%s.Exception%d-%02d-%02d %02d-%02d-%02d",
		(LPCTSTR)ModulePath,
		CurTime.Year(),CurTime.Month(),CurTime.Day(),
		CurTime.Hour(),CurTime.Minute(),CurTime.Second());

	PrintImportantLog(0,"开始输出异常Log文件:%s.Log",
		(LPCTSTR)ExceptionLogFileName);

	if(!m_ExceptionLog.Create(ExceptionLogFileName,0))
	{
		PrintImportantLog(0,"无法创建异常Log文件:%s.Log",
			(LPCTSTR)ExceptionLogFileName);
	}
	
	
	LogException("-----------------------------------------------------------------");

	LogException("程序发生异常:类型: 0x%x  地址: 0x%x", 
		pException->ExceptionRecord->ExceptionCode ,pException->ExceptionRecord->ExceptionAddress);

	if(pException->ExceptionRecord->ExceptionCode==EXCEPTION_ACCESS_VIOLATION)
	{
		LogException("地址:0x%x%s",
			pException->ExceptionRecord->ExceptionInformation[1],
			pException->ExceptionRecord->ExceptionInformation[0]?"不可写":"不可读");
	}
	

	static ADDRESS_INFO AddressInfo;

	GetAddressInfo((DWORD64)pException->ExceptionRecord->ExceptionAddress,&AddressInfo);
	LogException("地址描述:函数(%s),文件(%s)(%d)",AddressInfo.SymbolInfo.Name,AddressInfo.CppFileName,AddressInfo.LineNumber);

	LogException("调用堆栈:");

	
	ParseCallStack(pException->ContextRecord);

	LogException("-----------------------------------------------------------------");

	
	
}

void CExceptionParser::ParseCallStack(PCONTEXT pContextRecord,UINT MaxLoopCount)
{
	if(m_hProcess==NULL)
		return;


	static HINSTANCE LastInstance=NULL;

	static ADDRESS_INFO AddressInfo;

	static STACKFRAME64 StackFrame;

	ZeroMemory(&StackFrame,sizeof(StackFrame));

	StackFrame.AddrPC.Offset=pContextRecord->Eip;
	StackFrame.AddrPC.Mode=AddrModeFlat;
	StackFrame.AddrStack.Offset=pContextRecord->Esp;
	StackFrame.AddrStack.Mode=AddrModeFlat;
	StackFrame.AddrFrame.Offset=pContextRecord->Ebp;
	StackFrame.AddrFrame.Mode=AddrModeFlat;

	while(MaxLoopCount&&StackWalk64(IMAGE_FILE_MACHINE_I386,m_hProcess,GetCurrentThread(),
		&StackFrame,pContextRecord,0,SymFunctionTableAccess64,SymGetModuleBase64,0))
	{
		if(StackFrame.AddrFrame.Offset==0)
			break;
		BOOL Ret=GetAddressInfo(StackFrame.AddrPC.Offset,&AddressInfo);
		if(LastInstance!=AddressInfo.hInstance)
		{	
			LastInstance=AddressInfo.hInstance;
			LogException("调用模块:%s",AddressInfo.ModuleName);
		}
		LogException("调用地址:0x%X",(DWORD)StackFrame.AddrPC.Offset);
		if(Ret)
			LogException("地址描述:函数(%s),文件(%s)(%d)",AddressInfo.SymbolInfo.Name,AddressInfo.CppFileName,AddressInfo.LineNumber);
		
		
		MaxLoopCount--;
	}
}

BOOL CExceptionParser::GetAddressInfo(DWORD64 Address,ADDRESS_INFO * pAddressInfo)
{
	if(m_hProcess==NULL)
		return false;
	

	pAddressInfo->Address=Address;

	//获取地址对应的模块信息
	MEMORY_BASIC_INFORMATION mbi ;
	ZeroMemory(&mbi,sizeof(mbi));
	VirtualQuery((LPCVOID)Address, &mbi, sizeof( mbi ) ) ;			
	HINSTANCE hInstance = (HINSTANCE)mbi.AllocationBase ;

	if(hInstance)
	{	
		if(pAddressInfo->hInstance!=hInstance)
		{
			pAddressInfo->hInstance=hInstance;			
			GetModuleFileName( hInstance, pAddressInfo->ModuleName, MAX_PATH ) ;			
		}			
	}		

	DWORD64 Displacement=0;

	if(SymFromAddr(m_hProcess,Address,&Displacement,&(pAddressInfo->SymbolInfo)))
	{
		IMAGEHLP_LINE64 LineInfo;
		ZeroMemory(&LineInfo,sizeof(LineInfo));
		LineInfo.SizeOfStruct=sizeof(LineInfo);

		DWORD LineDisplacement=0;

		SymGetLineFromAddr64(m_hProcess,Address,&LineDisplacement,&LineInfo);
		if(LineInfo.FileName)
		{
			strncpy_0(pAddressInfo->CppFileName,MAX_PATH,LineInfo.FileName,MAX_PATH);
			pAddressInfo->LineNumber=LineInfo.LineNumber;
		}
		return TRUE;
	}
	return FALSE;
}

BOOL CExceptionParser::WriteDump(LPEXCEPTION_POINTERS pException)
{
	CWinFileAccessor DumpFile;

	CEasyString ModulePath=GetModuleFilePath(NULL);
	CEasyString DumpFileName;
	CEasyTime CurTime;

	CurTime.FetchLocalTime();

	DumpFileName.Format("%s.Dump%d-%02d-%02d %02d-%02d-%02d.dmp",
		(LPCTSTR)ModulePath,
		CurTime.Year(),CurTime.Month(),CurTime.Day(),
		CurTime.Hour(),CurTime.Minute(),CurTime.Second());

	PrintImportantLog(0xff,"开始写入Dump文件%s",(LPCTSTR)DumpFileName);

	if(DumpFile.Open(DumpFileName,IFileAccessor::modeCreateAlways|IFileAccessor::modeWrite))
	{		
		
		static DWORD ProcessID=GetCurrentProcessId();

		static MINIDUMP_EXCEPTION_INFORMATION ExceptionInfo;
		static MINIDUMP_EXCEPTION_INFORMATION * pExceptionInfo=NULL;

		if(pException)
		{
			ExceptionInfo.ThreadId=GetCurrentThreadId();
			ExceptionInfo.ExceptionPointers=pException;
			ExceptionInfo.ClientPointers=true;
			pExceptionInfo=&ExceptionInfo;
		}
		
		

		if(MiniDumpWriteDump(m_hProcess,ProcessID,DumpFile.GetFileHandle(),
			m_Flag&EXCEPTION_MAKE_FULL_DUMP?MiniDumpWithFullMemory:MiniDumpNormal,
			pExceptionInfo,NULL,NULL))
		{
			PrintImportantLog(0xff,"写入Dump文件成功%s",(LPCTSTR)DumpFileName);
			DumpFile.Close();
			return TRUE;
		}
		else
		{
			PrintImportantLog(0xff,"写入Dump文件失败%s",(LPCTSTR)DumpFileName);
		}
		DumpFile.Close();
		
	}
	else
	{
		PrintImportantLog(0xff,"打开Dump文件失败%s",(LPCTSTR)DumpFileName);
	}

	return FALSE;
}

BOOL CExceptionParser::SymInit()
{
	m_hProcess=GetCurrentProcess();
	CEasyString ModulePath=GetModulePath(NULL);
	if(!SymInitialize(m_hProcess,ModulePath,TRUE))
	{
		PrintImportantLog(0xff,"无法在当前模块路径找到PDB文件，尝试进行目录搜索");
		if(!SymInitialize(m_hProcess,NULL,TRUE))
		{
			PrintImportantLog(0xff,"无法到PDB文件");
			return FALSE;
		}
	}
	return TRUE;
}

UINT CExceptionParser::GetCallStack(DWORD64 * pAddressBuffer,UINT Depth)
{
	HINSTANCE LastInstance=NULL;
	STACKFRAME64 StackFrame;
	ADDRESS_INFO AddressInfo;

	ZeroMemory(&StackFrame,sizeof(StackFrame));

	
	StackFrame.AddrPC.Mode=AddrModeFlat;	
	StackFrame.AddrStack.Mode=AddrModeFlat;	
	StackFrame.AddrFrame.Mode=AddrModeFlat;

	DWORD REIP=0,RESP=0,REBP=0;
	_asm
	{
		mov RESP,esp;
		mov REBP,ebp;
	}

	StackFrame.AddrPC.Offset=REIP;
	StackFrame.AddrStack.Offset=RESP;
	StackFrame.AddrFrame.Offset=REBP;

	UINT Count=0;

	while(Depth&&StackWalk64(IMAGE_FILE_MACHINE_I386,GetCurrentProcess(),GetCurrentThread(),
		&StackFrame,NULL,NULL,SymFunctionTableAccess64,SymGetModuleBase64,NULL))
	{
		if(StackFrame.AddrFrame.Offset==0)
			break;
		pAddressBuffer[Count]=StackFrame.AddrPC.Offset;				
		Count++;
		Depth--;
	}
	return Count;
}

void CExceptionParser::LogException(LPCTSTR Format,...)
{
	va_list	vl;
	va_start(vl,Format);
	m_ExceptionLog.PrintLogVL(ILogPrinter::LOG_LEVEL_NORMAL,0,Format,vl);
	va_end( vl);
}

void CExceptionParser::InvalidParameterHandler(const wchar_t * expression,const wchar_t * function,const wchar_t * file,unsigned int line,uintptr_t pReserved)
{
	CEasyString StrExpression=expression;
	CEasyString StrFunction=function;
	CEasyString StrFile=file;

	PrintImportantLog(0xff,"非法的调用参数[%s][%s][%s][%d]",
		(LPCTSTR)StrExpression,
		(LPCTSTR)StrFunction,
		(LPCTSTR)StrFile,
		line);
}

void CExceptionParser::SignalHandler(int signal)
{
	PrintImportantLog(0xff,"系统提示%d",signal);
}