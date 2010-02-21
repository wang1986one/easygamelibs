#include "StdAfx.h"
#include "D3DGUITexture.h"

namespace D3DGUI{

IMPLEMENT_CLASS_INFO(CD3DGUITexture,CD3DTexture);

CD3DGUITexture::CD3DGUITexture():CD3DTexture()
{
}

CD3DGUITexture::CD3DGUITexture(CD3DTextureManager * pManager):CD3DTexture(pManager)
{
}

CD3DGUITexture::~CD3DGUITexture(void)
{
}

}