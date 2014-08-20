/****************************************************************************/
/*                                                                          */
/*      文件名:    CallInterfaceMakerDlg.cpp                                */
/*      创建日期:  2012年04月22日                                           */
/*      作者:      Sagasarate                                               */
/*                                                                          */
/*      本软件版权归Sagasarate(sagasarate@sina.com)所有                     */
/*      你可以将本软件用于任何商业和非商业软件开发，但                      */
/*      必须保留此版权声明                                                  */
/*                                                                          */
/****************************************************************************/
// CallInterfaceMakerDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "CallInterfaceMakerDlg.h"
#include <math.h>


#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// 用于应用程序“关于”菜单项的 CAboutDlg 对话框

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// 对话框数据
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

// 实现
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
END_MESSAGE_MAP()


// CCallInterfaceMakerDlg 对话框




CCallInterfaceMakerDlg::CCallInterfaceMakerDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CCallInterfaceMakerDlg::IDD, pParent)
	, m_Name(_T(""))
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	m_SelectAll=true;
	m_InterfaceIDSeed=1;
}

CCallInterfaceMakerDlg::~CCallInterfaceMakerDlg()
{
	for(size_t i=0;i<m_DataStructDefines.DataStructDefineList.size();i++)
	{
		delete m_DataStructDefines.DataStructDefineList[i];
	}
}

void CCallInterfaceMakerDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_INTERFACE_LIST, m_lvInterfaceList);
	DDX_Text(pDX, IDC_EDIT_NAME, m_Name);
}

BEGIN_MESSAGE_MAP(CCallInterfaceMakerDlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDC_BUTTON_ADD_INTERFACE, &CCallInterfaceMakerDlg::OnBnClickedButtonAddInterface)
	ON_BN_CLICKED(IDC_BUTTON_EDIT_INTERFACE, &CCallInterfaceMakerDlg::OnBnClickedButtonEditInterface)
	ON_BN_CLICKED(IDC_BUTTON_DEL_INTERFACE, &CCallInterfaceMakerDlg::OnBnClickedButtonDelInterface)
	ON_BN_CLICKED(IDC_BUTTON_EDIT_PREFIX, &CCallInterfaceMakerDlg::OnBnClickedButtonEditPrefix)
	ON_BN_CLICKED(IDC_BUTTON_EDIT_POSTFIX, &CCallInterfaceMakerDlg::OnBnClickedButtonEditPostfix)
	ON_BN_CLICKED(IDC_BUTTON_SAVE, &CCallInterfaceMakerDlg::OnBnClickedButtonSave)
	ON_BN_CLICKED(IDC_BUTTON_SAVE_AS, &CCallInterfaceMakerDlg::OnBnClickedButtonSaveAs)
	ON_BN_CLICKED(IDC_BUTTON_LOAD, &CCallInterfaceMakerDlg::OnBnClickedButtonLoad)
	ON_BN_CLICKED(IDC_BUTTON_SAVE_EXPORT, &CCallInterfaceMakerDlg::OnBnClickedButtonSaveExport)
	ON_NOTIFY(NM_DBLCLK, IDC_INTERFACE_LIST, &CCallInterfaceMakerDlg::OnNMDblclkInterfaceList)
	ON_BN_CLICKED(IDC_BUTTON_CONFIG, &CCallInterfaceMakerDlg::OnBnClickedButtonConfig)
	ON_BN_CLICKED(IDC_BUTTON_SELECT_ALL_INTERFACE, &CCallInterfaceMakerDlg::OnBnClickedButtonSelectAllInterface)
	ON_BN_CLICKED(IDC_BUTTON_DATA_STRUCT_DEF, &CCallInterfaceMakerDlg::OnBnClickedButtonDataStructDef)
END_MESSAGE_MAP()


// CCallInterfaceMakerDlg 消息处理程序

BOOL CCallInterfaceMakerDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// 将“关于...”菜单项添加到系统菜单中。

	// IDM_ABOUTBOX 必须在系统命令范围内。
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		CString strAboutMenu;
		strAboutMenu.LoadString(IDS_ABOUTBOX);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// 设置此对话框的图标。当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO: 在此添加额外的初始化代码

	m_TypeDefFileName=MakeModuleFullPath(NULL,TYPE_DEF_FILE_NAME);
	
	LoadTypeDef();
	LoadTemples();
	LoadConfig();
	LoadEnv();

	MakeCurVarType(true);

	m_lvInterfaceList.SetExtendedStyle(LVS_EX_CHECKBOXES|LVS_EX_FULLROWSELECT|LVS_EX_GRIDLINES);


	m_lvInterfaceList.InsertColumn(0,"名称",LVCFMT_LEFT,200);
	m_lvInterfaceList.InsertColumn(1,"方法数量",LVCFMT_LEFT,60);
	m_lvInterfaceList.InsertColumn(2,"类型",LVCFMT_LEFT,60);
	m_lvInterfaceList.InsertColumn(3,"ID",LVCFMT_LEFT,300);
	m_lvInterfaceList.InsertColumn(4,"描述",LVCFMT_LEFT,200);




	FillListItem();
	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CCallInterfaceMakerDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialog::OnSysCommand(nID, lParam);
	}
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CCallInterfaceMakerDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作区矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CCallInterfaceMakerDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


void CCallInterfaceMakerDlg::OnBnClickedButtonAddInterface()
{
	// TODO: 在此添加控件通知处理程序代码
	CDlgInterfaceEdtor Dlg;

	Dlg.SetID(m_InterfaceIDSeed);
	if(Dlg.DoModal()==IDOK)
	{
		m_InterfaceIDSeed++;
		CALLER_INTERFACE InterfaceInfo;
		Dlg.GetData(InterfaceInfo);
		m_InterfaceList.push_back(InterfaceInfo);
		AddListItem(InterfaceInfo,m_InterfaceList.size()-1);
	}
}

void CCallInterfaceMakerDlg::OnBnClickedButtonEditInterface()
{
	// TODO: 在此添加控件通知处理程序代码
	POSITION Pos=m_lvInterfaceList.GetFirstSelectedItemPosition();
	if(Pos)
	{
		int Item=m_lvInterfaceList.GetNextSelectedItem(Pos);
		UINT Index=(UINT)m_lvInterfaceList.GetItemData(Item);
		if(Index<m_InterfaceList.size())
		{
			CDlgInterfaceEdtor Dlg;
			Dlg.SetData(m_InterfaceList[Index]);

			if(Dlg.DoModal()==IDOK)
			{
				Dlg.GetData(m_InterfaceList[Index]);
				EditListItem(Item,m_InterfaceList[Index]);
			}
		}
	}
	else
	{
		MessageBox("请选择一个接口");
	}
}

void CCallInterfaceMakerDlg::OnBnClickedButtonDelInterface()
{
	// TODO: 在此添加控件通知处理程序代码
	POSITION Pos=m_lvInterfaceList.GetFirstSelectedItemPosition();
	if(Pos)
	{
		int Item=m_lvInterfaceList.GetNextSelectedItem(Pos);
		UINT Index=(UINT)m_lvInterfaceList.GetItemData(Item);
		if(Index<m_InterfaceList.size())
		{
			CString Msg;
			Msg.Format("是否要删除接口[%s]",
				m_InterfaceList[Index].Name);
			if(AfxMessageBox(Msg,MB_YESNO)==IDYES)
			{
				m_InterfaceList.erase(m_InterfaceList.begin()+Index);
				FillListItem();
			}
			
		}
	}
}

void CCallInterfaceMakerDlg::OnBnClickedButtonEditPrefix()
{
	// TODO: 在此添加控件通知处理程序代码
	CDlgTextEditor Dlg;

	Dlg.SetText(m_InterfacePrefix);
	if(Dlg.DoModal()==IDOK)
	{
		m_InterfacePrefix=Dlg.GetText();
	}
}

void CCallInterfaceMakerDlg::OnBnClickedButtonEditPostfix()
{
	// TODO: 在此添加控件通知处理程序代码
	CDlgTextEditor Dlg;

	Dlg.SetText(m_InterfacePostFix);
	if(Dlg.DoModal()==IDOK)
	{
		m_InterfacePostFix=Dlg.GetText();
	}
}


void CCallInterfaceMakerDlg::AddListItem(CALLER_INTERFACE& InterfaceInfo,UINT Index)
{
	CString Temp;
	int Item=m_lvInterfaceList.InsertItem(m_lvInterfaceList.GetItemCount(),InterfaceInfo.Name);
	m_lvInterfaceList.SetItemData(Item,(INT_PTR)Index);
	Temp.Format("%d",InterfaceInfo.MethodList.size());
	m_lvInterfaceList.SetItemText(Item,1,Temp);	
	if(InterfaceInfo.Type==INTERFACE_CALL)
	{
		m_lvInterfaceList.SetItemText(Item,2,"调用接口");	
	}
	else
	{
		m_lvInterfaceList.SetItemText(Item,2,"回调接口");	
	}
	Temp.Format("%d",InterfaceInfo.ID);
	m_lvInterfaceList.SetItemText(Item,3,Temp);
	m_lvInterfaceList.SetItemText(Item,4,InterfaceInfo.Description);
}
void CCallInterfaceMakerDlg::EditListItem(int Item,CALLER_INTERFACE& InterfaceInfo)
{
	CString Temp;
	m_lvInterfaceList.SetItemText(Item,0,InterfaceInfo.Name);
	Temp.Format("%d",InterfaceInfo.MethodList.size());
	m_lvInterfaceList.SetItemText(Item,1,Temp);	
	if(InterfaceInfo.Type==INTERFACE_CALL)
	{
		m_lvInterfaceList.SetItemText(Item,2,"调用接口");	
	}
	else
	{
		m_lvInterfaceList.SetItemText(Item,2,"回调接口");	
	}
	Temp.Format("%d",InterfaceInfo.ID);
	m_lvInterfaceList.SetItemText(Item,3,Temp);
	m_lvInterfaceList.SetItemText(Item,4,InterfaceInfo.Description);
}
bool CCallInterfaceMakerDlg::IsListItemChecked(UINT Index)
{
	for(int i=0;i<m_lvInterfaceList.GetItemCount();i++)
	{
		if(m_lvInterfaceList.GetCheck(i))
		{
			if((UINT)m_lvInterfaceList.GetItemData(i)==Index)
				return true;
		}
	}
	return false;
}
void CCallInterfaceMakerDlg::FillListItem()
{
	m_lvInterfaceList.DeleteAllItems();
	for(UINT i=0;i<m_InterfaceList.size();i++)
	{
		AddListItem(m_InterfaceList[i],i);
	}
}

bool CCallInterfaceMakerDlg::LoadTypeDef()
{
	xml_parser Parser;

	if(Parser.parse_file(m_TypeDefFileName,pug::parse_trim_attribute))
	{
		xml_node TypeDef=Parser.document();
		if(TypeDef.moveto_child("TypeDef"))
		{
			for(UINT i=0;i<TypeDef.children();i++)
			{
				xml_node Type=TypeDef.child(i);
				if(_stricmp(Type.name(),"Type")==0)
				{
					TYPE_DEFINE TypeInfo;
					TypeInfo.Name=DecodeString((LPCTSTR)Type.attribute("Name").getvalue());
					TypeInfo.CType = DecodeString((LPCTSTR)Type.attribute("CType").getvalue());
					LoadGenerateOperations(Type,TypeInfo.GenerateOperations);
					m_DataStructDefines.BaseTypeList.push_back(TypeInfo);
				}
			}
		}
		return true;
	}
	return false;
}
bool CCallInterfaceMakerDlg::SaveTypeDef()
{
	std::ofstream os;	
	os.open(m_TypeDefFileName, std::ios::trunc);	
	if (os.is_open())
	{
		pug::xml_parser Xml;

		Xml.create();
		xml_node Doc;
		Doc = Xml.document();
		xml_node pi = Doc.append_child(node_pi);
		pi.name(_T("xml"));
		pi.attribute(_T("version")) = _T("1.0");
		pi.attribute(_T("encoding")) = _T("gb2312");
		xml_node TypeDef=Doc.append_child(node_element,"TypeDef");
		
		for(size_t i=0;i<m_DataStructDefines.BaseTypeList.size();i++)
		{			
			xml_node Type=TypeDef.append_child(node_element,"Type");

			Type.append_attribute("Name",(LPCTSTR)m_DataStructDefines.BaseTypeList[i].Name);			
			Type.append_attribute("CType",(LPCTSTR)m_DataStructDefines.BaseTypeList[i].CType);
			SaveGenerateOperations(Type,m_DataStructDefines.BaseTypeList[i].GenerateOperations);
		}
		
		os << Xml.document();
		os.close();
		return true;
	}
	else
	{		
		return false;
	}
}
bool CCallInterfaceMakerDlg::LoadTemples()
{
	CFile TempleFile;

	CString FileName;

	FileName=MakeModuleFullPath(NULL,INTERFACE_HEADER_TEMPLE_FILE_NAME);
	if(TempleFile.Open(FileName,CFile::modeRead|CFile::shareDenyNone))
	{
		ULONG64 Size=TempleFile.GetLength();
		char * Buffer=new char[Size+1];
		TempleFile.Read(Buffer,Size);
		Buffer[Size]=0;
		m_InterfaceHeaderTemplate=Buffer;
		delete[] Buffer;
		TempleFile.Close();
	}
	else
	{
		CString Msg;
		Msg.Format("无法打开文件%s",FileName);
		MessageBox(Msg);
	}	

	FileName=MakeModuleFullPath(NULL,INTERFACE_TEMPLE_FILE_NAME);
	if(TempleFile.Open(FileName,CFile::modeRead|CFile::shareDenyNone))
	{
		ULONG64 Size=TempleFile.GetLength();
		char * Buffer=new char[Size+1];
		TempleFile.Read(Buffer,Size);
		Buffer[Size]=0;
		m_InterfaceTemplate=Buffer;
		delete[] Buffer;
		TempleFile.Close();
	}
	else
	{
		CString Msg;
		Msg.Format("无法打开文件%s",FileName);
		MessageBox(Msg);
	}	

	FileName=MakeModuleFullPath(NULL,PARAM_TEMPLE_FILE_NAME);
	if(TempleFile.Open(FileName,CFile::modeRead|CFile::shareDenyNone))
	{
		ULONG64 Size=TempleFile.GetLength();
		char * Buffer=new char[Size+1];
		TempleFile.Read(Buffer,Size);
		Buffer[Size]=0;
		m_ParamTemple=Buffer;
		delete[] Buffer;
		TempleFile.Close();
	}
	else
	{
		CString Msg;
		Msg.Format("无法打开文件%s",FileName);
		MessageBox(Msg);
	}

	FileName=MakeModuleFullPath(NULL,PARAM_DESCRIPTION_TEMPLE_FILE_NAME);
	if(TempleFile.Open(FileName,CFile::modeRead|CFile::shareDenyNone))
	{
		ULONG64 Size=TempleFile.GetLength();
		char * Buffer=new char[Size+1];
		TempleFile.Read(Buffer,Size);
		Buffer[Size]=0;
		m_ParamDescriptionTemple=Buffer;
		delete[] Buffer;
		TempleFile.Close();
	}
	else
	{
		CString Msg;
		Msg.Format("无法打开文件%s",FileName);
		MessageBox(Msg);
	}

	FileName=MakeModuleFullPath(NULL,PACK_SIZE_TEMPLE_FILE_NAME);
	if(TempleFile.Open(FileName,CFile::modeRead|CFile::shareDenyNone))
	{
		ULONG64 Size=TempleFile.GetLength();
		char * Buffer=new char[Size+1];
		TempleFile.Read(Buffer,Size);
		Buffer[Size]=0;
		m_PackSizeTemple=Buffer;
		delete[] Buffer;
		TempleFile.Close();
	}
	else
	{
		CString Msg;
		Msg.Format("无法打开文件%s",FileName);
		MessageBox(Msg);
	}

	FileName=MakeModuleFullPath(NULL,METHOD_HEADER_TEMPLE_FILE_NAME);
	if(TempleFile.Open(FileName,CFile::modeRead|CFile::shareDenyNone))
	{
		ULONG64 Size=TempleFile.GetLength();
		char * Buffer=new char[Size+1];
		TempleFile.Read(Buffer,Size);
		Buffer[Size]=0;
		m_MethodHeaderTemple=Buffer;
		delete[] Buffer;
		TempleFile.Close();
	}
	else
	{
		CString Msg;
		Msg.Format("无法打开文件%s",FileName);
		MessageBox(Msg);
	}

	FileName=MakeModuleFullPath(NULL,METHOD_HEADER_PURE_TAIL_TEMPLE_FILE_NAME);
	if(TempleFile.Open(FileName,CFile::modeRead|CFile::shareDenyNone))
	{
		ULONG64 Size=TempleFile.GetLength();
		char * Buffer=new char[Size+1];
		TempleFile.Read(Buffer,Size);
		Buffer[Size]=0;
		m_MethodHeaderPureTailTemple=Buffer;
		delete[] Buffer;
		TempleFile.Close();
	}
	else
	{
		CString Msg;
		Msg.Format("无法打开文件%s",FileName);
		MessageBox(Msg);
	}

	FileName=MakeModuleFullPath(NULL,METHOD_CALLER_SOURCE_TEMPLE_FILE_NAME);
	if(TempleFile.Open(FileName,CFile::modeRead|CFile::shareDenyNone))
	{
		ULONG64 Size=TempleFile.GetLength();
		char * Buffer=new char[Size+1];
		TempleFile.Read(Buffer,Size);
		Buffer[Size]=0;
		m_MethodCallerSourceTemple=Buffer;
		delete[] Buffer;
		TempleFile.Close();
	}
	else
	{
		CString Msg;
		Msg.Format("无法打开文件%s",FileName);
		MessageBox(Msg);
	}

	FileName=MakeModuleFullPath(NULL,MSG_PACK_METHOD_HEADER_TEMPLE_FILE_NAME);
	if(TempleFile.Open(FileName,CFile::modeRead|CFile::shareDenyNone))
	{
		ULONG64 Size=TempleFile.GetLength();
		char * Buffer=new char[Size+1];
		TempleFile.Read(Buffer,Size);
		Buffer[Size]=0;
		m_MsgPackMethodHeaderTemple=Buffer;
		delete[] Buffer;
		TempleFile.Close();
	}
	else
	{
		CString Msg;
		Msg.Format("无法打开文件%s",FileName);
		MessageBox(Msg);
	}

	FileName=MakeModuleFullPath(NULL,MSG_PACK_METHOD_SOURCE_TEMPLE_FILE_NAME);
	if(TempleFile.Open(FileName,CFile::modeRead|CFile::shareDenyNone))
	{
		ULONG64 Size=TempleFile.GetLength();
		char * Buffer=new char[Size+1];
		TempleFile.Read(Buffer,Size);
		Buffer[Size]=0;
		m_MsgPackMethodSourceTemple=Buffer;
		delete[] Buffer;
		TempleFile.Close();
	}
	else
	{
		CString Msg;
		Msg.Format("无法打开文件%s",FileName);
		MessageBox(Msg);
	}

	FileName=MakeModuleFullPath(NULL,MSG_HANDLER_METHOD_HEADER_TEMPLE_FILE_NAME);
	if(TempleFile.Open(FileName,CFile::modeRead|CFile::shareDenyNone))
	{
		ULONG64 Size=TempleFile.GetLength();
		char * Buffer=new char[Size+1];
		TempleFile.Read(Buffer,Size);
		Buffer[Size]=0;
		m_MsgHandlerMethodHeaderTemple=Buffer;
		delete[] Buffer;
		TempleFile.Close();
	}
	else
	{
		CString Msg;
		Msg.Format("无法打开文件%s",FileName);
		MessageBox(Msg);
	}

	FileName=MakeModuleFullPath(NULL,MSG_HANDLER_METHOD_SOURCE_TEMPLE_FILE_NAME);
	if(TempleFile.Open(FileName,CFile::modeRead|CFile::shareDenyNone))
	{
		ULONG64 Size=TempleFile.GetLength();
		char * Buffer=new char[Size+1];
		TempleFile.Read(Buffer,Size);
		Buffer[Size]=0;
		m_MsgHandlerMethodSourceTemple=Buffer;
		delete[] Buffer;
		TempleFile.Close();
	}
	else
	{
		CString Msg;
		Msg.Format("无法打开文件%s",FileName);
		MessageBox(Msg);
	}

	FileName=MakeModuleFullPath(NULL,MSG_MAP_INIT_TEMPLE_FILE_NAME);
	if(TempleFile.Open(FileName,CFile::modeRead|CFile::shareDenyNone))
	{
		ULONG64 Size=TempleFile.GetLength();
		char * Buffer=new char[Size+1];
		TempleFile.Read(Buffer,Size);
		Buffer[Size]=0;
		m_MsgMapInitTemple=Buffer;
		delete[] Buffer;
		TempleFile.Close();
	}
	else
	{
		CString Msg;
		Msg.Format("无法打开文件%s",FileName);
		MessageBox(Msg);
	}

	

	FileName=MakeModuleFullPath(NULL,MSG_CALLER_HEADER_TEMPLE_FILE_NAME);
	if(TempleFile.Open(FileName,CFile::modeRead|CFile::shareDenyNone))
	{
		ULONG64 Size=TempleFile.GetLength();
		char * Buffer=new char[Size+1];
		TempleFile.Read(Buffer,Size);
		Buffer[Size]=0;
		m_MsgCallerHeaderTemplate=Buffer;
		delete[] Buffer;
		TempleFile.Close();
	}
	else
	{
		CString Msg;
		Msg.Format("无法打开文件%s",FileName);
		MessageBox(Msg);
	}

	FileName=MakeModuleFullPath(NULL,MSG_CALLER_SOURCE_TEMPLE_FILE_NAME);
	if(TempleFile.Open(FileName,CFile::modeRead|CFile::shareDenyNone))
	{
		ULONG64 Size=TempleFile.GetLength();
		char * Buffer=new char[Size+1];
		TempleFile.Read(Buffer,Size);
		Buffer[Size]=0;
		m_MsgCallerSourceTemplate=Buffer;
		delete[] Buffer;
		TempleFile.Close();
	}
	else
	{
		CString Msg;
		Msg.Format("无法打开文件%s",FileName);
		MessageBox(Msg);
	}

	FileName=MakeModuleFullPath(NULL,MSG_HANDLER_HEADER_TEMPLE_FILE_NAME);
	if(TempleFile.Open(FileName,CFile::modeRead|CFile::shareDenyNone))
	{
		ULONG64 Size=TempleFile.GetLength();
		char * Buffer=new char[Size+1];
		TempleFile.Read(Buffer,Size);
		Buffer[Size]=0;
		m_MsgHandlerHeaderTemplate=Buffer;
		delete[] Buffer;
		TempleFile.Close();
	}
	else
	{
		CString Msg;
		Msg.Format("无法打开文件%s",FileName);
		MessageBox(Msg);
	}

	FileName=MakeModuleFullPath(NULL,MSG_HANDLER_SOURCE_TEMPLE_FILE_NAME);
	if(TempleFile.Open(FileName,CFile::modeRead|CFile::shareDenyNone))
	{
		ULONG64 Size=TempleFile.GetLength();
		char * Buffer=new char[Size+1];
		TempleFile.Read(Buffer,Size);
		Buffer[Size]=0;
		m_MsgHandlerSourceTemplate=Buffer;
		delete[] Buffer;
		TempleFile.Close();
	}
	else
	{
		CString Msg;
		Msg.Format("无法打开文件%s",FileName);
		MessageBox(Msg);
	}

	FileName=MakeModuleFullPath(NULL,DATA_OBJECT_DEFINE_HEADER_TEMPLE_FILE_NAME);
	if(TempleFile.Open(FileName,CFile::modeRead|CFile::shareDenyNone))
	{
		ULONG64 Size=TempleFile.GetLength();
		char * Buffer=new char[Size+1];
		TempleFile.Read(Buffer,Size);
		Buffer[Size]=0;
		m_DataObjectDefineHeaderTemple=Buffer;
		delete[] Buffer;
		TempleFile.Close();
	}
	else
	{
		CString Msg;
		Msg.Format("无法打开文件%s",FileName);
		MessageBox(Msg);
	}

	FileName=MakeModuleFullPath(NULL,DATA_OBJECT_DEFINE_SOURCE_TEMPLE_FILE_NAME);
	if(TempleFile.Open(FileName,CFile::modeRead|CFile::shareDenyNone))
	{
		ULONG64 Size=TempleFile.GetLength();
		char * Buffer=new char[Size+1];
		TempleFile.Read(Buffer,Size);
		Buffer[Size]=0;
		m_DataObjectDefineSourceTemple=Buffer;
		delete[] Buffer;
		TempleFile.Close();
	}
	else
	{
		CString Msg;
		Msg.Format("无法打开文件%s",FileName);
		MessageBox(Msg);
	}


	FileName=MakeModuleFullPath(NULL,STRUCT_DEFINE_HEADER_TEMPLE_FILE_NAME);
	if(TempleFile.Open(FileName,CFile::modeRead|CFile::shareDenyNone))
	{
		ULONG64 Size=TempleFile.GetLength();
		char * Buffer=new char[Size+1];
		TempleFile.Read(Buffer,Size);
		Buffer[Size]=0;
		m_StructDefineHeaderTemplate=Buffer;
		delete[] Buffer;
		TempleFile.Close();
	}
	else
	{
		CString Msg;
		Msg.Format("无法打开文件%s",FileName);
		MessageBox(Msg);
	}

	FileName=MakeModuleFullPath(NULL,ENUM_DEFINE_HEADER_TEMPLE_FILE_NAME);
	if(TempleFile.Open(FileName,CFile::modeRead|CFile::shareDenyNone))
	{
		ULONG64 Size=TempleFile.GetLength();
		char * Buffer=new char[Size+1];
		TempleFile.Read(Buffer,Size);
		Buffer[Size]=0;
		m_EnumDefineHeaderTemplate=Buffer;
		delete[] Buffer;
		TempleFile.Close();
	}
	else
	{
		CString Msg;
		Msg.Format("无法打开文件%s",FileName);
		MessageBox(Msg);
	}

	FileName=MakeModuleFullPath(NULL,SYSTEM_ENUM_DEFINE_HEADER_TEMPLE_FILE_NAME);
	if(TempleFile.Open(FileName,CFile::modeRead|CFile::shareDenyNone))
	{
		ULONG64 Size=TempleFile.GetLength();
		char * Buffer=new char[Size+1];
		TempleFile.Read(Buffer,Size);
		Buffer[Size]=0;
		m_SystemEnumDefineHeaderTemplate=Buffer;
		delete[] Buffer;
		TempleFile.Close();
	}
	else
	{
		CString Msg;
		Msg.Format("无法打开文件%s",FileName);
		MessageBox(Msg);
	}

	FileName=MakeModuleFullPath(NULL,STRUCT_DEFINE_TEMPLE_FILE_NAME);
	if(TempleFile.Open(FileName,CFile::modeRead|CFile::shareDenyNone))
	{
		ULONG64 Size=TempleFile.GetLength();
		char * Buffer=new char[Size+1];
		TempleFile.Read(Buffer,Size);
		Buffer[Size]=0;
		m_StructDefineTemplate=Buffer;
		delete[] Buffer;
		TempleFile.Close();
	}
	else
	{
		CString Msg;
		Msg.Format("无法打开文件%s",FileName);
		MessageBox(Msg);
	}
	
	FileName=MakeModuleFullPath(NULL,STRUCT_MEMBER_TEMPLE_FILE_NAME);
	if(TempleFile.Open(FileName,CFile::modeRead|CFile::shareDenyNone))
	{
		ULONG64 Size=TempleFile.GetLength();
		char * Buffer=new char[Size+1];
		TempleFile.Read(Buffer,Size);
		Buffer[Size]=0;
		m_StructMemberTemplate=Buffer;
		delete[] Buffer;
		TempleFile.Close();
	}
	else
	{
		CString Msg;
		Msg.Format("无法打开文件%s",FileName);
		MessageBox(Msg);
	}

	FileName=MakeModuleFullPath(NULL,DATA_OBJECT_MODIFY_FLAGS_TEMPLE_FILE_NAME);
	if(TempleFile.Open(FileName,CFile::modeRead|CFile::shareDenyNone))
	{
		ULONG64 Size=TempleFile.GetLength();
		char * Buffer=new char[Size+1];
		TempleFile.Read(Buffer,Size);
		Buffer[Size]=0;
		m_DataObjectModifyFlagsTemple=Buffer;
		delete[] Buffer;
		TempleFile.Close();
	}
	else
	{
		CString Msg;
		Msg.Format("无法打开文件%s",FileName);
		MessageBox(Msg);
	}

	FileName=MakeModuleFullPath(NULL,DATA_OBJECT_MODIFY_FLAG_DEFINE_TEMPLE_FILE_NAME);
	if(TempleFile.Open(FileName,CFile::modeRead|CFile::shareDenyNone))
	{
		ULONG64 Size=TempleFile.GetLength();
		char * Buffer=new char[Size+1];
		TempleFile.Read(Buffer,Size);
		Buffer[Size]=0;
		m_DataObjectModifyFlagDefineTemple=Buffer;
		delete[] Buffer;
		TempleFile.Close();
	}
	else
	{
		CString Msg;
		Msg.Format("无法打开文件%s",FileName);
		MessageBox(Msg);
	}

	FileName=MakeModuleFullPath(NULL,DATA_OBJECT_MODIFY_FLAG_UNIT_TEMPLE_FILE_NAME);
	if(TempleFile.Open(FileName,CFile::modeRead|CFile::shareDenyNone))
	{
		ULONG64 Size=TempleFile.GetLength();
		char * Buffer=new char[Size+1];
		TempleFile.Read(Buffer,Size);
		Buffer[Size]=0;
		m_DataObjectModifyFlagUnitTemple=Buffer;
		delete[] Buffer;
		TempleFile.Close();
	}
	else
	{
		CString Msg;
		Msg.Format("无法打开文件%s",FileName);
		MessageBox(Msg);
	}

	FileName=MakeModuleFullPath(NULL,PACK_OPERATION_UNIT_TEMPLE_FILE_NAME);
	if(TempleFile.Open(FileName,CFile::modeRead|CFile::shareDenyNone))
	{
		ULONG64 Size=TempleFile.GetLength();
		char * Buffer=new char[Size+1];
		TempleFile.Read(Buffer,Size);
		Buffer[Size]=0;
		m_PackOperationUnitTemple=Buffer;
		delete[] Buffer;
		TempleFile.Close();
	}
	else
	{
		CString Msg;
		Msg.Format("无法打开文件%s",FileName);
		MessageBox(Msg);
	}

	FileName=MakeModuleFullPath(NULL,UNPACK_OPERATION_UNIT_TEMPLE_FILE_NAME);
	if(TempleFile.Open(FileName,CFile::modeRead|CFile::shareDenyNone))
	{
		ULONG64 Size=TempleFile.GetLength();
		char * Buffer=new char[Size+1];
		TempleFile.Read(Buffer,Size);
		Buffer[Size]=0;
		m_UnpackOperationUnitTemple=Buffer;
		delete[] Buffer;
		TempleFile.Close();
	}
	else
	{
		CString Msg;
		Msg.Format("无法打开文件%s",FileName);
		MessageBox(Msg);
	}

	FileName=MakeModuleFullPath(NULL,ENUM_DFINE_TEMPLE_FILE_NAME);
	if(TempleFile.Open(FileName,CFile::modeRead|CFile::shareDenyNone))
	{
		ULONG64 Size=TempleFile.GetLength();
		char * Buffer=new char[Size+1];
		TempleFile.Read(Buffer,Size);
		Buffer[Size]=0;
		m_EnumDefineTemplate=Buffer;
		delete[] Buffer;
		TempleFile.Close();
	}
	else
	{
		CString Msg;
		Msg.Format("无法打开文件%s",FileName);
		MessageBox(Msg);
	}

	FileName=MakeModuleFullPath(NULL,ENUM_MEMBER_DFINE_TEMPLE_FILE_NAME);
	if(TempleFile.Open(FileName,CFile::modeRead|CFile::shareDenyNone))
	{
		ULONG64 Size=TempleFile.GetLength();
		char * Buffer=new char[Size+1];
		TempleFile.Read(Buffer,Size);
		Buffer[Size]=0;
		m_EnumMemberDefineTemplate=Buffer;
		delete[] Buffer;
		TempleFile.Close();
	}
	else
	{
		CString Msg;
		Msg.Format("无法打开文件%s",FileName);
		MessageBox(Msg);
	}

	FileName=MakeModuleFullPath(NULL,ENUM_STR_VALUE_DFINE_TEMPLE_FILE_NAME);
	if(TempleFile.Open(FileName,CFile::modeRead|CFile::shareDenyNone))
	{
		ULONG64 Size=TempleFile.GetLength();
		char * Buffer=new char[Size+1];
		TempleFile.Read(Buffer,Size);
		Buffer[Size]=0;
		m_EnumStrValueDefineTemplate=Buffer;
		delete[] Buffer;
		TempleFile.Close();
	}
	else
	{
		CString Msg;
		Msg.Format("无法打开文件%s",FileName);
		MessageBox(Msg);
	}

	FileName=MakeModuleFullPath(NULL,CONST_DFINE_TEMPLE_FILE_NAME);
	if(TempleFile.Open(FileName,CFile::modeRead|CFile::shareDenyNone))
	{
		ULONG64 Size=TempleFile.GetLength();
		char * Buffer=new char[Size+1];
		TempleFile.Read(Buffer,Size);
		Buffer[Size]=0;
		m_ConstDefineTemplate=Buffer;
		delete[] Buffer;
		TempleFile.Close();
	}
	else
	{
		CString Msg;
		Msg.Format("无法打开文件%s",FileName);
		MessageBox(Msg);
	}

	FileName=MakeModuleFullPath(NULL,MACRO_DFINE_TEMPLE_FILE_NAME);
	if(TempleFile.Open(FileName,CFile::modeRead|CFile::shareDenyNone))
	{
		ULONG64 Size=TempleFile.GetLength();
		char * Buffer=new char[Size+1];
		TempleFile.Read(Buffer,Size);
		Buffer[Size]=0;
		m_MacroDefineTemplate=Buffer;
		delete[] Buffer;
		TempleFile.Close();
	}
	else
	{
		CString Msg;
		Msg.Format("无法打开文件%s",FileName);
		MessageBox(Msg);
	}

	FileName=MakeModuleFullPath(NULL,CONST_DFINE_HEADER_TEMPLE_FILE_NAME);
	if(TempleFile.Open(FileName,CFile::modeRead|CFile::shareDenyNone))
	{
		ULONG64 Size=TempleFile.GetLength();
		char * Buffer=new char[Size+1];
		TempleFile.Read(Buffer,Size);
		Buffer[Size]=0;
		m_ConstDefineHeaderTemplate=Buffer;
		delete[] Buffer;
		TempleFile.Close();
	}
	else
	{
		CString Msg;
		Msg.Format("无法打开文件%s",FileName);
		MessageBox(Msg);
	}

	FileName=MakeModuleFullPath(NULL,STRING_CONST_DFINE_TEMPLE_FILE_NAME);
	if(TempleFile.Open(FileName,CFile::modeRead|CFile::shareDenyNone))
	{
		ULONG64 Size=TempleFile.GetLength();
		char * Buffer=new char[Size+1];
		TempleFile.Read(Buffer,Size);
		Buffer[Size]=0;
		m_StringConstDefineTemplate=Buffer;
		delete[] Buffer;
		TempleFile.Close();
	}
	else
	{
		CString Msg;
		Msg.Format("无法打开文件%s",FileName);
		MessageBox(Msg);
	}


	return true;
}

bool CCallInterfaceMakerDlg::LoadConfig()
{
	xml_parser Parser;

	CString FileName=MakeModuleFullPath(NULL,CONFIG_FILE_NAME);

	if(Parser.parse_file(FileName,pug::parse_trim_attribute))
	{

		xml_node Config=Parser.document();
			
		if(Config.moveto_child("Config"))
		{
			{
				xml_node InterfaceHeaderExport=Config;
				if(InterfaceHeaderExport.moveto_child("InterfaceHeaderExport"))
				{
					m_InterfaceConfig.InterfaceHeaderExportConfig.IsExport=(bool)InterfaceHeaderExport.attribute("IsExport");
					m_InterfaceConfig.InterfaceHeaderExportConfig.ExportExt = (LPCTSTR)InterfaceHeaderExport.attribute("FileExt").getvalue();
				}
			}
			{
				xml_node StructExport=Config;
				if(StructExport.moveto_child("StructExport"))
				{
					m_InterfaceConfig.StructExportConfig.IsExport=(bool)StructExport.attribute("IsExport");
					m_InterfaceConfig.StructExportConfig.ExportExt = (LPCTSTR)StructExport.attribute("FileExt").getvalue();
				}
			}
			{
				xml_node EnumExport=Config;
				if(EnumExport.moveto_child("EnumExport"))
				{
					m_InterfaceConfig.EnumExportConfig.IsExport=(bool)EnumExport.attribute("IsExport");
					m_InterfaceConfig.EnumExportConfig.ExportExt = (LPCTSTR)EnumExport.attribute("FileExt").getvalue();
				}
			}
			{
				xml_node ConstExport=Config;
				if(ConstExport.moveto_child("ConstExport"))
				{
					m_InterfaceConfig.ConstExportConfig.IsExport=(bool)ConstExport.attribute("IsExport");
					m_InterfaceConfig.ConstExportConfig.ExportExt = (LPCTSTR)ConstExport.attribute("FileExt").getvalue();
				}
			}
			{
				xml_node DOHeaderExport=Config;
				if(DOHeaderExport.moveto_child("DataStructHeaderExport"))
				{
					m_InterfaceConfig.DataStructHeaderExportConfig.IsExport=(bool)DOHeaderExport.attribute("IsExport");
					m_InterfaceConfig.DataStructHeaderExportConfig.ExportExt = (LPCTSTR)DOHeaderExport.attribute("FileExt").getvalue();
				}
			}
			{
				xml_node InterfaceExportConfig=Config;
				if(InterfaceExportConfig.moveto_child("InterfaceExport"))
				{
					m_InterfaceConfig.InterfaceExportConfig.IsExport=(bool)InterfaceExportConfig.attribute("IsExport");
					m_InterfaceConfig.InterfaceExportConfig.ExportExt = (LPCTSTR)InterfaceExportConfig.attribute("FileExt").getvalue();
				}
			}
			{
				xml_node CallHeaderExportConfig=Config;
				if(CallHeaderExportConfig.moveto_child("CallHeaderExport"))
				{
					m_InterfaceConfig.CallHeaderExportConfig.IsExport=(bool)CallHeaderExportConfig.attribute("IsExport");
					m_InterfaceConfig.CallHeaderExportConfig.ExportExt = (LPCTSTR)CallHeaderExportConfig.attribute("FileExt").getvalue();
				}
			}
			{
				xml_node CallSourceExportConfig=Config;
				if(CallSourceExportConfig.moveto_child("CallSourceExport"))
				{
					m_InterfaceConfig.CallSourceExportConfig.IsExport=(bool)CallSourceExportConfig.attribute("IsExport");
					m_InterfaceConfig.CallSourceExportConfig.ExportExt = (LPCTSTR)CallSourceExportConfig.attribute("FileExt").getvalue();
				}
			}
			{
				xml_node HandlerHeaderExportConfig=Config;
				if(HandlerHeaderExportConfig.moveto_child("HandlerHeaderExport"))
				{
					m_InterfaceConfig.HandlerHeaderExportConfig.IsExport=(bool)HandlerHeaderExportConfig.attribute("IsExport");
					m_InterfaceConfig.HandlerHeaderExportConfig.ExportExt = (LPCTSTR)HandlerHeaderExportConfig.attribute("FileExt").getvalue();
				}
			}
			{
				xml_node HandlerSourceExportConfig=Config;
				if(HandlerSourceExportConfig.moveto_child("HandlerSourceExport"))
				{
					m_InterfaceConfig.HandlerSourceExportConfig.IsExport=(bool)HandlerSourceExportConfig.attribute("IsExport");
					m_InterfaceConfig.HandlerSourceExportConfig.ExportExt = (LPCTSTR)HandlerSourceExportConfig.attribute("FileExt").getvalue();
				}
			}
			{
				xml_node DataObjectHeaderExportConfig=Config;
				if(DataObjectHeaderExportConfig.moveto_child("DataObjectHeaderExport"))
				{
					m_InterfaceConfig.DataObjectHeaderExportConfig.IsExport=(bool)DataObjectHeaderExportConfig.attribute("IsExport");
					m_InterfaceConfig.DataObjectHeaderExportConfig.ExportExt = (LPCTSTR)DataObjectHeaderExportConfig.attribute("FileExt").getvalue();
				}
			}
			{
				xml_node DataObjectSourceExportExportConfig=Config;
				if(DataObjectSourceExportExportConfig.moveto_child("DataObjectSourceExport"))
				{
					m_InterfaceConfig.DataObjectSourceExportConfig.IsExport=(bool)DataObjectSourceExportExportConfig.attribute("IsExport");
					m_InterfaceConfig.DataObjectSourceExportConfig.ExportExt = (LPCTSTR)DataObjectSourceExportExportConfig.attribute("FileExt").getvalue();
				}
			}
			{
				xml_node DataObjectModifyFlagsExportConfig=Config;
				if(DataObjectModifyFlagsExportConfig.moveto_child("DataObjectModifyFlagsExport"))
				{
					m_InterfaceConfig.DataObjectModifyFlagsExportConfig.IsExport=(bool)DataObjectModifyFlagsExportConfig.attribute("IsExport");
					m_InterfaceConfig.DataObjectModifyFlagsExportConfig.ExportExt = (LPCTSTR)DataObjectModifyFlagsExportConfig.attribute("FileExt").getvalue();
				}
			}
			{
				xml_node DefaultStructGenerateOperations=Config;
				if(DefaultStructGenerateOperations.moveto_child("DefaultStructGenerateOperations"))
				{
					LoadGenerateOperations(DefaultStructGenerateOperations,m_InterfaceConfig.DefaultStructGenerateOperations);
				}
			}
			{
				xml_node ArrayDefineConfig=Config;
				if(ArrayDefineConfig.moveto_child("ArrayDefineConfig"))
				{
					LoadGenerateOperations(ArrayDefineConfig,m_InterfaceConfig.ArrayDefineConfig);
				}
			}
		}
			
	}
	else
	{
		CString Msg;
		Msg.Format("无法打开文件%s",FileName);
		MessageBox(Msg);
		return false;
	}
	return true;
}
bool CCallInterfaceMakerDlg::SaveConfig()
{
	std::ofstream os;	
	CString FileName=MakeModuleFullPath(NULL,CONFIG_FILE_NAME);
	os.open(FileName, std::ios::trunc);	
	if (os.is_open())
	{
		pug::xml_parser Xml;

		Xml.create();
		xml_node Doc;
		Doc = Xml.document();
		xml_node pi = Doc.append_child(node_pi);
		pi.name(_T("xml"));
		pi.attribute(_T("version")) = _T("1.0");
		pi.attribute(_T("encoding")) = _T("gb2312");

		xml_node Config=Doc.append_child(node_element,"Config");

		{
			xml_node InterfaceHeaderExport=Config.append_child(node_element,"InterfaceHeaderExport");
			InterfaceHeaderExport.append_attribute("IsExport",
				(bool)m_InterfaceConfig.InterfaceHeaderExportConfig.IsExport);
			InterfaceHeaderExport.append_attribute("FileExt",
				m_InterfaceConfig.InterfaceHeaderExportConfig.ExportExt);
		}

		{
			xml_node StructExport=Config.append_child(node_element,"StructExport");
			StructExport.append_attribute("IsExport",
				(bool)m_InterfaceConfig.StructExportConfig.IsExport);
			StructExport.append_attribute("FileExt",
				m_InterfaceConfig.StructExportConfig.ExportExt);
		}

		{
			xml_node EnumExport=Config.append_child(node_element,"EnumExport");
			EnumExport.append_attribute("IsExport",
				(bool)m_InterfaceConfig.EnumExportConfig.IsExport);
			EnumExport.append_attribute("FileExt",
				m_InterfaceConfig.EnumExportConfig.ExportExt);
		}

		{
			xml_node ConstExport=Config.append_child(node_element,"ConstExport");
			ConstExport.append_attribute("IsExport",
				(bool)m_InterfaceConfig.ConstExportConfig.IsExport);
			ConstExport.append_attribute("FileExt",
				m_InterfaceConfig.ConstExportConfig.ExportExt);
		}

		{
			xml_node DOHeaderExport=Config.append_child(node_element,"DataStructHeaderExport");
			DOHeaderExport.append_attribute("IsExport",
				(bool)m_InterfaceConfig.DataStructHeaderExportConfig.IsExport);
			DOHeaderExport.append_attribute("FileExt",
				m_InterfaceConfig.DataStructHeaderExportConfig.ExportExt);
		}

		{
			xml_node InterfaceExportConfig=Config.append_child(node_element,"InterfaceExport");
			InterfaceExportConfig.append_attribute("IsExport",
				(bool)m_InterfaceConfig.InterfaceExportConfig.IsExport);
			InterfaceExportConfig.append_attribute("FileExt",
				m_InterfaceConfig.InterfaceExportConfig.ExportExt);
		}

		{
			xml_node CallHeaderExportConfig=Config.append_child(node_element,"CallHeaderExport");
			CallHeaderExportConfig.append_attribute("IsExport",
				(bool)m_InterfaceConfig.CallHeaderExportConfig.IsExport);
			CallHeaderExportConfig.append_attribute("FileExt",
				m_InterfaceConfig.CallHeaderExportConfig.ExportExt);
		}

		{
			xml_node CallSourceExportConfig=Config.append_child(node_element,"CallSourceExport");
			CallSourceExportConfig.append_attribute("IsExport",
				(bool)m_InterfaceConfig.CallSourceExportConfig.IsExport);
			CallSourceExportConfig.append_attribute("FileExt",
				m_InterfaceConfig.CallSourceExportConfig.ExportExt);
		}

		{
			xml_node HandlerHeaderExportConfig=Config.append_child(node_element,"HandlerHeaderExport");
			HandlerHeaderExportConfig.append_attribute("IsExport",
				(bool)m_InterfaceConfig.HandlerHeaderExportConfig.IsExport);
			HandlerHeaderExportConfig.append_attribute("FileExt",
				m_InterfaceConfig.HandlerHeaderExportConfig.ExportExt);
		}

		{
			xml_node HandlerSourceExportConfig=Config.append_child(node_element,"HandlerSourceExport");
			HandlerSourceExportConfig.append_attribute("IsExport",
				(bool)m_InterfaceConfig.HandlerSourceExportConfig.IsExport);
			HandlerSourceExportConfig.append_attribute("FileExt",
				m_InterfaceConfig.HandlerSourceExportConfig.ExportExt);
		}

		{
			xml_node DataObjectHeaderExportConfig=Config.append_child(node_element,"DataObjectHeaderExport");
			DataObjectHeaderExportConfig.append_attribute("IsExport",
				(bool)m_InterfaceConfig.DataObjectHeaderExportConfig.IsExport);
			DataObjectHeaderExportConfig.append_attribute("FileExt",
				m_InterfaceConfig.DataObjectHeaderExportConfig.ExportExt);
		}

		{
			xml_node DataObjectSourceExportConfig=Config.append_child(node_element,"DataObjectSourceExport");
			DataObjectSourceExportConfig.append_attribute("IsExport",
				(bool)m_InterfaceConfig.DataObjectSourceExportConfig.IsExport);
			DataObjectSourceExportConfig.append_attribute("FileExt",
				m_InterfaceConfig.DataObjectSourceExportConfig.ExportExt);
		}

		{
			xml_node DataObjectModifyFlagsExport=Config.append_child(node_element,"DataObjectModifyFlagsExport");
			DataObjectModifyFlagsExport.append_attribute("IsExport",
				(bool)m_InterfaceConfig.DataObjectModifyFlagsExportConfig.IsExport);
			DataObjectModifyFlagsExport.append_attribute("FileExt",
				m_InterfaceConfig.DataObjectModifyFlagsExportConfig.ExportExt);
		}

		{
			xml_node DefaultStructPackOperation=Config.append_child(node_element,"DefaultStructGenerateOperations");

			SaveGenerateOperations(DefaultStructPackOperation,m_InterfaceConfig.DefaultStructGenerateOperations);			
		}

		{
			xml_node ArrayDefineConfig=Config.append_child(node_element,"ArrayDefineConfig");

			SaveGenerateOperations(ArrayDefineConfig,m_InterfaceConfig.ArrayDefineConfig);			
		}


		os << Xml.document();
		os.close();
		return true;
	}
	else
	{		
		CString Msg;
		Msg.Format("无法打开文件%s",FileName);
		MessageBox(Msg);
		return false;
	}
}

bool CCallInterfaceMakerDlg::LoadEnv()
{
	xml_parser Parser;

	CString FileName=MakeModuleFullPath(NULL,ENV_FILE_NAME);
	if(Parser.parse_file(FileName,pug::parse_trim_attribute))
	{

		xml_node RunEnvironment=Parser.document();
			
		if(RunEnvironment.moveto_child("RunEnvironment"))
		{
			{
				xml_node ExportEnv=RunEnvironment;
				if(ExportEnv.moveto_child("ExportEnv"))
				{
					m_RunEnvInfo.RecentExportDir = DecodeString((LPCTSTR)ExportEnv.attribute("RecentExportDir").getvalue());
				}

				xml_node DataEnv=RunEnvironment;
				if(DataEnv.moveto_child("DataEnv"))
				{
					m_RunEnvInfo.RecentDataDir = DecodeString((LPCTSTR)DataEnv.attribute("RecentDataDir").getvalue());
				}
			}

			xml_node SelectedInterfaces=RunEnvironment;
			if(RunEnvironment.moveto_child("SelectedInterfaces"))
			{
				for(UINT i=0;i<RunEnvironment.children();i++)
				{
					CString InterfaceName=RunEnvironment.child(i).name();
					for(int j=0;j<m_lvInterfaceList.GetItemCount();j++)
					{
						if(m_lvInterfaceList.GetItemText(j,0)==InterfaceName)
						{
							m_lvInterfaceList.SetCheck(j);
						}
					}
				}
			}
		}
			
		
	}
	else
	{
		CString Msg;
		Msg.Format("无法打开文件%s",FileName);
		MessageBox(Msg);
		return false;
	}
	return true;
}
bool CCallInterfaceMakerDlg::SaveEnv()
{
	std::ofstream os;	
	CString FileName=MakeModuleFullPath(NULL,ENV_FILE_NAME);
	os.open(FileName, std::ios::trunc);	
	if (os.is_open())
	{
		pug::xml_parser Xml;

		Xml.create();
		xml_node Doc;
		Doc = Xml.document();
		xml_node pi = Doc.append_child(node_pi);
		pi.name(_T("xml"));
		pi.attribute(_T("version")) = _T("1.0");
		pi.attribute(_T("encoding")) = _T("gb2312");

		xml_node RunEnvironment=Doc.append_child(node_element,"RunEnvironment");

		{
			xml_node ExportEnv=RunEnvironment.append_child(node_element,"ExportEnv");
			ExportEnv.append_attribute("RecentExportDir",
				EncodeString(m_RunEnvInfo.RecentExportDir));

			xml_node DataEnv=RunEnvironment.append_child(node_element,"DataEnv");
			DataEnv.append_attribute("RecentDataDir",
				EncodeString(m_RunEnvInfo.RecentDataDir));
		}

		xml_node SelectedInterfaces=RunEnvironment.append_child(node_element,"SelectedInterfaces");

		for(size_t i=0;i<m_InterfaceList.size();i++)
		{
			if(IsListItemChecked(i))
			{
				SelectedInterfaces.append_child(node_element,m_InterfaceList[i].Name);
			}
		}

	
		os << Xml.document();
		os.close();
		return true;
	}
	else
	{	
		CString Msg;
		Msg.Format("无法打开文件%s",FileName);
		MessageBox(Msg);
		return false;
	}
}

void CCallInterfaceMakerDlg::MakeCurVarType(bool IncludeDataObject)
{
	m_CurVarTypeList=m_DataStructDefines.BaseTypeList;
	for(size_t i=0;i<m_DataStructDefines.DataStructDefineList.size();i++)
	{
		if(m_DataStructDefines.DataStructDefineList[i]->ListType==DATA_STRUCT_STRUCT)
		{
			STRUCT_DEFINE_LIST * pStructList=(STRUCT_DEFINE_LIST *)m_DataStructDefines.DataStructDefineList[i];
			for(size_t j=0;j<pStructList->StructList.size();j++)
			{
				if((!pStructList->StructList[j].IsDataObject)||IncludeDataObject)
				{
					TYPE_DEFINE Type;
					Type.Name=pStructList->StructList[j].Name;
					Type.CType=pStructList->StructList[j].Name;
					Type.Flag=TYPE_DEFINE_FLAG_STRUCT;
					if(pStructList->StructList[j].IsDataObject)
						Type.Flag|=TYPE_DEFINE_FLAG_DATA_OBJECT;
					Type.GenerateOperations=pStructList->StructList[j].GenerateOperations;			

					if(Type.GenerateOperations.PackOperation.IsEmpty())
						Type.GenerateOperations.PackOperation=m_InterfaceConfig.DefaultStructGenerateOperations.PackOperation;
					if(Type.GenerateOperations.UnpackOperation.IsEmpty())
						Type.GenerateOperations.UnpackOperation=m_InterfaceConfig.DefaultStructGenerateOperations.UnpackOperation;
					if(Type.GenerateOperations.SizeCaculateOperation.IsEmpty())
						Type.GenerateOperations.SizeCaculateOperation=m_InterfaceConfig.DefaultStructGenerateOperations.SizeCaculateOperation;
					if(Type.GenerateOperations.ReferenceDefine.IsEmpty())
						Type.GenerateOperations.ReferenceDefine=m_InterfaceConfig.DefaultStructGenerateOperations.ReferenceDefine;
					if(Type.GenerateOperations.ReferenceUse.IsEmpty())
						Type.GenerateOperations.ReferenceUse=m_InterfaceConfig.DefaultStructGenerateOperations.ReferenceUse;
					if(Type.GenerateOperations.VariableDefine.IsEmpty())
						Type.GenerateOperations.VariableDefine=m_InterfaceConfig.DefaultStructGenerateOperations.VariableDefine;
					if(Type.GenerateOperations.InitOperation.IsEmpty())
						Type.GenerateOperations.InitOperation=m_InterfaceConfig.DefaultStructGenerateOperations.InitOperation;
					if(Type.GenerateOperations.CloneOperation.IsEmpty())
						Type.GenerateOperations.CloneOperation=m_InterfaceConfig.DefaultStructGenerateOperations.CloneOperation;
					if(Type.GenerateOperations.GetMethodDeclare.IsEmpty())
						Type.GenerateOperations.GetMethodDeclare=m_InterfaceConfig.DefaultStructGenerateOperations.GetMethodDeclare;
					if(Type.GenerateOperations.GetMethodDefine.IsEmpty())
						Type.GenerateOperations.GetMethodDefine=m_InterfaceConfig.DefaultStructGenerateOperations.GetMethodDefine;
					if(Type.GenerateOperations.SetMethodDeclare.IsEmpty())
						Type.GenerateOperations.SetMethodDeclare=m_InterfaceConfig.DefaultStructGenerateOperations.SetMethodDeclare;
					if(Type.GenerateOperations.SetMethodDefine.IsEmpty())
						Type.GenerateOperations.SetMethodDefine=m_InterfaceConfig.DefaultStructGenerateOperations.SetMethodDefine;

					m_CurVarTypeList.push_back(Type);
				}
			}
		}
	}
	
}
void CCallInterfaceMakerDlg::OnBnClickedButtonLoad()
{
	// TODO: 在此添加控件通知处理程序代码	
	CFileDialog Dlg(true,"*.xml","*.xml",OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,"XML Files(*.xml)|*,xml|All Files(*.*)|*.*||");

	if(!m_RunEnvInfo.RecentDataDir.IsEmpty())
	{
		Dlg.m_ofn.lpstrInitialDir=m_RunEnvInfo.RecentDataDir;
	}

	if(Dlg.DoModal()==IDOK)
	{
		m_InterfacesFileName=Dlg.GetPathName();
		Load(m_InterfacesFileName);
		m_RunEnvInfo.RecentDataDir=Dlg.GetFolderPath();
		MakeCurVarType(true);
	}
}
void CCallInterfaceMakerDlg::OnBnClickedButtonSave()
{
	// TODO: 在此添加控件通知处理程序代码
	if(m_InterfacesFileName.IsEmpty())
	{
		OnBnClickedButtonSaveAs();
	}
	else
	{
		Save(m_InterfacesFileName);
	}
}

void CCallInterfaceMakerDlg::OnBnClickedButtonSaveAs()
{
	// TODO: 在此添加控件通知处理程序代码
	CFileDialog Dlg(false,"*.xml","*.xml",OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,"XML Files(*.xml)|*,xml|All Files(*.*)|*.*||");

	if(Dlg.DoModal()==IDOK)
	{
		m_InterfacesFileName=Dlg.GetPathName();
		Save(m_InterfacesFileName);
		m_RunEnvInfo.RecentDataDir=Dlg.GetFolderPath();
	}
}
int CALLBACK BrowseCallbackProc(HWND hwnd,UINT uMsg,LPARAM lParam,LPARAM lpData)
{
	if(uMsg == BFFM_INITIALIZED)
	{
		CString DefaultFolder=GetMainDlg()->GetRunEnv().RecentExportDir;
		if(!DefaultFolder.IsEmpty())
		{
			SendMessage(hwnd, BFFM_SETSELECTION,
				TRUE,(LPARAM)((LPCTSTR)DefaultFolder));
		}
	}
	return 0;
}

void CCallInterfaceMakerDlg::OnBnClickedButtonSaveExport()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(true);


	BROWSEINFO bi;
	char FilePath[MAX_PATH];
	LPITEMIDLIST pidl;

	ZeroMemory(&bi,sizeof(bi));

	//bi.pszDisplayName=FilePath;
	bi.lpszTitle="请选择输出目录";
	bi.ulFlags=BIF_RETURNONLYFSDIRS|BIF_USENEWUI|BIF_SHAREABLE;
	bi.lpfn=BrowseCallbackProc;
	pidl=::SHBrowseForFolder(&bi);
	if(pidl!=NULL)
	{
		::SHGetPathFromIDList(pidl,FilePath);
		IMalloc *pMalloc=NULL;
		::SHGetMalloc(&pMalloc);
		pMalloc->Free(pidl);
		pMalloc->Release();
		pMalloc=NULL;
		
		m_RunEnvInfo.RecentExportDir=FilePath;
		SaveEnv();
		
		MakeCurVarType(true);
		if(m_InterfaceConfig.InterfaceHeaderExportConfig.IsExport)
		{
			CString FileName;

			FileName.Format("%sInterfaces.%s",m_Name,m_InterfaceConfig.InterfaceHeaderExportConfig.ExportExt);
			ExportInterfaceHeader(FilePath,FileName);
		}

		if(m_InterfaceConfig.DataStructHeaderExportConfig.IsExport)
		{
			CString FileName;

			FileName.Format("%sDataStructs.%s",m_Name,m_InterfaceConfig.DataStructHeaderExportConfig.ExportExt);
			ExportDataStructDefineHeader(FilePath,FileName);
			
		}
		ExportDataStructDefines(FilePath);
		ExportDataObject(FilePath);
		ExportInterfaces(FilePath);

		
		AfxMessageBox("导出完毕");
		//FileName.Format("If%sMsgHandler.h",m_Name);

		//ExportMsgHandlerFile(FilePath,FileName);

		//FileName.Format("If%sSkeleton.h",m_Name);

		//ExportSkeletonFile(FilePath,FileName);

		//FileName.Format("If%sDoc.txt",m_Name);

		//ExportInterfaceDoc(FilePath,FileName);

	}	


	
}

void CCallInterfaceMakerDlg::OnCancel()
{
	// TODO: 在此添加专用代码和/或调用基类

	SaveConfig();
	//SaveEnv();
	CDialog::OnCancel();
}

void CCallInterfaceMakerDlg::OnOK()
{
	// TODO: 在此添加专用代码和/或调用基类

	//CDialog::OnOK();
}

bool CCallInterfaceMakerDlg::Save(LPCTSTR FileName)
{
	UpdateData(true);

	std::ofstream os;	
	os.open(FileName, std::ios::trunc);	
	if (os.is_open())
	{
		pug::xml_parser Xml;

		Xml.create();
		xml_node Doc;
		Doc = Xml.document();
		xml_node pi = Doc.append_child(node_pi);
		pi.name(_T("xml"));
		pi.attribute(_T("version")) = _T("1.0");
		pi.attribute(_T("encoding")) = _T("gb2312");
		

		xml_node Interfaces=Doc.append_child(node_element,"Interfaces");
		Interfaces.append_attribute("Name",(LPCTSTR)m_Name);
		Interfaces.append_attribute("IDSeed",(long)m_InterfaceIDSeed);

		xml_node GlobalDefine=Interfaces.append_child(node_element,"GlobalDefine");
		for(size_t i=0;i<m_DataStructDefines.DataStructDefineList.size();i++)
		{
			switch(m_DataStructDefines.DataStructDefineList[i]->ListType)
			{
			case DATA_STRUCT_CONST:
				{
					CONST_DEFINE_LIST * pConstList=(CONST_DEFINE_LIST *)m_DataStructDefines.DataStructDefineList[i];
					xml_node Consts=GlobalDefine.append_child(node_element,"Consts");
					Consts.append_attribute("Name",pConstList->ListName);
					SaveConstDefine(Consts,pConstList->ConstList);
				}
				break;
			case DATA_STRUCT_ENUM:
				{
					ENUM_DEFINE_LIST * pEnumList=(ENUM_DEFINE_LIST *)m_DataStructDefines.DataStructDefineList[i];
					xml_node Enums=GlobalDefine.append_child(node_element,"Enums");
					Enums.append_attribute("Name",pEnumList->ListName);
					SaveEnumDefine(Enums,pEnumList->EnumList);
				}
				break;
			case DATA_STRUCT_STRUCT:
				{
					STRUCT_DEFINE_LIST * pStructList=(STRUCT_DEFINE_LIST *)m_DataStructDefines.DataStructDefineList[i];
					xml_node Structs=GlobalDefine.append_child(node_element,"Structs");
					Structs.append_attribute("Name",pStructList->ListName);
					SaveStructDefine(Structs,pStructList->StructList);
				}
				break;
			}
		}		

		xml_node DataObjectModifyFlags=GlobalDefine.append_child(node_element,"DataObjectModifyFlags");
		SaveDataObjectModifyFlag(DataObjectModifyFlags,m_DataStructDefines.DataObjectModifyFlags);

		for(size_t i=0;i<m_InterfaceList.size();i++)
		{
			CALLER_INTERFACE& InterfaceInfo=m_InterfaceList[i];
			xml_node Interface=Interfaces.append_child(node_element,"Interface");
			
			Interface.append_attribute("Name",(LPCTSTR)InterfaceInfo.Name);			
			Interface.append_attribute("Type",(long)InterfaceInfo.Type);
			Interface.append_attribute("ID",(long)InterfaceInfo.ID);
			Interface.append_attribute("IDSeed",(long)InterfaceInfo.IDSeed);
			Interface.append_attribute("Description",(LPCTSTR)EncodeString(InterfaceInfo.Description));

			xml_node LocalDefine=Interface.append_child(node_element,"LocalDefine");
			xml_node LocalStructs=LocalDefine.append_child(node_element,"Structs");

			for(size_t j=0;j<InterfaceInfo.MethodList.size();j++)
			{
				INTERFACE_METHOD& MethodInfo=InterfaceInfo.MethodList[j];
				xml_node Method=Interface.append_child(node_element,"Method");
				
				Method.append_attribute("Name",(LPCTSTR)MethodInfo.Name);		
				Method.append_attribute("ID",(long)MethodInfo.ID);
				Method.append_attribute("ParamIDSeed",(long)MethodInfo.ParamIDSeed);
				Method.append_attribute("Description",(LPCTSTR)EncodeString(MethodInfo.Description));
				for(size_t k=0;k<MethodInfo.ParamList.size();k++)
				{
					METHOD_PARAM& ParamInfo=MethodInfo.ParamList[k];

					xml_node Param=Method.append_child(node_element,"Param");
					CString Temp;

					
					Param.append_attribute("Name",(LPCTSTR)ParamInfo.Name);
					Param.append_attribute("Type",(LPCTSTR)EncodeString(ParamInfo.Type));
					Param.append_attribute("ID",(long)ParamInfo.ID);
					Param.append_attribute("Length",(long)ParamInfo.Length);					
					Param.append_attribute("CanNull",ParamInfo.CanNull);
					Param.append_attribute("IsReference",ParamInfo.IsReference);
					Param.append_attribute("DefaultValue",(LPCTSTR)EncodeString(ParamInfo.DefaultValue));					
					Param.append_attribute("Description",(LPCTSTR)EncodeString(ParamInfo.Description));

				}

			}
		}
		xml_node Prefix=Interfaces.append_child(node_element,"Prefix");
		Prefix.append_attribute("Content",(LPCTSTR)EncodeString(m_InterfacePrefix));

		xml_node Postfix=Interfaces.append_child(node_element,"Postfix");
		Postfix.append_attribute("Content",(LPCTSTR)EncodeString(m_InterfacePostFix));

		

		os << Xml.document();
		os.close();
		return true;
	}
	else
	{		
		return false;
	}
}



bool CCallInterfaceMakerDlg::Load(LPCTSTR FileName)
{
	xml_parser Parser;

	if(Parser.parse_file(FileName,pug::parse_trim_attribute))
	{
		

		xml_node Interfaces=Parser.document();
		if(Interfaces.moveto_child("Interfaces"))
		{
			m_InterfaceList.clear();

			m_Name = (LPCTSTR)Interfaces.attribute("Name").getvalue();
			m_Name.Trim();
			m_InterfaceIDSeed=(long)Interfaces.attribute("IDSeed");

			xml_node GlobalDefine=Interfaces;
			if(GlobalDefine.moveto_child("GlobalDefine"))
			{
				for(UINT i=0;i<GlobalDefine.children();i++)
				{
					if(stricmp(GlobalDefine.child(i).get_name(),"Consts")==0)
					{
						CONST_DEFINE_LIST * pConstList=new CONST_DEFINE_LIST;
						pConstList->ListType=DATA_STRUCT_CONST;
						pConstList->ListName = (LPCTSTR)GlobalDefine.child(i).attribute("Name").getvalue();
						LoadConstDefine(GlobalDefine.child(i),pConstList->ConstList);
						m_DataStructDefines.DataStructDefineList.push_back(pConstList);
					}
					else if(stricmp(GlobalDefine.child(i).get_name(),"Enums")==0)
					{
						ENUM_DEFINE_LIST * pEnumList=new ENUM_DEFINE_LIST;
						pEnumList->ListType=DATA_STRUCT_ENUM;
						pEnumList->ListName = (LPCTSTR)GlobalDefine.child(i).attribute("Name").getvalue();
						LoadEnumDefine(GlobalDefine.child(i),pEnumList->EnumList);
						m_DataStructDefines.DataStructDefineList.push_back(pEnumList);
					}
					else if(stricmp(GlobalDefine.child(i).get_name(),"Structs")==0)
					{
						STRUCT_DEFINE_LIST * pStructList=new STRUCT_DEFINE_LIST;
						pStructList->ListType=DATA_STRUCT_STRUCT;
						pStructList->ListName = (LPCTSTR)GlobalDefine.child(i).attribute("Name").getvalue();
						LoadStructDefine(GlobalDefine.child(i),pStructList->StructList);
						m_DataStructDefines.DataStructDefineList.push_back(pStructList);
					}
					else if(stricmp(GlobalDefine.child(i).get_name(),"DataObjectModifyFlags")==0)
					{
						m_DataStructDefines.DataObjectModifyFlags.clear();
						LoadDataObjectModifyFlag(GlobalDefine.child(i),m_DataStructDefines.DataObjectModifyFlags);
					}
				}				
			}

			for(UINT i=0;i<Interfaces.children();i++)
			{
				xml_node Interface=Interfaces.child(i);
				if(_stricmp(Interface.name(),"Interface")==0)
				{
					CALLER_INTERFACE InterfaceInfo;

					InterfaceInfo.Name = (LPCTSTR)Interface.attribute("Name").getvalue();
					InterfaceInfo.Name.Trim();
					InterfaceInfo.Type=Interface.attribute("Type");
					InterfaceInfo.ID=(long)Interface.attribute("ID");					
					InterfaceInfo.IDSeed=(long)Interface.attribute("IDSeed");
					InterfaceInfo.Description = DecodeString((LPCTSTR)Interface.attribute("Description").getvalue());
				


					for(UINT j=0;j<Interface.children();j++)
					{
						xml_node Method=Interface.child(j);
						if(_stricmp(Method.name(),"Method")==0)
						{
							INTERFACE_METHOD MethodInfo;
							MethodInfo.Name = (LPCTSTR)Method.attribute("Name").getvalue();
							MethodInfo.Name.Trim();
							MethodInfo.ID=(long)Method.attribute("ID");
							MethodInfo.ParamIDSeed=(long)Method.attribute("ParamIDSeed");
							MethodInfo.Description = DecodeString((LPCTSTR)Method.attribute("Description").getvalue());
							
							for(UINT k=0;k<Method.children();k++)
							{
								xml_node Param=Method.child(k);
								if(_stricmp(Param.name(),"Param")==0)
								{

									METHOD_PARAM ParamInfo;

									ParamInfo.Name = (LPCTSTR)Param.attribute("Name").getvalue();
									ParamInfo.Name.Trim();

									ParamInfo.Type = DecodeString((LPCTSTR)Param.attribute("Type").getvalue());
									ParamInfo.ID=(long)Param.attribute("ID");
									ParamInfo.Length=(long)Param.attribute("Length");
									ParamInfo.CanNull=Param.attribute("CanNull");
									ParamInfo.IsReference=Param.attribute("IsReference");

									ParamInfo.DefaultValue = DecodeString((LPCTSTR)Param.attribute("DefaultValue").getvalue());
									ParamInfo.DefaultValue.Trim();
									
								
									ParamInfo.Description = DecodeString((LPCTSTR)Param.attribute("Description").getvalue());
									

									MethodInfo.ParamList.push_back(ParamInfo);
								}

							}	
							InterfaceInfo.MethodList.push_back(MethodInfo);
						}
					}
					m_InterfaceList.push_back(InterfaceInfo);
				}
			}
			xml_node Prefix=Interfaces;
			if(Prefix.moveto_child("Prefix"))
			{
				m_InterfacePrefix = DecodeString((LPCTSTR)Prefix.attribute("Content").getvalue());
			}
			xml_node Postfix=Interfaces;
			if(Postfix.moveto_child("Postfix"))
			{
				m_InterfacePostFix = DecodeString((LPCTSTR)Postfix.attribute("Content").getvalue());
			}		

			//for(size_t i=0;i<m_InterfaceList.size();i++)
			//{
			//	m_InterfaceList[i].ID=i+1;
			//	for(size_t j=0;j<m_InterfaceList[i].MethodList.size();j++)
			//	{
			//		m_InterfaceList[i].MethodList[j].ID=j+1;
			//	}
			//	m_InterfaceList[i].IDSeed=m_InterfaceList[i].MethodList.size()+1;
			//}
			//m_InterfaceIDSeed=m_InterfaceList.size()+1;


			FillListItem();
			UpdateData(false);

			LoadEnv();
			return true;
		}
	}
	
	return false;
}

bool CCallInterfaceMakerDlg::SaveStructDefine(xml_node& Structs,vector<STRUCT_DEFINE_INFO>& StructDefineList)
{
	for(size_t i=0;i<StructDefineList.size();i++)
	{
		xml_node Struct=Structs.append_child(node_element,"Struct");
		Struct.append_attribute("Name",(LPCTSTR)StructDefineList[i].Name);
		Struct.append_attribute("ShortName",(LPCTSTR)StructDefineList[i].ShortName);
		Struct.append_attribute("BaseStruct",(LPCTSTR)StructDefineList[i].BaseStruct);
		Struct.append_attribute("Description",(LPCTSTR)EncodeString(StructDefineList[i].Description));
		Struct.append_attribute("IDSeed",(long)StructDefineList[i].IDSeed);		
		Struct.append_attribute("IsDataObject",(bool)StructDefineList[i].IsDataObject);
		Struct.append_attribute("DeclareLater",(bool)StructDefineList[i].DeclareLater);
		Struct.append_attribute("ObjectID",(long)StructDefineList[i].ObjectID);

		xml_node GenerateOperations=Struct.append_child(node_element,"GenerateOperations");
		SaveGenerateOperations(GenerateOperations,StructDefineList[i].GenerateOperations);
		
		for(size_t j=0;j<StructDefineList[i].MemberList.size();j++)
		{
			xml_node Member=Struct.append_child(node_element,"Member");
			Member.append_attribute("Name",(LPCTSTR)StructDefineList[i].MemberList[j].Name);
			Member.append_attribute("Type",(LPCTSTR)EncodeString(StructDefineList[i].MemberList[j].Type));
			Member.append_attribute("Flag",(long)StructDefineList[i].MemberList[j].Flag);
			Member.append_attribute("IsArray",(bool)StructDefineList[i].MemberList[j].IsArray);
			Member.append_attribute("ArrayStartLength",(long)StructDefineList[i].MemberList[j].ArrayStartLength);
			Member.append_attribute("ArrayGrowLength",(long)StructDefineList[i].MemberList[j].ArrayGrowLength);
			Member.append_attribute("Description",(LPCTSTR)EncodeString(StructDefineList[i].MemberList[j].Description));
			Member.append_attribute("ID",(long)StructDefineList[i].MemberList[j].ID);			
		}
	}
	

	return true;
}

bool CCallInterfaceMakerDlg::LoadStructDefine(xml_node& Structs,vector<STRUCT_DEFINE_INFO>& StructDefineList)
{
	for(UINT i=0;i<Structs.children();i++)
	{
		xml_node Struct=Structs.child(i);
		if(_stricmp(Struct.name(),"Struct")==0)
		{
			STRUCT_DEFINE_INFO StructDefine;
			StructDefine.Name = (LPCTSTR)Struct.attribute("Name").getvalue();
			StructDefine.ShortName = (LPCTSTR)Struct.attribute("ShortName").getvalue();
			StructDefine.BaseStruct = (LPCTSTR)Struct.attribute("BaseStruct").getvalue();
			StructDefine.Description = DecodeString((LPCTSTR)Struct.attribute("Description").getvalue());
			StructDefine.IDSeed=(long)Struct.attribute("IDSeed");
			StructDefine.IsDataObject=(bool)Struct.attribute("IsDataObject");
			StructDefine.DeclareLater=(bool)Struct.attribute("DeclareLater");
			StructDefine.ObjectID=(long)Struct.attribute("ObjectID");

			

			for(UINT j=0;j<Struct.children();j++)
			{
				xml_node Member=Struct.child(j);
				if(_stricmp(Member.name(),"Member")==0)
				{
					STRUCT_MEMBER_INFO MemberInfo;
					MemberInfo.Name = (LPCTSTR)Member.attribute("Name").getvalue();
					MemberInfo.Type = DecodeString((LPCTSTR)Member.attribute("Type").getvalue());
					MemberInfo.Flag=(long)Member.attribute("Flag");
					MemberInfo.IsArray=(bool)Member.attribute("IsArray");
					MemberInfo.ArrayStartLength=(long)Member.attribute("ArrayStartLength");
					MemberInfo.ArrayGrowLength=(long)Member.attribute("ArrayGrowLength");
					MemberInfo.Description = DecodeString((LPCTSTR)Member.attribute("Description").getvalue());
					MemberInfo.ID=(long)Member.attribute("ID");
					StructDefine.MemberList.push_back(MemberInfo);
				}
				else if(_stricmp(Member.name(),"GenerateOperations")==0)
				{
					LoadGenerateOperations(Member,StructDefine.GenerateOperations);
				}
			}
			StructDefineList.push_back(StructDefine);
		}		
	}
	return true;
}

bool CCallInterfaceMakerDlg::SaveEnumDefine(xml_node& Enums,vector<ENUM_DEFINE_INFO>& EnumDefineList)
{
	for(size_t i=0;i<EnumDefineList.size();i++)
	{
		xml_node Enum=Enums.append_child(node_element,"Enum");
		Enum.append_attribute("Name",(LPCTSTR)EnumDefineList[i].Name);
		Enum.append_attribute("ShortName",(LPCTSTR)EnumDefineList[i].ShortName);
		Enum.append_attribute("Flag",(long)EnumDefineList[i].Flag);
		Enum.append_attribute("Description",(LPCTSTR)EncodeString(EnumDefineList[i].Description));
				

		for(size_t j=0;j<EnumDefineList[i].MemberList.size();j++)
		{
			xml_node Member=Enum.append_child(node_element,"Member");
			Member.append_attribute("Name",(LPCTSTR)EnumDefineList[i].MemberList[j].Name);
			Member.append_attribute("Value",(LPCTSTR)EncodeString(EnumDefineList[i].MemberList[j].Value));
			Member.append_attribute("StrValue",(LPCTSTR)EncodeString(EnumDefineList[i].MemberList[j].StrValue));
			Member.append_attribute("Description",(LPCTSTR)EncodeString(EnumDefineList[i].MemberList[j].Description));
			
		}
	}


	return true;
}
bool CCallInterfaceMakerDlg::LoadEnumDefine(xml_node& Enums,vector<ENUM_DEFINE_INFO>& EnumDefineList)
{
	for(UINT i=0;i<Enums.children();i++)
	{
		xml_node Enum=Enums.child(i);
		if(_stricmp(Enum.name(),"Enum")==0)
		{
			ENUM_DEFINE_INFO EnumDefine;
			EnumDefine.Name = (LPCTSTR)Enum.attribute("Name").getvalue();
			EnumDefine.ShortName = (LPCTSTR)Enum.attribute("ShortName").getvalue();
			EnumDefine.Flag=(long)Enum.attribute("Flag");
			EnumDefine.Description = DecodeString((LPCTSTR)Enum.attribute("Description").getvalue());
			



			for(UINT j=0;j<Enum.children();j++)
			{
				xml_node Member=Enum.child(j);
				if(_stricmp(Member.name(),"Member")==0)
				{
					ENUM_MEMBER_INFO MemberInfo;
					MemberInfo.Name = (LPCTSTR)Member.attribute("Name").getvalue();
					MemberInfo.Value = DecodeString((LPCTSTR)Member.attribute("Value").getvalue());
					MemberInfo.StrValue = DecodeString((LPCTSTR)Member.attribute("StrValue").getvalue());
					MemberInfo.Description = DecodeString((LPCTSTR)Member.attribute("Description").getvalue());
					
					EnumDefine.MemberList.push_back(MemberInfo);
				}
				
			}
			EnumDefineList.push_back(EnumDefine);
		}		
	}
	return true;
}

bool CCallInterfaceMakerDlg::SaveConstDefine(xml_node& Consts,vector<CONST_DEFINE_INFO>& ConstDefineList)
{
	for(size_t i=0;i<ConstDefineList.size();i++)
	{
		xml_node Const=Consts.append_child(node_element,"Const");
		Const.append_attribute("Name",(LPCTSTR)ConstDefineList[i].Name);
		Const.append_attribute("Type",(LPCTSTR)ConstDefineList[i].Type);
		Const.append_attribute("Value",(LPCTSTR)EncodeString(ConstDefineList[i].Value));
		Const.append_attribute("Description",(LPCTSTR)EncodeString(ConstDefineList[i].Description));
	}


	return true;
}
bool CCallInterfaceMakerDlg::LoadConstDefine(xml_node& Consts,vector<CONST_DEFINE_INFO>& ConstDefineList)
{
	for(UINT i=0;i<Consts.children();i++)
	{
		xml_node Enum=Consts.child(i);
		if(_stricmp(Enum.name(),"Const")==0)
		{
			CONST_DEFINE_INFO ConstDefine;
			ConstDefine.Name = (LPCTSTR)Enum.attribute("Name").getvalue();
			ConstDefine.Type = (LPCTSTR)Enum.attribute("Type").getvalue();
			ConstDefine.Value = DecodeString((LPCTSTR)Enum.attribute("Value").getvalue());
			ConstDefine.Description = DecodeString((LPCTSTR)Enum.attribute("Description").getvalue());

			ConstDefineList.push_back(ConstDefine);
		}		
	}
	return true;
}

bool CCallInterfaceMakerDlg::SaveDataObjectModifyFlag(xml_node& ModifyFlags,vector<DATA_OBJECT_MODIFY_FLAG>& DataObjectModifyFlags)
{
	for(size_t i=0;i<DataObjectModifyFlags.size();i++)
	{
		xml_node ModifyFlag=ModifyFlags.append_child(node_element,"ModifyFlag");
		ModifyFlag.append_attribute("Name",(LPCTSTR)DataObjectModifyFlags[i].Name);
		for(size_t j=0;j<DataObjectModifyFlags[i].ModifyFlag.size();j++)
		{
			xml_node Unit=ModifyFlag.append_child(node_element,"Unit");
			Unit.append_attribute("ClassName",(LPCTSTR)DataObjectModifyFlags[i].ModifyFlag[j].ClassName);
			for(size_t k=0;k<DataObjectModifyFlags[i].ModifyFlag[j].Members.size();k++)
			{
				xml_node Flag=Unit.append_child(node_element,"Flag");
				Flag.append_attribute("Name",(LPCTSTR)DataObjectModifyFlags[i].ModifyFlag[j].Members[k]);
			}
		}
	}
	return true;
}

bool CCallInterfaceMakerDlg::LoadDataObjectModifyFlag(xml_node& ModifyFlags,vector<DATA_OBJECT_MODIFY_FLAG>& DataObjectModifyFlags)
{
	for(UINT i=0;i<ModifyFlags.children();i++)
	{
		xml_node ModifyFlag=ModifyFlags.child(i);
		if(_stricmp(ModifyFlag.name(),"ModifyFlag")==0)
		{
			DATA_OBJECT_MODIFY_FLAG MF;
			MF.Name = (LPCTSTR)ModifyFlag.attribute("Name").getvalue();
			for(UINT j=0;j<ModifyFlag.children();j++)
			{
				xml_node Unit=ModifyFlag.child(j);
				if(_stricmp(Unit.name(),"Unit")==0)
				{
					MODIFY_FLAG_UNIT MFU;
					MFU.ClassName = (LPCTSTR)Unit.attribute("ClassName").getvalue();
					for(UINT k=0;k<Unit.children();k++)
					{
						xml_node Flag=Unit.child(k);
						if(_stricmp(Flag.name(),"Flag")==0)
						{
							MFU.Members.push_back((LPCTSTR)Flag.attribute("Name").getvalue());
						}
					}
					MF.ModifyFlag.push_back(MFU);
				}
			}
			DataObjectModifyFlags.push_back(MF);
		}
	}
	return true;
}

bool CCallInterfaceMakerDlg::SaveGenerateOperations(xml_node& XmlNode,GENERATE_OPERATIONS_INFO& GenerateOperations)
{
	XmlNode.append_attribute("PackOperation",(LPCTSTR)EncodeString(GenerateOperations.PackOperation));
	XmlNode.append_attribute("UnpackOperation",(LPCTSTR)EncodeString(GenerateOperations.UnpackOperation));
	XmlNode.append_attribute("SizeCaculateOperation",(LPCTSTR)EncodeString(GenerateOperations.SizeCaculateOperation));
	XmlNode.append_attribute("ReferenceDefine",(LPCTSTR)EncodeString(GenerateOperations.ReferenceDefine));
	XmlNode.append_attribute("ReferenceUse",(LPCTSTR)EncodeString(GenerateOperations.ReferenceUse));
	XmlNode.append_attribute("VariableDefine",(LPCTSTR)EncodeString(GenerateOperations.VariableDefine));
	XmlNode.append_attribute("InitOperation",(LPCTSTR)EncodeString(GenerateOperations.InitOperation));
	XmlNode.append_attribute("CloneOperation",(LPCTSTR)EncodeString(GenerateOperations.CloneOperation));
	XmlNode.append_attribute("GetMethodDeclare",(LPCTSTR)EncodeString(GenerateOperations.GetMethodDeclare));
	XmlNode.append_attribute("GetMethodDefine",(LPCTSTR)EncodeString(GenerateOperations.GetMethodDefine));
	XmlNode.append_attribute("SetMethodDeclare",(LPCTSTR)EncodeString(GenerateOperations.SetMethodDeclare));
	XmlNode.append_attribute("SetMethodDefine",(LPCTSTR)EncodeString(GenerateOperations.SetMethodDefine));
	XmlNode.append_attribute("IndexOperation",(LPCTSTR)EncodeString(GenerateOperations.IndexOperation));
	XmlNode.append_attribute("ConstIndexOperation",(LPCTSTR)EncodeString(GenerateOperations.ConstIndexOperation));

	return true;
}

bool CCallInterfaceMakerDlg::LoadGenerateOperations(xml_node& XmlNode,GENERATE_OPERATIONS_INFO& GenerateOperations)
{
	GenerateOperations.PackOperation = DecodeString((LPCTSTR)XmlNode.attribute("PackOperation").getvalue());
	GenerateOperations.UnpackOperation = DecodeString((LPCTSTR)XmlNode.attribute("UnpackOperation").getvalue());
	GenerateOperations.SizeCaculateOperation = DecodeString((LPCTSTR)XmlNode.attribute("SizeCaculateOperation").getvalue());
	GenerateOperations.ReferenceDefine = DecodeString((LPCTSTR)XmlNode.attribute("ReferenceDefine").getvalue());
	GenerateOperations.ReferenceUse = DecodeString((LPCTSTR)XmlNode.attribute("ReferenceUse").getvalue());
	GenerateOperations.VariableDefine = DecodeString((LPCTSTR)XmlNode.attribute("VariableDefine").getvalue());
	GenerateOperations.InitOperation = DecodeString((LPCTSTR)XmlNode.attribute("InitOperation").getvalue());
	GenerateOperations.CloneOperation = DecodeString((LPCTSTR)XmlNode.attribute("CloneOperation").getvalue());
	GenerateOperations.GetMethodDeclare = DecodeString((LPCTSTR)XmlNode.attribute("GetMethodDeclare").getvalue());
	GenerateOperations.GetMethodDefine = DecodeString((LPCTSTR)XmlNode.attribute("GetMethodDefine").getvalue());
	GenerateOperations.SetMethodDeclare = DecodeString((LPCTSTR)XmlNode.attribute("SetMethodDeclare").getvalue());
	GenerateOperations.SetMethodDefine = DecodeString((LPCTSTR)XmlNode.attribute("SetMethodDefine").getvalue());
	GenerateOperations.IndexOperation = DecodeString((LPCTSTR)XmlNode.attribute("IndexOperation").getvalue());
	GenerateOperations.ConstIndexOperation = DecodeString((LPCTSTR)XmlNode.attribute("ConstIndexOperation").getvalue());

	return true;
}

CString CCallInterfaceMakerDlg::EncodeString(CString Str)
{
	Str.Replace("&","&amp;");
	Str.Replace("<","&lt;");
	Str.Replace(">","&gt;");
	Str.Replace("\"","&quot;");
	Str.Replace("\r","&rt;");
	Str.Replace("\n","&nl;");
	Str.Replace("	","&tab;");
	return Str;
}
CString CCallInterfaceMakerDlg::DecodeString(CString Str)
{
	Str.Replace("&amp;","&");
	Str.Replace("&lt;","<");
	Str.Replace("&gt;",">");
	Str.Replace("&quot;","\"");
	Str.Replace("&rt;","\r");
	Str.Replace("&nl;","\n");
	Str.Replace("&tab;","	");
	return Str;
}

bool CCallInterfaceMakerDlg::ExportDataStructDefineHeader(LPCTSTR ExportDir,LPCTSTR FileName)
{
	CFile OutputFile;
	CString OutputFileName;

	OutputFileName=ExportDir;
	OutputFileName+="\\";
	OutputFileName+=FileName;

	if(OutputFile.Open(OutputFileName,CFile::modeCreate|CFile::modeWrite|CFile::shareDenyNone))
	{
		CString Source=m_InterfaceHeaderTemplate;

		CString Includes;
		CString Temp;			

		Temp.Format("#include \"%sSystemEnums.%s\"\r\n",m_Name,m_InterfaceConfig.EnumExportConfig.ExportExt);

		Includes+=Temp;

		for(size_t i=0;i<m_DataStructDefines.DataStructDefineList.size();i++)
		{
			switch(m_DataStructDefines.DataStructDefineList[i]->ListType)
			{
			case DATA_STRUCT_CONST:
				{
					CONST_DEFINE_LIST * pConstList=(CONST_DEFINE_LIST *)m_DataStructDefines.DataStructDefineList[i];

					Temp.Format("#include \"%s%sConsts.%s\"\r\n",m_Name,pConstList->ListName,m_InterfaceConfig.ConstExportConfig.ExportExt);

					Includes+=Temp;
				}
				break;
			case DATA_STRUCT_ENUM:
				{
					ENUM_DEFINE_LIST * pEnumList=(ENUM_DEFINE_LIST *)m_DataStructDefines.DataStructDefineList[i];

					Temp.Format("#include \"%s%sEnums.%s\"\r\n",m_Name,pEnumList->ListName,m_InterfaceConfig.EnumExportConfig.ExportExt);
					Includes+=Temp;
				}
				break;
			case DATA_STRUCT_STRUCT:
				{
					STRUCT_DEFINE_LIST * pStructList=(STRUCT_DEFINE_LIST *)m_DataStructDefines.DataStructDefineList[i];

					Temp.Format("#include \"%s%sStructs.%s\"\r\n",m_Name,pStructList->ListName,m_InterfaceConfig.StructExportConfig.ExportExt);
					Includes+=Temp;

					for(size_t j=0;j<pStructList->StructList.size();j++)
					{
						if(pStructList->StructList[j].IsDataObject)
						{
							if(m_InterfaceConfig.DataObjectHeaderExportConfig.IsExport)
							{
								Temp.Format("#include \"%s.%s\"\r\n",ClassNameToFileName(pStructList->StructList[j].Name),
									m_InterfaceConfig.DataObjectHeaderExportConfig.ExportExt);
								Includes+=Temp;
							}
						}

					}
				}		
				break;
			}	
			
		}		

		if(m_InterfaceConfig.DataObjectModifyFlagsExportConfig.IsExport)
		{
			Temp.Format("#include \"DataObjectModifyFlags.%s\"\r\n",m_InterfaceConfig.DataObjectModifyFlagsExportConfig.ExportExt);
			Includes+=Temp;
		}

		Source.Replace("<Prefix>","");
		Source.Replace("<Postfix>","");

		Source.Replace("<Includes>",Includes);

		OutputFile.Write((LPCTSTR)Source,Source.GetLength());
		OutputFile.Close();
	}
	else
	{
		CString Msg;
		Msg.Format("无法打开文件%s",OutputFileName);
		MessageBox(Msg);
	}	
	return false;
}

bool CCallInterfaceMakerDlg::ExportInterfaceHeader(LPCTSTR ExportDir,LPCTSTR FileName)
{
	CFile OutputFile;
	CString OutputFileName;

	OutputFileName=ExportDir;
	OutputFileName+="\\";
	OutputFileName+=FileName;

	if(OutputFile.Open(OutputFileName,CFile::modeCreate|CFile::modeWrite|CFile::shareDenyNone))
	{
		CString ProtocolFile=m_InterfaceHeaderTemplate;
		
		ProtocolFile.Replace("<Prefix>",m_InterfacePrefix);
		ProtocolFile.Replace("<Postfix>",m_InterfacePostFix);
		
		CString Includes;
		CString Temp;	
		
		for(size_t i=0;i<m_InterfaceList.size();i++)
		{
			CALLER_INTERFACE& InterfaceInfo=m_InterfaceList[i];	
			
			Temp.Format("#include \"%sInterface.%s\"\r\n",InterfaceInfo.Name,
				m_InterfaceConfig.InterfaceExportConfig.ExportExt);
			Includes+=Temp;
			Temp.Format("#include \"%sMsgCaller.%s\"\r\n",InterfaceInfo.Name,
				m_InterfaceConfig.CallHeaderExportConfig.ExportExt);
			Includes+=Temp;
			Temp.Format("#include \"%sMsgHandler.%s\"\r\n",InterfaceInfo.Name,
				m_InterfaceConfig.HandlerHeaderExportConfig.ExportExt);
			Includes+=Temp;

			Includes+="\r\n";
		}		
		
		ProtocolFile.Replace("<Includes>",Includes);

		OutputFile.Write((LPCTSTR)ProtocolFile,ProtocolFile.GetLength());
		OutputFile.Close();
	}
	else
	{
		CString Msg;
		Msg.Format("无法打开文件%s",OutputFileName);
		MessageBox(Msg);
	}	
	return false;
}

bool CCallInterfaceMakerDlg::ExportStructs(LPCTSTR ExportDir,LPCTSTR FileName,vector<STRUCT_DEFINE_INFO>& StructList)
{
	CFile OutputFile;
	CString OutputFileName;

	OutputFileName=ExportDir;
	OutputFileName+="\\";
	OutputFileName+=FileName;

	if(OutputFile.Open(OutputFileName,CFile::modeCreate|CFile::modeWrite|CFile::shareDenyNone))
	{
		CString DefineHeader=m_StructDefineHeaderTemplate;

		DefineHeader.Replace("<SpaceName>",m_Name);
	

		CString LineSpace=GetLineSpace(DefineHeader,"<Structs>");
		CString Structs=MakeStructDefines(StructList,LineSpace);
		DefineHeader.Replace("<Structs>",Structs);

		OutputFile.Write((LPCTSTR)DefineHeader,DefineHeader.GetLength());
		OutputFile.Close();
	}
	else
	{
		CString Msg;
		Msg.Format("无法打开文件%s",OutputFileName);
		MessageBox(Msg);
	}	
	return false;
}

bool CCallInterfaceMakerDlg::ExportSystemEnums(LPCTSTR ExportDir,LPCTSTR FileName)
{
	vector<ENUM_DEFINE_INFO> SystemEnumList;

	ENUM_DEFINE_INFO EnumInfo;

	EnumInfo.Name="GET_MODIFY_FLAG_TYPE";
	EnumInfo.MemberList.resize(3);
	EnumInfo.MemberList[0].Name="GMFT_COPY";
	EnumInfo.MemberList[1].Name="GMFT_AND";
	EnumInfo.MemberList[2].Name="GMFT_OR";
	SystemEnumList.push_back(EnumInfo);	
	
	EnumInfo.MemberList.clear();
	MakeDataObjectFlagIndicesEnum(EnumInfo);
	SystemEnumList.push_back(EnumInfo);

	EnumInfo.MemberList.clear();
	MakeDataObjectSSTIDsEnum(EnumInfo);
	SystemEnumList.push_back(EnumInfo);

	EnumInfo.MemberList.clear();
	EnumInfo.Name.Format("%s_INTERFACES",ClassNameToUpper(m_Name));

	EnumInfo.MemberList.resize(m_InterfaceList.size());
	for(size_t i=0;i<m_InterfaceList.size();i++)
	{
		EnumInfo.MemberList[i].Name.Format("%s_INTERFACE_%s",ClassNameToUpper(m_Name),ClassNameToUpper(m_InterfaceList[i].Name));
		EnumInfo.MemberList[i].Value.Format("%d",m_InterfaceList[i].ID);
	}

	SystemEnumList.push_back(EnumInfo);

	return ExportEnums(ExportDir,FileName,SystemEnumList,"System",true);
}

bool CCallInterfaceMakerDlg::ExportEnums(LPCTSTR ExportDir,LPCTSTR FileName,vector<ENUM_DEFINE_INFO>& EnumList,LPCTSTR ListName,bool IsSystem)
{
	CFile OutputFile;
	CString OutputFileName;

	OutputFileName=ExportDir;
	OutputFileName+="\\";
	OutputFileName+=FileName;

	if(OutputFile.Open(OutputFileName,CFile::modeCreate|CFile::modeWrite|CFile::shareDenyNone))
	{
		CString EnumDefineHeader;

		if(IsSystem)
			EnumDefineHeader=m_SystemEnumDefineHeaderTemplate;
		else
			EnumDefineHeader=m_EnumDefineHeaderTemplate;

		CString SpaceNameUp=ClassNameToUpper(m_Name);
		CString ListNameUp=ClassNameToUpper(ListName);

		EnumDefineHeader.Replace("<SpaceName>",m_Name);

		EnumDefineHeader.Replace("<!SpaceName>",SpaceNameUp);
		EnumDefineHeader.Replace("<!ListName>",ListNameUp);
	

		CString LineSpace=GetLineSpace(EnumDefineHeader,"<Enums>");
		CString Enums=MakeEnumDefines(EnumList,LineSpace);
		EnumDefineHeader.Replace("<Enums>",Enums);

		CString EnumStrValuesName;
		EnumStrValuesName.Format("%s_ENUM_STR_VALUES",ListNameUp);
		EnumDefineHeader.Replace("<EnumStrValuesName>",EnumStrValuesName);

		LineSpace=GetLineSpace(EnumDefineHeader,"<EnumStrValues>");
		CString EnumStrValues=MakeEnumStrValues(EnumList,LineSpace);
		EnumDefineHeader.Replace("<EnumStrValues>",EnumStrValues);

		OutputFile.Write((LPCTSTR)EnumDefineHeader,EnumDefineHeader.GetLength());
		OutputFile.Close();
	}
	else
	{
		CString Msg;
		Msg.Format("无法打开文件%s",OutputFileName);
		MessageBox(Msg);
	}	
	return false;
}

bool CCallInterfaceMakerDlg::ExportConsts(LPCTSTR ExportDir,LPCTSTR FileName,vector<CONST_DEFINE_INFO>& ConstList,LPCTSTR ListName)
{
	CFile OutputFile;
	CString OutputFileName;

	OutputFileName=ExportDir;
	OutputFileName+="\\";
	OutputFileName+=FileName;

	if(OutputFile.Open(OutputFileName,CFile::modeCreate|CFile::modeWrite|CFile::shareDenyNone))
	{
		CString ConstDefineHeader=m_ConstDefineHeaderTemplate;

		CString SpaceNameUp=ClassNameToUpper(m_Name);
		CString ListNameUp=ClassNameToUpper(ListName);

		ConstDefineHeader.Replace("<SpaceName>",m_Name);

		ConstDefineHeader.Replace("<!SpaceName>",SpaceNameUp);
		ConstDefineHeader.Replace("<!ListName>",ListNameUp);		

		CString LineSpace=GetLineSpace(ConstDefineHeader,"<Consts>");
		CString Consts=MakeConstDefines(ConstList,LineSpace);
		ConstDefineHeader.Replace("<Consts>",Consts);

		OutputFile.Write((LPCTSTR)ConstDefineHeader,ConstDefineHeader.GetLength());
		OutputFile.Close();
	}
	else
	{
		CString Msg;
		Msg.Format("无法打开文件%s",OutputFileName);
		MessageBox(Msg);
	}	
	return false;
}

bool CCallInterfaceMakerDlg::ExportDataStructDefines(LPCTSTR ExportDir)
{
	CString Temp;

	Temp.Format("%sSystemEnums.%s",m_Name,m_InterfaceConfig.EnumExportConfig.ExportExt);
	ExportSystemEnums(ExportDir,Temp);

	ExportDataObjectModifyFlags(ExportDir);

	for(size_t i=0;i<m_DataStructDefines.DataStructDefineList.size();i++)
	{
		switch(m_DataStructDefines.DataStructDefineList[i]->ListType)
		{
		case DATA_STRUCT_CONST:
			{
				CONST_DEFINE_LIST * pConstList=(CONST_DEFINE_LIST *)m_DataStructDefines.DataStructDefineList[i];

				Temp.Format("%s%sConsts.%s",m_Name,pConstList->ListName,m_InterfaceConfig.ConstExportConfig.ExportExt);

				ExportConsts(ExportDir,Temp,pConstList->ConstList,pConstList->ListName);
			}
			break;
		case DATA_STRUCT_ENUM:
			{
				ENUM_DEFINE_LIST * pEnumList=(ENUM_DEFINE_LIST *)m_DataStructDefines.DataStructDefineList[i];

				Temp.Format("%s%sEnums.%s",m_Name,pEnumList->ListName,m_InterfaceConfig.EnumExportConfig.ExportExt);

				ExportEnums(ExportDir,Temp,pEnumList->EnumList,pEnumList->ListName,false);
			}
			break;
		case DATA_STRUCT_STRUCT:
			{
				STRUCT_DEFINE_LIST * pStructList=(STRUCT_DEFINE_LIST *)m_DataStructDefines.DataStructDefineList[i];

				Temp.Format("%s%sStructs.%s",m_Name,pStructList->ListName,m_InterfaceConfig.StructExportConfig.ExportExt);

				ExportStructs(ExportDir,Temp,pStructList->StructList);
			}		
			break;
		}		
	}

	

	return true;
}

bool CCallInterfaceMakerDlg::ExportInterfaces(LPCTSTR ExportDir)
{
	for(size_t i=0;i<m_InterfaceList.size();i++)
	{
		if(IsListItemChecked(i))
		{
			CALLER_INTERFACE& InterfaceInfo=m_InterfaceList[i];


			if(m_InterfaceConfig.InterfaceExportConfig.IsExport)
			{
				CFile OutputFile;
				CString OutputFileName;
				OutputFileName.Format("%s\\%sInterface.%s",ExportDir,InterfaceInfo.Name,
					m_InterfaceConfig.InterfaceExportConfig.ExportExt);
				if(OutputFile.Open(OutputFileName,CFile::modeCreate|CFile::modeWrite|CFile::shareDenyNone))
				{			
					CString Interface=m_InterfaceTemplate;

					ENUM_DEFINE_INFO EnumInfo;

					EnumInfo.Name.Format("%s_METHODS",ClassNameToUpper(InterfaceInfo.Name));

					EnumInfo.MemberList.resize(InterfaceInfo.MethodList.size());
					for(size_t j=0;j<InterfaceInfo.MethodList.size();j++)
					{
						EnumInfo.MemberList[j].Name.Format("METHOD_%s",ClassNameToUpper(InterfaceInfo.MethodList[j].Name));
						EnumInfo.MemberList[j].Value.Format("%d",InterfaceInfo.MethodList[j].ID);
					}

					CString LineSpace=GetLineSpace(Interface,"<MethodIDEnums>");
					CString MethodIDEnums=MakeEnumDefine(EnumInfo,LineSpace);
					Interface.Replace("<MethodIDEnums>",MethodIDEnums);

					Interface.Replace("<SpaceName>",m_Name);
					Interface.Replace("<InterfaceName>",InterfaceInfo.Name);
					Interface.Replace("<InterfaceDescription>",InterfaceInfo.Description);
					LineSpace=GetLineSpace(Interface,"<Methods>");
					CString Methods=MakeMethods(InterfaceInfo.MethodList,LineSpace,true);			
					Interface.Replace("<Methods>",Methods);
					LineSpace=GetLineSpace(Interface,"<MethodSSTIDEnumDefine>");
					CString SSTIDDefines=MakeMethodSSTIDEnumDefines(InterfaceInfo,LineSpace);
					Interface.Replace("<MethodSSTIDEnumDefine>",SSTIDDefines);

					OutputFile.Write((LPCTSTR)Interface,Interface.GetLength());
					OutputFile.Close();
				}
				else
				{
					CString Msg;
					Msg.Format("无法打开文件%s",OutputFileName);
					MessageBox(Msg);
					return false;
				}	
			}
			ExportMsgCaller(ExportDir,InterfaceInfo);
			ExportMsgHandler(ExportDir,InterfaceInfo);
		}
	}
	return true;
}

bool CCallInterfaceMakerDlg::ExportMsgCaller(LPCTSTR ExportDir,CALLER_INTERFACE& InterfaceInfo)
{
	CFile HeaderFile,SourceFile;

	CString HeaderFileName,SourceFileName;


	if(m_InterfaceConfig.CallHeaderExportConfig.IsExport)
	{
		HeaderFileName.Format("%s\\%sMsgCaller.%s",ExportDir,InterfaceInfo.Name,
			m_InterfaceConfig.CallHeaderExportConfig.ExportExt);
		

		if(HeaderFile.Open(HeaderFileName,CFile::modeCreate|CFile::modeWrite|CFile::shareDenyNone))
		{
			CString Header=m_MsgCallerHeaderTemplate;

			Header.Replace("<SpaceName>",m_Name);
			Header.Replace("<InterfaceName>",InterfaceInfo.Name);
			CString LineSpace=GetLineSpace(Header,"<Methods>");
			CString Methods=MakeMethods(InterfaceInfo.MethodList,LineSpace,false);
			Header.Replace("<Methods>",Methods);

			LineSpace=GetLineSpace(Header,"<PackMethods>");
			Methods=MakeMsgPackMethods(InterfaceInfo.MethodList,LineSpace);		
			Header.Replace("<PackMethods>",Methods);

			HeaderFile.Write((LPCTSTR)Header,Header.GetLength());
			HeaderFile.Close();
		}
		else
		{
			CString Msg;
			Msg.Format("无法打开文件%s",HeaderFileName);
			MessageBox(Msg);
		}
	}

	if(m_InterfaceConfig.CallSourceExportConfig.IsExport)
	{
		SourceFileName.Format("%s\\%sMsgCaller.%s",ExportDir,InterfaceInfo.Name,
			m_InterfaceConfig.CallSourceExportConfig.ExportExt);
		if(SourceFile.Open(SourceFileName,CFile::modeCreate|CFile::modeWrite|CFile::shareDenyNone))
		{
			CString Source=m_MsgCallerSourceTemplate;

			Source.Replace("<SpaceName>",m_Name);
			Source.Replace("<InterfaceName>",InterfaceInfo.Name);

			CString LineSpace=GetLineSpace(Source,"<Methods>");
			CString Methods=MakeMethodsSource(InterfaceInfo,LineSpace);
			Source.Replace("<Methods>",Methods);	

			LineSpace=GetLineSpace(Source,"<PackMethods>");
			Methods=MakeMsgPackMethodsSource(InterfaceInfo,LineSpace);
			Source.Replace("<PackMethods>",Methods);


			SourceFile.Write((LPCTSTR)Source,Source.GetLength());
			SourceFile.Close();
		}
		else
		{
			CString Msg;
			Msg.Format("无法打开文件%s",SourceFileName);
			MessageBox(Msg);
		}
	}
	return true;
}

bool CCallInterfaceMakerDlg::ExportMsgHandler(LPCTSTR ExportDir,CALLER_INTERFACE& InterfaceInfo)
{
	CFile HeaderFile,SourceFile;

	CString HeaderFileName,SourceFileName;

	
	

	if(m_InterfaceConfig.HandlerHeaderExportConfig.IsExport)
	{
		HeaderFileName.Format("%s\\%sMsgHandler.%s",ExportDir,InterfaceInfo.Name,
			m_InterfaceConfig.HandlerHeaderExportConfig.ExportExt);
		if(HeaderFile.Open(HeaderFileName,CFile::modeCreate|CFile::modeWrite|CFile::shareDenyNone))
		{
			CString Header=m_MsgHandlerHeaderTemplate;

			Header.Replace("<SpaceName>",m_Name);
			Header.Replace("<InterfaceName>",InterfaceInfo.Name);
			CString LineSpace=GetLineSpace(Header,"<Methods>");
			CString Methods=MakeMsgHandlerMethods(InterfaceInfo,LineSpace);
			Header.Replace("<Methods>",Methods);
			HeaderFile.Write((LPCTSTR)Header,Header.GetLength());
			HeaderFile.Close();
		}
		else
		{
			CString Msg;
			Msg.Format("无法打开文件%s",HeaderFileName);
			MessageBox(Msg);
		}
	}

	if(m_InterfaceConfig.HandlerSourceExportConfig.IsExport)
	{
		SourceFileName.Format("%s\\%sMsgHandler.%s",ExportDir,InterfaceInfo.Name,
			m_InterfaceConfig.HandlerSourceExportConfig.ExportExt);
		if(SourceFile.Open(SourceFileName,CFile::modeCreate|CFile::modeWrite|CFile::shareDenyNone))
		{
			CString Source=m_MsgHandlerSourceTemplate;

			Source.Replace("<SpaceName>",m_Name);
			Source.Replace("<InterfaceName>",InterfaceInfo.Name);

			CString MethodCount;
			MethodCount.Format("%d",InterfaceInfo.MethodList.size());
			Source.Replace("<MethodCount>",MethodCount);

			CString LineSpace=GetLineSpace(Source,"<MsgMapInits>");
			CString MsgMapInits=MakeMsgMapInits(InterfaceInfo,LineSpace);	
			Source.Replace("<MsgMapInits>",MsgMapInits);	

			LineSpace=GetLineSpace(Source,"<Methods>");
			CString Methods=MakeMsgHandlerMethodsSource(InterfaceInfo,LineSpace);	
			Source.Replace("<Methods>",Methods);	


			SourceFile.Write((LPCTSTR)Source,Source.GetLength());
			SourceFile.Close();
		}
		else
		{
			CString Msg;
			Msg.Format("无法打开文件%s",SourceFileName);
			MessageBox(Msg);
		}
	}
	return true;
}

bool CCallInterfaceMakerDlg::ExportDataObject(LPCTSTR ExportDir)
{
	CFile HeaderFile,SourceFile;

	CString HeaderFileName,SourceFileName;

	if(m_InterfaceConfig.DataObjectHeaderExportConfig.IsExport)
	{
		for(size_t i=0;i<m_DataStructDefines.DataStructDefineList.size();i++)
		{
			if(m_DataStructDefines.DataStructDefineList[i]->ListType==DATA_STRUCT_STRUCT)
			{
				STRUCT_DEFINE_LIST * pStructList=(STRUCT_DEFINE_LIST *)m_DataStructDefines.DataStructDefineList[i];
				for(size_t j=0;j<pStructList->StructList.size();j++)
				{
					if(pStructList->StructList[j].IsDataObject)
					{
						HeaderFileName.Format("%s\\%s.%s",ExportDir,ClassNameToFileName(pStructList->StructList[j].Name),
							m_InterfaceConfig.DataObjectHeaderExportConfig.ExportExt);

						CString Header=RestoreFileToTemplate(HeaderFileName,m_DataObjectDefineHeaderTemple);

						if(HeaderFile.Open(HeaderFileName,CFile::modeCreate|CFile::modeWrite|CFile::shareDenyNone))
						{
							

							CString LineSpace=GetLineSpace(m_MsgHandlerHeaderTemplate,"<Description>");
							CString Description=ToComment(pStructList->StructList[j].Description,LineSpace);
							Header.Replace("<Description>",Description);

							
							Header.Replace("<ClassName>",pStructList->StructList[j].Name);
							if(pStructList->StructList[j].BaseStruct.IsEmpty())
							{
								RemoveBlock(Header,"<IfHaveBaseClass>","</IfHaveBaseClass>");
								Header.Replace("<IfNotHaveBaseClass>","");
								Header.Replace("</IfNotHaveBaseClass>","");
								Header.Replace("<BaseClass>","");

							}
							else
							{
								Header.Replace("<IfHaveBaseClass>","");
								Header.Replace("</IfHaveBaseClass>","");
								RemoveBlock(Header,"<IfNotHaveBaseClass>","</IfNotHaveBaseClass>");
								Header.Replace("<BaseClass>",pStructList->StructList[j].BaseStruct);
							}
							

							LineSpace=GetLineSpace(Header,"<Members>");
							CString Members=MakeStructMembers(pStructList->StructList[j],LineSpace);
							Header.Replace("<Members>",Members);
							
							LineSpace=GetLineSpace(Header,"<SSTIDEnumDefine>");
							CString SSTIDEnum=MakeStructSSTIDEnumDefines(pStructList->StructList[j],LineSpace);
							Header.Replace("<SSTIDEnumDefine>",SSTIDEnum);

							LineSpace=GetLineSpace(Header,"<ModifyFlagEnumDefine>");
							CString ModifyFlagEnumDefine=MakeDataObjectModifyFlagEnumDefine(pStructList->StructList[j],LineSpace);
							Header.Replace("<ModifyFlagEnumDefine>",ModifyFlagEnumDefine);

							LineSpace=GetLineSpace(Header,"<SetMethodsDeclare>");
							CString SetMethodsDeclare=MakeDataObjectSetMethodsDeclare(pStructList->StructList[j],LineSpace);
							Header.Replace("<SetMethodsDeclare>",SetMethodsDeclare);

							LineSpace=GetLineSpace(Header,"<GetMethodsDeclare>");
							CString GetMethodsDeclare=MakeDataObjectGetMethodsDeclare(pStructList->StructList[j],LineSpace);
							Header.Replace("<GetMethodsDeclare>",GetMethodsDeclare);


							LineSpace=GetLineSpace(Header,"<SetMethodsDefine>");
							CString SetMethodsDefine=MakeDataObjectSetMethodsDefine(pStructList->StructList[j],LineSpace);
							Header.Replace("<SetMethodsDefine>",SetMethodsDefine);

							LineSpace=GetLineSpace(Header,"<GetMethodsDefine>");
							CString GetMethodsDefine=MakeDataObjectGetMethodsDefine(pStructList->StructList[j],LineSpace);
							Header.Replace("<GetMethodsDefine>",GetMethodsDefine);
							

							HeaderFile.Write((LPCTSTR)Header,Header.GetLength());
							HeaderFile.Close();
						}
						else
						{
							CString Msg;
							Msg.Format("无法打开文件%s",HeaderFileName);
							MessageBox(Msg);
						}
					}
				}
			}
		}
		
	}

	if(m_InterfaceConfig.DataObjectSourceExportConfig.IsExport)
	{
		for(size_t i=0;i<m_DataStructDefines.DataStructDefineList.size();i++)
		{
			if(m_DataStructDefines.DataStructDefineList[i]->ListType==DATA_STRUCT_STRUCT)
			{
				STRUCT_DEFINE_LIST * pStructList=(STRUCT_DEFINE_LIST *)m_DataStructDefines.DataStructDefineList[i];
				for(size_t j=0;j<pStructList->StructList.size();j++)
				{
					if(pStructList->StructList[j].IsDataObject)
					{
						SourceFileName.Format("%s\\%s.%s",ExportDir,ClassNameToFileName(pStructList->StructList[j].Name),
							m_InterfaceConfig.DataObjectSourceExportConfig.ExportExt);
						CString Source=RestoreFileToTemplate(SourceFileName,m_DataObjectDefineSourceTemple);
						if(SourceFile.Open(SourceFileName,CFile::modeCreate|CFile::modeWrite|CFile::shareDenyNone))
						{
							CString LineSpace=GetLineSpace(m_MsgHandlerHeaderTemplate,"<Description>");
							CString Description=ToComment(pStructList->StructList[j].Description,LineSpace);
							Source.Replace("<Description>",Description);

							Source.Replace("<SpaceName>",m_Name);

							if(pStructList->StructList[j].MemberList.size())
							{
								Source.Replace("<IfHaveMember>","");
								Source.Replace("</IfHaveMember>","");
							}
							else
							{
								RemoveBlock(Source,"<IfHaveMember>","</IfHaveMember>");
							}

							if(pStructList->StructList[j].BaseStruct.IsEmpty())
							{
								RemoveBlock(Source,"<IfHaveBaseClass>","</IfHaveBaseClass>");
								Source.Replace("<IfNotHaveBaseClass>","");
								Source.Replace("</IfNotHaveBaseClass>","");
								Source.Replace("<BaseClass>","");
								
							}
							else
							{
								Source.Replace("<IfHaveBaseClass>","");
								Source.Replace("</IfHaveBaseClass>","");
								RemoveBlock(Source,"<IfNotHaveBaseClass>","</IfNotHaveBaseClass>");
								Source.Replace("<BaseClass>",pStructList->StructList[j].BaseStruct);
								Source.Replace("<BaseClass>",pStructList->StructList[j].BaseStruct);
							}

							Source.Replace("<ClassName>",pStructList->StructList[j].Name);

							CString FullModifyFlag;
							FullModifyFlag.Format("DOMF_%s_FULL",ClassNameToUpper(pStructList->StructList[j].Name));
							Source.Replace("<FULL_MODIFY_FLAG>",FullModifyFlag);
							

							LineSpace=GetLineSpace(Source,"<Members>");
							CString Members=MakeStructMembers(pStructList->StructList[j],LineSpace);
							Source.Replace("<Members>",Members);
							
							LineSpace=GetLineSpace(Source,"<SSTIDEnumDefine>");
							CString SSTIDEnum=MakeStructSSTIDEnumDefines(pStructList->StructList[j],LineSpace);
							Source.Replace("<SSTIDEnumDefine>",SSTIDEnum);

							LineSpace=GetLineSpace(Source,"<ModifyFlagEnumDefine>");
							CString ModifyFlagEnumDefine=MakeDataObjectModifyFlagEnumDefine(pStructList->StructList[j],LineSpace);
							Source.Replace("<ModifyFlagEnumDefine>",ModifyFlagEnumDefine);

							LineSpace=GetLineSpace(Source,"<DataObjectMembersGetModifyFlag>");
							CString DataObjectMembersGetModifyFlag=MakeDataObjectMembersGetModifyFlag(pStructList->StructList[j],LineSpace);
							Source.Replace("<DataObjectMembersGetModifyFlag>",DataObjectMembersGetModifyFlag);

							LineSpace=GetLineSpace(Source,"<DataObjectMembersIsModified>");
							CString DataObjectMembersIsModified=MakeDataDataObjectMembersIsModified(pStructList->StructList[j],LineSpace);
							Source.Replace("<DataObjectMembersIsModified>",DataObjectMembersIsModified);

							LineSpace=GetLineSpace(Source,"<DataObjectMembersClearModifyFlag>");
							CString DataObjectMembersClearModifyFlag=MakeDataDataObjectMembersClearModifyFlag(pStructList->StructList[j],LineSpace);
							Source.Replace("<DataObjectMembersClearModifyFlag>",DataObjectMembersClearModifyFlag);

							LineSpace=GetLineSpace(Source,"<SetMethodsDefine>");
							CString SetMethodsDefine=MakeDataObjectSetMethodsDefine(pStructList->StructList[j],LineSpace);
							Source.Replace("<SetMethodsDefine>",SetMethodsDefine);

							LineSpace=GetLineSpace(Source,"<GetMethodsDefine>");
							CString GetMethodsDefine=MakeDataObjectGetMethodsDefine(pStructList->StructList[j],LineSpace);
							Source.Replace("<GetMethodsDefine>",GetMethodsDefine);

							Source.Replace("<CLASS_FLAG_INDEX>","DATA_OBJECT_FLAG_"+ClassNameToUpper(pStructList->StructList[j].Name));


							LineSpace=GetLineSpace(Source,"<InitOperations>");
							CString InitOperations=MakeInitOperations(pStructList->StructList[j],LineSpace);
							Source.Replace("<InitOperations>",InitOperations);


							CString SSTIDEnumName;
							SSTIDEnumName.Format("%s_MEMBER_IDS",ClassNameToUpper(pStructList->StructList[j].Name));


							LineSpace=GetLineSpace(Source,"<PackOperations>");
							CString PackOperations=MakePackOperations(pStructList->StructList[j],SSTIDEnumName,LineSpace);
							Source.Replace("<PackOperations>",PackOperations);

							LineSpace=GetLineSpace(Source,"<UnpackOperations>");
							CString UnpackOperations=MakeUnpackOperations(pStructList->StructList[j],SSTIDEnumName,LineSpace);
							Source.Replace("<UnpackOperations>",UnpackOperations);

							LineSpace=GetLineSpace(Source,"<PacketSizes>");
							CString PackSizes=MakePackSizes(pStructList->StructList[j],LineSpace);
							Source.Replace("<PacketSizes>",PackSizes);

							LineSpace=GetLineSpace(Source,"<CloneOperations>");
							CString CloneOperations=MakeCloneOperations(pStructList->StructList[j],LineSpace,"","DataObject.");
							Source.Replace("<CloneOperations>",CloneOperations);

							SourceFile.Write((LPCTSTR)Source,Source.GetLength());
							SourceFile.Close();
						}
						else
						{
							CString Msg;
							Msg.Format("无法打开文件%s",SourceFileName);
							MessageBox(Msg);
						}
					}	
				}		
			}
		}
	}
	return true;
}

bool CCallInterfaceMakerDlg::ExportDataObjectModifyFlags(LPCTSTR ExportDir)
{
	CFile HeaderFile;

	CString HeaderFileName;


	if(m_InterfaceConfig.DataObjectModifyFlagsExportConfig.IsExport)
	{
		HeaderFileName.Format("%s\\DataObjectModifyFlags.%s",ExportDir,
			m_InterfaceConfig.DataObjectModifyFlagsExportConfig.ExportExt);

		CString Header=m_DataObjectModifyFlagsTemple;

		if(HeaderFile.Open(HeaderFileName,CFile::modeCreate|CFile::modeWrite|CFile::shareDenyNone))
		{
			Header.Replace("<SpaceName>",m_Name);

			CString LineSpace=GetLineSpace(Header,"<DataObjectModifyFlags>");
			CString DataObjectModifyFlags=MakeDataObjectModifyFlags(LineSpace);
			Header.Replace("<DataObjectModifyFlags>",DataObjectModifyFlags);
			

			HeaderFile.Write((LPCTSTR)Header,Header.GetLength());
			HeaderFile.Close();
		}
		else
		{
			CString Msg;
			Msg.Format("无法打开文件%s",HeaderFileName);
			MessageBox(Msg);
		}
		
	}
	return true;
}

CString CCallInterfaceMakerDlg::MakeMethods(vector<INTERFACE_METHOD> MethodList,LPCTSTR szLineSpace,bool IsPure)
{
	CString Methods;
	for(size_t j=0;j<MethodList.size();j++)
	{
		INTERFACE_METHOD& MethodInfo=MethodList[j];
		CString Method=m_MethodHeaderTemple;
		Method.Replace("<MethodName>",MethodInfo.Name);
		Method.Replace("<MethodDescription>",MethodInfo.Description);
		if(IsPure)
			Method.Replace("<IsPure>",m_MethodHeaderPureTailTemple);
		else
			Method.Replace("<IsPure>",";");
		CString Params=MakeParams(MethodInfo.ParamList,true,true);
		CString LineSpace=GetLineSpace(Method,"<ParamDescriptions>");
		CString ParamDescriptions=MakeParamDescriptions(MethodInfo.ParamList,LineSpace);
		

		Method.Replace("<Params>",Params);
		Method.Replace("<ParamDescriptions>",ParamDescriptions);
		Methods+=Method+"\r\n";
	}
	CString Space="\r\n";
	Space+=szLineSpace;
	Methods.Replace("\r\n",Space);
	return Methods;
}

CString CCallInterfaceMakerDlg::MakeMethodSSTIDEnumDefines(CALLER_INTERFACE& InterfaceInfo,LPCTSTR szLineSpace)
{
	CString MethodIDEnumDefines;

	vector<INTERFACE_METHOD>& MethodList=InterfaceInfo.MethodList;
	for(size_t i=0;i<MethodList.size();i++)
	{
		INTERFACE_METHOD& MethodInfo=MethodList[i];

		ENUM_DEFINE_INFO EnumInfo;

		EnumInfo.Name.Format("%s_%s_MEMBER_IDS",ClassNameToUpper(InterfaceInfo.Name),ClassNameToUpper(MethodInfo.Name));
		EnumInfo.ShortName=EnumInfo.Name;

		EnumInfo.MemberList.resize(MethodInfo.ParamList.size());

		CString MethodName;
		MethodName=ClassNameToUpper(MethodInfo.Name);	

		for(size_t j=0;j<MethodInfo.ParamList.size();j++)
		{
			CString ParamName;
			ParamName=ClassNameToUpper(MethodInfo.ParamList[j].Name);
			EnumInfo.MemberList[j].Name.Format("SST_%s_%s",MethodName,ParamName);
			EnumInfo.MemberList[j].Value.Format("%d",MethodInfo.ParamList[j].ID);
		}		

		MethodIDEnumDefines+=MakeEnumDefine(EnumInfo,szLineSpace);
		
	}
	
	return MethodIDEnumDefines;
}

CString CCallInterfaceMakerDlg::MakeStructSSTIDEnumDefines(STRUCT_DEFINE_INFO& StructInfo,LPCTSTR szLineSpace)
{
	ENUM_DEFINE_INFO EnumInfo;

	EnumInfo.Name.Format("%s_MEMBER_IDS",ClassNameToUpper(StructInfo.Name));
	EnumInfo.ShortName=EnumInfo.Name;

	EnumInfo.MemberList.resize(StructInfo.MemberList.size());

	CString StructName;
	StructName=StructInfo.ShortName;	

	for(size_t j=0;j<StructInfo.MemberList.size();j++)
	{
		CString MemberName;
		MemberName=ClassNameToUpper(StructInfo.MemberList[j].Name);
		EnumInfo.MemberList[j].Name.Format("SST_%s_%s",StructName,MemberName);
		EnumInfo.MemberList[j].Value.Format("%d",StructInfo.ObjectID+StructInfo.MemberList[j].ID);
	}

	return MakeEnumDefine(EnumInfo,szLineSpace);


	
}

CString CCallInterfaceMakerDlg::MakeMsgPackMethods(vector<INTERFACE_METHOD>& MethodList,LPCTSTR szLineSpace)
{
	CString Methods;
	for(size_t j=0;j<MethodList.size();j++)
	{
		INTERFACE_METHOD& MethodInfo=MethodList[j];
		CString Method=m_MsgPackMethodHeaderTemple;
		Method.Replace("<MethodName>",MethodInfo.Name);		
		CString Params=MakeParams(MethodInfo.ParamList,true,true);
		if(!Params.IsEmpty())
			Params=","+Params;
		Method.Replace("<,Params>",Params);
		Methods+=Method+"\r\n";
	}
	CString Space="\r\n";
	Space+=szLineSpace;
	Methods.Replace("\r\n",Space);
	return Methods;
}

CString CCallInterfaceMakerDlg::MakeParams(vector<METHOD_PARAM>& ParamList,bool IsDefine,bool IncludeDefaultValue)
{
	CString Params;
	for(size_t k=0;k<ParamList.size();k++)
	{
		METHOD_PARAM& ParamInfo=ParamList[k];

		TYPE_DEFINE * pTypeInfo=FindVarType(ParamInfo.Type);
		if(pTypeInfo)
		{		
			CString Param=m_ParamTemple;
			
			if(IsDefine)
			{
				Param.Replace("<ParamName>",ParamInfo.Name);
				if(ParamInfo.IsReference)
				{
					CString ReferenceDefine=pTypeInfo->GenerateOperations.ReferenceDefine;
					ReferenceDefine.Replace("<Type>",pTypeInfo->CType);
					Param.Replace("<ParamType>",ReferenceDefine);
				}
				else
				{
					Param.Replace("<ParamType>",pTypeInfo->CType);
				}

				
				if(ParamInfo.DefaultValue.IsEmpty()||(!IncludeDefaultValue))
				{
					Param.Replace("<ParamDefaultValue>","");
				}
				else
				{
					CString DefauleValue;
					DefauleValue.Format("= %s",ParamInfo.DefaultValue);
					Param.Replace("<ParamDefaultValue>",DefauleValue);
				}
			}
			else
			{
				if(ParamInfo.IsReference)
				{
					CString ReferenceUse=pTypeInfo->GenerateOperations.ReferenceUse;
					ReferenceUse.Replace("<Variable>",ParamInfo.Name);
					Param.Replace("<ParamName>",ReferenceUse);
				}
				else
				{
					Param.Replace("<ParamName>",ParamInfo.Name);
				}
				
				Param.Replace("<ParamType>","");
				Param.Replace("<ParamLenght>","");
				Param.Replace("<ParamDefaultValue>","");
			}
			if(k)
			{
				Params+=",";							
			}
			Params+=Param;
		}
	}
	return Params;
}

CString CCallInterfaceMakerDlg::MakeParamDescriptions(vector<METHOD_PARAM>& ParamList,LPCTSTR szLineSpace)
{
	CString ParamDescriptions;
	for(size_t k=0;k<ParamList.size();k++)
	{
		METHOD_PARAM& ParamInfo=ParamList[k];

		TYPE_DEFINE * pTypeInfo=FindVarType(ParamInfo.Type);
		if(pTypeInfo)
		{			

			CString ParamDescription=m_ParamDescriptionTemple;

			ParamDescription.Replace("<ParamName>",ParamInfo.Name);
			ParamDescription.Replace("<ParamDescript>",ParamInfo.Description);

			ParamDescriptions+=ParamDescription;
			if(k<ParamList.size()-1)
			{
				ParamDescriptions+="\r\n";
			}
		}
	}
	CString Space="\r\n";
	Space+=szLineSpace;
	ParamDescriptions.Replace("\r\n",Space);
	return ParamDescriptions;
}

CString CCallInterfaceMakerDlg::MakeParamDefines(vector<METHOD_PARAM>& ParamList,LPCTSTR szLineSpace)
{
	CString ParamDefines;

	int MaxLen=0;
	for(size_t i=0;i<ParamList.size();i++)
	{
		TYPE_DEFINE * pTypeInfo=FindVarType(ParamList[i].Type);
		if(pTypeInfo)
		{
			if(pTypeInfo->CType.GetLength()>MaxLen)
				MaxLen=pTypeInfo->CType.GetLength();
		}
	}
	int SpaceLen=(((int)ceil(MaxLen/4.0f))+1)*4;

	for(size_t k=0;k<ParamList.size();k++)
	{
		METHOD_PARAM& ParamInfo=ParamList[k];

		TYPE_DEFINE * pTypeInfo=FindVarType(ParamInfo.Type);
		if(pTypeInfo)
		{		
			CString Param=pTypeInfo->GenerateOperations.VariableDefine;
			Param.Replace("<Variable>",ParamInfo.Name);			
			Param.Replace("<Type>",pTypeInfo->CType);	
			CString Space;
			int CurSpaceCount=(int)ceil((SpaceLen-pTypeInfo->CType.GetLength())/4.0f);
			while(CurSpaceCount)
			{
				Space+="	";
				CurSpaceCount--;
			}
			Param.Replace("<Space>",Space);
			
			ParamDefines+=Param;
			ParamDefines+="\r\n";
		}
	}
	CString Space="\r\n";
	Space+=szLineSpace;
	ParamDefines.Replace("\r\n",Space);
	return ParamDefines;
}

CString CCallInterfaceMakerDlg::MakeMethodsSource(CALLER_INTERFACE& InterfaceInfo,LPCTSTR szLineSpace)
{
	CString Methods;
	for(size_t j=0;j<InterfaceInfo.MethodList.size();j++)
	{
		INTERFACE_METHOD& MethodInfo=InterfaceInfo.MethodList[j];
		CString Method=m_MethodCallerSourceTemple;
		CString SpaceNameUp=ClassNameToUpper(m_Name);		
		CString InterfaceNameUp=ClassNameToUpper(InterfaceInfo.Name);		
		CString MethodNameUp=ClassNameToUpper(MethodInfo.Name);		
		Method.Replace("<InterfaceName>",InterfaceInfo.Name);
		Method.Replace("<MethodName>",MethodInfo.Name);	
		Method.Replace("<!SpaceName>",SpaceNameUp);
		Method.Replace("<!InterfaceName>",InterfaceNameUp);
		Method.Replace("<!MethodName>",MethodNameUp);
		CString Params=MakeParams(MethodInfo.ParamList,true,true);
		Method.Replace("<Params>",Params);	
		Params=MakeParams(MethodInfo.ParamList,false,true);
		if(!Params.IsEmpty())
			Params=","+Params;
		Method.Replace("<,Params>",Params);
		CString LineSpace=GetLineSpace(Method,"<PacketSizes>");
		CString PackSizes=MakePackSizes(MethodInfo,LineSpace);
		Method.Replace("<PacketSizes>",PackSizes);
		CString InterfaceID;
		InterfaceID.Format("%s_INTERFACE_%s",ClassNameToUpper(m_Name),ClassNameToUpper(InterfaceInfo.Name));
		Method.Replace("<InterfaceID>",InterfaceID);
		CString MethodID;
		MethodID.Format("METHOD_%s",ClassNameToUpper(MethodInfo.Name));
		Method.Replace("<MethodID>",MethodID);
		Method.Replace("<IsAckMsg>",InterfaceInfo.Type==INTERFACE_CALL_BACK?"true":"false");



		Methods+=Method+"\r\n";
	}
	CString Space="\r\n";
	Space+=szLineSpace;
	Methods.Replace("\r\n",Space);
	return Methods;
}

CString CCallInterfaceMakerDlg::MakeMsgPackMethodsSource(CALLER_INTERFACE& InterfaceInfo,LPCTSTR szLineSpace)
{
	CString Methods;
	for(size_t j=0;j<InterfaceInfo.MethodList.size();j++)
	{
		INTERFACE_METHOD& MethodInfo=InterfaceInfo.MethodList[j];
		CString Method=m_MsgPackMethodSourceTemple;
		CString SpaceNameUp=ClassNameToUpper(m_Name);		
		CString InterfaceNameUp=ClassNameToUpper(InterfaceInfo.Name);		
		CString MethodNameUp=ClassNameToUpper(MethodInfo.Name);		
		Method.Replace("<InterfaceName>",InterfaceInfo.Name);
		Method.Replace("<MethodName>",MethodInfo.Name);
		Method.Replace("<!SpaceName>",SpaceNameUp);
		Method.Replace("<!InterfaceName>",InterfaceNameUp);
		Method.Replace("<!MethodName>",MethodNameUp);
		CString Params=MakeParams(MethodInfo.ParamList,true,true);
		if(!Params.IsEmpty())
			Params=","+Params;
		Method.Replace("<,Params>",Params);
		CString LineSpace=GetLineSpace(Method,"<PackOperations>");
		CString SSTIDEnumName;
		SSTIDEnumName.Format("%s_%s_MEMBER_IDS",ClassNameToUpper(InterfaceInfo.Name),ClassNameToUpper(MethodInfo.Name));
		CString PackOperations=MakePackOperations(MethodInfo,SSTIDEnumName,LineSpace);
		Method.Replace("<PackOperations>",PackOperations);

		Methods+=Method+"\r\n";
	}
	CString Space="\r\n";
	Space+=szLineSpace;
	Methods.Replace("\r\n",Space);
	return Methods;
}

CString CCallInterfaceMakerDlg::MakeInitOperations(INTERFACE_METHOD& MethodInfo,LPCTSTR szLineSpace)
{
	CString InitOperations;

	for(size_t i=0;i<MethodInfo.ParamList.size();i++)
	{
		
		TYPE_DEFINE * pTypeInfo=FindVarType(MethodInfo.ParamList[i].Type);
		if(pTypeInfo)
		{
			if(!pTypeInfo->GenerateOperations.InitOperation.IsEmpty())
			{
				CString Operation=pTypeInfo->GenerateOperations.InitOperation;				
				Operation.Replace("<Variable>",MethodInfo.ParamList[i].Name);

				InitOperations+=Operation;
				InitOperations+="\r\n";
			}
		}		
	}
	CString Space="\r\n";
	Space+=szLineSpace;
	InitOperations.Replace("\r\n",Space);
	return InitOperations;
}

CString CCallInterfaceMakerDlg::MakePackOperations(INTERFACE_METHOD& MethodInfo,CString SSTIDEnumName,LPCTSTR szLineSpace)
{
	CString PackOperations;

	for(size_t i=0;i<MethodInfo.ParamList.size();i++)
	{
		
		TYPE_DEFINE * pTypeInfo=FindVarType(MethodInfo.ParamList[i].Type);
		if(pTypeInfo)
		{
			CString Operation=m_PackOperationUnitTemple;	

			
			RemoveBlock(Operation,"<IfInDataObject>","</IfInDataObject>");
			Operation.Replace("<IfNotInDataObject>","");
			Operation.Replace("</IfNotInDataObject>","");
			RemoveBlock(Operation,"<IfNeedCheckModifyFlag>","</IfNeedCheckModifyFlag>");
			
			
			Operation.Replace("<Variable>",MethodInfo.ParamList[i].Name);			

			CString LineSpace=GetLineSpace(Operation,"<PackOperation>");
			CString PackOP=MakePackOperation(MethodInfo.ParamList[i].Type,
				MethodInfo.ParamList[i].Name,
				MethodInfo.Name,ClassNameToUpper(MethodInfo.Name),
				SSTIDEnumName,
				false,false,false,
				LineSpace);			

			Operation.Replace("<PackOperation>",PackOP);

			PackOperations+=Operation;
			PackOperations+="\r\n";
		}		
	}
	CString Space="\r\n";
	Space+=szLineSpace;
	PackOperations.Replace("\r\n",Space);
	return PackOperations;
}

CString CCallInterfaceMakerDlg::MakeUnpackOperations(INTERFACE_METHOD& MethodInfo,CString SSTIDEnumName,LPCTSTR szLineSpace)
{
	CString PackOperations;

	for(size_t i=0;i<MethodInfo.ParamList.size();i++)
	{

		TYPE_DEFINE * pTypeInfo=FindVarType(MethodInfo.ParamList[i].Type);
		if(pTypeInfo)
		{
			CString Operation=m_UnpackOperationUnitTemple;
						
			RemoveBlock(Operation,"<IfInDataObject>","</IfInDataObject>");
			Operation.Replace("<IfNotInDataObject>","");
			Operation.Replace("</IfNotInDataObject>","");
			RemoveBlock(Operation,"<IfNeedCheckModifyFlag>","</IfNeedCheckModifyFlag>");

			CString SST_ID;
			
			SST_ID.Format("SST_%s_%s",ClassNameToUpper(MethodInfo.Name),ClassNameToUpper(MethodInfo.ParamList[i].Name));
			SST_ID.MakeUpper();
			
			
			Operation.Replace("<SST_NAME>",SSTIDEnumName);
			Operation.Replace("<SST_ID>",SST_ID);

			Operation.Replace("<Variable>",MethodInfo.ParamList[i].Name);

			CString LineSpace=GetLineSpace(Operation,"<PackOperation>");
			CString UnpackOP=MakeUnpackOperation(MethodInfo.ParamList[i].Type,
				MethodInfo.ParamList[i].Name,
				MethodInfo.Name,ClassNameToUpper(MethodInfo.Name),
				SSTIDEnumName,
				false,false,false,
				LineSpace);

			Operation.Replace("<UnpackOperation>",UnpackOP);

			PackOperations+=Operation;
			PackOperations+="\r\n";
		}		
	}
	CString Space="\r\n";
	Space+=szLineSpace;
	PackOperations.Replace("\r\n",Space);
	return PackOperations;
}

CString CCallInterfaceMakerDlg::MakePackSizes(INTERFACE_METHOD& MethodInfo,LPCTSTR szLineSpace)
{
	CString PackSizes;

	for(size_t i=0;i<MethodInfo.ParamList.size();i++)
	{

		TYPE_DEFINE * pTypeInfo=FindVarType(MethodInfo.ParamList[i].Type);
		if(pTypeInfo)
		{
			CString Operation=pTypeInfo->GenerateOperations.SizeCaculateOperation;		

			RemoveBlock(Operation,"<IfDataObject>","</IfDataObject>");
			Operation.Replace("<IfNotDataObject>","");
			Operation.Replace("</IfNotDataObject>","");

			RemoveBlock(Operation,"<IfInDataObject>","</IfInDataObject>");
			Operation.Replace("<IfNotInDataObject>","");
			Operation.Replace("</IfNotInDataObject>","");

			Operation.Replace("<Variable>",MethodInfo.ParamList[i].Name);
			Operation.Replace("<Type>",pTypeInfo->CType);
			
			PackSizes+="+";
			PackSizes+=Operation;
			if(i<MethodInfo.ParamList.size()-1)
				PackSizes+="\r\n";
		}		
	}
	CString Space="\r\n";
	Space+=szLineSpace;
	PackSizes.Replace("\r\n",Space);
	return PackSizes;
}

CString CCallInterfaceMakerDlg::MakeInitOperations(STRUCT_DEFINE_INFO& StructInfo,LPCTSTR szLineSpace)
{
	CString InitOperations;

	for(size_t i=0;i<StructInfo.MemberList.size();i++)
	{
		
		TYPE_DEFINE * pTypeInfo=FindVarType(StructInfo.MemberList[i].Type);
		if(pTypeInfo)
		{
			if(!pTypeInfo->GenerateOperations.InitOperation.IsEmpty())
			{
				CString Operation=pTypeInfo->GenerateOperations.InitOperation;
				if(StructInfo.MemberList[i].IsArray)
				{
					Operation=m_InterfaceConfig.ArrayDefineConfig.InitOperation;
				}
				if(StructInfo.IsDataObject)
					Operation.Replace("<Variable>","m_"+StructInfo.MemberList[i].Name);
				else
					Operation.Replace("<Variable>",StructInfo.MemberList[i].Name);

				CString Temp;
				Temp.Format("%u",StructInfo.MemberList[i].ArrayStartLength);
				Operation.Replace("<ArrayStartLength>",Temp);
				Temp.Format("%u",StructInfo.MemberList[i].ArrayGrowLength);
				Operation.Replace("<ArrayGrowLength>",Temp);

				InitOperations+=Operation;
				InitOperations+="\r\n";
			}
		}		
	}
	CString Space="\r\n";
	Space+=szLineSpace;
	InitOperations.Replace("\r\n",Space);
	return InitOperations;
}
CString CCallInterfaceMakerDlg::MakePackOperations(STRUCT_DEFINE_INFO& StructInfo,CString SSTIDEnumName,LPCTSTR szLineSpace)
{
	CString PackOperations;

	for(size_t i=0;i<StructInfo.MemberList.size();i++)
	{
		if(StructInfo.MemberList[i].Flag&STRUCT_MEMBER_FLAG_EXCLUDE_IN_PACKET)
			continue;
		
		CString Operation=m_PackOperationUnitTemple;
		if(StructInfo.MemberList[i].IsArray)
		{
			Operation=m_InterfaceConfig.ArrayDefineConfig.PackOperation;
		}

		if(StructInfo.IsDataObject)
		{
			RemoveBlock(Operation,"<IfNotInDataObject>","</IfNotInDataObject>");
			Operation.Replace("<IfInDataObject>","");
			Operation.Replace("</IfInDataObject>","");
			if(StructInfo.MemberList[i].Flag&STRUCT_MEMBER_FLAG_MUST_PACK)
			{
				RemoveBlock(Operation,"<IfNeedCheckModifyFlag>","</IfNeedCheckModifyFlag>");
			}
			else
			{
				Operation.Replace("<IfNeedCheckModifyFlag>","");
				Operation.Replace("</IfNeedCheckModifyFlag>","");
			}
		}
		else
		{
			RemoveBlock(Operation,"<IfInDataObject>","</IfInDataObject>");
			Operation.Replace("<IfNotInDataObject>","");
			Operation.Replace("</IfNotInDataObject>","");
			RemoveBlock(Operation,"<IfNeedCheckModifyFlag>","</IfNeedCheckModifyFlag>");
		}

		if(StructInfo.IsDataObject)
			Operation.Replace("<Variable>","m_"+StructInfo.MemberList[i].Name);
		else
			Operation.Replace("<Variable>",StructInfo.MemberList[i].Name);

		CString ModifyFlagEnumName;
		ModifyFlagEnumName.Format("%s_MODIFY_FLAGS",ClassNameToUpper(StructInfo.Name));			
		Operation.Replace("<ModifyFlagEnumName>",ModifyFlagEnumName);

		CString ModifyFlag="MF_"+ClassNameToUpper(StructInfo.MemberList[i].Name);
		Operation.Replace("<ModifyFlag>",ModifyFlag);

		CString LineSpace=GetLineSpace(Operation,"<PackOperation>");
		CString PackOP=MakePackOperation(StructInfo.MemberList[i].Type,
			StructInfo.MemberList[i].Name,
			StructInfo.Name,StructInfo.ShortName,
			SSTIDEnumName,
			StructInfo.IsDataObject,StructInfo.IsDataObject&&((StructInfo.MemberList[i].Flag&STRUCT_MEMBER_FLAG_NOT_MONITOR_UPDATE)==0),
			StructInfo.MemberList[i].IsArray,
			LineSpace);

		Operation.Replace("<PackOperation>",PackOP);

		CString SST_ID;

		SST_ID.Format("SST_%s_%s",StructInfo.ShortName,ClassNameToUpper(StructInfo.MemberList[i].Name));
		SST_ID.MakeUpper();


		Operation.Replace("<SST_NAME>",SSTIDEnumName);
		Operation.Replace("<SST_ID>",SST_ID);


		PackOperations+=Operation;
		PackOperations+="\r\n";
	}
	CString Space="\r\n";
	Space+=szLineSpace;
	PackOperations.Replace("\r\n",Space);
	return PackOperations;
}
CString CCallInterfaceMakerDlg::MakeUnpackOperations(STRUCT_DEFINE_INFO& StructInfo,CString SSTIDEnumName,LPCTSTR szLineSpace)
{
	CString PackOperations;

	for(size_t i=0;i<StructInfo.MemberList.size();i++)
	{

		if(StructInfo.MemberList[i].Flag&STRUCT_MEMBER_FLAG_EXCLUDE_IN_PACKET)
			continue;

		TYPE_DEFINE * pTypeInfo=FindVarType(StructInfo.MemberList[i].Type);
		if(pTypeInfo)
		{
			CString Operation=m_UnpackOperationUnitTemple;
			if(StructInfo.MemberList[i].IsArray)
			{
				Operation=m_InterfaceConfig.ArrayDefineConfig.UnpackOperation;

				TYPE_DEFINE * pTypeInfo=FindVarType(StructInfo.MemberList[i].Type);
				CString Space;
				if(pTypeInfo)
				{
					CString VariableDefine=pTypeInfo->GenerateOperations.VariableDefine;
					VariableDefine.Replace("<Type>",pTypeInfo->CType);					
					VariableDefine.Replace("<Space>","	");					
					VariableDefine.Replace("<Variable>","ArrayElement");
					Operation.Replace("<ArrayElementVariableDefine>",VariableDefine);
				}
			}

			if(StructInfo.IsDataObject)
			{
				RemoveBlock(Operation,"<IfNotInDataObject>","</IfNotInDataObject>");
				Operation.Replace("<IfInDataObject>","");
				Operation.Replace("</IfInDataObject>","");
				if(StructInfo.MemberList[i].Flag&STRUCT_MEMBER_FLAG_MUST_PACK)
				{
					RemoveBlock(Operation,"<IfNeedCheckModifyFlag>","</IfNeedCheckModifyFlag>");
				}
				else
				{
					Operation.Replace("<IfNeedCheckModifyFlag>","");
					Operation.Replace("</IfNeedCheckModifyFlag>","");
				}
			}
			else
			{
				RemoveBlock(Operation,"<IfInDataObject>","</IfInDataObject>");
				Operation.Replace("<IfNotInDataObject>","");
				Operation.Replace("</IfNotInDataObject>","");
				RemoveBlock(Operation,"<IfNeedCheckModifyFlag>","</IfNeedCheckModifyFlag>");
			}

			CString SST_ID;
			
			SST_ID.Format("SST_%s_%s",StructInfo.ShortName,ClassNameToUpper(StructInfo.MemberList[i].Name));
			SST_ID.MakeUpper();
			

			Operation.Replace("<SST_NAME>",SSTIDEnumName);
			Operation.Replace("<SST_ID>",SST_ID);

			if(StructInfo.IsDataObject)
				Operation.Replace("<Variable>","m_"+StructInfo.MemberList[i].Name);
			else
				Operation.Replace("<Variable>",StructInfo.MemberList[i].Name);

			CString ModifyFlagEnumName;
			ModifyFlagEnumName.Format("%s_MODIFY_FLAGS",ClassNameToUpper(StructInfo.Name));			
			Operation.Replace("<ModifyFlagEnumName>",ModifyFlagEnumName);

			CString ModifyFlag="MF_"+ClassNameToUpper(StructInfo.MemberList[i].Name);
			Operation.Replace("<ModifyFlag>",ModifyFlag);

			CString LineSpace=GetLineSpace(Operation,"<UnpackOperation>");
			CString UnpackOP=MakeUnpackOperation(StructInfo.MemberList[i].Type,
				StructInfo.MemberList[i].Name,
				StructInfo.Name,StructInfo.ShortName,
				SSTIDEnumName,
				StructInfo.IsDataObject,StructInfo.IsDataObject&&((StructInfo.MemberList[i].Flag&STRUCT_MEMBER_FLAG_NOT_MONITOR_UPDATE)==0),
				StructInfo.MemberList[i].IsArray,
				LineSpace);

			Operation.Replace("<UnpackOperation>",UnpackOP);

			PackOperations+=Operation;
			PackOperations+="\r\n";
		}		
	}
	CString Space="\r\n";
	Space+=szLineSpace;
	PackOperations.Replace("\r\n",Space);
	return PackOperations;
}

CString CCallInterfaceMakerDlg::MakePackSizes(STRUCT_DEFINE_INFO& StructInfo,LPCTSTR szLineSpace)
{
	CString PackSizes;

	for(size_t i=0;i<StructInfo.MemberList.size();i++)
	{
		if(StructInfo.MemberList[i].Flag&STRUCT_MEMBER_FLAG_EXCLUDE_IN_PACKET)
			continue;

		TYPE_DEFINE * pTypeInfo=FindVarType(StructInfo.MemberList[i].Type);
		if(pTypeInfo)
		{
			CString PackSize=m_PackSizeTemple;
			CString Operation=pTypeInfo->GenerateOperations.SizeCaculateOperation;		
			CString Variable;

			if(StructInfo.IsDataObject)
			{
				RemoveBlock(Operation,"<IfNotInDataObject>","</IfNotInDataObject>");
				Operation.Replace("<IfInDataObject>","");
				Operation.Replace("</IfInDataObject>","");

				Variable="m_"+StructInfo.MemberList[i].Name;
				
			}
			else
			{
				RemoveBlock(Operation,"<IfInDataObject>","</IfInDataObject>");
				Operation.Replace("<IfNotInDataObject>","");
				Operation.Replace("</IfNotInDataObject>","");

				Variable=StructInfo.MemberList[i].Name;
			}

			if(pTypeInfo->Flag&TYPE_DEFINE_FLAG_DATA_OBJECT)
			{
				RemoveBlock(Operation,"<IfNotDataObject>","</IfNotDataObject>");
				Operation.Replace("<IfDataObject>","");
				Operation.Replace("</IfDataObject>","");
			}
			else
			{
				RemoveBlock(Operation,"<IfDataObject>","</IfDataObject>");
				Operation.Replace("<IfNotDataObject>","");
				Operation.Replace("</IfNotDataObject>","");
			}

			

			if(StructInfo.MemberList[i].IsArray)
			{
				CString Var=m_InterfaceConfig.ArrayDefineConfig.ConstIndexOperation;
				Var.Replace("<Variable>",Variable);
				Var.Replace("<Index>","i");
				Operation.Replace("<Variable>",Var);
				Operation.Replace("<Type>",pTypeInfo->CType);
				CString ArrayOP=m_InterfaceConfig.ArrayDefineConfig.SizeCaculateOperation;
				ArrayOP.Replace("<Variable>",Variable);
				ArrayOP.Replace("<ArrayElementSize>",Operation);
				PackSize=ArrayOP;
			}
			else
			{
				Operation.Replace("<Variable>",Variable);
				Operation.Replace("<Type>",pTypeInfo->CType);
				PackSize.Replace("<PackSize>",Operation);
			}

			PackSizes+=PackSize;
			PackSizes+="\r\n";
		}		
	}
	CString Space="\r\n";
	Space+=szLineSpace;
	PackSizes.Replace("\r\n",Space);
	return PackSizes;
}

CString CCallInterfaceMakerDlg::MakePackOperation(CString Type,CString Name,CString ParentName,CString ParentShortName,CString SSTIDEnumName,bool IsInDataObject,bool IsMFCheck,bool IsArray,LPCTSTR szLineSpace)
{
	CString Operation;

	TYPE_DEFINE * pTypeInfo=FindVarType(Type);
	if(pTypeInfo)
	{
		Operation=pTypeInfo->GenerateOperations.PackOperation;

		if(IsMFCheck)
		{
			RemoveBlock(Operation,"<IfNotMFCheck>","</IfNotMFCheck>");
			Operation.Replace("<IfMFCheck>","");
			Operation.Replace("</IfMFCheck>","");
		}
		else
		{
			RemoveBlock(Operation,"<IfMFCheck>","</IfMFCheck>");
			Operation.Replace("<IfNotMFCheck>","");
			Operation.Replace("</IfNotMFCheck>","");
		}

		if(IsInDataObject)
		{
			RemoveBlock(Operation,"<IfNotInDataObject>","</IfNotInDataObject>");
			Operation.Replace("<IfInDataObject>","");
			Operation.Replace("</IfInDataObject>","");
		}
		else
		{
			RemoveBlock(Operation,"<IfInDataObject>","</IfInDataObject>");
			Operation.Replace("<IfNotInDataObject>","");
			Operation.Replace("</IfNotInDataObject>","");
		}

		if(pTypeInfo->Flag&TYPE_DEFINE_FLAG_DATA_OBJECT)
		{
			RemoveBlock(Operation,"<IfNotDataObject>","</IfNotDataObject>");
			Operation.Replace("<IfDataObject>","");
			Operation.Replace("</IfDataObject>","");
		}
		else
		{
			RemoveBlock(Operation,"<IfDataObject>","</IfDataObject>");
			Operation.Replace("<IfNotDataObject>","");
			Operation.Replace("</IfNotDataObject>","");
		}

		CString SST_ID;
		
		
		SST_ID.Format("SST_%s_%s",ParentShortName,ClassNameToUpper(Name));
		SST_ID.MakeUpper();
		
		
		
		//CString VarName=Name;
		if(IsArray)
		{
			CString ArrayVar=m_InterfaceConfig.ArrayDefineConfig.ConstIndexOperation;
			ArrayVar.Replace("<Index>","i");
			Operation.Replace("<Variable>",ArrayVar);
		}
		
		if(IsInDataObject)
			Operation.Replace("<Variable>","m_"+Name);
		else
			Operation.Replace("<Variable>",Name);

		Operation.Replace("<Type>",pTypeInfo->CType);
		

		Operation.Replace("<SST_NAME>",SSTIDEnumName);
		Operation.Replace("<SST_ID>",SST_ID);

		CString ModifyFlagEnumName;
		ModifyFlagEnumName.Format("%s_MODIFY_FLAGS",ClassNameToUpper(ParentName));			
		Operation.Replace("<ModifyFlagEnumName>",ModifyFlagEnumName);

		CString ModifyFlag="MF_"+ClassNameToUpper(Name);
		Operation.Replace("<ModifyFlag>",ModifyFlag);
	}

	CString Space="\r\n";
	Space+=szLineSpace;
	Operation.Replace("\r\n",Space);
	return Operation;
}

CString CCallInterfaceMakerDlg::MakeUnpackOperation(CString Type,CString Name,CString ParentName,CString ParentShortName,CString SSTIDEnumName,bool IsInDataObject,bool IsMonitorUpdate,bool IsArray,LPCTSTR szLineSpace)
{
	CString Operation;

	TYPE_DEFINE * pTypeInfo=FindVarType(Type);
	if(pTypeInfo)
	{
		Operation=pTypeInfo->GenerateOperations.UnpackOperation;

		if(IsMonitorUpdate)
		{
			Operation.Replace("<IfMonitorUpdate>","");
			Operation.Replace("</IfMonitorUpdate>","");
		}
		else
		{
			RemoveBlock(Operation,"<IfMonitorUpdate>","</IfMonitorUpdate>");
		}

		if(IsInDataObject)
		{
			RemoveBlock(Operation,"<IfNotInDataObject>","</IfNotInDataObject>");
			Operation.Replace("<IfInDataObject>","");
			Operation.Replace("</IfInDataObject>","");
		}
		else
		{
			RemoveBlock(Operation,"<IfInDataObject>","</IfInDataObject>");
			Operation.Replace("<IfNotInDataObject>","");
			Operation.Replace("</IfNotInDataObject>","");
		}

		if(pTypeInfo->Flag&TYPE_DEFINE_FLAG_DATA_OBJECT)
		{
			RemoveBlock(Operation,"<IfNotDataObject>","</IfNotDataObject>");
			Operation.Replace("<IfDataObject>","");
			Operation.Replace("</IfDataObject>","");
		}
		else
		{
			RemoveBlock(Operation,"<IfDataObject>","</IfDataObject>");
			Operation.Replace("<IfNotDataObject>","");
			Operation.Replace("</IfNotDataObject>","");
		}
		CString SST_ID;
		SST_ID.Format("SST_%s_%s",ParentShortName,ClassNameToUpper(Name));
		SST_ID.MakeUpper();
		
		
		if(IsArray)
		{
			Operation.Replace("<Variable>","ArrayElement");
		}
		else
		{
			if(IsInDataObject)
				Operation.Replace("<Variable>","m_"+Name);
			else
				Operation.Replace("<Variable>",Name);
		}

		Operation.Replace("<Type>",pTypeInfo->CType);

		Operation.Replace("<SST_NAME>",SSTIDEnumName);
		Operation.Replace("<SST_ID>",SST_ID);

		CString ModifyFlagEnumName;
		ModifyFlagEnumName.Format("%s_MODIFY_FLAGS",ClassNameToUpper(ParentName));			
		Operation.Replace("<ModifyFlagEnumName>",ModifyFlagEnumName);

		CString ModifyFlag="MF_"+ClassNameToUpper(Name);
		Operation.Replace("<ModifyFlag>",ModifyFlag);

		
	}		

	CString Space="\r\n";
	Space+=szLineSpace;
	Operation.Replace("\r\n",Space);
	return Operation;
}

CString CCallInterfaceMakerDlg::MakeMsgHandlerMethods(CALLER_INTERFACE& InterfaceInfo,LPCTSTR szLineSpace)
{
	CString Methods;
	for(size_t j=0;j<InterfaceInfo.MethodList.size();j++)
	{
		INTERFACE_METHOD& MethodInfo=InterfaceInfo.MethodList[j];
		CString Method=m_MsgHandlerMethodHeaderTemple;
		Method.Replace("<MethodName>",MethodInfo.Name);
		Methods+=Method+"\r\n";
	}
	CString Space="\r\n";
	Space+=szLineSpace;
	Methods.Replace("\r\n",Space);
	return Methods;
}

CString CCallInterfaceMakerDlg::MakeMsgHandlerMethodsSource(CALLER_INTERFACE& InterfaceInfo,LPCTSTR szLineSpace)
{
	CString Methods;
	for(size_t j=0;j<InterfaceInfo.MethodList.size();j++)
	{
		INTERFACE_METHOD& MethodInfo=InterfaceInfo.MethodList[j];
		CString Method=m_MsgHandlerMethodSourceTemple;

		if(InterfaceInfo.MethodList[j].ParamList.size())
		{
			Method.Replace("<IfHaveMember>","");
			Method.Replace("</IfHaveMember>","");
		}
		else
		{
			RemoveBlock(Method,"<IfHaveMember>","</IfHaveMember>");
		}

		
		Method.Replace("<InterfaceName>",InterfaceInfo.Name);
		Method.Replace("<MethodName>",MethodInfo.Name);			
		CString Params=MakeParams(MethodInfo.ParamList,false,true);		
		Method.Replace("<Params>",Params);
		CString LineSpace=GetLineSpace(Method,"<ParamDefines>");
		CString ParamDefines=MakeParamDefines(MethodInfo.ParamList,LineSpace);
		Method.Replace("<ParamDefines>",ParamDefines);

		LineSpace=GetLineSpace(Method,"<ParamInitOperations>");
		CString ParamInitOperations=MakeInitOperations(MethodInfo,LineSpace);
		Method.Replace("<ParamInitOperations>",ParamInitOperations);

		LineSpace=GetLineSpace(Method,"<UnpackOperations>");
		CString SSTIDEnumName;
		SSTIDEnumName.Format("%s_%s_MEMBER_IDS",ClassNameToUpper(InterfaceInfo.Name),ClassNameToUpper(MethodInfo.Name));
		CString UnpackOperations=MakeUnpackOperations(MethodInfo,SSTIDEnumName,LineSpace);
		Method.Replace("<UnpackOperations>",UnpackOperations);

		Methods+=Method+"\r\n";
	}
	CString Space="\r\n";
	Space+=szLineSpace;
	Methods.Replace("\r\n",Space);
	return Methods;
}

CString CCallInterfaceMakerDlg::MakeMsgMapInits(CALLER_INTERFACE& InterfaceInfo,LPCTSTR szLineSpace)
{
	CString MapInits;
	for(size_t j=0;j<InterfaceInfo.MethodList.size();j++)
	{
		INTERFACE_METHOD& MethodInfo=InterfaceInfo.MethodList[j];
		CString MapInit=m_MsgMapInitTemple;

		CString SpaceNameUp=ClassNameToUpper(m_Name);		
		CString InterfaceNameUp=ClassNameToUpper(InterfaceInfo.Name);		
		CString MethodNameUp=ClassNameToUpper(MethodInfo.Name);		

		MapInit.Replace("<InterfaceName>",InterfaceInfo.Name);
		MapInit.Replace("<MethodName>",MethodInfo.Name);	
		MapInit.Replace("<!SpaceName>",SpaceNameUp);
		MapInit.Replace("<!InterfaceName>",InterfaceNameUp);
		MapInit.Replace("<!MethodName>",MethodNameUp);
		CString InterfaceID;
		InterfaceID.Format("%s_INTERFACE_%s",ClassNameToUpper(m_Name),ClassNameToUpper(InterfaceInfo.Name));
		MapInit.Replace("<InterfaceID>",InterfaceID);
		CString MethodID;
		MethodID.Format("METHOD_%s",ClassNameToUpper(MethodInfo.Name));
		MapInit.Replace("<MethodID>",MethodID);
		MapInit.Replace("<IsAckMsg>",InterfaceInfo.Type==INTERFACE_CALL_BACK?"true":"false");	

		MapInits+=MapInit+"\r\n";
	}
	CString Space="\r\n";
	Space+=szLineSpace;
	MapInits.Replace("\r\n",Space);
	return MapInits;
}

CString CCallInterfaceMakerDlg::MakeStructDefines(vector<STRUCT_DEFINE_INFO>& StructDefineList,LPCTSTR szLineSpace)
{
	CString StructDefines;
	for(size_t i=0;i<StructDefineList.size();i++)
	{		
		if(!StructDefineList[i].IsDataObject)
		{
			CString Struct=m_StructDefineTemplate;

			if(StructDefineList[i].MemberList.size())
			{
				Struct.Replace("<IfHaveMember>","");
				Struct.Replace("</IfHaveMember>","");
			}
			else
			{
				RemoveBlock(Struct,"<IfHaveMember>","</IfHaveMember>");
			}

			CString LineSpace=GetLineSpace(Struct,"<StructDescription>");
			CString StructDescription=ToComment(StructDefineList[i].Description,LineSpace);
			Struct.Replace("<StructDescription>",StructDescription);
			Struct.Replace("<StructName>",StructDefineList[i].Name);
			CString BaseStruct;
			if(!StructDefineList[i].BaseStruct.IsEmpty())
			{
				BaseStruct=":"+StructDefineList[i].BaseStruct;
			}
			Struct.Replace("<BaseStruct>",BaseStruct);
			LineSpace=GetLineSpace(Struct,"<Members>");
			CString Members=MakeStructMembers(StructDefineList[i],LineSpace);
			Struct.Replace("<Members>",Members);

			LineSpace=GetLineSpace(Struct,"<SSTIDEnumDefine>");
			CString SSTIDEnum=MakeStructSSTIDEnumDefines(StructDefineList[i],LineSpace);
			Struct.Replace("<SSTIDEnumDefine>",SSTIDEnum);

			LineSpace=GetLineSpace(Struct,"<InitOperations>");
			CString InitOperations=MakeInitOperations(StructDefineList[i],LineSpace);
			Struct.Replace("<InitOperations>",InitOperations);

			CString SSTIDEnumName;
			SSTIDEnumName.Format("%s_MEMBER_IDS",ClassNameToUpper(StructDefineList[i].Name));

			LineSpace=GetLineSpace(Struct,"<PackOperations>");
			CString PackOperations=MakePackOperations(StructDefineList[i],SSTIDEnumName,LineSpace);
			Struct.Replace("<PackOperations>",PackOperations);

			LineSpace=GetLineSpace(Struct,"<UnpackOperations>");
			CString UnpackOperations=MakeUnpackOperations(StructDefineList[i],SSTIDEnumName,LineSpace);
			Struct.Replace("<UnpackOperations>",UnpackOperations);

			LineSpace=GetLineSpace(Struct,"<PacketSizes>");
			CString PackSizes=MakePackSizes(StructDefineList[i],LineSpace);
			Struct.Replace("<PacketSizes>",PackSizes);

			LineSpace=GetLineSpace(Struct,"<CloneFromOperations>");
			CString CloneOperations=MakeCloneOperations(StructDefineList[i],LineSpace,"","Value.");
			Struct.Replace("<CloneFromOperations>",CloneOperations);

			LineSpace=GetLineSpace(Struct,"<CloneToOperations>");
			CloneOperations=MakeCloneOperations(StructDefineList[i],LineSpace,"CloneObject.","");
			Struct.Replace("<CloneToOperations>",CloneOperations);

			StructDefines+=Struct+"\r\n\r\n";
		}
	}
	CString Space="\r\n";
	Space+=szLineSpace;
	StructDefines.Replace("\r\n",Space);
	return StructDefines;
}

CString CCallInterfaceMakerDlg::MakeStructMembers(STRUCT_DEFINE_INFO& StructInfo,LPCTSTR szLineSpace)
{
	CString Members;
	int MaxLen=0;
	for(size_t i=0;i<StructInfo.MemberList.size();i++)
	{
		TYPE_DEFINE * pTypeInfo=FindVarType(StructInfo.MemberList[i].Type);
		if(pTypeInfo)
		{
			if(pTypeInfo->CType.GetLength()>MaxLen)
				MaxLen=pTypeInfo->CType.GetLength();
		}
		
	}
	int HeadLen=GetHeadLen(m_StructMemberTemplate,"<VariableDefine>");
	MaxLen+=HeadLen;
	int SpaceLen=(((int)ceil(MaxLen/4.0f))+1)*4-HeadLen;
	
	for(size_t i=0;i<StructInfo.MemberList.size();i++)
	{
		CString Member=m_StructMemberTemplate;
		
		TYPE_DEFINE * pTypeInfo=FindVarType(StructInfo.MemberList[i].Type);
		CString Space;
		if(pTypeInfo)
		{
			CString VariableDefine=pTypeInfo->GenerateOperations.VariableDefine;

			if(StructInfo.MemberList[i].IsArray)
			{
				VariableDefine=m_InterfaceConfig.ArrayDefineConfig.VariableDefine;
			}

			VariableDefine.Replace("<Type>",pTypeInfo->CType);
			int CurSpaceCount=(int)ceil((SpaceLen-pTypeInfo->CType.GetLength())/4.0f);
			while(CurSpaceCount)
			{
				Space+="	";
				CurSpaceCount--;
			}
			VariableDefine.Replace("<Space>",Space);
			CString VarName;
			
				
			VarName=StructInfo.MemberList[i].Name;
			if(StructInfo.IsDataObject)
			{
				VarName="m_"+VarName;
			}
			VariableDefine.Replace("<Variable>",VarName);
			Member.Replace("<VariableDefine>",VariableDefine);
		}
		CString Description=StructInfo.MemberList[i].Description;
		if(!Description.IsEmpty())
		{
			Description="//"+Description;
			Description.Replace("\r","");
			Description.Replace("\n","");
		}
		Member.Replace("<Description>",Description);

		Members+=Member+"\r\n";
	}
	CString Space="\r\n";
	Space+=szLineSpace;
	Members.Replace("\r\n",Space);
	return Members;
}

CString CCallInterfaceMakerDlg::MakeCloneOperations(STRUCT_DEFINE_INFO& StructInfo,LPCTSTR szLineSpace,LPCTSTR szLeftValue,LPCTSTR szRightValue)
{
	CString Source;

	for(size_t j=0;j<StructInfo.MemberList.size();j++)
	{
		TYPE_DEFINE * pTypeInfo=FindVarType(StructInfo.MemberList[j].Type);
		if(pTypeInfo)
		{
			CString Operation=pTypeInfo->GenerateOperations.CloneOperation;
			if(StructInfo.MemberList[j].IsArray)
				Operation=m_InterfaceConfig.ArrayDefineConfig.CloneOperation;

			if(StructInfo.MemberList[j].Flag&STRUCT_MEMBER_FLAG_NOT_MONITOR_UPDATE)
			{
				RemoveBlock(Operation,"<IfMonitorUpdate>","</IfMonitorUpdate>");
			}
			else
			{
				Operation.Replace("<IfMonitorUpdate>","");
				Operation.Replace("</IfMonitorUpdate>","");
			}

			if(pTypeInfo->Flag&TYPE_DEFINE_FLAG_DATA_OBJECT)
			{
				RemoveBlock(Operation,"<IfNotDataObject>","</IfNotDataObject>");
				Operation.Replace("<IfDataObject>","");
				Operation.Replace("</IfDataObject>","");
			}
			else
			{
				RemoveBlock(Operation,"<IfDataObject>","</IfDataObject>");
				Operation.Replace("<IfNotDataObject>","");
				Operation.Replace("</IfNotDataObject>","");
			}

			if(StructInfo.IsDataObject)
			{
				RemoveBlock(Operation,"<IfNotInDataObject>","</IfNotInDataObject>");
				Operation.Replace("<IfInDataObject>","");
				Operation.Replace("</IfInDataObject>","");
			}
			else
			{
				RemoveBlock(Operation,"<IfInDataObject>","</IfInDataObject>");
				Operation.Replace("<IfNotInDataObject>","");
				Operation.Replace("</IfNotInDataObject>","");
			}

			Operation.Replace("<LeftValue>",szLeftValue);
			Operation.Replace("<RightValue>",szRightValue);

			Operation.Replace("<Type>",pTypeInfo->CType);
			Operation.Replace("<VariableName>",StructInfo.MemberList[j].Name);
			if(StructInfo.IsDataObject)
				Operation.Replace("<Variable>","m_"+StructInfo.MemberList[j].Name);
			else
				Operation.Replace("<Variable>",StructInfo.MemberList[j].Name);
			Operation.Replace("<ClassName>",StructInfo.Name);

			CString ModifyFlagEnumName;
			ModifyFlagEnumName.Format("%s_MODIFY_FLAGS",ClassNameToUpper(StructInfo.Name));			
			Operation.Replace("<ModifyFlagEnumName>",ModifyFlagEnumName);

			CString ModifyFlag="MF_"+ClassNameToUpper(StructInfo.MemberList[j].Name);
			Operation.Replace("<ModifyFlag>",ModifyFlag);

			Source+=Operation+"\r\n";
		}		
	}

	CString Space="\r\n";
	Space+=szLineSpace;
	Source.Replace("\r\n",Space);
	return Source;
}

bool CCallInterfaceMakerDlg::MakeDataObjectFlagIndicesEnum(ENUM_DEFINE_INFO& EnumInfo)
{

	EnumInfo.Name="DATA_OBJECT_FLAGS";
	EnumInfo.ShortName=EnumInfo.Name;

	int DefineCount=0;

	for(size_t i=0;i<m_DataStructDefines.DataStructDefineList.size();i++)
	{
		if(m_DataStructDefines.DataStructDefineList[i]->ListType==DATA_STRUCT_STRUCT)
		{
			STRUCT_DEFINE_LIST * pStructList=(STRUCT_DEFINE_LIST *)m_DataStructDefines.DataStructDefineList[i];
			for(size_t j=0;j<pStructList->StructList.size();j++)
			{
				if(pStructList->StructList[j].IsDataObject)
				{
					ENUM_MEMBER_INFO MemberInfo;
					CString MemberName;
					MemberName=ClassNameToUpper(pStructList->StructList[j].Name);
					MemberInfo.Name.Format("DATA_OBJECT_FLAG_%s",MemberName);			

					EnumInfo.MemberList.push_back(MemberInfo);

					DefineCount++;
				}

			}
		}
	}
	
	ENUM_MEMBER_INFO MemberInfo;
	MemberInfo.Name="DATA_OBJECT_FLAG_MAX";
	EnumInfo.MemberList.push_back(MemberInfo);
	
	return true;
}

bool CCallInterfaceMakerDlg::MakeDataObjectSSTIDsEnum(ENUM_DEFINE_INFO& EnumInfo)
{

	EnumInfo.Name="DATA_OBJECT_SSTIDS";
	EnumInfo.ShortName=EnumInfo.Name;

	int DefineCount=0;

	for(size_t i=0;i<m_DataStructDefines.DataStructDefineList.size();i++)
	{
		if(m_DataStructDefines.DataStructDefineList[i]->ListType==DATA_STRUCT_STRUCT)
		{
			STRUCT_DEFINE_LIST * pStructList=(STRUCT_DEFINE_LIST *)m_DataStructDefines.DataStructDefineList[i];
			for(size_t j=0;j<pStructList->StructList.size();j++)
			{
				if(pStructList->StructList[j].IsDataObject)
				{
					ENUM_MEMBER_INFO MemberInfo;
					CString MemberName;
					MemberName=ClassNameToUpper(pStructList->StructList[j].Name);
					MemberInfo.Name.Format("SST_DO_%s",MemberName);
					MemberInfo.Value.Format("%d",pStructList->StructList[j].ObjectID);

					EnumInfo.MemberList.push_back(MemberInfo);

					DefineCount++;
				}

			}
		}
	}	
	return true;
}

CString CCallInterfaceMakerDlg::MakeDataObjectModifyFlagEnumDefine(STRUCT_DEFINE_INFO& StructInfo,LPCTSTR szLineSpace)
{

	ENUM_DEFINE_INFO EnumInfo;

	EnumInfo.Name.Format("%s_MODIFY_FLAGS",ClassNameToUpper(StructInfo.Name));
	EnumInfo.ShortName=EnumInfo.Name;
	EnumInfo.Flag|=ENUM_DEFINE_FLAG_IS_64BIT;

	EnumInfo.MemberList.resize(StructInfo.MemberList.size()+1);

	CString EnumType="UINT64";
	TYPE_DEFINE * pType=FindVarType(EnumType);
	if(pType)
		EnumType=pType->CType;

	UINT64 MF_ALL=0;
	for(size_t j=0;j<StructInfo.MemberList.size();j++)
	{
		CString MemberName;
		MemberName=ClassNameToUpper(StructInfo.MemberList[j].Name);
		EnumInfo.MemberList[j].Name.Format("MF_%s",MemberName);
		EnumInfo.MemberList[j].Value.Format("(((%s)1)<<%d)",EnumType,j);
		MF_ALL=(MF_ALL<<1)|1;
	}
	EnumInfo.MemberList[StructInfo.MemberList.size()].Name="MF_ALL";
	EnumInfo.MemberList[StructInfo.MemberList.size()].Value.Format("0x%llX",MF_ALL);

	return MakeEnumDefine(EnumInfo,szLineSpace);

	
}

CString CCallInterfaceMakerDlg::MakeDataObjectMembersGetModifyFlag(STRUCT_DEFINE_INFO& StructInfo,LPCTSTR szLineSpace)
{
	CString Source;

	for(size_t j=0;j<StructInfo.MemberList.size();j++)
	{
		TYPE_DEFINE * pTypeInfo=FindVarType(StructInfo.MemberList[j].Type);
		if(pTypeInfo&&(pTypeInfo->Flag&TYPE_DEFINE_FLAG_DATA_OBJECT)&&
			((StructInfo.MemberList[j].Flag&STRUCT_MEMBER_FLAG_NOT_MONITOR_UPDATE)==0)&&
			(!StructInfo.MemberList[j].IsArray))
		{
			CString Temp;

			Temp.Format("m_%s.GetModifyFlag(ModifyFlags,GetType,MemberFlags);\r\n",StructInfo.MemberList[j].Name);

			Source+=Temp;
		}		
	}


	CString Space="\r\n";
	Space+=szLineSpace;
	Source.Replace("\r\n",Space);
	return Source;
}

CString CCallInterfaceMakerDlg::MakeDataDataObjectMembersIsModified(STRUCT_DEFINE_INFO& StructInfo,LPCTSTR szLineSpace)
{
	CString Source;

	for(size_t j=0;j<StructInfo.MemberList.size();j++)
	{
		TYPE_DEFINE * pTypeInfo=FindVarType(StructInfo.MemberList[j].Type);
		if(pTypeInfo&&(pTypeInfo->Flag&TYPE_DEFINE_FLAG_DATA_OBJECT)&&
			((StructInfo.MemberList[j].Flag&STRUCT_MEMBER_FLAG_NOT_MONITOR_UPDATE)==0)&&
			(!StructInfo.MemberList[j].IsArray))
		{
			CString Temp;

			Temp.Format("||m_%s.IsModified(MemberFlags)\r\n",StructInfo.MemberList[j].Name);

			Source+=Temp;
		}		
	}

	if(Source.IsEmpty())
		Source="||false";

	CString Space="\r\n";
	Space+=szLineSpace;
	Source.Replace("\r\n",Space);
	return Source;
}

CString CCallInterfaceMakerDlg::MakeDataDataObjectMembersClearModifyFlag(STRUCT_DEFINE_INFO& StructInfo,LPCTSTR szLineSpace)
{
	CString Source;

	for(size_t j=0;j<StructInfo.MemberList.size();j++)
	{
		TYPE_DEFINE * pTypeInfo=FindVarType(StructInfo.MemberList[j].Type);
		if(pTypeInfo&&(pTypeInfo->Flag&TYPE_DEFINE_FLAG_DATA_OBJECT)&&
			((StructInfo.MemberList[j].Flag&STRUCT_MEMBER_FLAG_NOT_MONITOR_UPDATE)==0)&&
			(!StructInfo.MemberList[j].IsArray))
		{
			CString Temp;

			Temp.Format("m_%s.ClearModifyFlag(MemberFlags);\r\n",StructInfo.MemberList[j].Name);

			Source+=Temp;
		}		
	}

	CString Space="\r\n";
	Space+=szLineSpace;
	Source.Replace("\r\n",Space);
	return Source;
}



CString CCallInterfaceMakerDlg::MakeDataObjectSetMethodsDeclare(STRUCT_DEFINE_INFO& StructInfo,LPCTSTR szLineSpace)
{
	CString Methods;

	for(size_t j=0;j<StructInfo.MemberList.size();j++)
	{
		TYPE_DEFINE * pTypeInfo=FindVarType(StructInfo.MemberList[j].Type);
		if(pTypeInfo)
		{
			CString Method=pTypeInfo->GenerateOperations.SetMethodDeclare;
			if(StructInfo.MemberList[j].IsArray)
				Method=m_InterfaceConfig.ArrayDefineConfig.SetMethodDeclare;

			Method.Replace("<Type>",pTypeInfo->CType);
			Method.Replace("<VariableName>",StructInfo.MemberList[j].Name);

			Methods+=Method+"\r\n";
		}		
	}

	CString Space="\r\n";
	Space+=szLineSpace;
	Methods.Replace("\r\n",Space);
	return Methods;
}

CString CCallInterfaceMakerDlg::MakeDataObjectSetMethodsDefine(STRUCT_DEFINE_INFO& StructInfo,LPCTSTR szLineSpace)
{
	CString Methods;

	for(size_t j=0;j<StructInfo.MemberList.size();j++)
	{
		TYPE_DEFINE * pTypeInfo=FindVarType(StructInfo.MemberList[j].Type);
		if(pTypeInfo)
		{
			CString Method=pTypeInfo->GenerateOperations.SetMethodDefine;
			if(StructInfo.MemberList[j].IsArray)
				Method=m_InterfaceConfig.ArrayDefineConfig.SetMethodDefine;

			if(StructInfo.MemberList[j].Flag&STRUCT_MEMBER_FLAG_NOT_MONITOR_UPDATE)
			{
				RemoveBlock(Method,"<IfMonitorUpdate>","</IfMonitorUpdate>");
			}
			else
			{
				Method.Replace("<IfMonitorUpdate>","");
				Method.Replace("</IfMonitorUpdate>","");
			}

			Method.Replace("<Type>",pTypeInfo->CType);
			Method.Replace("<VariableName>",StructInfo.MemberList[j].Name);
			Method.Replace("<Variable>","m_"+StructInfo.MemberList[j].Name);
			Method.Replace("<ClassName>",StructInfo.Name);

			CString ModifyFlagEnumName;
			ModifyFlagEnumName.Format("%s_MODIFY_FLAGS",ClassNameToUpper(StructInfo.Name));			
			Method.Replace("<ModifyFlagEnumName>",ModifyFlagEnumName);

			CString ModifyFlag="MF_"+ClassNameToUpper(StructInfo.MemberList[j].Name);
			Method.Replace("<ModifyFlag>",ModifyFlag);

			Methods+=Method+"\r\n";
		}		
	}

	CString Space="\r\n";
	Space+=szLineSpace;
	Methods.Replace("\r\n",Space);
	return Methods;
}

CString CCallInterfaceMakerDlg::MakeDataObjectGetMethodsDeclare(STRUCT_DEFINE_INFO& StructInfo,LPCTSTR szLineSpace)
{
	CString Methods;

	for(size_t j=0;j<StructInfo.MemberList.size();j++)
	{
		TYPE_DEFINE * pTypeInfo=FindVarType(StructInfo.MemberList[j].Type);
		if(pTypeInfo)
		{
			CString Method=pTypeInfo->GenerateOperations.GetMethodDeclare;
			if(StructInfo.MemberList[j].IsArray)
				Method=m_InterfaceConfig.ArrayDefineConfig.GetMethodDeclare;

			Method.Replace("<Type>",pTypeInfo->CType);
			Method.Replace("<VariableName>",StructInfo.MemberList[j].Name);

			Methods+=Method+"\r\n";
		}		
	}

	CString Space="\r\n";
	Space+=szLineSpace;
	Methods.Replace("\r\n",Space);
	return Methods;
}

CString CCallInterfaceMakerDlg::MakeDataObjectGetMethodsDefine(STRUCT_DEFINE_INFO& StructInfo,LPCTSTR szLineSpace)
{
	CString Methods;

	for(size_t j=0;j<StructInfo.MemberList.size();j++)
	{
		TYPE_DEFINE * pTypeInfo=FindVarType(StructInfo.MemberList[j].Type);
		if(pTypeInfo)
		{
			CString Method=pTypeInfo->GenerateOperations.GetMethodDefine;
			if(StructInfo.MemberList[j].IsArray)
				Method=m_InterfaceConfig.ArrayDefineConfig.GetMethodDefine;

			Method.Replace("<Type>",pTypeInfo->CType);
			Method.Replace("<VariableName>",StructInfo.MemberList[j].Name);
			Method.Replace("<Variable>","m_"+StructInfo.MemberList[j].Name);
			Method.Replace("<ClassName>",StructInfo.Name);

			Methods+=Method+"\r\n";
		}		
	}

	CString Space="\r\n";
	Space+=szLineSpace;
	Methods.Replace("\r\n",Space);
	return Methods;
}

CString CCallInterfaceMakerDlg::MakeDataObjectModifyFlags(LPCTSTR szLineSpace)
{
	CString Source;

	vector<STRUCT_DEFINE_INFO> DataObjectList;

	for(size_t i=0;i<m_DataStructDefines.DataStructDefineList.size();i++)
	{
		if(m_DataStructDefines.DataStructDefineList[i]->ListType==DATA_STRUCT_STRUCT)
		{
			STRUCT_DEFINE_LIST * pStructList=(STRUCT_DEFINE_LIST *)m_DataStructDefines.DataStructDefineList[i];
			for(size_t j=0;j<pStructList->StructList.size();j++)
			{
				if(pStructList->StructList[j].IsDataObject)
				{
					DataObjectList.push_back(pStructList->StructList[j]);
				}
				
			}
		}
	}

	UINT ModifyFlagDefineSize=0;

	for(size_t i=0;i<m_DataStructDefines.DataStructDefineList.size();i++)
	{
		if(m_DataStructDefines.DataStructDefineList[i]->ListType==DATA_STRUCT_STRUCT)
		{
			STRUCT_DEFINE_LIST * pStructList=(STRUCT_DEFINE_LIST *)m_DataStructDefines.DataStructDefineList[i];
			for(size_t j=0;j<pStructList->StructList.size();j++)
			{
				if(pStructList->StructList[j].IsDataObject)
				{
					CString Define=m_DataObjectModifyFlagDefineTemple;

					DATA_OBJECT_MODIFY_FLAG ModifyFlag=MakeDataObjectFullModifyFlag(pStructList->StructList[j],DataObjectList);		

					ModifyFlagDefineSize=ModifyFlag.ModifyFlag.size();

					ModifyFlag.Name.Format("DOMF_%s_FULL",ClassNameToUpper(pStructList->StructList[j].Name));
					Define.Replace("<FlagName>",ModifyFlag.Name);


					CString Space=GetLineSpace(Define,"<FlagData>");
					CString FlagData=MakeDataObjectModifyFlagData(ModifyFlag,Space);
					Define.Replace("<FlagData>",FlagData);

					Source+=Define+"\r\n";
				}
			}
		}
	}

	for(size_t i=0;i<m_DataStructDefines.DataObjectModifyFlags.size();i++)
	{
		
		CString Define=m_DataObjectModifyFlagDefineTemple;		

		if(m_DataStructDefines.DataObjectModifyFlags[i].ModifyFlag.size()!=ModifyFlagDefineSize)
		{
			CString Msg;
			Msg.Format("[%s]的大小不正确,",m_DataStructDefines.DataObjectModifyFlags[i].Name);
			AfxMessageBox(Msg);
		}
		
		Define.Replace("<FlagName>",m_DataStructDefines.DataObjectModifyFlags[i].Name);


		CString Space=GetLineSpace(Define,"<FlagData>");
		CString FlagData=MakeDataObjectModifyFlagData(m_DataStructDefines.DataObjectModifyFlags[i],Space);
		Define.Replace("<FlagData>",FlagData);

		Source+=Define+"\r\n";
		
	}
	
	CString Space="\r\n";
	Space+=szLineSpace;
	Source.Replace("\r\n",Space);
	return Source;
}

CString CCallInterfaceMakerDlg::MakeDataObjectModifyFlagData(DATA_OBJECT_MODIFY_FLAG& ModifyFlag,LPCTSTR szLineSpace)
{
	CString Source;

	for(size_t i=0;i<ModifyFlag.ModifyFlag.size();i++)
	{
		CString Member;
		for(size_t j=0;j<ModifyFlag.ModifyFlag[i].Members.size();j++)
		{
			
			CString FlagUnit=m_DataObjectModifyFlagUnitTemple;

			FlagUnit.Replace("<ClassName>",ModifyFlag.ModifyFlag[i].ClassName);

			CString ModifyFlagEnumName;
			ModifyFlagEnumName.Format("%s_MODIFY_FLAGS",ClassNameToUpper(ModifyFlag.ModifyFlag[i].ClassName));			
			FlagUnit.Replace("<ModifyFlagEnumName>",ModifyFlagEnumName);

			CString Flag;
			Flag.Format("MF_%s",ClassNameToUpper(ModifyFlag.ModifyFlag[i].Members[j]));
			FlagUnit.Replace("<ModifyFlag>",Flag);
			if(j)
				Member+="|";
			Member+=FlagUnit;
		}
		if(Member.IsEmpty())
			Member="0";
		if(i)
			Source+=",\r\n";
		Source+=Member;
	}

	CString Space="\r\n";
	Space+=szLineSpace;
	Source.Replace("\r\n",Space);
	return Source;
}

CString CCallInterfaceMakerDlg::MakeEnumDefine(ENUM_DEFINE_INFO& EnumInfo,LPCTSTR szLineSpace)
{
	CString Source=m_EnumDefineTemplate;

	Source.Replace("<EnumName>",EnumInfo.Name);
	
	if(EnumInfo.Description.IsEmpty())
		Source.Replace("<Description>","");
	else
		Source.Replace("<Description>","//"+EnumInfo.Description);

	if(EnumInfo.Flag&ENUM_DEFINE_FLAG_IS_64BIT)
	{
		TYPE_DEFINE * pType=FindVarType("UINT64");
		if(pType)
			Source.Replace("<EnumType>",":"+pType->CType);
	}
	else
	{
		Source.Replace("<EnumType>","");
	}
	

	CString Space=GetLineSpace(Source,"<EnumMembers>");	

	CString EnumMembers;

	UINT MF_ALL=0;
	for(size_t i=0;i<EnumInfo.MemberList.size();i++)
	{
		
		CString Member=m_EnumMemberDefineTemplate;

		Member.Replace("<Name>",EnumInfo.MemberList[i].Name);

		if(EnumInfo.MemberList[i].Value.IsEmpty())
			Member.Replace("<Value>","");
		else
			Member.Replace("<Value>","="+EnumInfo.MemberList[i].Value);

		if(EnumInfo.MemberList[i].Description.IsEmpty())
			Member.Replace("<Description>","");
		else
			Member.Replace("<Description>","//"+EnumInfo.MemberList[i].Description);

		if(i)
			EnumMembers+=Space;
		EnumMembers+=Member+"\r\n";		
	}	

	Source.Replace("<EnumMembers>",EnumMembers);

	Space="\r\n";
	Space+=szLineSpace;
	Source.Replace("\r\n",Space);
	return Source;
}

CString CCallInterfaceMakerDlg::MakeEnumDefines(vector<ENUM_DEFINE_INFO>& EnumDefineList,LPCTSTR szLineSpace)
{
	CString EnumDefines;
	for(size_t i=0;i<EnumDefineList.size();i++)
	{		
		CString Enum=MakeEnumDefine(EnumDefineList[i],"");		

		EnumDefines+=Enum+"\r\n\r\n";
		
	}
	CString Space="\r\n";
	Space+=szLineSpace;
	EnumDefines.Replace("\r\n",Space);
	return EnumDefines;
}

CString CCallInterfaceMakerDlg::MakeEnumStrValue(ENUM_DEFINE_INFO& EnumInfo,LPCTSTR szLineSpace)
{
	CString Source=m_EnumStrValueDefineTemplate;

	Source.Replace("<EnumName>",EnumInfo.Name);

	CString MemberCount;

	MemberCount.Format("%d",EnumInfo.MemberList.size());

	Source.Replace("<EnumMemberCount>",MemberCount);


	CString StrValues;

	for(size_t i=0;i<EnumInfo.MemberList.size();i++)
	{
		if(i)
			StrValues+=",";

		StrValues+=MakeStringConst("\""+EnumInfo.MemberList[i].StrValue+"\"");
	}	

	Source.Replace("<EnumStrValues>",StrValues);

	CString Space="\r\n";
	Space+=szLineSpace;
	Source.Replace("\r\n",Space);
	return Source;
}

CString CCallInterfaceMakerDlg::MakeConstDefines(vector<CONST_DEFINE_INFO>& ConstDefineList,LPCTSTR szLineSpace)
{
	CString Source;
	
	
	for(size_t i=0;i<ConstDefineList.size();i++)
	{
		TYPE_DEFINE * pType=FindVarType(ConstDefineList[i].Type);
		CString Const;
		if(pType)
		{
			Const=m_ConstDefineTemplate;

			Const.Replace("<Type>",pType->CType);
		}
		else
		{
			Const=m_MacroDefineTemplate;
		}

		if(ConstDefineList[i].Description.IsEmpty())
			Const.Replace("<Description>","");
		else
			Const.Replace("<Description>","\\"+ConstDefineList[i].Description);
		Const.Replace("<Name>",ConstDefineList[i].Name);
		CString Value=ConstDefineList[i].Value;
		Value.Trim();
		if(Value.GetLength()&&Value[0]=='"'&&Value[Value.GetLength()-1]=='"')
			Value=MakeStringConst(Value);		
		Const.Replace("<Value>",Value);

		Source+=Const+"\r\n\r\n";

	}	

	

	CString Space="\r\n";
	Space+=szLineSpace;
	Source.Replace("\r\n",Space);
	return Source;
}

CString CCallInterfaceMakerDlg::MakeStringConst(LPCTSTR StrValue)
{
	CString Value=m_StringConstDefineTemplate;

	Value.Replace("<StrValue>",StrValue);

	return Value;
}

CString CCallInterfaceMakerDlg::MakeEnumStrValues(vector<ENUM_DEFINE_INFO>& EnumDefineList,LPCTSTR szLineSpace)
{
	CString EnumDefines;
	for(size_t i=0;i<EnumDefineList.size();i++)
	{		
		if(EnumDefineList[i].Flag&ENUM_DEFINE_FLAG_EXPORT_STR_VALUE)
		{
			CString StrValues=MakeEnumStrValue(EnumDefineList[i],"");

			EnumDefines+=StrValues+"\r\n\r\n";
		}
	}
	CString Space="\r\n";
	Space+=szLineSpace;
	EnumDefines.Replace("\r\n",Space);
	return EnumDefines;
}

DATA_OBJECT_MODIFY_FLAG CCallInterfaceMakerDlg::MakeDataObjectFullModifyFlag(STRUCT_DEFINE_INFO& StructInfo,vector<STRUCT_DEFINE_INFO>& DataObjectList)
{
	DATA_OBJECT_MODIFY_FLAG ModifyFlag;

	ModifyFlag.ModifyFlag.resize(DataObjectList.size());

	for(size_t i=0;i<DataObjectList.size();i++)
	{
		if(DataObjectList[i].Name==StructInfo.Name||HaveMemberByType(StructInfo,DataObjectList[i].Name))
		{
			ModifyFlag.ModifyFlag[i].ClassName=DataObjectList[i].Name;
			for(size_t j=0;j<DataObjectList[i].MemberList.size();j++)
			{
				if((DataObjectList[i].MemberList[j].Flag&STRUCT_MEMBER_FLAG_EXCLUDE_IN_PACKET)==0)
				{
					ModifyFlag.ModifyFlag[i].Members.push_back(DataObjectList[i].MemberList[j].Name);
				}
			}
		}
	}

	return ModifyFlag;
}

bool CCallInterfaceMakerDlg::HaveMemberByType(STRUCT_DEFINE_INFO& StructInfo,CString Type)
{
	if(StructInfo.BaseStruct==Type)
		return true;
	STRUCT_DEFINE_INFO * pStructInfo=GetStructDefineInfo(StructInfo.BaseStruct);
	if(pStructInfo)
	{
		if(HaveMemberByType(*pStructInfo,Type))
			return true;
	}
	for(size_t i=0;i<StructInfo.MemberList.size();i++)
	{
		if(StructInfo.MemberList[i].Type==Type)
			return true;		
		pStructInfo=GetStructDefineInfo(StructInfo.MemberList[i].Type);
		if(pStructInfo)
		{
			if(HaveMemberByType(*pStructInfo,Type))
				return true;
		}
	}
	return false;
}

STRUCT_DEFINE_INFO * CCallInterfaceMakerDlg::GetStructDefineInfo(CString Type)
{
	for(size_t i=0;i<m_DataStructDefines.DataStructDefineList.size();i++)
	{
		if(m_DataStructDefines.DataStructDefineList[i]->ListType==DATA_STRUCT_STRUCT)
		{
			STRUCT_DEFINE_LIST * pStructList=(STRUCT_DEFINE_LIST *)m_DataStructDefines.DataStructDefineList[i];
			for(size_t j=0;j<pStructList->StructList.size();j++)
			{
				if(pStructList->StructList[j].Name==Type)
					return &(pStructList->StructList[j]);
			}
		}
	}	
	return NULL;
}

void CCallInterfaceMakerDlg::GetDataObjectDefineList(vector<STRUCT_DEFINE_INFO>& DataObjectList)
{
	for(size_t i=0;i<m_DataStructDefines.DataStructDefineList.size();i++)
	{
		if(m_DataStructDefines.DataStructDefineList[i]->ListType==DATA_STRUCT_STRUCT)
		{
			STRUCT_DEFINE_LIST * pStructList=(STRUCT_DEFINE_LIST *)m_DataStructDefines.DataStructDefineList[i];
			for(size_t j=0;j<pStructList->StructList.size();j++)
			{
				if(pStructList->StructList[j].IsDataObject)
					DataObjectList.push_back(pStructList->StructList[j]);
			}
		}
	}	
}

void CCallInterfaceMakerDlg::OnNMDblclkInterfaceList(NMHDR *pNMHDR, LRESULT *pResult)
{
	//LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<NMITEMACTIVATE>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码
	*pResult = 0;
	OnBnClickedButtonEditInterface();
}

void CCallInterfaceMakerDlg::OnBnClickedButtonConfig()
{
	// TODO: 在此添加控件通知处理程序代码
	CDlgConfig Dlg;
	Dlg.m_Config=m_InterfaceConfig;
	if(Dlg.DoModal()==IDOK)
	{
		m_InterfaceConfig=Dlg.m_Config;
		SaveConfig();
	}
}

CString CCallInterfaceMakerDlg::GetLineSpace(CString& TempleStr,LPCTSTR szWord)
{
	CString Space;

	int Pos=TempleStr.Find(szWord);
	if(Pos>=0)
	{
		Space=TempleStr.Left(Pos);
		int SpaceLen=0;
		for(int i=Space.GetLength()-1;i>=0;i--)
		{
			if(Space[i]!=' '&&Space[i]!='	')
				break;
			SpaceLen++;
		}
		Space=Space.Right(SpaceLen);
	}
	return Space;
}
int CCallInterfaceMakerDlg::GetHeadLen(CString& TempleStr,LPCTSTR szWord)
{
	CString Head;
	int HeadLen=0;

	int Pos=TempleStr.Find(szWord);
	if(Pos>=0)
	{
		Head=TempleStr.Left(Pos);
		for(int i=Head.GetLength()-1;i>=0;i--)
		{
			if(Head[i]=='\r'||Head[i]=='\n')
				break;
			HeadLen++;
		}
	}
	return HeadLen;
}
CString CCallInterfaceMakerDlg::ToComment(CString& Content,LPCTSTR szLineSpace)
{
	CString ToComment="//"+Content;
	CString Space="\r\n//";
	Space+=szLineSpace;
	ToComment.Replace("\r\n",Space);
	return ToComment;
}

CString CCallInterfaceMakerDlg::ClassNameToFileName(LPCTSTR szClassName)
{
	CString Output=szClassName;
	Output.Trim();
	if(Output.GetLength()>=2)
	{
		if(Output[0]=='C'&&IsBigChar(Output[1]))
			Output.Delete(0);
	}
	return Output;
}

CString CCallInterfaceMakerDlg::ClassNameToUpper(LPCTSTR szClassName)
{
	CString Output=ClassNameToFileName(szClassName);
	for(int i=Output.GetLength()-1;i>0;i--)
	{
		if(IsBigChar(Output[i])&&i&&(!IsBigChar(Output[i-1])))
		{
			Output.Insert(i,'_');
		}
	}
	Output.MakeUpper();
	return Output;
}

bool CCallInterfaceMakerDlg::IsBigChar(TCHAR c)
{
	return c>='A'&&c<='Z';
}


void CCallInterfaceMakerDlg::OnBnClickedButtonSelectAllInterface()
{
	// TODO: 在此添加控件通知处理程序代码
	for(int i=0;i<m_lvInterfaceList.GetItemCount();i++)
	{
		m_lvInterfaceList.SetCheck(i,m_SelectAll);
	}
	m_SelectAll=!m_SelectAll;
}



void CCallInterfaceMakerDlg::RemoveBlock(CString& Template,LPCTSTR szBlockStart,LPCTSTR szBlockEnd)
{
	int StartPos=Template.Find(szBlockStart);
	while(StartPos>=0)
	{
		int EndPos=Template.Find(szBlockEnd);
		if(EndPos<0)
			return;
		EndPos+=strlen(szBlockEnd);
		if(EndPos<Template.GetLength()-1&&Template[EndPos]=='\r'&&Template[EndPos+1]=='\n')
			EndPos+=2;
		int CheckPos=StartPos;
		bool NeedCutHeader=false;
		while(CheckPos>0)
		{
			if(Template[CheckPos-1]!=' '&&Template[CheckPos-1]!='	')
			{
				if(Template[CheckPos-1]=='\r'||Template[CheckPos-1]=='\n')
					NeedCutHeader=true;
				break;
			}
			CheckPos--;
		}
		if(NeedCutHeader)
		{
			while(StartPos>0)
			{
				if(Template[StartPos-1]!=' '&&Template[StartPos-1]!='	')//&&Template[StartPos-1]!='\r'&&Template[StartPos-1]!='\n')
					break;
				StartPos--;
			}
		}
		Template.Delete(StartPos,EndPos-StartPos);
		StartPos=Template.Find(szBlockStart);
	}
}

CString CCallInterfaceMakerDlg::RestoreFileToTemplate(LPCTSTR szFileName,const CString& szTemplate)
{
	CFileStatus FileStatus;
	if(CFile::GetStatus(szFileName,FileStatus))
	{
		CFile InputFile;
		if(InputFile.Open(szFileName,CFile::modeRead|CFile::shareDenyNone))
		{
			ULONG64 Size=InputFile.GetLength();
			CString szInputFile;
			InputFile.Read(szInputFile.GetBuffer(Size),Size);
			szInputFile.ReleaseBuffer(Size);
			InputFile.Close();

			return RestoreToTemplate(szInputFile,szTemplate);
		}
	}
	
	return szTemplate;
}

CString CCallInterfaceMakerDlg::RestoreToTemplate(LPCTSTR szInput,const CString& szTemplate)
{
	CString Output=szInput;
	int BlockIndex=1;
	while(true)
	{
		CString BlockStart;
		CString BlockEnd;
		BlockStart.Format("<GenerateArea%dStart>",BlockIndex);
		BlockEnd.Format("<GenerateArea%dEnd>",BlockIndex);
		int TemplateStartPos=szTemplate.Find(BlockStart);
		int TemplateEndPos=szTemplate.Find(BlockEnd);
		int InputStartPos=Output.Find(BlockStart);
		int InputEndPos=Output.Find(BlockEnd);
		if(TemplateStartPos>=0&&TemplateEndPos>=0&&InputStartPos>=0&&InputEndPos>=0)
		{
			CString TemplateBlock=szTemplate.Mid(TemplateStartPos,TemplateEndPos-TemplateStartPos+BlockEnd.GetLength());
			Output.Delete(InputStartPos,InputEndPos-InputStartPos+BlockEnd.GetLength());
			Output.Insert(InputStartPos,TemplateBlock);
		}
		else
		{
			break;
		}
		BlockIndex++;
	}
	return Output;
}

void CCallInterfaceMakerDlg::GetDataStructPacketList(int Type,vector<CString>& NameList)
{
	for(size_t i=0;i<m_DataStructDefines.DataStructDefineList.size();i++)
	{
		if(m_DataStructDefines.DataStructDefineList[i]->ListType==Type)
			NameList.push_back(m_DataStructDefines.DataStructDefineList[i]->ListName);
	}
}

bool CCallInterfaceMakerDlg::MoveDataStruct(int Type,CString SourcePacket,CString TargetPacket,CString Name)
{
	BASE_DATA_STRUCT_DEFINE_LIST * pSourcePacket=NULL;
	BASE_DATA_STRUCT_DEFINE_LIST * pTargetPacket=NULL;
	for(size_t i=0;i<m_DataStructDefines.DataStructDefineList.size();i++)
	{
		if(m_DataStructDefines.DataStructDefineList[i]->ListType==Type&&m_DataStructDefines.DataStructDefineList[i]->ListName==SourcePacket)
			pSourcePacket=m_DataStructDefines.DataStructDefineList[i];
		if(m_DataStructDefines.DataStructDefineList[i]->ListType==Type&&m_DataStructDefines.DataStructDefineList[i]->ListName==TargetPacket)
			pTargetPacket=m_DataStructDefines.DataStructDefineList[i];
		if(pSourcePacket&&pTargetPacket)
			break;
	}
	if(pSourcePacket&&pTargetPacket)
	{
		switch(Type)
		{
		case DATA_STRUCT_CONST:
			{
				CONST_DEFINE_LIST * pSourceList=(CONST_DEFINE_LIST *)pSourcePacket;
				CONST_DEFINE_LIST * pTargetList=(CONST_DEFINE_LIST *)pTargetPacket;
				for(size_t i=0;i<pSourceList->ConstList.size();i++)
				{
					if(pSourceList->ConstList[i].Name==Name)
					{
						pTargetList->ConstList.push_back(pSourceList->ConstList[i]);
						pSourceList->ConstList.erase(pSourceList->ConstList.begin()+i);
						return true;
					}
				}
			}
			break;
		case DATA_STRUCT_ENUM:
			{
				ENUM_DEFINE_LIST * pSourceList=(ENUM_DEFINE_LIST *)pSourcePacket;
				ENUM_DEFINE_LIST * pTargetList=(ENUM_DEFINE_LIST *)pTargetPacket;
				for(size_t i=0;i<pSourceList->EnumList.size();i++)
				{
					if(pSourceList->EnumList[i].Name==Name)
					{
						pTargetList->EnumList.push_back(pSourceList->EnumList[i]);
						pSourceList->EnumList.erase(pSourceList->EnumList.begin()+i);
						return true;
					}
				}
			}
			break;
		case DATA_STRUCT_STRUCT:
			{
				STRUCT_DEFINE_LIST * pSourceList=(STRUCT_DEFINE_LIST *)pSourcePacket;
				STRUCT_DEFINE_LIST * pTargetList=(STRUCT_DEFINE_LIST *)pTargetPacket;
				for(size_t i=0;i<pSourceList->StructList.size();i++)
				{
					if(pSourceList->StructList[i].Name==Name)
					{
						pTargetList->StructList.push_back(pSourceList->StructList[i]);
						pSourceList->StructList.erase(pSourceList->StructList.begin()+i);
						return true;
					}
				}
			}
			break;
		}
	}
	return false;
}

void CCallInterfaceMakerDlg::OnBnClickedButtonDataStructDef()
{
	// TODO: 在此添加控件通知处理程序代码
	CDlgDataStructDefine Dlg;

	Dlg.m_pDataStructDefines=&m_DataStructDefines;
	if(Dlg.DoModal()==IDOK)
	{
		
	}	
}
