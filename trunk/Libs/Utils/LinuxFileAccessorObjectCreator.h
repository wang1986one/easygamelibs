/****************************************************************************/
/*                                                                          */
/*      �ļ���:    LinuxFileAccessorObjectCreator.h                         */
/*      ��������:  2009��07��06��                                           */
/*      ����:      Sagasarate                                               */
/*                                                                          */
/*      �������Ȩ��Sagasarate(sagasarate@sina.com)����                     */
/*      ����Խ�����������κ���ҵ�ͷ���ҵ�����������                      */
/*      ���뱣���˰�Ȩ����                                                  */
/*                                                                          */
/****************************************************************************/
#pragma once

class CLinuxFileAccessorObjectCreator :
	public IFileObjectCreator
{
protected:
	DECLARE_CLASS_INFO_STATIC(CLinuxFileAccessorObjectCreator);
public:
	CLinuxFileAccessorObjectCreator(void);
	~CLinuxFileAccessorObjectCreator(void);

	virtual IFileAccessor * CreateFileAccessor();
};
