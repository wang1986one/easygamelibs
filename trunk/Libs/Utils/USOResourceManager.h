/****************************************************************************/
/*                                                                          */
/*      �ļ���:    USOResourceManager.h                                     */
/*      ��������:  2009��09��25��                                           */
/*      ����:      Sagasarate                                               */
/*                                                                          */
/*      �������Ȩ��Sagasarate(sagasarate@sina.com)����                     */
/*      ����Խ�����������κ���ҵ�ͷ���ҵ�����������                      */
/*      ���뱣���˰�Ȩ����                                                  */
/*                                                                          */
/****************************************************************************/
#pragma once




#define USO_FILE_FLAG				'OSU'
#define USO_FILE_CUR_VERSION		0x01001000


struct USO_FILE_HEAD
{
	DWORD	Flag;
	DWORD	Version;	
};

struct USO_BLOCK_HEAD
{
	UINT	Size;		
	UINT	Type;	
};

enum USO_BLOCK_TYPE
{
	USO_BLOCK_RESOURCE='SER',	
	USO_BLOCK_OBJECT='JBO',	
};


class CUSOResourceManager :
	public CNameObject
{
protected:
	
	CEasyArray<CUSOObjectCreateFilter*>		m_ObjectCreateFilterList;


	CEasyMap<CEasyString,CNameObject *>	m_Resources;


	CEasyArray<CNameObject *>			m_Objects;		
	
	CEasyMap<CEasyString,CLASS_INFO *>	m_ObjectCreateInfoByName;
	CEasyMap<CEasyString,CLASS_INFO *>	m_ObjectCreateInfoByType;

	DECLARE_FILE_PATH_MANAGER;
	DECLARE_FILE_CHANNEL_MANAGER;

	DECLARE_CLASS_INFO(CUSOResourceManager)

	
public:
	CUSOResourceManager();	
	virtual ~CUSOResourceManager(void);	
	virtual void  Destory();

	void AddObjectCreateFilter(CUSOObjectCreateFilter* pObjectCreateFilter);

	bool Import(LPCTSTR USOFileName);
	bool Import(IFileAccessor * pFile);
	bool ImportFromDir(LPCTSTR Dir);

	bool Export(LPCTSTR USOFileName);
	bool Export(IFileAccessor * pFile);
	bool ExportToDir(LPCTSTR Dir);
	

	CNameObject * FindResource(LPCTSTR szResourceName,CLASS_INFO& ClassInfo);
	LPCTSTR FindResourceKey(CNameObject * pResource);
	bool AddResource(CNameObject * pResource);
	
	int GetResourceCount();
	LPVOID GetFirstResourcePos();
	CNameObject * GetNextResource(LPVOID& Pos);
	
	bool DeleteResource(LPCTSTR szResourceName);
	void ClearAllResource();

	void AddObject(CNameObject * pObject);
	CNameObject * GetObject(LPCTSTR ObjectName);
	CNameObject * GetObjectRecursive(LPCTSTR ObjectName);
	UINT GetObjectCount();
	CNameObject * GetObjectByIndex(UINT Index);
	bool DeleteObject(CNameObject * pObject);
	void ClearAllObject();

	void PickResource(CNameObject * pObject);

//	bool GetObjectHead(CNameObject::STORAGE_STRUCT& Head);

	CNameObject * CreateObject(LPCTSTR TypeName,LPCTSTR ObjectName,bool ReferenceWhenExist=false);

	void AddObjectCreateInfoByName(LPCTSTR ObjectName,CLASS_INFO * pClassInfo);
	void ClearObjectCreateInfoByName();
	void AddObjectCreateInfoByType(LPCTSTR ObjectType,CLASS_INFO * pClassInfo);
	void ClearObjectCreateInfoByType();

protected:		
	
	bool ReadResourceBlock(IFileAccessor * pFile,USO_BLOCK_HEAD& BlockHead);
	bool ReadObjectBlock(IFileAccessor * pFile,USO_BLOCK_HEAD& BlockHead);

	bool WriteResourceBlock(IFileAccessor * pFile,CNameObject * pObject);
	bool WriteObjectBlock(IFileAccessor * pFile,CNameObject * pObject);

	
};

inline void CUSOResourceManager::AddObjectCreateFilter(CUSOObjectCreateFilter* pObjectCreateFilter)
{
	m_ObjectCreateFilterList.Add(pObjectCreateFilter);
}
