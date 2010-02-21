#pragma once
#include "ifileaccessor.h"

class CStandardFileAccessor :
	public IFileAccessor
{
protected:
	FILE *	m_hFile;
	bool	m_IsWriteFlush;

	DECLARE_CLASS_INFO_STATIC(CStandardFileAccessor)
public:
	CStandardFileAccessor(void);
	virtual ~CStandardFileAccessor(void);

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
