/****************************************************************************/
/*                                                                          */
/*      �ļ���:    DlgHeightTest.cpp                                        */
/*      ��������:  2011��10��07��                                           */
/*      ����:      Sagasarate                                               */
/*                                                                          */
/*      �������Ȩ��Sagasarate(sagasarate@sina.com)����                     */
/*      ����Խ�����������κ���ҵ�ͷ���ҵ�����������                      */
/*      ���뱣���˰�Ȩ����                                                  */
/*                                                                          */
/****************************************************************************/
// DlgHeightTest.cpp : ʵ���ļ�
//

#include "stdafx.h"




// CDlgHeightTest �Ի���

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


// CDlgHeightTest ��Ϣ�������

void CDlgHeightTest::OnBnClickedDoTest()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
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
				AfxMessageBox(_T("�߶Ȳ���δ����"));
			}
		}
		UpdateData(false);
	}
}
