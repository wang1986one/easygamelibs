/****************************************************************************/
/*                                                                          */
/*      �ļ���:    MemFileAccessor.h                                        */
/*      ��������:  2011��10��07��                                           */
/*      ����:      Sagasarate                                               */
/*                                                                          */
/*      �������Ȩ��Sagasarate(sagasarate@sina.com)����                     */
/*      ����Խ�����������κ���ҵ�ͷ���ҵ�����������                      */
/*      ���뱣���˰�Ȩ����                                                  */
/*                                                                          */
/****************************************************************************/
/****************************************************************************/
/*                                                                          */
/*      �ļ���:    MemFileAccessor.h                                        */
/*      ��������:  2010��02��09��                                           */
/*      ����:      Sagasarate                                               */
/*                                                                          */
/*      �������Ȩ��Sagasarate(sagasarate@sina.com)����                     */
/*      ����Խ�����������κ���ҵ�ͷ���ҵ�����������                      */
/*      ���뱣���˰�Ȩ����                                                  */
/*                                                                          */
/****************************************************************************/
#pragma once

class CMemFileAccessor :
	public IFileAccessor
{
protected:
	char *			m_Buffer;
	UINT			m_BufferSize;
	UINT			m_DataPtr;
	UINT			m_WriteSize;
public:
	CMemFileAccessor(void);
	~CMemFileAccessor(void);

	virtual BOOL Open(LPCTSTR FileName,int OpenMode);
	virtual void Close();

	virtual ULONG64 GetSize();

	virtual ULONG64 Read(LPVOID pBuff,ULONG64 Size);
	virtual ULONG64 Write(LPCVOID pBuff,ULONG64 Size);


	virtual BOOL IsEOF();

	virtual BOOL Seek(LONG64 Offset,int SeekMode);
	virtual ULONG64 GetCurPos();
	
	UINT GetWriteSize();
	bool AllocBuffer(UINT Size);
	bool CopyToClipBroad(UINT ClipFormat);
	bool CopyFromClipBroad(UINT ClipFormat);


	virtual BOOL SetCreateTime(const CEasyTime& Time);
	virtual BOOL GetCreateTime(CEasyTime& Time);

	virtual BOOL SetLastAccessTime(const CEasyTime& Time);
	virtual BOOL GetLastAccessTime(CEasyTime& Time);

	virtual BOOL SetLastWriteTime(const CEasyTime& Time);
	virtual BOOL GetLastWriteTime(CEasyTime& Time);
};
