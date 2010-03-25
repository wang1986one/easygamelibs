/****************************************************************************/
/*                                                                          */
/*      文件名:    DOSMsgManager.h                                          */
/*      创建日期:  2009年09月11日                                           */
/*      作者:      Sagasarate                                               */
/*                                                                          */
/*      本软件版权归Sagasarate(sagasarate@sina.com)所有                     */
/*      你可以将本软件用于任何商业和非商业软件开发，但                      */
/*      必须保留此版权声明                                                  */
/*                                                                          */
/****************************************************************************/
#pragma once




typedef BOOL (CNameObject::*DOS_MSG_PROC)(CDOSMessage * pMessage);



struct DOS_MSG_PROC_ENTRY
{
	CNameObject * pObject;
	DOS_MSG_PROC pProc;
};


class CDOSMsgManager:
	public CNameObject
{
protected:	
	CStaticMap<WORD,DOS_MSG_PROC_ENTRY>	m_MsgMap;

	DECLARE_CLASS_INFO(CDOSMsgManager);
public:
	CDOSMsgManager(void);
	~CDOSMsgManager(void);

	BOOL Init(int MaxMsgCount);

	BOOL DealMsg(CDOSMessage * pMessage);

	virtual void RegisterMsgProc(BOOL IsRegister,WORD MsgID,CNameObject * ProcObject,DOS_MSG_PROC MsgProc);

};
