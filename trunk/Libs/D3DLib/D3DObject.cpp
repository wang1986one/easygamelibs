/****************************************************************************/
/*                                                                          */
/*      文件名:    D3DObject.cpp                                            */
/*      创建日期:  2010年02月09日                                           */
/*      作者:      Sagasarate                                               */
/*                                                                          */
/*      本软件版权归Sagasarate(sagasarate@sina.com)所有                     */
/*      你可以将本软件用于任何商业和非商业软件开发，但                      */
/*      必须保留此版权声明                                                  */
/*                                                                          */
/****************************************************************************/
#include "StdAfx.h"
#include "d3drender.h"


namespace D3DLib{

D3DMATERIAL9 CD3DObject::SELECTED_SUBMESH_MATERIAL=
{
	{1.0f,0.0f,0.0f,1.0f},
	{1.0f,0.0f,0.0f,1.0f},
	{1.0f,0.0f,0.0f,1.0f},
	{1.0f,0.0f,0.0f,1.0f},
	40.0f
};

IMPLEMENT_CLASS_INFO(CD3DObject,CTreeObject);

CD3DObject::CD3DObject():CTreeObject()
{
	m_pD3DDevice=NULL;
	m_pRender=NULL;		
	m_Layer=0;
	m_pParent=NULL;
	m_LocalMatrix.SetIdentity();
	m_WorldMatrix.SetIdentity();
	m_Flag=OBJECT_FLAG_VISIBLE;		
	m_pBoundingFrame=NULL;
}

CD3DObject::~CD3DObject(void)
{		
	Destory();
}

void CD3DObject::Destory()
{	
	SAFE_RELEASE(m_pBoundingFrame);

	CTreeObject::Destory();	
	if(m_pRender)
		m_pRender->DelObject(this);
	if(m_pRender)
		m_pRender->DelRootObject(this);
}

bool CD3DObject::Reset()
{
	for(UINT i=0;i<GetChildCount();i++)
		GetChildByIndex(i)->Reset();
	return true;
}

bool CD3DObject::Restore()
{
	for(UINT i=0;i<GetChildCount();i++)
		GetChildByIndex(i)->Restore();
	return true;
}

void CD3DObject::SetRender(CD3DRender * pRender)
{
	m_pRender=pRender;
	SetDevice(m_pRender->GetDevice());
}

void CD3DObject::SetParent(CTreeObject* pParent)
{
	if(m_pParent==NULL)		
	{
		if(m_pRender)
			m_pRender->DelRootObject(this);
	}
	CTreeObject::SetParent(pParent);
	if(m_pParent==NULL)
	{
		if(m_pRender)
			m_pRender->AddRootObject(this);
	}
}

CD3DMatrix CD3DObject::GetWorldMatrixDirect()
{
	if(GetParent())
	{
		return m_LocalMatrix*GetParent()->GetWorldMatrixDirect();
	}
	else
	{
		return m_LocalMatrix;
	}
}


int CD3DObject::GetSubMeshCount()
{
	return 0;
}

CD3DSubMesh *  CD3DObject::GetSubMesh(UINT index)
{
	return NULL;
}

CD3DSubMeshMaterial * CD3DObject::GetSubMeshMaterial(int index)
{
	CD3DSubMesh * pSubMesh=GetSubMesh(index);
	if(pSubMesh)
	{
		return &(pSubMesh->GetMaterial());
	}
	return NULL;
}

CD3DBoundingBox * CD3DObject::GetBoundingBox()
{
	return NULL;
}

CD3DBoundingSphere * CD3DObject::GetBoundingSphere()
{
	return NULL;
}

//CD3DBoundingBox * CD3DObject::GetWholeBoundingBox()
//{
//	return NULL;
//}
//
//CD3DBoundingSphere * CD3DObject::GetWholeBoundingSphere()
//{
//	return NULL;
//}

CD3DLight * CD3DObject::GetLight(int Index)
{
	return NULL;
}

CD3DLight ** CD3DObject::GetLights()
{
	return NULL;
}

void CD3DObject::PrepareRender(CD3DDevice * pDevice,CD3DSubMesh * pSubMesh,CD3DSubMeshMaterial * pMaterial,CEasyArray<CD3DLight *>& LightList,CD3DCamera * pCamera)
{
	if(pSubMesh&&pMaterial)
	{	

		//设置材质

		//如果是被选中的使用特殊材质
		if(pSubMesh->IsSelected())
			pDevice->GetD3DDevice()->SetMaterial(&SELECTED_SUBMESH_MATERIAL);
		else
			pDevice->GetD3DDevice()->SetMaterial(&(pMaterial->GetMaterial()));
		
		//设置纹理
		if(pMaterial->GetFX())
		{			
			pMaterial->GetFX()->SetTexture("TexLay0",pMaterial->GetTexture(0));
			//pMaterial->GetFX()->SetTexture("TexLay1",pMaterial->GetTexture(1));
		}	
	}
	else
	{		
		//设置变换矩阵
		pDevice->GetD3DDevice()->SetTransform(D3DTS_WORLD,&GetWorldMatrix());
	}
}


void CD3DObject::Update(FLOAT Time)
{
	FUNCTION_BEGIN;
	//更新世界矩阵
	if(GetParent())
		m_WorldMatrix=m_LocalMatrix*GetParent()->GetWorldMatrix();
	else
		m_WorldMatrix=m_LocalMatrix;
	FUNCTION_END;

	//更新子对象
	for(UINT i=0;i<GetChildCount();i++)
		GetChildByIndex(i)->Update(Time);
}


void CD3DObject::SetVisible(bool IsVisible)
{
	if(IsVisible)
	{
		m_Flag|=OBJECT_FLAG_VISIBLE;
	}
	else
	{
		m_Flag&=~OBJECT_FLAG_VISIBLE;
	}
	
}

void CD3DObject::SetVisibleRecursive(bool IsVisible)
{
	SetVisible(IsVisible);
	for(UINT i=0;i<GetChildCount();i++)
	{
		GetChildByIndex(i)->SetVisibleRecursive(IsVisible);
	}
}

bool CD3DObject::IsVisible()
{
	return (m_Flag&OBJECT_FLAG_VISIBLE)!=0;
}

bool CD3DObject::RayIntersect(const CD3DVector3& Point,const CD3DVector3& Dir,CD3DVector3& IntersectPoint,FLOAT& Distance,bool TestOnly)
{
	if(GetBoundingBox())
	{
		CD3DBoundingBox BBox;


		BBox=(*GetBoundingBox())*GetWorldMatrix();

		if(!BBox.RayIntersect(Point,Dir,IntersectPoint,Distance))
			return false;
	}

	bool IsIntersect=false;

	Distance=3.4E+38f;
	FLOAT Dis;
	CD3DVector3 InterPoint;

	for(int i=0;i<GetSubMeshCount();i++)
	{
		CD3DSubMesh * pSubMesh=GetSubMesh(i);
		if(pSubMesh)
		{
			if(pSubMesh->RayIntersect(GetWorldMatrix(),Point,Dir,InterPoint,Dis,TestOnly))
			{
				IsIntersect=true;
				if(Dis<Distance)
				{
					Distance=Dis;
					IntersectPoint=InterPoint;
				}
				if(TestOnly)
				{
					break;
				}
			}
		}
	}
	return IsIntersect;
}

//bool CD3DObject::GetHeightByXZ(FLOAT x,FLOAT z,FLOAT& y)
//{
//	CD3DBoundingBox Box=(*GetBoundingBox())*GetWorldMatrix();
//
//	if(x<Box.m_Min.x||x>Box.m_Max.x||z<Box.m_Min.z||z>Box.m_Max.z)
//		return false;
//	CD3DVector3 Point(x,MAX_HEIGHT,z);
//	CD3DVector3 Dir(0,-1,0);
//	CD3DVector3 IntersectPoint;
//	FLOAT Distance;
//	FLOAT Height=-MAX_HEIGHT;
//
//	for(int i=0;i<GetSubMeshCount();i++)
//	{
//		CD3DSubMesh * pSubMesh=GetSubMesh(i);
//		if(pSubMesh)
//		{
//			if(pSubMesh->RayIntersect(GetWorldMatrix(),Point,Dir,IntersectPoint,Distance,false))
//			{
//				if(IntersectPoint.y>Height)					
//					Height=IntersectPoint.y;				
//			}			
//		}
//	}
//	if(Height>-MAX_HEIGHT)
//	{
//		y=Height;
//		return true;
//	}
//	return false;
//}

CD3DObject * CD3DObject::PickObject(CD3DVector3 Point,CD3DVector3 Dir,FLOAT& Distance)
{
	Distance=MAX_HEIGHT;
	CD3DObject * pObject=NULL;
	CD3DVector3 IntersectPoint;
	FLOAT ObjDis;

	if(RayIntersect(Point,Dir,IntersectPoint,ObjDis,false))
	{
		Distance=ObjDis;
		pObject=this;
	}

	for(UINT i=0;i<GetChildCount();i++)
	{
		if((!GetChildByIndex(i)->IsVisible())||(!GetChildByIndex(i)->CheckFlag(CD3DObject::OBJECT_FLAG_RENDERED)))
			continue;

		CD3DObject * pObj=GetChildByIndex(i)->PickObject(Point,Dir,ObjDis);		
		if(pObj&&ObjDis<Distance)
		{
			Distance=ObjDis;
			pObject=pObj;
		}
		
	}
	
	return pObject;
}

void CD3DObject::ShowBoundingFrame(int Operator)
{
	switch(Operator)
	{
	case DBFO_HIDE:
		{
			if(m_pBoundingFrame)
				m_pBoundingFrame->SetVisible(false);
		}
		break;
	case DBFO_SHOW:
		{
			if(m_pBoundingFrame)
				m_pBoundingFrame->SetVisible(true);
			else
			{
				if(GetRender()==NULL)
					return;
				if(GetBoundingBox()==NULL)
					return;
				m_pBoundingFrame=new CD3DBoundingFrame();
				m_pBoundingFrame->SetRender(GetRender());
				m_pBoundingFrame->CreateFromBBox(*GetBoundingBox());
				m_pBoundingFrame->SetParent(this);
				GetRender()->AddObject(m_pBoundingFrame);
			}
		}
		break;
	case DBFO_RELEASE:
		SAFE_RELEASE(m_pBoundingFrame);
		break;
	}
}

void CD3DObject::UpdateBoundingFrame()
{
	if(m_pBoundingFrame)
	{
		m_pBoundingFrame->CreateFromBBox(*GetBoundingBox());
	}
}


bool CD3DObject::CloneFrom(CNameObject * pObject,UINT Param)
{
	if(!pObject->IsKindOf(GET_CLASS_INFO(CD3DObject)))
		return false;

	if(!CTreeObject::CloneFrom(pObject,Param))
		return false;	

	CD3DObject * pSource=(CD3DObject *)pObject;

	m_pRender=pSource->m_pRender;		
	m_Layer=pSource->m_Layer;		
	m_LocalMatrix=pSource->m_LocalMatrix;
	m_WorldMatrix=pSource->m_WorldMatrix;		
	m_Flag=pSource->m_Flag;		
	
	return true;
}

void CD3DObject::PickResource(CNameObjectSet * pObjectSet,UINT Param)
{
	for(int i=0;i<GetSubMeshCount();i++)
	{
		CD3DSubMesh * pSubMesh=GetSubMesh(i);
		if(pSubMesh)
		{
			pSubMesh->PickResource(pObjectSet,Param);
		}
	}
	for(UINT i=0;i<GetChildCount();i++)
	{
		GetChildByIndex(i)->PickResource(pObjectSet,Param);
	}
}

bool CD3DObject::ToSmartStruct(CSmartStruct& Packet,CUSOFile * pUSOFile,UINT Param)
{
	if(!CTreeObject::ToSmartStruct(Packet,pUSOFile,Param))
		return false;	

	CHECK_SMART_STRUCT_ADD_AND_RETURN(Packet.AddMember(SST_D3DO_LOCAL_MATIRX,(char *)&m_LocalMatrix,sizeof(m_LocalMatrix)));	
	CHECK_SMART_STRUCT_ADD_AND_RETURN(Packet.AddMember(SST_D3DO_IS_FLAG,m_Flag));	

	return true;
}

bool CD3DObject::FromSmartStruct(CSmartStruct& Packet,CUSOFile * pUSOFile,UINT Param)
{
	if(!CTreeObject::FromSmartStruct(Packet,pUSOFile,Param))
		return false;

	void * Pos=Packet.GetFirstMemberPosition();
	while(Pos)
	{
		WORD MemberID;
		CSmartValue Value=Packet.GetNextMember(Pos,MemberID);
		switch(MemberID)
		{
		case SST_D3DO_LOCAL_MATIRX:
			memcpy(&m_LocalMatrix,(LPCTSTR)Value,sizeof(m_LocalMatrix));
			break;
		case SST_D3DO_IS_FLAG:
			m_Flag=Value;
			break;
		}
	}
	return true;
}

UINT CD3DObject::GetSmartStructSize(UINT Param)
{
	SAFE_RELEASE(m_pBoundingFrame);

	UINT Size=CTreeObject::GetSmartStructSize(Param);
	Size+=SMART_STRUCT_STRING_MEMBER_SIZE(sizeof(m_LocalMatrix));
	Size+=SMART_STRUCT_FIX_MEMBER_SIZE(sizeof(m_Flag));
	return Size;
}



}