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

