/****************************************************************************/
/*                                                                          */
/*      �ļ���:    ObjectPropertyView.h                                     */
/*      ��������:  2011��10��07��                                           */
/*      ����:      Sagasarate                                               */
/*                                                                          */
/*      �������Ȩ��Sagasarate(sagasarate@sina.com)����                     */
/*      ����Խ�����������κ���ҵ�ͷ���ҵ�����������                      */
/*      ���뱣���˰�Ȩ����                                                  */
/*                                                                          */
/****************************************************************************/
#pragma once
#include "DlgTextureConfig.h"
#include "DlgFontConfig.h"
#include "DlgColorConfig.h"
#include "DlgButtonTextureConfig.h"
#include "DlgScrollBarTextureConfig.h"
#include "DlgEditTextureConfig.h"
#include "DlgSimpleListTextureConfig.h"
#include "DlgComboTextureConfig.h"
#include "DlgProgressBarTextureConfig.h"

// CObjectPropertyView ��ͼ
class CD3DUIEditDoc;

class CObjectPropertyView : public CView
{
	DECLARE_DYNCREATE(CObjectPropertyView)

protected:
	CObjectPropertyView();           // ��̬������ʹ�õ��ܱ����Ĺ��캯��
	virtual ~CObjectPropertyView();
protected:
	CPropertyGrid m_PropertyGrid;

public:
	virtual void OnDraw(CDC* pDC);      // ��д�Ի��ƴ���ͼ
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:
	CD3DWnd *						m_CurObject;
	CDlgTextureConfig				m_DlgWndTextureConfig;
	CDlgButtonTextureConfig			m_DlgButtonTextureConfig;
	CDlgScrollBarTextureConfig		m_DlgScrollBarTextureConfig;
	CDlgFontConfig					m_DlgFontConfig; 
	CDlgColorConfig					m_DlgColorConfig;
	CDlgEditTextureConfig			m_DlgEditTextureConfig;
	CDlgSimpleListTextureConfig		m_DlgSimpleListTextureConfig;
	CDlgComboTextureConfig			m_DlgComboTextureConfig;
	CDlgProgressBarTextureConfig	m_DlgProgressBarTextureConfig;

	DECLARE_MESSAGE_MAP()
public:
	virtual void OnInitialUpdate();
	afx_msg void OnSize(UINT nType, int cx, int cy);

	afx_msg LRESULT OnPropertyChanged( WPARAM wParam, LPARAM lParam);

public:
	inline CD3DUIEditDoc* GetDocument() const
	{ 
		return reinterpret_cast<CD3DUIEditDoc*>(m_pDocument); 
	}

	void SetGUIObject(CD3DWnd * pObject);

	void RefreshObjectProperty();
	void UpdateObjectRect();
	void UpdateObjectText();
};


