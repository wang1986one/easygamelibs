/****************************************************************************/
/*                                                                          */
/*      �ļ���:    DOSMessage.h                                             */
/*      ��������:  2009��10��23��                                           */
/*      ����:      Sagasarate                                               */
/*                                                                          */
/*      �������Ȩ��Sagasarate(sagasarate@sina.com)����                     */
/*      ����Խ�����������κ���ҵ�ͷ���ҵ�����������                      */
/*      ���뱣���˰�Ȩ����                                                  */
/*                                                                          */
/****************************************************************************/
#pragma once

#define MAX_MESSAGE_PACKET_SIZE		4096

typedef DWORD	MSG_ID_TYPE;
typedef DWORD	MSG_LEN_TYPE;
typedef WORD	ID_LIST_COUNT_TYPE;


#pragma pack(push)

#pragma pack(1)

class CDOSSimpleMessage
{
public:
	struct DOS_SIMPLE_MESSAGE_HEAD
	{
		MSG_LEN_TYPE	MsgLen;
		MSG_ID_TYPE		MsgID;	
		WORD			MsgFlag;
	};	
protected:
	DOS_SIMPLE_MESSAGE_HEAD	m_MsgHeader;
	char					m_DataBuffer[4];
public:
	void Init()
	{
		m_MsgHeader.MsgLen=sizeof(DOS_SIMPLE_MESSAGE_HEAD);
		m_MsgHeader.MsgID=0;		
		m_MsgHeader.MsgFlag=0;
	}
	DOS_SIMPLE_MESSAGE_HEAD& GetMsgHeader()
	{
		return m_MsgHeader;
	}
	void SetMsgID(MSG_ID_TYPE CmdID)
	{
		m_MsgHeader.MsgID=CmdID;
	}
	MSG_ID_TYPE GetMsgID() const
	{
		return m_MsgHeader.MsgID;
	}
	void SetDataLength(MSG_LEN_TYPE Length)
	{
		m_MsgHeader.MsgLen=sizeof(DOS_SIMPLE_MESSAGE_HEAD)+Length;
	}
	MSG_LEN_TYPE GetDataLength() const
	{
		return m_MsgHeader.MsgLen-sizeof(DOS_SIMPLE_MESSAGE_HEAD);
	}
	MSG_LEN_TYPE GetMsgLength() const
	{
		return m_MsgHeader.MsgLen;
	}
	void SetMsgFlag(WORD Flag)
	{
		m_MsgHeader.MsgFlag=Flag;
	}
	WORD GetMsgFlag()
	{
		return m_MsgHeader.MsgFlag;
	}
	void * GetDataBuffer()
	{
		return m_DataBuffer;
	}
	CSmartStruct GetDataPacket()
	{
		CSmartStruct DataPacket(m_DataBuffer,GetDataLength(),false);

		return DataPacket;
	}
	void ClearDataPacket()
	{
		CSmartStruct DataPacket(m_DataBuffer,GetDataLength(),true);
	}
	
	static MSG_LEN_TYPE GetMsgHeaderLength()
	{
		return sizeof(DOS_SIMPLE_MESSAGE_HEAD);
	}
	static MSG_LEN_TYPE CaculateMsgLength(MSG_LEN_TYPE DataLength)
	{
		return sizeof(DOS_SIMPLE_MESSAGE_HEAD)+DataLength;
	}
};

class CDOSMessage
{
protected:		
	struct DOS_MESSAGE_HEAD
	{
		union
		{
			struct
			{
				MSG_LEN_TYPE	MsgLen;		
				MSG_ID_TYPE		MsgID;
				WORD			MsgFlag;
				OBJECT_ID_BASE	SenderID;
			};
			struct  
			{
				OBJECT_ID_BASE									Reserve;
				CDOSSimpleMessage::DOS_SIMPLE_MESSAGE_HEAD		SimpleMsgHeader;
			};
		};
	};
	DOS_MESSAGE_HEAD					m_MsgHeader;
	char								m_DataBuffer[4];
public:
	void Init()
	{
		m_MsgHeader.MsgLen=sizeof(DOS_MESSAGE_HEAD);
		m_MsgHeader.MsgID=0;
		m_MsgHeader.MsgFlag=0;
		m_MsgHeader.SenderID.ID=0;
	}
	void SetMsgID(MSG_ID_TYPE CmdID)
	{
		m_MsgHeader.MsgID=CmdID;
	}
	MSG_ID_TYPE GetMsgID() const
	{
		return m_MsgHeader.MsgID;
	}
	void SetDataLength(MSG_LEN_TYPE Length)
	{
		m_MsgHeader.MsgLen=sizeof(DOS_MESSAGE_HEAD)+Length;
	}
	MSG_LEN_TYPE GetDataLength() const
	{
		return m_MsgHeader.MsgLen-sizeof(DOS_MESSAGE_HEAD);
	}
	void SetMsgFlag(WORD Flag)
	{
		m_MsgHeader.MsgFlag=Flag;
	}
	WORD GetMsgFlag()
	{
		return m_MsgHeader.MsgFlag;
	}
	void SetData(LPVOID pData,MSG_LEN_TYPE Length)
	{
		SetDataLength(Length);
		memcpy(m_DataBuffer,pData,Length);
	}
	MSG_LEN_TYPE GetMsgLength() const
	{
		return m_MsgHeader.MsgLen;
	}
	void SetSenderID(OBJECT_ID SenderID)
	{
		m_MsgHeader.SenderID=SenderID;
	}
	OBJECT_ID GetSenderID() const
	{
		return m_MsgHeader.SenderID;
	}
	void * GetDataBuffer()
	{
		return m_DataBuffer;
	}
	CSmartStruct GetDataPacket()
	{
		CSmartStruct DataPacket(m_DataBuffer,GetDataLength(),false);

		return DataPacket;
	}
	CSmartStruct GetEmptyDataPacket()
	{
		CSmartStruct DataPacket(m_DataBuffer,GetDataLength(),true);

		return DataPacket;
	}
	void ClearDataPacket()
	{
		CSmartStruct DataPacket(m_DataBuffer,GetDataLength(),true);
	}
	CDOSSimpleMessage * MakeSimpleMessage()
	{
		m_MsgHeader.SimpleMsgHeader.MsgFlag=m_MsgHeader.MsgFlag;
		m_MsgHeader.SimpleMsgHeader.MsgID=m_MsgHeader.MsgID;
		m_MsgHeader.SimpleMsgHeader.MsgLen=CDOSSimpleMessage::GetMsgHeaderLength()+GetDataLength();		
		return (CDOSSimpleMessage *)(&m_MsgHeader.SimpleMsgHeader);
	}
	static MSG_LEN_TYPE GetMsgHeaderLength()
	{
		return sizeof(DOS_MESSAGE_HEAD);
	}
	static MSG_LEN_TYPE CaculateMsgLength(MSG_LEN_TYPE DataLength)
	{
		return sizeof(DOS_MESSAGE_HEAD)+DataLength;
	}
};

class CDOSMessagePacket
{
protected:
	UINT			m_AllocTime;
	volatile UINT	m_RefCount;
	MSG_LEN_TYPE	m_PacketLen;
	CDOSMessage		m_Message;
public:	
	void Init()
	{
		m_Message.Init();
		m_AllocTime=0;
		m_RefCount=0;
		m_PacketLen=sizeof(MSG_LEN_TYPE)+CDOSMessage::GetMsgHeaderLength()+sizeof(ID_LIST_COUNT_TYPE);
	}
	CDOSMessage& GetMessage()
	{
		return m_Message;
	}	
	void SetAllocTime(UINT Time)
	{
		m_AllocTime=Time;
	}
	UINT GetAllocTime() const
	{
		return m_AllocTime;
	}
	UINT IncRefCount()
	{
		return AtomicInc(&m_RefCount);
	}
	UINT DecRefCount()
	{
		return AtomicDec(&m_RefCount);
	}
	UINT GetRefCount() const
	{
		return m_RefCount;
	}
	void SetPacketLength(MSG_LEN_TYPE PacketLength)
	{
		m_PacketLen=PacketLength;
	}
	MSG_LEN_TYPE GetPacketLength() const
	{
		return m_PacketLen;
	}
	void * GetPacketBuffer()
	{
		return &m_PacketLen;
	}	
	void SetTargetIDs(ID_LIST_COUNT_TYPE TargetCount,OBJECT_ID * pTargetIDs)
	{
		GetTargetIDCount()=TargetCount;
		if(pTargetIDs)
			memcpy(GetTargetIDs(),pTargetIDs,sizeof(OBJECT_ID)*TargetCount);
	}
	ID_LIST_COUNT_TYPE& GetTargetIDCount()
	{
		return *((ID_LIST_COUNT_TYPE *)((BYTE *)GetPacketBuffer()+sizeof(MSG_LEN_TYPE)+m_Message.GetMsgLength()));
	}
	OBJECT_ID * GetTargetIDs()
	{
		return (OBJECT_ID *)((BYTE *)GetPacketBuffer()+sizeof(MSG_LEN_TYPE)+m_Message.GetMsgLength()+sizeof(ID_LIST_COUNT_TYPE));
	}
	void AddTargetID(OBJECT_ID TargetID)
	{
		GetTargetIDs()[GetTargetIDCount()]=TargetID;
		GetTargetIDCount()++;
	}
	void MakePacketLength()
	{
		m_PacketLen=sizeof(MSG_LEN_TYPE)+GetMessage().GetMsgLength()+sizeof(ID_LIST_COUNT_TYPE)+sizeof(OBJECT_ID)*GetTargetIDCount();
	}
	static MSG_LEN_TYPE CaculatePacketLength(MSG_LEN_TYPE DataLength,ID_LIST_COUNT_TYPE TargetIDCount)
	{
		return sizeof(MSG_LEN_TYPE)+CDOSMessage::CaculateMsgLength(DataLength)+sizeof(ID_LIST_COUNT_TYPE)+sizeof(OBJECT_ID)*TargetIDCount;
	}
	static MSG_LEN_TYPE CaculateRealPacketLength(MSG_LEN_TYPE PacketLength)
	{
		return sizeof(UINT)+sizeof(UINT)+PacketLength;
	}
};

#pragma pack(pop)

enum DOS_SYSTEM_MESSAGE
{
	DSM_NONE=0,
	
	DSM_PROXY_REGISTER_MSG_MAP,
	DSM_PROXY_REGISTER_MSG_MAP_RESULT,
	DSM_PROXY_UNREGISTER_MSG_MAP,
	DSM_PROXY_UNREGISTER_MSG_MAP_RESULT,
	DSM_PROXY_REGISTER_GLOBAL_MSG_MAP,
	DSM_PROXY_REGISTER_GLOBAL_MSG_MAP_RESULT,
	DSM_PROXY_UNREGISTER_GLOBAL_MSG_MAP,
	DSM_PROXY_UNREGISTER_GLOBAL_MSG_MAP_RESULT,
	DSM_PROXY_DISCONNECT,
	DSM_PROXY_KEEP_ALIVE,
	DSM_ROUTE_LINK_LOST,
	DSM_OBJECT_ALIVE_TEST,	
	DSM_OBJECT_FIND,
	DSM_OBJECT_REPORT,
	DSM_MAX,
};

enum DOS_MESSAGE_FLAG
{
	DOS_MESSAGE_FLAG_SYSTEM_MESSAGE=1,
	DOS_MESSAGE_FLAG_COMPRESSED=(1<<1),
};