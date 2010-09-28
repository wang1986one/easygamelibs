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

}

CD3DWOWWMOGroupModel::~CD3DWOWWMOGroupModel(void)
{
	Destory();
}

void CD3DWOWWMOGroupModel::Destory()
{
	SAFE_RELEASE(m_pModelResource);
	m_pGroupInfo=NULL;
	m_GroupIndex=0;
	CD3DBaseStaticModel::Destory();
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
		return true;
	}
	return false;
}

void CD3DWOWWMOGroupModel::PrepareRender(CD3DDevice * pDevice,CD3DSubMesh * pSubMesh,CD3DSubMeshMaterial * pMaterial,CEasyArray<CD3DLight *>& LightList,CD3DCamera * pCamera)
{
	if(pSubMesh&&pMaterial)
	{	

		if(pMaterial->GetFX())
		{
			//设置灯光
			if(LightList.GetCount())
			{		
				D3DLIGHT9	Light;
				char		szParamName[32];
				pMaterial->GetFX()->SetInt("LightCount",LightList.GetCount());
				for(UINT i=0;i<LightList.GetCount();i++)
				{
					LightList[i]->GetCurLight(Light);
					sprintf_s(szParamName,32,"LightType[%d]",i);
					pMaterial->GetFX()->SetInt(szParamName,Light.Type);
					sprintf_s(szParamName,32,"LightPos[%d]",i);
					pMaterial->GetFX()->SetVector(szParamName,CD3DVector3(Light.Position));
					sprintf_s(szParamName,32,"LightDir[%d]",i);
					pMaterial->GetFX()->SetVector(szParamName,CD3DVector3(Light.Direction));
					sprintf_s(szParamName,32,"LightAmbient[%d]",i);
					pMaterial->GetFX()->SetColor(szParamName,Light.Ambient);
					sprintf_s(szParamName,32,"LightDiffuse[%d]",i);
					pMaterial->GetFX()->SetColor(szParamName,Light.Diffuse);
					sprintf_s(szParamName,32,"LightSpecular[%d]",i);
					pMaterial->GetFX()->SetColor(szParamName,Light.Specular);
					sprintf_s(szParamName,32,"LightRange[%d]",i);
					pMaterial->GetFX()->SetFloat(szParamName,Light.Range);
					sprintf_s(szParamName,32,"LightAtn0[%d]",i);
					pMaterial->GetFX()->SetFloat(szParamName,Light.Attenuation0);
					sprintf_s(szParamName,32,"LightAtn1[%d]",i);
					pMaterial->GetFX()->SetFloat(szParamName,Light.Attenuation1);
					sprintf_s(szParamName,32,"LightAtn2[%d]",i);
					pMaterial->GetFX()->SetFloat(szParamName,Light.Attenuation2);
					//sprintf_s(szParamName,32,"LightFalloff[%d]",i);
					//pMaterial->GetFX()->SetFloat(szParamName,Light.Falloff);
					//sprintf_s(szParamName,32,"LightTheta[%d]",i);
					//pMaterial->GetFX()->SetFloat(szParamName,Light.Theta);
					//sprintf_s(szParamName,32,"LightPhi[%d]",i);
					//pMaterial->GetFX()->SetFloat(szParamName,Light.Phi);

				}

			}
			//设置雾
			pMaterial->GetFX()->SetColor("FogColor",GetRender()->GetFogColor());
			pMaterial->GetFX()->SetFloat("FogNear",GetRender()->GetFogNear());
			pMaterial->GetFX()->SetFloat("FogFar",GetRender()->GetFogFar());

			//设置材质
			D3DMATERIAL9 * pD3DMaterial;
			if(pSubMesh->IsSelected())
				pD3DMaterial=&SELECTED_SUBMESH_MATERIAL;
			else
				pD3DMaterial=&(pMaterial->GetMaterial());
			pMaterial->GetFX()->SetColor("MaterialAmbient",pD3DMaterial->Ambient);
			pMaterial->GetFX()->SetColor("MaterialDiffuse",pD3DMaterial->Diffuse);
			pMaterial->GetFX()->SetColor("MaterialSpecular",pD3DMaterial->Specular);
			pMaterial->GetFX()->SetColor("MaterialEmissive",pD3DMaterial->Emissive);
			pMaterial->GetFX()->SetFloat("MaterialPower",pD3DMaterial->Power);

			//设置纹理
			pMaterial->GetFX()->SetTexture("TexLay0",pMaterial->GetTexture(0));
			//pMaterial->GetFX()->SetTexture("TexLay1",pMaterial->GetTexture(1));

			CD3DMatrix Mat=GetWorldMatrix()*pCamera->GetViewMat();

			pMaterial->GetFX()->SetMatrix("WorldMatrix",GetWorldMatrix());
			pMaterial->GetFX()->SetMatrix("WorldViewMatrix",Mat);
			pMaterial->GetFX()->SetMatrix("PrjMatrix",pCamera->GetProjectMat());


			pMaterial->GetFX()->SetVector("CameraPos",pCamera->GetWorldMatrix().GetTranslation());
			pMaterial->GetFX()->SetFloat("CameraNear",pCamera->GetNear());
			pMaterial->GetFX()->SetFloat("CameraFar",pCamera->GetFar());
		}	
	}
	else
	{

	}
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

CD3DSubMeshMaterial * CD3DWOWWMOGroupModel::GetSubMeshMaterial(int index)
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

}