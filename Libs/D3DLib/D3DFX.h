/****************************************************************************/
/*                                                                          */
/*      文件名:    D3DFX.h                                                  */
/*      创建日期:  2009年09月25日                                           */
/*      作者:      Sagasarate                                               */
/*                                                                          */
/*      本软件版权归Sagasarate(sagasarate@sina.com)所有                     */
/*      你可以将本软件用于任何商业和非商业软件开发，但                      */
/*      必须保留此版权声明                                                  */
/*                                                                          */
/****************************************************************************/
#pragma once

namespace D3DLib{

class CD3DFXManager;
class CD3DTexture;

class CD3DFX :
	public CNameObject
{
protected:
	//struct STORAGE_STRUCT:public CNameObject::STORAGE_STRUCT
	//{		
	//	UINT			DataSize;	
	//};
	enum SST_MEMEBER_ID
	{
		SST_D3DFX_EFFECT_DATA=SST_NO_MAX,
		SST_D3DFX_MAX=SST_NO_MAX+50,
	};

	CD3DFXManager *	m_pManager;
	LPD3DXEFFECT	m_pEffect;
	D3DXHANDLE		m_hActiveTech;
	char *			m_pEffectData;
	int				m_EffectDataSize;

	DECLARE_FILE_PATH_MANAGER
	DECLARE_FILE_CHANNEL_MANAGER

	DECLARE_CLASS_INFO(CD3DFX)
	
public:
	CD3DFX();
	CD3DFX(CD3DFXManager * pD3DFXManager);
	virtual ~CD3DFX(void);
	void SetManager(CD3DFXManager * pD3DFXManager);
	CD3DFXManager * GetManager();

	virtual void Destory();
	virtual bool Reset();
	virtual bool Restore();

	LPCTSTR GetFXContent();
	
	//virtual void Release();

	bool LoadFromFile(LPCTSTR FileName);
	bool LoadFromFileDirect(LPCTSTR FileName);
	bool LoadFromMemory(const void * pData,int DataSize);
	bool LoadFXDirect(const void * pData,int DataSize);

	bool SetActiveTechnique(int Index);
	bool SetActiveTechnique(LPCTSTR TecName);

	bool UseActiveTechnique();

	inline LPD3DXEFFECT GetEffect()
	{
		return m_pEffect;
	}

	int Begin();
	bool End();

	bool BeginPass(int Pass);
	bool EndPass(int Pass);

	bool SetTexture(LPCTSTR ParamName,CD3DTexture * pTexture);
	bool SetMatrix(LPCTSTR ParamName,const CD3DMatrix& Mat);
	bool SetMatrixArray(LPCTSTR ParamName,const CD3DMatrix * pMats,int Count);
	bool SetVector(LPCTSTR ParamName,const CD3DVector2& Vec);
	bool SetVector(LPCTSTR ParamName,const CD3DVector3& Vec);
	bool SetVector(LPCTSTR ParamName,const CD3DVector4& Vec);
	bool SetQuaternion(LPCTSTR ParamName,const CD3DQuaternion& Quat);
	bool SetColor(LPCTSTR ParamName,const D3DCOLORVALUE& Color);
	bool SetInt(LPCTSTR ParamName,int Value);
	bool SetIntArray(LPCTSTR ParamName,const int * pValues,int Count);
	bool SetFloat(LPCTSTR ParamName,FLOAT Value);
	bool SetBool(LPCTSTR ParamName,BOOL Value);

	bool GetInt(LPCTSTR ParamName,int& Value);
	bool GetFloat(LPCTSTR ParamName,FLOAT& Value);
	bool GetBool(LPCTSTR ParamName,BOOL& Value);	


	//virtual void AddUseRef()
	//{
	//	AtomicInc(&m_UseRef);
	//}
	//virtual void Release()
	//{
	//	DecUseRef();
	//	if(GetUseRef()<=0)
	//		delete this;
	//}

	virtual bool ToSmartStruct(CSmartStruct& Packet,CUSOFile * pUSOFile,UINT Param=0);
	virtual bool FromSmartStruct(CSmartStruct& Packet,CUSOFile * pUSOFile,UINT Param=0);
	virtual UINT GetSmartStructSize(UINT Param=0);

protected:
	//virtual CNameObject::STORAGE_STRUCT * USOCreateHead(UINT Param=0);
	//virtual int USOWriteHead(CNameObject::STORAGE_STRUCT * pHead,CUSOFile * pUSOFile,UINT Param=0);
	//virtual bool USOWriteData(CNameObject::STORAGE_STRUCT * pHead,CUSOFile * pUSOFile,UINT Param=0);

	//virtual int USOReadHead(CNameObject::STORAGE_STRUCT * pHead,CUSOFile * pUSOFile,UINT Param=0);
	//virtual int USOReadData(CNameObject::STORAGE_STRUCT * pHead,CUSOFile * pUSOFile,BYTE * pData,int DataSize,UINT Param=0);
	//virtual bool USOReadFinish(CNameObject::STORAGE_STRUCT * pHead,UINT Param=0);

	
	
};

inline void CD3DFX::SetManager(CD3DFXManager * pD3DFXManager)
{
	m_pManager=pD3DFXManager;
}

inline CD3DFXManager * CD3DFX::GetManager()
{
	return m_pManager;
}

inline LPCTSTR CD3DFX::GetFXContent()
{
	return m_pEffectData;
}



}