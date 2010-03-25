/****************************************************************************/
/*                                                                          */
/*      文件名:    D3DFXManager.cpp                                         */
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



IMPLEMENT_CLASS_INFO_STATIC(CD3DFXManager,CNameObject);

CD3DFXManager::CD3DFXManager(CD3DDevice * pD3DDevice,int StorageSize):CNameObject()
{
	m_pD3DDevice=pD3DDevice;
	m_FXStorage.Create(StorageSize);
}

CD3DFXManager::~CD3DFXManager(void)
{
#ifdef _DEBUG
	void * Pos;

	Pos=m_FXStorage.GetFirstObjectPos();
	while(Pos)
	{
		char Msg[256];
		CD3DFX * pFx=*(m_FXStorage.GetNext(Pos));
		sprintf_s(Msg,256,"FX<%s>未释放！\r\n",(LPCTSTR)pFx->GetName());
		OutputDebugString(Msg);
	}
	m_FXStorage.Destory();
#endif
}

bool CD3DFXManager::Reset()
{
	void * Pos;

	Pos=m_FXStorage.GetFirstObjectPos();
	while(Pos)
	{		
		CD3DFX * pFx=*(m_FXStorage.GetNext(Pos));
		pFx->Reset();
	}
	return true;
}

bool CD3DFXManager::Restore()
{
	void * Pos;

	Pos=m_FXStorage.GetFirstObjectPos();
	while(Pos)
	{		
		CD3DFX * pFx=*(m_FXStorage.GetNext(Pos));
		pFx->Restore();
	}
	return true;
}

bool CD3DFXManager::AddFX(CD3DFX * pFX,LPCTSTR Name)
{
	UINT ID=m_FXStorage.AddObject(pFX,Name);
	if(ID)
	{
		pFX->SetID(ID);
		pFX->SetName(Name);
		return true;
	}
	else
	{
		return false;
	}
}

bool CD3DFXManager::DeleteFX(UINT ID)
{
	
	return m_FXStorage.DeleteObject(ID);		
		
}

bool CD3DFXManager::DeleteFX(LPCTSTR TextureName)
{
	CD3DFX ** ppFX=m_FXStorage.GetObject(TextureName);
	if(ppFX)
	{
		m_FXStorage.DeleteObject((*ppFX)->GetID());		
		return true;
	}
	return false;
}

CD3DFX * CD3DFXManager::LoadFX(LPCTSTR FileName)
{
	CEasyString FXFileName=CD3DFX::FindFileOne(FileName);

	CD3DFX * pFX=GetFX(FXFileName);
	if(pFX)
	{
		pFX->AddUseRef();
		return pFX;
	}

	pFX=new CD3DFX(this);

#ifdef D3D_DEBUG_INFO
	if(pFX->LoadFromFileDirect(FXFileName))
	{
		if(AddFX(pFX,FXFileName))
			return pFX;
	}
#else
	if(pFX->LoadFromFile(FXFileName))
	{
		if(AddFX(pFX,FXFileName))
			return pFX;
	}
#endif
	delete pFX;
	return NULL;
}

CD3DFX * CD3DFXManager::LoadFXFromMemory(LPCTSTR Name,const void * pData,int DataSize)
{	
	CD3DFX * pFX=GetFX(Name);
	if(pFX)
	{
		pFX->AddUseRef();
		return pFX;
	}

	pFX=new CD3DFX(this);

	if(pFX->LoadFromMemory(pData,DataSize))
	{
		if(AddFX(pFX,Name))
			return pFX;
	}
	delete pFX;
	return NULL;
}

int CD3DFXManager::GetCount()
{
	return m_FXStorage.GetObjectCount();
}

LPVOID CD3DFXManager::GetFirstPos()
{
	return m_FXStorage.GetFirstObjectPos();
}

LPVOID CD3DFXManager::GetLastPos()
{
	return m_FXStorage.GetLastObjectPos();
}

CD3DFX * CD3DFXManager::GetNext(LPVOID& Pos)
{
	return *m_FXStorage.GetNext(Pos);
}

CD3DFX * CD3DFXManager::GetPrev(LPVOID& Pos)
{
	return *m_FXStorage.GetPrev(Pos);
}

}