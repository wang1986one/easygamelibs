#include "StdAfx.h"


namespace D3DLib{

IMPLEMENT_FILE_CHANNEL_MANAGER(CBLZDBCFile)

CBLZDBCFile::CBLZDBCFile(void)
{
	m_pData=NULL;
	m_DataSize=0;
	m_IsSelfRelease=false;

}

CBLZDBCFile::~CBLZDBCFile(void)
{
	Close();
}

bool CBLZDBCFile::Load(LPCTSTR FileName,UINT RecordSize)
{
	IFileAccessor * pFile;

	pFile=CreateFileAccessor();
	if(pFile==NULL)
		return false;
	if(!pFile->Open(FileName,IFileAccessor::modeRead))
	{
		pFile->Release();
		return false;	
	}

	bool Ret=Load(pFile,RecordSize);
	pFile->Release();
	return Ret;
}

bool CBLZDBCFile::Load(IFileAccessor * pFile,UINT RecordSize)
{
	UINT FileSize=(UINT)pFile->GetSize();	
	BYTE * pData=new BYTE[FileSize];
	pFile->Read(pData,FileSize);
	return Load(pData,FileSize,RecordSize,true);
}

bool CBLZDBCFile::Load(BYTE * pData,UINT DataSize,UINT RecordSize,bool IsSelfRelease)
{
	Close();
	m_pData=pData;
	m_DataSize=DataSize;
	m_IsSelfRelease=IsSelfRelease;

	BLZ_DBC_HEADER * pHeader=(BLZ_DBC_HEADER *)m_pData;
	if(pHeader->Tag!=BLZ_DBC_HEADER_TAG)
		return false;

	if(RecordSize)
	{
		if(pHeader->RecordSize!=RecordSize)
			return false;
	}

	return true;
}

void CBLZDBCFile::Close()
{
	if(m_IsSelfRelease)
	{
		SAFE_DELETE_ARRAY(m_pData);
	}
	m_pData=NULL;
	m_DataSize=0;
	m_IsSelfRelease=false;
}

UINT CBLZDBCFile::GetRecordSize()
{
	BLZ_DBC_HEADER * pHeader=(BLZ_DBC_HEADER *)m_pData;
	return pHeader->RecordSize;
}

UINT CBLZDBCFile::GetRecordCount()
{
	BLZ_DBC_HEADER * pHeader=(BLZ_DBC_HEADER *)m_pData;
	return pHeader->RecordCount;
}
UINT CBLZDBCFile::GetFieldCount()
{
	BLZ_DBC_HEADER * pHeader=(BLZ_DBC_HEADER *)m_pData;
	return pHeader->FieldCount;
}
int CBLZDBCFile::GetDataInt(UINT Record,UINT Field)
{
	BLZ_DBC_HEADER * pHeader=(BLZ_DBC_HEADER *)m_pData;
	if(Record<pHeader->RecordCount&&Field<pHeader->FieldCount)
	{
		int * pRecord=(int *)(m_pData+sizeof(BLZ_DBC_HEADER)+pHeader->RecordSize*Record);
		return pRecord[Field];
	}
	return 0;
}
UINT CBLZDBCFile::GetDataUint(UINT Record,UINT Field)
{
	BLZ_DBC_HEADER * pHeader=(BLZ_DBC_HEADER *)m_pData;
	if(Record<pHeader->RecordCount&&Field<pHeader->FieldCount)
	{
		UINT * pRecord=(UINT *)(m_pData+sizeof(BLZ_DBC_HEADER)+pHeader->RecordSize*Record);
		return pRecord[Field];
	}
	return 0;
}
float CBLZDBCFile::GetDataFloat(UINT Record,UINT Field)
{
	BLZ_DBC_HEADER * pHeader=(BLZ_DBC_HEADER *)m_pData;
	if(Record<pHeader->RecordCount&&Field<pHeader->FieldCount)
	{
		float * pRecord=(float *)(m_pData+sizeof(BLZ_DBC_HEADER)+pHeader->RecordSize*Record);
		return pRecord[Field];
	}
	return 0;
}
LPCSTR CBLZDBCFile::GetDataString(UINT Record,UINT Field)
{
	BLZ_DBC_HEADER * pHeader=(BLZ_DBC_HEADER *)m_pData;
	if(Record<pHeader->RecordCount&&Field<pHeader->FieldCount)
	{
		UINT * pRecord=(UINT *)(m_pData+sizeof(BLZ_DBC_HEADER)+pHeader->RecordSize*Record);
		char * pStringTable=(char *)(m_pData+sizeof(BLZ_DBC_HEADER)+pHeader->RecordSize*pHeader->RecordCount);
		return pStringTable+pRecord[Field];
	}
	return NULL;
}
CEasyString CBLZDBCFile::GetDataLocalString(UINT Record,UINT Field)
{
	BLZ_DBC_HEADER * pHeader=(BLZ_DBC_HEADER *)m_pData;
	if(Record<pHeader->RecordCount&&Field<pHeader->FieldCount)
	{
		UINT * pRecord=(UINT *)(m_pData+sizeof(BLZ_DBC_HEADER)+pHeader->RecordSize*Record);
		char * pStringTable=(char *)(m_pData+sizeof(BLZ_DBC_HEADER)+pHeader->RecordSize*pHeader->RecordCount);
		char * pString=pStringTable+pRecord[Field];

		WCHAR	Buffer[1024];

		int len=MultiByteToWideChar(CP_UTF8,0,pString,strlen(pString),Buffer,1024);
		Buffer[len]=0;
		return Buffer;
	}
	return "";
}

}