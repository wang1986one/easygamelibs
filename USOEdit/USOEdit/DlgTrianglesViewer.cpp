/****************************************************************************/
/*                                                                          */
/*      文件名:    DlgTrianglesViewer.cpp                                   */
/*      创建日期:  2011年10月07日                                           */
/*      作者:      Sagasarate                                               */
/*                                                                          */
/*      本软件版权归Sagasarate(sagasarate@sina.com)所有                     */
/*      你可以将本软件用于任何商业和非商业软件开发，但                      */
/*      必须保留此版权声明                                                  */
/*                                                                          */
/****************************************************************************/
// DlgTrianglesViewer.cpp : 实现文件
//

#include "stdafx.h"
#include "DlgTrianglesViewer.h"




// CDlgTrianglesViewer 对话框

IMPLEMENT_DYNAMIC(CDlgTrianglesViewer, CPropertyConfigDialog)

CDlgTrianglesViewer::CDlgTrianglesViewer(CWnd* pParent /*=NULL*/)
	: CPropertyConfigDialog(CDlgTrianglesViewer::IDD, pParent)
	, m_VertexCount(0)
	, m_TriangleCount(0)
{
	m_IsModual=false;
	m_pSubMesh=NULL;
	m_ViewMode=VIEW_MODE_VERTEX;
}

CDlgTrianglesViewer::~CDlgTrianglesViewer()
{
}

void CDlgTrianglesViewer::DoDataExchange(CDataExchange* pDX)
{
	CPropertyConfigDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT1, m_VertexCount);
	DDX_Text(pDX, IDC_EDIT7, m_TriangleCount);
	DDX_Control(pDX, IDC_VERTEXT_LIST, m_lvVertexList);
	DDX_Control(pDX, IDC_RADIO_VERTEX_MODE, m_rdVertexMode);
	DDX_Control(pDX, IDC_RADIO_TRIANGLE_MODE, m_rdTriangleMode);
}


BEGIN_MESSAGE_MAP(CDlgTrianglesViewer, CPropertyConfigDialog)
	ON_BN_CLICKED(IDC_RADIO_VERTEX_MODE, &CDlgTrianglesViewer::OnBnClickedRadioVertexMode)
	ON_BN_CLICKED(IDC_RADIO_TRIANGLE_MODE, &CDlgTrianglesViewer::OnBnClickedRadioTriangleMode)
	ON_BN_CLICKED(IDC_BUTTON_MODIFY, &CDlgTrianglesViewer::OnBnClickedButtonModify)
	ON_NOTIFY(NM_CLICK, IDC_VERTEXT_LIST, &CDlgTrianglesViewer::OnNMClickVertextList)
END_MESSAGE_MAP()


// CDlgTrianglesViewer 消息处理程序

void CDlgTrianglesViewer::Init(CD3DSubMesh * pSubMesh)
{
	m_pSubMesh=pSubMesh;
	m_VertexCount=m_pSubMesh->GetVertexCount();
	m_TriangleCount=m_pSubMesh->GetPrimitiveCount();
	m_rdVertexMode.SetCheck(BST_CHECKED);
	m_rdTriangleMode.SetCheck(BST_UNCHECKED);
	m_ViewMode=VIEW_MODE_VERTEX;
	ParseVertexFormat();
	MakeColumns();
	FillList();
	UpdateData(false);
}
void CDlgTrianglesViewer::OnBnClickedRadioVertexMode()
{
	// TODO: 在此添加控件通知处理程序代码
	if(m_ViewMode!=VIEW_MODE_VERTEX)
	{
		m_ViewMode=VIEW_MODE_VERTEX;
		FillList();
	}
}

void CDlgTrianglesViewer::OnBnClickedRadioTriangleMode()
{
	// TODO: 在此添加控件通知处理程序代码
	if(m_ViewMode!=VIEW_MODE_TRIANGLE)
	{
		m_ViewMode=VIEW_MODE_TRIANGLE;
		FillList();
	}	
}


void CDlgTrianglesViewer::FillList()
{	
	m_lvVertexList.DeleteAllItems();
	if(m_ViewMode==VIEW_MODE_VERTEX)
	{
		BYTE * pVertexData=NULL;
		if(m_pSubMesh->GetVertexBuffer())
		{
			pVertexData=m_pSubMesh->GetVertexBuffer();
		}
		else if(m_pSubMesh->GetDXVertexBuffer())
		{
			m_pSubMesh->GetDXVertexBuffer()->Lock(0,0,(LPVOID *)&pVertexData,0);
		}

		if(pVertexData)
		{
			m_lvVertexList.SetRedraw(false);
			for(UINT i=0;i<m_pSubMesh->GetVertexCount();i++)
			{
				InsertLine(i,pVertexData+(i+m_pSubMesh->GetVertexStart())*m_pSubMesh->GetVertexFormat().VertexSize,
					m_pSubMesh->GetVertexFormat().VertexSize);
			}
			m_lvVertexList.SetRedraw(true);
		}

		if(m_pSubMesh->GetDXVertexBuffer()&&pVertexData)
		{
			m_pSubMesh->GetDXVertexBuffer()->Unlock();
		}
		
	}
	else
	{
		if(m_pSubMesh->GetVertexFormat().IndexSize==2)
		{
			WORD * pIndexData=NULL;
			BYTE * pVertexData=NULL;
			if(m_pSubMesh->GetIndexBuffer())
			{
				pIndexData=(WORD *)m_pSubMesh->GetIndexBuffer();
			}
			else if(m_pSubMesh->GetDXIndexBuffer())
			{
				m_pSubMesh->GetDXIndexBuffer()->Lock(0,0,(LPVOID *)&pIndexData,0);
			}
			if(m_pSubMesh->GetVertexBuffer())
			{
				pVertexData=m_pSubMesh->GetVertexBuffer();
			}
			else if(m_pSubMesh->GetDXVertexBuffer())
			{
				m_pSubMesh->GetDXVertexBuffer()->Lock(0,0,(LPVOID *)&pVertexData,0);
			}

			if(pVertexData&&pIndexData)
			{
				m_lvVertexList.SetRedraw(false);
				for(UINT i=0;i<m_pSubMesh->GetIndexCount();i++)
				{
					InsertLine(i,pVertexData+pIndexData[i+m_pSubMesh->GetIndexStart()]*m_pSubMesh->GetVertexFormat().VertexSize,
						m_pSubMesh->GetVertexFormat().VertexSize);
				}
				m_lvVertexList.SetRedraw(true);
			}

			if(m_pSubMesh->GetDXVertexBuffer()&&pVertexData)
			{
				m_pSubMesh->GetDXVertexBuffer()->Unlock();
			}
			if(m_pSubMesh->GetDXIndexBuffer()&&pIndexData)
			{
				m_pSubMesh->GetDXIndexBuffer()->Unlock();
			}
		}
		else
		{

		}
	}
}

void CDlgTrianglesViewer::MakeColumns()
{
	CEasyString Temp;
	UINT ColumnCount=m_lvVertexList.GetHeaderCtrl()->GetItemCount();
	for(UINT i=0;i<ColumnCount;i++)
		m_lvVertexList.DeleteColumn(0);
	int ColCount=0;
	m_lvVertexList.InsertColumn(ColCount++,_T("序号"),LVCFMT_LEFT,50);
	for(UINT i=0;i<m_VertextFormat.GetCount();i++)
	{
		switch(m_VertextFormat[i].Type)
		{
		case VE_POS:
			m_lvVertexList.InsertColumn(ColCount++,_T("X"),LVCFMT_LEFT,60);
			m_lvVertexList.InsertColumn(ColCount++,_T("Y"),LVCFMT_LEFT,60);
			m_lvVertexList.InsertColumn(ColCount++,_T("Z"),LVCFMT_LEFT,60);
			break;
		case VE_POSW:
			m_lvVertexList.InsertColumn(ColCount++,_T("X"),LVCFMT_LEFT,60);
			m_lvVertexList.InsertColumn(ColCount++,_T("Y"),LVCFMT_LEFT,60);
			m_lvVertexList.InsertColumn(ColCount++,_T("Z"),LVCFMT_LEFT,60);
			m_lvVertexList.InsertColumn(ColCount++,_T("W"),LVCFMT_LEFT,60);
			break;
		case VE_FLOAT:
			m_lvVertexList.InsertColumn(ColCount++,_T("Float"),LVCFMT_LEFT,60);
			break;
		case VE_FLOAT2:
			m_lvVertexList.InsertColumn(ColCount++,_T("FloatX"),LVCFMT_LEFT,60);
			m_lvVertexList.InsertColumn(ColCount++,_T("FloatY"),LVCFMT_LEFT,60);
			break;
		case VE_FLOAT3:
			m_lvVertexList.InsertColumn(ColCount++,_T("FloatX"),LVCFMT_LEFT,60);
			m_lvVertexList.InsertColumn(ColCount++,_T("FloatY"),LVCFMT_LEFT,60);
			m_lvVertexList.InsertColumn(ColCount++,_T("FloatZ"),LVCFMT_LEFT,60);
			break;
		case VE_FLOAT4:
			m_lvVertexList.InsertColumn(ColCount++,_T("FloatX"),LVCFMT_LEFT,60);
			m_lvVertexList.InsertColumn(ColCount++,_T("FloatY"),LVCFMT_LEFT,60);
			m_lvVertexList.InsertColumn(ColCount++,_T("FloatZ"),LVCFMT_LEFT,60);
			m_lvVertexList.InsertColumn(ColCount++,_T("FloatW"),LVCFMT_LEFT,60);
			break;
		case VE_UBYTE4:
			m_lvVertexList.InsertColumn(ColCount++,_T("UB1"),LVCFMT_LEFT,60);
			m_lvVertexList.InsertColumn(ColCount++,_T("UB2"),LVCFMT_LEFT,60);
			m_lvVertexList.InsertColumn(ColCount++,_T("UB3"),LVCFMT_LEFT,60);
			m_lvVertexList.InsertColumn(ColCount++,_T("UB4"),LVCFMT_LEFT,60);
			break;
		case VE_NORMAL:
			m_lvVertexList.InsertColumn(ColCount++,_T("NormalX"),LVCFMT_LEFT,60);
			m_lvVertexList.InsertColumn(ColCount++,_T("NormalY"),LVCFMT_LEFT,60);
			m_lvVertexList.InsertColumn(ColCount++,_T("NormalZ"),LVCFMT_LEFT,60);
			break;
		case VE_PSIZE:
			m_lvVertexList.InsertColumn(ColCount++,_T("PointSize"),LVCFMT_LEFT,60);	
			break;
		case VE_DIFFUSE:
			m_lvVertexList.InsertColumn(ColCount++,_T("DiffuseB"),LVCFMT_LEFT,60);
			m_lvVertexList.InsertColumn(ColCount++,_T("DiffuseG"),LVCFMT_LEFT,60);
			m_lvVertexList.InsertColumn(ColCount++,_T("DiffuseR"),LVCFMT_LEFT,60);
			m_lvVertexList.InsertColumn(ColCount++,_T("DiffuseA"),LVCFMT_LEFT,60);		
			break;
		case VE_SPECULAR:
			m_lvVertexList.InsertColumn(ColCount++,_T("SpecularB"),LVCFMT_LEFT,60);				
			m_lvVertexList.InsertColumn(ColCount++,_T("SpecularG"),LVCFMT_LEFT,60);
			m_lvVertexList.InsertColumn(ColCount++,_T("SpecularR"),LVCFMT_LEFT,60);			
			m_lvVertexList.InsertColumn(ColCount++,_T("SpecularA"),LVCFMT_LEFT,60);
			break;
		case VE_TEX:
			Temp.Format(_T("Tex%dX"),m_VertextFormat[i].Index);
			m_lvVertexList.InsertColumn(ColCount++,Temp,LVCFMT_LEFT,60);
			Temp.Format(_T("Tex%dY"),m_VertextFormat[i].Index);
			m_lvVertexList.InsertColumn(ColCount++,Temp,LVCFMT_LEFT,60);
			break;
		}
	}	
}
BOOL CDlgTrianglesViewer::OnInitDialog()
{
	CPropertyConfigDialog::OnInitDialog();

	// TODO:  在此添加额外的初始化
	m_lvVertexList.SetExtendedStyle(LVS_EX_FULLROWSELECT|LVS_EX_GRIDLINES);
	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}

void CDlgTrianglesViewer::InsertLine(int Index,BYTE * pVertexData,UINT VertexSize)
{
	CEasyString Temp;
	Temp.Format(_T("%d"),Index);
	int Item=m_lvVertexList.InsertItem(Index,Temp);
	int SubItem=1;
	for(UINT i=0;i<m_VertextFormat.GetCount();i++)
	{
		switch(m_VertextFormat[i].Type)
		{
		case VE_POS:
		case VE_FLOAT3:
		case VE_NORMAL:
			if(VertexSize>=sizeof(CD3DVector3))
			{
				CD3DVector3 * pPos=(CD3DVector3 *)pVertexData;
				Temp.Format(_T("%g"),pPos->x);
				m_lvVertexList.SetItemText(Item,SubItem++,Temp);
				Temp.Format(_T("%g"),pPos->y);
				m_lvVertexList.SetItemText(Item,SubItem++,Temp);
				Temp.Format(_T("%g"),pPos->z);
				m_lvVertexList.SetItemText(Item,SubItem++,Temp);
				pVertexData+=sizeof(CD3DVector3);
				VertexSize-=sizeof(CD3DVector3);
			}
			break;
		case VE_POSW:
		case VE_FLOAT4:
			if(VertexSize>=sizeof(CD3DVector4))
			{
				CD3DVector4 * pPos=(CD3DVector4 *)pVertexData;
				Temp.Format(_T("%g"),pPos->x);
				m_lvVertexList.SetItemText(Item,SubItem++,Temp);
				Temp.Format(_T("%g"),pPos->y);
				m_lvVertexList.SetItemText(Item,SubItem++,Temp);
				Temp.Format(_T("%g"),pPos->z);
				m_lvVertexList.SetItemText(Item,SubItem++,Temp);
				Temp.Format(_T("%g"),pPos->w);
				m_lvVertexList.SetItemText(Item,SubItem++,Temp);
				pVertexData+=sizeof(CD3DVector4);
				VertexSize-=sizeof(CD3DVector4);
			}
			break;
		case VE_FLOAT:
		case VE_PSIZE:
			if(VertexSize>=sizeof(FLOAT))
			{
				FLOAT * pData=(FLOAT *)pVertexData;
				Temp.Format(_T("%g"),*pData);
				m_lvVertexList.SetItemText(Item,SubItem++,Temp);
				pVertexData+=sizeof(FLOAT);
				VertexSize-=sizeof(FLOAT);
			}
			break;
		case VE_FLOAT2:		
		case VE_TEX:
			if(VertexSize>=sizeof(CD3DVector2))
			{
				CD3DVector2 * pPos=(CD3DVector2 *)pVertexData;
				Temp.Format(_T("%g"),pPos->x);
				m_lvVertexList.SetItemText(Item,SubItem++,Temp);
				Temp.Format(_T("%g"),pPos->y);
				m_lvVertexList.SetItemText(Item,SubItem++,Temp);		
				pVertexData+=sizeof(CD3DVector2);
				VertexSize-=sizeof(CD3DVector2);
			}
			break;
		case VE_UBYTE4:
		case VE_DIFFUSE:
		case VE_SPECULAR:
			if(VertexSize>=sizeof(BYTE)*4)
			{
				BYTE * pData=(BYTE *)pVertexData;
				Temp.Format(_T("%d"),pData[0]);
				m_lvVertexList.SetItemText(Item,SubItem++,Temp);
				Temp.Format(_T("%d"),pData[1]);
				m_lvVertexList.SetItemText(Item,SubItem++,Temp);
				Temp.Format(_T("%d"),pData[2]);
				m_lvVertexList.SetItemText(Item,SubItem++,Temp);
				Temp.Format(_T("%d"),pData[3]);
				m_lvVertexList.SetItemText(Item,SubItem++,Temp);
				pVertexData+=sizeof(BYTE)*4;
				VertexSize-=sizeof(BYTE)*4;
			}
			break;
		}
	}	
}

void CDlgTrianglesViewer::ParseVertexFormat()
{
	m_VertextFormat.Clear();
	DWORD FVF=m_pSubMesh->GetVertexFormat().FVF;
	DWORD FVFPos=FVF&D3DFVF_POSITION_MASK;
	int ColCount=0;
	VERTEX_FORMAT FormatInfo;
	switch(FVFPos)
	{
	case D3DFVF_XYZ:
		FormatInfo.Type=VE_POS;
		m_VertextFormat.Add(FormatInfo);
		break;
	case D3DFVF_XYZRHW:
		FormatInfo.Type=VE_POSW;
		m_VertextFormat.Add(FormatInfo);
		break;
	case D3DFVF_XYZB1:
		FormatInfo.Type=VE_POS;
		m_VertextFormat.Add(FormatInfo);
		if(FVF&D3DFVF_LASTBETA_UBYTE4)
		{
			FormatInfo.Type=VE_UBYTE4;
			m_VertextFormat.Add(FormatInfo);			
		}
		else
		{
			FormatInfo.Type=VE_FLOAT;
			m_VertextFormat.Add(FormatInfo);
		}
		break;
	case D3DFVF_XYZB2:
		FormatInfo.Type=VE_POS;
		m_VertextFormat.Add(FormatInfo);
		if(FVF&D3DFVF_LASTBETA_UBYTE4)
		{
			FormatInfo.Type=VE_FLOAT;
			m_VertextFormat.Add(FormatInfo);
			FormatInfo.Type=VE_UBYTE4;
			m_VertextFormat.Add(FormatInfo);
		}
		else
		{			
			FormatInfo.Type=VE_FLOAT;
			m_VertextFormat.Add(FormatInfo);
		}
		break;
	case D3DFVF_XYZB3:
		FormatInfo.Type=VE_POS;
		m_VertextFormat.Add(FormatInfo);
		if(FVF&D3DFVF_LASTBETA_UBYTE4)
		{
			FormatInfo.Type=VE_FLOAT2;
			m_VertextFormat.Add(FormatInfo);
			FormatInfo.Type=VE_UBYTE4;
			m_VertextFormat.Add(FormatInfo);
		}
		else
		{			
			FormatInfo.Type=VE_FLOAT3;
			m_VertextFormat.Add(FormatInfo);
		}
		break;
	case D3DFVF_XYZB4:
		FormatInfo.Type=VE_POS;
		m_VertextFormat.Add(FormatInfo);
		if(FVF&D3DFVF_LASTBETA_UBYTE4)
		{
			FormatInfo.Type=VE_FLOAT3;
			m_VertextFormat.Add(FormatInfo);
			FormatInfo.Type=VE_UBYTE4;
			m_VertextFormat.Add(FormatInfo);
		}
		else
		{			
			FormatInfo.Type=VE_FLOAT4;
			m_VertextFormat.Add(FormatInfo);
		}
		break;
	case D3DFVF_XYZB5:
		FormatInfo.Type=VE_POS;
		m_VertextFormat.Add(FormatInfo);
		if(FVF&D3DFVF_LASTBETA_UBYTE4)
		{
			FormatInfo.Type=VE_FLOAT4;
			m_VertextFormat.Add(FormatInfo);
			FormatInfo.Type=VE_UBYTE4;
			m_VertextFormat.Add(FormatInfo);
		}
		else
		{			
			FormatInfo.Type=VE_FLOAT4;
			m_VertextFormat.Add(FormatInfo);
			FormatInfo.Type=VE_FLOAT;
			m_VertextFormat.Add(FormatInfo);
		}
		break;
	case D3DFVF_XYZW:
		FormatInfo.Type=VE_POSW;
		m_VertextFormat.Add(FormatInfo);
		break;
	}
	if(FVF&D3DFVF_NORMAL)
	{
		FormatInfo.Type=VE_NORMAL;
		m_VertextFormat.Add(FormatInfo);
	}
	if(FVF&D3DFVF_PSIZE)
	{
		FormatInfo.Type=VE_PSIZE;
		m_VertextFormat.Add(FormatInfo);
	}
	if(FVF&D3DFVF_DIFFUSE)
	{
		FormatInfo.Type=VE_DIFFUSE;
		m_VertextFormat.Add(FormatInfo);	
	}
	if(FVF&D3DFVF_SPECULAR)
	{
		FormatInfo.Type=VE_SPECULAR;
		m_VertextFormat.Add(FormatInfo);
	}
	UINT TexCount=(FVF&D3DFVF_TEXCOUNT_MASK)>>D3DFVF_TEXCOUNT_SHIFT;
	for(UINT i=0;i<TexCount;i++)
	{
		FormatInfo.Type=VE_TEX;
		FormatInfo.Index=i;
		m_VertextFormat.Add(FormatInfo);	
	}
}
void CDlgTrianglesViewer::OnBnClickedButtonModify()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(true);
	if(m_TriangleCount<=m_pSubMesh->GetIndexCount()/3)
	{
		m_pSubMesh->SetPrimitiveCount(m_TriangleCount);
	}
}

void CDlgTrianglesViewer::OnNMClickVertextList(NMHDR *pNMHDR, LRESULT *pResult)
{
	//LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<NMITEMACTIVATE>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码
	*pResult = 0;
}
