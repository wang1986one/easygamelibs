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