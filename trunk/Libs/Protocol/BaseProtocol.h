/****************************************************************************/
/*                                                                          */
/*      �ļ���:    BaseProtocol.h                                           */
/*      ��������:  2009��07��06��                                           */
/*      ����:      Sagasarate                                               */
/*                                                                          */
/*      �������Ȩ��Sagasarate(sagasarate@sina.com)����                     */
/*      ����Խ�����������κ���ҵ�ͷ���ҵ�����������                      */
/*      ���뱣���˰�Ȩ����                                                  */
/*                                                                          */
/****************************************************************************/
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


//#define MAX_MSG_SIZE	4096

#pragma pack(pop)