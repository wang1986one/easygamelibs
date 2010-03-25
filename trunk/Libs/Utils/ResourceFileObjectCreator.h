/****************************************************************************/
/*                                                                          */
/*      �ļ���:    ResourceFileObjectCreator.h                              */
/*      ��������:  2009��07��06��                                           */
/*      ����:      Sagasarate                                               */
/*                                                                          */
/*      �������Ȩ��Sagasarate(sagasarate@sina.com)����                     */
/*      ����Խ�����������κ���ҵ�ͷ���ҵ�����������                      */
/*      ���뱣���˰�Ȩ����                                                  */
/*                                                                          */
/****************************************************************************/
#pragma once


class CResourceFileObjectCreator :
	public IFileObjectCreator
{
protected:
	DECLARE_CLASS_INFO_STATIC(CResourceFileObjectCreator);
public:
	CResourceFileObjectCreator(void);
	virtual ~CResourceFileObjectCreator(void);

	virtual IFileAccessor * CreateFileAccessor();
};
