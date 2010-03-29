/****************************************************************************/
/*                                                                          */
/*      文件名:    DlgWorkDirBowser.cpp                                     */
/*      创建日期:  2010年02月09日                                           */
/*      作者:      Sagasarate                                               */
/*                                                                          */
/*      本软件版权归Sagasarate(sagasarate@sina.com)所有                     */
/*      你可以将本软件用于任何商业和非商业软件开发，但                      */
/*      必须保留此版权声明                                                  */
/*                                                                          */
/****************************************************************************/
// DlgWorkDirBowser.cpp : 实现文件
//

#include "stdafx.h"
#include "DlgWorkDirBowser.h"



#define FILE_ATTRIBUTE_DRIVER              0x10000000  

// CDlgWorkDirBowser 对话框

IMPLEMENT_DYNAMIC(CDlgWorkDirBowser, CDialog)

CDlgWorkDirBowser::CDlgWorkDirBowser(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgWorkDirBowser::IDD, pParent)
{
	m_ServerIndex=0;
	m_pView=NULL;
	m_IsInEdit=false;
}

CDlgWorkDirBowser::~CDlgWorkDirBowser()
{
}

void CDlgWorkDirBowser::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_FILES, m_lvFileList);
	DDX_Control(pDX, IDC_LIST_FILES_LOCAL, m_lvLocalFileList);
}


BEGIN_MESSAGE_MAP(CDlgWorkDirBowser, CDialog)
	ON_NOTIFY(NM_DBLCLK, IDC_LIST_FILES, &CDlgWorkDirBowser::OnNMDblclkListFiles)
	ON_NOTIFY(LVN_COLUMNCLICK, IDC_LIST_FILES, &CDlgWorkDirBowser::OnLvnColumnclickListFiles)
	ON_NOTIFY(LVN_COLUMNCLICK, IDC_LIST_FILES_LOCAL, &CDlgWorkDirBowser::OnLvnColumnclickListFilesLocal)
	ON_NOTIFY(NM_DBLCLK, IDC_LIST_FILES_LOCAL, &CDlgWorkDirBowser::OnNMDblclkListFilesLocal)
	ON_BN_CLICKED(IDC_UPLOAD, &CDlgWorkDirBowser::OnBnClickedUpload)
	ON_BN_CLICKED(IDC_DOWNLOAD, &CDlgWorkDirBowser::OnBnClickedDownload)
	ON_COMMAND(ID_SERVER_CREATE_DIR, &CDlgWorkDirBowser::OnServerCreateDir)
	ON_NOTIFY(NM_RCLICK, IDC_LIST_FILES, &CDlgWorkDirBowser::OnNMRClickListFiles)
	ON_COMMAND(ID_SERVER_EDIT_FILE, &CDlgWorkDirBowser::OnServerEditFile)
	ON_COMMAND(ID_SERVER_RUN_FILE, &CDlgWorkDirBowser::OnServerRunFile)
	ON_COMMAND(ID_SERVER_DELETE_FILE, &CDlgWorkDirBowser::OnServerDeleteFile)
	ON_COMMAND(ID_SERVER_REFRESH, &CDlgWorkDirBowser::OnServerRefresh)
	ON_COMMAND(ID_LOCAL_REFRESH, &CDlgWorkDirBowser::OnLocalRefresh)
	ON_NOTIFY(NM_RCLICK, IDC_LIST_FILES_LOCAL, &CDlgWorkDirBowser::OnNMRClickListFilesLocal)
	ON_COMMAND(ID_SERVER_RUN_SCRIPT, &CDlgWorkDirBowser::OnServerRunScript)
END_MESSAGE_MAP()


// CDlgWorkDirBowser 消息处理程序



BOOL CDlgWorkDirBowser::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  在此添加额外的初始化


	m_TransferProgress.Create(m_TransferProgress.IDD);

	GetSHIcon("",FILE_ATTRIBUTE_DIRECTORY,true);
	GetSHIcon("",FILE_ATTRIBUTE_DIRECTORY,false);

	m_lvFileList.SetImageList(&m_SmallIcons,LVSIL_SMALL);
	m_lvFileList.SetImageList(&m_LargeIcons,LVSIL_NORMAL);

	m_lvFileList.SetExtendedStyle(m_lvFileList.GetExtendedStyle()|LVS_EX_FULLROWSELECT);

	m_lvFileList.InsertColumn(0,"文件名",LVCFMT_LEFT,200);
	m_lvFileList.InsertColumn(1,"大小",LVCFMT_RIGHT,100);
	m_lvFileList.InsertColumn(2,"创建日期",LVCFMT_LEFT,130);
	m_lvFileList.InsertColumn(3,"修改日期",LVCFMT_LEFT,130);


	m_lvLocalFileList.SetImageList(&m_SmallIcons,LVSIL_SMALL);
	m_lvLocalFileList.SetImageList(&m_LargeIcons,LVSIL_NORMAL);

	m_lvLocalFileList.SetExtendedStyle(m_lvLocalFileList.GetExtendedStyle()|LVS_EX_FULLROWSELECT);

	m_lvLocalFileList.InsertColumn(0,"文件名",LVCFMT_LEFT,200);
	m_lvLocalFileList.InsertColumn(1,"大小",LVCFMT_RIGHT,100);
	m_lvLocalFileList.InsertColumn(2,"创建日期",LVCFMT_LEFT,130);
	m_lvLocalFileList.InsertColumn(3,"修改日期",LVCFMT_LEFT,130);

	BrowseLocal(".\\");

	m_RefreshTimer.SaveTime();

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}


int CDlgWorkDirBowser::GetSHIcon(LPCTSTR FileType,DWORD FileAttr,BOOL Large)
{
	HIMAGELIST hSysImageList;
	SHFILEINFO FileInfo;	

	if(Large)
	{
		hSysImageList=(HIMAGELIST)SHGetFileInfo(FileType,FileAttr,&FileInfo,
			sizeof(FileInfo),SHGFI_USEFILEATTRIBUTES|SHGFI_SYSICONINDEX|SHGFI_LARGEICON);
		if(m_LargeIcons.m_hImageList==NULL)
			m_LargeIcons.Attach(hSysImageList);
	}
	else
	{
		hSysImageList=(HIMAGELIST)SHGetFileInfo(FileType,FileAttr,&FileInfo,
			sizeof(FileInfo),SHGFI_USEFILEATTRIBUTES|SHGFI_SYSICONINDEX|SHGFI_SMALLICON);
		if(m_SmallIcons.m_hImageList==NULL)
			m_SmallIcons.Attach(hSysImageList);
	}	
	return FileInfo.iIcon;
}


void CDlgWorkDirBowser::Browse(LPCTSTR szDir)
{
	m_lvFileList.DeleteAllItems();
	CServerManagerClientApp::GetInstance()->BrowseWorkDir(m_ServerAddress,m_ServerIndex,szDir);
}

void CDlgWorkDirBowser::BrowseLocal(LPCTSTR szDir)
{
	CFileSearcher FileSearcher;
	int Item=0;
	bool IsInDirectory=false;

	CEasyString SearchFolder=szDir;
	SearchFolder=MakeFullPath(SearchFolder+"\\*.*");

	m_CurLocalDir=GetPathDirectory(SearchFolder);

	m_lvLocalFileList.DeleteAllItems();	

	if(FileSearcher.FindFirst(SearchFolder))
	{
		while(FileSearcher.FindNext())
		{
			int Icon;
			CEasyString Temp;
			
			Icon=GetSHIcon(FileSearcher.GetFileName(),FileSearcher.GetFileAttributes(),false);
			Item=m_lvLocalFileList.InsertItem(Item,FileSearcher.GetFileName(),Icon);
			if((FileSearcher.GetFileAttributes()&FILE_ATTRIBUTE_DIRECTORY)==0)
			{
				m_lvLocalFileList.SetItemText(Item,1,FormatNumberWords(FileSearcher.GetFileSize()));
			}
			FileSearcher.GetCreationTime().Format(Temp,"%Y-%m-%d %H:%M:%S");
			m_lvLocalFileList.SetItemText(Item,2,Temp);
			FileSearcher.GetLastWriteTime().Format(Temp,"%Y-%m-%d %H:%M:%S");
			m_lvLocalFileList.SetItemText(Item,3,Temp);
			m_lvLocalFileList.SetItemData(Item,FileSearcher.GetFileAttributes());
			if(FileSearcher.IsDots())
				IsInDirectory=true;
			Item++;
		}
	}

	if(!IsInDirectory)
	{
		FileSearcher.FetchLogicalDrive();
		for(int i=0;i<FileSearcher.GetLogicalDriveCount();i++)
		{
			int Icon;
			Icon=GetSHIcon(FileSearcher.GetLogicDriveName(i),FILE_ATTRIBUTE_DIRECTORY,false);
			Item=m_lvLocalFileList.InsertItem(Item,FileSearcher.GetLogicDriveName(i),Icon);
			m_lvLocalFileList.SetItemData(Item,0xFFFFFFFF);
			Item++;			
		}
	}

	SORT_PARAM Param;
	Param.pList=&m_lvLocalFileList;	
	Param.SortCol=0;
	Param.SortOrder=1;

	m_lvLocalFileList.SortItemsEx(ItemSortCallBack,(DWORD_PTR)&Param);
}

void CDlgWorkDirBowser::OnBrowseResult(LPCTSTR szDir,LPCTSTR szFileName,DWORD FileAttr,ULONG64 FileSize,time_t CreateTime,time_t LastAccessTime,time_t LastWriteTime)
{
	if(m_CurDir!=szDir)
		m_CurDir=szDir;

	int Icon;
	CEasyString Temp;
	CEasyTime Time;
	int Item=m_lvFileList.GetItemCount();
	Icon=GetSHIcon(szFileName,FileAttr,false);
	Item=m_lvFileList.InsertItem(Item,szFileName,Icon);
	if((FileAttr&FILE_ATTRIBUTE_DIRECTORY)==0)
	{
		m_lvFileList.SetItemText(Item,1,FormatNumberWords(FileSize));
	}
	Time=CreateTime;
	Time.Format(Temp,"%Y-%m-%d %H:%M:%S");
	m_lvFileList.SetItemText(Item,2,Temp);
	Time=LastWriteTime;
	Time.Format(Temp,"%Y-%m-%d %H:%M:%S");
	m_lvFileList.SetItemText(Item,3,Temp);
	m_lvFileList.SetItemData(Item,(DWORD_PTR)FileAttr);
}

void CDlgWorkDirBowser::OnBrowseFinish()
{
	int Index=0;
	LVCOLUMN ColInfo;
	ColInfo.mask=LVCF_IMAGE;
	m_lvFileList.GetColumn(Index,&ColInfo);	
	ColInfo.iImage=1;
	m_lvFileList.SetColumn(Index,&ColInfo);
	SORT_PARAM Param;
	Param.pList=&m_lvFileList;	
	Param.SortCol=Index;
	Param.SortOrder=ColInfo.iImage;

	m_lvFileList.SortItemsEx(ItemSortCallBack,(DWORD_PTR)&Param);
}
void CDlgWorkDirBowser::OnNMDblclkListFiles(NMHDR *pNMHDR, LRESULT *pResult)
{
	//LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<NMITEMACTIVATE>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码
	*pResult = 0;


	POSITION Pos=m_lvFileList.GetFirstSelectedItemPosition();

	if(Pos)
	{
		int Item=m_lvFileList.GetNextSelectedItem(Pos);		
		DWORD FileAttr=(DWORD)m_lvFileList.GetItemData(Item);
		CString FileName=m_lvFileList.GetItemText(Item,0);
		if(FileAttr&FILE_ATTRIBUTE_DIRECTORY)
		{
			Browse(m_CurDir+"\\"+(LPCTSTR)FileName);
		}
	}
}

int CDlgWorkDirBowser::ItemSortCallBack(LPARAM lParam1, LPARAM lParam2,LPARAM lParamSort)
{
	SORT_PARAM * pSortParam=(SORT_PARAM *)lParamSort;

	int Order=1;
	if(pSortParam->SortOrder==2)
	{
		Order=-1;
	}

	DWORD FileAttr1=(DWORD)pSortParam->pList->GetItemData(lParam1)&(FILE_ATTRIBUTE_DIRECTORY|FILE_ATTRIBUTE_DRIVER);
	DWORD FileAttr2=(DWORD)pSortParam->pList->GetItemData(lParam2)&(FILE_ATTRIBUTE_DIRECTORY|FILE_ATTRIBUTE_DRIVER);

	CString Text1=pSortParam->pList->GetItemText(lParam1,pSortParam->SortCol);
	CString Text2=pSortParam->pList->GetItemText(lParam2,pSortParam->SortCol);


	if(FileAttr1==FileAttr2)
	{
		if(pSortParam->SortOrder==1)
			return _stricmp(Text1,Text2);
		else
			return _stricmp(Text2,Text1);
	}
	else
	{
		return FileAttr1<FileAttr2;
	}	
}
void CDlgWorkDirBowser::OnLvnColumnclickListFiles(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码
	*pResult = 0;

	int Index=pNMLV->iSubItem;
	LVCOLUMN ColInfo;
	ColInfo.mask=LVCF_IMAGE;
	m_lvFileList.GetColumn(Index,&ColInfo);

	if(ColInfo.iImage==1)
		ColInfo.iImage=2;
	else
		ColInfo.iImage=1;

	m_lvFileList.SetColumn(Index,&ColInfo);
	SORT_PARAM Param;
	Param.pList=&m_lvFileList;	
	Param.SortCol=Index;
	Param.SortOrder=ColInfo.iImage;

	m_lvFileList.SortItemsEx(ItemSortCallBack,(DWORD_PTR)&Param);
}

void CDlgWorkDirBowser::OnLvnColumnclickListFilesLocal(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码
	*pResult = 0;

	int Index=pNMLV->iSubItem;
	LVCOLUMN ColInfo;
	ColInfo.mask=LVCF_IMAGE;
	m_lvLocalFileList.GetColumn(Index,&ColInfo);

	if(ColInfo.iImage==1)
		ColInfo.iImage=2;
	else
		ColInfo.iImage=1;

	m_lvLocalFileList.SetColumn(Index,&ColInfo);
	SORT_PARAM Param;
	Param.pList=&m_lvLocalFileList;	
	Param.SortCol=Index;
	Param.SortOrder=ColInfo.iImage;

	m_lvLocalFileList.SortItemsEx(ItemSortCallBack,(DWORD_PTR)&Param);
}

void CDlgWorkDirBowser::OnNMDblclkListFilesLocal(NMHDR *pNMHDR, LRESULT *pResult)
{
	//LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<NMITEMACTIVATE>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码
	*pResult = 0;


	POSITION Pos=m_lvLocalFileList.GetFirstSelectedItemPosition();

	if(Pos)
	{
		int Item=m_lvLocalFileList.GetNextSelectedItem(Pos);		
		DWORD FileAttr=(DWORD)m_lvLocalFileList.GetItemData(Item);
		CString FileName=m_lvLocalFileList.GetItemText(Item,0);
		if(FileAttr==0xFFFFFFFF)
		{
			BrowseLocal((LPCTSTR)FileName);
		}
		else if(FileAttr&FILE_ATTRIBUTE_DIRECTORY)
		{			
			BrowseLocal(m_CurLocalDir+"\\"+(LPCTSTR)FileName);
		}
	}
}

void CDlgWorkDirBowser::OnBnClickedUpload()
{
	// TODO: 在此添加控件通知处理程序代码
	if(m_TransferList.GetCount())
	{
		AfxMessageBox("目前正有文件传输中");
		return;
	}

	POSITION Pos=m_lvLocalFileList.GetFirstSelectedItemPosition();

	while(Pos)
	{
		int Item=m_lvLocalFileList.GetNextSelectedItem(Pos);		
		DWORD FileAttr=(DWORD)m_lvLocalFileList.GetItemData(Item);
		CString FileName=m_lvLocalFileList.GetItemText(Item,0);
		if((FileAttr&FILE_ATTRIBUTE_DIRECTORY)==0)
		{			
			bool IsExist=false;
			for(int i=0;i<m_lvFileList.GetItemCount();i++)
			{					
				DWORD ServerFileAttr=(DWORD)m_lvFileList.GetItemData(i);
				CString ServerFileName=m_lvFileList.GetItemText(i,0);
				if((ServerFileAttr&FILE_ATTRIBUTE_DIRECTORY)==0)
				{
					if(FileName.CompareNoCase(ServerFileName)==0)
					{
						IsExist=true;
						break;
					}
				}
			}
			if(IsExist)
			{
				CEasyString PromptText;
				PromptText.Format("文件[%s]已存在，是否要覆盖",FileName);
				int Ret=AfxMessageBox(PromptText,MB_YESNOCANCEL);
				if(Ret==IDNO)
					continue;
				if(Ret==IDCANCEL)
				{
					m_TransferList.Clear();
					return;
				}
			}
			TRANSFER_FILE_INFO TransferInfo;
			TransferInfo.SourceFilePath=MakeFullPath(m_CurLocalDir+"\\"+(LPCTSTR)FileName);
			TransferInfo.TargetFilePath=m_CurDir+"\\"+(LPCTSTR)FileName;

			m_TransferList.Add(TransferInfo);
		}
	}
	NextUploadFile();
}

void CDlgWorkDirBowser::OnBnClickedDownload()
{
	// TODO: 在此添加控件通知处理程序代码
	if(m_TransferList.GetCount())
	{
		AfxMessageBox("目前正有文件传输中");
		return;
	}

	POSITION Pos=m_lvFileList.GetFirstSelectedItemPosition();

	while(Pos)
	{
		int Item=m_lvFileList.GetNextSelectedItem(Pos);		
		DWORD FileAttr=(DWORD)m_lvFileList.GetItemData(Item);
		CString FileName=m_lvFileList.GetItemText(Item,0);
		if((FileAttr&FILE_ATTRIBUTE_DIRECTORY)==0)
		{			
			bool IsExist=false;
			for(int i=0;i<m_lvLocalFileList.GetItemCount();i++)
			{					
				DWORD ServerFileAttr=(DWORD)m_lvLocalFileList.GetItemData(i);
				CString ServerFileName=m_lvLocalFileList.GetItemText(i,0);
				if((ServerFileAttr&FILE_ATTRIBUTE_DIRECTORY)==0)
				{
					if(FileName.CompareNoCase(ServerFileName)==0)
					{
						IsExist=true;
						break;
					}
				}
			}
			if(IsExist)
			{
				CEasyString PromptText;
				PromptText.Format("文件[%s]已存在，是否要覆盖",FileName);
				int Ret=AfxMessageBox(PromptText,MB_YESNOCANCEL);
				if(Ret==IDNO)
					continue;
				if(Ret==IDCANCEL)
				{
					m_TransferList.Clear();
					return;
				}
			}
			TRANSFER_FILE_INFO TransferInfo;
			TransferInfo.SourceFilePath=m_CurDir+"\\"+(LPCTSTR)FileName;
			TransferInfo.TargetFilePath=MakeFullPath(m_CurLocalDir+"\\"+(LPCTSTR)FileName);

			m_TransferList.Add(TransferInfo);
		}
	}
	NextDownloadFile();
}

void CDlgWorkDirBowser::NextUploadFile()
{
	if(m_TransferList.GetCount())
	{
		CServerConnection * pConnection=CServerManagerClientApp::GetInstance()->GetServerConnection(m_ServerAddress);
		if(pConnection)
		{
			CWinFileAccessor FileAccessor;
			if(FileAccessor.Open(m_TransferList[0].SourceFilePath,IFileAccessor::modeOpen|IFileAccessor::modeRead|IFileAccessor::shareShareAll))
			{
				UINT TotalSize=FileAccessor.GetSize();

				CEasyTime LastWriteTime;

				FileAccessor.GetLastWriteTime(LastWriteTime);


				CEasyString Title;
				Title.Format("正在压缩:%s",(LPCTSTR)m_TransferList[0].SourceFilePath);
				m_TransferProgress.SetWindowText(Title);

				m_TransferProgress.ShowWindow(SW_SHOW);



				CEasyBuffer ReadBuffer;

				ReadBuffer.Create(TotalSize);
				m_TransferBuffer.Create(TotalSize*2);

				size_t PackSize=TotalSize*2;

				m_PackProp.Create(LZMA_PROPS_SIZE);
				size_t PackPropsSize=LZMA_PROPS_SIZE;


				if(FileAccessor.Read(ReadBuffer.GetBuffer(),TotalSize)==TotalSize)
				{
					if(LzmaCompress((BYTE *)m_TransferBuffer.GetBuffer(),&PackSize,
						(BYTE *)ReadBuffer.GetBuffer(),TotalSize,
						(BYTE *)m_PackProp.GetBuffer(),&PackPropsSize,5,
						1<<24,3,0,2,32,2)==SZ_OK)
					{
						m_TransferBuffer.SetUsedSize(PackSize);
						m_TransferLeftSize=PackSize;
						m_pView->PrintLog("文件[%s]压缩完毕，压缩比=%f(%u/%u)",
							(LPCTSTR)m_TransferList[0].SourceFilePath,
							(float)PackSize/TotalSize,PackSize,TotalSize);

						m_TransferProgress.InitProgress(PackSize);

						CEasyString Title;
						Title.Format("正在上传:%s",(LPCTSTR)m_TransferList[0].SourceFilePath);
						m_TransferProgress.SetWindowText(Title);

						pConnection->QueryStartUpload(m_ServerIndex,
							(LPCTSTR)m_TransferList[0].SourceFilePath,
							(LPCTSTR)m_TransferList[0].TargetFilePath,
							LastWriteTime,
							PackSize,TotalSize,m_PackProp.GetBuffer(),PackPropsSize);		
					}
					else
					{
						CEasyString PromptText;
						PromptText.Format("文件[%s]压缩失败",m_TransferList[0].SourceFilePath);
						AfxMessageBox(PromptText);
						m_TransferList.Clear();
					}

					FileAccessor.Close();
					
				}
				else
				{
					CEasyString PromptText;
					PromptText.Format("文件[%s]读取失败",m_TransferList[0].SourceFilePath);
					AfxMessageBox(PromptText);
					m_TransferList.Clear();
				}
			}
			else
			{
				CEasyString PromptText;
				PromptText.Format("文件[%s]打开失败",m_TransferList[0].SourceFilePath);
				AfxMessageBox(PromptText);
				m_TransferList.Clear();
			}
		}
		else
		{
			AfxMessageBox("服务器连接不存在");
			m_TransferList.Clear();
		}
	}
	else
	{
		Browse(m_CurDir);
	}
}

void CDlgWorkDirBowser::NextDownloadFile()
{
	if(m_TransferList.GetCount())
	{
		CServerConnection * pConnection=CServerManagerClientApp::GetInstance()->GetServerConnection(m_ServerAddress);
		if(pConnection)
		{
			CWinFileAccessor FileAccessor;
			if(FileAccessor.Open(m_TransferList[0].TargetFilePath,IFileAccessor::modeCreateAlways|IFileAccessor::modeWrite))
			{
				CEasyString Title;
				Title.Format("正在下载:%s",(LPCTSTR)m_TransferList[0].SourceFilePath);
				m_TransferProgress.SetWindowText(Title);

				pConnection->QueryStartDownload(m_ServerIndex,m_TransferList[0].SourceFilePath,m_TransferList[0].TargetFilePath);
				FileAccessor.Close();
			}
			else
			{
				CEasyString PromptText;
				PromptText.Format("文件[%s]打开失败",m_TransferList[0].SourceFilePath);
				AfxMessageBox(PromptText);
				m_TransferList.Clear();
			}
		}
		else
		{
			AfxMessageBox("服务器连接不存在");
			m_TransferList.Clear();
		}
	}
	else
	{
		BrowseLocal(m_CurLocalDir);
	}
}

void CDlgWorkDirBowser::ContinueDownloadFile(bool IsSucceed)
{
	if(IsSucceed)
	{
		CServerConnection * pConnection=CServerManagerClientApp::GetInstance()->GetServerConnection(m_ServerAddress);
		if(pConnection)
		{
			pConnection->QueryDownloadData(m_ServerIndex);
		}
		else
		{
			AfxMessageBox("服务器连接不存在");
			m_TransferList.Clear();
		}
	}
	else
	{
		AfxMessageBox("上传文件失败");
		m_TransferList.Clear();
	}
}


void CDlgWorkDirBowser::ContinueUploadFile(bool IsSucceed)
{
	if(IsSucceed)
	{
		CServerConnection * pConnection=CServerManagerClientApp::GetInstance()->GetServerConnection(m_ServerAddress);
		if(pConnection)
		{
			UINT TotalSize=m_TransferBuffer.GetUsedSize();
			
			if(m_TransferLeftSize)
			{
				UINT DataSize=MAX_DATA_TRANSFER_SIZE;
				if(m_TransferLeftSize<DataSize)
					DataSize=m_TransferLeftSize;
				BYTE * pDataBuffer=(BYTE *)m_TransferBuffer.GetBuffer()+m_TransferBuffer.GetUsedSize()-m_TransferLeftSize;

				m_TransferProgress.UpdateProgress(m_TransferBuffer.GetUsedSize()-m_TransferLeftSize);

				m_TransferLeftSize-=DataSize;

				
					
				pConnection->QueryContinueUpload(m_ServerIndex,m_TransferBuffer.GetUsedSize(),m_TransferLeftSize,
					pDataBuffer,DataSize);
			}
			else
			{
				m_TransferProgress.ShowWindow(SW_HIDE);

				if(m_pView)
				{
					m_pView->PrintLog("上传文件成功");
				}
				
				m_TransferList.Delete(0);
				NextUploadFile();
			}
		}
		else
		{
			AfxMessageBox("服务器连接不存在");
			m_TransferList.Clear();
		}
	}
	else
	{
		AfxMessageBox("上传文件失败");
		m_TransferList.Clear();
	}
}


void CDlgWorkDirBowser::OnStartDownloadResult(int Result,UINT TotalSize,UINT OrgTotalSize,LPCVOID pPackProps,UINT PackPropSize,time_t LastWriteTime)
{
	if(Result==SSMRT_SUCCEED)
	{
		m_TransferBuffer.Create(TotalSize);
		m_OriginSize=OrgTotalSize;
		m_PackProp.Create(PackPropSize);
		m_PackProp.PushBack(pPackProps,PackPropSize);
		m_CurTransferFileLastWriteTime=LastWriteTime;
		if(TotalSize)
		{
			m_TransferProgress.InitProgress(TotalSize);
			m_TransferProgress.ShowWindow(SW_SHOW);
			ContinueDownloadFile(true);
		}
		else
		{
			if(m_pView)
			{
				m_pView->PrintLog("下载文件成功");
			}

			m_TransferList.Delete(0);
			NextDownloadFile();
		}
	}
	else
	{
		AfxMessageBox("下载文件失败");
		m_TransferList.Clear();
	}
}
void CDlgWorkDirBowser::OnDownloadData(int Result,UINT TotalSize,UINT LeftSize,LPCVOID pData,UINT DataSize)
{
	if(Result==SSMRT_SUCCEED)
	{
		m_TransferBuffer.PushBack(pData,DataSize);
		m_TransferProgress.UpdateProgress(m_TransferBuffer.GetUsedSize());
		if(LeftSize)
		{
			ContinueDownloadFile(true);
		}
		else
		{
			m_pView->PrintLog("下载文件完毕");

			m_TransferProgress.SetWindowText("正在解压");

		
			CEasyBuffer WriteBuffer;
			WriteBuffer.Create(m_OriginSize);

			size_t UnpackSize=m_OriginSize;
			size_t SourceSize=m_TransferBuffer.GetUsedSize();

			if(LzmaUncompress((BYTE *)WriteBuffer.GetBuffer(),&UnpackSize,
				(BYTE *)m_TransferBuffer.GetBuffer(),&SourceSize,
				(BYTE *)m_PackProp.GetBuffer(),m_PackProp.GetUsedSize())==SZ_OK)
			{
				m_pView->PrintLog("数据解压完毕,大小%u",
					UnpackSize);
				if(m_IsInEdit)
				{
					m_IsInEdit=false;
					DoEdit((LPCTSTR)WriteBuffer.GetBuffer(),UnpackSize);
				}
				else
				{
					CWinFileAccessor FileAccessor;
					if(FileAccessor.Open(m_TransferList[0].TargetFilePath,CWinFileAccessor::modeCreateAlways|CWinFileAccessor::modeWrite))
					{
						if(FileAccessor.Write(WriteBuffer.GetBuffer(),UnpackSize)==UnpackSize)
						{
							FileAccessor.SetLastWriteTime(m_CurTransferFileLastWriteTime);
							m_pView->PrintLog("[%s]数据写入完毕,大小%u",
								(LPCTSTR)m_TransferList[0].TargetFilePath,
								UnpackSize);
						}
						else
						{
							m_pView->PrintLog("数据写入失败");
						}
					}
					else
					{
						m_pView->PrintLog("打开文件[%s]失败",(LPCTSTR)m_TransferList[0].TargetFilePath);
					}
					
					FileAccessor.Close();
				}
			}
			else
			{
				m_pView->PrintLog("数据解压失败");
			}

			m_TransferProgress.ShowWindow(SW_HIDE);

			m_TransferList.Delete(0);
			NextDownloadFile();
		}
	}
	else
	{
		AfxMessageBox("下载文件失败");
		m_TransferList.Clear();
	}
}


void CDlgWorkDirBowser::OnCreateDirResult(int Result)
{
	if(Result==SSMRT_SUCCEED)
	{
		m_pView->PrintLog("创建目录成功");
		Browse(m_CurDir);
	}
	else
	{
		m_pView->PrintLog("创建目录失败");
	}
}


void CDlgWorkDirBowser::OnDeleteFileResult(int Result)
{
	if(m_RefreshTimer.IsTimeOut(MIN_SERVER_DIR_REFRESH_TIME))
	{
		m_RefreshTimer.SaveTime();
		Browse(m_CurDir);
	}
}


void CDlgWorkDirBowser::OnNMRClickListFiles(NMHDR *pNMHDR, LRESULT *pResult)
{
	//LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<NMITEMACTIVATE>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码
	*pResult = 0;

	CPoint MousePoint;
	GetCursorPos(&MousePoint);

	CMenu Menu;
	VERIFY(Menu.LoadMenu(IDR_TRANSFER_SERVER_LIST_POPUP));

	CMenu* pPopup = Menu.GetSubMenu(0);
	ASSERT(pPopup != NULL);
	CWnd* pWndPopupOwner = this;

	while (pWndPopupOwner->GetStyle() & WS_CHILD)
		pWndPopupOwner = pWndPopupOwner->GetParent();


	pPopup->TrackPopupMenu(TPM_LEFTALIGN | TPM_RIGHTBUTTON, MousePoint.x, MousePoint.y,
		pWndPopupOwner);
}

void CDlgWorkDirBowser::OnNMRClickListFilesLocal(NMHDR *pNMHDR, LRESULT *pResult)
{
	//LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<NMITEMACTIVATE>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码
	*pResult = 0;


	CPoint MousePoint;
	GetCursorPos(&MousePoint);

	CMenu Menu;
	VERIFY(Menu.LoadMenu(IDR_TRANSFER_LOCAL_LIST_POPUP));

	CMenu* pPopup = Menu.GetSubMenu(0);
	ASSERT(pPopup != NULL);
	CWnd* pWndPopupOwner = this;

	while (pWndPopupOwner->GetStyle() & WS_CHILD)
		pWndPopupOwner = pWndPopupOwner->GetParent();


	pPopup->TrackPopupMenu(TPM_LEFTALIGN | TPM_RIGHTBUTTON, MousePoint.x, MousePoint.y,
		pWndPopupOwner);
}


void CDlgWorkDirBowser::OnServerCreateDir()
{
	// TODO: 在此添加命令处理程序代码
	CServerConnection * pConnection=CServerManagerClientApp::GetInstance()->GetServerConnection(m_ServerAddress);
	if(pConnection)
	{
		CDlgInput Dlg;

		Dlg.m_Title="请输入目录名";

		if(Dlg.DoModal()==IDOK)
		{
			CEasyString Dir=m_CurDir+"\\"+(LPCTSTR)Dlg.m_InputText;
			pConnection->QueryCreateDir(m_ServerIndex,Dir);
		}
	}
}
void CDlgWorkDirBowser::OnServerEditFile()
{
	// TODO: 在此添加命令处理程序代码
	CServerConnection * pConnection=CServerManagerClientApp::GetInstance()->GetServerConnection(m_ServerAddress);
	if(pConnection)
	{

		POSITION Pos=m_lvFileList.GetFirstSelectedItemPosition();

		if(Pos)
		{
			int Item=m_lvFileList.GetNextSelectedItem(Pos);		
			DWORD FileAttr=(DWORD)m_lvFileList.GetItemData(Item);
			CString FileName=m_lvFileList.GetItemText(Item,0);
			CString FileSize=m_lvFileList.GetItemText(Item,1);
			if(FileSize.Find('M')>=0)
			{
				if(AfxMessageBox("文件过大,是否要继续编辑",MB_YESNO)==IDNO)
					return;
			}
			if((FileAttr&FILE_ATTRIBUTE_DIRECTORY)==0)
			{		
				m_CurEditFilePath=m_CurDir+"\\"+(LPCTSTR)FileName;

				CEasyString Title;
				Title.Format("正在下载:%s",(LPCTSTR)m_CurEditFilePath);
				m_TransferProgress.SetWindowText(Title);

				pConnection->QueryStartDownload(m_ServerIndex,m_CurEditFilePath,"");
				m_IsInEdit=true;
			}
		}
	}
	else
	{
		AfxMessageBox("服务器连接不存在");		
	}
}

void CDlgWorkDirBowser::DoEdit(LPCTSTR Text,UINT Size)
{
	CDlgEditBox Dlg;

	Dlg.m_Text.SetString(Text,Size);
	if(Dlg.DoModal()==IDOK)
	{
		CServerConnection * pConnection=CServerManagerClientApp::GetInstance()->GetServerConnection(m_ServerAddress);
		if(pConnection)
		{
			UINT TotalSize=Dlg.m_Text.GetLength();

			CEasyTime LastWriteTime;

			LastWriteTime.FetchSystemTime();		

			CEasyString Title;
			Title.Format("正在压缩:%s",(LPCTSTR)m_CurEditFilePath);
			m_TransferProgress.SetWindowText(Title);

			m_TransferProgress.ShowWindow(SW_SHOW);

			
			m_TransferBuffer.Create(TotalSize);

			size_t PackSize=TotalSize;

			m_PackProp.Create(LZMA_PROPS_SIZE);
			size_t PackPropsSize=LZMA_PROPS_SIZE;


			
			if(LzmaCompress((BYTE *)m_TransferBuffer.GetBuffer(),&PackSize,
				(BYTE *)Dlg.m_Text.GetBuffer(),TotalSize,
				(BYTE *)m_PackProp.GetBuffer(),&PackPropsSize,5,
				1<<24,3,0,2,32,2)==SZ_OK)
			{
				m_TransferBuffer.SetUsedSize(PackSize);
				m_TransferLeftSize=PackSize;
				m_pView->PrintLog("文件压缩完毕，压缩比=%f(%u/%u)",					
					(float)PackSize/TotalSize,PackSize,TotalSize);


				m_TransferProgress.InitProgress(PackSize);

				CEasyString Title;
				Title.Format("正在上传:%s",(LPCTSTR)m_CurEditFilePath);
				m_TransferProgress.SetWindowText(Title);

				pConnection->QueryStartUpload(m_ServerIndex,
					"",
					(LPCTSTR)m_CurEditFilePath,		
					LastWriteTime,
					PackSize,TotalSize,m_PackProp.GetBuffer(),PackPropsSize);		
			}
			else
			{
				CEasyString PromptText;
				PromptText.Format("压缩失败");
				AfxMessageBox(PromptText);				
			}
		}
	}
}
void CDlgWorkDirBowser::OnServerRunFile()
{
	// TODO: 在此添加命令处理程序代码
	CServerConnection * pConnection=CServerManagerClientApp::GetInstance()->GetServerConnection(m_ServerAddress);
	if(pConnection)
	{

		POSITION Pos=m_lvFileList.GetFirstSelectedItemPosition();

		if(Pos)
		{
			int Item=m_lvFileList.GetNextSelectedItem(Pos);		
			DWORD FileAttr=(DWORD)m_lvFileList.GetItemData(Item);
			CString FileName=m_lvFileList.GetItemText(Item,0);			
			if((FileAttr&FILE_ATTRIBUTE_DIRECTORY)==0)
			{	
				CDlgInput Dlg;

				Dlg.m_Title="请输入运行参数";

				if(Dlg.DoModal()==IDOK)
				{
					CEasyString FilePath=m_CurDir+"\\"+(LPCTSTR)FileName;

					pConnection->QueryCreateProcess(m_ServerIndex,FilePath,m_CurDir,Dlg.m_InputText);
					m_IsInEdit=true;					
				}
				
			}
		}
	}
	else
	{
		AfxMessageBox("服务器连接不存在");		
	}
}

void CDlgWorkDirBowser::OnServerDeleteFile()
{
	// TODO: 在此添加命令处理程序代码
	CServerConnection * pConnection=CServerManagerClientApp::GetInstance()->GetServerConnection(m_ServerAddress);
	if(pConnection)
	{

		POSITION Pos=m_lvFileList.GetFirstSelectedItemPosition();

		while(Pos)
		{
			int Item=m_lvFileList.GetNextSelectedItem(Pos);		
			DWORD FileAttr=(DWORD)m_lvFileList.GetItemData(Item);
			CString FileName=m_lvFileList.GetItemText(Item,0);	

			CEasyString PromptText;
			PromptText.Format("是否要删除文件[%s]？",FileName);

			int Ret=AfxMessageBox(PromptText,MB_YESNOCANCEL);
			if(Ret==IDYES)
			{
				CEasyString FilePath=m_CurDir+"\\"+(LPCTSTR)FileName;

				pConnection->QueryDeleteFile(m_ServerIndex,FilePath);
			}
			else if(Ret==IDNO)
			{
				continue;
			}
			else
			{
				break;
			}			
		}
	}
	else
	{
		AfxMessageBox("服务器连接不存在");		
	}
}

void CDlgWorkDirBowser::OnServerRefresh()
{
	// TODO: 在此添加命令处理程序代码
	Browse(m_CurDir);
}

void CDlgWorkDirBowser::OnLocalRefresh()
{
	// TODO: 在此添加命令处理程序代码
	BrowseLocal(m_CurLocalDir);
}



void CDlgWorkDirBowser::OnServerRunScript()
{
	// TODO: 在此添加命令处理程序代码
	CServerConnection * pConnection=CServerManagerClientApp::GetInstance()->GetServerConnection(m_ServerAddress);
	if(pConnection)
	{

		POSITION Pos=m_lvFileList.GetFirstSelectedItemPosition();

		if(Pos)
		{
			int Item=m_lvFileList.GetNextSelectedItem(Pos);		
			DWORD FileAttr=(DWORD)m_lvFileList.GetItemData(Item);
			CString FileName=m_lvFileList.GetItemText(Item,0);	

			CEasyString PromptText;
			PromptText.Format("是否要执行文件[%s]？",FileName);

			int Ret=AfxMessageBox(PromptText,MB_YESNOCANCEL);
			if(Ret==IDYES)
			{
				CEasyString FilePath=m_CurDir+"\\"+(LPCTSTR)FileName;

				pConnection->QueryRunScript(m_ServerIndex,FilePath);
			}				
		}
	}
	else
	{
		AfxMessageBox("服务器连接不存在");		
	}
}
