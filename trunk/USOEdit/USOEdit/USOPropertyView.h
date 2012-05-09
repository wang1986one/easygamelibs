/****************************************************************************/
/*                                                                          */
/*      �ļ���:    USOPropertyView.h                                        */
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
/*      �ļ���:    USOPropertyView.h                                        */
/*      ��������:  2009��09��11��                                           */
/*      ����:      Sagasarate                                               */
/*                                                                          */
/*      �������Ȩ��Sagasarate(sagasarate@sina.com)����                     */
/*      ����Խ�����������κ���ҵ�ͷ���ҵ�����������                      */
/*      ���뱣���˰�Ȩ����                                                  */
/*                                                                          */
/****************************************************************************/
// USOPropertyView.h : CUSOPropertyView ��Ľӿ�
//


#pragma once


class CUSOEditDoc;

class CUSOPropertyView : public CPropertyView
{
protected: // �������л�����
	CUSOPropertyView();
	DECLARE_DYNCREATE(CUSOPropertyView)

// ����

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

// ����
public:

// ��д
	public:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	protected:
	virtual void OnInitialUpdate(); // ������һ�ε���



public:
	void SetUSOObject(CUSOObject * pUSOObject);
	void RefreshObjectProperty();

// ʵ��
public:
	virtual ~CUSOPropertyView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

	

// ���ɵ���Ϣӳ�亯��
	afx_msg LRESULT OnPropertyEditStart( WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnPropertyEditEnd( WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnPropertyChanged( WPARAM wParam, LPARAM lParam);
protected:
	DECLARE_MESSAGE_MAP()
};

#ifndef _DEBUG  // USOPropertyView.cpp �ĵ��԰汾
inline CUSOEditDoc* CUSOPropertyView::GetDocument()
   { return reinterpret_cast<CUSOEditDoc*>(m_pDocument); }
#endif

