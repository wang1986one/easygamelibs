/****************************************************************************/
/*                                                                          */
/*      文件名:    DlgMapView.h                                             */
/*      创建日期:  2011年10月07日                                           */
/*      作者:      Sagasarate                                               */
/*                                                                          */
/*      本软件版权归Sagasarate(sagasarate@sina.com)所有                     */
/*      你可以将本软件用于任何商业和非商业软件开发，但                      */
/*      必须保留此版权声明                                                  */
/*                                                                          */
/****************************************************************************/
#pragma once
#include "afxwin.h"


// CDlgMapView 对话框

class CDlgMapView : public CDialog
{
	DECLARE_DYNAMIC(CDlgMapView)
protected:
	UINT		m_MapID;
	HBITMAP		m_hbmpMap;
	CDC			m_MapDC;
	UINT		m_MapWidth;
	UINT		m_MapHeight;
	UINT		m_StartBlockX;
	UINT		m_StartBlockY;
	int			m_MapShowX;
	int			m_MapShowY;
	bool		m_IsInDrag;
	int			m_DragStartX;
	int			m_DragStartY;
	int			m_ViewWidth;
	int			m_ViewHeight;

	CPen		m_Pen;

	CEasyArray<CBLZWOWDatabase::BLZ_DB_LIGHT_INFO *> * m_pLightInfos;
	
public:
	CDlgMapView(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CDlgMapView();

	UINT		m_SelectedMapX;
	UINT		m_SelectedMapY;
// 对话框数据
	enum { IDD = IDD_DIALOG_MAP_VIEW };

	UINT GetMapID()
	{
		return m_MapID;
	}

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();

	void SetMap(UINT MapID,HBITMAP hbmpMap,UINT Width,UINT Height,UINT StartBlockX,UINT StartBlockY);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	virtual BOOL PreTranslateMessage(MSG* pMsg);
};
