/****************************************************************************/
/*                                                                          */
/*      文件名:    DOSMsgCatcher.h                                          */
/*      创建日期:  2009年07月06日                                           */
/*      作者:      Sagasarate                                               */
/*                                                                          */
/*      本软件版权归Sagasarate(sagasarate@sina.com)所有                     */
/*      你可以将本软件用于任何商业和非商业软件开发，但                      */
/*      必须保留此版权声明                                                  */
/*                                                                          */
/****************************************************************************/
#pragma once


#define DOS_DECLARE_MSG_MAP()	protected: virtual void RegisterMsgProcess(BOOL IsRegister,CDOSMsgManager * pMsgManager);

#define DOS_BEGIN_MSG_MAP(ClassName,ParentClassName) \
	void ClassName::RegisterMsgProcess(BOOL IsRegister,CDOSMsgManager * pMsgManager)\
	{\
		ParentClassName::RegisterMsgProcess(IsRegister,pMsgManager);

#define DOS_ON_MSG(MsgID,MsgFn) pMsgManager->RegisterMsgProcess(IsRegister,MsgID,this,static_cast<DOS_MSG_PROC>(MsgFn));

#define DOS_END_MSG_MAP() \
	}


#define DOS_ENTER_MSG_MAP(pMsgManager) RegisterMsgProcess(true,(pMsgManager));
#define DOS_LEAVE_MSG_MAP(pMsgManager) RegisterMsgProcess(false,(pMsgManager));


//class CDOSMsgCatcher:
//	public CNameObject
//{
//protected:
//	DECLARE_CLASS_INFO(CDOSMsgCatcher);
//public:
//	CDOSMsgCatcher(void);
//	virtual ~CDOSMsgCatcher(void);
//protected:
//	virtual void RegisterMsgProcess(BOOL IsRegister,CDOSMsgManager * pMsgManager);
//};

