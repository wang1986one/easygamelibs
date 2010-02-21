#pragma once

#pragma pack(push)
#pragma pack(4)

struct SMSG_HEADER			//������Ϣͷ
{
	DWORD Size;				//��Ϣ����С
	DWORD MsgID;				//��ϢID
};

struct SMSG					//������Ϣ�ṹ
{
	SMSG_HEADER	Header;
	char		Data[1];
};

enum SERVER_TYPE
{
	SRV_DBPROXY=1,					//���ݿ���������
	SRV_STATUS,						//״̬������
	SRV_STATUS_DISPATCH,			//״̬������ƽ�⸺�ط�����
	SRV_AUTH,						//��֤������
	SRV_LOBBY,						//����������
	SRV_ROOM,						//���������
	SRV_CHANNEL,					//Ƶ��������	
};


#define MAX_MSG_SIZE	4096

#pragma pack(pop)