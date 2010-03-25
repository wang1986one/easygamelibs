/****************************************************************************/
/*                                                                          */
/*      文件名:    USOFile.h                                                */
/*      创建日期:  2009年09月25日                                           */
/*      作者:      Sagasarate                                               */
/*                                                                          */
/*      本软件版权归Sagasarate(sagasarate@sina.com)所有                     */
/*      你可以将本软件用于任何商业和非商业软件开发，但                      */
/*      必须保留此版权声明                                                  */
/*                                                                          */
/****************************************************************************/
#pragma once
#include <vector>
#include <map>
#include <string>



#define USO_FILE_FLAG				'OSU'
#define USO_FILE_CUR_VERSION		0x01000000


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


class CUSOFile :
	public CNameObject
{
protected:
	
	vector<CUSOObjectCreateFilter*>		m_ObjectCreateFilterList;


	CNameObjectSet						m_Resources;


	std::vector<CNameObject *>			m_Objects;	
	IFileAccessor *						m_pFile;
	
	std::map<std::string,CLASS_INFO *>	m_ObjectCreateInfoByName;
	std::map<std::string,CLASS_INFO *>	m_ObjectCreateInfoByType;

	DECLARE_FILE_PATH_MANAGER;
	DECLARE_FILE_CHANNEL_MANAGER;

	DECLARE_CLASS_INFO(CUSOFile)

	
public:
	CUSOFile();	
	virtual ~CUSOFile(void);	
	virtual void  Destory();

	void AddObjectCreateFilter(CUSOObjectCreateFilter* pObjectCreateFilter);

	bool Open(LPCTSTR USOFileName);
	bool Open(IFileAccessor * pFile);	

	bool Save(LPCTSTR USOFileName);
	bool Save(IFileAccessor * pFile);

	IFileAccessor * GetFile();

	int ResourceObjectToIndex(CNameObject * pResource);
	CNameObject * ResourceIndexToObject(UINT Index,CLASS_INFO& ClassInfo);
	bool AddResource(CNameObject * pResource);
	bool AddResourceFront(CNameObject * pResource);
	CNameObject * GetResource(LPCTSTR ResourceName);
	int GetResourceCount();
	CNameObject * GetResourceByIndex(int Index);
	bool DeleteResource(CNameObject * pResource);
	void ClearAllResource();

	void AddObject(CNameObject * pObject);
	CNameObject * GetObject(LPCTSTR ObjectName);
	CNameObject * GetObjectRecursive(LPCTSTR ObjectName);
	int GetObjectCount();
	CNameObject * GetObjectByIndex(int Index);
	bool DeleteObject(CNameObject * pObject);

	void PickResource(CNameObject * pObject);

//	bool GetObjectHead(CNameObject::STORAGE_STRUCT& Head);

	CNameObject * CreateObject(LPCTSTR TypeName,LPCTSTR ObjectName,bool ReferenceWhenExist=false);

	void AddObjectCreateInfoByName(LPCTSTR ObjectName,CLASS_INFO * pClassInfo);
	void AddObjectCreateInfoByType(LPCTSTR ObjectType,CLASS_INFO * pClassInfo);

protected:	
	
	bool ReadResource();
	bool ReadObject();
	bool SaveResource();
	bool SaveObject();	
	bool ReadResourceBlock(USO_BLOCK_HEAD& BlockHead);
	bool ReadObjectBlock(USO_BLOCK_HEAD& BlockHead);

	
};

inline void CUSOFile::AddObjectCreateFilter(CUSOObjectCreateFilter* pObjectCreateFilter)
{
	m_ObjectCreateFilterList.push_back(pObjectCreateFilter);
}
