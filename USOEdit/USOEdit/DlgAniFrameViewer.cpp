/****************************************************************************/
/*                                                                          */
/*      文件名:    DlgAniFrameViewer.cpp                                    */
/*      创建日期:  2011年10月07日                                           */
/*      作者:      Sagasarate                                               */
/*                                                                          */
/*      本软件版权归Sagasarate(sagasarate@sina.com)所有                     */
/*      你可以将本软件用于任何商业和非商业软件开发，但                      */
/*      必须保留此版权声明                                                  */
/*                                                                          */
/****************************************************************************/
// DlgAniFrameViewer.cpp : 实现文件
//

#include "stdafx.h"
#include "DlgAniFrameViewer.h"




// CDlgAniFrameViewer 对话框

IMPLEMENT_DYNAMIC(CDlgAniFrameViewer, CDialog)

CDlgAniFrameViewer::CDlgAniFrameViewer(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgAniFrameViewer::IDD, pParent)
	, m_GlobalSequenceID(0)
	, m_InterpolationType(_T(""))
{

}

CDlgAniFrameViewer::~CDlgAniFrameViewer()
{
}

void CDlgAniFrameViewer::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_ANI, m_lvAnis);
	DDX_Control(pDX, IDC_LIST_FRAME, m_lvFrames);
	DDX_Text(pDX, IDC_EDIT_GLOBAL_SEQUENCE_ID, m_GlobalSequenceID);
	DDX_Text(pDX, IDC_EDIT_INTERPOLATION_TYPE, m_InterpolationType);
}


BEGIN_MESSAGE_MAP(CDlgAniFrameViewer, CDialog)
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_LIST_ANI, &CDlgAniFrameViewer::OnLvnItemchangedListAni)
END_MESSAGE_MAP()


// CDlgAniFrameViewer 消息处理程序

BOOL CDlgAniFrameViewer::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  在此添加额外的初始化
	m_lvAnis.SetExtendedStyle(LVS_EX_FULLROWSELECT|LVS_EX_GRIDLINES);

	m_lvAnis.InsertColumn(0,_T("Index"),LVCFMT_LEFT,50);
	m_lvAnis.InsertColumn(1,_T("类型"),LVCFMT_LEFT,80);
	m_lvAnis.InsertColumn(2,_T("帧数"),LVCFMT_LEFT,60);
	m_lvAnis.InsertColumn(3,_T("全局序列ID"),LVCFMT_LEFT,80);
	m_lvAnis.InsertColumn(4,_T("插值方式"),LVCFMT_LEFT,70);

	m_lvFrames.SetExtendedStyle(LVS_EX_FULLROWSELECT|LVS_EX_GRIDLINES);

	m_lvFrames.InsertColumn(0,_T("时间"),LVCFMT_LEFT,50);
	m_lvFrames.InsertColumn(1,_T("帧"),LVCFMT_LEFT,150);


	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}

void CDlgAniFrameViewer::InitTransAniFrames(ANIMATION_TRANSLATION_BLOCK& Trans,ANIMATION_ROTATION_BLOCK& Rotation,ANIMATION_SCALING_BLOCK& Scale)
{
	m_lvAnis.DeleteAllItems();
	m_lvFrames.DeleteAllItems();
	m_FrameDatas.Clear();

	UINT AniCount=Trans.Animations.GetCount();
	if(AniCount<Rotation.Animations.GetCount())
		AniCount=Rotation.Animations.GetCount();
	if(AniCount<Scale.Animations.GetCount())
		AniCount=Scale.Animations.GetCount();

	for(UINT i=0;i<AniCount;i++)
	{
		if(i<Trans.Animations.GetCount())
		{
			if(Trans.Animations[i].TimeStamps.GetCount())
			{
				CEasyString Temp;

				Temp.Format(_T("%d"),i);
				int Item=m_lvAnis.InsertItem(m_lvAnis.GetItemCount(),Temp);
				m_lvAnis.SetItemText(Item,1,_T("位移"));
				Temp.Format(_T("%d"),Trans.Animations[i].TimeStamps.GetCount());
				m_lvAnis.SetItemText(Item,2,Temp);
				Temp.Format(_T("%d"),Trans.GlobalSequenceID);
				m_lvAnis.SetItemText(Item,3,Temp);
				switch(Trans.InterpolationType)
				{
				case INTERPOLATE_TYPE_LINEAR:
					Temp=_T("线性");
					break;
				case INTERPOLATE_TYPE_HERMITE:
					Temp=_T("样条");
					break;
				default:
					Temp=_T("无");
				}
				m_lvAnis.SetItemText(Item,4,Temp);				
				m_lvAnis.SetItemData(Item,m_FrameDatas.GetCount());
				FRAME_DATA FrameData;
				FrameData.Type=FRAME_DATA_TYPE_VEC3;
				FrameData.pData=&(Trans.Animations[i]);
				m_FrameDatas.Add(FrameData);
			}
		}
		if(i<Rotation.Animations.GetCount())
		{
			if(Rotation.Animations[i].TimeStamps.GetCount())
			{
				CEasyString Temp;

				Temp.Format(_T("%d"),i);
				int Item=m_lvAnis.InsertItem(m_lvAnis.GetItemCount(),Temp);
				m_lvAnis.SetItemText(Item,1,_T("旋转"));
				Temp.Format(_T("%d"),Rotation.Animations[i].TimeStamps.GetCount());
				m_lvAnis.SetItemText(Item,2,Temp);
				Temp.Format(_T("%d"),Rotation.GlobalSequenceID);
				m_lvAnis.SetItemText(Item,3,Temp);
				switch(Rotation.InterpolationType)
				{
				case INTERPOLATE_TYPE_LINEAR:
					Temp=_T("线性");
					break;
				case INTERPOLATE_TYPE_HERMITE:
					Temp=_T("样条");
					break;
				default:
					Temp=_T("无");
				}
				m_lvAnis.SetItemText(Item,4,Temp);
				m_lvAnis.SetItemData(Item,m_FrameDatas.GetCount());
				FRAME_DATA FrameData;
				FrameData.Type=FRAME_DATA_TYPE_VEC4;
				FrameData.pData=&(Rotation.Animations[i]);
				m_FrameDatas.Add(FrameData);
			}
		}
		if(i<Scale.Animations.GetCount())
		{
			if(Scale.Animations[i].TimeStamps.GetCount())
			{
				CEasyString Temp;

				Temp.Format(_T("%d"),i);
				int Item=m_lvAnis.InsertItem(m_lvAnis.GetItemCount(),Temp);
				m_lvAnis.SetItemText(Item,1,_T("缩放"));
				Temp.Format(_T("%d"),Scale.Animations[i].TimeStamps.GetCount());
				m_lvAnis.SetItemText(Item,2,Temp);
				Temp.Format(_T("%d"),Scale.GlobalSequenceID);
				m_lvAnis.SetItemText(Item,3,Temp);
				switch(Scale.InterpolationType)
				{
				case INTERPOLATE_TYPE_LINEAR:
					Temp=_T("线性");
					break;
				case INTERPOLATE_TYPE_HERMITE:
					Temp=_T("样条");
					break;
				default:
					Temp=_T("无");
				}
				m_lvAnis.SetItemText(Item,4,Temp);
				m_lvAnis.SetItemData(Item,m_FrameDatas.GetCount());
				FRAME_DATA FrameData;
				FrameData.Type=FRAME_DATA_TYPE_VEC3;
				FrameData.pData=&(Scale.Animations[i]);
				m_FrameDatas.Add(FrameData);
			}
		}
	}
}

void CDlgAniFrameViewer::InitParticleParams(CD3DParticleEmitterResource::PARTICLE_EMITTER_INFO* ParticleInfo)
{
	m_lvAnis.DeleteAllItems();
	m_lvFrames.DeleteAllItems();
	m_FrameDatas.Clear();

	for(UINT i=0;i<ParticleInfo->EmissionRate.Animations.GetCount();i++)
	{
		CEasyString Temp;

		Temp.Format(_T("%d"),i);
		int Item=m_lvAnis.InsertItem(m_lvAnis.GetItemCount(),Temp);
		m_lvAnis.SetItemText(Item,1,_T("粒子发射速率"));
		Temp.Format(_T("%d"),ParticleInfo->EmissionRate.Animations[i].TimeStamps.GetCount());
		m_lvAnis.SetItemText(Item,2,Temp);
		Temp.Format(_T("%d"),ParticleInfo->EmissionRate.GlobalSequenceID);
		m_lvAnis.SetItemText(Item,3,Temp);
		switch(ParticleInfo->EmissionRate.InterpolationType)
		{
		case INTERPOLATE_TYPE_LINEAR:
			Temp=_T("线性");
			break;
		case INTERPOLATE_TYPE_HERMITE:
			Temp=_T("样条");
			break;
		default:
			Temp=_T("无");
		}
		m_lvAnis.SetItemText(Item,4,Temp);
		m_lvAnis.SetItemData(Item,m_FrameDatas.GetCount());
		FRAME_DATA FrameData;
		FrameData.Type=FRAME_DATA_TYPE_FLOAT;
		FrameData.pData=&(ParticleInfo->EmissionRate.Animations[i]);
		m_FrameDatas.Add(FrameData);
	}
	for(UINT i=0;i<ParticleInfo->LifeSpan.Animations.GetCount();i++)
	{
		CEasyString Temp;

		Temp.Format(_T("%d"),i);
		int Item=m_lvAnis.InsertItem(m_lvAnis.GetItemCount(),Temp);
		m_lvAnis.SetItemText(Item,1,_T("生存期"));
		Temp.Format(_T("%d"),ParticleInfo->LifeSpan.Animations[i].TimeStamps.GetCount());
		m_lvAnis.SetItemText(Item,2,Temp);
		Temp.Format(_T("%d"),ParticleInfo->LifeSpan.GlobalSequenceID);
		m_lvAnis.SetItemText(Item,3,Temp);
		switch(ParticleInfo->LifeSpan.InterpolationType)
		{
		case INTERPOLATE_TYPE_LINEAR:
			Temp=_T("线性");
			break;
		case INTERPOLATE_TYPE_HERMITE:
			Temp=_T("样条");
			break;
		default:
			Temp=_T("无");
		}
		m_lvAnis.SetItemText(Item,4,Temp);
		m_lvAnis.SetItemData(Item,m_FrameDatas.GetCount());
		FRAME_DATA FrameData;
		FrameData.Type=FRAME_DATA_TYPE_FLOAT;
		FrameData.pData=&(ParticleInfo->LifeSpan.Animations[i]);
		m_FrameDatas.Add(FrameData);
	}
	for(UINT i=0;i<ParticleInfo->EmissionAreaLength.Animations.GetCount();i++)
	{
		CEasyString Temp;

		Temp.Format(_T("%d"),i);
		int Item=m_lvAnis.InsertItem(m_lvAnis.GetItemCount(),Temp);
		m_lvAnis.SetItemText(Item,1,_T("发射区长度"));
		Temp.Format(_T("%d"),ParticleInfo->EmissionAreaLength.Animations[i].TimeStamps.GetCount());
		m_lvAnis.SetItemText(Item,2,Temp);
		Temp.Format(_T("%d"),ParticleInfo->EmissionAreaLength.GlobalSequenceID);
		m_lvAnis.SetItemText(Item,3,Temp);
		switch(ParticleInfo->EmissionAreaLength.InterpolationType)
		{
		case INTERPOLATE_TYPE_LINEAR:
			Temp=_T("线性");
			break;
		case INTERPOLATE_TYPE_HERMITE:
			Temp=_T("样条");
			break;
		default:
			Temp=_T("无");
		}
		m_lvAnis.SetItemText(Item,4,Temp);
		m_lvAnis.SetItemData(Item,m_FrameDatas.GetCount());
		FRAME_DATA FrameData;
		FrameData.Type=FRAME_DATA_TYPE_FLOAT;
		FrameData.pData=&(ParticleInfo->EmissionAreaLength.Animations[i]);
		m_FrameDatas.Add(FrameData);
	}
	for(UINT i=0;i<ParticleInfo->EmissionAreaWidth.Animations.GetCount();i++)
	{
		CEasyString Temp;

		Temp.Format(_T("%d"),i);
		int Item=m_lvAnis.InsertItem(m_lvAnis.GetItemCount(),Temp);
		m_lvAnis.SetItemText(Item,1,_T("发射区宽度"));
		Temp.Format(_T("%d"),ParticleInfo->EmissionAreaWidth.Animations[i].TimeStamps.GetCount());
		m_lvAnis.SetItemText(Item,2,Temp);
		Temp.Format(_T("%d"),ParticleInfo->EmissionAreaWidth.GlobalSequenceID);
		m_lvAnis.SetItemText(Item,3,Temp);
		switch(ParticleInfo->EmissionAreaWidth.InterpolationType)
		{
		case INTERPOLATE_TYPE_LINEAR:
			Temp=_T("线性");
			break;
		case INTERPOLATE_TYPE_HERMITE:
			Temp=_T("样条");
			break;
		default:
			Temp=_T("无");
		}
		m_lvAnis.SetItemText(Item,4,Temp);
		m_lvAnis.SetItemData(Item,m_FrameDatas.GetCount());
		FRAME_DATA FrameData;
		FrameData.Type=FRAME_DATA_TYPE_FLOAT;
		FrameData.pData=&(ParticleInfo->EmissionAreaWidth.Animations[i]);
		m_FrameDatas.Add(FrameData);
	}
	for(UINT i=0;i<ParticleInfo->HorizontalRange.Animations.GetCount();i++)
	{
		CEasyString Temp;

		Temp.Format(_T("%d"),i);
		int Item=m_lvAnis.InsertItem(m_lvAnis.GetItemCount(),Temp);
		m_lvAnis.SetItemText(Item,1,_T("发射角水平变化范围"));
		Temp.Format(_T("%d"),ParticleInfo->HorizontalRange.Animations[i].TimeStamps.GetCount());
		m_lvAnis.SetItemText(Item,2,Temp);
		Temp.Format(_T("%d"),ParticleInfo->HorizontalRange.GlobalSequenceID);
		m_lvAnis.SetItemText(Item,3,Temp);
		switch(ParticleInfo->HorizontalRange.InterpolationType)
		{
		case INTERPOLATE_TYPE_LINEAR:
			Temp=_T("线性");
			break;
		case INTERPOLATE_TYPE_HERMITE:
			Temp=_T("样条");
			break;
		default:
			Temp=_T("无");
		}
		m_lvAnis.SetItemText(Item,4,Temp);
		m_lvAnis.SetItemData(Item,m_FrameDatas.GetCount());
		FRAME_DATA FrameData;
		FrameData.Type=FRAME_DATA_TYPE_FLOAT;
		FrameData.pData=&(ParticleInfo->HorizontalRange.Animations[i]);
		m_FrameDatas.Add(FrameData);
	}
	for(UINT i=0;i<ParticleInfo->VerticalRange.Animations.GetCount();i++)
	{
		CEasyString Temp;

		Temp.Format(_T("%d"),i);
		int Item=m_lvAnis.InsertItem(m_lvAnis.GetItemCount(),Temp);
		m_lvAnis.SetItemText(Item,1,_T("发射角垂直变化范围"));
		Temp.Format(_T("%d"),ParticleInfo->VerticalRange.Animations[i].TimeStamps.GetCount());
		m_lvAnis.SetItemText(Item,2,Temp);
		Temp.Format(_T("%d"),ParticleInfo->VerticalRange.GlobalSequenceID);
		m_lvAnis.SetItemText(Item,3,Temp);
		switch(ParticleInfo->VerticalRange.InterpolationType)
		{
		case INTERPOLATE_TYPE_LINEAR:
			Temp=_T("线性");
			break;
		case INTERPOLATE_TYPE_HERMITE:
			Temp=_T("样条");
			break;
		default:
			Temp=_T("无");
		}
		m_lvAnis.SetItemText(Item,4,Temp);
		m_lvAnis.SetItemData(Item,m_FrameDatas.GetCount());
		FRAME_DATA FrameData;
		FrameData.Type=FRAME_DATA_TYPE_FLOAT;
		FrameData.pData=&(ParticleInfo->VerticalRange.Animations[i]);
		m_FrameDatas.Add(FrameData);
	}
	for(UINT i=0;i<ParticleInfo->SpeedVariation.Animations.GetCount();i++)
	{
		CEasyString Temp;

		Temp.Format(_T("%d"),i);
		int Item=m_lvAnis.InsertItem(m_lvAnis.GetItemCount(),Temp);
		m_lvAnis.SetItemText(Item,1,_T("加速度"));
		Temp.Format(_T("%d"),ParticleInfo->SpeedVariation.Animations[i].TimeStamps.GetCount());
		m_lvAnis.SetItemText(Item,2,Temp);
		Temp.Format(_T("%d"),ParticleInfo->SpeedVariation.GlobalSequenceID);
		m_lvAnis.SetItemText(Item,3,Temp);
		switch(ParticleInfo->SpeedVariation.InterpolationType)
		{
		case INTERPOLATE_TYPE_LINEAR:
			Temp=_T("线性");
			break;
		case INTERPOLATE_TYPE_HERMITE:
			Temp=_T("样条");
			break;
		default:
			Temp=_T("无");
		}
		m_lvAnis.SetItemText(Item,4,Temp);
		m_lvAnis.SetItemData(Item,m_FrameDatas.GetCount());
		FRAME_DATA FrameData;
		FrameData.Type=FRAME_DATA_TYPE_FLOAT;
		FrameData.pData=&(ParticleInfo->SpeedVariation.Animations[i]);
		m_FrameDatas.Add(FrameData);
	}
	for(UINT i=0;i<ParticleInfo->Decelerate.Animations.GetCount();i++)
	{
		CEasyString Temp;

		Temp.Format(_T("%d"),i);
		int Item=m_lvAnis.InsertItem(m_lvAnis.GetItemCount(),Temp);
		m_lvAnis.SetItemText(Item,1,_T("减速度"));
		Temp.Format(_T("%d"),ParticleInfo->Decelerate.Animations[i].TimeStamps.GetCount());
		m_lvAnis.SetItemText(Item,2,Temp);
		Temp.Format(_T("%d"),ParticleInfo->Decelerate.GlobalSequenceID);
		m_lvAnis.SetItemText(Item,3,Temp);
		switch(ParticleInfo->Decelerate.InterpolationType)
		{
		case INTERPOLATE_TYPE_LINEAR:
			Temp=_T("线性");
			break;
		case INTERPOLATE_TYPE_HERMITE:
			Temp=_T("样条");
			break;
		default:
			Temp=_T("无");
		}
		m_lvAnis.SetItemText(Item,4,Temp);
		m_lvAnis.SetItemData(Item,m_FrameDatas.GetCount());
		FRAME_DATA FrameData;
		FrameData.Type=FRAME_DATA_TYPE_FLOAT;
		FrameData.pData=&(ParticleInfo->Decelerate.Animations[i]);
		m_FrameDatas.Add(FrameData);
	}
	for(UINT i=0;i<ParticleInfo->Gravity.Animations.GetCount();i++)
	{
		CEasyString Temp;

		Temp.Format(_T("%d"),i);
		int Item=m_lvAnis.InsertItem(m_lvAnis.GetItemCount(),Temp);
		m_lvAnis.SetItemText(Item,1,_T("重力"));
		Temp.Format(_T("%d"),ParticleInfo->Gravity.Animations[i].TimeStamps.GetCount());
		m_lvAnis.SetItemText(Item,2,Temp);
		Temp.Format(_T("%d"),ParticleInfo->Gravity.GlobalSequenceID);
		m_lvAnis.SetItemText(Item,3,Temp);
		switch(ParticleInfo->Gravity.InterpolationType)
		{
		case INTERPOLATE_TYPE_LINEAR:
			Temp=_T("线性");
			break;
		case INTERPOLATE_TYPE_HERMITE:
			Temp=_T("样条");
			break;
		default:
			Temp=_T("无");
		}
		m_lvAnis.SetItemText(Item,4,Temp);
		m_lvAnis.SetItemData(Item,m_FrameDatas.GetCount());
		FRAME_DATA FrameData;
		FrameData.Type=FRAME_DATA_TYPE_FLOAT;
		FrameData.pData=&(ParticleInfo->Gravity.Animations[i]);
		m_FrameDatas.Add(FrameData);
	}
	
	{
		CEasyString Temp;

		int Item=m_lvAnis.InsertItem(m_lvAnis.GetItemCount(),_T("0"));
		m_lvAnis.SetItemText(Item,1,_T("粒子颜色"));
		m_lvAnis.SetItemText(Item,2,_T("1"));		
		m_lvAnis.SetItemData(Item,m_FrameDatas.GetCount());
		FRAME_DATA FrameData;
		FrameData.Type=FRAME_DATA_TYPE_FAKE_VEC3;
		FrameData.pData=&(ParticleInfo->ParticleColor);
		m_FrameDatas.Add(FrameData);
	}

	{
		CEasyString Temp;

		int Item=m_lvAnis.InsertItem(m_lvAnis.GetItemCount(),_T("0"));
		m_lvAnis.SetItemText(Item,1,_T("粒子透明"));
		m_lvAnis.SetItemText(Item,2,_T("1"));		
		m_lvAnis.SetItemData(Item,m_FrameDatas.GetCount());
		FRAME_DATA FrameData;
		FrameData.Type=FRAME_DATA_TYPE_FAKE_FLOAT;
		FrameData.pData=&(ParticleInfo->ParticleOpacity);
		m_FrameDatas.Add(FrameData);
	}

	{
		CEasyString Temp;

		int Item=m_lvAnis.InsertItem(m_lvAnis.GetItemCount(),_T("0"));
		m_lvAnis.SetItemText(Item,1,_T("粒子大小"));
		m_lvAnis.SetItemText(Item,2,_T("1"));		
		m_lvAnis.SetItemData(Item,m_FrameDatas.GetCount());
		FRAME_DATA FrameData;
		FrameData.Type=FRAME_DATA_TYPE_FAKE_VEC2;
		FrameData.pData=&(ParticleInfo->ParticleSizes);
		m_FrameDatas.Add(FrameData);
	}

}
void CDlgAniFrameViewer::OnLvnItemchangedListAni(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码
	*pResult = 0;

	FRAME_DATA * pFrameData=m_FrameDatas.GetObject(pNMLV->lParam);
	if(pFrameData)
	{
		m_lvFrames.DeleteAllItems();
		switch(pFrameData->Type)
		{
		case FRAME_DATA_TYPE_SHORT:
			break;
		case FRAME_DATA_TYPE_FLOAT:
			{
				ANIMATION_FRAME<FLOAT> * pFrames=(ANIMATION_FRAME<FLOAT> *)pFrameData->pData;
				for(UINT i=0;i<pFrames->TimeStamps.GetCount();i++)
				{
					CEasyString Temp;
					Temp.Format(_T("%d"),pFrames->TimeStamps[i]);
					int Item=m_lvFrames.InsertItem(i,Temp);
					Temp.Format(_T("%g"),pFrames->Keys[i]);
					m_lvFrames.SetItemText(Item,1,Temp);
				}
			}
			break;
		case FRAME_DATA_TYPE_VEC2:
			break;
		case FRAME_DATA_TYPE_VEC3:
			{
				ANIMATION_FRAME<CD3DVector3> * pFrames=(ANIMATION_FRAME<CD3DVector3> *)pFrameData->pData;
				for(UINT i=0;i<pFrames->TimeStamps.GetCount();i++)
				{
					CEasyString Temp;
					Temp.Format(_T("%d"),pFrames->TimeStamps[i]);
					int Item=m_lvFrames.InsertItem(i,Temp);
					Temp.Format(_T("%g,%g,%g"),pFrames->Keys[i].x,pFrames->Keys[i].y,pFrames->Keys[i].z);
					m_lvFrames.SetItemText(Item,1,Temp);
				}
			}
			break;
		case FRAME_DATA_TYPE_VEC4:
			{
				ANIMATION_FRAME<CD3DVector4> * pFrames=(ANIMATION_FRAME<CD3DVector4> *)pFrameData->pData;
				for(UINT i=0;i<pFrames->TimeStamps.GetCount();i++)
				{
					CEasyString Temp;
					Temp.Format(_T("%d"),pFrames->TimeStamps[i]);
					int Item=m_lvFrames.InsertItem(i,Temp);
					Temp.Format(_T("%g,%g,%g,%g"),pFrames->Keys[i].x,pFrames->Keys[i].y,pFrames->Keys[i].z,pFrames->Keys[i].w);
					m_lvFrames.SetItemText(Item,1,Temp);
				}
			}
			break;
		case FRAME_DATA_TYPE_FAKE_FLOAT:
			{
				FAKE_ANIMATION_FRAME<FLOAT> * pFrames=(FAKE_ANIMATION_FRAME<FLOAT> *)pFrameData->pData;
				for(UINT i=0;i<pFrames->TimeStamps.GetCount();i++)
				{
					CEasyString Temp;
					Temp.Format(_T("%d"),pFrames->TimeStamps[i]);
					int Item=m_lvFrames.InsertItem(i,Temp);
					Temp.Format(_T("%g"),pFrames->Keys[i]);
					m_lvFrames.SetItemText(Item,1,Temp);
				}
			}
			break;
		case FRAME_DATA_TYPE_FAKE_VEC2:
			{
				FAKE_ANIMATION_FRAME<CD3DVector2> * pFrames=(FAKE_ANIMATION_FRAME<CD3DVector2> *)pFrameData->pData;
				for(UINT i=0;i<pFrames->TimeStamps.GetCount();i++)
				{
					CEasyString Temp;
					Temp.Format(_T("%d"),pFrames->TimeStamps[i]);
					int Item=m_lvFrames.InsertItem(i,Temp);
					Temp.Format(_T("%g,%g"),pFrames->Keys[i].x,pFrames->Keys[i].y);
					m_lvFrames.SetItemText(Item,1,Temp);
				}
			}
			break;
		case FRAME_DATA_TYPE_FAKE_VEC3:
			{
				FAKE_ANIMATION_FRAME<CD3DVector3> * pFrames=(FAKE_ANIMATION_FRAME<CD3DVector3> *)pFrameData->pData;
				for(UINT i=0;i<pFrames->TimeStamps.GetCount();i++)
				{
					CEasyString Temp;
					Temp.Format(_T("%d"),pFrames->TimeStamps[i]);
					int Item=m_lvFrames.InsertItem(i,Temp);
					Temp.Format(_T("%g,%g,%g"),pFrames->Keys[i].x,pFrames->Keys[i].y,pFrames->Keys[i].z);
					m_lvFrames.SetItemText(Item,1,Temp);
				}
			}
			break;
		}
	}
}
