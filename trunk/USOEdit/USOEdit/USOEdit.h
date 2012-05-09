/****************************************************************************/
/*                                                                          */
/*      �ļ���:    USOEdit.h                                                */
/*      ��������:  2011��10��07��                                           */
/*      ����:      Sagasarate                                               */
/*                                                                          */
/*      �������Ȩ��Sagasarate(sagasarate@sina.com)����                     */
/*      ����Խ�����������κ���ҵ�ͷ���ҵ�����������                      */
/*      ���뱣���˰�Ȩ����                                                  */
/*                                                                          */
/****************************************************************************/
/****************************************************************************/
/*                                                                          */
/*      �ļ���:    USOEdit.h                                                */
/*      ��������:  2010��02��09��                                           */
/*      ����:      Sagasarate                                               */
/*                                                                          */
/*      �������Ȩ��Sagasarate(sagasarate@sina.com)����                     */
/*      ����Խ�����������κ���ҵ�ͷ���ҵ�����������                      */
/*      ���뱣���˰�Ȩ����                                                  */
/*                                                                          */
/****************************************************************************/
// USOEdit.h : USOEdit Ӧ�ó������ͷ�ļ�
//
#pragma once

#ifndef __AFXWIN_H__
	#error �ڰ������� PCH �Ĵ��ļ�֮ǰ������stdafx.h�� 
#endif

#include "resource.h"       // ������
//#include "D3DViewPort.h"

// CUSOEditApp:
// �йش����ʵ�֣������ USOEdit.cpp
//

class CD3DViewPort;

class CUSOEditApp : public CWinApp
{
public:
	CUSOEditApp();

protected:	
	CArray<CD3DViewPort *,CD3DViewPort *>		m_D3DViewPortList;
	CEasyArray<CEasyString>						m_TexturePathList;
	CEasyArray<CEasyString>						m_FXPathList;
	CEasyArray<CEasyString>						m_MPQFileList;
	CEasyString									m_BLZDBCPath;
	CAMERA_SETTING								m_CameraSetting;
	VIEWPORT_SETTING							m_ViewportSetting;
	CMPQFileSystem								m_MPQFileSystem;
	CDlgFileViewer								m_DlgFileViewer;
	
	
// ��д
public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();
// ʵ��
	afx_msg void OnAppAbout();
	DECLARE_MESSAGE_MAP()
	virtual BOOL OnIdle(LONG lCount);
	virtual BOOL OnDDECommand(LPTSTR lpszCommand);
public:
	static CUSOEditApp * GetInstance();
	void AddD3DViewPort(CD3DViewPort * pD3DViewPort)
	{
		DeleteD3DViewPort(pD3DViewPort);
		m_D3DViewPortList.Add(pD3DViewPort);
	}
	void DeleteD3DViewPort(CD3DViewPort * pD3DViewPort)
	{
		for(int i=0;i<m_D3DViewPortList.GetCount();i++)
		{
			if(m_D3DViewPortList[i]==pD3DViewPort)
			{
				m_D3DViewPortList.RemoveAt(i);
				return;
			}
		}
	}
	CEasyArray<CEasyString>& GetTexturePathList()
	{
		return m_TexturePathList;
	}
	CEasyArray<CEasyString>& GetFXPathList()
	{
		return m_FXPathList;
	}

	CAMERA_SETTING& GetCameraSetting()
	{
		return m_CameraSetting;
	}
	VIEWPORT_SETTING& GetViewportSetting()
	{
		return m_ViewportSetting;
	}

	CMPQFileSystem& GetMPQFileSystem()
	{
		return m_MPQFileSystem;
	}
	CDlgFileViewer& GetFileViewer()
	{
		return m_DlgFileViewer;
	}
	
	
	afx_msg void OnSetTexturePath();
	afx_msg void OnSetFxPath();
	afx_msg void OnLoadBlzDbc();


	void LoadSetting();
	void SaveSetting();
public:
	
	
	afx_msg void OnSetMpqFile();
};




