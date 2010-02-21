#pragma once

namespace D3DLib{

class CD3DFXManager :
	public CNameObject
{
protected:
	CD3DDevice *					m_pD3DDevice;
	CNameStorage<CD3DFX *>		m_FXStorage;

	DECLARE_CLASS_INFO_STATIC(CD3DFXManager)
public:
	CD3DFXManager(CD3DDevice * pD3DDevice,int StorageSize);
	virtual ~CD3DFXManager(void);

	bool Reset();
	bool Restore();		

	CD3DDevice * GetDevice();

	CD3DFX * GetFX(UINT ID);

	CD3DFX * GetFX(LPCTSTR FXName);

	bool AddFX(CD3DFX * pFX,LPCTSTR Name);

	bool DeleteFX(UINT ID);
	bool DeleteFX(LPCTSTR TextureName);

	CD3DFX * LoadFX(LPCTSTR FileName);
	CD3DFX * LoadFXFromMemory(LPCTSTR Name,const void * pData,int DataSize);

	int GetCount();
	LPVOID GetFirstPos();
	LPVOID GetLastPos();
	CD3DFX * GetNext(LPVOID& Pos);
	CD3DFX * GetPrev(LPVOID& Pos);
};

inline CD3DDevice * CD3DFXManager::GetDevice()
{
	return m_pD3DDevice;
}

inline CD3DFX * CD3DFXManager::GetFX(UINT ID)
{
	CD3DFX ** ppFX=m_FXStorage.GetObject(ID);
	if(ppFX)
		return *ppFX;
	else
		return NULL;
}

inline CD3DFX * CD3DFXManager::GetFX(LPCTSTR FXName)
{
	CD3DFX ** ppFX=m_FXStorage.GetObject(FXName);
	if(ppFX)
		return *ppFX;
	else
		return NULL;
}

}