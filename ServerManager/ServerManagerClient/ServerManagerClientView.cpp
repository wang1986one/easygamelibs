// ServerManagerClientView.cpp : CServerManagerClientView 类的实现
//

#include "stdafx.h"
#include "ServerManagerClientView.h"



#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CServerManagerClientView

IMPLEMENT_DYNCREATE(CServerManagerClientView, CFormView)

BEGIN_MESSAGE_MAP(CServerManagerClientView, CFormView)
	ON_WM_SIZE()
	
	ON_NOTIFY(NM_RCLICK, IDC_SERVICE_LIST, &CServerManagerClientView::OnNMRClickServiceList)
	ON_COMMAND(ID_STARTUP_SERVICE, &CServerManagerClientView::OnStartupService)
	ON_COMMAND(ID_SHUTDOWN_SERVCE, &CServerManagerClientView::OnShutdownServce)
	ON_NOTIFY(LVN_COLUMNCLICK, IDC_SERVICE_LIST, &CServerManagerClientView::OnLvnColumnclickServiceList)
	ON_COMMAND(ID_POPUP_BROWSE_WORK_DIR, &CServerManagerClientView::OnPopupBrowseWorkDir)
	ON_COMMAND(ID_SHOW_HIDE_SERVICE, &CServerManagerClientView::OnShowHideService)
	ON_UPDATE_COMMAND_UI(ID_SHOW_HIDE_SERVICE, &CServerManagerClientView::OnUpdateShowHideService)
END_MESSAGE_MAP()

// CServerManagerClientView 构造/析构

CServerManagerClientView::CServerManagerClientView()
	: CFormView(CServerManagerClientView::IDD)
{
	// TODO: 在此处添加构造代码
	m_ShowHideService=false;
}

CServerManagerClientView::~CServerManagerClientView()
{
}

void CServerManagerClientView::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_TYPE_TREE, m_tvShowType);
	DDX_Control(pDX, IDC_SERVICE_LIST, m_lvServiceInfos);
	DDX_Control(pDX, IDC_EDIT_LOG, m_edLog);
}

BOOL CServerManagerClientView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: 在此处通过修改
	//  CREATESTRUCT cs 来修改窗口类或样式

	return CFormView::PreCreateWindow(cs);
}

void CServerManagerClientView::OnInitialUpdate()
{
	CFormView::OnInitialUpdate();
	GetParentFrame()->RecalcLayout();
	ResizeParentToFit();

	m_DialogItemSorter.SetDialog(this);
	m_DialogItemSorter.SetSortType(0,CDialogItemSorter::SORT_HORIZONTAL|CDialogItemSorter::PARAM_ALIGN_SPACE,5);

	m_DialogItemSorter.AddDialogItem(0,1,
		CDialogItemSorter::SORT_HORIZONTAL|
		CDialogItemSorter::RESIZE_FIT_PARENT_HEIGHT,
		5,IDC_TYPE_TREE,CRect(0,0,100,0));
	m_DialogItemSorter.AddDialogItem(0,2,
		CDialogItemSorter::SORT_VERTICAL|
		CDialogItemSorter::RESIZE_FIT_PARENT_WIDTH|CDialogItemSorter::RESIZE_FIT_PARENT_HEIGHT,
		10,IDC_SERVICE_LIST,CRect(0,0,0,0));

	m_DialogItemSorter.AddDialogItem(2,21,		
		CDialogItemSorter::RESIZE_FIT_PARENT_WIDTH|CDialogItemSorter::RESIZE_FIT_PARENT_HEIGHT,
		0,IDC_SERVICE_LIST,CRect(0,0,0,0));

	m_DialogItemSorter.AddDialogItem(2,22,		
		CDialogItemSorter::RESIZE_FIT_PARENT_WIDTH,
		0,IDC_EDIT_LOG,CRect(0,0,0,100));

	HTREEITEM hRoot=m_tvShowType.InsertItem("所有",NULL,NULL);
	m_hGroupByServer=m_tvShowType.InsertItem("按服务器分类",hRoot,NULL);
	m_hGroupByService=m_tvShowType.InsertItem("按服务类型分类",hRoot,NULL);	
	m_tvShowType.Expand(hRoot,TVE_EXPAND);

	m_lvServiceInfos.InsertColumn(0,"服务器",LVCFMT_LEFT,120);
	m_lvServiceInfos.InsertColumn(1,"服务名称",LVCFMT_LEFT,100);
	m_lvServiceInfos.InsertColumn(2,"状态",LVCFMT_CENTER,50);
	m_lvServiceInfos.InsertColumn(3,"CPU占用率",LVCFMT_RIGHT,100);
	m_lvServiceInfos.InsertColumn(4,"内存占用",LVCFMT_RIGHT,90);
	m_lvServiceInfos.InsertColumn(5,"虚拟内存占用",LVCFMT_RIGHT,100);
	m_lvServiceInfos.InsertColumn(6,"外网流量(发送/接收)",LVCFMT_RIGHT,130);
	m_lvServiceInfos.InsertColumn(7,"内网流量(发送/接收)",LVCFMT_RIGHT,130);
	m_lvServiceInfos.InsertColumn(8,"可执行文件日期",LVCFMT_CENTER,130);
	m_lvServiceInfos.InsertColumn(9,"可执行文件路径",LVCFMT_LEFT,300);
	

	m_lvServiceInfos.SetExtendedStyle(LVS_EX_GRIDLINES|LVS_EX_FULLROWSELECT);


	m_DlgWorkDirBowser.Create(m_DlgWorkDirBowser.IDD,AfxGetMainWnd());
}


// CServerManagerClientView 诊断

#ifdef _DEBUG
void CServerManagerClientView::AssertValid() const
{
	CFormView::AssertValid();
}

void CServerManagerClientView::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}

CServerManagerClientDoc* CServerManagerClientView::GetDocument() const // 非调试版本是内联的
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CServerManagerClientDoc)));
	return (CServerManagerClientDoc*)m_pDocument;
}
#endif //_DEBUG


// CServerManagerClientView 消息处理程序

void CServerManagerClientView::OnSize(UINT nType, int cx, int cy)
{
	CFormView::OnSize(nType, cx, cy);

	// TODO: 在此处添加消息处理程序代码
	m_DialogItemSorter.DoSort();
}

void CServerManagerClientView::PrintLogVL(LPCTSTR szFormat,va_list vl)
{
	char LogBuffer[1000];


	CEasyTime CurTime;
	CurTime.FetchLocalTime();


	sprintf_s(LogBuffer,1000,"[%02d-%02d][%02d:%02d:%02d]:",
		CurTime.Month(),CurTime.Day(),
		CurTime.Hour(),CurTime.Minute(),CurTime.Second());


	vsprintf_s(LogBuffer+17,1000-17,szFormat, vl );
	

	int s1,s2;
	int sm1,sm2;
	int SelLine;
	int EndLine;

	m_edLog.SetRedraw(false);

	m_edLog.GetSel(sm1,sm2);
	m_edLog.SetSel(0,-1);
	m_edLog.GetSel(s1,s2);
	SelLine=m_edLog.LineFromChar(sm1);
	EndLine=m_edLog.GetLineCount()-1;
	if(s2>MAX_LOG_MSG_LEN)
	{
		m_edLog.SetSel(0,-1);
		m_edLog.Clear();
		s2=0;
	}
	m_edLog.SetSel(s2,s2);	
	m_edLog.ReplaceSel(LogBuffer);


	m_edLog.SetSel(0,-1);
	m_edLog.GetSel(s1,s2);
	m_edLog.SetSel(s2,s2);	

	m_edLog.ReplaceSel("\r\n");



	m_edLog.SetRedraw(true);	


	if(SelLine==EndLine)
		m_edLog.LineScroll(m_edLog.GetLineCount());
	else
		m_edLog.SetSel(sm1,sm2);
}

void CServerManagerClientView::PrintLog(LPCTSTR szFormat,...)
{
	va_list vl;

	va_start(vl,szFormat);
	PrintLogVL(szFormat,vl);
	va_end(vl);
}

void CServerManagerClientView::SetServiceInfo(UINT Index,LPCTSTR szServerAddress,LPCTSTR Name,LPCTSTR ImageFilePath,time_t ImageFileTime,UINT Status,float CPUUsed,UINT MemoryUsed,UINT VirtualMemoryUsed)
{
	if((!m_ShowHideService)&&Index==0)
		return;
	int Item=-1;
	for(int i=0;i<m_lvServiceInfos.GetItemCount();i++)
	{
		UINT ServiceIndex=m_lvServiceInfos.GetItemData(i);
		CString ServerAddress=m_lvServiceInfos.GetItemText(i,0);
		if(ServerAddress==szServerAddress&&ServiceIndex==Index)
		{
			Item=i;
		}
	}
	if(Item<0)
	{
		Item=m_lvServiceInfos.InsertItem(m_lvServiceInfos.GetItemCount(),szServerAddress);
		m_lvServiceInfos.SetItemData(Item,Index);
	}
	if(Item>=0)
	{
		CEasyString Temp;
		m_lvServiceInfos.SetItemText(Item,0,szServerAddress);
		Temp.Format("[%u]%s",Index,Name);
		m_lvServiceInfos.SetItemText(Item,1,Temp);
		if(Status==SS_RUNING)
		{
			Temp="运行中";
		}
		else
		{
			Temp="已停止";
		}
		m_lvServiceInfos.SetItemText(Item,2,Temp);
		Temp.Format("%.5g%%",CPUUsed*100);
		m_lvServiceInfos.SetItemText(Item,3,Temp);
		Temp=FormatNumberWords(MemoryUsed);
		m_lvServiceInfos.SetItemText(Item,4,Temp);
		Temp=FormatNumberWords(VirtualMemoryUsed);
		m_lvServiceInfos.SetItemText(Item,5,Temp);		
		if(ImageFileTime==-1)
		{
			Temp="无";
		}
		else
		{
			CEasyTime Time(ImageFileTime);
			Time.Format(Temp,"%Y-%m-%d %H:%M:%S");
		}		
		m_lvServiceInfos.SetItemText(Item,8,Temp);
		m_lvServiceInfos.SetItemText(Item,9,ImageFilePath);

	}
}

void CServerManagerClientView::SetInternetAdapterInfo(LPCTSTR szServerAddress,float SendFlux,float RecvFlux)
{
	for(int i=0;i<m_lvServiceInfos.GetItemCount();i++)
	{
		UINT ServiceIndex=m_lvServiceInfos.GetItemData(i);
		CString ServerAddress=m_lvServiceInfos.GetItemText(i,0);
		if(ServerAddress==szServerAddress)
		{
			CEasyString Temp;
			Temp.Format("%s/%s",
				(LPCTSTR)FormatNumberWordsFloat(SendFlux,true),
				(LPCTSTR)FormatNumberWordsFloat(RecvFlux,true));
			m_lvServiceInfos.SetItemText(i,6,Temp);
		}
	}
}

void CServerManagerClientView::SetIntranetAdapterInfo(LPCTSTR szServerAddress,float SendFlux,float RecvFlux)
{
	for(int i=0;i<m_lvServiceInfos.GetItemCount();i++)
	{
		UINT ServiceIndex=m_lvServiceInfos.GetItemData(i);
		CString ServerAddress=m_lvServiceInfos.GetItemText(i,0);
		if(ServerAddress==szServerAddress)
		{
			CEasyString Temp;
			Temp.Format("%s/%s",
				(LPCTSTR)FormatNumberWordsFloat(SendFlux,true),
				(LPCTSTR)FormatNumberWordsFloat(RecvFlux,true));
			m_lvServiceInfos.SetItemText(i,7,Temp);
		}
	}
}

void CServerManagerClientView::DisconnectNotify(LPCTSTR szServerAddress)
{
	for(int i=0;i<m_lvServiceInfos.GetItemCount();i++)
	{		
		CString ServerAddress=m_lvServiceInfos.GetItemText(i,0);
		if(ServerAddress==szServerAddress)
		{
			m_lvServiceInfos.SetItemText(i,2,"已断开");
		}
	}
}

void CServerManagerClientView::OnBrowseResult(LPCTSTR szDir,LPCTSTR szFileName,DWORD FileAttr,ULONG64 FileSize,time_t CreateTime,time_t LastAccessTime,time_t LastWriteTime)
{
	m_DlgWorkDirBowser.OnBrowseResult(szDir,szFileName,FileAttr,FileSize,CreateTime,LastAccessTime,LastWriteTime);
}

void  CServerManagerClientView::OnBrowseFinish()
{	
	m_DlgWorkDirBowser.OnBrowseFinish();
}

void CServerManagerClientView::OnNMRClickServiceList(NMHDR *pNMHDR, LRESULT *pResult)
{
	//LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<NMITEMACTIVATE>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码
	CPoint MousePoint;
	GetCursorPos(&MousePoint);

	CMenu Menu;
	VERIFY(Menu.LoadMenu(IDR_SERVICE_LIST_POPUP));

	CMenu* pPopup = Menu.GetSubMenu(0);
	ASSERT(pPopup != NULL);
	CWnd* pWndPopupOwner = this;

	while (pWndPopupOwner->GetStyle() & WS_CHILD)
		pWndPopupOwner = pWndPopupOwner->GetParent();

	
	pPopup->TrackPopupMenu(TPM_LEFTALIGN | TPM_RIGHTBUTTON, MousePoint.x, MousePoint.y,
		pWndPopupOwner);
	*pResult = 0;
}

void CServerManagerClientView::OnStartupService()
{
	// TODO: 在此添加命令处理程序代码
	POSITION Pos=m_lvServiceInfos.GetFirstSelectedItemPosition();

	while(Pos)
	{
		int Item=m_lvServiceInfos.GetNextSelectedItem(Pos);
		UINT Index=m_lvServiceInfos.GetItemData(Item);
		CString ServerAddress=m_lvServiceInfos.GetItemText(Item,0);
		CServerManagerClientApp::GetInstance()->StartupService(ServerAddress,Index);
	}
}

void CServerManagerClientView::OnShutdownServce()
{
	// TODO: 在此添加命令处理程序代码
	POSITION Pos=m_lvServiceInfos.GetFirstSelectedItemPosition();

	while(Pos)
	{
		int Item=m_lvServiceInfos.GetNextSelectedItem(Pos);
		UINT Index=m_lvServiceInfos.GetItemData(Item);
		CString ServerAddress=m_lvServiceInfos.GetItemText(Item,0);
		CServerManagerClientApp::GetInstance()->ShutdownService(ServerAddress,Index);
	}
}

void CServerManagerClientView::OnLvnColumnclickServiceList(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码
	*pResult = 0;

	m_SortParam.pListCtrl=&m_lvServiceInfos;
	m_SortParam.SortColumn=pNMLV->iSubItem;

	m_lvServiceInfos.SortItemsEx(&CServerManagerClientView::ItemSortCallBack,(DWORD_PTR)&m_SortParam);
}


int CServerManagerClientView::ItemSortCallBack(LPARAM lParam1, LPARAM lParam2,LPARAM lParamSort)
{
	SORT_PARAM * pSortParam=(SORT_PARAM *)lParamSort;

	CString Text1=pSortParam->pListCtrl->GetItemText(lParam1,pSortParam->SortColumn);
	CString Text2=pSortParam->pListCtrl->GetItemText(lParam2,pSortParam->SortColumn);

	return Text1.CompareNoCase(Text2);
}
void CServerManagerClientView::OnPopupBrowseWorkDir()
{
	// TODO: 在此添加命令处理程序代码
	POSITION Pos=m_lvServiceInfos.GetFirstSelectedItemPosition();

	if(Pos)
	{
		int Item=m_lvServiceInfos.GetNextSelectedItem(Pos);
		UINT Index=m_lvServiceInfos.GetItemData(Item);
		CString ServerAddress=m_lvServiceInfos.GetItemText(Item,0);
		m_DlgWorkDirBowser.ShowWindow(SW_SHOW);
		m_DlgWorkDirBowser.SetServerInfo(this,ServerAddress,Index);
		m_DlgWorkDirBowser.Browse("");		
	}
}

void CServerManagerClientView::OnShowHideService()
{
	// TODO: 在此添加命令处理程序代码
	if(m_ShowHideService)
		m_ShowHideService=false;
	else
		m_ShowHideService=true;
	m_lvServiceInfos.DeleteAllItems();
}

void CServerManagerClientView::OnUpdateShowHideService(CCmdUI *pCmdUI)
{
	// TODO: 在此添加命令更新用户界面处理程序代码
	pCmdUI->SetCheck(m_ShowHideService);
}
