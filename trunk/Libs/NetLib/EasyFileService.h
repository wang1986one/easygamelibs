/****************************************************************************/
/*                                                                          */
/*      �ļ���:    EasyFileService.h                                        */
/*      ��������:  2009��07��06��                                           */
/*      ����:      Sagasarate                                               */
/*                                                                          */
/*      �������Ȩ��Sagasarate(sagasarate@sina.com)����                     */
/*      ����Խ�����������κ���ҵ�ͷ���ҵ�����������                      */
/*      ���뱣���˰�Ȩ����                                                  */
/*                                                                          */
/****************************************************************************/
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
