/****************************************************************************/
/*                                                                          */
/*      �ļ���:    MPQFileAccessor.h                                        */
/*      ��������:  2011��10��07��                                           */
/*      ����:      Sagasarate                                               */
/*                                                                          */
/*      �������Ȩ��Sagasarate(sagasarate@sina.com)����                     */
/*      ����Խ�����������κ���ҵ�ͷ���ҵ�����������                      */
/*      ���뱣���˰�Ȩ����                                                  */
/*                                                                          */
/****************************************************************************/
#pragma once

class CMPQFileSystem;

class CMPQFileAccessor :
	public IFileAccessor
{
protected:
	DECLARE_CLASS_INFO_STATIC(CMPQFileAccessor)

	CMPQFileSystem *		m_pManager;
	IFileAccessor *			m_pRealFSFile;
	HANDLE					m_hMPQ;
	HANDLE					m_hFile;

public:
	CMPQFileAccessor(CMPQFileSystem * pManager);
	~CMPQFileAccessor(void);

	virtual BOOL Open(LPCTSTR FileName,int OpenMode);
	virtual void Close();

	virtual ULONG64 GetSize();

	virtual ULONG64 Read(LPVOID pBuff,ULONG64 Size);
	virtual ULONG64 Write(LPCVOID pBuff,ULONG64 Size);


	virtual BOOL IsEOF();

	virtual BOOL Seek(LONG64 Offset,int SeekMode);
	virtual ULONG64 GetCurPos();

	virtual BOOL SetCreateTime(const CEasyTime& Time);
	virtual BOOL GetCreateTime(CEasyTime& Time);

	virtual BOOL SetLastAccessTime(const CEasyTime& Time);
	virtual BOOL GetLastAccessTime(CEasyTime& Time);

	virtual BOOL SetLastWriteTime(const CEasyTime& Time);
	virtual BOOL GetLastWriteTime(CEasyTime& Time);
};
