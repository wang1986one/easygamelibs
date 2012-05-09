/****************************************************************************/
/*                                                                          */
/*      �ļ���:    USOTreeView.h                                            */
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
/*      �ļ���:    USOTreeView.h                                            */
/*      ��������:  2009��07��06��                                           */
/*      ����:      Sagasarate                                               */
/*                                                                          */
/*      �������Ȩ��Sagasarate(sagasarate@sina.com)����                     */
/*      ����Խ�����������κ���ҵ�ͷ���ҵ�����������                      */
/*      ���뱣���˰�Ȩ����                                                  */
/*                                                                          */
/****************************************************************************/
// USOTreeView.h : CUSOTreeView ��Ľӿ�
//


#pragma once

class CUSOEditDoc;

class CUSOTreeView : public CTreeView
{
protected: // �������л�����
	CUSOTreeView();
	DECLARE_DYNCREATE(CUSOTreeView)

// ����
public:
	CUSOEditDoc* GetDocument();

// ����
public:

// ��д
	public:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	protected:
	virtual void OnInitialUpdate(); // ������һ�ε���

// ʵ��
public:
	virtual ~CUSOTreeView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

	CUSOObject * GetSelectedObject();

protected:

// ���ɵ���Ϣӳ�亯��
protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnTvnSelchanged(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnNMRclick(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnTvnBegindrag(NMHDR *pNMHDR, LRESULT *pResult);


};

#ifndef _DEBUG  // USOTreeView.cpp �ĵ��԰汾
inline CUSOEditDoc* CUSOTreeView::GetDocument()
   { return reinterpret_cast<CUSOEditDoc*>(m_pDocument); }
#endif

