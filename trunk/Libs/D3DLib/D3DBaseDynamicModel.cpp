/****************************************************************************/
/*                                                                          */
/*      �ļ���:    D3DBaseDynamicModel.cpp                                  */
/*      ��������:  2009��09��11��                                           */
/*      ����:      Sagasarate                                               */
/*                                                                          */
/*      �������Ȩ��Sagasarate(sagasarate@sina.com)����                     */
/*      ����Խ�����������κ���ҵ�ͷ���ҵ�����������                      */
/*      ���뱣���˰�Ȩ����                                                  */
/*                                                                          */
/****************************************************************************/
#include "StdAfx.h"

namespace D3DLib{

IMPLEMENT_CLASS_INFO(CD3DBaseDynamicModel,CD3DObject);

CD3DBaseDynamicModel::CD3DBaseDynamicModel(void):CD3DObject()
{
	
}

CD3DBaseDynamicModel::~CD3DBaseDynamicModel(void)
{
	Destory();
}

void CD3DBaseDynamicModel::Destory()
{
	
	CD3DObject::Destory();
}

bool CD3DBaseDynamicModel::Reset()
{	
	return m_pBoundingFrame->Reset()&&CD3DObject::Reset();
}

bool CD3DBaseDynamicModel::Restore()
{
	return m_pBoundingFrame->Restore()&&CD3DObject::Reset();
}

bool CD3DBaseDynamicModel::Play(bool IsLoop)
{
	return false;
}
bool CD3DBaseDynamicModel::Stop()
{
	return false;
}
bool CD3DBaseDynamicModel::IsPlaying()
{
	return false;
}



}