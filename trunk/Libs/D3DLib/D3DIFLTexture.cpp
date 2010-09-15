/****************************************************************************/
/*                                                                          */
/*      �ļ���:    D3DIFLTexture.cpp                                        */
/*      ��������:  2009��09��25��                                           */
/*      ����:      Sagasarate                                               */
/*                                                                          */
/*      �������Ȩ��Sagasarate(sagasarate@sina.com)����                     */
/*      ����Խ�����������κ���ҵ�ͷ���ҵ�����������                      */
/*      ���뱣���˰�Ȩ����                                                  */
/*                                                                          */
/****************************************************************************/
#include "StdAfx.h"

namespace D3DLib{

IMPLEMENT_CLASS_INFO(CD3DIFLTexture,CD3DTexture);

CD3DIFLTexture::CD3DIFLTexture():CD3DTexture()
{
	m_CurFrame=0;	
	m_FlipStartTime=0.0f;
}

CD3DIFLTexture::CD3DIFLTexture(CD3DTextureManager * pManager):CD3DTexture(pManager)
{
	m_CurFrame=0;
	m_FlipStartTime=0.0f;
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
	PrintSystemLog(0,"װ��IFL����<%s>...",Temp);
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

void CD3DIFLTexture::AddFrame(CD3DTexture *	pTexture,float FrameTime)
{
	if(pTexture)
	{
		TEXTURE_FRAME Frame;

		Frame.pTexture=pTexture;
		Frame.FrameTime=FrameTime;

		m_TextrueList.Add(Frame);
	}
	
}

LPDIRECT3DTEXTURE9 CD3DIFLTexture::GetD3DTexture()
{
	if(m_CurFrame<m_TextrueList.GetCount())
	{		
		return m_TextrueList[m_CurFrame].pTexture->GetD3DTexture();
	}
	return NULL;
}

void CD3DIFLTexture::Update(FLOAT Time)
{
	if(m_TextrueList.GetCount())
	{	
		if(Time-m_FlipStartTime>m_TextrueList[m_CurFrame].FrameTime)
		{
			m_FlipStartTime=Time;
			m_CurFrame++;
			if(m_CurFrame<0)
				m_CurFrame=(int)m_TextrueList.GetCount()-1;
			if(m_CurFrame>=(int)m_TextrueList.GetCount())
				m_CurFrame=0;			
		}	
	}
}



void CD3DIFLTexture::Destory()
{
	for(int i=0;i<(int)m_TextrueList.GetCount();i++)
	{
		SAFE_RELEASE(m_TextrueList[i].pTexture);
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

		CHECK_SMART_STRUCT_ADD_AND_RETURN(SubPacket.AddMember(SST_FI_TIME,m_TextrueList[i].FrameTime));
		int ResourceID=pUSOFile->ResourceObjectToIndex(m_TextrueList[i].pTexture);
		CHECK_SMART_STRUCT_ADD_AND_RETURN(SubPacket.AddMember(SST_FI_TEXTURE,ResourceID));
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





}