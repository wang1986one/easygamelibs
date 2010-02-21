#include "StdAfx.h"

namespace D3DLib{

IMPLEMENT_FILE_PATH_MANAGER(CD3DFX)
IMPLEMENT_FILE_CHANNEL_MANAGER(CD3DFX)

IMPLEMENT_CLASS_INFO(CD3DFX,CNameObject);

CD3DFX::CD3DFX():CNameObject()
{
	m_pManager=NULL;
	m_pEffect=NULL;
	m_hActiveTech=NULL;
	m_pEffectData=NULL;
	m_EffectDataSize=0;
}

CD3DFX::CD3DFX(CD3DFXManager * pD3DFXManager):CNameObject()
{
	m_pManager=pD3DFXManager;
	m_pEffect=NULL;
	m_hActiveTech=NULL;
	m_pEffectData=NULL;
	m_EffectDataSize=0;
}

CD3DFX::~CD3DFX(void)
{
	Destory();
	if(m_pManager)
		m_pManager->DeleteFX(GetID());
}

void CD3DFX::Destory()
{
	SAFE_RELEASE(m_pEffect);
	SAFE_DELETE_ARRAY(m_pEffectData);
	m_hActiveTech=NULL;
	m_EffectDataSize=0;	
	CNameObject::Destory();
}

bool CD3DFX::Reset()
{
	SAFE_RELEASE(m_pEffect);
	return true;
}

bool CD3DFX::Restore()
{
	if(m_pEffectData)
		return LoadFXDirect(m_pEffectData,m_EffectDataSize);
	return true;
}

//void CD3DFX::Release()
//{
//	DecUseRef();
//	if(GetUseRef()<=0)
//	{		
//		delete this;
//	}
//}

bool CD3DFX::LoadFromFile(LPCTSTR FileName)
{
	IFileAccessor * pFile;


	CEasyString FxFileName=FindFileOne(FileName);
	pFile=CFileSystemManager::GetInstance()->CreateFileAccessor(0);
	if(pFile==NULL)
		return false;

#ifdef _DEBUG
	char Msg[1024];
	sprintf_s(Msg,1024,"装载FX<%s>.....\r\n",(LPCTSTR)FxFileName);
	OutputDebugString(Msg);
#endif
	if(pFile->Open(FileName,IFileAccessor::modeRead))
	{
		int Size=(int)pFile->GetSize();

		SAFE_DELETE_ARRAY(m_pEffectData);
		m_pEffectData=new char[Size+1];
		m_EffectDataSize=Size;

		pFile->Read(m_pEffectData,Size);
		m_pEffectData[Size]=0;
		
		pFile->Release();
		
		if(LoadFXDirect(m_pEffectData,m_EffectDataSize))
			return true;
	}
	else
		pFile->Release();
#ifdef _DEBUG	
	sprintf_s(Msg,1024,"装载FX<%s>失败\r\n",FileName);
	OutputDebugString(Msg);
#endif
	return false;
}

bool CD3DFX::LoadFromFileDirect(LPCTSTR FileName)
{
	HRESULT hr;
	LPD3DXBUFFER pErrors=NULL;

	Reset();

	CEasyString FxFileName=FindFileOne(FileName);
	hr=D3DXCreateEffectFromFile(
		m_pManager->GetDevice()->GetD3DDevice(),
		FxFileName,
		NULL,
		NULL,
#ifdef D3D_DEBUG_INFO
		D3DXSHADER_DEBUG|D3DXSHADER_FORCE_VS_SOFTWARE_NOOPT,
#else
		D3DXSHADER_SKIPVALIDATION,
#endif
		NULL,
		&m_pEffect,
		&pErrors);
	if(m_pEffect==NULL)
	{
		CEasyString ErrorMsg;
		if(pErrors)
			ErrorMsg.Format("无法加载FX,Err=(%s)",(char*)(pErrors->GetBufferPointer()));
		else
			ErrorMsg.Format("无法加载FX");
		MessageBox(GetForegroundWindow(),ErrorMsg,"无法加载FX",MB_OK);
		SAFE_RELEASE(pErrors);
		return false;
	}
	return true;
}

bool CD3DFX::LoadFromMemory(const void * pData,int DataSize)
{	
	SAFE_DELETE_ARRAY(m_pEffectData);
	m_pEffectData=new char[DataSize+1];
	m_EffectDataSize=DataSize;
	
	memcpy(m_pEffectData,pData,DataSize);
	m_pEffectData[DataSize]=0;
	
	return LoadFXDirect(pData,DataSize);
}


bool CD3DFX::SetActiveTechnique(int Index)
{	
	if(m_pEffect)
	{
		D3DXHANDLE  hTech = m_pEffect->GetTechnique(Index);
		if(hTech == NULL)
			return false;
		if(FAILED(m_pEffect->ValidateTechnique(hTech)))
			return false;

		m_hActiveTech = hTech;
	}
	else
		return false;

	
	return true;
}

bool CD3DFX::SetActiveTechnique(LPCTSTR TecName)
{
	if(m_pEffect)
	{
		D3DXHANDLE  hTech = m_pEffect->GetTechniqueByName(TecName);
		if(hTech == NULL)
			return false;
		if(FAILED(m_pEffect->ValidateTechnique(hTech)))
			return false;		

		m_hActiveTech = hTech;
	}
	else
		return false;
	return true;
}

bool CD3DFX::UseActiveTechnique()
{
	if(m_hActiveTech)
	{
		if(FAILED(m_pEffect->SetTechnique(m_hActiveTech)))
			return false;
	}
	return false;
}

int CD3DFX::Begin()
{
	UINT Pass;
	if(SUCCEEDED(m_pEffect->Begin(&Pass,D3DXFX_DONOTSAVESTATE)))
	{
		return (int)Pass;
	}
	return -1;
}

bool CD3DFX::End()
{
	if(SUCCEEDED(m_pEffect->End()))
		return true;
	else
		return false;
}

bool CD3DFX::BeginPass(int Pass)
{
	if(SUCCEEDED(m_pEffect->BeginPass(Pass)))
		return true;
	else
		return false;
}

bool CD3DFX::EndPass(int Pass)
{
	if(SUCCEEDED(m_pEffect->EndPass()))
		return true;
	else
		return false;
}


bool CD3DFX::SetTexture(LPCTSTR ParamName,CD3DTexture * pTexture)
{	
	HRESULT	hr;

	if(pTexture==NULL)
		return false;

	

	if(pTexture->GetD3DTexture())
	{
		hr=m_pEffect->SetTexture(ParamName,pTexture->GetD3DTexture());
		if(SUCCEEDED(hr))
		{
			return true;
		} 
	}
	return false;
}

bool CD3DFX::SetMatrix(LPCTSTR ParamName,const CD3DMatrix& Mat)
{	
	HRESULT	hr;
	
	hr=m_pEffect->SetMatrix(ParamName,&Mat);
	if(SUCCEEDED(hr))
	{
		return true;
	} 
	
	return false;
}

bool CD3DFX::SetMatrixArray(LPCTSTR ParamName,const CD3DMatrix * pMats,int Count)
{	
	HRESULT	hr;

	if(pMats==NULL)
		return false;

	
	hr=m_pEffect->SetMatrixArray(ParamName,pMats,Count);
	if(SUCCEEDED(hr))
	{
		return true;
	} 
	
	return false;
}

bool CD3DFX::SetVector(LPCTSTR ParamName,const CD3DVector2& Vec)
{
	HRESULT	hr;
	hr=m_pEffect->SetFloatArray(ParamName,(CONST FLOAT *)Vec,2);
	if(SUCCEEDED(hr))
	{
		return true;
	} 
	return false;
}

bool CD3DFX::SetVector(LPCTSTR ParamName,const CD3DVector3& Vec)
{
	HRESULT	hr;
	hr=m_pEffect->SetFloatArray(ParamName,(CONST FLOAT *)Vec,3);
	if(SUCCEEDED(hr))
	{
		return true;
	} 
	return false;
}

bool CD3DFX::SetVector(LPCTSTR ParamName,const CD3DVector4& Vec)
{
	HRESULT	hr;
	hr=m_pEffect->SetVector(ParamName,&Vec);
	if(SUCCEEDED(hr))
	{
		return true;
	} 
	return false;
}

bool CD3DFX::SetQuaternion(LPCTSTR ParamName,const CD3DQuaternion& Quat)
{
	HRESULT	hr;
	hr=m_pEffect->SetFloatArray(ParamName,(CONST FLOAT *)Quat,4);
	if(SUCCEEDED(hr))
	{
		return true;
	} 
	return false;
}

bool CD3DFX::SetColor(LPCTSTR ParamName,const D3DCOLORVALUE& Color)
{
	HRESULT	hr;
	hr=m_pEffect->SetFloatArray(ParamName,(CONST FLOAT *)&Color,4);
	if(SUCCEEDED(hr))
	{
		return true;
	} 
	return false;
}

bool CD3DFX::SetInt(LPCTSTR ParamName,int Value)
{
	HRESULT	hr;
	hr=m_pEffect->SetInt(ParamName,Value);
	if(SUCCEEDED(hr))
	{
		return true;
	} 
	return false;
}

bool CD3DFX::SetIntArray(LPCTSTR ParamName,const int * pValues,int Count)
{
	HRESULT	hr;
	hr=m_pEffect->SetIntArray(ParamName,pValues,Count);
	if(SUCCEEDED(hr))
	{
		return true;
	} 
	return false;
}

bool CD3DFX::SetFloat(LPCTSTR ParamName,FLOAT Value)
{
	HRESULT	hr;
	hr=m_pEffect->SetFloat(ParamName,Value);
	if(SUCCEEDED(hr))
	{
		return true;
	} 
	return false;
}

bool CD3DFX::SetBool(LPCTSTR ParamName,BOOL Value)
{
	HRESULT	hr;
	hr=m_pEffect->SetBool(ParamName,Value);
	if(SUCCEEDED(hr))
	{
		return true;
	} 
	return false;
}

bool CD3DFX::GetInt(LPCTSTR ParamName,int& Value)
{
	HRESULT	hr;
	hr=m_pEffect->GetInt(ParamName,&Value);
	if(SUCCEEDED(hr))
	{
		return true;
	} 
	return false;
}

bool CD3DFX::GetFloat(LPCTSTR ParamName,FLOAT& Value)
{
	HRESULT	hr;
	hr=m_pEffect->GetFloat(ParamName,&Value);
	if(SUCCEEDED(hr))
	{
		return true;
	} 
	return false;
}

bool CD3DFX::GetBool(LPCTSTR ParamName,BOOL& Value)
{
	HRESULT	hr;
	hr=m_pEffect->GetBool(ParamName,&Value);
	if(SUCCEEDED(hr))
	{
		return true;
	} 
	return false;
}

bool CD3DFX::LoadFXDirect(const void * pData,int DataSize)
{
	HRESULT hr;
	LPD3DXBUFFER pErrors=NULL;

	Reset();

	hr=D3DXCreateEffect(
		m_pManager->GetDevice()->GetD3DDevice(),
		pData,
		DataSize,
		NULL,
		NULL,
#ifdef D3D_DEBUG_INFO
		D3DXSHADER_DEBUG|D3DXSHADER_SKIPOPTIMIZATION,		
#else
		D3DXSHADER_SKIPVALIDATION,
#endif
		NULL,
		&m_pEffect,
		&pErrors);
	if(m_pEffect==NULL)
	{
		CEasyString ErrorMsg;
		if(pErrors)
			ErrorMsg.Format("无法加载FX,Err=(%s)",(char*)(pErrors->GetBufferPointer()));
		else
			ErrorMsg.Format("无法加载FX");
		MessageBox(GetForegroundWindow(),ErrorMsg,"无法加载FX",MB_OK);
		SAFE_RELEASE(pErrors);
		return false;
	}

	return true;
}



//bool CD3DFX::ToUSOFile(CUSOFile * pUSOFile,UINT Param)
//{	
//
//	if(pUSOFile==NULL)
//		return false;	
//
//	IFileAccessor * pFile=pUSOFile->GetFile();
//	if(pFile==NULL)
//		return false;
//	
//
//	STORAGE_STRUCT Data;
//
//	strncpy_0(Data.ObjectHead.Type,USO_FILE_MAX_TYPE_LEN,GetClassInfo().ClassName,USO_FILE_MAX_TYPE_LEN);		
//	strncpy_0(Data.ObjectHead.Name,USO_FILE_MAX_OBJECT_NAME,GetName(),USO_FILE_MAX_OBJECT_NAME);
//	Data.ObjectHead.Size=sizeof(STORAGE_STRUCT)+(UINT)m_EffectDataSize;
//
//	
//	Data.DataSize=(UINT)m_EffectDataSize;
//
//	pFile->Write(&Data,sizeof(STORAGE_STRUCT));
//	pFile->Write(m_pEffectData,Data.DataSize);
//
//
//	return true;
//}
//
//bool CD3DFX::FromUSOFile(CUSOFile * pUSOFile,UINT Param)
//{
//	if(pUSOFile==NULL)
//		return false;	
//
//	IFileAccessor * pFile=pUSOFile->GetFile();
//	if(pFile==NULL)
//		return false;
//
//	STORAGE_STRUCT * pData;
//	BYTE * pBuff;
//	UINT Size;
//
//	pFile->Read(&Size,sizeof(UINT));
//	pBuff=new BYTE[Size];
//	pFile->Read(pBuff+sizeof(UINT),Size-sizeof(UINT));
//	pData=(STORAGE_STRUCT *)pBuff;
//	pData->ObjectHead.Size=Size;
//	
//
//	if((!GetClassInfo().IsKindOf(pData->ObjectHead.Type))||
//		pData->ObjectHead.Size<sizeof(STORAGE_STRUCT))
//	{	
//		delete[] pBuff;
//		return false;
//	}
//	pData->ObjectHead.Name[USO_FILE_MAX_OBJECT_NAME-1]=0;
//	SetName(pData->ObjectHead.Name);
//	if(m_pManager)
//		m_pManager->AddFX(this,GetName());
//
//	bool ret=LoadFromMemory(pBuff+sizeof(STORAGE_STRUCT),pData->DataSize);
//	
//	delete[] pBuff;
//	return ret;
//
//}

bool CD3DFX::ToSmartStruct(CSmartStruct& Packet,CUSOFile * pUSOFile,UINT Param)
{
	if(!CNameObject::ToSmartStruct(Packet,pUSOFile,Param))
		return false;	
	if(m_pEffectData)
	{
		CHECK_SMART_STRUCT_ADD_AND_RETURN(Packet.AddMember(SST_D3DFX_EFFECT_DATA,m_pEffectData,m_EffectDataSize));
	}
	
	return true;
}
bool CD3DFX::FromSmartStruct(CSmartStruct& Packet,CUSOFile * pUSOFile,UINT Param)
{
	if(!CNameObject::FromSmartStruct(Packet,pUSOFile,Param))
		return false;

	CSmartValue EffectData=Packet.GetMember(SST_D3DFX_EFFECT_DATA);

	if(!LoadFromMemory((LPCTSTR)EffectData,EffectData.GetLength()))
		return false;

	if(m_pManager)
		m_pManager->AddFX(this,GetName());

	return true;
}

UINT CD3DFX::GetSmartStructSize(UINT Param)
{
	UINT Size=CNameObject::GetSmartStructSize(Param);	
	Size+=SMART_STRUCT_STRING_MEMBER_SIZE(m_EffectDataSize);
	return Size;
}

//CNameObject::STORAGE_STRUCT * CD3DFX::USOCreateHead(UINT Param)
//{
//	STORAGE_STRUCT * pHead=new STORAGE_STRUCT;
//	ZeroMemory(pHead,sizeof(STORAGE_STRUCT));
//	pHead->Size=sizeof(STORAGE_STRUCT);
//	return pHead;
//}
//
//int CD3DFX::USOWriteHead(CNameObject::STORAGE_STRUCT * pHead,CUSOFile * pUSOFile,UINT Param)
//{
//	int HeadSize=CNameObject::USOWriteHead(pHead,pUSOFile,Param);
//	if(HeadSize<0)
//		return -1;
//
//	STORAGE_STRUCT * pLocalHead=(STORAGE_STRUCT *)pHead;
//
//	pLocalHead->Size+=(UINT)m_EffectDataSize;
//	pLocalHead->DataSize=(UINT)m_EffectDataSize;
//
//	return sizeof(STORAGE_STRUCT);
//}
//
//bool CD3DFX::USOWriteData(CNameObject::STORAGE_STRUCT * pHead,CUSOFile * pUSOFile,UINT Param)
//{
//	if(!CNameObject::USOWriteData(pHead,pUSOFile,Param))
//		return false;
//
//	if(pUSOFile==NULL)
//		return false;	
//
//	IFileAccessor * pFile=pUSOFile->GetFile();
//	if(pFile==NULL)
//		return false;
//
//	if(m_EffectDataSize)
//	{
//		if(pFile->Write(m_pEffectData,m_EffectDataSize)<m_EffectDataSize)
//			return false;
//	}
//	return true;
//}
//
//int CD3DFX::USOReadHead(CNameObject::STORAGE_STRUCT * pHead,CUSOFile * pUSOFile,UINT Param)
//{	
//	int ReadSize=CNameObject::USOReadHead(pHead,pUSOFile,Param);
//	if(ReadSize<0)
//		return -1;	
//
//	return sizeof(STORAGE_STRUCT);
//}
//
//int CD3DFX::USOReadData(CNameObject::STORAGE_STRUCT * pHead,CUSOFile * pUSOFile,BYTE * pData,int DataSize,UINT Param)
//{
//	int ReadSize=CNameObject::USOReadData(pHead,pUSOFile,pData,DataSize,Param);
//
//	pData+=ReadSize;
//	DataSize-=ReadSize;
//
//	STORAGE_STRUCT * pLocalHead=(STORAGE_STRUCT *)pHead;
//
//	if(pLocalHead->DataSize)
//	{
//		if(!LoadFromMemory(pData,pLocalHead->DataSize))
//			return -1;
//
//		ReadSize+=pLocalHead->DataSize;
//	}
//	return ReadSize;
//}
//
//bool CD3DFX::USOReadFinish(CNameObject::STORAGE_STRUCT * pHead,UINT Param)
//{
//	if(!CNameObject::USOReadFinish(pHead,Param))
//		return false;
//
//	if(m_pManager)
//		m_pManager->AddFX(this,GetName());
//	return true;
//}

}