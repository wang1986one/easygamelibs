/****************************************************************************/
/*                                                                          */
/*      �ļ���:    D3DGUITexture.cpp                                        */
/*      ��������:  2009��09��11��                                           */
/*      ����:      Sagasarate                                               */
/*                                                                          */
/*      �������Ȩ��Sagasarate(sagasarate@sina.com)����                     */
/*      ����Խ�����������κ���ҵ�ͷ���ҵ�����������                      */
/*      ���뱣���˰�Ȩ����                                                  */
/*                                                                          */
/****************************************************************************/
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