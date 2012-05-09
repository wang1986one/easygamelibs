/****************************************************************************/
/*                                                                          */
/*      文件名:    USOPropertyView.h                                        */
/*      创建日期:  2011年10月07日                                           */
/*      作者:      Sagasarate                                               */
/*                                                                          */
/*      本软件版权归Sagasarate(sagasarate@sina.com)所有                     */
/*      你可以将本软件用于任何商业和非商业软件开发，但                      */
/*      必须保留此版权声明                                                  */
/*                                                                          */
/****************************************************************************/
/****************************************************************************/
/*                                                                          */
/*      文件名:    USOPropertyView.h                                        */
/*      创建日期:  2009年09月11日                                           */
/*      作者:      Sagasarate                                               */
/*                                                                          */
/*      本软件版权归Sagasarate(sagasarate@sina.com)所有                     */
/*      你可以将本软件用于任何商业和非商业软件开发，但                      */
/*      必须保留此版权声明                                                  */
/*                                                                          */
/****************************************************************************/
// USOPropertyView.h : CUSOPropertyView 类的接口
//


#pragma once


class CUSOEditDoc;

class CUSOPropertyView : public CPropertyView
{
protected: // 仅从序列化创建
	CUSOPropertyView();
	DECLARE_DYNCREATE(CUSOPropertyView)

// 属性

	CUSOObject *					m_pUSOObject;
	CDlgColorConfig					m_DlgDiffuseConfig;
	CDlgColorConfig					m_DlgAmbientConfig;
	CDlgColorConfig					m_DlgSpecularConfig;
	CDlgColorConfig					m_DlgEmissiveConfig;
	CDlgFXEditor					m_DlgFXEditor;
	CDlgFXSelect					m_DlgFXSelect;
	CDlgWOWCharSetting				m_DlgWOWCharSetting;
	CDlgAniSetting					m_DlgAniSetting;
	CDlgTrianglesViewer				m_DlgTrianglesViewer;
	CDlgBoneViewer					m_DlgBoneViewer;
	CDlgAniSequenceViewer			m_DlgAniSequenceViewer;
	CDlgParticleEmitterParamViewer	m_DlgParticleEmitterParamViewer;
	CDlgM2CameraViewer				m_DlgM2CameraViewer;
	CDlgAttachmentViewer			m_DlgAttachmentViewer;
	CDlgM2SkinView					m_DlgM2SkinView;

public:
	CUSOEditDoc* GetDocument();

// 操作
public:

// 重写
	public:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	protected:
	virtual void OnInitialUpdate(); // 构造后第一次调用



public:
	void SetUSOObject(CUSOObject * pUSOObject);
	void RefreshObjectProperty();

// 实现
public:
	virtual ~CUSOPropertyView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

	

// 生成的消息映射函数
	afx_msg LRESULT OnPropertyEditStart( WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnPropertyEditEnd( WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnPropertyChanged( WPARAM wParam, LPARAM lParam);
protected:
	DECLARE_MESSAGE_MAP()
};

#ifndef _DEBUG  // USOPropertyView.cpp 的调试版本
inline CUSOEditDoc* CUSOPropertyView::GetDocument()
   { return reinterpret_cast<CUSOEditDoc*>(m_pDocument); }
#endif

