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



int _tmain(int argc, _TCHAR* argv[])
{	
	if(argc>1)
	{	
		CEasyScriptExecutor ScriptExcutor;
		CESThread ESThread;
		CESVariableList VarList;
		CESFactionList FnList;
		CESBolanStack Script;
		CScriptFunctions Functions;

		VarList.Create(128);
		FnList.Create(128);

		ESThread.SetVariableList(&VarList);
		ESThread.SetFactionList(&FnList);
		ESThread.SetScript(&Script);
		

		FnList.AddFaction("StartService",1,&Functions,(LPSCRIPT_FACTION)&CScriptFunctions::StartService);
		FnList.AddFaction("StopService",1,&Functions,(LPSCRIPT_FACTION)&CScriptFunctions::StopService);
		FnList.AddFaction("CopyFile",2,&Functions,(LPSCRIPT_FACTION)&CScriptFunctions::CopyFile);
		FnList.AddFaction("Sleep",1,&Functions,(LPSCRIPT_FACTION)&CScriptFunctions::Sleep);

		CWinFileAccessor FileAccessor;
		if(FileAccessor.Open(argv[1],IFileAccessor::modeOpen|IFileAccessor::modeRead|IFileAccessor::shareShareAll))
		{
			CEasyString Script;
			UINT Len=FileAccessor.GetSize();
			Script.Resize(Len);
			if(FileAccessor.Read((LPVOID)Script.GetBuffer(),Len)==Len)
			{				
				ESThread.PushScript(Script);
				if(ESThread.GetResultCode()==0)
				{
					ScriptExcutor.ExecScript(ESThread);
					if(ESThread.GetResultCode())
					{
						LPCTSTR ErrMsg=ESGetErrorMsg(ESThread.GetResultCode());
						PrintImportantLog(0,"ִ�нű�ʧ��:%s����%d����",
							ErrMsg,ESThread.GetLastLine());
					}
				}
				else
				{
					LPCTSTR ErrMsg=ESGetErrorMsg(ESThread.GetResultCode());
					PrintImportantLog(0,"�����ű�ʧ��:%s����%d����",
						ErrMsg,ESThread.GetLastLine());
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

