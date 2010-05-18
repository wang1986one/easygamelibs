/****************************************************************************/
/*                                                                          */
/*      �ļ���:    D3DBaseStaticModel.cpp                                   */
/*      ��������:  2010��02��09��                                           */
/*      ����:      Sagasarate                                               */
/*                                                                          */
/*      �������Ȩ��Sagasarate(sagasarate@sina.com)����                     */
/*      ����Խ�����������κ���ҵ�ͷ���ҵ�����������                      */
/*      ���뱣���˰�Ȩ����                                                  */
/*                                                                          */
/****************************************************************************/
#include "StdAfx.h"

namespace D3DLib{

IMPLEMENT_CLASS_INFO(CD3DBaseStaticModel,CD3DObject);

CD3DBaseStaticModel::CD3DBaseStaticModel(void)
{
}

CD3DBaseStaticModel::~CD3DBaseStaticModel(void)
{
	Destory();
}


void CD3DBaseStaticModel::Destory()
{
	CD3DObject::Destory();
}

bool CD3DBaseStaticModel::Reset()
{	
	return m_pBoundingFrame->Reset()&&CD3DObject::Reset();
}

bool CD3DBaseStaticModel::Restore()
{
	return m_pBoundingFrame->Restore()&&CD3DObject::Reset();
}



}