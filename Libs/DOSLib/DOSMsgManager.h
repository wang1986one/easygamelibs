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
