/****************************************************************************/
/*                                                                          */
/*      �ļ���:    D3DTextTexture.cpp                                       */
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


LPCTSTR TEXT_TEXTURE_FX_NT=
	"texture TexLay0 < string name = \"test.jpg\"; >;"
	"texture TexLay1 < string name = \"test1.jpg\"; >;"
	"technique tec0"
	"{"
	"    pass p0"
	"    {"
	"		MultiSampleAntialias = FALSE;"
	"		Lighting=false;"
	"		zenable = false;"
	"		zwriteenable = false;"
	"		CullMode = ccw;"
	"		fogenable = false;"
	"		Texture[0] = <TexLay0>;"
	"		AlphaTestEnable = false;"
	"		AlphaBlendEnable = true;"
	"		SrcBlend = SrcAlpha;"
	"		DestBlend = InvSrcAlpha;"
	"		Texture[0] = <TexLay0>;"
	"     	ColorOp[0] = SelectArg2;"
	"       ColorArg1[0] = Texture;"
	"       ColorArg2[0] = Diffuse;"      	
	"       AlphaOp[0] = SelectArg2;"
	"       AlphaArg1[0] = Texture;"
	"       AlphaArg2[0] = diffuse;"
	"		ColorOp[1] = disable;"
	"		AlphaOp[1] = disable;"
	"		AddressU[0] = clamp;"
	"		AddressV[0] = clamp;"      
	"		MinFilter[0] = none;"
	"       MagFilter[0] = none;"
	"       MipFilter[0] = none;"
	"		VertexShader = NULL;							\r\n"
	"		PixelShader  = NULL;							\r\n"
	"    }"
	"}";
	
IMPLEMENT_CLASS_INFO(CD3DTextTexture,CD3DTexture);

CD3DTextTexture::CD3DTextTexture():CD3DTexture()
{
	ZeroMemory(&m_LogFont,sizeof(m_LogFont));
	m_pD3DFont=NULL;
	m_MipLevels=1;
	m_Width=100;
	m_Height=100;	
	m_Color=0xffffffff;
	m_BKColor=0x00000000;
	m_ShadowWidth=0;
	m_ShadowMode=0;
	m_ShadowColor=0xff000000;
	m_Align=DT_WORDBREAK;	
	m_AllowUpdate=true;
	m_pFX=NULL;
	m_CharSpace=0;
	m_LineSpace=0;
	m_WantUpdate=false;

}

CD3DTextTexture::CD3DTextTexture(CD3DTextureManager * pManager):CD3DTexture(pManager)
{
	ZeroMemory(&m_LogFont,sizeof(m_LogFont));
	m_pD3DFont=NULL;
	m_MipLevels=1;
	m_Width=100;
	m_Height=100;	
	m_Color=0xffffffff;
	m_BKColor=0x00000000;
	m_ShadowWidth=0;
	m_ShadowMode=0;
	m_ShadowColor=0xff000000;
	m_Align=DT_WORDBREAK;	
	m_AllowUpdate=true;
	m_pFX=NULL;
	m_CharSpace=0;
	m_LineSpace=0;
	m_WantUpdate=false;

}

CD3DTextTexture::~CD3DTextTexture(void)
{
	Destory();
}

void CD3DTextTexture::Destory()
{
	SAFE_RELEASE(m_pD3DFont);
	SAFE_RELEASE(m_pTexture);
	SAFE_RELEASE(m_pFX);
	CD3DTexture::Destory();
}

bool CD3DTextTexture::Reset()
{
	//SAFE_RELEASE(m_pD3DFont);
	//Destory();
	//return true;
	return CD3DTexture::Reset();
}

bool CD3DTextTexture::Restore()
{
	//return Create(&m_LogFont,m_Width,m_Height,m_MipLevels,m_Color);
	return CD3DTexture::Restore();
}

bool CD3DTextTexture::Create(LOGFONT * pLogFont,int Width,int Height,int MipLevels,D3DCOLOR FontColor)
{
	//HRESULT hr;

	if(pLogFont)
	{	
		if(!SetFont(pLogFont))
			return false;		
	}

	SAFE_RELEASE(m_pTexture);
	
	if(!CreateTexture(Width,Height,D3DFMT_DXT5,D3DUSAGE_RENDERTARGET,D3DPOOL_DEFAULT,MipLevels))
		return false;	

	if(m_pFX==NULL)
		m_pFX=m_pManager->GetDevice()->GetFXManager()->LoadFXFromMemory("TEXT_TEXTURE_FX_NT",(VOID *)TEXT_TEXTURE_FX_NT,(int)strlen(TEXT_TEXTURE_FX_NT));

	m_Width=Width;
	m_Height=Height;
	m_Color=FontColor;
	m_MipLevels=MipLevels;
	m_WantUpdate=true;

	//UpdateTexture();
	return true;
}

bool CD3DTextTexture::SetSize(int Width,int Height)
{
	if(Width!=m_Width||Height!=m_Height)
	{
		m_Width=Width;
		m_Height=Height;
		return Create(NULL,m_Width,m_Height,m_MipLevels,m_Color);
	}
	return true;
}

bool CD3DTextTexture::SetFont(const LOGFONT* pLogFont)
{
	CD3DBaseFont * pNewFont;
	

	pNewFont=m_pManager->GetDevice()->GetFontManager()->CreateFontCN(pLogFont,m_MipLevels);
	//pNewFont=m_pManager->GetDevice()->GetFontManager()->CreateDX9Font(pLogFont,m_MipLevels);

	if(pNewFont)
	{
		SAFE_RELEASE(m_pD3DFont);
		m_pD3DFont=pNewFont;
		if(pLogFont)
			m_LogFont=*pLogFont;
		m_WantUpdate=true;
		//UpdateTexture();
		return true;
	}
	return false;
}


void CD3DTextTexture::SetColor(D3DCOLOR Color)
{
	//����������ɫ
	if(m_Color!=Color)
	{
		m_Color=Color;
		m_WantUpdate=true;
		//UpdateTexture();
	}
}


void CD3DTextTexture::SetText(LPCTSTR szText)
{
	//������������
	CEasyStringW NewText(szText);
	if(m_Text!=NewText)
	{
		m_Text=NewText;
		m_WantUpdate=true;
		//UpdateTexture();
	}
}

void CD3DTextTexture::SetTextW(LPCWSTR szText)
{
	//������������
	if(m_Text!=szText)
	{
		m_Text=szText;
		m_WantUpdate=true;
		//UpdateTexture();
	}
}

void CD3DTextTexture::SetAlign(DWORD dwAlign)
{
	//���ö���ģʽ
	if(m_Align!=dwAlign)
	{
		m_Align=dwAlign;
		m_WantUpdate=true;
		//UpdateTexture();
	}
}
void CD3DTextTexture::SetShadowMode(DWORD ShadowMode)
{
	//������Ӱģʽ��0=����Ӱ��1=�ķ�����Ӱ��2=�˷�����Ӱ
	if(m_ShadowMode!=ShadowMode)
	{
		m_ShadowMode=ShadowMode;
		m_WantUpdate=true;
		//UpdateTexture();
	}
}
void CD3DTextTexture::SetShadowColor(D3DCOLOR ShadowColor)
{
	//������Ӱ��ɫ
	if(m_ShadowColor!=ShadowColor)
	{
		m_ShadowColor=ShadowColor;
		m_WantUpdate=true;
		//UpdateTexture();
	}
}
void CD3DTextTexture::SetShadowWidth(int Width)
{
	//������Ӱ���
	if(m_ShadowWidth!=Width)
	{
		m_ShadowWidth=Width;
		m_WantUpdate=true;
		//UpdateTexture();
	}
}

void CD3DTextTexture::SetBKColor(D3DCOLOR Color)
{	
	//���ñ���ɫ
	if(m_BKColor!=Color)
	{
		m_BKColor=Color;
		m_WantUpdate=true;
		//UpdateTexture();
	}
}

void CD3DTextTexture::SetCharSpace(int Space)
{
	if(m_CharSpace!=Space&&m_pD3DFont)
	{
		m_pD3DFont->SetCharSpace(Space);
		m_CharSpace=m_pD3DFont->GetCharSpace();
		m_WantUpdate=true;
		//UpdateTexture();
	}
}

void CD3DTextTexture::SetLineSpace(int Space)
{
	if(m_LineSpace!=Space&&m_pD3DFont)
	{
		m_pD3DFont->SetLineSpace(Space);
		m_LineSpace=m_pD3DFont->GetLineSpace();
		m_WantUpdate=true;
		//UpdateTexture();
	}
}

void CD3DTextTexture::EnableUpdate(bool AllowUpdate)
{
	m_AllowUpdate=AllowUpdate;	
	//if(m_AllowUpdate)
	//	UpdateTexture();
}

HDC CD3DTextTexture::GetDC()
{
	if(m_pD3DFont)
		return m_pD3DFont->GetDC();
	return NULL;
}


void CD3DTextTexture::OnPrepareRenderData()
{
	if(m_WantUpdate&&m_AllowUpdate)
	{
		UpdateTexture();
	}
}

bool CD3DTextTexture::ToSmartStruct(CSmartStruct& Packet,CUSOResourceManager * pResourceManager,UINT Param)
{
	if(!CNameObject::ToSmartStruct(Packet,pResourceManager,Param))
		return false;	
	

	CHECK_SMART_STRUCT_ADD_AND_RETURN(Packet.AddMember(SST_D3DTTEX_LOG_FONT,(char *)&m_LogFont,sizeof(m_LogFont)));
	CHECK_SMART_STRUCT_ADD_AND_RETURN(Packet.AddMember(SST_D3DTTEX_COLOR,m_Color));
	CHECK_SMART_STRUCT_ADD_AND_RETURN(Packet.AddMember(SST_D3DTTEX_BACK_COLOR,m_BKColor));
	CHECK_SMART_STRUCT_ADD_AND_RETURN(Packet.AddMember(SST_D3DTTEX_SHADOW_MODE,m_ShadowMode));
	CHECK_SMART_STRUCT_ADD_AND_RETURN(Packet.AddMember(SST_D3DTTEX_SHADOW_WIDTH,m_ShadowWidth));
	CHECK_SMART_STRUCT_ADD_AND_RETURN(Packet.AddMember(SST_D3DTTEX_SHADOWCOLOR,m_ShadowColor));
	CHECK_SMART_STRUCT_ADD_AND_RETURN(Packet.AddMember(SST_D3DTTEX_ALIGN,m_Align));
	CHECK_SMART_STRUCT_ADD_AND_RETURN(Packet.AddMember(SST_D3DTTEX_CHARSPACE,m_CharSpace));
	CHECK_SMART_STRUCT_ADD_AND_RETURN(Packet.AddMember(SST_D3DTTEX_LINESPACE,m_LineSpace));
	CHECK_SMART_STRUCT_ADD_AND_RETURN(Packet.AddMember(SST_D3DTTEX_TEXT,(LPCWSTR)m_Text));
	CHECK_SMART_STRUCT_ADD_AND_RETURN(Packet.AddMember(SST_D3DTTEX_MIPLEVELS,m_MipLevels));
	CHECK_SMART_STRUCT_ADD_AND_RETURN(Packet.AddMember(SST_D3DTTEX_WIDTH,m_Width));
	CHECK_SMART_STRUCT_ADD_AND_RETURN(Packet.AddMember(SST_D3DTTEX_HEIGHT,m_Height));

	

	return true;
}
bool CD3DTextTexture::FromSmartStruct(CSmartStruct& Packet,CUSOResourceManager * pResourceManager,UINT Param)
{
	if(!CNameObject::FromSmartStruct(Packet,pResourceManager,Param))
		return false;

	void * Pos=Packet.GetFirstMemberPosition();
	while(Pos)
	{
		WORD MemberID;
		CSmartValue Value=Packet.GetNextMember(Pos,MemberID);
		switch(MemberID)
		{
		case SST_D3DTTEX_LOG_FONT:
			memcpy(&m_LogFont,(LPCTSTR)Value,sizeof(m_LogFont));
			break;
		case SST_D3DTTEX_COLOR:
			m_Color=Value;
			break;
		case SST_D3DTTEX_BACK_COLOR:
			m_BKColor=Value;
			break;
		case SST_D3DTTEX_SHADOW_MODE:
			m_ShadowMode=Value;
			break;
		case SST_D3DTTEX_SHADOW_WIDTH:
			m_ShadowWidth=Value;
			break;
		case SST_D3DTTEX_SHADOWCOLOR:
			m_ShadowColor=Value;
			break;
		case SST_D3DTTEX_ALIGN:
			m_Align=Value;
			break;
		case SST_D3DTTEX_CHARSPACE:
			m_CharSpace=Value;
			break;
		case SST_D3DTTEX_LINESPACE:
			m_LineSpace=Value;
			break;
		case SST_D3DTTEX_TEXT:
			m_Text=(LPCWSTR)Value;
			break;
		case SST_D3DTTEX_MIPLEVELS:
			m_MipLevels=Value;
			break;
		case SST_D3DTTEX_WIDTH:
			m_Width=Value;
			break;
		case SST_D3DTTEX_HEIGHT:
			m_Height=Value;
			break;
		}
	}
	if(!Create(&m_LogFont,m_Width,m_Height,m_MipLevels,m_Color))
		return false;

	//UpdateTexture();

	if(m_pManager)
		m_pManager->AddTexture(this,GetName());

	return true;
}

UINT CD3DTextTexture::GetSmartStructSize(UINT Param)
{
	UINT Size=CNameObject::GetSmartStructSize(Param);	

	Size+=SMART_STRUCT_STRING_MEMBER_SIZE(sizeof(m_LogFont));
	Size+=SMART_STRUCT_FIX_MEMBER_SIZE(sizeof(m_Color));
	Size+=SMART_STRUCT_FIX_MEMBER_SIZE(sizeof(m_BKColor));
	Size+=SMART_STRUCT_FIX_MEMBER_SIZE(sizeof(m_ShadowMode));
	Size+=SMART_STRUCT_FIX_MEMBER_SIZE(sizeof(m_ShadowWidth));
	Size+=SMART_STRUCT_FIX_MEMBER_SIZE(sizeof(m_ShadowColor));
	Size+=SMART_STRUCT_FIX_MEMBER_SIZE(sizeof(m_Align));
	Size+=SMART_STRUCT_FIX_MEMBER_SIZE(sizeof(m_CharSpace));
	Size+=SMART_STRUCT_FIX_MEMBER_SIZE(sizeof(m_LineSpace));
	Size+=SMART_STRUCT_WSTRING_MEMBER_SIZE(m_Text.GetLength());
	Size+=SMART_STRUCT_FIX_MEMBER_SIZE(sizeof(m_MipLevels));
	Size+=SMART_STRUCT_FIX_MEMBER_SIZE(sizeof(m_Width));
	Size+=SMART_STRUCT_FIX_MEMBER_SIZE(sizeof(m_Height));

	return Size;
}

void CD3DTextTexture::UpdateTexture()
{
	//���ｫ������Ⱦ������
	if(!m_AllowUpdate)
		return;
	
	
	if(m_pTexture==NULL)	
		return;

	CEasyArray<LPDIRECT3DSURFACE9> SavedRenderTarget;
 	SavedRenderTarget.Resize(m_pManager->GetDevice()->GetRenderTargetCount());

	for(UINT i=0;i<m_pManager->GetDevice()->GetRenderTargetCount();i++)
	{
		SavedRenderTarget[i]=m_pManager->GetDevice()->GetRenderTarget(i);
		if(i)
		{
			m_pManager->GetDevice()->SetRenderTarget(i,(LPDIRECT3DSURFACE9)NULL);
		}
		else
		{
			m_pManager->GetDevice()->SetRenderTarget(i,this,0);
		}
	}

	m_pManager->GetDevice()->StartRender(m_BKColor);

	if(!m_Text.IsEmpty()&&m_pD3DFont&&m_pFX) 
	{

		RECT rc;
		RECT ShadowRect;

		rc.top=m_ShadowWidth;
		rc.bottom=m_Height-m_ShadowWidth;
		rc.left=m_ShadowWidth;
		rc.right=m_Width-m_ShadowWidth;

		int Pass=m_pFX->Begin();

		for(int i=0;i<Pass;i++)
		{
			m_pFX->UseActiveTechnique();
			m_pFX->BeginPass(i);
			if(m_ShadowMode&FONT_SHADOW_TOP)
			{
				ShadowRect=rc;
				ShadowRect.top-=m_ShadowWidth;
				ShadowRect.bottom-=m_ShadowWidth;
				m_pD3DFont->DrawTextW(LPCWSTR(m_Text),(int)m_Text.GetLength(),&ShadowRect,m_Align,m_ShadowColor);

			}
			if(m_ShadowMode&FONT_SHADOW_BOTTOM)
			{
				ShadowRect=rc;
				ShadowRect.top+=m_ShadowWidth;
				ShadowRect.bottom+=m_ShadowWidth;
				m_pD3DFont->DrawTextW(LPCWSTR(m_Text),(int)m_Text.GetLength(),&ShadowRect,m_Align,m_ShadowColor);

			}
			if(m_ShadowMode&FONT_SHADOW_LEFT)
			{
				ShadowRect=rc;
				ShadowRect.left-=m_ShadowWidth;
				ShadowRect.right-=m_ShadowWidth;
				m_pD3DFont->DrawTextW(LPCWSTR(m_Text),(int)m_Text.GetLength(),&ShadowRect,m_Align,m_ShadowColor);

			}
			if(m_ShadowMode&FONT_SHADOW_RIGHT)
			{
				ShadowRect=rc;
				ShadowRect.left+=m_ShadowWidth;
				ShadowRect.right+=m_ShadowWidth;
				m_pD3DFont->DrawTextW(LPCWSTR(m_Text),(int)m_Text.GetLength(),&ShadowRect,m_Align,m_ShadowColor);

			}
			if(m_ShadowMode&FONT_SHADOW_TOP_LEFT)
			{
				ShadowRect=rc;
				ShadowRect.top-=m_ShadowWidth;
				ShadowRect.bottom-=m_ShadowWidth;
				ShadowRect.left-=m_ShadowWidth;
				ShadowRect.right-=m_ShadowWidth;
				m_pD3DFont->DrawTextW(LPCWSTR(m_Text),(int)m_Text.GetLength(),&ShadowRect,m_Align,m_ShadowColor);

			}
			if(m_ShadowMode&FONT_SHADOW_BOTTOM_LEFT)
			{
				ShadowRect=rc;
				ShadowRect.top-=m_ShadowWidth;
				ShadowRect.bottom-=m_ShadowWidth;
				ShadowRect.left+=m_ShadowWidth;
				ShadowRect.right+=m_ShadowWidth;
				m_pD3DFont->DrawTextW(LPCWSTR(m_Text),(int)m_Text.GetLength(),&ShadowRect,m_Align,m_ShadowColor);

			}
			if(m_ShadowMode&FONT_SHADOW_TOP_RIGHT)
			{
				ShadowRect=rc;
				ShadowRect.top+=m_ShadowWidth;
				ShadowRect.bottom+=m_ShadowWidth;
				ShadowRect.left-=m_ShadowWidth;
				ShadowRect.right-=m_ShadowWidth;
				m_pD3DFont->DrawTextW(LPCWSTR(m_Text),(int)m_Text.GetLength(),&ShadowRect,m_Align,m_ShadowColor);

			}
			if(m_ShadowMode&FONT_SHADOW_BOTTOM_RIGHT)
			{
				ShadowRect=rc;
				ShadowRect.top+=m_ShadowWidth;
				ShadowRect.bottom+=m_ShadowWidth;
				ShadowRect.left+=m_ShadowWidth;
				ShadowRect.right+=m_ShadowWidth;
				m_pD3DFont->DrawTextW(LPCWSTR(m_Text),(int)m_Text.GetLength(),&ShadowRect,m_Align,m_ShadowColor);

			}
			
			m_pD3DFont->DrawTextW(LPCWSTR(m_Text),(int)m_Text.GetLength(),&rc,m_Align,m_Color);		

			m_pFX->EndPass(i);
		}
		m_pFX->End();
	}	

	m_pManager->GetDevice()->EndRender(false);

	for(UINT i=0;i<m_pManager->GetDevice()->GetRenderTargetCount();i++)
	{		
		m_pManager->GetDevice()->SetRenderTarget(i,SavedRenderTarget[i]);		
		SAFE_RELEASE(SavedRenderTarget[i]);
	}

	m_WantUpdate=false;
	
    return;
}




}
