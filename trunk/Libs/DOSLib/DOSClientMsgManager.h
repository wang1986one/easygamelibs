/****************************************************************************/
/*                                                                          */
/*      �ļ���:    DOSClientMsgManager.h                                    */
/*      ��������:  2009��07��06��                                           */
/*      ����:      Sagasarate                                               */
/*                                                                          */
/*      �������Ȩ��Sagasarate(sagasarate@sina.com)����                     */
/*      ����Խ�����������κ���ҵ�ͷ���ҵ�����������                      */
/*      ���뱣���˰�Ȩ����                                                  */
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
