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