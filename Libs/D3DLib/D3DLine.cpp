#include "StdAfx.h"

namespace D3DLib{

const LPCTSTR DEFAULT_LINE_FX_NT=
"technique tec0											\r\n"
"{														\r\n"
"    pass p0											\r\n"
"    {													\r\n"
"		zenable = true;									\r\n"
"		zwriteenable = true;							\r\n"
"		lighting = false;								\r\n"
"		CullMode = none;								\r\n"
"		fogenable = false;								\r\n"
"		AlphaTestEnable = false;						\r\n"
"		AlphaBlendEnable = false;						\r\n"
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

IMPLEMENT_CLASS_INFO(CD3DLine,CD3DObject);

CD3DLine::CD3DLine(void)
{
	m_pSubMesh=NULL;
}

CD3DLine::~CD3DLine(void)
{
}

void CD3DLine::Destory()
{
	CD3DObject::Destory();

	SAFE_RELEASE(m_pSubMesh);
}
bool CD3DLine::Reset()
{
	return m_pSubMesh->Reset()&&CD3DObject::Reset();
}
bool CD3DLine::Restore()
{
	return m_pSubMesh->Restore()&&CD3DObject::Restore();	
}

bool CD3DLine::Create(CD3DVector3 P1,CD3DVector3 P2,D3DCOLOR Color1,D3DCOLOR Color2)
{
	if(GetDevice()==NULL)
		return false;

	Destory();

	m_BoundingBox.ComputeFromVertex(&P1,1,sizeof(P1));
	m_BoundingBox.AppendFromVertex(&P2,1,sizeof(P2));

	m_pSubMesh=new CD3DSubMesh(GetDevice());

	m_pSubMesh->GetVertexFormat().FVF=D3DFVF_XYZ|D3DFVF_DIFFUSE;
	m_pSubMesh->GetVertexFormat().VertexSize=sizeof(VERTEX);
	m_pSubMesh->SetVertexCount(2);
	m_pSubMesh->SetPrimitiveCount(1);
	m_pSubMesh->SetPrimitiveType(D3DPT_LINELIST);
	m_pSubMesh->GetBoundingBox()=m_BoundingBox;

	m_pSubMesh->AllocDXVertexBuffer();	


	m_pSubMesh->GetMaterial().SetFX(
		GetDevice()->GetFXManager()->LoadFXFromMemory("DEFAULT_LINE_FX_NT",(void *)DEFAULT_LINE_FX_NT,(int)strlen(DEFAULT_LINE_FX_NT)));


	Modify(P1,P2,Color1,Color2);

	return true;
}

void CD3DLine::Modify(CD3DVector3 P1,CD3DVector3 P2,D3DCOLOR Color1,D3DCOLOR Color2)
{
	VERTEX * pBuff;

	m_pSubMesh->GetDXVertexBuffer()->Lock(0,0,(LPVOID *)&pBuff,0);


	pBuff[0].Pos=P1;
	pBuff[1].Pos=P2;
	pBuff[0].Color=Color1;
	pBuff[1].Color=Color2;

	m_pSubMesh->GetDXVertexBuffer()->Unlock();
}

int CD3DLine::GetSubMeshCount()
{
	return 1;
}
CD3DSubMesh * CD3DLine::GetSubMesh(UINT index)
{
	return m_pSubMesh;
}
CD3DBoundingBox * CD3DLine::GetBoundingBox()
{
	return &m_BoundingBox;
}

bool CD3DLine::ToSmartStruct(CSmartStruct& Packet,CUSOResourceManager * pResourceManager,UINT Param)
{
	PrintSystemLog(0,"CD3DLine被保存");
	return CD3DObject::ToSmartStruct(Packet,pResourceManager,Param);
}
UINT CD3DLine::GetSmartStructSize(UINT Param)
{
	PrintSystemLog(0,"CD3DLine被保存");
	return CD3DObject::GetSmartStructSize(Param);
}


}