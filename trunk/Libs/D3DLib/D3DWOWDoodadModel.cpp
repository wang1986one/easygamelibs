/****************************************************************************/
/*                                                                          */
/*      �ļ���:    D3DWOWDoodadModel.cpp	                                */
/*      ��������:  2010��08��05��                                           */
/*      ����:      Sagasarate                                               */
/*                                                                          */
/*      �������Ȩ��Sagasarate(sagasarate@sina.com)����                     */
/*      ����Խ�����������κ���ҵ�ͷ���ҵ�����������                      */
/*      ���뱣���˰�Ȩ����                                                  */
/*                                                                          */
/****************************************************************************/
#include "StdAfx.h"


namespace D3DLib{

IMPLEMENT_CLASS_INFO(CD3DWOWDoodadModel,CD3DWOWM2Model);

CD3DWOWDoodadModel::CD3DWOWDoodadModel(void)
{
	m_GroupIndex=0xffff;
}

CD3DWOWDoodadModel::~CD3DWOWDoodadModel(void)
{
	Destory();
}

void CD3DWOWDoodadModel::Destory()
{
	CD3DWOWM2Model::Destory();
	m_GroupIndex=0xffff;
}

}