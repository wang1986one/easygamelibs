/****************************************************************************/
/*                                                                          */
/*      文件名:    USOTreeView.h                                            */
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
/*      文件名:    USOTreeView.h                                            */
/*      创建日期:  2009年07月06日                                           */
/*      作者:      Sagasarate                                               */
/*                                                                          */
/*      本软件版权归Sagasarate(sagasarate@sina.com)所有                     */
/*      你可以将本软件用于任何商业和非商业软件开发，但                      */
/*      必须保留此版权声明                                                  */
/*                                                                          */
/****************************************************************************/
// USOTreeView.h : CUSOTreeView 类的接口
//


#pragma once

class CUSOEditDoc;

class CUSOTreeView : public CTreeView
{
protected: // 仅从序列化创建
	CUSOTreeView();
	DECLARE_DYNCREATE(CUSOTreeView)

// 属性
public:
	CUSOEditDoc* GetDocument();

// 操作
public:

// 重写
	public:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	protected:
	virtual void OnInitialUpdate(); // 构造后第一次调用

// 实现
public:
	virtual ~CUSOTreeView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

	CUSOObject * GetSelectedObject();

protected:

// 生成的消息映射函数
protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnTvnSelchanged(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnNMRclick(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnTvnBegindrag(NMHDR *pNMHDR, LRESULT *pResult);


};

#ifndef _DEBUG  // USOTreeView.cpp 的调试版本
inline CUSOEditDoc* CUSOTreeView::GetDocument()
   { return reinterpret_cast<CUSOEditDoc*>(m_pDocument); }
#endif

