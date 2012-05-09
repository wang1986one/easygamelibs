/****************************************************************************/
/*                                                                          */
/*      文件名:    DlgTextureConfig.cpp                                     */
/*      创建日期:  2011年10月07日                                           */
/*      作者:      Sagasarate                                               */
/*                                                                          */
/*      本软件版权归Sagasarate(sagasarate@sina.com)所有                     */
/*      你可以将本软件用于任何商业和非商业软件开发，但                      */
/*      必须保留此版权声明                                                  */
/*                                                                          */
/****************************************************************************/
// DlgTextureConfig.cpp : 实现文件
//

#include "stdafx.h"
#include "D3DUIEdit.h"
#include "DlgTextureConfig.h"
#include ".\dlgtextureconfig.h"


// CDlgTextureConfig 对话框

#define RECT_COLOR		0xffff00ff
#define BORDER_COLOR	0xff0000ff

IMPLEMENT_DYNAMIC(CDlgTextureConfig, CPropertyConfigDialog)
CDlgTextureConfig::CDlgTextureConfig(CWnd* pParent /*=NULL*/)
	: CPropertyConfigDialog(CDlgTextureConfig::IDD, pParent)
	, m_TextureFileName(_T(""))	
	, m_ShowZoom(1)
	,m_TextureRect(0,0,20,20)
	
{
	m_pTexture=NULL;
	m_pTextureRect=NULL;
	ZeroMemory(m_pLines,sizeof(m_pLines));	
	m_WndBorders[RECT_TOP]=2;
	m_WndBorders[RECT_BOTTOM]=2;
	m_WndBorders[RECT_LEFT]=2;
	m_WndBorders[RECT_RIGHT]=2;
	m_WndBorders[RECT_TOP_LEFT]=2;
	m_WndBorders[RECT_TOP_RIGHT]=2;
	m_WndBorders[RECT_BOTTOM_LEFT]=2;
	m_WndBorders[RECT_BOTTOM_RIGHT]=2;
	m_pGUIObject=NULL;
}

CDlgTextureConfig::~CDlgTextureConfig()
{
	
}

void CDlgTextureConfig::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT1, m_TextureFileName);	
	DDX_Control(pDX, IDC_SPIN1, m_ZoomSpin);
	DDX_Text(pDX, IDC_EDIT2, m_ShowZoom);
	DDV_MinMaxInt(pDX, m_ShowZoom, 1, 8);
	DDX_Text(pDX, IDC_EDIT3, m_TextureRect.left);
	DDX_Text(pDX, IDC_EDIT4, m_TextureRect.top);
	DDX_Text(pDX, IDC_EDIT5, m_TextureRect.right);
	DDX_Text(pDX, IDC_EDIT6, m_TextureRect.bottom);

	DDX_Text(pDX, IDC_EDIT7, m_WndBorders[RECT_LEFT]);
	DDX_Text(pDX, IDC_EDIT8, m_WndBorders[RECT_TOP]);
	DDX_Text(pDX, IDC_EDIT9, m_WndBorders[RECT_RIGHT]);
	DDX_Text(pDX, IDC_EDIT10, m_WndBorders[RECT_BOTTOM]);

	DDX_Text(pDX, IDC_EDIT11, m_WndBorders[RECT_TOP_LEFT]);
	DDX_Text(pDX, IDC_EDIT12, m_WndBorders[RECT_TOP_RIGHT]);
	DDX_Text(pDX, IDC_EDIT13, m_WndBorders[RECT_BOTTOM_LEFT]);
	DDX_Text(pDX, IDC_EDIT14, m_WndBorders[RECT_BOTTOM_RIGHT]);
}


BEGIN_MESSAGE_MAP(CDlgTextureConfig, CPropertyConfigDialog)
	ON_BN_CLICKED(IDC_BUTTON_SELECT_FILE, OnBnClickedButtonSelectFile)
	ON_EN_CHANGE(IDC_EDIT2, OnEnChangeEdit2)
	ON_MESSAGE(WM_D3DGUI_RESIZED,OnGUIWndResized)
	ON_MESSAGE(WM_D3DGUI_RESIZING,OnGUIWndResizing)	
	ON_BN_CLICKED(IDAPPLY, OnBnClickedApply)
	ON_BN_CLICKED(IDC_BUTTON_EDIT_BORDERS, OnBnClickedButtonEditBorders)
END_MESSAGE_MAP()


// CDlgTextureConfig 消息处理程序

BOOL CDlgTextureConfig::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  在此添加额外的初始化
	CRect Rect;
	GetClientRect(&Rect);
	Rect.left+=10;
	Rect.right-=10;
	Rect.top+=10;
	Rect.bottom-=100;
	m_pD3DViewBox=(CD3DViewBox *)(RUNTIME_CLASS(CD3DViewBox)->CreateObject());
	m_pD3DViewBox->Create(NULL,NULL,WS_CHILD|WS_VISIBLE|WS_BORDER,Rect,this,11028);
	m_pD3DViewBox->Init();
	m_ZoomSpin.SetRange32(1,8);


	CD3DWnd * pWnd=m_pGUIObject;
	if(pWnd)
	{
		CEasyString Temp;
		pWnd->GetTexture(Temp,m_TextureRect);			
		m_TextureFileName=Temp;
		pWnd->GetBorders(m_WndBorders);
	}
	
	
	UpdateData(false);
	LoadTexture();

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}

void CDlgTextureConfig::OnBnClickedButtonSelectFile()
{
	// TODO: 在此添加控件通知处理程序代码
	CFileDialog dlg(true,_T(""),_T("*.png;*.tga;*.bmp;*.jpg;*.blp"),0,
		_T("All Suport Files|*.png;*.tga;*.bmp;*.jpg;*.blp|PNG File(*.png)|*.png|TGA File(*.tga)|*.tga|BMP File(*.bmp)|*.bmp|JPG File(*.jpg)|*.jpg|BLP File(*.blp)|*.blp|All File(*.*)|*.*||"));
	

	if(dlg.DoModal()==IDOK)
	{
		UpdateData(true);
		m_TextureFileName=dlg.GetPathName();
		UpdateData(false);
		LoadTexture();
	}
}

void CDlgTextureConfig::LoadTexture()
{
	SAFE_RELEASE(m_pTexture);
	m_pTexture=m_pD3DViewBox->GetObjectCreator()->CreateTextureFromFile(m_TextureFileName);		
	ShowTexture();	
}
BOOL CDlgTextureConfig::DestroyWindow()
{
	// TODO: 在此添加专用代码和/或调用基类
	for(int i=0;i<LINE_MAX;i++)
	{
		SAFE_RELEASE(m_pLines[i]);
	}
	SAFE_RELEASE(m_pTexture);
	SAFE_RELEASE(m_pTextureRect);	
	return CPropertyConfigDialog::DestroyWindow();
}

void CDlgTextureConfig::OnEnChangeEdit2()
{
	// TODO:  如果该控件是 RICHEDIT 控件，则它将不会
	// 发送该通知，除非重写 CPropertyConfigDialog::OnInitDialog()
	// 函数并调用 CRichEditCtrl().SetEventMask()，
	// 同时将 ENM_CHANGE 标志“或”运算到掩码中。

	// TODO:  在此添加控件通知处理程序代码	
	if(m_pTexture)
	{
		UpdateData(true);
	}
	ShowTexture();
}

void CDlgTextureConfig::ShowTexture()
{
	if(m_pTexture)
	{		
		if(m_pTextureRect==NULL)
			m_pTextureRect=m_pD3DViewBox->GetObjectCreator()->CreateWndRect(NULL);
		int Width=m_pTexture->GetWidth()*m_ShowZoom;
		int Height=m_pTexture->GetHeight()*m_ShowZoom;
		m_pTextureRect->SetRect(&FLOAT_RECT(0,0,Width,Height));
		m_pTextureRect->SetTexture(m_pTexture);
		m_pTextureRect->SetUV(&FLOAT_RECT(0.0f,0.0f,1.0f,1.0f));
		m_pD3DViewBox->SetContentSize(Width+4*m_ShowZoom,Height+4*m_ShowZoom);
		BulidLines();
	}
}

void CDlgTextureConfig::BulidLines()
{


	WIN_BORDERS borders;


	
	//检测窗口大小
	int LMinWidth,RMinWidth;

	LMinWidth=m_WndBorders[RECT_LEFT];
	if(LMinWidth<m_WndBorders[RECT_TOP_LEFT])
		LMinWidth=m_WndBorders[RECT_TOP_LEFT];
	if(LMinWidth<m_WndBorders[RECT_BOTTOM_LEFT])
		LMinWidth=m_WndBorders[RECT_BOTTOM_LEFT];

	RMinWidth=m_WndBorders[RECT_RIGHT];
	if(RMinWidth<m_WndBorders[RECT_TOP_RIGHT])
		RMinWidth=m_WndBorders[RECT_TOP_RIGHT];
	if(RMinWidth<m_WndBorders[RECT_BOTTOM_RIGHT])
		RMinWidth=m_WndBorders[RECT_BOTTOM_RIGHT];

	int MinWidth=LMinWidth+RMinWidth;
	int MinHeight=m_WndBorders[RECT_TOP]+m_WndBorders[RECT_BOTTOM];

	if(m_TextureRect.bottom<m_TextureRect.top+MinHeight)
		m_TextureRect.bottom=m_TextureRect.top+MinHeight;
	if(m_TextureRect.right<m_TextureRect.left+MinWidth)
		m_TextureRect.right=m_TextureRect.left+MinWidth;

	int LineWidth=m_ShowZoom;

	int Top=m_TextureRect.top*m_ShowZoom;
	int Bottom=m_TextureRect.bottom*m_ShowZoom;
	int Left=m_TextureRect.left*m_ShowZoom;
	int Right=m_TextureRect.right*m_ShowZoom;

	int BorderTop=(m_TextureRect.top+m_WndBorders[RECT_TOP])*m_ShowZoom;
	int BorderBottom=(m_TextureRect.bottom-m_WndBorders[RECT_BOTTOM])*m_ShowZoom;
	int BorderLeft=(m_TextureRect.left+m_WndBorders[RECT_LEFT])*m_ShowZoom;
	int BorderRight=(m_TextureRect.right-m_WndBorders[RECT_RIGHT])*m_ShowZoom;

	int BorderTopLeft=(m_TextureRect.left+m_WndBorders[RECT_TOP_LEFT])*m_ShowZoom;
	int BorderTopRight=(m_TextureRect.right-m_WndBorders[RECT_TOP_RIGHT])*m_ShowZoom;
	int BorderBottomLeft=(m_TextureRect.left+m_WndBorders[RECT_BOTTOM_LEFT])*m_ShowZoom;
	int BorderBottomRight=(m_TextureRect.right-m_WndBorders[RECT_BOTTOM_RIGHT])*m_ShowZoom;


	borders[RECT_TOP]=BorderBottom-BorderTop;
	borders[RECT_BOTTOM]=0;
	borders[RECT_LEFT]=0;
	borders[RECT_RIGHT]=0;
	borders[RECT_TOP_LEFT]=0;
	borders[RECT_TOP_RIGHT]=0;
	borders[RECT_BOTTOM_LEFT]=0;
	borders[RECT_BOTTOM_RIGHT]=0;


	if(m_pLines[LINE_BORDER_LEFT]==NULL)
		m_pLines[LINE_BORDER_LEFT]=m_pD3DViewBox->GetGUI()->CreateWnd(CRect(0,0,0,0));	
	m_pLines[LINE_BORDER_LEFT]->SetBorders(borders);
	m_pLines[LINE_BORDER_LEFT]->SetRect(CRect(BorderLeft,BorderTop,BorderLeft+LineWidth,BorderBottom));
	m_pLines[LINE_BORDER_LEFT]->EnableDrag(true);
	m_pLines[LINE_BORDER_LEFT]->SetID(LINE_BORDER_LEFT);
	m_pLines[LINE_BORDER_LEFT]->SetBKColor(BORDER_COLOR);
	m_pLines[LINE_BORDER_LEFT]->TopTo(true);

	if(m_pLines[LINE_BORDER_RIGHT]==NULL)
		m_pLines[LINE_BORDER_RIGHT]=m_pD3DViewBox->GetGUI()->CreateWnd(CRect(0,0,0,0));	
	m_pLines[LINE_BORDER_RIGHT]->SetBorders(borders);
	m_pLines[LINE_BORDER_RIGHT]->SetRect(CRect(BorderRight,BorderTop,BorderRight+LineWidth,BorderBottom));
	m_pLines[LINE_BORDER_RIGHT]->EnableDrag(true);
	m_pLines[LINE_BORDER_RIGHT]->SetID(LINE_BORDER_RIGHT);
	m_pLines[LINE_BORDER_RIGHT]->SetBKColor(BORDER_COLOR);
	m_pLines[LINE_BORDER_RIGHT]->TopTo(true);

	borders[RECT_TOP]=BorderTop-Top;
	borders[RECT_BOTTOM]=0;
	borders[RECT_LEFT]=0;
	borders[RECT_RIGHT]=0;
	borders[RECT_TOP_LEFT]=0;
	borders[RECT_TOP_RIGHT]=0;
	borders[RECT_BOTTOM_LEFT]=0;
	borders[RECT_BOTTOM_RIGHT]=0;

	if(m_pLines[LINE_BORDER_TOP_LEFT]==NULL)
		m_pLines[LINE_BORDER_TOP_LEFT]=m_pD3DViewBox->GetGUI()->CreateWnd(CRect(0,0,0,0));	
	m_pLines[LINE_BORDER_TOP_LEFT]->SetBorders(borders);
	m_pLines[LINE_BORDER_TOP_LEFT]->SetRect(CRect(BorderTopLeft,Top,BorderTopLeft+LineWidth,BorderTop));
	m_pLines[LINE_BORDER_TOP_LEFT]->EnableDrag(true);
	m_pLines[LINE_BORDER_TOP_LEFT]->SetID(LINE_BORDER_TOP_LEFT);
	m_pLines[LINE_BORDER_TOP_LEFT]->SetBKColor(BORDER_COLOR);
	m_pLines[LINE_BORDER_TOP_LEFT]->TopTo(true);

	if(m_pLines[LINE_BORDER_TOP_RIGHT]==NULL)
		m_pLines[LINE_BORDER_TOP_RIGHT]=m_pD3DViewBox->GetGUI()->CreateWnd(CRect(0,0,0,0));	
	m_pLines[LINE_BORDER_TOP_RIGHT]->SetBorders(borders);
	m_pLines[LINE_BORDER_TOP_RIGHT]->SetRect(CRect(BorderTopRight,Top,BorderTopRight+LineWidth,BorderTop));
	m_pLines[LINE_BORDER_TOP_RIGHT]->EnableDrag(true);
	m_pLines[LINE_BORDER_TOP_RIGHT]->SetID(LINE_BORDER_TOP_RIGHT);
	m_pLines[LINE_BORDER_TOP_RIGHT]->SetBKColor(BORDER_COLOR);
	m_pLines[LINE_BORDER_TOP_RIGHT]->TopTo(true);

	borders[RECT_TOP]=Bottom-BorderBottom;
	borders[RECT_BOTTOM]=0;
	borders[RECT_LEFT]=0;
	borders[RECT_RIGHT]=0;
	borders[RECT_TOP_LEFT]=0;
	borders[RECT_TOP_RIGHT]=0;
	borders[RECT_BOTTOM_LEFT]=0;
	borders[RECT_BOTTOM_RIGHT]=0;

	if(m_pLines[LINE_BORDER_BOTTOM_LEFT]==NULL)
		m_pLines[LINE_BORDER_BOTTOM_LEFT]=m_pD3DViewBox->GetGUI()->CreateWnd(CRect(0,0,0,0));	
	m_pLines[LINE_BORDER_BOTTOM_LEFT]->SetBorders(borders);
	m_pLines[LINE_BORDER_BOTTOM_LEFT]->SetRect(CRect(BorderBottomLeft,BorderBottom,BorderBottomLeft+LineWidth,Bottom));
	m_pLines[LINE_BORDER_BOTTOM_LEFT]->EnableDrag(true);
	m_pLines[LINE_BORDER_BOTTOM_LEFT]->SetID(LINE_BORDER_BOTTOM_LEFT);
	m_pLines[LINE_BORDER_BOTTOM_LEFT]->SetBKColor(BORDER_COLOR);
	m_pLines[LINE_BORDER_BOTTOM_LEFT]->TopTo(true);

	if(m_pLines[LINE_BORDER_BOTTOM_RIGHT]==NULL)
		m_pLines[LINE_BORDER_BOTTOM_RIGHT]=m_pD3DViewBox->GetGUI()->CreateWnd(CRect(0,0,0,0));	
	m_pLines[LINE_BORDER_BOTTOM_RIGHT]->SetBorders(borders);
	m_pLines[LINE_BORDER_BOTTOM_RIGHT]->SetRect(CRect(BorderBottomRight,BorderBottom,BorderBottomRight+LineWidth,Bottom));
	m_pLines[LINE_BORDER_BOTTOM_RIGHT]->EnableDrag(true);
	m_pLines[LINE_BORDER_BOTTOM_RIGHT]->SetID(LINE_BORDER_BOTTOM_RIGHT);
	m_pLines[LINE_BORDER_BOTTOM_RIGHT]->SetBKColor(BORDER_COLOR);
	m_pLines[LINE_BORDER_BOTTOM_RIGHT]->TopTo(true);


	borders[RECT_TOP]=LineWidth;
	borders[RECT_BOTTOM]=0;
	borders[RECT_LEFT]=0;
	borders[RECT_RIGHT]=0;
	borders[RECT_TOP_LEFT]=0;
	borders[RECT_TOP_RIGHT]=0;
	borders[RECT_BOTTOM_LEFT]=0;
	borders[RECT_BOTTOM_RIGHT]=0;

	if(m_pLines[LINE_TOP]==NULL)
		m_pLines[LINE_TOP]=m_pD3DViewBox->GetGUI()->CreateWnd(CRect(0,0,0,0));	
	m_pLines[LINE_TOP]->SetBorders(borders);
	m_pLines[LINE_TOP]->SetRect(CRect(Left,Top,Right,Top+LineWidth));
	m_pLines[LINE_TOP]->EnableDrag(true);
	m_pLines[LINE_TOP]->SetID(LINE_TOP);
	m_pLines[LINE_TOP]->SetBKColor(RECT_COLOR);
	m_pLines[LINE_TOP]->TopTo(true);


	if(m_pLines[LINE_BOTTOM]==NULL)
		m_pLines[LINE_BOTTOM]=m_pD3DViewBox->GetGUI()->CreateWnd(CRect(0,0,0,0));	
	m_pLines[LINE_BOTTOM]->SetBorders(borders);
	m_pLines[LINE_BOTTOM]->SetRect(CRect(Left,Bottom,Right,Bottom+LineWidth));
	m_pLines[LINE_BOTTOM]->EnableDrag(true);
	m_pLines[LINE_BOTTOM]->SetID(LINE_BOTTOM);
	m_pLines[LINE_BOTTOM]->SetBKColor(RECT_COLOR);
	m_pLines[LINE_BOTTOM]->TopTo(true);


	if(m_pLines[LINE_BORDER_TOP]==NULL)
		m_pLines[LINE_BORDER_TOP]=m_pD3DViewBox->GetGUI()->CreateWnd(CRect(0,0,0,0));	
	m_pLines[LINE_BORDER_TOP]->SetBorders(borders);
	m_pLines[LINE_BORDER_TOP]->SetRect(CRect(Left,BorderTop,Right,BorderTop+LineWidth));
	m_pLines[LINE_BORDER_TOP]->EnableDrag(true);
	m_pLines[LINE_BORDER_TOP]->SetID(LINE_BORDER_TOP);
	m_pLines[LINE_BORDER_TOP]->SetBKColor(BORDER_COLOR);
	m_pLines[LINE_BORDER_TOP]->TopTo(true);

	if(m_pLines[LINE_BORDER_BOTTOM]==NULL)
		m_pLines[LINE_BORDER_BOTTOM]=m_pD3DViewBox->GetGUI()->CreateWnd(CRect(0,0,0,0));	
	m_pLines[LINE_BORDER_BOTTOM]->SetBorders(borders);
	m_pLines[LINE_BORDER_BOTTOM]->SetRect(CRect(Left,BorderBottom,Right,BorderBottom+LineWidth));
	m_pLines[LINE_BORDER_BOTTOM]->EnableDrag(true);
	m_pLines[LINE_BORDER_BOTTOM]->SetID(LINE_BORDER_BOTTOM);
	m_pLines[LINE_BORDER_BOTTOM]->SetBKColor(BORDER_COLOR);
	m_pLines[LINE_BORDER_BOTTOM]->TopTo(true);

	borders[RECT_TOP]=m_TextureRect.Height()*m_ShowZoom;
	borders[RECT_BOTTOM]=0;
	borders[RECT_LEFT]=0;
	borders[RECT_RIGHT]=0;
	borders[RECT_TOP_LEFT]=0;
	borders[RECT_TOP_RIGHT]=0;
	borders[RECT_BOTTOM_LEFT]=0;
	borders[RECT_BOTTOM_RIGHT]=0;


	if(m_pLines[LINE_LEFT]==NULL)
		m_pLines[LINE_LEFT]=m_pD3DViewBox->GetGUI()->CreateWnd(CRect(0,0,0,0));	
	m_pLines[LINE_LEFT]->SetBorders(borders);
	m_pLines[LINE_LEFT]->SetRect(CRect(Left,Top,Left+LineWidth,Bottom));
	m_pLines[LINE_LEFT]->EnableDrag(true);
	m_pLines[LINE_LEFT]->SetID(LINE_LEFT);
	m_pLines[LINE_LEFT]->SetBKColor(RECT_COLOR);
	m_pLines[LINE_LEFT]->TopTo(true);

	if(m_pLines[LINE_RIGHT]==NULL)
		m_pLines[LINE_RIGHT]=m_pD3DViewBox->GetGUI()->CreateWnd(CRect(0,0,0,0));	
	m_pLines[LINE_RIGHT]->SetBorders(borders);
	m_pLines[LINE_RIGHT]->SetRect(CRect(Right,Top,Right+LineWidth,Bottom));
	m_pLines[LINE_RIGHT]->EnableDrag(true);
	m_pLines[LINE_RIGHT]->SetID(LINE_RIGHT);
	m_pLines[LINE_RIGHT]->SetBKColor(RECT_COLOR);
	m_pLines[LINE_RIGHT]->TopTo(true);



}

LRESULT CDlgTextureConfig::OnGUIWndResized(WPARAM wParam,LPARAM lParam)
{
	UpdateData(false);
	return true;
}

LRESULT CDlgTextureConfig::OnGUIWndResizing(WPARAM wParam,LPARAM lParam)
{
	CRect Rect;
	switch(wParam)
	{
	case LINE_BORDER_TOP:
		{
			Rect=m_pLines[LINE_BORDER_TOP]->GetRect();
			m_WndBorders[RECT_TOP]=(Rect.top/m_ShowZoom-m_TextureRect.top);
			if(m_WndBorders[RECT_TOP]<0)
				m_WndBorders[RECT_TOP]=0;
			BulidLines();
		}
		break;
	case LINE_BORDER_BOTTOM:
		{
			Rect=m_pLines[LINE_BORDER_BOTTOM]->GetRect();
			m_WndBorders[RECT_BOTTOM]=(m_TextureRect.bottom-Rect.top/m_ShowZoom);
			if(m_WndBorders[RECT_BOTTOM]<0)
				m_WndBorders[RECT_BOTTOM]=0;
			BulidLines();
		}
		break;
	case LINE_TOP:
		{
			Rect=m_pLines[LINE_TOP]->GetRect();
			m_TextureRect.top=Rect.top/m_ShowZoom;			
			BulidLines();
		}
		break;
	case LINE_BOTTOM:
		{
			Rect=m_pLines[LINE_BOTTOM]->GetRect();
			m_TextureRect.bottom=Rect.top/m_ShowZoom;			
			BulidLines();
		}
		break;
	case LINE_BORDER_LEFT:
		{
			Rect=m_pLines[LINE_BORDER_LEFT]->GetRect();
			m_WndBorders[RECT_LEFT]=(Rect.left/m_ShowZoom-m_TextureRect.left);
			if(m_WndBorders[RECT_LEFT]<0)
				m_WndBorders[RECT_LEFT]=0;
			BulidLines();
		}
		break;
	case LINE_BORDER_RIGHT:
		{
			Rect=m_pLines[LINE_BORDER_RIGHT]->GetRect();
			m_WndBorders[RECT_RIGHT]=(m_TextureRect.right-Rect.right/m_ShowZoom);
			if(m_WndBorders[RECT_RIGHT]<0)
				m_WndBorders[RECT_RIGHT]=0;
			BulidLines();
		}
		break;
	case LINE_BORDER_TOP_LEFT:
		{
			Rect=m_pLines[LINE_BORDER_TOP_LEFT]->GetRect();
			m_WndBorders[RECT_TOP_LEFT]=(Rect.left/m_ShowZoom-m_TextureRect.left);
			if(m_WndBorders[RECT_TOP_LEFT]<0)
				m_WndBorders[RECT_TOP_LEFT]=0;
			BulidLines();
		}
		break;
	case LINE_BORDER_TOP_RIGHT:
		{
			Rect=m_pLines[LINE_BORDER_TOP_RIGHT]->GetRect();
			m_WndBorders[RECT_TOP_RIGHT]=(m_TextureRect.right-Rect.right/m_ShowZoom);
			if(m_WndBorders[RECT_TOP_RIGHT]<0)
				m_WndBorders[RECT_TOP_RIGHT]=0;
			BulidLines();
		}
		break;
	case LINE_BORDER_BOTTOM_LEFT:
		{
			Rect=m_pLines[LINE_BORDER_BOTTOM_LEFT]->GetRect();
			m_WndBorders[RECT_BOTTOM_LEFT]=(Rect.left/m_ShowZoom-m_TextureRect.left);
			if(m_WndBorders[RECT_BOTTOM_LEFT]<0)
				m_WndBorders[RECT_BOTTOM_LEFT]=0;
			BulidLines();
		}
		break;
	case LINE_BORDER_BOTTOM_RIGHT:
		{
			Rect=m_pLines[LINE_BORDER_BOTTOM_RIGHT]->GetRect();
			m_WndBorders[RECT_BOTTOM_RIGHT]=(m_TextureRect.right-Rect.right/m_ShowZoom);
			if(m_WndBorders[RECT_BOTTOM_RIGHT]<0)
				m_WndBorders[RECT_BOTTOM_RIGHT]=0;
			BulidLines();
		}
		break;
	case LINE_LEFT:
		{
			Rect=m_pLines[LINE_LEFT]->GetRect();
			m_TextureRect.left=Rect.left/m_ShowZoom;			
			BulidLines();
		}
		break;
	case LINE_RIGHT:
		{
			Rect=m_pLines[LINE_RIGHT]->GetRect();
			m_TextureRect.right=Rect.left/m_ShowZoom;			
			BulidLines();
		}
		break;
	}
	return true;
}

void CDlgTextureConfig::OnOK()
{
	// TODO: 在此添加专用代码和/或调用基类
	OnBnClickedApply();
	CPropertyConfigDialog::OnOK();
}

void CDlgTextureConfig::OnBnClickedApply()
{
	// TODO: 在此添加控件通知处理程序代码
	
	CD3DWnd * pWnd=m_pGUIObject;
	if(pWnd)
	{
		pWnd->SetTexture(m_TextureFileName,m_TextureRect);			
		pWnd->SetBorders(m_WndBorders);			
	}
	
}

void CDlgTextureConfig::OnBnClickedButtonEditBorders()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(true);
	BulidLines();
}
