/****************************************************************************/
/*                                                                          */
/*      �ļ���:    DlgMapView.h                                             */
/*      ��������:  2011��10��07��                                           */
/*      ����:      Sagasarate                                               */
/*                                                                          */
/*      �������Ȩ��Sagasarate(sagasarate@sina.com)����                     */
/*      ����Խ�����������κ���ҵ�ͷ���ҵ�����������                      */
/*      ���뱣���˰�Ȩ����                                                  */
/*                                                                          */
/****************************************************************************/
#pragma once
#include "afxwin.h"


// CDlgMapView �Ի���

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
	CDlgMapView(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CDlgMapView();

	UINT		m_SelectedMapX;
	UINT		m_SelectedMapY;
// �Ի�������
	enum { IDD = IDD_DIALOG_MAP_VIEW };

	UINT GetMapID()
	{
		return m_MapID;
	}

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();

	void SetMap(UINT MapID,HBITMAP hbmpMap,UINT Width,UINT Height,UINT StartBlockX,UINT StartBlockY);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	virtual BOOL PreTranslateMessage(MSG* pMsg);
};
