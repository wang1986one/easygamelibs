#pragma once

namespace D3DLib{

class CD3DObjectResourceManager :
	public CNameObject
{
protected:
	CD3DDevice *							m_pD3DDevice;
	CNameStorage<CD3DObjectResource *>	m_ObjectStorage;

	DECLARE_CLASS_INFO_STATIC(CD3DObjectResourceManager)
public:
	CD3DObjectResourceManager(CD3DDevice * pD3DDevice,int StorageSize);
	virtual ~CD3DObjectResourceManager(void);

	bool AddResource(CD3DObjectResource * pResource,LPCTSTR ResourceName);

	CD3DObjectResource * DeleteResource(UINT ID);
	CD3DObjectResource * DeleteResource(LPCTSTR ResourceName);


	CD3DObjectResource * GetResource(UINT ID);

	CD3DObjectResource * GetResource(LPCTSTR ResourceName);

	CD3DDevice * GetDevice();

	int GetCount();
	LPVOID GetFirstPos();
	LPVOID GetLastPos();
	CD3DObjectResource * GetNext(LPVOID& Pos);
	CD3DObjectResource * GetPrev(LPVOID& Pos);
};

inline CD3DObjectResource * CD3DObjectResourceManager::GetResource(UINT ID)
{
	CD3DObjectResource ** ppResource=m_ObjectStorage.GetObject(ID);
	if(ppResource)
		return *ppResource;
	else
		return NULL;
}

inline CD3DObjectResource * CD3DObjectResourceManager::GetResource(LPCTSTR ResourceName)
{
	CD3DObjectResource ** ppResource=m_ObjectStorage.GetObject(ResourceName);
	if(ppResource)
		return *ppResource;
	else
		return NULL;
}

inline CD3DDevice * CD3DObjectResourceManager::GetDevice()
{
	return m_pD3DDevice;
}

}