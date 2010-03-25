/****************************************************************************/
/*                                                                          */
/*      �ļ���:    StandardFileObjectCreator.h                              */
/*      ��������:  2009��07��06��                                           */
/*      ����:      Sagasarate                                               */
/*                                                                          */
/*      �������Ȩ��Sagasarate(sagasarate@sina.com)����                     */
/*      ����Խ�����������κ���ҵ�ͷ���ҵ�����������                      */
/*      ���뱣���˰�Ȩ����                                                  */
/*                                                                          */
/****************************************************************************/
#pragma once
#include "ifileobjectcreator.h"


class CStandardFileObjectCreator :
	public IFileObjectCreator
{
protected:
	DECLARE_CLASS_INFO_STATIC(CStandardFileObjectCreator);
public:
	CStandardFileObjectCreator(void);
	virtual ~CStandardFileObjectCreator(void);

	virtual IFileAccessor * CreateFileAccessor();
	
};
