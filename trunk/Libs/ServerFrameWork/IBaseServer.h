/****************************************************************************/
/*                                                                          */
/*      �ļ���:    IBaseServer.h                                            */
/*      ��������:  2009��07��06��                                           */
/*      ����:      Sagasarate                                               */
/*                                                                          */
/*      �������Ȩ��Sagasarate(sagasarate@sina.com)����                     */
/*      ����Խ�����������κ���ҵ�ͷ���ҵ�����������                      */
/*      ���뱣���˰�Ȩ����                                                  */
/*                                                                          */
/****************************************************************************/
#pragma once

class IBaseServer
{
public:	

	virtual CNetServer * GetServer()=0;

	virtual BOOL PrintConsoleLog(LPCTSTR szLogMsg)=0;

	virtual void ExecCommand(LPCTSTR szCommand)=0;

	virtual BOOL SetServerStatus(WORD StatusID,const CSmartValue& Value)=0;
	virtual CSmartValue GetServerStatus(WORD StatusID)=0;
	virtual CSmartStruct& GetAllServerStatus()=0;
	virtual void QueryShowDown()=0;
	virtual bool IsServerTerminated()=0;
};