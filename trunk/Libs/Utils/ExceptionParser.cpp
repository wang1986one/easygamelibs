/****************************************************************************/
/*                                                                          */
/*      �ļ���:    ExceptionParser.cpp                                      */
/*      ��������:  2010��02��09��                                           */
/*      ����:      Sagasarate                                               */
/*                                                                          */
/*      �������Ȩ��Sagasarate(sagasarate@sina.com)����                     */
/*      ����Խ�����������κ���ҵ�ͷ���ҵ�����������                      */
/*      ���뱣���˰�Ȩ����                                                  */
/*                                                                          */
/****************************************************************************/
#include "StdAfx.h"
#include ".\exceptionparser.h"
#include <signal.h>


#ifdef UNICODE
#define SymInitializeT			SymInitializeW
#define SymGetLineFromAddr64T	SymGetLineFromAddrW64
#define IMAGEHLP_LINE64T		IMAGEHLP_LINEW64
#define SymLoadModuleExT		SymLoadModuleExW
#else
#define SymInitializeT			SymInitialize
#define SymGetLineFromAddr64T	SymGetLineFromAddr64
#define IMAGEHLP_LINE64T		IMAGEHLP_LINE64
#define SymLoadModuleExT		SymLoadModuleEx
#endif



void DisableSetUnhandledExceptionFilter()

{
#ifdef _M_IX86
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
				PrintImportantLog(0xff,_T("����SetUnhandledExceptionFilter�ɹ�"));
			}
			VirtualProtect(addr, size, dwOldFlag, &dwTempFlag);
		}

		

	}
#endif
}



LONG CExceptionParser::ExceptionHander(LPEXCEPTION_POINTERS pException)
{
	PrintImportantLog(0xff,_T("�յ�δ������쳣"));
	CExceptionParser::GetInstance()->ParseException(pException);
	return 0;
}


void CExceptionParser::ExceptionTranslator(UINT Code,LPEXCEPTION_POINTERS pException)
{
	PrintImportantLog(0xff,_T("��׽���쳣,Code=%u"),Code);
	CExceptionParser::GetInstance()->ParseException(pException);	
}

LONG CExceptionParser::ExceptionPrinter(LPEXCEPTION_POINTERS pException,UINT64 DebugInfo1,LPCTSTR szFunName)
{
	PrintImportantLog(0xff,_T("��ʼ����쳣,DebugInfo1=0x%llX,�ں���%s"),DebugInfo1,szFunName);
	CExceptionParser::GetInstance()->ParseException(pException);
	return EXCEPTION_EXECUTE_HANDLER;
}

CExceptionParser::CExceptionParser(void)
{
	m_hProcess=NULL;
	m_hExceptionLog=INVALID_HANDLE_VALUE;
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
		PrintImportantLog(0xff,_T("����δ�����쳣��׽"));
		SetUnhandledExceptionFilter((LPTOP_LEVEL_EXCEPTION_FILTER)ExceptionHander);

		

		_set_abort_behavior(0, _WRITE_ABORT_MSG | _CALL_REPORTFAULT);
		signal(SIGABRT,SignalHandler);
		_set_invalid_parameter_handler(InvalidParameterHandler);

	}
	if(m_Flag&EXCEPTION_USE_API_HOOK)
	{
		PrintImportantLog(0xff,_T("����ϵͳ�쳣��׽"));
		DisableSetUnhandledExceptionFilter();
	}
	if(m_Flag&EXCEPTION_SET_TRANSLATOR)
	{
		PrintImportantLog(0xff,_T("�����쳣�������"));
		_set_se_translator(ExceptionTranslator);
	}

	if(m_Flag&EXCEPTION_PRE_INIT_SYM)
		SymInit();
}

void CExceptionParser::ParseException(LPEXCEPTION_POINTERS pException)
{
	CAutoLock Lock(m_CriticalSection);

	m_hProcess=GetCurrentProcess();	

	PrintImportantLog(0xff,_T("�����쳣����"));

	PROCESS_MEMORY_COUNTERS MemoryInfo;
	if(GetProcessMemoryInfo(m_hProcess,&MemoryInfo,sizeof(MemoryInfo)))
	{
#ifdef _WIN64
		PrintImportantLog(0xff,_T("�����ڴ�ռ��:%llu,�����ڴ�ռ��:%llu"),MemoryInfo.WorkingSetSize,MemoryInfo.PagefileUsage);
#else
		PrintImportantLog(0xff,_T("�����ڴ�ռ��:%u,�����ڴ�ռ��:%u"),MemoryInfo.WorkingSetSize,MemoryInfo.PagefileUsage);
#endif		
	}
	else
	{
		PrintImportantLog(0xff,_T("��ȡ�����ڴ�ʹ�����ʧ��%d"),GetLastError());
	}

	if(m_ExceptionCount>0)
	{
		PrintImportantLog(0xff,_T("��������쳣��׽%d"),m_ExceptionCount);
		return;
	}
	m_ExceptionCount++;


	
	if((m_Flag&EXCEPTION_PRE_INIT_SYM)==0)
		SymInit();



	TCHAR szModulePath[MAX_PATH];

	GetModuleFileName(NULL,szModulePath,MAX_PATH);	

	TCHAR szExceptionLogFileName[MAX_PATH];
	SYSTEMTIME CurTime;

	GetLocalTime(&CurTime);

	_stprintf_s(szExceptionLogFileName,MAX_PATH,_T("%s.Exception%d-%02d-%02d %02d-%02d-%02d.log"),
		szModulePath,
		CurTime.wYear,CurTime.wMonth,CurTime.wDay,
		CurTime.wHour,CurTime.wMinute,CurTime.wSecond);

	PrintImportantLog(0,_T("��ʼ����쳣Log�ļ�:%s.Log"),
		szExceptionLogFileName);

	m_hExceptionLog=CreateFile(szExceptionLogFileName,GENERIC_WRITE,0,NULL,CREATE_ALWAYS,FILE_ATTRIBUTE_NORMAL|FILE_FLAG_WRITE_THROUGH,NULL);
	if(m_hExceptionLog==INVALID_HANDLE_VALUE)
	{
		PrintImportantLog(0,_T("�޷������쳣Log�ļ�:%s.Log"),
			szExceptionLogFileName);
	}
	
	
	LogException(_T("-----------------------------------------------------------------"));

	LogException(_T("�������쳣:����: 0x%x  ��ַ: 0x%x"), 
		pException->ExceptionRecord->ExceptionCode ,pException->ExceptionRecord->ExceptionAddress);

	switch(pException->ExceptionRecord->ExceptionCode)
	{
	case EXCEPTION_ACCESS_VIOLATION:
		{
			LogException(_T("��ַ:0x%x%s"),
				pException->ExceptionRecord->ExceptionInformation[1],
				pException->ExceptionRecord->ExceptionInformation[0]?_T("����д"):_T("���ɶ�"));
		}
		break;
	case EXCEPTION_DATATYPE_MISALIGNMENT:
		LogException(_T("EXCEPTION_DATATYPE_MISALIGNMENT"));
		break;
	case EXCEPTION_BREAKPOINT:
		LogException(_T("EXCEPTION_BREAKPOINT"));
		break;
	case EXCEPTION_SINGLE_STEP:
		LogException(_T("EXCEPTION_SINGLE_STEP"));
		break;
	case EXCEPTION_ARRAY_BOUNDS_EXCEEDED:
		LogException(_T("EXCEPTION_ARRAY_BOUNDS_EXCEEDED"));
		break;
	case EXCEPTION_FLT_DENORMAL_OPERAND:
		LogException(_T("EXCEPTION_FLT_DENORMAL_OPERAND"));
		break;
	case EXCEPTION_FLT_DIVIDE_BY_ZERO:
		LogException(_T("EXCEPTION_FLT_DIVIDE_BY_ZERO"));
		break;
	case EXCEPTION_FLT_INEXACT_RESULT:
		LogException(_T("EXCEPTION_FLT_INEXACT_RESULT"));
		break;
	case EXCEPTION_FLT_INVALID_OPERATION:
		LogException(_T("EXCEPTION_FLT_INVALID_OPERATION"));
		break;
	case EXCEPTION_FLT_OVERFLOW:
		LogException(_T("EXCEPTION_FLT_OVERFLOW"));
		break;
	case EXCEPTION_FLT_STACK_CHECK:
		LogException(_T("EXCEPTION_FLT_STACK_CHECK"));
		break;
	case EXCEPTION_FLT_UNDERFLOW:
		LogException(_T("EXCEPTION_FLT_UNDERFLOW"));
		break;
	case EXCEPTION_INT_DIVIDE_BY_ZERO:
		LogException(_T("EXCEPTION_INT_DIVIDE_BY_ZERO"));
		break;
	case EXCEPTION_INT_OVERFLOW:
		LogException(_T("EXCEPTION_INT_OVERFLOW"));
		break;
	case EXCEPTION_PRIV_INSTRUCTION:
		LogException(_T("EXCEPTION_PRIV_INSTRUCTION"));
		break;
	case EXCEPTION_IN_PAGE_ERROR:
		LogException(_T("EXCEPTION_IN_PAGE_ERROR"));
		break;
	case EXCEPTION_ILLEGAL_INSTRUCTION:
		LogException(_T("EXCEPTION_ILLEGAL_INSTRUCTION"));
		break;
	case EXCEPTION_NONCONTINUABLE_EXCEPTION:
		LogException(_T("EXCEPTION_NONCONTINUABLE_EXCEPTION"));
		break;
	case EXCEPTION_STACK_OVERFLOW:
		LogException(_T("ջ���"));
		break;
	case EXCEPTION_INVALID_DISPOSITION:
		LogException(_T("EXCEPTION_INVALID_DISPOSITION"));
		break;
	case EXCEPTION_GUARD_PAGE:
		LogException(_T("EXCEPTION_GUARD_PAGE"));
		break;
	case EXCEPTION_INVALID_HANDLE:
		LogException(_T("EXCEPTION_INVALID_HANDLE"));
		break;	
	}

	static ADDRESS_INFO AddressInfo;

	GetAddressInfo((DWORD64)pException->ExceptionRecord->ExceptionAddress,&AddressInfo);
	LogException(_T("��ַ����:����(%s),�ļ�(%s)(%d)"),AddressInfo.SymbolInfo.Name,AddressInfo.CppFileName,AddressInfo.LineNumber);

	LogException(_T("���ö�ջ:"));

	ParseCallStack(pException->ContextRecord);

	LogException(_T("-----------------------------------------------------------------"));

	if(m_Flag&EXCEPTION_MAKE_DUMP)
		WriteDump(pException);
	
}


void CExceptionParser::ParseCallStack(PCONTEXT pContextRecord,UINT MaxLoopCount)
{
	if(m_hProcess==NULL)
		return;


	static HINSTANCE LastInstance=NULL;

	static ADDRESS_INFO AddressInfo;

	static STACKFRAME64 StackFrame;

	ZeroMemory(&StackFrame,sizeof(StackFrame));

	DWORD MachineType;

#ifdef _M_IX86
	MachineType=IMAGE_FILE_MACHINE_I386;
	StackFrame.AddrPC.Offset=pContextRecord->Eip;
	StackFrame.AddrPC.Mode=AddrModeFlat;
	StackFrame.AddrStack.Offset=pContextRecord->Esp;
	StackFrame.AddrStack.Mode=AddrModeFlat;
	StackFrame.AddrFrame.Offset=pContextRecord->Ebp;
	StackFrame.AddrFrame.Mode=AddrModeFlat;
#else
	MachineType=IMAGE_FILE_MACHINE_AMD64;
	StackFrame.AddrPC.Offset=pContextRecord->Rip;
	StackFrame.AddrPC.Mode=AddrModeFlat;
	StackFrame.AddrStack.Offset=pContextRecord->Rsp;
	StackFrame.AddrStack.Mode=AddrModeFlat;
	StackFrame.AddrFrame.Offset=pContextRecord->Rbp;
	StackFrame.AddrFrame.Mode=AddrModeFlat;
#endif

	while(MaxLoopCount&&StackWalk64(MachineType,m_hProcess,GetCurrentThread(),
		&StackFrame,pContextRecord,NULL,SymFunctionTableAccess64,SymGetModuleBase64,NULL))
	{
		if(StackFrame.AddrFrame.Offset==0)
			break;
		BOOL Ret=GetAddressInfo(StackFrame.AddrPC.Offset,&AddressInfo);
		if(LastInstance!=AddressInfo.hInstance)
		{	
			LastInstance=AddressInfo.hInstance;
			LogException(_T("����ģ��:[0x%llX]%s"),(UINT64)LastInstance,AddressInfo.ModuleName);
		}
		LogException(_T("���õ�ַ:0x%llX"),(DWORD64)StackFrame.AddrPC.Offset);
		if(Ret)
			LogException(_T("��ַ����:����(%s),�ļ�(%s)(%d)"),AddressInfo.SymbolInfo.Name,AddressInfo.CppFileName,AddressInfo.LineNumber);
		
		
		MaxLoopCount--;
	}

}

BOOL CExceptionParser::GetAddressInfo(DWORD64 Address,ADDRESS_INFO * pAddressInfo)
{
	if(m_hProcess==NULL)
		return false;
	

	pAddressInfo->Address=Address;

	//��ȡ��ַ��Ӧ��ģ����Ϣ
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
		IMAGEHLP_LINE64T LineInfo;
		ZeroMemory(&LineInfo,sizeof(LineInfo));
		LineInfo.SizeOfStruct=sizeof(LineInfo);

		DWORD LineDisplacement=0;

		SymGetLineFromAddr64T(m_hProcess,Address,&LineDisplacement,&LineInfo);
		if(LineInfo.FileName)
		{
			strncpy_0(pAddressInfo->CppFileName,MAX_PATH,LineInfo.FileName,MAX_PATH);
			pAddressInfo->LineNumber=LineInfo.LineNumber;
		}
		return TRUE;
	}
	PrintImportantLog(0,_T("SymFromAddr(0x%X,0x%llX,%d):%d"),
		m_hProcess,Address,pAddressInfo->SymbolInfo.MaxNameLen,GetLastError());
	return FALSE;
}

BOOL CExceptionParser::WriteDump(LPEXCEPTION_POINTERS pException)
{
	HANDLE hDumpFile;

	TCHAR szModuleFileName[MAX_PATH];
	GetModuleFileName(NULL,szModuleFileName,MAX_PATH);	

	TCHAR szDumpFileName[MAX_PATH];

	SYSTEMTIME CurTime;

	GetLocalTime(&CurTime);

	_stprintf_s(szDumpFileName,MAX_PATH,_T("%s.Dump%d-%02d-%02d %02d-%02d-%02d.dmp"),
		szModuleFileName,
		CurTime.wYear,CurTime.wMonth,CurTime.wDay,
		CurTime.wHour,CurTime.wMinute,CurTime.wSecond);	

	PrintImportantLog(0xff,_T("��ʼд��%sDump�ļ�%s"),
		m_Flag&EXCEPTION_MAKE_FULL_DUMP?_T("����"):_T("��С"),
		szDumpFileName);
	hDumpFile=CreateFile(szDumpFileName,GENERIC_WRITE,0,NULL,CREATE_ALWAYS,FILE_ATTRIBUTE_NORMAL,NULL);
	if(hDumpFile!=INVALID_HANDLE_VALUE)
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
		
		PrintImportantLog(0xff,_T("����Dump�ļ��ɹ�"));

		if(MiniDumpWriteDump(m_hProcess,ProcessID,hDumpFile,
			m_Flag&EXCEPTION_MAKE_FULL_DUMP?MiniDumpWithFullMemory:MiniDumpNormal,
			pExceptionInfo,NULL,NULL))
		{
			PrintImportantLog(0xff,_T("д��Dump�ļ��ɹ�%s"),szDumpFileName);
			CloseHandle(hDumpFile);
			return TRUE;
		}
		else
		{
			PrintImportantLog(0xff,_T("д��Dump�ļ�ʧ��%s"),szDumpFileName);
		}
		CloseHandle(hDumpFile);
		
	}
	else
	{
		PrintImportantLog(0xff,_T("��Dump�ļ�ʧ��%s"),szDumpFileName);
	}

	return FALSE;
}

BOOL CExceptionParser::SymInit()
{
	PrintImportantLog(0xff,_T("��ʼ��ʼ������"));

	m_hProcess=GetCurrentProcess();	

	SymCleanup(m_hProcess);

	TCHAR szModulePath[MAX_PATH];

	GetModuleFileName(NULL,szModulePath,MAX_PATH);

	TCHAR * pTail=_tcsrchr(szModulePath,'\\');
	if(pTail)
	{
		pTail++;
		*pTail=0;
	}
	
	PrintImportantLog(0xff,_T("���������ļ���:%s"),szModulePath);
	if(!SymInitializeT(m_hProcess,pTail,TRUE))
	{
		PrintImportantLog(0xff,_T("�޷��ڵ�ǰģ��·���ҵ�PDB�ļ������Խ���Ŀ¼����"));
		if(!SymInitialize(m_hProcess,NULL,TRUE))
		{
			PrintImportantLog(0xff,_T("�޷���PDB�ļ�"));
			return FALSE;
		}
	}
	PrintImportantLog(0xff,_T("��ʼ���������"));

	return TRUE;
}



BOOL CExceptionParser::SymLoadFromModule(LPCTSTR szModuleFileName)
{
	PrintImportantLog(0xff,_T("��ʼ��%s���ط���"),szModuleFileName);
	HMODULE hModule=GetModuleHandle(szModuleFileName);
	if(hModule)
	{
		MODULEINFO ModuleInfo;
		ZeroMemory(&ModuleInfo,sizeof(ModuleInfo));
		if(GetModuleInformation(m_hProcess,hModule,&ModuleInfo,sizeof(ModuleInfo)))
		{
			PrintImportantLog(0xff,_T("ģ����ʼ��ַ:0x%llX"),(DWORD64)ModuleInfo.lpBaseOfDll);
			if(SymLoadModuleExT(m_hProcess,NULL,szModuleFileName,NULL,(DWORD64)ModuleInfo.lpBaseOfDll,0,NULL,0))
			{
				PrintImportantLog(0xff,_T("���ط��ųɹ�"));
				return TRUE;
			}
			else
			{
				PrintImportantLog(0xff,_T("���ط���ʧ��%d"),GetLastError());			
			}
		}
		else
		{
			PrintImportantLog(0xff,_T("��ȡģ����Ϣʧ��%d"),GetLastError());
		}
	}
	else
	{
		PrintImportantLog(0xff,_T("��ȡģ����ʧ��%d"),GetLastError());
	}
	return FALSE;
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

#ifdef _M_IX86
	DWORD REIP=0,RESP=0,REBP=0;
	_asm
	{
		mov RESP,esp;
		mov REBP,ebp;
	}

	StackFrame.AddrPC.Offset=REIP;
	StackFrame.AddrStack.Offset=RESP;
	StackFrame.AddrFrame.Offset=REBP;
#else

#endif
	UINT Count=0;

	CONTEXT Context;

	ZeroMemory(&Context,sizeof(Context));

	while(Depth&&StackWalk64(IMAGE_FILE_MACHINE_I386,GetCurrentProcess(),GetCurrentThread(),
		&StackFrame,&Context,NULL,SymFunctionTableAccess64,SymGetModuleBase64,NULL))
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
	if(m_hExceptionLog!=INVALID_HANDLE_VALUE)
	{
		va_list	vl;
		va_start(vl,Format);
		TCHAR szBuff[4096];
		_vstprintf_s(szBuff,4096,Format,vl);
		_tcscat_s(szBuff,4096,_T("\r\n"));
		DWORD Len=_tcslen(szBuff);
		WriteFile(m_hExceptionLog,szBuff,Len,&Len,NULL);
		va_end( vl);
	}
}

void CExceptionParser::InvalidParameterHandler(const wchar_t * expression,const wchar_t * function,const wchar_t * file,unsigned int line,uintptr_t pReserved)
{
#ifdef UNICODE
	PrintImportantLog(0xff,_T("�Ƿ��ĵ��ò���[%s][%s][%s][%d]"),
		expression,
		function,
		file,
		line);
#else
	char szExpression[1024];
	char szFunction[1024];
	char szFile[MAX_PATH];

	UnicodeToAnsi(expression,wcslen(expression),szExpression,1024);
	UnicodeToAnsi(function,wcslen(function),szFunction,1024);
	UnicodeToAnsi(file,wcslen(file),szFile,1024);

	szExpression[1023]=0;
	szFunction[1023]=0;
	szFile[MAX_PATH-1]=0;

	PrintImportantLog(0xff,_T("�Ƿ��ĵ��ò���[%s][%s][%s][%d]"),
		szExpression,
		szFunction,
		szFile,
		line);
#endif
}

void CExceptionParser::SignalHandler(int signal)
{
	PrintImportantLog(0xff,_T("ϵͳ��ʾ%d"),signal);
}