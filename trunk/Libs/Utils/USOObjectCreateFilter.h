/****************************************************************************/
/*                                                                          */
/*      �ļ���:    USOObjectCreateFilter.h                                  */
/*      ��������:  2009��07��06��                                           */
/*      ����:      Sagasarate                                               */
/*                                                                          */
/*      �������Ȩ��Sagasarate(sagasarate@sina.com)����                     */
/*      ����Խ�����������κ���ҵ�ͷ���ҵ�����������                      */
/*      ���뱣���˰�Ȩ����                                                  */
/*                                                                          */
/****************************************************************************/
#pragma once

class CUSOObjectCreateFilter :
	public CNameObject
{
protected:

	DECLARE_CLASS_INFO(CUSOObjectCreateFilter)
public:
	CUSOObjectCreateFilter(void);
	~CUSOObjectCreateFilter(void);

	virtual bool OnObjectCreate(CNameObject * pObject);
	virtual CNameObject * FindObject(CLASS_INFO * pClassInfo,LPCTSTR ObjectName);
};
