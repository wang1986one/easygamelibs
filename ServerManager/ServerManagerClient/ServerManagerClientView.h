// ServerManagerClientView.h : CServerManagerClientView 类的接口
//


#pragma once


class CServerManagerClientDoc;

class CServerManagerClientView : public CFormView
{
protected: // 仅从序列化创建

	struct SORT_PARAM
	{
		CListCtrl *		pListCtrl;
		int				SortColumn;
	};

	CDialogItemSorter		m_DialogItemSorter;
	SORT_PARAM				m_SortParam;
	
	HTREEITEM				m_hGroupByServer;
	HTREEITEM				m_hGroupByService;

	CDlgWorkDirBowser		m_DlgWorkDirBowser;
	CTreeCtrl				m_tvShowType;
	CListCtrl				m_lvServiceInfos;
	CEdit					m_edLog;

	bool					m_ShowHideService;


	CServerManagerClientView();
	DECLARE_DYNCREATE(CServerManagerClientView)

public:
	enum{ IDD = IDD_SERVERMANAGERCLIENT_FORM };

// 属性
public:
	CServerManagerClientDoc* GetDocument() const;

// 操作
public:
	void PrintLogVL(LPCTSTR szFormat,va_list vl);
	void PrintLog(LPCTSTR szFormat,...);
	void SetServiceInfo(UINT Index,LPCTSTR szServerAddress,LPCTSTR Name,LPCTSTR ImageFilePath,time_t ImageFileTime,UINT Status,float CPUUsed,UINT MemoryUsed,UINT VirtualMemoryUsed);
	void SetInternetAdapterInfo(LPCTSTR szServerAddress,float SendFlux,float RecvFlux);
	void SetIntranetAdapterInfo(LPCTSTR szServerAddress,float SendFlux,float RecvFlux);
	void DisconnectNotify(LPCTSTR szServerAddress);
	void OnBrowseResult(LPCTSTR szDir,LPCTSTR szFileName,DWORD FileAttr,ULONG64 FileSize,time_t CreateTime,time_t LastAccessTime,time_t LastWriteTime);
	void OnBrowseFinish();

	CDlgWorkDirBowser * GetWorkDirBrowser()
	{
		return &m_DlgWorkDirBowser;
	}
// 重写
public:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
	virtual void OnInitialUpdate(); // 构造后第一次调用

// 实现
public:
	virtual ~CServerManagerClientView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

	static int CALLBACK ItemSortCallBack(LPARAM lParam1, LPARAM lParam2,LPARAM lParamSort);

// 生成的消息映射函数
protected:
	DECLARE_MESSAGE_MAP()

	afx_msg void OnSize(UINT nType, int cx, int cy);	
	afx_msg void OnNMRClickServiceList(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnStartupService();
	afx_msg void OnShutdownServce();
	afx_msg void OnLvnColumnclickServiceList(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnPopupBrowseWorkDir();
public:
	afx_msg void OnShowHideService();
	afx_msg void OnUpdateShowHideService(CCmdUI *pCmdUI);
};

#ifndef _DEBUG  // ServerManagerClientView.cpp 中的调试版本
inline CServerManagerClientDoc* CServerManagerClientView::GetDocument() const
   { return reinterpret_cast<CServerManagerClientDoc*>(m_pDocument); }
#endif

