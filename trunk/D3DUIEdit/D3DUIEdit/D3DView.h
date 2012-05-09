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
#pragma once

#include "d3dviewport.h"
// CD3DView ��ͼ

class CD3DUIEditDoc;

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

	inline CD3DUIEditDoc* GetDocument() const
	{ 
		return reinterpret_cast<CD3DUIEditDoc*>(m_pDocument); 
	}
};


