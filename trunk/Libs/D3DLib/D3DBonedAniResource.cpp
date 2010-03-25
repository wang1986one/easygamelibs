/****************************************************************************/
/*                                                                          */
/*      文件名:    D3DBonedAniResource.cpp                                  */
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


#define	SYS_FLAG			(*(int*)"ORB ")			// SYS 系统文件标志
#define SYS_VERSION			0x00010000
#define	SKELETON_FLAG		(*(int*)"BONE")			// 骨骼文件标志
#define SKELETON_VERSION	0x00010002
#define	ACTION_FLAG			(*(int*)"ACTN")			// 动化文件标志
#define ACTION_VERSION		0x00010002
#define	SKIN_FLAG			(*(int*)"SKIN")			// 皮肤文件标志
#define SKIN_VERSION		0x00010003
#define	MODEL_FLAG			(*(int*)"MDL ")			// 刚体文件标志
#define MODEL_VERSION		0x00010002

struct FILE_HEADER
{
	DWORD	MainFlag;
	DWORD	MainVersion;
	DWORD	SubFlag;
	DWORD	SubVersion;
};

struct DATA_BLOCK_HEADER
{
	int Offset;
	int Length;
};

enum ACT_BLOCK_TYPE
{
	ABT_INFO,
	ABT_POS,
	ABT_QUAT,
	ABT_MAX,
};

struct ACT_FRAME_INFO
{
	int nFrame;
	int nFps;
	int nBones;
};

IMPLEMENT_FILE_CHANNEL_MANAGER(CD3DBonedAniResource)

IMPLEMENT_CLASS_INFO(CD3DBonedAniResource,CD3DObjectResource);

CD3DBonedAniResource::CD3DBonedAniResource()
	:CD3DObjectResource()
{	

}

CD3DBonedAniResource::CD3DBonedAniResource(CD3DObjectResourceManager* pManager)
	:CD3DObjectResource(pManager)
{	
	
}

CD3DBonedAniResource::~CD3DBonedAniResource(void)
{
	Destory();
}

void CD3DBonedAniResource::Destory()
{
	m_Frames.Clear();
	CD3DObjectResource::Destory();
}

bool CD3DBonedAniResource::LoadFromACT(LPCTSTR FileName,bool Reverse)
{
	IFileAccessor * pFile;

	Destory();

	pFile=CFileSystemManager::GetInstance()->CreateFileAccessor(m_FileChannel);
	if(pFile==NULL)
		return false;
	if(!pFile->Open(FileName,IFileAccessor::modeRead))
	{
		pFile->Release();
		return false;	
	}
	int FileSize=(int)pFile->GetSize();	
	BYTE * pData=new BYTE[FileSize];
	pFile->Read(pData,FileSize);
	pFile->Release();

	BYTE * DataReadPtr=pData;
	FILE_HEADER * pHeader=(FILE_HEADER *)DataReadPtr;
	DataReadPtr+=sizeof(FILE_HEADER);

	//校验文件类型和版本号
	if( (pHeader->MainFlag != SYS_FLAG) || (pHeader->MainVersion != SYS_VERSION) ||
		(pHeader->SubFlag != ACTION_FLAG) || (pHeader->SubVersion != ACTION_VERSION))
	{
		delete[] pData;
		return false;
	}

	DATA_BLOCK_HEADER * pBlockHeaders=(DATA_BLOCK_HEADER *)DataReadPtr;

	ACT_FRAME_INFO *	pFrameInfo;
	CD3DVector3*		pPoss;
	CD3DQuaternion *	pQuaternions;

	int FrameCount;
	int PosCount;
	int QuaternionCount;
	

	pFrameInfo=(ACT_FRAME_INFO *)(DataReadPtr+pBlockHeaders[ABT_INFO].Offset);
	FrameCount=pBlockHeaders[ABT_INFO].Length/sizeof(ACT_FRAME_INFO);

	pPoss=(CD3DVector3 *)(DataReadPtr+pBlockHeaders[ABT_POS].Offset);
	PosCount=pBlockHeaders[ABT_POS].Length/sizeof(CD3DVector3);

	pQuaternions=(CD3DQuaternion *)(DataReadPtr+pBlockHeaders[ABT_QUAT].Offset);
	QuaternionCount=pBlockHeaders[ABT_QUAT].Length/sizeof(CD3DQuaternion);

	//int KeyFrameCount=pFrameInfo->nFrame;
	int KeyFrameCount=PosCount/pFrameInfo->nBones;
	m_Frames.Resize(KeyFrameCount);

	for(int i=0;i<KeyFrameCount;i++)
	{
		ANI_FRAME * pAniFrame;

		if(Reverse)
		{
			pAniFrame=&(m_Frames[KeyFrameCount-i-1]);
			pAniFrame->Time=(KeyFrameCount-i-1)*(1.0f/pFrameInfo->nFps);
		}
		else
		{
			pAniFrame=&(m_Frames[i]);
			pAniFrame->Time=i*(1.0f/pFrameInfo->nFps);
		}
		pAniFrame->BonesTrans.Resize(pFrameInfo->nBones);
		for(int j=0;j<pFrameInfo->nBones;j++)
		{
			pAniFrame->BonesTrans[j].Pos=pPoss[i*pFrameInfo->nBones+j];
			pAniFrame->BonesTrans[j].Rotation=pQuaternions[i*pFrameInfo->nBones+j];
			pAniFrame->BonesTrans[j].Scale=CD3DVector3(1.0f,1.0f,1.0f);
		}
	}

	delete[] pData;
	return true;
}

FLOAT CD3DBonedAniResource::GetAnimationLen()
{
	if(m_Frames.GetCount())
	{
		return m_Frames[m_Frames.GetCount()-1].Time;
	}
	return 0;
}

int CD3DBonedAniResource::GetBoneCount()
{
	if(m_Frames.GetCount())
	{
		return (int)m_Frames[0].BonesTrans.GetCount();
	}
	return 0;
}

bool CD3DBonedAniResource::GetBoneMatrix(FLOAT Time,int BoneID,CD3DMatrix& Mat)
{	
	FLOAT AniLen=GetAnimationLen();
	if(AniLen<=0)
		return false;

	while(Time>AniLen)
		Time-=AniLen;

	int Frame=0;
	while(Frame<(int)m_Frames.GetCount()&&Time>m_Frames[Frame].Time)
	{
		Frame++;
	}	

	BONE_TRANS CurFrame;
	if(Frame==m_Frames.GetCount())
	{
		if(BoneID<0||BoneID>=(int)m_Frames[Frame].BonesTrans.GetCount())
			return false;
		BONE_TRANS& KeyFrame=m_Frames[Frame].BonesTrans[BoneID];
		CurFrame=KeyFrame;		
	}
	else
	{
		if(BoneID<0||BoneID>=(int)m_Frames[Frame].BonesTrans.GetCount())
			return false;

		BONE_TRANS& KeyFrame1=m_Frames[Frame].BonesTrans[BoneID];
		BONE_TRANS& KeyFrame2=m_Frames[Frame+1].BonesTrans[BoneID];

		FLOAT S=Time-m_Frames[Frame].Time;
		CurFrame.Pos=CD3DVector3::Lerp(KeyFrame1.Pos,KeyFrame2.Pos,S);
		CurFrame.Rotation=CD3DQuaternion::SLerp(KeyFrame1.Rotation,KeyFrame2.Rotation,S);
		CurFrame.Scale=CD3DVector3::Lerp(KeyFrame1.Scale,KeyFrame2.Scale,S);
	}
	Mat=CD3DMatrix::FromScale(CurFrame.Scale)*CD3DMatrix::FromRotationQuaternion(CurFrame.Rotation);
	Mat.SetTranslation(CurFrame.Pos);
	return true;
}

int CD3DBonedAniResource::GetBoneFrame(FLOAT Time,CD3DMatrix * pMats,BONE_TRANS * pTrans)
{
	FLOAT AniLen=GetAnimationLen();
	if(AniLen<=0)
		return 0;

	while(Time>AniLen)
		Time-=AniLen;

	int Frame=0;
	while(Frame<(int)(m_Frames.GetCount()-1)&&Time>m_Frames[Frame+1].Time)
	{
		Frame++;
	}
	
	if(Frame>=(int)m_Frames.GetCount()-1)
	{
		Frame=(int)m_Frames.GetCount()-1;
		
		for(int i=0;i<(int)m_Frames[Frame].BonesTrans.GetCount();i++)
		{
			BONE_TRANS CurBone=m_Frames[Frame].BonesTrans[i];
			if(pTrans)
			{
				pTrans[i]=CurBone;
			}
			if(pMats)
			{
				pMats[i]=CD3DMatrix::FromScale(CurBone.Scale)*
					CD3DMatrix::FromRotationQuaternion(CurBone.Rotation);
				pMats[i].SetTranslation(CurBone.Pos);
			}
		}
		return (int)m_Frames[Frame].BonesTrans.GetCount();
	}
	else
	{
		FLOAT S=(Time-m_Frames[Frame].Time)/(m_Frames[Frame+1].Time-m_Frames[Frame].Time);
		for(int i=0;i<(int)m_Frames[Frame].BonesTrans.GetCount();i++)
		{
			BONE_TRANS& Bone1=m_Frames[Frame].BonesTrans[i];
			BONE_TRANS& Bone2=m_Frames[Frame+1].BonesTrans[i];
			BONE_TRANS CurBone;

						
			CurBone.Pos=CD3DVector3::Lerp(Bone1.Pos,Bone2.Pos,S);
			CurBone.Rotation=CD3DQuaternion::SLerp(Bone1.Rotation,Bone2.Rotation,S);
			CurBone.Scale=CD3DVector3::Lerp(Bone1.Scale,Bone2.Scale,S);
			if(pTrans)
			{
				pTrans[i]=CurBone;
			}
			if(pMats)
			{
				pMats[i]=CD3DMatrix::FromScale(CurBone.Scale)*
					CD3DMatrix::FromRotationQuaternion(CurBone.Rotation);
				pMats[i].SetTranslation(CurBone.Pos);
			}
		}
		return (int)m_Frames[Frame].BonesTrans.GetCount();
	}
	
}




//bool CD3DBonedAniResource::ToUSOFile(CUSOFile * pUSOFile,UINT Param)
//{	
//
//	if(pUSOFile==NULL)
//		return false;	
//
//	IFileAccessor * pFile=pUSOFile->GetFile();
//	if(pFile==NULL)
//		return false;
//
//	//	SAFE_RELEASE(m_pBoundingFrame);
//
//
//	STORAGE_STRUCT Data;
//
//	strncpy_0(Data.ObjectHead.Type,USO_FILE_MAX_TYPE_LEN,GetClassInfo().ClassName,USO_FILE_MAX_TYPE_LEN);		
//	strncpy_0(Data.ObjectHead.Name,USO_FILE_MAX_OBJECT_NAME,GetName(),USO_FILE_MAX_OBJECT_NAME);
//	
//
//	Data.ObjectHead.Size=sizeof(STORAGE_STRUCT);
//	Data.FrameCount=(int)m_Frames.GetCount();	
//	
//	for(int i=0;i<(int)m_Frames.GetCount();i++)
//	{
//		Data.ObjectHead.Size+=sizeof(STORAGE_FRAME_STRUCT);		
//		Data.ObjectHead.Size+=(int)m_Frames[i].BonesTrans.GetCount()*sizeof(BONE_TRANS);		
//	}	
//
//	pFile->Write(&Data,sizeof(STORAGE_STRUCT));
//
//	for(int i=0;i<(int)m_Frames.GetCount();i++)
//	{
//		STORAGE_FRAME_STRUCT Frame;
//		Frame.Time=m_Frames[i].Time;
//		Frame.BoneCount=(int)m_Frames[i].BonesTrans.GetCount();
//		pFile->Write(&Frame,sizeof(STORAGE_FRAME_STRUCT));
//		for(int j=0;j<(int)m_Frames[i].BonesTrans.GetCount();j++)
//		{
//			pFile->Write(&(m_Frames[i].BonesTrans[j]),sizeof(BONE_TRANS));
//		}
//	}	
//
//	return true;
//}
//
//bool CD3DBonedAniResource::FromUSOFile(CUSOFile * pUSOFile,UINT Param)
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
//
//	Destory();	
//
//	pData->ObjectHead.Name[USO_FILE_MAX_OBJECT_NAME-1]=0;
//	SetName(pData->ObjectHead.Name);
//
//	if(m_pManager)
//		m_pManager->AddResource(this,GetName());
//
//	BYTE * pCurData=pBuff+sizeof(STORAGE_STRUCT);
//
//	m_Frames.resize(pData->FrameCount);
//	for(int i=0;i<pData->FrameCount;i++)
//	{
//		STORAGE_FRAME_STRUCT * pFrame=(STORAGE_FRAME_STRUCT *)pCurData;
//		pCurData+=sizeof(STORAGE_FRAME_STRUCT);
//
//		
//		m_Frames[i].Time=pFrame->Time;
//		
//		for(int j=0;j<pFrame->BoneCount;j++)
//		{
//			BONE_TRANS * pBone=(BONE_TRANS *)pCurData;
//			pCurData+=sizeof(BONE_TRANS);
//			m_Frames[i].BonesTrans.push_back(*pBone);
//		}
//		
//	}	
//	
//
//	delete[] pBuff;
//	return true;
//
//}

bool CD3DBonedAniResource::ToSmartStruct(CSmartStruct& Packet,CUSOFile * pUSOFile,UINT Param)
{
	if(!CD3DObjectResource::ToSmartStruct(Packet,pUSOFile,Param))
		return false;	

	for(UINT i=0;i<m_Frames.GetCount();i++)
	{
		UINT BufferSize;
		void * pBuffer=Packet.PrepareMember(BufferSize);
		CSmartStruct SubPacket(pBuffer,BufferSize,true);
		CHECK_SMART_STRUCT_ADD_AND_RETURN(SubPacket.AddMember(SST_FI_TIME,m_Frames[i].Time));
		for(UINT j=0;j<m_Frames[i].BonesTrans.GetCount();j++)
		{
			pBuffer=SubPacket.PrepareMember(BufferSize);
			CSmartStruct TransInfo(pBuffer,BufferSize,true);
			CHECK_SMART_STRUCT_ADD_AND_RETURN(TransInfo.AddMember(SST_TI_TRANSLATION,
				(char *)&(m_Frames[i].BonesTrans[j].Pos),sizeof(m_Frames[i].BonesTrans[j].Pos)));
			CHECK_SMART_STRUCT_ADD_AND_RETURN(TransInfo.AddMember(SST_TI_ROTATION,
				(char *)&(m_Frames[i].BonesTrans[j].Rotation),sizeof(m_Frames[i].BonesTrans[j].Rotation)));
			CHECK_SMART_STRUCT_ADD_AND_RETURN(TransInfo.AddMember(SST_TI_SCALING,
				(char *)&(m_Frames[i].BonesTrans[j].Scale),sizeof(m_Frames[i].BonesTrans[j].Scale)));
			
			if(!SubPacket.FinishMember(SST_FI_TRANSFORM,TransInfo.GetDataLen()))
				return false;
			
		}
		if(!Packet.FinishMember(SST_D3DBAR_FRAME,SubPacket.GetDataLen()))
			return false;
	}
	return true;
}

bool CD3DBonedAniResource::FromSmartStruct(CSmartStruct& Packet,CUSOFile * pUSOFile,UINT Param)
{
	if(!CD3DObjectResource::FromSmartStruct(Packet,pUSOFile,Param))
		return false;
	void * Pos=Packet.GetFirstMemberPosition();
	while(Pos)
	{
		WORD MemberID;
		CSmartStruct Value=Packet.GetNextMember(Pos,MemberID);
		switch(MemberID)
		{	
		case SST_D3DBAR_FRAME:
			{
				m_Frames.Resize(m_Frames.GetCount()+1);
				ANI_FRAME& FrameInfo=m_Frames[m_Frames.GetCount()-1];

				CSmartStruct SubPacket=Value;
				void * SubPos=SubPacket.GetFirstMemberPosition();
				while(SubPos)
				{
					WORD SubMemberID;
					CSmartValue SubValue=SubPacket.GetNextMember(SubPos,SubMemberID);
					switch(SubMemberID)
					{
					case SST_FI_TIME:
						FrameInfo.Time=SubValue;
						break;
					case SST_FI_TRANSFORM:
						{
							CSmartStruct TransPacket=SubValue;
							BONE_TRANS TransInfo;
							memcpy(&TransInfo.Pos,(LPCTSTR)TransPacket.GetMember(SST_TI_TRANSLATION),sizeof(TransInfo.Pos));
							memcpy(&TransInfo.Rotation,(LPCTSTR)TransPacket.GetMember(SST_TI_ROTATION),sizeof(TransInfo.Rotation));
							memcpy(&TransInfo.Scale,(LPCTSTR)TransPacket.GetMember(SST_TI_SCALING),sizeof(TransInfo.Scale));
							FrameInfo.BonesTrans.Add(TransInfo);
						}
						break;
					}
				}
			}
			break;
		}
	}
	
	return true;
}

UINT CD3DBonedAniResource::GetSmartStructSize(UINT Param)
{
	UINT Size=CD3DObjectResource::GetSmartStructSize(Param);

	for(UINT i=0;i<m_Frames.GetCount();i++)
	{
		Size+=SMART_STRUCT_FIX_MEMBER_SIZE(sizeof(m_Frames[i].Time));		
		for(UINT j=0;j<m_Frames[i].BonesTrans.GetCount();j++)
		{
			Size+=SMART_STRUCT_STRING_MEMBER_SIZE(sizeof(m_Frames[i].BonesTrans[j].Pos));
			Size+=SMART_STRUCT_STRING_MEMBER_SIZE(sizeof(m_Frames[i].BonesTrans[j].Rotation));
			Size+=SMART_STRUCT_STRING_MEMBER_SIZE(sizeof(m_Frames[i].BonesTrans[j].Scale));
			Size+=SMART_STRUCT_STRUCT_MEMBER_SIZE(0);
		}
		Size+=SMART_STRUCT_STRUCT_MEMBER_SIZE(0);
	}
	return Size;
}

//CNameObject::STORAGE_STRUCT * CD3DBonedAniResource::USOCreateHead(UINT Param)
//{
//	STORAGE_STRUCT * pHead=new STORAGE_STRUCT;
//	ZeroMemory(pHead,sizeof(STORAGE_STRUCT));
//	pHead->Size=sizeof(STORAGE_STRUCT);
//	return pHead;
//}
//
//int CD3DBonedAniResource::USOWriteHead(CNameObject::STORAGE_STRUCT * pHead,CUSOFile * pUSOFile,UINT Param)
//{
//	int HeadSize=CNameObject::USOWriteHead(pHead,pUSOFile,Param);
//	if(HeadSize<0)
//		return -1;
//
//	STORAGE_STRUCT * pLocalHead=(STORAGE_STRUCT *)pHead;
//
//	
//	for(int i=0;i<(int)m_Frames.GetCount();i++)
//	{
//		pLocalHead->Size+=sizeof(STORAGE_FRAME_STRUCT);		
//		pLocalHead->Size+=(int)m_Frames[i].BonesTrans.GetCount()*sizeof(BONE_TRANS);		
//	}
//	
//	pLocalHead->FrameCount=(int)m_Frames.GetCount();	
//
//	return sizeof(STORAGE_STRUCT);
//}
//
//bool CD3DBonedAniResource::USOWriteData(CNameObject::STORAGE_STRUCT * pHead,CUSOFile * pUSOFile,UINT Param)
//{
//	if(!CNameObject::USOWriteData(pHead,pUSOFile,Param))
//		return false;
//
//	if(pUSOFile==NULL)
//		return false;	
//
//	IFileAccessor * pFile=pUSOFile->GetFile();
//	if(pFile==NULL)
//		return false;
//
//	for(int i=0;i<(int)m_Frames.GetCount();i++)
//	{
//		STORAGE_FRAME_STRUCT Frame;
//		Frame.Time=m_Frames[i].Time;
//		Frame.BoneCount=(int)m_Frames[i].BonesTrans.GetCount();
//		if(pFile->Write(&Frame,sizeof(STORAGE_FRAME_STRUCT))<sizeof(STORAGE_FRAME_STRUCT))
//		{
//			return false;
//		}
//		for(int j=0;j<(int)m_Frames[i].BonesTrans.GetCount();j++)
//		{
//			if(pFile->Write(&(m_Frames[i].BonesTrans[j]),sizeof(BONE_TRANS))<sizeof(BONE_TRANS))
//			{
//				return false;
//			}
//		}
//	}	
//	return true;
//}
//
//bool CD3DBonedAniResource::USOWriteChild(CNameObject::STORAGE_STRUCT * pHead,CUSOFile * pUSOFile,UINT Param)
//{
//	return CNameObject::USOWriteChild(pHead,pUSOFile,Param);		
//}
//
//int CD3DBonedAniResource::USOReadHead(CNameObject::STORAGE_STRUCT * pHead,CUSOFile * pUSOFile,UINT Param)
//{	
//	int ReadSize=CNameObject::USOReadHead(pHead,pUSOFile,Param);
//	if(ReadSize<0)
//		return -1;	
//
//	return sizeof(STORAGE_STRUCT);
//}
//
//
//int CD3DBonedAniResource::USOReadData(CNameObject::STORAGE_STRUCT * pHead,CUSOFile * pUSOFile,BYTE * pData,int DataSize,UINT Param)
//{
//	int ReadSize=CNameObject::USOReadData(pHead,pUSOFile,pData,DataSize,Param);
//
//	pData+=ReadSize;
//	DataSize-=ReadSize;
//
//	STORAGE_STRUCT * pLocalHead=(STORAGE_STRUCT *)pHead;
//
//
//	m_Frames.Resize(pLocalHead->FrameCount);
//	for(int i=0;i<pLocalHead->FrameCount;i++)
//	{
//		STORAGE_FRAME_STRUCT * pFrame=(STORAGE_FRAME_STRUCT *)pData;
//		pData+=sizeof(STORAGE_FRAME_STRUCT);
//		ReadSize+=sizeof(STORAGE_FRAME_STRUCT);
//
//		m_Frames[i].Time=pFrame->Time;
//
//		for(int j=0;j<pFrame->BoneCount;j++)
//		{
//			BONE_TRANS * pBone=(BONE_TRANS *)pData;
//			pData+=sizeof(BONE_TRANS);
//			ReadSize+=sizeof(BONE_TRANS);
//			m_Frames[i].BonesTrans.Add(*pBone);
//		}
//	}	
//	return ReadSize;
//}
//
//bool CD3DBonedAniResource::USOReadChild(CNameObject::STORAGE_STRUCT * pHead,CUSOFile * pUSOFile,UINT Param)
//{	
//	return CNameObject::USOReadChild(pHead,pUSOFile,Param);		
//}
//
//
//bool CD3DBonedAniResource::USOReadFinish(CNameObject::STORAGE_STRUCT * pHead,UINT Param)
//{
//	if(!CNameObject::USOReadFinish(pHead,Param))
//		return false;
//
//	if(m_pManager)
//		m_pManager->AddResource(this,GetName());
//
//	return true;
//}

}