/****************************************************************************/
/*                                                                          */
/*      文件名:    DOSClientMsgCatcher.h                                    */
/*      创建日期:  2009年07月06日                                           */
/*      作者:      Sagasarate                                               */
/*                                                                          */
/*      本软件版权归Sagasarate(sagasarate@sina.com)所有                     */
/*      你可以将本软件用于任何商业和非商业软件开发，但                      */
/*      必须保留此版权声明                                                  */
/*                                                                          */
/****************************************************************************/
#pragma once


#define DECLARE_CMSG_MAP()	protected: virtual VOID RegisterMsgProc(BOOL IsRegister,CDOSClientMsgManager * pMsgManager);

#define BEGIN_CMSG_MAP(ClassName,ParentClassName) \
	VOID ClassName::RegisterMsgProc(BOOL IsRegister,CDOSClientMsgManager * pMsgManager)\
	{\
		ParentClassName::RegisterMsgProc(IsRegister,pMsgManager);

#define ON_CMSG(MsgID,MsgFn) pMsgManager->RegisterMsgProc(IsRegister,MsgID,this,(DOS_MSG_PROC)MsgFn);

#define END_CMSG_MAP() \
	}


#define ENTER_CMSG_MAP(pMsgManager) RegisterMsgProc(true,(pMsgManager));
#define LEAVE_CMSG_MAP(pMsgManager) RegisterMsgProc(false,(pMsgManager));


//class CDOSClientMsgCatcher:
//	public CNameObject
//{
//protected:
//	DECLARE_CLASS_INFO(CDOSClientMsgCatcher);
//public:
//	CDOSClientMsgCatcher(void);
//	virtual ~CDOSClientMsgCatcher(void);
//protected:
//	virtual void RegisterMsgProc(BOOL IsRegister,CDOSClientMsgManager * pMsgManager);
//};

