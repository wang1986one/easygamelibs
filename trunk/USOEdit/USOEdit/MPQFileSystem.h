/****************************************************************************/
/*                                                                          */
/*      文件名:    MPQFileSystem.h                                          */
/*      创建日期:  2011年10月07日                                           */
/*      作者:      Sagasarate                                               */
/*                                                                          */
/*      本软件版权归Sagasarate(sagasarate@sina.com)所有                     */
/*      你可以将本软件用于任何商业和非商业软件开发，但                      */
/*      必须保留此版权声明                                                  */
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
