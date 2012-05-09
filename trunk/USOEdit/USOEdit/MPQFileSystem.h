/****************************************************************************/
/*                                                                          */
/*      �ļ���:    MPQFileSystem.h                                          */
/*      ��������:  2011��10��07��                                           */
/*      ����:      Sagasarate                                               */
/*                                                                          */
/*      �������Ȩ��Sagasarate(sagasarate@sina.com)����                     */
/*      ����Խ�����������κ���ҵ�ͷ���ҵ�����������                      */
/*      ���뱣���˰�Ȩ����                                                  */
/*                                                                          */
/****************************************************************************/
#pragma once

class CMPQFileSystem :
	public IFileObjectCreator
{
protected:
	DECLARE_CLASS_INFO_STATIC(CMPQFileSystem)

	CEasyArray<HANDLE>		m_MPQList;
public:
	CMPQFileSystem(void);
	~CMPQFileSystem(void);

	virtual void Destory();

	virtual IFileAccessor * CreateFileAccessor();

	bool AddMPQ(LPCTSTR szMPQFileName);
	void ClearAllMPQ();

	HANDLE FindFileMPQ(LPCTSTR szFileName);
	void GetFileList(CEasyArray<CEasyString>& FileNameList);
};
