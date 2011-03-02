#include "StdAfx.h"

namespace D3DLib{

const LPCTSTR DEFAULT_BOARD_FX_NT=
	"texture TexLay0 < string name = \"test.jpg\"; >;		\r\n"
	"texture TexLay1 < string name = \"test1.jpg\"; >;		\r\n"
	"technique tec0											\r\n"
	"{														\r\n"
	"    pass p0											\r\n"
	"    {													\r\n"
	"		zenable = true;									\r\n"
	"		zwriteenable = false;							\r\n"
	"		lighting = false;								\r\n"
	"		CullMode = none;								\r\n"
	"		fogenable = false;								\r\n"
	"		Texture[0] = <TexLay0>;							\r\n"
	"		AlphaTestEnable = false;						\r\n"
	"		AlphaBlendEnable = true;						\r\n"
	"		SrcBlend = SrcAlpha;							\r\n"
	"		DestBlend = InvSrcAlpha;						\r\n"
	"     	ColorOp[0] = SelectArg1;						\r\n"
	"       ColorArg1[0] = Diffuse;							\r\n"      	
	"       AlphaOp[0] = SelectArg1;						\r\n"
	"       AlphaArg1[0] = diffuse;							\r\n"
	"		ColorOp[1] = disable;							\r\n"
	"		AlphaOp[1] = disable;							\r\n"
	"		VertexShader = NULL;							\r\n"
	"		PixelShader  = NULL;							\r\n"
	"    }													\r\n"
	"}														\r\n";

IMPLEMENT_CLASS_INFO(CD3DBoard,CD3DObject);

CD3DBoard::CD3DBoard(void)
{
	m_pSubMesh[0]=NULL;
	m_pSubMesh[1]=NULL;
}

CD3DBoard::~CD3DBoard(void)
{
	Destory();
}

void CD3DBoard::Destory()
{
	CD3DObject::Destory();

	SAFE_RELEASE(m_pSubMesh[0]);
	SAFE_RELEASE(m_pSubMesh[1]);
	
}

bool CD3DBoard::Reset()
{		
	return m_pSubMesh[0]->Reset()&&m_pSubMesh[1]->Reset()&&CD3DObject::Reset();

}

bool CD3DBoard::Restore()
{		
	return m_pSubMesh[0]->Restore()&&m_pSubMesh[1]->Restore()&&CD3DObject::Restore();	
}

bool CD3DBoard::Create(CD3DVector3 V1,CD3DVector3 V2,CD3DVector3 V3,CD3DVector3 V4,CD3DVector3 Normal,D3DCOLOR Color)
{
	if(GetDevice()==NULL)
		return false;

	Destory();

	m_pSubMesh[0]=new CD3DSubMesh(GetDevice());

	m_pSubMesh[0]->GetVertexFormat().FVF=D3DFVF_XYZ|D3DFVF_DIFFUSE;
	m_pSubMesh[0]->GetVertexFormat().VertexSize=sizeof(VERTEX);
	m_pSubMesh[0]->SetVertexCount(4);
	m_pSubMesh[0]->SetPrimitiveCount(2);
	m_pSubMesh[0]->SetPrimitiveType(D3DPT_TRIANGLESTRIP);
	
	m_pSubMesh[0]->AllocDXVertexBuffer();

	VERTEX * pBuff;

	m_pSubMesh[0]->GetDXVertexBuffer()->Lock(0,0,(LPVOID *)&pBuff,0);

	pBuff[0].Pos=V1;
	pBuff[0].Color=Color;
	pBuff[1].Pos=V2;
	pBuff[1].Color=Color;
	pBuff[2].Pos=V3;
	pBuff[2].Color=Color;
	pBuff[3].Pos=V4;
	pBuff[3].Color=Color;

	m_pSubMesh[0]->GetDXVertexBuffer()->Unlock();
	m_pSubMesh[0]->CreateBounding();

	m_pSubMesh[0]->GetMaterial().SetFX(
		GetDevice()->GetFXManager()->LoadFXFromMemory("DEFAULT_BOARD_FX_NT",
		(void *)DEFAULT_BOARD_FX_NT,(int)strlen(DEFAULT_BOARD_FX_NT)));

	m_pSubMesh[0]->SetTransparent(true);



	m_pSubMesh[1]=new CD3DSubMesh(GetDevice());

	m_pSubMesh[1]->GetVertexFormat().FVF=D3DFVF_XYZ|D3DFVF_DIFFUSE;
	m_pSubMesh[1]->GetVertexFormat().VertexSize=sizeof(VERTEX);
	m_pSubMesh[1]->SetVertexCount(2);
	m_pSubMesh[1]->SetPrimitiveCount(1);
	m_pSubMesh[1]->SetPrimitiveType(D3DPT_LINELIST);

	m_pSubMesh[1]->AllocDXVertexBuffer();


	m_pSubMesh[1]->GetDXVertexBuffer()->Lock(0,0,(LPVOID *)&pBuff,0);

	
	pBuff[0].Pos=(V1+V4)/2;
	pBuff[0].Color=0xFF0000FF;
	pBuff[1].Pos=(V1+V4)/2+Normal*(V4-V1).Length()/2;
	pBuff[1].Color=0xFFFF0000;
	
	m_pSubMesh[1]->GetDXVertexBuffer()->Unlock();
	m_pSubMesh[1]->CreateBounding();

	m_pSubMesh[1]->GetMaterial().SetFX(
		GetDevice()->GetFXManager()->LoadFXFromMemory("DEFAULT_BOARD_FX_NT",
		(void *)DEFAULT_BOARD_FX_NT,(int)strlen(DEFAULT_BOARD_FX_NT)));

	m_pSubMesh[1]->SetTransparent(true);
	m_pSubMesh[1]->CreateBounding();

	m_BoundingBox=m_pSubMesh[0]->GetBoundingBox();
	m_BoundingBox.Merge(m_pSubMesh[1]->GetBoundingBox());
	return true;

}

int CD3DBoard::GetSubMeshCount()
{
	return 2;
}

CD3DSubMesh * CD3DBoard::GetSubMesh(UINT index)
{
	if(index<2)
		return m_pSubMesh[index];
	return NULL;
}

CD3DBoundingBox * CD3DBoard::GetBoundingBox()
{	
	return &m_BoundingBox;
}

bool CD3DBoard::ToSmartStruct(CSmartStruct& Packet,CUSOResourceManager * pResourceManager,UINT Param)
{
	PrintSystemLog(0,"CD3DBoard被保存");
	return CD3DObject::ToSmartStruct(Packet,pResourceManager,Param);
}

UINT CD3DBoard::GetSmartStructSize(UINT Param)
{
	PrintSystemLog(0,"CD3DBoard被保存");
	return CD3DObject::GetSmartStructSize(Param);
}

}