/****************************************************************************/
/*                                                                          */
/*      �ļ���:    D3DFX.cpp                                                */
/*      ��������:  2009��09��25��                                           */
/*      ����:      Sagasarate                                               */
/*                                                                          */
/*      �������Ȩ��Sagasarate(sagasarate@sina.com)����                     */
/*      ����Խ�����������κ���ҵ�ͷ���ҵ�����������                      */
/*      ���뱣���˰�Ȩ����                                                  */
/*                                                                          */
/****************************************************************************/
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
}

CD3DFX::CD3DFX(CD3DFXManager * pD3DFXManager):CNameObject()
{
	m_pManager=pD3DFXManager;
	m_pEffect=NULL;
	m_hActiveTech=NULL;
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
	m_hActiveTech=NULL;
	CNameObject::Destory();
}

bool CD3DFX::Reset()
{
	SAFE_RELEASE(m_pEffect);
	return true;
}

bool CD3DFX::Restore()
{
	if(m_CompiledEffectData.GetUsedSize())
		return LoadFXDirect(m_CompiledEffectData.GetBuffer(),m_CompiledEffectData.GetUsedSize());
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

	PrintD3DDebugLog(0,"װ��FX<%s>.....",(LPCTSTR)FxFileName);
	if(pFile->Open(FileName,IFileAccessor::modeRead))
	{
		UINT64 Size=pFile->GetSize();

		m_EffectData.Create(Size+1);		

		Size=pFile->Read(m_EffectData.GetBuffer(),Size);
		m_EffectData.SetUsedSize(Size);
		m_EffectData.PushConstBack(0,1);
		
		pFile->Release();
		
		if(LoadFromMemory(NULL,0))
			return true;
	}
	else
		pFile->Release();
	PrintD3DLog(0,"װ��FX<%s>ʧ��",FileName);
	return false;
}

#ifdef D3D_DEBUG_INFO

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
		D3DXSHADER_DEBUG|D3DXSHADER_FORCE_VS_SOFTWARE_NOOPT,
		NULL,
		&m_pEffect,
		&pErrors);
	if(m_pEffect==NULL)
	{
		CEasyString ErrorMsg;
		if(pErrors)
			ErrorMsg.Format("�޷�����FX,Err=(%s)",(char*)(pErrors->GetBufferPointer()));
		else
			ErrorMsg.Format("�޷�����FX");
		MessageBox(GetForegroundWindow(),ErrorMsg,"�޷�����FX",MB_OK);
		SAFE_RELEASE(pErrors);
		return false;
	}

	m_hActiveTech=m_pEffect->GetTechnique(0);

	return true;
}

#endif

bool CD3DFX::LoadFromMemory(const void * pData,int DataSize)
{	
	if(pData)
	{
		m_EffectData.Create(DataSize+1);
		m_EffectData.PushBack(pData,DataSize);
		m_EffectData.PushConstBack(0,1);
	}


	LPD3DXEFFECTCOMPILER pCompiler=NULL;
	LPD3DXBUFFER pErrors=NULL;
	if(D3DXCreateEffectCompiler((LPCTSTR)m_EffectData.GetBuffer(),m_EffectData.GetUsedSize(),
		NULL,NULL,
		0,
		&pCompiler,
		&pErrors)==D3D_OK)
	{
		LPD3DXBUFFER pCompiledData;
		if(pCompiler->CompileEffect(0,&pCompiledData,&pErrors)==D3D_OK)
		{
			m_CompiledEffectData.Create(pCompiledData->GetBufferSize());
			m_CompiledEffectData.PushBack(pCompiledData->GetBufferPointer(),
				pCompiledData->GetBufferSize());
			SAFE_RELEASE(pCompiledData);

			return LoadFXDirect(m_CompiledEffectData.GetBuffer(),m_CompiledEffectData.GetUsedSize());
		}
		else
		{
			if(pErrors)
				PrintD3DLog(0,"�޷�����FX,Err=(%s)",(char*)(pErrors->GetBufferPointer()));
			else
				PrintD3DLog(0,"�޷�����FX");
			SAFE_RELEASE(pErrors);
		}
		SAFE_RELEASE(pCompiler);
		return false;
	}
	else
	{
		if(pErrors)
			PrintD3DLog(0,"�޷�����FX������,Err=(%s)",(char*)(pErrors->GetBufferPointer()));
		else
			PrintD3DLog(0,"�޷�����FX����");
		SAFE_RELEASE(pErrors);
		return false;
	}
	
	
}

bool CD3DFX::LoadCompiledFromMemory(const void * pData,int DataSize,const void * pSrcData,int SrcDataSize)
{
	m_CompiledEffectData.Create(DataSize);
	m_CompiledEffectData.PushBack(pData,DataSize);

	m_EffectData.Create(SrcDataSize+1);
	m_EffectData.PushBack(pSrcData,SrcDataSize);
	m_EffectData.PushConstBack(0,1);


	return LoadFXDirect(m_CompiledEffectData.GetBuffer(),m_CompiledEffectData.GetUsedSize());
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

bool CD3DFX::SetActiveTechnique(LPCTSTR TechName)
{
	if(m_pEffect)
	{
		D3DXHANDLE  hTech = m_pEffect->GetTechniqueByName(TechName);
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

bool CD3DFX::UseTechnique(LPCTSTR TechName)
{
	if(m_pEffect)
	{
		D3DXHANDLE  hTech = m_pEffect->GetTechniqueByName(TechName);
		if(hTech == NULL)
			return false;
		if(FAILED(m_pEffect->ValidateTechnique(hTech)))
			return false;		

		if(FAILED(m_pEffect->SetTechnique(hTech)))
			return false;
	}
	else
		return false;
	return true;
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

bool CD3DFX::SetColor(LPCTSTR ParamName,D3DCOLOR Color)
{
	D3DCOLORVALUE ColorValue;
	ColorValue.a=((FLOAT)((Color>>24)&0xFF))/255.0f;
	ColorValue.r=((FLOAT)((Color>>16)&0xFF))/255.0f;
	ColorValue.g=((FLOAT)((Color>>8)&0xFF))/255.0f;
	ColorValue.b=((FLOAT)((Color)&0xFF))/255.0f;
	return SetColor(ParamName,ColorValue);
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
		if(pErrors)
			PrintD3DLog(0,"�޷�����FX,Err=(%s)",(char*)(pErrors->GetBufferPointer()));
		else
			PrintD3DLog(0,"�޷�����FX");
		SAFE_RELEASE(pErrors);
		return false;
	}

	m_hActiveTech=m_pEffect->GetTechnique(0);

	return true;
}



bool CD3DFX::ToSmartStruct(CSmartStruct& Packet,CUSOFile * pUSOFile,UINT Param)
{
	if(!CNameObject::ToSmartStruct(Packet,pUSOFile,Param))
		return false;	
	if(m_EffectData.GetUsedSize())
	{
		CHECK_SMART_STRUCT_ADD_AND_RETURN(Packet.AddMember(SST_D3DFX_EFFECT_DATA,(LPCTSTR)m_EffectData.GetBuffer(),m_EffectData.GetUsedSize()));
	}

	if(m_CompiledEffectData.GetUsedSize())
	{
		CHECK_SMART_STRUCT_ADD_AND_RETURN(Packet.AddMember(SST_D3DFX_COMPILED_EFFECT_DATA,(LPCTSTR)m_CompiledEffectData.GetBuffer(),m_CompiledEffectData.GetUsedSize()));
	}
	
	return true;
}
bool CD3DFX::FromSmartStruct(CSmartStruct& Packet,CUSOFile * pUSOFile,UINT Param)
{
	if(!CNameObject::FromSmartStruct(Packet,pUSOFile,Param))
		return false;

	CSmartValue EffectData=Packet.GetMember(SST_D3DFX_EFFECT_DATA);
	CSmartValue CompiledEffectData=Packet.GetMember(SST_D3DFX_COMPILED_EFFECT_DATA);

	if(CompiledEffectData.GetLength())
	{
		if(!LoadCompiledFromMemory((LPCTSTR)CompiledEffectData,CompiledEffectData.GetLength(),
			(LPCTSTR)EffectData,EffectData.GetLength()))
			return false;
	}
	else
	{
		if(!LoadFromMemory((LPCTSTR)EffectData,EffectData.GetLength()))
			return false;
	}

	if(m_pManager)
		m_pManager->AddFX(this,GetName());

	return true;
}

UINT CD3DFX::GetSmartStructSize(UINT Param)
{
	UINT Size=CNameObject::GetSmartStructSize(Param);	
	Size+=SMART_STRUCT_STRING_MEMBER_SIZE(m_EffectData.GetUsedSize());
	Size+=SMART_STRUCT_STRING_MEMBER_SIZE(m_CompiledEffectData.GetUsedSize());
	return Size;
}



}