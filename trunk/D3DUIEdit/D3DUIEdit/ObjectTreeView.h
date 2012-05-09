/****************************************************************************/
/*                                                                          */
/*      �ļ���:    ObjectTreeView.h                                         */
/*      ��������:  2011��10��07��                                           */
/*      ����:      Sagasarate                                               */
/*                                                                          */
/*      �������Ȩ��Sagasarate(sagasarate@sina.com)����                     */
/*      ����Խ�����������κ���ҵ�ͷ���ҵ�����������                      */
/*      ���뱣���˰�Ȩ����                                                  */
/*                                                                          */
/****************************************************************************/
#pragma once


// CObjectTreeView ��ͼ

class CD3DUIEditDoc;

class CObjectTreeView : public CTreeView
{
	DECLARE_DYNCREATE(CObjectTreeView)

protected:
	CObjectTreeView();           // ��̬������ʹ�õ��ܱ����Ĺ��캯��
	virtual ~CObjectTreeView();

public:
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:
	DECLARE_MESSAGE_MAP()
public:
	inline CD3DUIEditDoc* GetDocument() const
	{ 
		return reinterpret_cast<CD3DUIEditDoc*>(m_pDocument); 
	}
protected:
	HTREEITEM	m_ObjectParentItem;
public:
	virtual void OnInitialUpdate();
	void AddGUIObject(CD3DWnd * pWnd);
	void UpdateGUIObject(CD3DSizeBar * pWnd);
	HTREEITEM GetParentItem(CD3DWnd * pWnd);
protected:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
public:
	afx_msg void OnTvnSelchanged(NMHDR *pNMHDR, LRESULT *pResult);
};


