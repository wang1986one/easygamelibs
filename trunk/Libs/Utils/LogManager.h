/****************************************************************************/
/*                                                                          */
/*      �ļ���:    LogManager.h                                             */
/*      ��������:  2009��10��23��                                           */
/*      ����:      Sagasarate                                               */
/*                                                                          */
/*      �������Ȩ��Sagasarate(sagasarate@sina.com)����                     */
/*      ����Խ�����������κ���ҵ�ͷ���ҵ�����������                      */
/*      ���뱣���˰�Ȩ����                                                  */
/*                                                                          */
/****************************************************************************/
#pragma once



#define LOG_SYSTEM_CHANNEL		1010100
#define LOG_IMPORTANT_CHANNEL	1010101

#define IMPORTANT_LOG_FILE_NAME	_T("ImportantError")

extern TCHAR LOG_MANAGER_INSTANCE[];

class CLogManager :
	public CNameObject,
	public CStaticObject2<CLogManager,LOG_MANAGER_INSTANCE>
{
protected:
	CEasyMap<UINT,ILogPrinter *>	m_LogChannels;

	DECLARE_CLASS_INFO_STATIC(CLogManager);
public:
	CLogManager(void);
	virtual ~CLogManager(void);

	UINT GetChannelCount();
	void AddChannel(UINT ChannelID,ILogPrinter * pLogPrinter);
	ILogPrinter * GetChannel(UINT ChannelID);
	BOOL DelChannel(UINT ChannelID);

	BOOL PrintLog(UINT ChannelID,int Level,DWORD Color,LPCTSTR Format,...);
	BOOL PrintLogVL(UINT ChannelID,int Level,DWORD Color,LPCTSTR Format,va_list vl);
};
