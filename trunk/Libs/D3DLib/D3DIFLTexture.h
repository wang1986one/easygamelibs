/****************************************************************************/
/*                                                                          */
/*      文件名:    D3DIFLTexture.h                                          */
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

class CD3DIFLTexture :
	public CD3DTexture
{
protected:
	struct TEXTURE_FRAME
	{
		CD3DTexture *	pTexture;
		float			FrameTime;
	};

	//struct STORAGE_STRUCT:public CNameObject::STORAGE_STRUCT
	//{		
	//	UINT			FrameCount;
	//	
	//};
	enum SST_MEMBER_ID
	{
		SST_D3DITEX_FRAME=SST_NO_MAX,		
		SST_D3DITEX_MAX=SST_NO_MAX+50,
	};
	enum SST_FRAME_INFO
	{
		SST_FI_TIME=1,
		SST_FI_TEXTURE,
	};

	//struct STORAGE_FRAME_STRUCT
	//{
	//	float FrameTime;
	//	int	TextureIndex;
	//};

	CEasyArray<TEXTURE_FRAME>	m_TextrueList;
	float						m_FlipStartTime;
	int							m_CurFrame;

	DECLARE_CLASS_INFO(CD3DIFLTexture)
public:
	CD3DIFLTexture();
	CD3DIFLTexture(CD3DTextureManager * pManager);
	virtual ~CD3DIFLTexture(void);

	virtual bool LoadTexture(LPCTSTR TextureFileName,UINT MipLevels=1,bool UseFilter=true,bool IsManaged=true,D3DCOLOR KeyColor=0);

	virtual LPDIRECT3DTEXTURE9 GetD3DTexture();
	virtual bool ApplyTexture(int stage);

	virtual void Destory();

	virtual bool Reset();
	virtual bool Restore();	

	int GetFrameCount();
	CD3DTexture * GetFrameTexture(int FrameIndex);

	virtual void PickResource(CNameObjectSet * pObjectSet,UINT Param=0);

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

}