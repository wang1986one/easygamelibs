/****************************************************************************/
/*                                                                          */
/*      �ļ���:    USOEditDoc.h                                             */
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
/*      �ļ���:    USOEditDoc.h                                             */
/*      ��������:  2010��02��09��                                           */
/*      ����:      Sagasarate                                               */
/*                                                                          */
/*      �������Ȩ��Sagasarate(sagasarate@sina.com)����                     */
/*      ����Խ�����������κ���ҵ�ͷ���ҵ�����������                      */
/*      ���뱣���˰�Ȩ����                                                  */
/*                                                                          */
/****************************************************************************/
// USOEditDoc.h :  CUSOEditDoc ��Ľӿ�
//
#pragma once

#include "DlgImport.h"
#include "DlgWOWM2Import.h"
#include "DlgRenderPerformance.h"
#include <vector>

class CUSOTreeView;
class CUSOPropertyView;
class CD3DView;
class CControlPanelView;
class CD3DViewPort;
class CUSOObject;

typedef std::vector<CD3DFX *> CFXPtrArray;

class CUSOEditDoc : public CDocument
{
protected: // �������л�����
	CUSOEditDoc();
	DECLARE_DYNCREATE(CUSOEditDoc)

// ����
protected:
	CUSOTreeView *				m_pUSOTreeView;
	CUSOPropertyView *			m_pUSOPropertyView;
	CD3DView *					m_pD3DView;
	CControlPanelView *			m_pControlPanelView;
	CD3DViewPort *				m_pD3DViewPort;

	CUSOD3DObjectCreateFilter	m_D3DObjectCreateFilter;
	CUSOResourceManager			m_USOFile;

	CUSOObject *				m_pObjects;
	CUSOObject *				m_pResources;	
	CUSOObject *				m_pWOWM2ModelResource;
	CUSOObject *				m_pTextureResource;
	CUSOObject *				m_pFXResource;
	CUSOObject *				m_pFontResource;
	CUSOObject *				m_pPreview;
	CUSOObject *				m_pSystemObjects;

	CUSOObject *				m_pOtherResource;

	CDlgImport					m_ImportDlg;
	CDlgWOWM2Import				m_WOWM2ImportDlg;
	CDlgWOWCharImport			m_WOWCharImportDlg;
	DlgWOWCreatureImport		m_WOWCreatureImport;
	CDlgWOWItemImport			m_WOWItemImportDlg;
	CDlgWOWWMOImport			m_WOWWMOImportDlg;
	CDlgWOWADTImport			m_WOWADTImportDlg;
	CDlgRenderPerformance		m_RenderPerformance;
	CDlgCameraSetting			m_CameraSetting;
	CDlgTextureViewer			m_TextureViewer;
	CDlgLightSetting			m_SystemLightSetting;
	CDlgSenceTreeView			m_SenceTreeView;
	CDlgHeightTest				m_HeightTest;
	


	CUSOObject *				m_pPreviewUSOObject;

	UINT						m_USOClipFormat;
	UINT						m_CurMapID;
	
// ����
public:

// ��д
	public:
	
	virtual void Serialize(CArchive& ar);

// ʵ��
public:
	virtual ~CUSOEditDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif
	

	CUSOTreeView * GetTreeView()
	{
		return m_pUSOTreeView;
	}
	CUSOPropertyView * GetPropertyView()
	{
		return m_pUSOPropertyView;
	}
	CD3DView * GetD3DView()
	{
		return m_pD3DView;
	}
	CControlPanelView * GetControlPanelView()
	{
		return m_pControlPanelView;
	}
	CD3DViewPort * GetD3DViewPort()
	{
		return m_pD3DViewPort;
	}
	CUSOObject * GetRootObject()
	{
		return m_pObjects;
	}
	UINT GetCurMapID()
	{
		return m_CurMapID;
	}

	bool Init();
	
	
	void UpdateAllTreeItem();
	void GetFXList(CFXPtrArray& FXPtrArray);

	CD3DObject * PickObject(CD3DVector3 Point,CD3DVector3 Dir);
	bool GetHeightByXZ(CD3DVector3 Pos,FLOAT& Height);

	CUSOObject * AddObject(CUSOObject * pParent,CNameObject * pObject,CLASS_INFO * pClassInfo,bool IsAddToUSO,bool IsResource,LPCTSTR ObjectName,LPCTSTR ExtObjectName=NULL);

	void SetPreviewObject(LPCTSTR szFileName,int Type);
	CUSOObject * GetSelectedObject();
protected:
	

	CD3DObject * PickChildObject(CD3DObject * pObject,CD3DVector3 Point,CD3DVector3 Dir,FLOAT& MinDistance);
	void UpdateResources();
	void RePickResource();

// ���ɵ���Ϣӳ�亯��
protected:
	DECLARE_MESSAGE_MAP()
public:
	virtual void OnChangedViewList();
	virtual BOOL OnNewDocument();	
	virtual BOOL OnOpenDocument(LPCTSTR lpszPathName);
	virtual BOOL OnSaveDocument(LPCTSTR lpszPathName);
	virtual void OnCloseDocument();
	
	afx_msg void OnImportX();		
	afx_msg void OnImportUso();

	afx_msg void OnDeleteObj();
	afx_msg void OnClearResource();
	
	afx_msg void OnEditCut();
	afx_msg void OnEditCopy();
	afx_msg void OnEditPaste();

	bool ImportUSO(CUSOObject * pParent,CUSOResourceManager * pResourceManager);
	afx_msg void OnImportTexture();
	afx_msg void OnImportFx();	
	afx_msg void OnImportM2();
	afx_msg void OnSwitchVisible();
	afx_msg void OnHideAllSubmesh();	
	afx_msg void OnImportM2Character();
	afx_msg void OnShowRanderPerformance();
	afx_msg void OnImportM2Item();
	afx_msg void OnImportWmo();
	afx_msg void OnCameraSetting();
	afx_msg void OnImportAdt();
	afx_msg void OnLookObject();
	afx_msg void OnResourceStat();
	afx_msg void OnShowAllSubmesh();

	afx_msg void OnViewTexture();
	afx_msg void OnSystemLightSetting();
	afx_msg void OnFileViewer();
	afx_msg void OnSaveDepthTexture();
	afx_msg void OnImportM2Creature();
	afx_msg void OnCreateScene();
	afx_msg void OnExportToDir();
	afx_msg void OnSenceTreeView();
	afx_msg void OnImportPreviewObject();
	afx_msg void OnHeightTest();
	afx_msg void OnImportWdl();
};


