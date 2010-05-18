/****************************************************************************/
/*                                                                          */
/*      �ļ���:    D3DBaseStaticModel.h                                     */
/*      ��������:  2010��02��09��                                           */
/*      ����:      Sagasarate                                               */
/*                                                                          */
/*      �������Ȩ��Sagasarate(sagasarate@sina.com)����                     */
/*      ����Խ�����������κ���ҵ�ͷ���ҵ�����������                      */
/*      ���뱣���˰�Ȩ����                                                  */
/*                                                                          */
/****************************************************************************/
#pragma once

namespace D3DLib{

class CD3DBaseStaticModel :
	public CD3DObject
{
protected:
protected:

	DECLARE_CLASS_INFO(CD3DBaseStaticModel)
public:
	CD3DBaseStaticModel(void);
	~CD3DBaseStaticModel(void);

	virtual void Destory();

	virtual bool Reset();
	virtual bool Restore();
	

};

}