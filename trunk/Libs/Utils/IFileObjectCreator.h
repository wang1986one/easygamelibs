/****************************************************************************/
/*                                                                          */
/*      �ļ���:    IFileObjectCreator.h                                     */
/*      ��������:  2009��07��06��                                           */
/*      ����:      Sagasarate                                               */
/*                                                                          */
/*      �������Ȩ��Sagasarate(sagasarate@sina.com)����                     */
/*      ����Խ�����������κ���ҵ�ͷ���ҵ�����������                      */
/*      ���뱣���˰�Ȩ����                                                  */
/*                                                                          */
/****************************************************************************/
#pragma once

class IFileObjectCreator:public CNameObject
{
protected:
	DECLARE_CLASS_INFO_STATIC(IFileObjectCreator)
public:
	IFileObjectCreator();
	virtual ~IFileObjectCreator(void);

	virtual IFileAccessor * CreateFileAccessor()=0;
	
};
