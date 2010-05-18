/****************************************************************************/
/*                                                                          */
/*      文件名:    D3DTextureManager.cpp                                    */
/*      创建日期:  2009年09月11日                                           */
/*      作者:      Sagasarate                                               */
/*                                                                          */
/*      本软件版权归Sagasarate(sagasarate@sina.com)所有                     */
/*      你可以将本软件用于任何商业和非商业软件开发，但                      */
/*      必须保留此版权声明                                                  */
/*                                                                          */
/****************************************************************************/
#include "StdAfx.h"


namespace D3DLib{

IMPLEMENT_CLASS_INFO_STATIC(CD3DTextureManager,CNameObject);

CD3DTextureManager::CD3DTextureManager(CD3DDevice * pD3DDevice,int StorageSize):CNameObject()
{
	m_pD3DDevice=pD3DDevice;
	m_TextureStorage.Create(StorageSize);
}

CD3DTextureManager::~CD3DTextureManager(void)
{
#ifdef _DEBUG
	void * Pos;

	Pos=m_TextureStorage.GetFirstObjectPos();
	while(Pos)
	{
		CD3DTexture * pTexture=*(m_TextureStorage.GetNext(Pos));
		PrintSystemLog(0,"纹理<%s>未释放！",(LPCTSTR)pTexture->GetName());
	}
	m_TextureStorage.Destory();
#endif
}

bool CD3DTextureManager::Reset()
{
	bool Ret=true;
	void * Pos;

	Pos=m_TextureStorage.GetFirstObjectPos();
	while(Pos)
	{		
		CD3DTexture * pTexture=*(m_TextureStorage.GetNext(Pos));
		if(!pTexture->Reset())
			Ret=false;

	}
	return Ret;
}

bool CD3DTextureManager::Restore()
{
	bool Ret=true;
	void * Pos;

	Pos=m_TextureStorage.GetFirstObjectPos();
	while(Pos)
	{		
		CD3DTexture * pTexture=*(m_TextureStorage.GetNext(Pos));
		if(!pTexture->Restore())
			Ret=false;

	}
	return Ret;
}

CD3DTexture * CD3DTextureManager::CreateTexture(LPCTSTR TextureName)
{	
	CD3DTexture * pTexture=new CD3DTexture(this);
	
	if(AddTexture(pTexture,TextureName))
		return pTexture;
	return NULL;
	
}

bool CD3DTextureManager::AddTexture(CD3DTexture * pTexture,LPCTSTR TextureName)
{
	UINT ID=m_TextureStorage.AddObject(pTexture,TextureName);
	if(ID)
	{
		pTexture->SetID(ID);
		pTexture->SetName(TextureName);
		return true;
	}
	else
	{
		return false;
	}
}

bool CD3DTextureManager::DeleteTexture(UINT ID)
{
	
	return m_TextureStorage.DeleteObject(ID);		
		
}

bool CD3DTextureManager::DeleteTexture(LPCTSTR TextureName)
{
	CD3DTexture ** ppTexture=m_TextureStorage.GetObject(TextureName);
	if(ppTexture)
	{
		m_TextureStorage.DeleteObject((*ppTexture)->GetID());		
		return true;
	}
	return false;
}

CD3DTexture * CD3DTextureManager::LoadTexture(LPCTSTR TextureFileName,UINT MipLevels,bool UseFilter,bool IsManaged,D3DCOLOR KeyColor)
{
	CEasyString FileName=CD3DTexture::FindFileOne(TextureFileName);
	CD3DTexture * pTexture=GetTextrue(FileName);
	if(pTexture)
	{
		pTexture->AddUseRef();
		return pTexture;
	}

	pTexture=new CD3DTexture(this);

	if(pTexture->LoadTexture(FileName,MipLevels,UseFilter,IsManaged,KeyColor))
	{
		if(AddTexture(pTexture,FileName))
			return pTexture;
	}	
	delete pTexture;
	return NULL;
}

CD3DIFLTexture * CD3DTextureManager::LoadIFLTexture(LPCTSTR TextureFileName,UINT MipLevels,bool UseFilter,bool IsManaged,D3DCOLOR KeyColor)
{
	CEasyString FileName=CD3DTexture::FindFileOne(TextureFileName);
	CD3DIFLTexture * pTexture=(CD3DIFLTexture *)GetTextrue(FileName);
	if(pTexture)
	{
		pTexture->AddUseRef();
		return pTexture;
	}

	
	pTexture=new CD3DIFLTexture(this);
	if(pTexture->LoadTexture(FileName,MipLevels,UseFilter,IsManaged,KeyColor))
	{
		if(AddTexture(pTexture,FileName))
			return pTexture;
	}
	
	delete pTexture;
	return NULL;
}

CD3DTextTexture * CD3DTextureManager::CreateTextTexture(LPCTSTR TextureName,LOGFONT * pLogFont,int Width,int Height,int MipLevels,D3DCOLOR FontColor)
{
	CD3DTextTexture * pTexture;

	//CD3DTextTexture * pTexture=(CD3DTextTexture *)GetTextrue(TextureName);
	//if(pTexture)
	//{
	//	pTexture->AddUseRef();
	//	return pTexture;
	//}

	pTexture=new CD3DTextTexture(this);

	if(pTexture->Create(pLogFont,Width,Height,MipLevels,FontColor))
	{
		if(AddTexture(pTexture,TextureName))
			return pTexture;
	}
	delete pTexture;
	return NULL;
}

int CD3DTextureManager::GetCount()
{
	return m_TextureStorage.GetObjectCount();
}

LPVOID CD3DTextureManager::GetFirstPos()
{
	return m_TextureStorage.GetFirstObjectPos();
}

LPVOID CD3DTextureManager::GetLastPos()
{
	return m_TextureStorage.GetLastObjectPos();
}

CD3DTexture * CD3DTextureManager::GetNext(LPVOID& Pos)
{
	return *m_TextureStorage.GetNext(Pos);
}

CD3DTexture * CD3DTextureManager::GetPrev(LPVOID& Pos)
{
	return *m_TextureStorage.GetPrev(Pos);
}

}