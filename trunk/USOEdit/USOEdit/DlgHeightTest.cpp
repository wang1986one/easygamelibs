/****************************************************************************/
/*                                                                          */
/*      文件名:    DlgHeightTest.cpp                                        */
/*      创建日期:  2011年10月07日                                           */
/*      作者:      Sagasarate                                               */
/*                                                                          */
/*      本软件版权归Sagasarate(sagasarate@sina.com)所有                     */
/*      你可以将本软件用于任何商业和非商业软件开发，但                      */
/*      必须保留此版权声明                                                  */
/*                                                                          */
/****************************************************************************/
// DlgHeightTest.cpp : 实现文件
//

#include "stdafx.h"




// CDlgHeightTest 对话框

IMPLEMENT_DYNAMIC(CDlgHeightTest, CDialog)

CDlgHeightTest::CDlgHeightTest(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgHeightTest::IDD, pParent)
	, m_PosX(0)
	, m_PosZ(0)
	, m_PosY(0)
	, m_Height(0)
{
	m_pViewPort=NULL;
}

CDlgHeightTest::~CDlgHeightTest()
{
}

void CDlgHeightTest::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_POS_X, m_PosX);
	DDX_Text(pDX, IDC_EDIT_POS_Z, m_PosZ);
	DDX_Text(pDX, IDC_EDIT_POS_Y, m_PosY);
	DDX_Text(pDX, IDC_EDIT_HEIGHT, m_Height);
}


BEGIN_MESSAGE_MAP(CDlgHeightTest, CDialog)
	ON_BN_CLICKED(IDC_DO_TEST, &CDlgHeightTest::OnBnClickedDoTest)
END_MESSAGE_MAP()


// CDlgHeightTest 消息处理程序

void CDlgHeightTest::OnBnClickedDoTest()
{
	// TODO: 在此添加控件通知处理程序代码
	if(m_pViewPort)
	{
		UpdateData(true);

		CD3DWOWWMOModel * pModel=(CD3DWOWWMOModel *)m_pViewPort->GetSelectObject();
		if(pModel&&pModel->IsKindOf(GET_CLASS_INFO(CD3DWOWWMOModel)))
		{
			FLOAT Height,WaterHeight;
			CD3DVector3 Pos;
			CD3DBoundingBox BBox=(*pModel->GetBoundingBox())*pModel->GetWorldMatrix();
			Pos.x=m_PosX;
			Pos.z=m_PosZ;
			Pos.y=BBox.m_Max.y;
			m_PosY=Pos.y;
			if(pModel->GetHeightByXZ(Pos,BBox.m_Min.y,BBox.m_Max.y,Height,WaterHeight))
			{
				m_Height=Height;
			}
			else
			{
				AfxMessageBox(_T("高度测试未命中"));
			}
		}
		UpdateData(false);
	}
}
