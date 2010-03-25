/****************************************************************************/
/*                                                                          */
/*      文件名:    D3DIFLTexture.cpp                                        */
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

IMPLEMENT_CLASS_INFO(CD3DIFLTexture,CD3DTexture);

CD3DIFLTexture::CD3DIFLTexture():CD3DTexture()
{
	m_CurFrame=0;	
}

CD3DIFLTexture::CD3DIFLTexture(CD3DTextureManager * pManager):CD3DTexture(pManager)
{
	m_CurFrame=0;	
}

CD3DIFLTexture::~CD3DIFLTexture(void)
{
	Destory();
}

bool CD3DIFLTexture::LoadTexture(LPCTSTR TextureFileName,UINT MipLevels,bool UseFilter,bool IsManaged,D3DCOLOR KeyColor)
{	
	CEasyString Temp;

	if(m_pManager==NULL)
		return false;
	Temp=FindFileOne(TextureFileName);
#ifdef _DEBUG
	char Msg[1024];
	sprintf_s(Msg,1024,"装载IFL纹理<%s>...\r\n",Temp);
	OutputDebugString(Msg);
#endif

	CStringFile	Lines(Temp,m_FileChannel);
	
	for(int i=0;i<Lines.GetLineCount();i++)
	{
		char Line[MAX_PATH+1];
		strcpy_s(Line,MAX_PATH,Lines[i]);
		
		char * time=strchr(Line,' ');
		if(time)
		{
			TEXTURE_FRAME Frame;
			time[0]=0;
			time++;			
			Temp=FindFileOne(Line);
			Frame.pTexture=m_pManager->LoadTexture(Temp,MipLevels,UseFilter,IsManaged,KeyColor);
			if(Frame.pTexture)
			{
				Frame.FrameTime=(FLOAT)atof(time);
				m_TextrueList.Add(Frame);
			}
			else
			{
				SAFE_DELETE(Frame.pTexture);
			}

		}		
	}
	if(m_TextrueList.GetCount())
		return true;
	else
		return false;
}

LPDIRECT3DTEXTURE9 CD3DIFLTexture::GetD3DTexture()
{
	if(m_TextrueList.GetCount())
	{	
		FLOAT Time=(FLOAT)CEasyTimer::GetTime()/1000.0f;
		if(Time-m_FlipStartTime>m_TextrueList[m_CurFrame].FrameTime)
		{
			m_FlipStartTime=Time;
			m_CurFrame++;
			if(m_CurFrame<0)
				m_CurFrame=(int)m_TextrueList.GetCount()-1;
			if(m_CurFrame>=(int)m_TextrueList.GetCount())
				m_CurFrame=0;			
		}	
		return m_TextrueList[m_CurFrame].pTexture->GetD3DTexture();
	}
	return NULL;
}

bool CD3DIFLTexture::ApplyTexture(int stage)
{
	if(m_TextrueList.GetCount())
	{	
		FLOAT Time=(FLOAT)CEasyTimer::GetTime()/1000.0f;
		if(Time-m_FlipStartTime>m_TextrueList[m_CurFrame].FrameTime)
		{
			m_FlipStartTime=Time;
			m_CurFrame++;
			if(m_CurFrame<0)
				m_CurFrame=(int)m_TextrueList.GetCount()-1;
			if(m_CurFrame>=(int)m_TextrueList.GetCount())
				m_CurFrame=0;			
		}
		if(m_pManager&&m_TextrueList[m_CurFrame].pTexture)
		{
			if(m_pManager->GetDevice())
			{			
				if(SUCCEEDED(m_pManager->GetDevice()->GetD3DDevice()->SetTexture(stage,m_pTexture)))
					return true;			
			}
		}
	}	
	return false;
}

void CD3DIFLTexture::Destory()
{
	for(int i=0;i<(int)m_TextrueList.GetCount();i++)
	{
		SAFE_DELETE(m_TextrueList[i].pTexture);
	}	
	m_TextrueList.Clear();
	CD3DTexture::Destory();
}

bool CD3DIFLTexture::Reset()
{
	for(int i=0;i<(int)m_TextrueList.GetCount();i++)
	{
		m_TextrueList[i].pTexture->Reset();
	}
	return true;
}

bool CD3DIFLTexture::Restore()
{
	bool Ret=true;
	for(int i=0;i<(int)m_TextrueList.GetCount();i++)
	{
		if(!m_TextrueList[i].pTexture->Restore())
			Ret=false;
	}
	return Ret;
}

int CD3DIFLTexture::GetFrameCount()
{
	return (int)m_TextrueList.GetCount();
}

CD3DTexture * CD3DIFLTexture::GetFrameTexture(int FrameIndex)
{
	if(FrameIndex>=0&&FrameIndex<(int)m_TextrueList.GetCount())
	{
		return m_TextrueList[FrameIndex].pTexture;
	}
	return NULL;
}

void CD3DIFLTexture::PickResource(CNameObjectSet * pObjectSet,UINT Param)
{
	for(int i=0;i<(int)m_TextrueList.GetCount();i++)
	{
		pObjectSet->Add(m_TextrueList[i].pTexture);
	}	
}

bool CD3DIFLTexture::ToSmartStruct(CSmartStruct& Packet,CUSOFile * pUSOFile,UINT Param)
{
	if(!CNameObject::ToSmartStruct(Packet,pUSOFile,Param))
		return false;	

	for(UINT i=0;i<m_TextrueList.GetCount();i++)
	{
		UINT BufferSize;
		void * pBuffer=Packet.PrepareMember(BufferSize);
		CSmartStruct SubPacket(pBuffer,BufferSize,true);

		CHECK_SMART_STRUCT_ADD_AND_RETURN(Packet.AddMember(SST_FI_TIME,m_TextrueList[i].FrameTime));
		int ResourceID=pUSOFile->ResourceObjectToIndex(m_TextrueList[i].pTexture);
		CHECK_SMART_STRUCT_ADD_AND_RETURN(Packet.AddMember(SST_FI_TEXTURE,ResourceID));
		if(!Packet.FinishMember(SST_D3DITEX_FRAME,SubPacket.GetDataLen()))
			return false;
	}

	return true;
}
bool CD3DIFLTexture::FromSmartStruct(CSmartStruct& Packet,CUSOFile * pUSOFile,UINT Param)
{
	if(!CNameObject::FromSmartStruct(Packet,pUSOFile,Param))
		return false;

	void * Pos=Packet.GetFirstMemberPosition();
	while(Pos)
	{
		WORD MemberID;
		CSmartStruct Value=Packet.GetNextMember(Pos,MemberID);
		switch(MemberID)
		{	
		case SST_D3DITEX_FRAME:
			{
				TEXTURE_FRAME Frame;
				Frame.FrameTime=Value.GetMember(SST_FI_TIME);
				int ResourceID=Value.GetMember(SST_FI_TEXTURE);
				Frame.pTexture=(CD3DTexture *)pUSOFile->ResourceIndexToObject(ResourceID,GET_CLASS_INFO(CD3DTexture));
				if(Frame.pTexture)
				{
					Frame.pTexture->AddUseRef();
					m_TextrueList.Add(Frame);
				}
			}
			break;
		}
	}

	if(m_pManager)
		m_pManager->AddTexture(this,GetName());

	return true;
}

UINT CD3DIFLTexture::GetSmartStructSize(UINT Param)
{
	UINT Size=CNameObject::GetSmartStructSize(Param);

	for(UINT i=0;i<m_TextrueList.GetCount();i++)
	{
		Size+=SMART_STRUCT_FIX_MEMBER_SIZE(sizeof(m_TextrueList[i].FrameTime));
		Size+=SMART_STRUCT_FIX_MEMBER_SIZE(sizeof(int));	
		Size+=SMART_STRUCT_STRUCT_MEMBER_SIZE(0);
	}	
	return Size;
}


//bool CD3DIFLTexture::ToUSOFile(CUSOFile * pUSOFile,UINT Param)
//{	
//
//	if(pUSOFile==NULL)
//		return false;	
//
//	IFileAccessor * pFile=pUSOFile->GetFile();
//	if(pFile==NULL)
//		return false;	
//
//	STORAGE_STRUCT Data;
//	
//	strncpy_0(Data.ObjectHead.Type,USO_FILE_MAX_TYPE_LEN,GetClassInfo().ClassName,USO_FILE_MAX_TYPE_LEN);		
//	strncpy_0(Data.ObjectHead.Name,USO_FILE_MAX_OBJECT_NAME,GetName(),USO_FILE_MAX_OBJECT_NAME);
//	Data.FrameCount=(UINT)m_TextrueList.GetCount();
//	Data.ObjectHead.Size=sizeof(STORAGE_STRUCT)+Data.FrameCount*sizeof(STORAGE_FRAME_STRUCT);
//
//	pFile->Write(&Data,sizeof(STORAGE_STRUCT));
//	
//	for(int i=0;i<(int)m_TextrueList.GetCount();i++)
//	{
//		STORAGE_FRAME_STRUCT Frame;
//		Frame.FrameTime=m_TextrueList[i].FrameTime;
//		Frame.TextureIndex=pUSOFile->ResourceObjectToIndex(m_TextrueList[i].pTexture);
//		pFile->Write(&Frame,sizeof(Frame));		
//	}	
//
//	return true;
//}
//
//bool CD3DIFLTexture::FromUSOFile(CUSOFile * pUSOFile,UINT Param)
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
//		pData->ObjectHead.Size<sizeof(STORAGE_STRUCT)+pData->FrameCount*sizeof(float))
//	{	
//		delete[] pBuff;
//		return false;
//	}
//	pData->ObjectHead.Name[USO_FILE_MAX_OBJECT_NAME-1]=0;
//	SetName(pData->ObjectHead.Name);
//	if(m_pManager)
//		m_pManager->AddTexture(this,GetName());
//
//	STORAGE_FRAME_STRUCT * pFrames=(STORAGE_FRAME_STRUCT *)(pBuff+sizeof(STORAGE_STRUCT));
//
//	Destory();
//	for(UINT i=0;i<pData->FrameCount;i++)
//	{
//		TEXTURE_FRAME Frame;
//		Frame.FrameTime=pFrames[i].FrameTime;
//		Frame.pTexture=dynamic_cast<CD3DTexture *>(pUSOFile->ResourceIndexToObject(pFrames[i].TextureIndex));
//		if(Frame.pTexture)
//		{
//			Frame.pTexture->AddUseRef();
//			m_TextrueList.push_back(Frame);
//		}
//	}
//	
//	delete[] pBuff;
//	return true;
//
//}

//CNameObject::STORAGE_STRUCT * CD3DIFLTexture::USOCreateHead(UINT Param)
//{
//	STORAGE_STRUCT * pHead=new STORAGE_STRUCT;
//	ZeroMemory(pHead,sizeof(STORAGE_STRUCT));
//	pHead->Size=sizeof(STORAGE_STRUCT);
//	return pHead;
//}
//
//int CD3DIFLTexture::USOWriteHead(CNameObject::STORAGE_STRUCT * pHead,CUSOFile * pUSOFile,UINT Param)
//{
//	int HeadSize=CNameObject::USOWriteHead(pHead,pUSOFile,Param);
//	if(HeadSize<0)
//		return -1;
//
//	STORAGE_STRUCT * pLocalHead=(STORAGE_STRUCT *)pHead;
//
//	pLocalHead->Size+=m_TextrueList.GetCount()*sizeof(STORAGE_FRAME_STRUCT);
//
//	pLocalHead->FrameCount=(UINT)m_TextrueList.GetCount();
//	
//	return sizeof(STORAGE_STRUCT);
//}
//
//bool CD3DIFLTexture::USOWriteData(CNameObject::STORAGE_STRUCT * pHead,CUSOFile * pUSOFile,UINT Param)
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
//	for(int i=0;i<(int)m_TextrueList.GetCount();i++)
//	{
//		STORAGE_FRAME_STRUCT Frame;
//		Frame.FrameTime=m_TextrueList[i].FrameTime;
//		Frame.TextureIndex=pUSOFile->ResourceObjectToIndex(m_TextrueList[i].pTexture);
//		if(pFile->Write(&Frame,sizeof(Frame))<sizeof(Frame))
//			return false;
//	}	
//	return true;
//}
//
//int CD3DIFLTexture::USOReadHead(CNameObject::STORAGE_STRUCT * pHead,CUSOFile * pUSOFile,UINT Param)
//{	
//	int ReadSize=CNameObject::USOReadHead(pHead,pUSOFile,Param);
//	if(ReadSize<0)
//		return -1;	
//
//	return sizeof(STORAGE_STRUCT);
//}
//
//int CD3DIFLTexture::USOReadData(CNameObject::STORAGE_STRUCT * pHead,CUSOFile * pUSOFile,BYTE * pData,int DataSize,UINT Param)
//{
//	int ReadSize=CNameObject::USOReadData(pHead,pUSOFile,pData,DataSize,Param);
//
//	pData+=ReadSize;
//	DataSize-=ReadSize;
//
//	STORAGE_STRUCT * pLocalHead=(STORAGE_STRUCT *)pHead;
//
//	
//	STORAGE_FRAME_STRUCT * pFrames=(STORAGE_FRAME_STRUCT *)pData;		
//	ReadSize+=sizeof(STORAGE_FRAME_STRUCT)*pLocalHead->FrameCount;
//	for(UINT i=0;i<pLocalHead->FrameCount;i++)
//	{
//		TEXTURE_FRAME Frame;
//		Frame.FrameTime=pFrames[i].FrameTime;
//		Frame.pTexture=dynamic_cast<CD3DTexture *>(pUSOFile->ResourceIndexToObject(pFrames[i].TextureIndex,GET_CLASS_INFO(CD3DTexture)));
//		if(Frame.pTexture)
//		{
//			Frame.pTexture->AddUseRef();
//			m_TextrueList.Add(Frame);
//		}
//	}		
//	
//	return ReadSize;
//}
//
//bool CD3DIFLTexture::USOReadFinish(CNameObject::STORAGE_STRUCT * pHead,UINT Param)
//{
//	if(!CNameObject::USOReadFinish(pHead,Param))
//		return false;
//
//	if(m_pManager)
//		m_pManager->AddTexture(this,GetName());
//	return true;
//}


}