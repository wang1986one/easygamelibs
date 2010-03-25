/****************************************************************************/
/*                                                                          */
/*      �ļ���:    WinFileAccessorObjectCreator.h                           */
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

class CWinFileAccessorObjectCreator :
	public IFileObjectCreator
{
protected:
	DECLARE_CLASS_INFO_STATIC(CWinFileAccessorObjectCreator);
public:
	CWinFileAccessorObjectCreator(void);
	virtual ~CWinFileAccessorObjectCreator(void);

	virtual IFileAccessor * CreateFileAccessor();
};
