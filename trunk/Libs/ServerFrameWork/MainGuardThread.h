/****************************************************************************/
/*                                                                          */
/*      �ļ���:    MainGuardThread.h                                        */
/*      ��������:  2009��07��06��                                           */
/*      ����:      Sagasarate                                               */
/*                                                                          */
/*      �������Ȩ��Sagasarate(sagasarate@sina.com)����                     */
/*      ����Խ�����������κ���ҵ�ͷ���ҵ�����������                      */
/*      ���뱣���˰�Ȩ����                                                  */
/*                                                                          */
/****************************************************************************/
#pragma once


class CMainGuardThread :
	public CGuardThread,public CStaticObject<CGuardThread>
{
public:
	CMainGuardThread(void);
	~CMainGuardThread(void);
};
