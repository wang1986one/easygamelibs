/****************************************************************************/
/*                                                                          */
/*      文件名:    DlgAniFrameViewer.h                                      */
/*      创建日期:  2011年10月07日                                           */
/*      作者:      Sagasarate                                               */
/*                                                                          */
/*      本软件版权归Sagasarate(sagasarate@sina.com)所有                     */
/*      你可以将本软件用于任何商业和非商业软件开发，但                      */
/*      必须保留此版权声明                                                  */
/*                                                                          */
/****************************************************************************/
#pragma once
#include "afxcmn.h"


// CDlgAniFrameViewer 对话框

class CDlgAniFrameViewer : public CDialog
{
	DECLARE_DYNAMIC(CDlgAniFrameViewer)
protected:
	enum FRAME_DATA_TYPE
	{
		FRAME_DATA_TYPE_SHORT,
		FRAME_DATA_TYPE_FLOAT,
		FRAME_DATA_TYPE_VEC2,
		FRAME_DATA_TYPE_VEC3,
		FRAME_DATA_TYPE_VEC4,
		FRAME_DATA_TYPE_FAKE_FLOAT,
		FRAME_DATA_TYPE_FAKE_VEC2,
		FRAME_DATA_TYPE_FAKE_VEC3,
	};
	struct FRAME_DATA
	{
		int		Type;
		void *	pData;

	};
	CListCtrl				m_lvAnis;
	CListCtrl				m_lvFrames;
	int						m_GlobalSequenceID;
	CString					m_InterpolationType;
	CEasyArray<FRAME_DATA>	m_FrameDatas;

	

public:
	CDlgAniFrameViewer(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CDlgAniFrameViewer();

// 对话框数据
	enum { IDD = IDD_DIALOG_ANI_FRAME_VIEW };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:	
	virtual BOOL OnInitDialog();

	void InitTransAniFrames(ANIMATION_TRANSLATION_BLOCK& Trans,ANIMATION_ROTATION_BLOCK& Rotation,ANIMATION_SCALING_BLOCK& Scale);
	void InitParticleParams(CD3DParticleEmitterResource::PARTICLE_EMITTER_INFO* ParticleInfo);
	afx_msg void OnLvnItemchangedListAni(NMHDR *pNMHDR, LRESULT *pResult);
};
