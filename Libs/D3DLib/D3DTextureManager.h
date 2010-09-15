/****************************************************************************/
/*                                                                          */
/*      文件名:    D3DTextureManager.h                                      */
/*      创建日期:  2009年09月11日                                           */
/*      作者:      Sagasarate                                               */
/*                                                                          */
/*      本软件版权归Sagasarate(sagasarate@sina.com)所有                     */
/*      你可以将本软件用于任何商业和非商业软件开发，但                      */
/*      必须保留此版权声明                                                  */
/*                                                                          */
/****************************************************************************/
#pragma once


namespace D3DLib{

class CD3DDevice;

class CD3DTextureManager :
	public CNameObject
{
protected:
	CD3DDevice *					m_pD3DDevice;
	CNameStorage<CD3DTexture *>		m_TextureStorage;
	CEasyArray<CD3DTexture *>		m_AniTextureList;

	DECLARE_CLASS_INFO_STATIC(CD3DTextureManager)
public:
	CD3DTextureManager(CD3DDevice * pD3DDevice,int StorageSize);
	virtual ~CD3DTextureManager(void);

	virtual bool Reset();
	virtual bool Restore();	

	CD3DTexture * CreateTexture(LPCTSTR TextureName);

	bool AddTexture(CD3DTexture * pTexture,LPCTSTR TextureName);

	bool DeleteTexture(UINT ID);
	bool DeleteTexture(LPCTSTR TextureName);


	CD3DTexture * GetTextrue(UINT ID);

	CD3DTexture * GetTextrue(LPCTSTR TextureName);

	CD3DTexture * LoadTexture(LPCTSTR TextureFileName,UINT MipLevels=1,bool UseFilter=true,bool IsManaged=true,D3DCOLOR KeyColor=0);
	CD3DIFLTexture * LoadIFLTexture(LPCTSTR TextureFileName,UINT MipLevels=1,bool UseFilter=true,bool IsManaged=true,D3DCOLOR KeyColor=0);
	CD3DTextTexture * CreateTextTexture(LPCTSTR TextureName,LOGFONT * pLogFont,int Width,int Height,int MipLevels,D3DCOLOR FontColor);

	CD3DDevice * GetDevice();

	int GetCount();
	LPVOID GetFirstPos();
	LPVOID GetLastPos();
	CD3DTexture * GetNext(LPVOID& Pos);
	CD3DTexture * GetPrev(LPVOID& Pos);

	void Update(FLOAT Time);

protected:
	void AddAniTexture(CD3DTexture * pTexture);
	void DelAniTexture(CD3DTexture * pTexture);
};

inline CD3DTexture * CD3DTextureManager::GetTextrue(UINT ID)
{
	CD3DTexture ** ppTexture=m_TextureStorage.GetObject(ID);
	if(ppTexture)
		return *ppTexture;
	else
		return NULL;
}

inline CD3DTexture * CD3DTextureManager::GetTextrue(LPCTSTR TextureName)
{
	CD3DTexture ** ppTexture=m_TextureStorage.GetObject(TextureName);
	if(ppTexture)
		return *ppTexture;
	else
		return NULL;
}

inline CD3DDevice * CD3DTextureManager::GetDevice()
{
	return m_pD3DDevice;
}



}