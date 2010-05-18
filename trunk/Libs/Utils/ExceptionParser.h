/****************************************************************************/
/*                                                                          */
/*      �ļ���:    ExceptionParser.h                                        */
/*      ��������:  2009��10��23��                                           */
/*      ����:      Sagasarate                                               */
/*                                                                          */
/*      �������Ȩ��Sagasarate(sagasarate@sina.com)����                     */
/*      ����Խ�����������κ���ҵ�ͷ���ҵ�����������                      */
/*      ���뱣���˰�Ȩ����                                                  */
/*                                                                          */
/****************************************************************************/
#pragma once
#include <Dbghelp.h>

#define MAX_SYMBOL_NAME		1024

enum EXCEPTION_HANDLE_MODE
{
	EXCEPTION_SET_DEFAULT_HANDLER=1,
	EXCEPTION_SET_TRANSLATOR=(1<<1),
	EXCEPTION_USE_API_HOOK=(1<<2),
};

class CExceptionParser :
	public CStaticObject<CExceptionParser>
{
public:
	struct SYMBOL_INFO_EX:public SYMBOL_INFO
	{
		char NameBuffer[MAX_SYMBOL_NAME];
		SYMBOL_INFO_EX()
		{
			ZeroMemory(this,sizeof(*this));
			MaxNameLen=MAX_SYMBOL_NAME;
		}
	};

	struct ADDRESS_INFO
	{
		DWORD64			Address;
		HINSTANCE		hInstance;
		char			ModuleName[MAX_PATH];
		SYMBOL_INFO_EX	SymbolInfo;
		char			CppFileName[MAX_PATH];
		int				LineNumber;
		ADDRESS_INFO()
		{
			Address=0;
			hInstance=NULL;
			ModuleName[0]=0;
			CppFileName[0]=0;
			LineNumber=-1;
		}
	};
protected:
	CEasyCriticalSection	m_CriticalSection;
	HANDLE					m_hProcess;
	int						m_ExceptionCount;
	CFileLogPrinter			m_ExceptionLog;
public:
	CExceptionParser(void);
	~CExceptionParser(void);

	void Init(UINT HandleMode);

	void ParseException(LPEXCEPTION_POINTERS pException);

	static LONG ExceptionHander(LPEXCEPTION_POINTERS pException);
	static void ExceptionTranslator(UINT Code,LPEXCEPTION_POINTERS pException);

	BOOL WriteDump(LPEXCEPTION_POINTERS pException);

	BOOL SymInit();

	UINT GetCallStack(DWORD64 * pAddressBuffer,UINT Depth);

	BOOL GetAddressInfo(DWORD64 Address,ADDRESS_INFO * pAddressInfo);

protected:
	void ParseCallStack(PCONTEXT pContextRecord,UINT MaxLoopCount=16);	

	void LogException(LPCTSTR Format,...);


	static void InvalidParameterHandler(
		const wchar_t * expression,
		const wchar_t * function, 
		const wchar_t * file, 
		unsigned int line,
		uintptr_t pReserved
		);

	static void SignalHandler(int signal);	
	
};



