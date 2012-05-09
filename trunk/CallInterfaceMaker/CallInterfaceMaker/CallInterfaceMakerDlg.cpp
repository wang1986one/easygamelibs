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
	, m_ID(_T(""))
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CCallInterfaceMakerDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_INTERFACE_LIST, m_lvInterfaceList);
	DDX_Text(pDX, IDC_EDIT_NAME, m_Name);
	DDX_Text(pDX, IDC_EDIT_MODULE_ID, m_ID);
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
	ON_BN_CLICKED(IDC_BUTTON_TYPE_DEF, &CCallInterfaceMakerDlg::OnBnClickedButtonTypeDef)
	ON_NOTIFY(NM_DBLCLK, IDC_INTERFACE_LIST, &CCallInterfaceMakerDlg::OnNMDblclkInterfaceList)
	ON_BN_CLICKED(IDC_BUTTON_CONFIG, &CCallInterfaceMakerDlg::OnBnClickedButtonConfig)
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

	LPTSTR szTemp=m_TypeDefFileName.GetBuffer(MAX_PATH);
	_fullpath(szTemp,TYPE_DEF_FILE_NAME,MAX_PATH);
	m_TypeDefFileName.ReleaseBuffer();
	
	LoadTypeDef(m_TypeDefFileName);
	LoadTemples();
	LoadConfig();

	m_lvInterfaceList.SetExtendedStyle(LVS_EX_CHECKBOXES|LVS_EX_FULLROWSELECT|LVS_EX_GRIDLINES);


	m_lvInterfaceList.InsertColumn(0,"名称",LVCFMT_LEFT,100);
	m_lvInterfaceList.InsertColumn(1,"方法数量",LVCFMT_LEFT,60);
	m_lvInterfaceList.InsertColumn(2,"类型",LVCFMT_LEFT,60);
	m_lvInterfaceList.InsertColumn(3,"ID",LVCFMT_LEFT,100);
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

	if(Dlg.DoModal()==IDOK)
	{
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
	m_lvInterfaceList.SetItemText(Item,3,InterfaceInfo.ID);
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
	m_lvInterfaceList.SetItemText(Item,3,InterfaceInfo.ID);
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

bool CCallInterfaceMakerDlg::LoadTypeDef(LPCTSTR FileName)
{
	xml_parser Parser;

	if(Parser.parse_file(FileName,pug::parse_trim_attribute))
	{
		xml_node TypeDef=Parser.document();
		if(TypeDef.moveto_child("TypeDef"))
		{
			for(UINT i=0;i<TypeDef.children();i++)
			{
				xml_node Type=TypeDef.child(i);
				if(_stricmp(Type.name(),"Type")==0)
				{
					PARAM_TYPE TypeInfo;
					TypeInfo.Name=DecodeString(Type.attribute("Name").getvalue().c_str());
					TypeInfo.CType=DecodeString(Type.attribute("CType").getvalue().c_str());
					TypeInfo.IsFixedLength=(bool)Type.attribute("IsFixedLength");
					TypeInfo.NeedNULLCheck=(bool)Type.attribute("NeedNULLCheck");
					TypeInfo.ConstructOperation=DecodeString(Type.attribute("ConstructOperation").getvalue().c_str());
					TypeInfo.EvaluateOperation=DecodeString(Type.attribute("EvaluateOperation").getvalue().c_str());
					TypeInfo.NULLCheckOperation=DecodeString(Type.attribute("NULLCheckOperation").getvalue().c_str());
					TypeInfo.SizeCaculateOperation=DecodeString(Type.attribute("SizeCaculateOperation").getvalue().c_str());
					TypeInfo.PackOperation=DecodeString(Type.attribute("PackOperation").getvalue().c_str());
					TypeInfo.UnpackOperation=DecodeString(Type.attribute("UnpackOperation").getvalue().c_str());
					TypeInfo.ReferenceDefine=DecodeString(Type.attribute("ReferenceDefine").getvalue().c_str());
					m_ParamTypeList.push_back(TypeInfo);
				}
			}
		}
		return true;
	}
	return false;
}
bool CCallInterfaceMakerDlg::SaveTypeDef(LPCTSTR FileName)
{
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
		xml_node TypeDef=Doc.append_child(node_element,"TypeDef");
		
		for(size_t i=0;i<m_ParamTypeList.size();i++)
		{			
			xml_node Type=TypeDef.append_child(node_element,"Type");

			Type.append_attribute("Name",(LPCTSTR)m_ParamTypeList[i].Name);			
			Type.append_attribute("CType",(LPCTSTR)m_ParamTypeList[i].CType);
			Type.append_attribute("IsFixedLength",m_ParamTypeList[i].IsFixedLength);
			Type.append_attribute("NeedNULLCheck",m_ParamTypeList[i].NeedNULLCheck);
			Type.append_attribute("ConstructOperation",(LPCTSTR)EncodeString(m_ParamTypeList[i].ConstructOperation));
			Type.append_attribute("EvaluateOperation",(LPCTSTR)EncodeString(m_ParamTypeList[i].EvaluateOperation));
			Type.append_attribute("NULLCheckOperation",(LPCTSTR)EncodeString(m_ParamTypeList[i].NULLCheckOperation));
			Type.append_attribute("SizeCaculateOperation",(LPCTSTR)EncodeString(m_ParamTypeList[i].SizeCaculateOperation));
			Type.append_attribute("PackOperation",(LPCTSTR)EncodeString(m_ParamTypeList[i].PackOperation));
			Type.append_attribute("UnpackOperation",(LPCTSTR)EncodeString(m_ParamTypeList[i].UnpackOperation));
			Type.append_attribute("ReferenceDefine",(LPCTSTR)EncodeString(m_ParamTypeList[i].ReferenceDefine));
			
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

	if(TempleFile.Open(PROTOCOL_FILE_TEMPLE_FILE_NAME,CFile::modeRead|CFile::shareDenyNone))
	{
		ULONG64 Size=TempleFile.GetLength();
		char * Buffer=new char[Size+1];
		TempleFile.Read(Buffer,Size);
		Buffer[Size]=0;
		m_ProtocolFileTemplate=Buffer;
		delete[] Buffer;
		TempleFile.Close();
	}
	else
	{
		CString Msg;
		Msg.Format("无法打开文件%s",PROTOCOL_FILE_TEMPLE_FILE_NAME);
		MessageBox(Msg);
	}	

	if(TempleFile.Open(INTERFACE_TEMPLE_FILE_NAME,CFile::modeRead|CFile::shareDenyNone))
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
		Msg.Format("无法打开文件%s",INTERFACE_TEMPLE_FILE_NAME);
		MessageBox(Msg);
	}	

	if(TempleFile.Open(PARAM_TEMPLE_FILE_NAME,CFile::modeRead|CFile::shareDenyNone))
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
		Msg.Format("无法打开文件%s",PARAM_TEMPLE_FILE_NAME);
		MessageBox(Msg);
	}

	if(TempleFile.Open(PARAM_DESCRIPTION_TEMPLE_FILE_NAME,CFile::modeRead|CFile::shareDenyNone))
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
		Msg.Format("无法打开文件%s",PARAM_DESCRIPTION_TEMPLE_FILE_NAME);
		MessageBox(Msg);
	}

	if(TempleFile.Open(METHOD_HEADER_TEMPLE_FILE_NAME,CFile::modeRead|CFile::shareDenyNone))
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
		Msg.Format("无法打开文件%s",METHOD_HEADER_TEMPLE_FILE_NAME);
		MessageBox(Msg);
	}

	if(TempleFile.Open(METHOD_HEADER_PURE_TAIL_TEMPLE_FILE_NAME,CFile::modeRead|CFile::shareDenyNone))
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
		Msg.Format("无法打开文件%s",METHOD_HEADER_PURE_TAIL_TEMPLE_FILE_NAME);
		MessageBox(Msg);
	}

	if(TempleFile.Open(METHOD_CALLER_SOURCE_TEMPLE_FILE_NAME,CFile::modeRead|CFile::shareDenyNone))
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
		Msg.Format("无法打开文件%s",METHOD_CALLER_SOURCE_TEMPLE_FILE_NAME);
		MessageBox(Msg);
	}

	if(TempleFile.Open(MSG_PACK_METHOD_HEADER_TEMPLE_FILE_NAME,CFile::modeRead|CFile::shareDenyNone))
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
		Msg.Format("无法打开文件%s",MSG_PACK_METHOD_HEADER_TEMPLE_FILE_NAME);
		MessageBox(Msg);
	}

	if(TempleFile.Open(MSG_PACK_METHOD_SOURCE_TEMPLE_FILE_NAME,CFile::modeRead|CFile::shareDenyNone))
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
		Msg.Format("无法打开文件%s",MSG_PACK_METHOD_SOURCE_TEMPLE_FILE_NAME);
		MessageBox(Msg);
	}

	if(TempleFile.Open(MSG_HANDLER_METHOD_HEADER_TEMPLE_FILE_NAME,CFile::modeRead|CFile::shareDenyNone))
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
		Msg.Format("无法打开文件%s",MSG_HANDLER_METHOD_HEADER_TEMPLE_FILE_NAME);
		MessageBox(Msg);
	}

	if(TempleFile.Open(MSG_HANDLER_METHOD_SOURCE_TEMPLE_FILE_NAME,CFile::modeRead|CFile::shareDenyNone))
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
		Msg.Format("无法打开文件%s",MSG_HANDLER_METHOD_SOURCE_TEMPLE_FILE_NAME);
		MessageBox(Msg);
	}

	if(TempleFile.Open(MSG_MAP_INIT_TEMPLE_FILE_NAME,CFile::modeRead|CFile::shareDenyNone))
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
		Msg.Format("无法打开文件%s",MSG_MAP_INIT_TEMPLE_FILE_NAME);
		MessageBox(Msg);
	}

	if(TempleFile.Open(METHOD_SST_ID_ENUM_DEFINE_TEMPLE_FILE_NAME,CFile::modeRead|CFile::shareDenyNone))
	{
		ULONG64 Size=TempleFile.GetLength();
		char * Buffer=new char[Size+1];
		TempleFile.Read(Buffer,Size);
		Buffer[Size]=0;
		m_MethodSSTIDEnumDefineTemple=Buffer;
		delete[] Buffer;
		TempleFile.Close();
	}
	else
	{
		CString Msg;
		Msg.Format("无法打开文件%s",METHOD_SST_ID_ENUM_DEFINE_TEMPLE_FILE_NAME);
		MessageBox(Msg);
	}


	

	if(TempleFile.Open(MSG_CALLER_HEADER_TEMPLE_FILE_NAME,CFile::modeRead|CFile::shareDenyNone))
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
		Msg.Format("无法打开文件%s",MSG_CALLER_HEADER_TEMPLE_FILE_NAME);
		MessageBox(Msg);
	}
	if(TempleFile.Open(MSG_CALLER_SOURCE_TEMPLE_FILE_NAME,CFile::modeRead|CFile::shareDenyNone))
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
		Msg.Format("无法打开文件%s",MSG_CALLER_SOURCE_TEMPLE_FILE_NAME);
		MessageBox(Msg);
	}

	if(TempleFile.Open(MSG_HANDLER_HEADER_TEMPLE_FILE_NAME,CFile::modeRead|CFile::shareDenyNone))
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
		Msg.Format("无法打开文件%s",MSG_HANDLER_HEADER_TEMPLE_FILE_NAME);
		MessageBox(Msg);
	}

	if(TempleFile.Open(MSG_HANDLER_SOURCE_TEMPLE_FILE_NAME,CFile::modeRead|CFile::shareDenyNone))
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
		Msg.Format("无法打开文件%s",MSG_HANDLER_SOURCE_TEMPLE_FILE_NAME);
		MessageBox(Msg);
	}


	
	return true;
}

bool CCallInterfaceMakerDlg::LoadConfig()
{
	xml_parser Parser;

	if(Parser.parse_file(CONFIG_FILE_NAME,pug::parse_trim_attribute))
	{

		xml_node Config=Parser.document();
			
		if(Config.moveto_child("Config"))
		{
			{
				xml_node ProtocolFileExportConfig=Config;
				if(ProtocolFileExportConfig.moveto_child("ProtocolFileExport"))
				{
					m_InterfaceConfig.ProtocolFileExportConfig.IsExport=ProtocolFileExportConfig.attribute("IsExport");
					m_InterfaceConfig.ProtocolFileExportConfig.ExportExt=ProtocolFileExportConfig.attribute("FileExt").getvalue().c_str();
				}
			}
			{
				xml_node InterfaceExportConfig=Config;
				if(InterfaceExportConfig.moveto_child("InterfaceExport"))
				{
					m_InterfaceConfig.InterfaceExportConfig.IsExport=InterfaceExportConfig.attribute("IsExport");
					m_InterfaceConfig.InterfaceExportConfig.ExportExt=InterfaceExportConfig.attribute("FileExt").getvalue().c_str();
				}
			}
			{
				xml_node CallHeaderExportConfig=Config;
				if(CallHeaderExportConfig.moveto_child("CallHeaderExport"))
				{
					m_InterfaceConfig.CallHeaderExportConfig.IsExport=CallHeaderExportConfig.attribute("IsExport");
					m_InterfaceConfig.CallHeaderExportConfig.ExportExt=CallHeaderExportConfig.attribute("FileExt").getvalue().c_str();
				}
			}
			{
				xml_node CallSourceExportConfig=Config;
				if(CallSourceExportConfig.moveto_child("CallSourceExport"))
				{
					m_InterfaceConfig.CallSourceExportConfig.IsExport=CallSourceExportConfig.attribute("IsExport");
					m_InterfaceConfig.CallSourceExportConfig.ExportExt=CallSourceExportConfig.attribute("FileExt").getvalue().c_str();
				}
			}
			{
				xml_node HandlerHeaderExportConfig=Config;
				if(HandlerHeaderExportConfig.moveto_child("HandlerHeaderExport"))
				{
					m_InterfaceConfig.HandlerHeaderExportConfig.IsExport=HandlerHeaderExportConfig.attribute("IsExport");
					m_InterfaceConfig.HandlerHeaderExportConfig.ExportExt=HandlerHeaderExportConfig.attribute("FileExt").getvalue().c_str();
				}
			}
			{
				xml_node HandlerSourceExportConfig=Config;
				if(HandlerSourceExportConfig.moveto_child("HandlerSourceExport"))
				{
					m_InterfaceConfig.HandlerSourceExportConfig.IsExport=HandlerSourceExportConfig.attribute("IsExport");
					m_InterfaceConfig.HandlerSourceExportConfig.ExportExt=HandlerSourceExportConfig.attribute("FileExt").getvalue().c_str();
				}
			}
		}
			
	}
	else
	{
		CString Msg;
		Msg.Format("无法打开文件%s",CONFIG_FILE_NAME);
		MessageBox(Msg);
		return false;
	}
	return true;
}
bool CCallInterfaceMakerDlg::SaveConfig()
{
	std::ofstream os;	
	os.open(CONFIG_FILE_NAME, std::ios::trunc);	
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
			xml_node ProtocolFileExportConfig=Config.append_child(node_element,"ProtocolFileExport");
			ProtocolFileExportConfig.append_attribute("IsExport",
				m_InterfaceConfig.ProtocolFileExportConfig.IsExport);
			ProtocolFileExportConfig.append_attribute("FileExt",
				m_InterfaceConfig.ProtocolFileExportConfig.ExportExt);
		}

		{
			xml_node InterfaceExportConfig=Config.append_child(node_element,"InterfaceExport");
			InterfaceExportConfig.append_attribute("IsExport",
				m_InterfaceConfig.InterfaceExportConfig.IsExport);
			InterfaceExportConfig.append_attribute("FileExt",
				m_InterfaceConfig.InterfaceExportConfig.ExportExt);
		}

		{
			xml_node CallHeaderExportConfig=Config.append_child(node_element,"CallHeaderExport");
			CallHeaderExportConfig.append_attribute("IsExport",
				m_InterfaceConfig.CallHeaderExportConfig.IsExport);
			CallHeaderExportConfig.append_attribute("FileExt",
				m_InterfaceConfig.CallHeaderExportConfig.ExportExt);
		}

		{
			xml_node CallSourceExportConfig=Config.append_child(node_element,"CallSourceExport");
			CallSourceExportConfig.append_attribute("IsExport",
				m_InterfaceConfig.CallSourceExportConfig.IsExport);
			CallSourceExportConfig.append_attribute("FileExt",
				m_InterfaceConfig.CallSourceExportConfig.ExportExt);
		}

		{
			xml_node HandlerHeaderExportConfig=Config.append_child(node_element,"HandlerHeaderExport");
			HandlerHeaderExportConfig.append_attribute("IsExport",
				m_InterfaceConfig.HandlerHeaderExportConfig.IsExport);
			HandlerHeaderExportConfig.append_attribute("FileExt",
				m_InterfaceConfig.HandlerHeaderExportConfig.ExportExt);
		}

		{
			xml_node HandlerSourceExportConfig=Config.append_child(node_element,"HandlerSourceExport");
			HandlerSourceExportConfig.append_attribute("IsExport",
				m_InterfaceConfig.HandlerSourceExportConfig.IsExport);
			HandlerSourceExportConfig.append_attribute("FileExt",
				m_InterfaceConfig.HandlerSourceExportConfig.ExportExt);
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
void CCallInterfaceMakerDlg::OnBnClickedButtonLoad()
{
	// TODO: 在此添加控件通知处理程序代码
	CFileDialog Dlg(true,"*.xml","*.xml",OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,"XML Files(*.xml)|*,xml|All Files(*.*)|*.*||");

	if(Dlg.DoModal()==IDOK)
	{
		m_InterfacesFileName=Dlg.GetPathName();
		Load(m_InterfacesFileName);
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
	}
}
int CALLBACK BrowseCallbackProc(HWND hwnd,UINT uMsg,LPARAM lParam,LPARAM lpData)
{
	if(uMsg == BFFM_INITIALIZED)
	{
		CString DefaultFolder=theApp.GetProfileString("Main","DefaultBrowseFolder","");
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
		
		theApp.WriteProfileString("Main","DefaultBrowseFolder",FilePath);
		

		if(m_InterfaceConfig.ProtocolFileExportConfig.IsExport)
		{
			CString FileName;

			FileName.Format("%sProtocol.%s",m_Name,m_InterfaceConfig.ProtocolFileExportConfig.ExportExt);
			ExportProtocolFile(FilePath,FileName);
		}
		ExportInterfaces(FilePath);

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
		Interfaces.append_attribute("ID",(LPCTSTR)m_ID);
		for(size_t i=0;i<m_InterfaceList.size();i++)
		{
			CALLER_INTERFACE& InterfaceInfo=m_InterfaceList[i];
			xml_node Interface=Interfaces.append_child(node_element,"Interface");
			
			Interface.append_attribute("Name",(LPCTSTR)InterfaceInfo.Name);			
			Interface.append_attribute("Type",(long)InterfaceInfo.Type);
			Interface.append_attribute("ID",(LPCTSTR)InterfaceInfo.ID);
			Interface.append_attribute("Description",(LPCTSTR)EncodeString(InterfaceInfo.Description));
			for(size_t j=0;j<InterfaceInfo.MethodList.size();j++)
			{
				INTERFACE_METHOD& MethodInfo=InterfaceInfo.MethodList[j];
				xml_node Method=Interface.append_child(node_element,"Method");
				
				Method.append_attribute("Name",(LPCTSTR)MethodInfo.Name);		
				Method.append_attribute("ID",(LPCTSTR)MethodInfo.ID);
				Method.append_attribute("ParamIDSeed",(long)MethodInfo.ParamIDSeed);
				Method.append_attribute("Description",(LPCTSTR)EncodeString(MethodInfo.Description));
				for(size_t k=0;k<MethodInfo.ParamList.size();k++)
				{
					METHOD_PARAM& ParamInfo=MethodInfo.ParamList[k];

					xml_node Param=Method.append_child(node_element,"Param");
					CString Temp;

					
					Param.append_attribute("Name",(LPCTSTR)ParamInfo.Name);

					PARAM_TYPE * pTypeInfo=GetParamType(ParamInfo.Type);
					if(pTypeInfo)
					{									
						Param.append_attribute("Type",(LPCTSTR)EncodeString(pTypeInfo->Name));
					}
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

			m_Name=Interfaces.attribute("Name").getvalue().c_str();
			m_Name.Trim();
			m_ID=Interfaces.attribute("ID").getvalue().c_str();
			m_ID.Trim();

			for(UINT i=0;i<Interfaces.children();i++)
			{
				xml_node Interface=Interfaces.child(i);
				if(_stricmp(Interface.name(),"Interface")==0)
				{
					CALLER_INTERFACE InterfaceInfo;

					InterfaceInfo.Name=Interface.attribute("Name").getvalue().c_str();
					InterfaceInfo.Name.Trim();
					InterfaceInfo.Type=Interface.attribute("Type");
					InterfaceInfo.ID=Interface.attribute("ID").getvalue().c_str();
					InterfaceInfo.ID.Trim();
					InterfaceInfo.Description=DecodeString(Interface.attribute("Description").getvalue().c_str());
					for(UINT j=0;j<Interface.children();j++)
					{
						xml_node Method=Interface.child(j);
						if(_stricmp(Method.name(),"Method")==0)
						{
							INTERFACE_METHOD MethodInfo;
							MethodInfo.Name=Method.attribute("Name").getvalue().c_str();
							MethodInfo.Name.Trim();
							MethodInfo.ID=Method.attribute("ID").getvalue().c_str();
							MethodInfo.ID.Trim();
							MethodInfo.ParamIDSeed=(long)Method.attribute("ParamIDSeed");
							MethodInfo.Description=DecodeString(Method.attribute("Description").getvalue().c_str());
							
							for(UINT k=0;k<Method.children();k++)
							{
								xml_node Param=Method.child(k);
								if(_stricmp(Param.name(),"Param")==0)
								{

									METHOD_PARAM ParamInfo;

									ParamInfo.Name=Param.attribute("Name").getvalue().c_str();
									ParamInfo.Name.Trim();

									CString Temp=DecodeString(Param.attribute("Type").getvalue().c_str());
									Temp.Trim();									
									ParamInfo.Type=FindTypeIndex(Temp);

									ParamInfo.ID=(long)Param.attribute("ID");
									ParamInfo.Length=(long)Param.attribute("Length");

									ParamInfo.CanNull=Param.attribute("CanNull");
									ParamInfo.IsReference=Param.attribute("IsReference");

									ParamInfo.DefaultValue=DecodeString(Param.attribute("DefaultValue").getvalue().c_str());
									ParamInfo.DefaultValue.Trim();
									
								
									ParamInfo.Description=DecodeString(Param.attribute("Description").getvalue().c_str());
									

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
				m_InterfacePrefix=DecodeString(Prefix.attribute("Content").getvalue().c_str());
			}
			xml_node Postfix=Interfaces;
			if(Postfix.moveto_child("Postfix"))
			{
				m_InterfacePostFix=DecodeString(Postfix.attribute("Content").getvalue().c_str());
			}			
			FillListItem();
			UpdateData(false);
			return true;
		}
	}
	return false;
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

bool CCallInterfaceMakerDlg::ExportProtocolFile(LPCTSTR ExportDir,LPCTSTR FileName)
{
	CFile OutputFile;
	CString OutputFileName;

	OutputFileName=ExportDir;
	OutputFileName+="\\";
	OutputFileName+=FileName;

	if(OutputFile.Open(OutputFileName,CFile::modeCreate|CFile::modeWrite))
	{
		CString ProtocolFile=m_ProtocolFileTemplate;
		
		ProtocolFile.Replace("<Prefix>",m_InterfacePrefix);
		ProtocolFile.Replace("<Postfix>",m_InterfacePostFix);
		
		CString Includes;
		CString Temp;
		Temp.Format("#include \"%sObjectInterfaces.h\"\r\n\r\n",m_Name);
		Includes+=Temp;
		
		for(size_t i=0;i<m_InterfaceList.size();i++)
		{
			CALLER_INTERFACE& InterfaceInfo=m_InterfaceList[i];	

			if(InterfaceInfo.Type==INTERFACE_CALL)
			{
				Temp.Format("#include \"%sMethods.%s\"\r\n",InterfaceInfo.Name,
					m_InterfaceConfig.InterfaceExportConfig.ExportExt);
				Includes+=Temp;
			}
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
				if(OutputFile.Open(OutputFileName,CFile::modeCreate|CFile::modeWrite))
				{			
					CString Interface=m_InterfaceTemplate;
					Interface.Replace("<SpaceName>",m_Name);
					Interface.Replace("<InterfaceName>",InterfaceInfo.Name);
					Interface.Replace("<InterfaceDescription>",InterfaceInfo.Description);
					CString Methods=MakeMethods(InterfaceInfo.MethodList,true);			
					Interface.Replace("<Methods>",Methods);
					CString SSTIDDefines=MakeMethodSSTIDEnumDefines(InterfaceInfo);
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
		

		if(HeaderFile.Open(HeaderFileName,CFile::modeCreate|CFile::modeWrite))
		{
			CString Header=m_MsgCallerHeaderTemplate;

			Header.Replace("<SpaceName>",m_Name);
			Header.Replace("<InterfaceName>",InterfaceInfo.Name);
			CString Methods=MakeMethods(InterfaceInfo.MethodList,false);
			Methods+="\r\n";
			Methods+=MakeMsgPackMethods(InterfaceInfo.MethodList);
		
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

	if(m_InterfaceConfig.CallSourceExportConfig.IsExport)
	{
		SourceFileName.Format("%s\\%sMsgCaller.%s",ExportDir,InterfaceInfo.Name,
			m_InterfaceConfig.CallSourceExportConfig.ExportExt);
		if(SourceFile.Open(SourceFileName,CFile::modeCreate|CFile::modeWrite))
		{
			CString Source=m_MsgCallerSourceTemplate;

			Source.Replace("<SpaceName>",m_Name);
			Source.Replace("<InterfaceName>",InterfaceInfo.Name);

			CString Methods=MakeMethodsSource(InterfaceInfo);
			Methods+="\r\n";
			Methods+=MakeMsgPackMethodsSource(InterfaceInfo);
			

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

bool CCallInterfaceMakerDlg::ExportMsgHandler(LPCTSTR ExportDir,CALLER_INTERFACE& InterfaceInfo)
{
	CFile HeaderFile,SourceFile;

	CString HeaderFileName,SourceFileName;

	
	

	if(m_InterfaceConfig.HandlerHeaderExportConfig.IsExport)
	{
		HeaderFileName.Format("%s\\%sMsgHandler.%s",ExportDir,InterfaceInfo.Name,
			m_InterfaceConfig.HandlerHeaderExportConfig.ExportExt);
		if(HeaderFile.Open(HeaderFileName,CFile::modeCreate|CFile::modeWrite))
		{
			CString Header=m_MsgHandlerHeaderTemplate;

			Header.Replace("<SpaceName>",m_Name);
			Header.Replace("<InterfaceName>",InterfaceInfo.Name);
			CString Methods=MakeMsgHandlerMethods(InterfaceInfo);
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
		if(SourceFile.Open(SourceFileName,CFile::modeCreate|CFile::modeWrite))
		{
			CString Source=m_MsgHandlerSourceTemplate;

			Source.Replace("<SpaceName>",m_Name);
			Source.Replace("<InterfaceName>",InterfaceInfo.Name);

			CString MethodCount;
			MethodCount.Format("%d",InterfaceInfo.MethodList.size());
			Source.Replace("<MethodCount>",MethodCount);

			CString MsgMapInits=MakeMsgMapInits(InterfaceInfo);	
			Source.Replace("<MsgMapInits>",MsgMapInits);	

			CString Methods=MakeMsgHandlerMethodsSource(InterfaceInfo);	
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

CString CCallInterfaceMakerDlg::MakeMethods(vector<INTERFACE_METHOD> MethodList,bool IsPure)
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
		CString ParamDescriptions=MakeParamDescriptions(MethodInfo.ParamList);
		

		Method.Replace("<Params>",Params);
		Method.Replace("<ParamDescriptions>",ParamDescriptions);
		Methods+=Method+"\r\n";
	}
	return Methods;
}

CString CCallInterfaceMakerDlg::MakeMethodSSTIDEnumDefines(CALLER_INTERFACE& InterfaceInfo)
{
	CString MethodIDEnumDefines;

	CString Space;

	int Pos=m_MethodSSTIDEnumDefineTemple.Find("<SST_IDs>");
	if(Pos>=0)
	{
		Space=m_MethodSSTIDEnumDefineTemple.Left(Pos);
		int SpaceLen=0;
		for(int i=Space.GetLength()-1;i>=0;i--)
		{
			if(Space[i]!=' '&&Space[i]!='	')
				break;
			SpaceLen++;
		}
		Space=Space.Right(SpaceLen);
	}
	


	vector<INTERFACE_METHOD>& MethodList=InterfaceInfo.MethodList;
	for(size_t i=0;i<MethodList.size();i++)
	{
		INTERFACE_METHOD& MethodInfo=MethodList[i];

		if(MethodInfo.ParamList.size())
		{
			CString EnumDefine=m_MethodSSTIDEnumDefineTemple;
			
			CString SST_NAME;
			
			SST_NAME.Format("%s_MEMBER_IDS",MethodInfo.Name);;
			SST_NAME.MakeUpper();

			EnumDefine.Replace("<SST_NAME>",SST_NAME);

			CString SSTIDs;
			CString MethodName;
			MethodName=MethodInfo.Name;
			MethodName.MakeUpper();
			
			for(size_t j=0;j<MethodInfo.ParamList.size();j++)
			{
				CString ParamName;
				ParamName=MethodInfo.ParamList[j].Name;
				ParamName.MakeUpper();
				CString Temp;
				if(j==0)
					Temp.Format("%s_MEMBER_%s=%d,\r\n",
					MethodName,ParamName,MethodInfo.ParamList[j].ID);
				else
					Temp.Format("%s%s_MEMBER_%s=%d,\r\n",
						Space,MethodName,ParamName,MethodInfo.ParamList[j].ID);
				SSTIDs+=Temp;
			}

			EnumDefine.Replace("<SST_IDs>",SSTIDs);

			MethodIDEnumDefines+=EnumDefine+"\r\n";
		}
		
	}
	return MethodIDEnumDefines;
}

CString CCallInterfaceMakerDlg::MakeMsgPackMethods(vector<INTERFACE_METHOD> MethodList)
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
	return Methods;
}

CString CCallInterfaceMakerDlg::MakeParams(vector<METHOD_PARAM>& ParamList,bool IsDefine,bool IncludeDefaultValue)
{
	CString Params;
	for(size_t k=0;k<ParamList.size();k++)
	{
		METHOD_PARAM& ParamInfo=ParamList[k];

		PARAM_TYPE * pTypeInfo=GetParamType(ParamInfo.Type);
		if(pTypeInfo)
		{		
			CString Param=m_ParamTemple;
			Param.Replace("<ParamName>",ParamInfo.Name);
			if(IsDefine)
			{
				if(ParamInfo.IsReference)
					Param.Replace("<ParamType>",pTypeInfo->ReferenceDefine);
				else
					Param.Replace("<ParamType>",pTypeInfo->CType);

				if(pTypeInfo->IsFixedLength)
				{
					Param.Replace("<ParamLenght>","");
				}
				else
				{
					CString Length;
					Length.Format("[%u]",ParamInfo.Length);
					Param.Replace("<ParamLenght>",Length);
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

CString CCallInterfaceMakerDlg::MakeParamDescriptions(vector<METHOD_PARAM>& ParamList)
{
	CString ParamDescriptions;
	for(size_t k=0;k<ParamList.size();k++)
	{
		METHOD_PARAM& ParamInfo=ParamList[k];

		PARAM_TYPE * pTypeInfo=GetParamType(ParamInfo.Type);
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
	return ParamDescriptions;
}

CString CCallInterfaceMakerDlg::MakeParamDefines(vector<METHOD_PARAM>& ParamList)
{
	CString ParamDefines;
	for(size_t k=0;k<ParamList.size();k++)
	{
		METHOD_PARAM& ParamInfo=ParamList[k];

		PARAM_TYPE * pTypeInfo=GetParamType(ParamInfo.Type);
		if(pTypeInfo)
		{		
			CString Param=m_ParamTemple;
			Param.Replace("<ParamName>",ParamInfo.Name);			
			Param.Replace("<ParamType>",pTypeInfo->CType);
			Param.Replace("<ParamLenght>","");
			Param.Replace("<ParamDefaultValue>","");		
			
			ParamDefines+="	";
			ParamDefines+=Param;
			ParamDefines+=";\r\n";
		}
	}
	return ParamDefines;
}

CString CCallInterfaceMakerDlg::MakeMethodsSource(CALLER_INTERFACE& InterfaceInfo)
{
	CString Methods;
	for(size_t j=0;j<InterfaceInfo.MethodList.size();j++)
	{
		INTERFACE_METHOD& MethodInfo=InterfaceInfo.MethodList[j];
		CString Method=m_MethodCallerSourceTemple;
		CString SpaceNameUp=m_Name;
		SpaceNameUp.MakeUpper();
		CString InterfaceNameUp=InterfaceInfo.Name;
		InterfaceNameUp.MakeUpper();
		CString MethodNameUp=MethodInfo.Name;
		MethodNameUp.MakeUpper();
		Method.Replace("<InterfaceName>",InterfaceInfo.Name);
		Method.Replace("<MethodName>",MethodInfo.Name);	
		Method.Replace("<!SpaceName>",SpaceNameUp);
		Method.Replace("<!InterfaceName>",InterfaceNameUp);
		Method.Replace("<!MethodName>",MethodNameUp);
		CString Params=MakeParams(MethodInfo.ParamList,true,true);
		Method.Replace("<Params>",Params);	
		Params=MakeParams(MethodInfo.ParamList,false,true);
		if(!Params.IsEmpty())
			Params="\r\n		,"+Params;
		Method.Replace("<,Params>",Params);	
		CString PackSizes=MakePackSizes(MethodInfo);
		Method.Replace("<PacketSizes>",PackSizes);
		Method.Replace("<InterfaceID>",InterfaceInfo.ID);
		Method.Replace("<MethodID>",MethodInfo.ID);
		Method.Replace("<IsAckMsg>",InterfaceInfo.Type==INTERFACE_CALL_BACK?"true":"false");



		Methods+=Method+"\r\n";
	}
	return Methods;
}

CString CCallInterfaceMakerDlg::MakeMsgPackMethodsSource(CALLER_INTERFACE& InterfaceInfo)
{
	CString Methods;
	for(size_t j=0;j<InterfaceInfo.MethodList.size();j++)
	{
		INTERFACE_METHOD& MethodInfo=InterfaceInfo.MethodList[j];
		CString Method=m_MsgPackMethodSourceTemple;
		CString SpaceNameUp=m_Name;
		SpaceNameUp.MakeUpper();
		CString InterfaceNameUp=InterfaceInfo.Name;
		InterfaceNameUp.MakeUpper();
		CString MethodNameUp=MethodInfo.Name;
		MethodNameUp.MakeUpper();
		Method.Replace("<InterfaceName>",InterfaceInfo.Name);
		Method.Replace("<MethodName>",MethodInfo.Name);
		Method.Replace("<!SpaceName>",SpaceNameUp);
		Method.Replace("<!InterfaceName>",InterfaceNameUp);
		Method.Replace("<!MethodName>",MethodNameUp);
		CString Params=MakeParams(MethodInfo.ParamList,true,true);
		if(!Params.IsEmpty())
			Params=","+Params;
		Method.Replace("<,Params>",Params);
		CString PackOperations=MakePackOperations(MethodInfo);
		Method.Replace("<PackOperations>",PackOperations);

		Methods+=Method+"\r\n";
	}
	return Methods;
}

CString CCallInterfaceMakerDlg::MakePackOperations(INTERFACE_METHOD MethodInfo)
{
	CString PackOperations;

	for(size_t i=0;i<MethodInfo.ParamList.size();i++)
	{
		
		PARAM_TYPE * pTypeInfo=GetParamType(MethodInfo.ParamList[i].Type);
		if(pTypeInfo)
		{
			CString Operation=pTypeInfo->PackOperation;
			CString SST_ID;
			CString SST_NAME;
			SST_ID.Format("%s_MEMBER_%s",MethodInfo.Name,MethodInfo.ParamList[i].Name);
			SST_ID.MakeUpper();
			SST_NAME.Format("%s_MEMBER_IDS",MethodInfo.Name);;
			SST_NAME.MakeUpper();
			Operation.Replace("<Variable>",MethodInfo.ParamList[i].Name);
			Operation.Replace("<SST_NAME>",SST_NAME);
			Operation.Replace("<SST_ID>",SST_ID);

			PackOperations+=Operation;
			PackOperations+="\r\n";
		}		
	}

	return PackOperations;
}

CString CCallInterfaceMakerDlg::MakeUnpackOperations(INTERFACE_METHOD MethodInfo)
{
	CString PackOperations;

	for(size_t i=0;i<MethodInfo.ParamList.size();i++)
	{

		PARAM_TYPE * pTypeInfo=GetParamType(MethodInfo.ParamList[i].Type);
		if(pTypeInfo)
		{
			CString Operation=pTypeInfo->UnpackOperation;
			CString SST_ID;
			CString SST_NAME;
			SST_ID.Format("%s_MEMBER_%s",MethodInfo.Name,MethodInfo.ParamList[i].Name);
			SST_ID.MakeUpper();
			SST_NAME.Format("%s_MEMBER_IDS",MethodInfo.Name);;
			SST_NAME.MakeUpper();
			Operation.Replace("<Variable>",MethodInfo.ParamList[i].Name);
			Operation.Replace("<SST_NAME>",SST_NAME);
			Operation.Replace("<SST_ID>",SST_ID);

			PackOperations+=Operation;
			PackOperations+="\r\n";
		}		
	}

	return PackOperations;
}

CString CCallInterfaceMakerDlg::MakePackSizes(INTERFACE_METHOD MethodInfo)
{
	CString PackSizes;

	for(size_t i=0;i<MethodInfo.ParamList.size();i++)
	{

		PARAM_TYPE * pTypeInfo=GetParamType(MethodInfo.ParamList[i].Type);
		if(pTypeInfo)
		{
			CString PackSize=pTypeInfo->SizeCaculateOperation;			
			PackSize.Replace("<Variable>",MethodInfo.ParamList[i].Name);

			PackSizes+="\r\n		+";
			PackSizes+=PackSize;
			
		}		
	}

	return PackSizes;
}

CString CCallInterfaceMakerDlg::MakeMsgHandlerMethods(CALLER_INTERFACE& InterfaceInfo)
{
	CString Methods;
	for(size_t j=0;j<InterfaceInfo.MethodList.size();j++)
	{
		INTERFACE_METHOD& MethodInfo=InterfaceInfo.MethodList[j];
		CString Method=m_MsgHandlerMethodHeaderTemple;
		Method.Replace("<MethodName>",MethodInfo.Name);
		Methods+=Method+"\r\n";
	}
	return Methods;
}

CString CCallInterfaceMakerDlg::MakeMsgHandlerMethodsSource(CALLER_INTERFACE& InterfaceInfo)
{
	CString Methods;
	for(size_t j=0;j<InterfaceInfo.MethodList.size();j++)
	{
		INTERFACE_METHOD& MethodInfo=InterfaceInfo.MethodList[j];
		CString Method=m_MsgHandlerMethodSourceTemple;
		Method.Replace("<InterfaceName>",InterfaceInfo.Name);
		Method.Replace("<MethodName>",MethodInfo.Name);			
		CString Params=MakeParams(MethodInfo.ParamList,false,true);		
		Method.Replace("<Params>",Params);
		CString ParamDefines=MakeParamDefines(MethodInfo.ParamList);
		Method.Replace("<ParamDefines>",ParamDefines);
		CString UnpackOperations=MakeUnpackOperations(MethodInfo);
		Method.Replace("<UnpackOperations>",UnpackOperations);

		Methods+=Method+"\r\n";
	}
	return Methods;
}

CString CCallInterfaceMakerDlg::MakeMsgMapInits(CALLER_INTERFACE& InterfaceInfo)
{
	CString MapInits;
	for(size_t j=0;j<InterfaceInfo.MethodList.size();j++)
	{
		INTERFACE_METHOD& MethodInfo=InterfaceInfo.MethodList[j];
		CString MapInit=m_MsgMapInitTemple;

		CString SpaceNameUp=m_Name;
		SpaceNameUp.MakeUpper();
		CString InterfaceNameUp=InterfaceInfo.Name;
		InterfaceNameUp.MakeUpper();
		CString MethodNameUp=MethodInfo.Name;
		MethodNameUp.MakeUpper();

		MapInit.Replace("<InterfaceName>",InterfaceInfo.Name);
		MapInit.Replace("<MethodName>",MethodInfo.Name);	
		MapInit.Replace("<!SpaceName>",SpaceNameUp);
		MapInit.Replace("<!InterfaceName>",InterfaceNameUp);
		MapInit.Replace("<!MethodName>",MethodNameUp);
		MapInit.Replace("<InterfaceID>",InterfaceInfo.ID);
		MapInit.Replace("<MethodID>",MethodInfo.ID);
		MapInit.Replace("<IsAckMsg>",InterfaceInfo.Type==INTERFACE_CALL_BACK?"true":"false");	

		MapInits+=MapInit+"\r\n";
	}
	return MapInits;
}

void CCallInterfaceMakerDlg::OnBnClickedButtonTypeDef()
{
	// TODO: 在此添加控件通知处理程序代码
	CDlgTypeEditor Dlg;

	Dlg.m_TypeList=m_ParamTypeList;
	if(Dlg.DoModal()==IDOK)
	{
		m_ParamTypeList=Dlg.m_TypeList;
		SaveTypeDef(m_TypeDefFileName);
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
	Dlg.m_pConfig=&m_InterfaceConfig;
	Dlg.DoModal();
}
