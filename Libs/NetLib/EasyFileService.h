#pragma once

class CEasyFileService :
	public CNetService
{
protected:

	DECLARE_CLASS_INFO_STATIC(CEasyFileService);
public:
	CEasyFileService(void);
	virtual ~CEasyFileService(void);


	virtual CBaseTCPConnection * CreateConnection(CIPAddress& RemoteAddress);
	virtual BOOL DeleteConnection(CBaseTCPConnection * pConnection);
};
