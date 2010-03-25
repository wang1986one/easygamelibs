/****************************************************************************/
/*                                                                          */
/*      文件名:    DOSClientMsgManager.h                                    */
/*      创建日期:  2009年07月06日                                           */
/*      作者:      Sagasarate                                               */
/*                                                                          */
/*      本软件版权归Sagasarate(sagasarate@sina.com)所有                     */
/*      你可以将本软件用于任何商业和非商业软件开发，但                      */
/*      必须保留此版权声明                                                  */
/*                                                                          */
/****************************************************************************/
#pragma once


class CDOSClientMsgCatcher;

typedef BOOL (CNameObject::*CMSG_PROC)(CDOSSimpleMessage * pMessage);



struct CMSG_PROC_ENTRY
{
	CNameObject * pObject;
	CMSG_PROC pProc;
};


typedef std::vector<CMSG_PROC_ENTRY> CMSG_PROC_LIST;


class CDOSClientMsgManager:
	public CNameObject
{
protected:
	std::map<WORD,CMSG_PROC_LIST>	m_MsgMap;

	DECLARE_CLASS_INFO(CDOSClientMsgManager);
public:
	CDOSClientMsgManager(void);
	~CDOSClientMsgManager(void);


	BOOL DealMsg(CDOSSimpleMessage * pMessage);

	void RegisterMsgProc(BOOL IsRegister,WORD MsgID,CNameObject * ProcObject,CMSG_PROC MsgProc);

};
