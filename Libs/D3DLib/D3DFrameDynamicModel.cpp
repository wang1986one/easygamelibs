#include "StdAfx.h"
#include ".\d3dframedynamicmodel.h"

namespace D3DLib{

IMPLEMENT_CLASS_INFO(CD3DFrameDynamicModel,CD3DBaseDynamicModel);

IMPLEMENT_FILE_CHANNEL_MANAGER(CD3DFrameDynamicModel);

CD3DFrameDynamicModel::CD3DFrameDynamicModel(void):CD3DBaseDynamicModel()
{
	m_CurFrameIndex=0;
	m_PlayStatus=PLAY_STOP;
	m_PlayStartTime=-1.0f;
	m_TotalFrameTime=0.0f;
}

CD3DFrameDynamicModel::~CD3DFrameDynamicModel(void)
{
	Destory();
}

void CD3DFrameDynamicModel::Destory()
{
	for(int i=0;i<(int)m_AniFrames.GetCount();i++)
	{
		SAFE_RELEASE(m_AniFrames[i].pModelResource);
	}
	m_AniFrames.Clear();
	m_CurFrameIndex=0;
	m_PlayStatus=PLAY_STOP;
	m_PlayStartTime=-1.0f;
	m_TotalFrameTime=0.0f;
	CD3DBaseDynamicModel::Destory();
}

bool CD3DFrameDynamicModel::Reset()
{
	bool Ret=true;
	for(int i=0;i<(int)m_AniFrames.GetCount();i++)
	{
		Ret=Ret&&m_AniFrames[i].pModelResource->Reset();
	}	
	return Ret&&CD3DBaseDynamicModel::Reset();
}

bool CD3DFrameDynamicModel::Restore()
{
	bool Ret=true;
	for(int i=0;i<(int)m_AniFrames.GetCount();i++)
	{
		Ret=Ret&&m_AniFrames[i].pModelResource->Restore();
	}	
	return Ret&&CD3DBaseDynamicModel::Restore();
}

bool CD3DFrameDynamicModel::Play(bool IsLoop)
{
	if(IsLoop)
		m_PlayStatus=PLAY_LOOP;
	else
		m_PlayStatus=PLAY_ONCE;
	m_PlayStartTime=-1.0f;
	return true;
}
bool CD3DFrameDynamicModel::Stop()
{
	m_PlayStatus=PLAY_STOP;
	m_CurFrameIndex=0;
	m_PlayStartTime=-1.0f;
	return true;
}

bool CD3DFrameDynamicModel::IsPlaying()
{
	return m_PlayStatus!=PLAY_STOP;
}

bool CD3DFrameDynamicModel::LoadFromMDA(LPCTSTR FileName)
{
	IFileAccessor * pFile;

	if(GetDevice()==NULL)
		return false;

	Destory();

	pFile=CFileSystemManager::GetInstance()->CreateFileAccessor(m_FileChannel);
	if(pFile==NULL)
		return false;
	if(!pFile->Open(FileName,IFileAccessor::modeRead))
	{
		pFile->Release();
		return false;	
	}
	int FrameCount;
	pFile->Read(&FrameCount,sizeof(int));
	for(int i=0;i<FrameCount;i++)
	{
		FRAME_DATA	Frame;
		char		FrameName[MAX_PATH];

		Frame.StartTime=i*1.0f/24;
		Frame.EndTime=(i+1)*1.0f/24;
		Frame.pModelResource=NULL;

		sprintf_s(FrameName,MAX_PATH,"%s-%d",FileName,i);

		CD3DStaticModelResource * pResource=(CD3DStaticModelResource *)
			GetDevice()->GetObjectResourceManager()->
			GetResource(FrameName);
		if(pResource==NULL)
		{
			pResource=new CD3DStaticModelResource(GetDevice()->GetObjectResourceManager());
			if(!pResource->LoadFromMDA(pFile))
			{
				SAFE_RELEASE(pResource);
				continue;
			}
			GetDevice()->GetObjectResourceManager()->AddResource(pResource,FrameName);
		}
		else
		{
			pResource->AddUseRef();
		}
		Frame.pModelResource=pResource;
		m_TotalFrameTime=Frame.EndTime;
		m_AniFrames.Add(Frame);
	}
	
	pFile->Release();
	BulidBouding();
	return true;
}



//bool CD3DFrameDynamicModel::ToUSOFile(CUSOFile * pUSOFile,UINT Param)
//{
//	if(pUSOFile==NULL)
//		return false;	
//
//	IFileAccessor * pFile=pUSOFile->GetFile();
//	if(pFile==NULL)
//		return false;
//
//	SAFE_RELEASE(m_pBoundingFrame);
//
//
//	STORAGE_STRUCT Data;
//
//	strncpy_0(Data.ObjectHead.Type,USO_FILE_MAX_TYPE_LEN,GetClassInfo().ClassName,USO_FILE_MAX_TYPE_LEN);	
//	strncpy_0(Data.ObjectHead.Name,USO_FILE_MAX_OBJECT_NAME,GetName(),USO_FILE_MAX_OBJECT_NAME);
//	Data.ObjectHead.StorageID=GetStorageID();
//
//	Data.LocalMatrix=GetLocalMatrix();
//	Data.IsVisible=IsVisible();
//	Data.ChildCount=(int)m_ChildList.GetCount();
//	Data.FrameCount=(int)m_AniFrames.GetCount();
//	Data.PlayStatus=m_PlayStatus;
//	Data.ObjectHead.Size=sizeof(STORAGE_STRUCT)+Data.FrameCount*sizeof(STORAGE_FRAME_STRUCT);
//
//	pFile->Write(&Data,sizeof(STORAGE_STRUCT));
//
//	for(int i=0;i<Data.FrameCount;i++)
//	{
//		STORAGE_FRAME_STRUCT Frame;
//		Frame.StartTime=m_AniFrames[i].StartTime;
//		Frame.EndTime=m_AniFrames[i].EndTime;
//		Frame.ResourceID=pUSOFile->ResourceObjectToIndex(m_AniFrames[i].pModelResource);
//		pFile->Write(&Frame,sizeof(Frame));
//	}
//
//	for(int i=0;i<(int)m_ChildList.GetCount();i++)
//	{
//		m_ChildList[i]->ToUSOFile(pUSOFile,Param);
//	}
//
//	return true;
//}
//bool CD3DFrameDynamicModel::FromUSOFile(CUSOFile * pUSOFile,UINT Param)
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
//
//	Destory();	
//
//	pData->ObjectHead.Name[USO_FILE_MAX_OBJECT_NAME-1]=0;
//	SetName(pData->ObjectHead.Name);
//	SetStorageID(pData->ObjectHead.StorageID);
//	SetLocalMatrix(pData->LocalMatrix);
//	SetVisible(pData->IsVisible);
//	m_PlayStatus=pData->PlayStatus;
//
//	BYTE * pCurData=pBuff+sizeof(STORAGE_STRUCT);
//
//	STORAGE_FRAME_STRUCT * pFrame=(STORAGE_FRAME_STRUCT *)pCurData;
//	m_AniFrames.resize(pData->FrameCount);
//	for(int i=0;i<pData->FrameCount;i++)
//	{
//		m_AniFrames[i].StartTime=pFrame[i].StartTime;
//		m_AniFrames[i].EndTime=pFrame[i].EndTime;
//		m_AniFrames[i].pModelResource=(CD3DStaticModelResource*)pUSOFile->ResourceIndexToObject(pFrame[i].ResourceID);
//		if(m_AniFrames[i].pModelResource)
//		{			
//			if(m_AniFrames[i].pModelResource->IsKindOf(GET_CLASS_INFO(CD3DStaticModelResource)))
//				m_AniFrames[i].pModelResource->AddUseRef();
//			else
//				m_AniFrames[i].pModelResource=NULL;
//		}
//		m_TotalFrameTime=pFrame[i].EndTime;
//	}	
//	delete[] pBuff;
//	BulidBouding();
//	return true;
//}

void CD3DFrameDynamicModel::PickResource(CNameObjectSet * pObjectSet,UINT Param)
{
	for(int i=0;i<(int)m_AniFrames.GetCount();i++)
	{
		m_AniFrames[i].pModelResource->PickResource(pObjectSet,Param);
		pObjectSet->Add(m_AniFrames[i].pModelResource);		
	}
	for(UINT i=0;i<GetChildCount();i++)
	{
		GetChildByIndex(i)->PickResource(pObjectSet,Param);
	}
}

bool CD3DFrameDynamicModel::CloneFrom(CNameObject * pObject,UINT Param)
{
	if(!pObject->IsKindOf(GET_CLASS_INFO(CD3DFrameDynamicModel)))
		return false;

	Destory();	

	if(!CD3DObject::CloneFrom(pObject,Param))
		return false;

	CD3DFrameDynamicModel * pModel=(CD3DFrameDynamicModel *)pObject;
	for(size_t i=0;i<pModel->m_AniFrames.GetCount();i++)
	{
		pModel->m_AniFrames[i].pModelResource->AddUseRef();
		m_AniFrames.Add(pModel->m_AniFrames[i]);
	}
	m_CurFrameIndex=pModel->m_CurFrameIndex;
	m_PlayStatus=pModel->m_PlayStatus;
	m_PlayStartTime=pModel->m_PlayStartTime;
	m_TotalFrameTime=pModel->m_TotalFrameTime;
	m_BoundingBox=pModel->m_BoundingBox;
	m_BoundingSphere=pModel->m_BoundingSphere;

	return true;

}

int CD3DFrameDynamicModel::GetSubMeshCount()
{
	if(m_AniFrames.GetCount()>0)
	{
		return m_AniFrames[m_CurFrameIndex].pModelResource->GetSubMeshCount();
	}
	return 0;
}
CD3DSubMesh * CD3DFrameDynamicModel::GetSubMesh(int index)
{
	if(m_AniFrames.GetCount()>0)
	{
		return m_AniFrames[m_CurFrameIndex].pModelResource->GetSubMesh(index);
	}
	return NULL;
}

CD3DBoundingBox * CD3DFrameDynamicModel::GetBoundingBox()
{	
	return &m_BoundingBox;
}
CD3DBoundingSphere * CD3DFrameDynamicModel::GetBoundingSphere()
{	
	return &m_BoundingSphere;
}

bool CD3DFrameDynamicModel::RayIntersect(const CD3DVector3& Point,const CD3DVector3& Dir,CD3DVector3& IntersectPoint,FLOAT& Distance,bool TestOnly=true)
{	
	if(m_AniFrames.GetCount()<=0)
		return false;

	CD3DStaticModelResource* pResource=m_AniFrames[m_CurFrameIndex].pModelResource;	

	CD3DBoundingBox BBox;

	BBox=(*GetBoundingBox())*GetWorldMatrix();

	if(!BBox.RayIntersect(Point,Dir,IntersectPoint,Distance))
		return false;


	for(int i=0;i<pResource->GetSubMeshCount();i++)
	{
		CD3DSubMesh * pSubMesh=pResource->GetSubMesh(i);
		if(pSubMesh)
		{
			if(pSubMesh->RayIntersect(GetWorldMatrix(),Point,Dir,IntersectPoint,Distance,false))
				return true;
		}
	}
	return false;
}

bool CD3DFrameDynamicModel::GetHeightByXZ(FLOAT x,FLOAT z,FLOAT& y)
{	
	if(m_AniFrames.GetCount()<=0)
		return false;

	CD3DStaticModelResource* pResource=m_AniFrames[m_CurFrameIndex].pModelResource;


	CD3DBoundingBox Box=(*GetBoundingBox())*GetWorldMatrix();

	if(x<Box.m_Min.x||x>Box.m_Max.x||z<Box.m_Min.z||z>Box.m_Max.z)
		return false;
	CD3DVector3 Point(x,MAX_HEIGHT,z);
	CD3DVector3 Dir(0,-1,0);
	CD3DVector3 IntersectPoint;
	FLOAT Distance;
	FLOAT Height=-MAX_HEIGHT;

	for(int i=0;i<pResource->GetSubMeshCount();i++)
	{
		CD3DSubMesh * pSubMesh=pResource->GetSubMesh(i);
		if(pSubMesh)
		{
			if(pSubMesh->RayIntersect(GetWorldMatrix(),Point,Dir,IntersectPoint,Distance,false))
			{
				if(IntersectPoint.y>Height)					
					Height=IntersectPoint.y;		
			}		
		}
	}
	if(Height>-MAX_HEIGHT)
	{
		y=Height;
		return true;
	}
	return false;
}

void CD3DFrameDynamicModel::Update(FLOAT Time)
{
	FUNCTION_BEGIN;
	if(m_PlayStatus!=PLAY_STOP)
	{
		if(m_PlayStartTime<0)
			m_PlayStartTime=Time;
		float PastTime=Time-m_PlayStartTime;
		if(PastTime>m_TotalFrameTime&&m_PlayStatus!=PLAY_LOOP)
		{
			Stop();
			return;
		}		
		m_CurFrameIndex=0;
		if(m_AniFrames.GetCount()>1)
		{
			while(PastTime>m_TotalFrameTime)
			{
				PastTime-=m_TotalFrameTime;
			}
			while(PastTime>m_AniFrames[m_CurFrameIndex].EndTime)
			{
				m_CurFrameIndex++;			
			}
		}

	}	
	FUNCTION_END;
	CD3DBaseDynamicModel::Update(Time);	
}

int CD3DFrameDynamicModel::GetFrameCount()
{
	return (int)m_AniFrames.GetCount();
}
CD3DStaticModelResource* CD3DFrameDynamicModel::GetFrameModelResource(int Index)
{
	if(Index>=0&&Index<(int)m_AniFrames.GetCount())
	{
		return m_AniFrames[Index].pModelResource;
	}
	return NULL;
}


bool CD3DFrameDynamicModel::ToSmartStruct(CSmartStruct& Packet,CUSOFile * pUSOFile,UINT Param)
{
	SAFE_RELEASE(m_pBoundingFrame);

	if(!CD3DObject::ToSmartStruct(Packet,pUSOFile,Param))
		return false;	

	for(UINT i=0;i<m_AniFrames.GetCount();i++)
	{
		UINT BufferSize;
		void * pBuffer=Packet.PrepareMember(BufferSize);
		CSmartStruct SubPacket(pBuffer,BufferSize,true);

		CHECK_SMART_STRUCT_ADD_AND_RETURN(SubPacket.AddMember(SST_FI_START_TIME,m_AniFrames[i].StartTime));	
		CHECK_SMART_STRUCT_ADD_AND_RETURN(SubPacket.AddMember(SST_FI_END_TIME,m_AniFrames[i].EndTime));	
		int ResourceID=pUSOFile->ResourceObjectToIndex(m_AniFrames[i].pModelResource);
		CHECK_SMART_STRUCT_ADD_AND_RETURN(SubPacket.AddMember(SST_FI_MODEL_RESOURCE,ResourceID));	

		if(!Packet.FinishMember(SST_D3DFDM_FRAME_INFO,SubPacket.GetDataLen()))
			return false;
	}

	CHECK_SMART_STRUCT_ADD_AND_RETURN(Packet.AddMember(SST_D3DFDM_BOUDING_BOX,(char *)&m_BoundingBox,sizeof(m_BoundingBox)));	
	CHECK_SMART_STRUCT_ADD_AND_RETURN(Packet.AddMember(SST_D3DFDM_BOUDING_SPHERE,(char *)&m_BoundingSphere,sizeof(m_BoundingSphere)));	
	CHECK_SMART_STRUCT_ADD_AND_RETURN(Packet.AddMember(SST_D3DFDM_PLAY_STATUS,m_PlayStatus));	

	return true;
}

bool CD3DFrameDynamicModel::FromSmartStruct(CSmartStruct& Packet,CUSOFile * pUSOFile,UINT Param)
{
	if(!CD3DObject::FromSmartStruct(Packet,pUSOFile,Param))
		return false;

	void * Pos=Packet.GetFirstMemberPosition();
	int PartIndex=0;
	while(Pos)
	{
		WORD MemberID;
		CSmartValue Value=Packet.GetNextMember(Pos,MemberID);
		switch(MemberID)
		{	
		case SST_D3DFDM_FRAME_INFO:
			{
				CSmartStruct SubPacket=Value;
				FRAME_DATA Frame;
				Frame.StartTime=SubPacket.GetMember(SST_FI_START_TIME);
				Frame.EndTime=SubPacket.GetMember(SST_FI_END_TIME);
				int ResourceID=SubPacket.GetMember(SST_FI_MODEL_RESOURCE);
				Frame.pModelResource=(CD3DStaticModelResource *)pUSOFile->ResourceIndexToObject(ResourceID,GET_CLASS_INFO(CD3DStaticModelResource));
				if(Frame.pModelResource)
				{
					Frame.pModelResource->AddUseRef();
					m_AniFrames.Add(Frame);
				}
				m_TotalFrameTime=Frame.EndTime;
			}
			break;
		case SST_D3DFDM_BOUDING_BOX:
			memcpy(&m_BoundingBox,(LPCTSTR)Value,sizeof(m_BoundingBox));
			break;
		case SST_D3DFDM_BOUDING_SPHERE:
			memcpy(&m_BoundingSphere,(LPCTSTR)Value,sizeof(m_BoundingSphere));
			break;
		case SST_D3DFDM_PLAY_STATUS:
			m_PlayStatus=Value;
			break;
		}
	}
	return true;
}

UINT CD3DFrameDynamicModel::GetSmartStructSize(UINT Param)
{
	UINT Size=CD3DObject::GetSmartStructSize(Param);

	for(UINT i=0;i<m_AniFrames.GetCount();i++)
	{
		Size+=SMART_STRUCT_FIX_MEMBER_SIZE(sizeof(m_AniFrames[i].StartTime));
		Size+=SMART_STRUCT_FIX_MEMBER_SIZE(sizeof(m_AniFrames[i].EndTime));
		Size+=SMART_STRUCT_FIX_MEMBER_SIZE(sizeof(int));
		Size+=SMART_STRUCT_STRUCT_MEMBER_SIZE(0);
	}
	Size+=SMART_STRUCT_STRING_MEMBER_SIZE(sizeof(m_BoundingBox));
	Size+=SMART_STRUCT_STRING_MEMBER_SIZE(sizeof(m_BoundingSphere));
	Size+=SMART_STRUCT_FIX_MEMBER_SIZE(sizeof(m_PlayStatus));
	return Size;
}

void CD3DFrameDynamicModel::BulidBouding()
{
	if(m_AniFrames.GetCount()>0)
	{
		m_BoundingBox=*m_AniFrames[0].pModelResource->GetBoundingBox();
		m_BoundingSphere=*m_AniFrames[0].pModelResource->GetBoundingSphere();
		for(int i=1;i<(int)m_AniFrames.GetCount();i++)
		{
			m_BoundingBox+=*m_AniFrames[i].pModelResource->GetBoundingBox();
			m_BoundingSphere+=*m_AniFrames[i].pModelResource->GetBoundingSphere();
		}
	}
}


//CNameObject::STORAGE_STRUCT * CD3DFrameDynamicModel::USOCreateHead(UINT Param)
//{
//	STORAGE_STRUCT * pHead=new STORAGE_STRUCT;
//	ZeroMemory(pHead,sizeof(STORAGE_STRUCT));
//	pHead->Size=sizeof(STORAGE_STRUCT);
//	return pHead;
//}
//int CD3DFrameDynamicModel::USOWriteHead(CNameObject::STORAGE_STRUCT * pHead,CUSOFile * pUSOFile,UINT Param)
//{
//	int HeadSize=CD3DObject::USOWriteHead(pHead,pUSOFile,Param);
//	if(HeadSize<0)
//		return -1;
//
//	STORAGE_STRUCT * pLocalHead=(STORAGE_STRUCT *)pHead;
//
//	pLocalHead->Size+=m_AniFrames.GetCount()*sizeof(STORAGE_FRAME_STRUCT);
//
//	pLocalHead->FrameCount=(int)m_AniFrames.GetCount();
//	pLocalHead->PlayStatus=m_PlayStatus;
//	
//
//	return sizeof(STORAGE_STRUCT);
//}
//
//bool CD3DFrameDynamicModel::USOWriteData(CNameObject::STORAGE_STRUCT * pHead,CUSOFile * pUSOFile,UINT Param)
//{
//	if(!CD3DObject::USOWriteData(pHead,pUSOFile,Param))
//		return false;
//
//	if(pUSOFile==NULL)
//		return false;	
//
//	IFileAccessor * pFile=pUSOFile->GetFile();
//	if(pFile==NULL)
//		return false;
//
//	for(size_t i=0;i<m_AniFrames.GetCount();i++)
//	{
//		STORAGE_FRAME_STRUCT Frame;
//		Frame.StartTime=m_AniFrames[i].StartTime;
//		Frame.EndTime=m_AniFrames[i].EndTime;
//		Frame.ResourceID=pUSOFile->ResourceObjectToIndex(m_AniFrames[i].pModelResource);
//		if(pFile->Write(&Frame,sizeof(Frame))<sizeof(Frame))
//			return false;
//	}	
//
//	return true;
//}
//
//int CD3DFrameDynamicModel::USOReadHead(CNameObject::STORAGE_STRUCT * pHead,CUSOFile * pUSOFile,UINT Param)
//{
//	int ReadSize=CD3DObject::USOReadHead(pHead,pUSOFile,Param);
//	if(ReadSize<0)
//		return -1;
//
//	STORAGE_STRUCT * pLocalHead=(STORAGE_STRUCT *)pHead;
//
//	m_PlayStatus=pLocalHead->PlayStatus;
//
//	return sizeof(STORAGE_STRUCT);
//}
//
//int CD3DFrameDynamicModel::USOReadData(CNameObject::STORAGE_STRUCT * pHead,CUSOFile * pUSOFile,BYTE * pData,int DataSize,UINT Param)
//{
//	int ReadSize=CD3DObject::USOReadData(pHead,pUSOFile,pData,DataSize,Param);
//
//	pData+=ReadSize;
//	DataSize-=ReadSize;
//
//	STORAGE_STRUCT * pLocalHead=(STORAGE_STRUCT *)pHead;
//
//	STORAGE_FRAME_STRUCT * pFrame=(STORAGE_FRAME_STRUCT *)pData;
//	ReadSize+=sizeof(STORAGE_FRAME_STRUCT)*pLocalHead->FrameCount;
//	m_AniFrames.Resize(pLocalHead->FrameCount);
//	for(int i=0;i<pLocalHead->FrameCount;i++)
//	{
//		m_AniFrames[i].StartTime=pFrame[i].StartTime;
//		m_AniFrames[i].EndTime=pFrame[i].EndTime;
//		m_AniFrames[i].pModelResource=(CD3DStaticModelResource*)pUSOFile->ResourceIndexToObject(pFrame[i].ResourceID,GET_CLASS_INFO(CD3DStaticModelResource));
//		if(m_AniFrames[i].pModelResource)
//		{			
//			if(m_AniFrames[i].pModelResource->IsKindOf(GET_CLASS_INFO(CD3DStaticModelResource)))
//				m_AniFrames[i].pModelResource->AddUseRef();
//			else
//				m_AniFrames[i].pModelResource=NULL;
//		}
//		
//		m_TotalFrameTime=pFrame[i].EndTime;
//	}	
//
//	return ReadSize;
//}
//
//bool CD3DFrameDynamicModel::USOReadFinish(CNameObject::STORAGE_STRUCT * pHead,UINT Param)
//{
//	if(!CNameObject::USOReadFinish(pHead,Param))
//		return false;
//	BulidBouding();
//	return true;
//}

}