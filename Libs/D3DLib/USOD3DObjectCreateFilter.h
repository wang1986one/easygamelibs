/****************************************************************************/
/*                                                                          */
/*      �ļ���:    USOD3DObjectCreateFilter.h                               */
/*      ��������:  2009��09��11��                                           */
/*      ����:      Sagasarate                                               */
/*                                                                          */
/*      ��������Ȩ��Sagasarate(sagasarate@sina.com)����                     */
/*      ����Խ������������κ���ҵ�ͷ���ҵ������������                      */
/*      ���뱣���˰�Ȩ����                                                  */
/*                                                                          */
/****************************************************************************/
#pragma once

namespace D3DLib{


class CUSOD3DObjectCreateFilter :
	public CUSOObjectCreateFilter
{
protected:
	CD3DDevice *	m_pD3DDevice;

	DECLARE_CLASS_INFO(CUSOD3DObjectCreateFilter)
public:
	CUSOD3DObjectCreateFilter();
	CUSOD3DObjectCreateFilter(CD3DDevice * pDevice);
	~CUSOD3DObjectCreateFilter(void);

	void SetDevice(CD3DDevice * pDevice);

	virtual bool OnObjectCreate(CNameObject * pObject);
	virtual CNameObject * FindObject(CLASS_INFO * pClassInfo,LPCTSTR ObjectName);
};


}