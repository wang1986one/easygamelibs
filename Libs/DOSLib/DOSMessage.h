#pragma once

#define MAX_MESSAGE_PACKET_SIZE		4096

#pragma pack(push)

#pragma pack(1)

class CDOSSimpleMessage
{
public:
	struct DOS_SIMPLE_MESSAGE_HEAD
	{
		WORD		MsgLen;
		WORD		CmdID;	
	};	
protected:
	DOS_SIMPLE_MESSAGE_HEAD	m_MsgHeader;
	char					m_DataBuffer[4];
public:
	void Init()
	{
		m_MsgHeader.MsgLen=sizeof(DOS_SIMPLE_MESSAGE_HEAD);
		m_MsgHeader.CmdID=0;		
	}
	void SetCmdID(WORD CmdID)
	{
		m_MsgHeader.CmdID=CmdID;
	}
	WORD GetCmdID() const
	{
		return m_MsgHeader.CmdID;
	}
	void SetDataLength(WORD Length)
	{
		m_MsgHeader.MsgLen=sizeof(DOS_SIMPLE_MESSAGE_HEAD)+Length;
	}
	WORD GetDataLength() const
	{
		return m_MsgHeader.MsgLen-sizeof(DOS_SIMPLE_MESSAGE_HEAD);
	}
	WORD GetMsgLength() const
	{
		return m_MsgHeader.MsgLen;
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
	
	static WORD GetMsgHeaderLength()
	{
		return sizeof(DOS_SIMPLE_MESSAGE_HEAD);
	}
	static WORD CaculateMsgLength(WORD DataLength)
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
				WORD		MsgLen;		
				WORD		CmdID;
				OBJECT_ID	SenderID;
			};
			struct  
			{
				OBJECT_ID										Reserve;
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
		m_MsgHeader.CmdID=0;
		m_MsgHeader.SenderID.ID=0;
	}
	void SetCmdID(WORD CmdID)
	{
		m_MsgHeader.CmdID=CmdID;
	}
	WORD GetCmdID() const
	{
		return m_MsgHeader.CmdID;
	}
	void SetDataLength(WORD Length)
	{
		m_MsgHeader.MsgLen=sizeof(DOS_MESSAGE_HEAD)+Length;
	}
	WORD GetDataLength() const
	{
		return m_MsgHeader.MsgLen-sizeof(DOS_MESSAGE_HEAD);
	}
	WORD GetMsgLength() const
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
		m_MsgHeader.SimpleMsgHeader.MsgLen=CDOSSimpleMessage::GetMsgHeaderLength()+GetDataLength();
		m_MsgHeader.SimpleMsgHeader.CmdID=m_MsgHeader.CmdID;
		return (CDOSSimpleMessage *)(&m_MsgHeader.SimpleMsgHeader);
	}
	static WORD GetMsgHeaderLength()
	{
		return sizeof(DOS_MESSAGE_HEAD);
	}
	static WORD CaculateMsgLength(WORD DataLength)
	{
		return sizeof(DOS_MESSAGE_HEAD)+DataLength;
	}
};

class CDOSMessagePacket
{
protected:
	UINT			m_AllocTime;
	volatile UINT	m_RefCount;
	WORD			m_PacketLen;
	CDOSMessage		m_Message;
public:	
	void Init()
	{
		m_Message.Init();
		m_AllocTime=0;
		m_RefCount=0;
		m_PacketLen=sizeof(WORD)+CDOSMessage::GetMsgHeaderLength()+sizeof(WORD);
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
	void SetPacketLength(WORD PacketLength)
	{
		m_PacketLen=PacketLength;
	}
	WORD GetPacketLength() const
	{
		return m_PacketLen;
	}
	void * GetPacketBuffer()
	{
		return &m_PacketLen;
	}	
	void SetTargetIDs(WORD TargetCount,OBJECT_ID * pTargetIDs)
	{
		GetTargetIDCount()=TargetCount;
		if(pTargetIDs)
			memcpy(GetTargetIDs(),pTargetIDs,sizeof(OBJECT_ID)*TargetCount);
	}
	WORD& GetTargetIDCount()
	{
		return *((WORD *)((BYTE *)GetPacketBuffer()+sizeof(WORD)+m_Message.GetMsgLength()));
	}
	OBJECT_ID * GetTargetIDs()
	{
		return (OBJECT_ID *)((BYTE *)GetPacketBuffer()+sizeof(WORD)+m_Message.GetMsgLength()+sizeof(WORD));
	}
	void AddTargetID(OBJECT_ID TargetID)
	{
		GetTargetIDs()[GetTargetIDCount()]=TargetID;
		GetTargetIDCount()++;
	}
	void MakePacketLength()
	{
		m_PacketLen=sizeof(WORD)+GetMessage().GetMsgLength()+sizeof(WORD)+sizeof(OBJECT_ID)*GetTargetIDCount();
	}
	static WORD CaculatePacketLength(WORD DataLength,WORD TargetIDCount)
	{
		return sizeof(WORD)+CDOSMessage::CaculateMsgLength(DataLength)+sizeof(WORD)+sizeof(OBJECT_ID)*TargetIDCount;
	}
	static WORD CaculateRealPacketLength(WORD PacketLength)
	{
		return sizeof(UINT)+sizeof(UINT)+PacketLength;
	}
};

#pragma pack(pop)

//struct DOS_MESSAGE_HEAD
//{
//	WORD		MsgLen;		
//	WORD		CmdID;
//	OBJECT_ID	SenderID;
//};
//
//struct DOS_MESSAGE
//{
//	DOS_MESSAGE_HEAD	Header;
//	char				Datas[1];
//};
//
//struct DOS_MESSAGE_PACKET
//{
//	WORD		PacketLen;
//	WORD		RefCount;
//	DOS_MESSAGE Msg;
//};
//
//struct DOS_MESSAGE_PACKET_HEAD
//{
//	WORD				PacketLen;
//	WORD				RefCount;
//	DOS_MESSAGE_HEAD	Header;
//};
//
//struct DOS_SIMPLE_MESSAGE_HEAD
//{
//	WORD		MsgLen;
//	WORD		CmdID;	
//};
//
//struct DOS_SIMPLE_MESSAGE
//{
//	DOS_SIMPLE_MESSAGE_HEAD	Header;
//	char					Datas[1];
//};

enum DOS_SYSTEM_MESSAGE
{
	DSM_NONE=0,
	//DSM_ROUTER_REGISTER,
	//注册路由，通知自己的路由ID
	//DSM_PROXY_OBJECT_REGISTER,
	//DSM_PROXY_OBJECT_UNREGISTER,
	//DSM_PROXY_OBJECT_CHANGE_LINK,
	DSM_PROXY_REGISTER_MSG_MAP,
	DSM_PROXY_UNREGISTER_MSG_MAP,
	DSM_PROXY_REGISTER_GLOBAL_MSG_MAP,
	DSM_PROXY_UNREGISTER_GLOBAL_MSG_MAP,
	DSM_PROXY_DISCONNECT,
	//OBJECT_ID NewLinkObjectID;
	//DSM_PROXY_OBJECT_CLOSE,
	DSM_MAX,
};

