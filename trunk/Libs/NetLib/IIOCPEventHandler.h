/****************************************************************************/
/*                                                                          */
/*      �ļ���:    IIOCPEventHandler.h                                      */
/*      ��������:  2009��07��06��                                           */
/*      ����:      Sagasarate                                               */
/*                                                                          */
/*      �������Ȩ��Sagasarate(sagasarate@sina.com)����                     */
/*      ����Խ�����������κ���ҵ�ͷ���ҵ�����������                      */
/*      ���뱣���˰�Ȩ����                                                  */
/*                                                                          */
/****************************************************************************/
#pragma once

enum IOCP_EVENT_ID
{
	IOE_PACKET,
	IOE_ERROR,
};
class COverLappedObject;

class IIOCPEventHandler
{
public:	
	virtual BOOL OnIOCPEvent(int EventID,COverLappedObject * pOverLappedObject)=0;
};