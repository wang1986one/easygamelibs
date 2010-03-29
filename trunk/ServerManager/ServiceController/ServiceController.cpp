/****************************************************************************/
/*                                                                          */
/*      �ļ���:    ServiceController.cpp                                    */
/*      ��������:  2010��02��09��                                           */
/*      ����:      Sagasarate                                               */
/*                                                                          */
/*      �������Ȩ��Sagasarate(sagasarate@sina.com)����                     */
/*      ����Խ�����������κ���ҵ�ͷ���ҵ�����������                      */
/*      ���뱣���˰�Ȩ����                                                  */
/*                                                                          */
/****************************************************************************/
// ServiceController.cpp : �������̨Ӧ�ó������ڵ㡣
//

#include "stdafx.h"

int StartService(INT_PTR FnParam,CVariableList* pVarList,CBolan* pResult,CBolan* pParams,int ParamCount)
{
	pResult->ValueType=VALUE_TYPE_NUMBER;
	pResult->value=0;
	CWinServiceController ServiceController;

	if(ServiceController.OpenService(pParams[0].StrValue))
	{
		if(ServiceController.StartupService(NULL,0,5*60*1000))
		{
			pResult->value=1;
			PrintImportantLog(0,"��������%s�ɹ�",(LPCTSTR)pParams[0].StrValue);
			return 0;
		}
	}
	PrintImportantLog(0,"��������%sʧ��",(LPCTSTR)pParams[0].StrValue);
	return 0;
}

int StopService(INT_PTR FnParam,CVariableList* pVarList,CBolan* pResult,CBolan* pParams,int ParamCount)
{
	pResult->ValueType=VALUE_TYPE_NUMBER;
	pResult->value=0;
	CWinServiceController ServiceController;

	if(ServiceController.OpenService(pParams[0].StrValue))
	{
		if(ServiceController.ShutdownService(5*60*1000))
		{
			pResult->value=1;
			PrintImportantLog(0,"�رշ���%s�ɹ�",(LPCTSTR)pParams[0].StrValue);
			return 0;
		}
	}
	PrintImportantLog(0,"�رշ���%sʧ��",(LPCTSTR)pParams[0].StrValue);
	return 0;
}

int CopyFile(INT_PTR FnParam,CVariableList* pVarList,CBolan* pResult,CBolan* pParams,int ParamCount)
{
	SHFILEOPSTRUCT Operation;
	char szFileFrom[MAX_PATH];
	char szFileTo[MAX_PATH];

	strncpy_0(szFileFrom,MAX_PATH,MakeFullPath(pParams[0].StrValue),MAX_PATH);
	szFileFrom[strlen(szFileFrom)+1]=0;

	strncpy_0(szFileTo,MAX_PATH,MakeFullPath(pParams[1].StrValue),MAX_PATH);
	szFileTo[strlen(szFileTo)+1]=0;

	Operation.hwnd=NULL;
	Operation.wFunc=FO_COPY;
	Operation.pFrom=szFileFrom;
	Operation.pTo=szFileTo;
	Operation.fAnyOperationsAborted=FALSE;
	Operation.hNameMappings=NULL;
	Operation.lpszProgressTitle=NULL;
	Operation.fFlags=FOF_NOCONFIRMATION|FOF_NOCONFIRMMKDIR|FOF_NOERRORUI|FOF_SILENT;

	pResult->ValueType=VALUE_TYPE_NUMBER;
	UINT Code=SHFileOperation(&Operation);
	if(Code==0)
	{
		PrintImportantLog(0,"�����ļ�[%s]��[%s]�ɹ�",
			(LPCTSTR)pParams[0].StrValue,
			(LPCTSTR)pParams[1].StrValue);
		pResult->value=1;
	}
	else
	{		
		PrintImportantLog(0,"�����ļ�[%s]��[%s]ʧ��0x%X",
			(LPCTSTR)pParams[0].StrValue,
			(LPCTSTR)pParams[1].StrValue,
			Code);
		pResult->value=0;
	}


	return 0;
}

int Sleep(INT_PTR FnParam,CVariableList* pVarList,CBolan* pResult,CBolan* pParams,int ParamCount)
{
	PrintImportantLog(0,"Sleep%u����",(UINT)pParams[0].value);
	Sleep(pParams[0].value);
	return 0;
}

int _tmain(int argc, _TCHAR* argv[])
{	
	if(argc>1)
	{		
		CExpressionCaculator ScriptExcutor(128,128);

		ScriptExcutor.AddFaction("StartService",1,NULL,StartService);
		ScriptExcutor.AddFaction("StopService",1,NULL,StopService);
		ScriptExcutor.AddFaction("CopyFile",2,NULL,CopyFile);
		ScriptExcutor.AddFaction("Sleep",1,NULL,Sleep);

		CWinFileAccessor FileAccessor;
		if(FileAccessor.Open(argv[1],IFileAccessor::modeOpen|IFileAccessor::modeRead|IFileAccessor::shareShareAll))
		{
			CEasyString Script;
			UINT Len=FileAccessor.GetSize();
			Script.Resize(Len);
			if(FileAccessor.Read((LPVOID)Script.GetBuffer(),Len)==Len)
			{
				CBolan Result;
				int Code=ScriptExcutor.ExecScript(Script,Result);
				if(Code)
				{
					LPCTSTR ErrMsg=ScriptExcutor.GetErrorMsg(Code);
					PrintImportantLog(0,"ִ�нű�ʧ��:%s",ErrMsg);
				}
			}
			else
			{
				PrintImportantLog(0,"��ȡ�ű��ļ�ʧ��");
			}
		}
		else
		{
			PrintImportantLog(0,"�޷��򿪽ű��ļ�%s",argv[1]);
		}
	}
	else
	{
		printf("Usage:ServiceController ScriptFileName");
	}
	return 0;
}

