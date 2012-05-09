/****************************************************************************/
/*                                                                          */
/*      �ļ���:    D3DView.h                                                */
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
/*      �ļ���:    D3DView.h                                                */
/*      ��������:  2009��07��06��                                           */
/*      ����:      Sagasarate                                               */
/*                                                                          */
/*      �������Ȩ��Sagasarate(sagasarate@sina.com)����                     */
/*      ����Խ�����������κ���ҵ�ͷ���ҵ�����������                      */
/*      ���뱣���˰�Ȩ����                                                  */
/*                                                                          */
/****************************************************************************/
#pragma once

#include "d3dviewport.h"
// CD3DView ��ͼ

class CUSOEditDoc;

class CD3DView : public CScrollView
{
	DECLARE_DYNCREATE(CD3DView)

protected:
	CD3DView();           // ��̬������ʹ�õ��ܱ����Ĺ��캯��
	virtual ~CD3DView();

public:
	virtual void OnDraw(CDC* pDC);      // ��д�Ի��ƴ���ͼ
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:
	DECLARE_MESSAGE_MAP()
public:
//	afx_msg void OnTimer(UINT nIDEvent);
	virtual void OnInitialUpdate();


protected:
	CD3DViewPort * m_pViewPort;

protected:
//	virtual BOOL OnWndMsg(UINT message, WPARAM wParam, LPARAM lParam, LRESULT* pResult);
public:
	virtual BOOL Create(LPCTSTR lpszClassName, LPCTSTR lpszWindowName, DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID, CCreateContext* pContext = NULL);


	CD3DGUI * GetGUI()
	{
		return m_pViewPort->GetGUI();
	}

	CD3DViewPort * GetViewPort()
	{
		return m_pViewPort;
	}

	inline CUSOEditDoc* GetDocument() const
	{ 
		return reinterpret_cast<CUSOEditDoc*>(m_pDocument); 
	}
	virtual BOOL PreTranslateMessage(MSG* pMsg);
protected:
//	virtual void OnActivateView(BOOL bActivate, CView* pActivateView, CView* pDeactiveView);
public:
	afx_msg void OnKillFocus(CWnd* pNewWnd);
};


