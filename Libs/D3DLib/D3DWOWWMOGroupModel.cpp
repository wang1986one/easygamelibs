/****************************************************************************/
/*                                                                          */
/*      文件名:    D3DWOWWMOGroupModel.cpp	                                */
/*      创建日期:  2010年08月04日                                           */
/*      作者:      Sagasarate                                               */
/*                                                                          */
/*      本软件版权归Sagasarate(sagasarate@sina.com)所有                     */
/*      你可以将本软件用于任何商业和非商业软件开发，但                      */
/*      必须保留此版权声明                                                  */
/*                                                                          */
/****************************************************************************/
#include "StdAfx.h"

namespace D3DLib{

IMPLEMENT_CLASS_INFO(CD3DWOWWMOGroupModel,CD3DBaseStaticModel);

CD3DWOWWMOGroupModel::CD3DWOWWMOGroupModel(void)
{
	m_pModelResource=NULL;
	m_pGroupInfo=NULL;
	m_GroupIndex=0;

	//m_IsSelected=false;
	//m_pHeightTestLine=NULL;

}

CD3DWOWWMOGroupModel::~CD3DWOWWMOGroupModel(void)
{
	Destory();
}

void CD3DWOWWMOGroupModel::Destory()
{
	CD3DBaseStaticModel::Destory();

	//m_pHeightTestLine=NULL;
	SAFE_RELEASE(m_pModelResource);
	m_pGroupInfo=NULL;
	m_GroupIndex=0;
	
}

bool CD3DWOWWMOGroupModel::LoadFromResource(CD3DWOWWMOModelResource * pModelResource,UINT GroupIndex)
{
	Destory();


	m_pGroupInfo=pModelResource->GetGroupInfo(GroupIndex);
	if(m_pGroupInfo)
	{
		m_pModelResource=pModelResource;
		m_pModelResource->AddUseRef();
		m_GroupIndex=GroupIndex;
		SetName(m_pGroupInfo->Name);
		//CreatePortalBoard();
		//CreateBSPBoard();
		return true;
	}
	return false;
}

void CD3DWOWWMOGroupModel::OnPrepareRender(CD3DBaseRender * pRender,CD3DFX * pFX,CEasyArray<CD3DLight *>& LightList,CD3DCamera * pCamera)
{
	//设置灯光
	if(LightList.GetCount())
	{		
		D3DLIGHT9	Light;
		char		szParamName[32];
		pFX->SetInt("LightCount",LightList.GetCount());
		for(UINT i=0;i<LightList.GetCount();i++)
		{
			LightList[i]->GetCurLight(Light);
			sprintf_s(szParamName,32,"LightType[%d]",i);
			pFX->SetInt(szParamName,Light.Type);
			sprintf_s(szParamName,32,"LightPos[%d]",i);
			pFX->SetVector(szParamName,CD3DVector3(Light.Position));
			sprintf_s(szParamName,32,"LightDir[%d]",i);
			pFX->SetVector(szParamName,CD3DVector3(Light.Direction));
			sprintf_s(szParamName,32,"LightAmbient[%d]",i);
			pFX->SetColor(szParamName,Light.Ambient);
			sprintf_s(szParamName,32,"LightDiffuse[%d]",i);
			pFX->SetColor(szParamName,Light.Diffuse);
			sprintf_s(szParamName,32,"LightSpecular[%d]",i);
			pFX->SetColor(szParamName,Light.Specular);
			sprintf_s(szParamName,32,"LightRange[%d]",i);
			pFX->SetFloat(szParamName,Light.Range);
			sprintf_s(szParamName,32,"LightAtn0[%d]",i);
			pFX->SetFloat(szParamName,Light.Attenuation0);
			sprintf_s(szParamName,32,"LightAtn1[%d]",i);
			pFX->SetFloat(szParamName,Light.Attenuation1);
			sprintf_s(szParamName,32,"LightAtn2[%d]",i);
			pFX->SetFloat(szParamName,Light.Attenuation2);
			//sprintf_s(szParamName,32,"LightFalloff[%d]",i);
			//pFX->SetFloat(szParamName,Light.Falloff);
			//sprintf_s(szParamName,32,"LightTheta[%d]",i);
			//pFX->SetFloat(szParamName,Light.Theta);
			//sprintf_s(szParamName,32,"LightPhi[%d]",i);
			//pFX->SetFloat(szParamName,Light.Phi);

		}

	}
	////设置雾
	//CD3DSceneRender * pRender=(CD3DSceneRender *)GetRender();
	//pFX->SetColor("FogColor",pRender->GetFogColor());
	//pFX->SetFloat("FogNear",pRender->GetFogNear());
	//pFX->SetFloat("FogFar",pRender->GetFogFar());

	CD3DMatrix Mat=GetWorldMatrixR()*pCamera->GetViewMatR();

	pFX->SetMatrix("WorldMatrix",GetWorldMatrixR());
	pFX->SetMatrix("WorldViewMatrix",Mat);
	//pFX->SetMatrix("PrjMatrix",pCamera->GetProjectMatR());


	//pFX->SetVector("CameraPos",pCamera->GetWorldMatrixR().GetTranslation());
	//pFX->SetFloat("CameraNear",pCamera->GetNear());
	//pFX->SetFloat("CameraFar",pCamera->GetFar());
}
void CD3DWOWWMOGroupModel::OnPrepareRenderSubMesh(CD3DBaseRender * pRender,CD3DFX * pFX,CD3DSubMesh * pSubMesh,CD3DSubMeshMaterial * pMaterial,CEasyArray<CD3DLight *>& LightList,CD3DCamera * pCamera)
{
	////设置材质
	//D3DMATERIAL9 * pD3DMaterial;
	//if(pSubMesh->IsSelected())
	//	pD3DMaterial=&SELECTED_SUBMESH_MATERIAL;
	//else
	//	pD3DMaterial=&(pMaterial->GetMaterial());
	//pMaterial->GetFX()->SetColor("MaterialAmbient",pD3DMaterial->Ambient);
	//pMaterial->GetFX()->SetColor("MaterialDiffuse",pD3DMaterial->Diffuse);
	//pMaterial->GetFX()->SetColor("MaterialSpecular",pD3DMaterial->Specular);
	//pMaterial->GetFX()->SetColor("MaterialEmissive",pD3DMaterial->Emissive);
	//pMaterial->GetFX()->SetFloat("MaterialPower",pD3DMaterial->Power);

	//设置纹理
	pFX->SetTexture("TexLay0",pMaterial->GetTexture(0));
	//pMaterial->GetFX()->SetTexture("TexLay1",pMaterial->GetTexture(1));

}


int CD3DWOWWMOGroupModel::GetSubMeshCount()
{
	if(m_pGroupInfo)
	{
		return m_pGroupInfo->GroupSubMeshList.GetCount();
	}
	return 0;
}

CD3DSubMesh * CD3DWOWWMOGroupModel::GetSubMesh(UINT index)
{
	if(m_pGroupInfo)
	{
		if(index<m_pGroupInfo->GroupSubMeshList.GetCount())
		{
			return m_pGroupInfo->GroupSubMeshList[index];
		}
	}
	return NULL;
}

CD3DSubMeshMaterial * CD3DWOWWMOGroupModel::GetSubMeshMaterial(UINT index)
{
	if(m_pGroupInfo)
	{
		if(index<m_pGroupInfo->GroupSubMeshList.GetCount())
		{
			return &(m_pGroupInfo->GroupSubMeshList[index]->GetMaterial());
		}
	}
	return NULL;
}

CD3DBoundingBox * CD3DWOWWMOGroupModel::GetBoundingBox()
{
	if(m_pGroupInfo)
	{		
		return &(m_pGroupInfo->BoundingBox);
	}
	return NULL;
}

CD3DBoundingSphere * CD3DWOWWMOGroupModel::GetBoundingSphere()
{
	if(m_pModelResource)
	{
		m_pModelResource->GetBoundingSphere();
	}
	return NULL;
}
bool CD3DWOWWMOGroupModel::GetHeightByXZ(const CD3DVector3& Pos,FLOAT MinHeight,FLOAT MaxHeight,FLOAT& Height,FLOAT& WaterHeight)
{
	//if(m_IsSelected)
	//{
	//	m_IsSelected=false;
	//	for(UINT i=0;i<m_pGroupInfo->VertexList.GetCount();i++)
	//	{
	//		m_pGroupInfo->VertexList[i].Diffuse=0xFFFFFFFF;
	//	}
	//}
 //	if(m_pHeightTestLine==NULL)
	//{
	//	m_pHeightTestLine=new CD3DLine();
	//	m_pHeightTestLine->SetDevice(GetDevice());
	//	CD3DVector3 P1=Pos;
	//	CD3DVector3 P2=Pos;
	//	P1.y=MaxHeight;
	//	P2.y=MinHeight;
	//	P1=P1*GetWorldMatrix().GetInverse();
	//	P2=P2*GetWorldMatrix().GetInverse();
	//	m_pHeightTestLine->Create(P1,P2,D3DCOLOR_XRGB(0,0,0xFF),D3DCOLOR_XRGB(0xFF,0,0));
	//	m_pHeightTestLine->SetParent(this);
	//	if(GetRender())
	//		GetRender()->AddObject(m_pHeightTestLine);
	//}
	//else
	//{
	//	CD3DVector3 P1=Pos;
	//	CD3DVector3 P2=Pos;
	//	P1.y=MaxHeight;
	//	P2.y=MinHeight;
	//	P1=P1*GetWorldMatrix().GetInverse();
	//	P2=P2*GetWorldMatrix().GetInverse();
	//	m_pHeightTestLine->Modify(P1,P2,D3DCOLOR_XRGB(0,0,0xFF),D3DCOLOR_XRGB(0xFF,0,0));
	//}
	bool HaveHeight=false;
	if(GetBoundingBox())
	{
		CD3DVector3 FindPos=Pos*GetWorldMatrix().GetInverse();
		CD3DVector3 RayPos=FindPos;
		CD3DVector3 RayDir(0,-1,0);
		CD3DVector3 IntersectPoint;
		FLOAT Distance;
		MinHeight=MinHeight-GetWorldMatrix().GetTranslation().y;
		MaxHeight=MaxHeight-GetWorldMatrix().GetTranslation().y;
		RayPos.y=MaxHeight;		
		CD3DBoundingBox WMOBBox=*GetBoundingBox();
		if(WMOBBox.RayIntersect(RayPos,RayDir,IntersectPoint,Distance)||
			WMOBBox.CheckRelation(FindPos)!=CD3DBoundingBox::RELATION_TYPE_OUT||
			WMOBBox.CheckRelation(RayPos)!=CD3DBoundingBox::RELATION_TYPE_OUT)
		{
			CD3DWOWWMOModelResource::BSP_NODE * pRoot=m_pGroupInfo->BSPTree.GetObject(0);
			if(pRoot)
			{				
				CD3DWOWWMOModelResource::BSP_NODE * pNode=FindBSPNode(pRoot,FindPos);
				if(pNode)
				{
					FLOAT FinalHeight=MinHeight;
					
					for(UINT i=0;i<pNode->FaceCount;i++)
					{
						WORD Face=m_pGroupInfo->BSPFaceList[i+pNode->FirstFace];
						if((m_pGroupInfo->FaceFlags[Face]&CD3DWOWWMOModelResource::FF_NO_COLLIDE)==0)
						{
							WORD Index1=m_pGroupInfo->IndexList[Face*3];
							WORD Index2=m_pGroupInfo->IndexList[Face*3+1];						
							WORD Index3=m_pGroupInfo->IndexList[Face*3+2];
							FLOAT U,V;						
							if(D3DXIntersectTri(
								&m_pGroupInfo->VertexList[Index1].Pos,
								&m_pGroupInfo->VertexList[Index2].Pos,
								&m_pGroupInfo->VertexList[Index3].Pos,
								&RayPos,&RayDir,&U,&V,&Distance))
							{
								FLOAT H1=m_pGroupInfo->VertexList[Index1].Pos.y;
								FLOAT H2=m_pGroupInfo->VertexList[Index2].Pos.y;
								FLOAT H3=m_pGroupInfo->VertexList[Index3].Pos.y;
								FLOAT H=H1+(H2-H1)*U+(H3-H1)*V;
								if(H>MinHeight&&H<MaxHeight)
								{
									HaveHeight=true;
									if(H>FinalHeight)
										FinalHeight=H;
								}
							}
							//m_pGroupInfo->VertexList[Index1].Diffuse=0xFFFF0000;
							//m_pGroupInfo->VertexList[Index2].Diffuse=0xFFFF0000;
							//m_pGroupInfo->VertexList[Index3].Diffuse=0xFFFF0000;
							//m_IsSelected=true;						
						}
					}
					if(HaveHeight)
					{
						CD3DVector3 HeightVector(0,FinalHeight,0);
						HeightVector=HeightVector*GetWorldMatrix();
						Height=HeightVector.y;
						WaterHeight=HeightVector.y;
					}
				}
			}
		}
	}
	
	return HaveHeight;
}


void CD3DWOWWMOGroupModel::CreatePortalBoard()
{
	if(m_pGroupInfo)
	{
		for(UINT i=0;i<m_pGroupInfo->PortalList.GetCount();i++)
		{
			if(m_pGroupInfo->PortalList[i].Vertices.GetCount()>=4)
			{
				CD3DBoard * pBoard=new CD3DBoard();
				pBoard->SetDevice(GetDevice());
				if(pBoard->Create(m_pGroupInfo->PortalList[i].Vertices[0],
					m_pGroupInfo->PortalList[i].Vertices[3],
					m_pGroupInfo->PortalList[i].Vertices[1],
					m_pGroupInfo->PortalList[i].Vertices[2],
					m_pGroupInfo->PortalList[i].Normal,
					0x300000FF))
				{
					CEasyString Name;
					Name.Format("%s%d",
						GetName(),i);
					pBoard->SetParent(this);
					pBoard->SetName(Name);
					m_PortalBoards.Add(pBoard);
				}
				else
				{
					SAFE_RELEASE(pBoard);
				}
			}
		}
	}
}

void CD3DWOWWMOGroupModel::CreateBSPBoard(CD3DWOWWMOModelResource::BSP_NODE * pNode)
{
	
	CD3DBoundingBox& BBox=m_pGroupInfo->BoundingBox;
	switch(pNode->PlaneType)
	{
	case CD3DWOWWMOModelResource::BPT_XY:
		{
			pNode->pFaceBoard=new CD3DBoard();
			pNode->pFaceBoard->SetDevice(GetDevice());
			pNode->pFaceBoard->Create(
				CD3DVector3(BBox.m_Min.x,BBox.m_Max.y,pNode->Distance),
				CD3DVector3(BBox.m_Max.x,BBox.m_Max.y,pNode->Distance),
				CD3DVector3(BBox.m_Min.x,BBox.m_Min.y,pNode->Distance),
				CD3DVector3(BBox.m_Max.x,BBox.m_Min.y,pNode->Distance),
				CD3DVector3(0,0,1),D3DCOLOR_ARGB(0x80,0xff,0,0));

			pNode->pFaceBoard->SetParent(this);
			if(GetRender())
				GetRender()->AddObject(pNode->pFaceBoard);
		}
		break;
	case CD3DWOWWMOModelResource::BPT_YZ:
		{
			pNode->pFaceBoard=new CD3DBoard();
			pNode->pFaceBoard->SetDevice(GetDevice());
			pNode->pFaceBoard->Create(
				CD3DVector3(pNode->Distance,BBox.m_Min.y,BBox.m_Max.z),
				CD3DVector3(pNode->Distance,BBox.m_Max.y,BBox.m_Max.z),
				CD3DVector3(pNode->Distance,BBox.m_Min.y,BBox.m_Min.z),
				CD3DVector3(pNode->Distance,BBox.m_Max.y,BBox.m_Min.z),
				CD3DVector3(1,0,0),D3DCOLOR_ARGB(0x80,0xff,0,0));

			pNode->pFaceBoard->SetParent(this);
			if(GetRender())
				GetRender()->AddObject(pNode->pFaceBoard);
		}
		break;
	case CD3DWOWWMOModelResource::BPT_XZ:
		{
			pNode->pFaceBoard=new CD3DBoard();
			pNode->pFaceBoard->SetDevice(GetDevice());
			pNode->pFaceBoard->Create(
				CD3DVector3(BBox.m_Min.x,pNode->Distance,BBox.m_Max.z),
				CD3DVector3(BBox.m_Max.x,pNode->Distance,BBox.m_Max.z),
				CD3DVector3(BBox.m_Min.x,pNode->Distance,BBox.m_Min.z),
				CD3DVector3(BBox.m_Max.x,pNode->Distance,BBox.m_Min.z),
				CD3DVector3(0,1,0),D3DCOLOR_ARGB(0x80,0xff,0,0));

			pNode->pFaceBoard->SetParent(this);
			if(GetRender())
				GetRender()->AddObject(pNode->pFaceBoard);
		}
		break;
	case CD3DWOWWMOModelResource::BPT_LEAF:
		break;
	}
	
		
}

void CD3DWOWWMOGroupModel::SetBSPColor(CD3DWOWWMOModelResource::BSP_NODE * pTree,D3DCOLOR Color)
{
	if(pTree->FaceCount)
	{
		for(UINT i=0;i<pTree->FaceCount;i++)
		{
			WORD Face=m_pGroupInfo->BSPFaceList[i+pTree->FirstFace];
			WORD Index=m_pGroupInfo->IndexList[Face*3];
			m_pGroupInfo->VertexList[Index].Diffuse=Color;
			Index=m_pGroupInfo->IndexList[Face*3+1];
			m_pGroupInfo->VertexList[Index].Diffuse=Color;
			Index=m_pGroupInfo->IndexList[Face*3+2];
			m_pGroupInfo->VertexList[Index].Diffuse=Color;
		}
	}
	if(pTree->LeftChildIndex>=0)
	{
		SetBSPColor(m_pGroupInfo->BSPTree.GetObject(pTree->LeftChildIndex),Color);
	}
	if(pTree->RightChildIndex>=0)
	{
		SetBSPColor(m_pGroupInfo->BSPTree.GetObject(pTree->RightChildIndex),Color);
	}
}

CD3DWOWWMOModelResource::BSP_NODE * CD3DWOWWMOGroupModel::FindBSPNode(CD3DWOWWMOModelResource::BSP_NODE * pRoot,CD3DVector3 Pos)
{
	if(pRoot==NULL)
		return NULL;
	//CreateBSPBoard(pRoot);
	switch(pRoot->PlaneType)
	{
	case CD3DWOWWMOModelResource::BPT_XY:
		if(Pos.z<pRoot->Distance)
			return FindBSPNode(m_pGroupInfo->BSPTree.GetObject(pRoot->RightChildIndex),Pos);
		else
			return FindBSPNode(m_pGroupInfo->BSPTree.GetObject(pRoot->LeftChildIndex),Pos);
		break;
	case CD3DWOWWMOModelResource::BPT_YZ:
		if(Pos.x<pRoot->Distance)
			return FindBSPNode(m_pGroupInfo->BSPTree.GetObject(pRoot->RightChildIndex),Pos);
		else
			return FindBSPNode(m_pGroupInfo->BSPTree.GetObject(pRoot->LeftChildIndex),Pos);
		break;
	case CD3DWOWWMOModelResource::BPT_XZ:
		if(Pos.y<pRoot->Distance)
			return FindBSPNode(m_pGroupInfo->BSPTree.GetObject(pRoot->RightChildIndex),Pos);
		else
			return FindBSPNode(m_pGroupInfo->BSPTree.GetObject(pRoot->LeftChildIndex),Pos);		
		break;
	case CD3DWOWWMOModelResource::BPT_LEAF:
		return pRoot;
		break;
	}
	return NULL;
}

}