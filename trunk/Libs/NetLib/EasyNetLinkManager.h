/****************************************************************************/
/*                                                                          */
/*      �ļ���:    EasyNetLinkManager.h                                     */
/*      ��������:  2009��07��06��                                           */
/*      ����:      Sagasarate                                               */
/*                                                                          */
/*      �������Ȩ��Sagasarate(sagasarate@sina.com)����                     */
/*      ����Խ�����������κ���ҵ�ͷ���ҵ�����������                      */
/*      ���뱣���˰�Ȩ����                                                  */
/*                                                                          */
/****************************************************************************/
#pragma once
#include <map>
#include <vector>


class CEasyNetLinkManager :
	public CNameObject
{
protected:
	CNetServer *								m_pServer;

	std::map<UINT,CEasyNetLinkConnection*>		m_ConnectionMap;
	std::vector<CEasyNetLinkConnection*>		m_ConnectionList;	
	

	std::map<UINT,CEasyNetLinkService*>			m_ServiceMap;
	std::vector<CEasyNetLinkService*>			m_ServiceList;

	CIDStorage<int>								m_ConnectionIDPool;
	

	DECLARE_CLASS_INFO_STATIC(CEasyNetLinkManager);
public:
	CEasyNetLinkManager(void);
	virtual ~CEasyNetLinkManager(void);

	BOOL Init(CNetServer * pServer,LPCTSTR ConfigFileName);
	BOOL Init(CNetServer * pServer,xml_node& Config);
	BOOL Init(CNetServer * pServer);
	virtual void Destory();

	CEasyNetLinkService * AddService(UINT ID,UINT ReportID,const CIPAddress& ListenAddress,
		BOOL NeedReallocConnectionID,UINT MaxPacketSize=0,bool IsUseListenThread=true,
		int ParallelAcceptCount=DEFAULT_PARALLEL_ACCEPT,UINT AcceptQueueSize=DEFAULT_SERVER_ACCEPT_QUEUE,
		UINT RecvQueueSize=DEFAULT_SERVER_RECV_DATA_QUEUE,UINT SendQueueSize=DEFAULT_SERVER_SEND_DATA_QUEUE);
	BOOL AddConnection(UINT ReportID,const CIPAddress& ConnectionAddress,UINT MaxPacketSize=0,
		UINT RecvQueueSize=DEFAULT_SERVER_RECV_DATA_QUEUE,UINT SendQueueSize=DEFAULT_SERVER_SEND_DATA_QUEUE);

	virtual int Update(int ProcessPacketLimit=DEFAULT_SERVER_PROCESS_PACKET_LIMIT);

	virtual CEasyNetLinkConnection * OnCreateConnection(UINT ID);
	virtual CEasyNetLinkService * OnCreateService(UINT ID);

	
	CEasyNetLinkConnection * GetConnection(UINT ID);
	CEasyNetLinkConnection * GetFirstConnection();
	CEasyNetLinkService * GetService(UINT ID);

	UINT GetConnectionCount();
	CEasyNetLinkConnection * GetConnectionByIndex(UINT Index);


	virtual CEasyNetLinkConnection * CreateAcceptConnection(UINT ReportID);
	virtual CEasyNetLinkConnection * CreateConnection(UINT ID);
	virtual BOOL AcceptConnection(CEasyNetLinkConnection * pConnection);
	virtual BOOL DeleteConnection(CEasyNetLinkConnection * pConnection);

	virtual CEasyNetLinkService * CreateLinkService(UINT ID);
	virtual BOOL DeleteLinkService(CEasyNetLinkService * pService);

	virtual void OnLinkStart(CEasyNetLinkConnection * pConnection);
	virtual void OnLinkEnd(CEasyNetLinkConnection * pConnection);


	void PrintLinkInfo(UINT LogChannel);
};
