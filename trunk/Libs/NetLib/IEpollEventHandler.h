/****************************************************************************/
/*                                                                          */
/*      �ļ���:    IEpollEventHandler.h                                     */
/*      ��������:  2009��07��06��                                           */
/*      ����:      Sagasarate                                               */
/*                                                                          */
/*      �������Ȩ��Sagasarate(sagasarate@sina.com)����                     */
/*      ����Խ�����������κ���ҵ�ͷ���ҵ�����������                      */
/*      ���뱣���˰�Ȩ����                                                  */
/*                                                                          */
/****************************************************************************/
#pragma once

enum EPOLL_EVENT_ID
{
	EPE_PACKET,
	EPE_ERROR,
};
class CEpollEventObject;

class IEpollEventHandler
{
public:	
	virtual BOOL OnEpollEvent(UINT EventID)=0;
};