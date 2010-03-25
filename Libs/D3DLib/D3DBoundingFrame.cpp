/****************************************************************************/
/*                                                                          */
/*      文件名:    D3DBoundingFrame.cpp                                     */
/*      创建日期:  2009年09月25日                                           */
/*      作者:      Sagasarate                                               */
/*                                                                          */
/*      本软件版权归Sagasarate(sagasarate@sina.com)所有                     */
/*      你可以将本软件用于任何商业和非商业软件开发，但                      */
/*      必须保留此版权声明                                                  */
/*                                                                          */
/****************************************************************************/
#include "StdAfx.h"


namespace D3DLib{


static LPCTSTR DEFAULT_BOUND_FRAME_FX_NT=
	"texture TexLay0 < string name = \"test.jpg\"; >;	\r\n"
	"texture TexLay1 < string name = \"test1.jpg\"; >;	\r\n"
	"technique tec0										\r\n"
	"{													\r\n"
	"    pass p0										\r\n"
	"    {												\r\n"
	"		MultiSampleAntialias = false;				\r\n"	
	"		Lighting = false;							\r\n"
	"		zenable = true;								\r\n"
	"		zwriteenable = true;						\r\n"
	"		CullMode = none;							\r\n"
	"		fogenable = false;							\r\n"
	"		Texture[0] = <TexLay0>;						\r\n"
	"		AlphaTestEnable = false;					\r\n"
	"		AlphaBlendEnable = false;					\r\n"
	"     	ColorOp[0] = SelectArg1;					\r\n"
	"       ColorArg1[0] = Diffuse;						\r\n"      	
	"       AlphaOp[0] = disable;						\r\n"
	"		ColorOp[1] = disable;						\r\n"
	"		AlphaOp[1] = disable;						\r\n"
	"		VertexShader = NULL;						\r\n"
	"		PixelShader  = NULL;						\r\n"
	"    }												\r\n"
	"}													\r\n";



IMPLEMENT_CLASS_INFO(CD3DBoundingFrame,CD3DObject);

CD3DBoundingFrame::CD3DBoundingFrame(void):CD3DObject()
{
	m_pSubMesh=NULL;
}

CD3DBoundingFrame::~CD3DBoundingFrame(void)
{
	Destory();
}

void CD3DBoundingFrame::Destory()
{
	SAFE_RELEASE(m_pSubMesh);	
	CD3DObject::Destory();
}

bool CD3DBoundingFrame::Reset()
{		
	return m_pSubMesh->Reset()&&CD3DObject::Reset();
	
}

bool CD3DBoundingFrame::Restore()
{		
	return m_pSubMesh->Restore()&&CD3DObject::Restore();	
}

bool CD3DBoundingFrame::CreateFromBBox(const CD3DBoundingBox& BBox)
{
	if(GetDevice()==NULL)
		return false;

	Destory();

	m_BoundingBox=BBox;

	m_pSubMesh=new CD3DSubMesh;

	m_pSubMesh->GetVertexFormat().FVF=D3DFVF_XYZ|D3DFVF_DIFFUSE;
	m_pSubMesh->GetVertexFormat().VertexSize=sizeof(BF_VERTEX);
	m_pSubMesh->SetVertexCount(12*2);
	m_pSubMesh->SetPrimitiveCount(12);
	m_pSubMesh->SetPrimitiveType(D3DPT_LINELIST);
	m_pSubMesh->GetBoundingBox()=BBox;

	m_pSubMesh->AllocDXVertexBuffer(GetDevice());
	
	

	//D3DCOLORVALUE WhiteColor={1.0f,1.0f,1.0f,1.0f};
	//D3DCOLORVALUE BlackColor={0.0f,0.0f,0.0f,1.0f};
	//D3DCOLORVALUE RedColor={1.0f,0.0f,0.0f,1.0f};

	//m_pSubMesh->Material.Material.Ambient=RedColor;
	//m_pSubMesh->Material.Material.Diffuse=RedColor;
	//m_pSubMesh->Material.Material.Specular=BlackColor;
	//m_pSubMesh->Material.Material.Emissive=RedColor;
	//m_pSubMesh->Material.Material.Power=100.0f;
	m_pSubMesh->GetMaterial().SetFX(
		GetDevice()->GetFXManager()->LoadFXFromMemory("DEFAULT_BOUND_FRAME_FX_NT",(void *)DEFAULT_BOUND_FRAME_FX_NT,(int)strlen(DEFAULT_BOUND_FRAME_FX_NT)));

	
	BF_VERTEX * pBuff;



	
	m_pSubMesh->GetDXVertexBuffer()->Lock(0,0,(LPVOID *)&pBuff,0);


	BuildLine(pBuff,
		CD3DVector3(BBox.m_Min.x,BBox.m_Min.y,BBox.m_Min.z),
		CD3DVector3(BBox.m_Max.x,BBox.m_Min.y,BBox.m_Min.z));
	pBuff+=2;

	BuildLine(pBuff,
		CD3DVector3(BBox.m_Max.x,BBox.m_Min.y,BBox.m_Min.z),
		CD3DVector3(BBox.m_Max.x,BBox.m_Max.y,BBox.m_Min.z));
	pBuff+=2;

	BuildLine(pBuff,
		CD3DVector3(BBox.m_Max.x,BBox.m_Max.y,BBox.m_Min.z),
		CD3DVector3(BBox.m_Min.x,BBox.m_Max.y,BBox.m_Min.z));
	pBuff+=2;

	BuildLine(pBuff,
		CD3DVector3(BBox.m_Min.x,BBox.m_Max.y,BBox.m_Min.z),
		CD3DVector3(BBox.m_Min.x,BBox.m_Min.y,BBox.m_Min.z));
	pBuff+=2;

	//
	BuildLine(pBuff,
		CD3DVector3(BBox.m_Min.x,BBox.m_Min.y,BBox.m_Min.z),
		CD3DVector3(BBox.m_Min.x,BBox.m_Min.y,BBox.m_Max.z));
	pBuff+=2;

	BuildLine(pBuff,
		CD3DVector3(BBox.m_Max.x,BBox.m_Min.y,BBox.m_Min.z),
		CD3DVector3(BBox.m_Max.x,BBox.m_Min.y,BBox.m_Max.z));
	pBuff+=2;

	BuildLine(pBuff,
		CD3DVector3(BBox.m_Max.x,BBox.m_Max.y,BBox.m_Min.z),
		CD3DVector3(BBox.m_Max.x,BBox.m_Max.y,BBox.m_Max.z));
	pBuff+=2;

	BuildLine(pBuff,
		CD3DVector3(BBox.m_Min.x,BBox.m_Max.y,BBox.m_Min.z),
		CD3DVector3(BBox.m_Min.x,BBox.m_Max.y,BBox.m_Max.z));
	pBuff+=2;
	//
	BuildLine(pBuff,
		CD3DVector3(BBox.m_Min.x,BBox.m_Min.y,BBox.m_Max.z),
		CD3DVector3(BBox.m_Max.x,BBox.m_Min.y,BBox.m_Max.z));
	pBuff+=2;

	BuildLine(pBuff,
		CD3DVector3(BBox.m_Max.x,BBox.m_Min.y,BBox.m_Max.z),
		CD3DVector3(BBox.m_Max.x,BBox.m_Max.y,BBox.m_Max.z));
	pBuff+=2;

	BuildLine(pBuff,
		CD3DVector3(BBox.m_Max.x,BBox.m_Max.y,BBox.m_Max.z),
		CD3DVector3(BBox.m_Min.x,BBox.m_Max.y,BBox.m_Max.z));
	pBuff+=2;

	BuildLine(pBuff,
		CD3DVector3(BBox.m_Min.x,BBox.m_Max.y,BBox.m_Max.z),
		CD3DVector3(BBox.m_Min.x,BBox.m_Min.y,BBox.m_Max.z));
	pBuff+=2;

	m_pSubMesh->GetDXVertexBuffer()->Unlock();


	return true;
}

int CD3DBoundingFrame::GetSubMeshCount()
{
	return 1;
}

CD3DSubMesh * CD3DBoundingFrame::GetSubMesh(int index)
{
	return m_pSubMesh;
}

CD3DBoundingBox * CD3DBoundingFrame::GetBoundingBox()
{
	return &m_BoundingBox;
}

bool CD3DBoundingFrame::RayIntersect(const CD3DVector3& Point,const CD3DVector3& Dir,CD3DVector3& IntersectPoint,FLOAT& Distance,bool TestOnly)
{
	return false;
}
bool CD3DBoundingFrame::GetHeightByXZ(FLOAT x,FLOAT z,FLOAT& y)
{
	return false;
}

void CD3DBoundingFrame::BuildRect(BF_VERTEX * pBuff,CD3DVector3 p1,CD3DVector3 p2,CD3DVector3 p3,CD3DVector3 p4)
{
	pBuff[0].Pos=p1;
	pBuff[1].Pos=p2;
	pBuff[2].Pos=p4;
	pBuff[3].Pos=p2;
	pBuff[4].Pos=p3;
	pBuff[5].Pos=p4;

	
	for(int i=0;i<6;i++)
	{
		pBuff[i].Color=D3DCOLOR_ARGB(255,255,0,0);
	}
}

void CD3DBoundingFrame::BuildLine(BF_VERTEX * pBuff,CD3DVector3 p1,CD3DVector3 p2)
{
	pBuff[0].Pos=p1;
	pBuff[1].Pos=p2;
	for(int i=0;i<2;i++)
	{
		pBuff[i].Color=D3DCOLOR_ARGB(255,255,0,0);
	}
}

bool CD3DBoundingFrame::ToSmartStruct(CSmartStruct& Packet,CUSOFile * pUSOFile,UINT Param)
{
	PrintSystemLog(0,"CD3DBoundingFrame被保存");
	return CD3DObject::ToSmartStruct(Packet,pUSOFile,Param);
}

}