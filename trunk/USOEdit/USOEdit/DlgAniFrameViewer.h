/****************************************************************************/
/*                                                                          */
/*      �ļ���:    DlgAniFrameViewer.h                                      */
/*      ��������:  2011��10��07��                                           */
/*      ����:      Sagasarate                                               */
/*                                                                          */
/*      �������Ȩ��Sagasarate(sagasarate@sina.com)����                     */
/*      ����Խ�����������κ���ҵ�ͷ���ҵ�����������                      */
/*      ���뱣���˰�Ȩ����                                                  */
/*                                                                          */
/****************************************************************************/
#pragma once
#include "afxcmn.h"


// CDlgAniFrameViewer �Ի���

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
	CDlgAniFrameViewer(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CDlgAniFrameViewer();

// �Ի�������
	enum { IDD = IDD_DIALOG_ANI_FRAME_VIEW };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:	
	virtual BOOL OnInitDialog();

	void InitTransAniFrames(ANIMATION_TRANSLATION_BLOCK& Trans,ANIMATION_ROTATION_BLOCK& Rotation,ANIMATION_SCALING_BLOCK& Scale);
	void InitParticleParams(CD3DParticleEmitterResource::PARTICLE_EMITTER_INFO* ParticleInfo);
	afx_msg void OnLvnItemchangedListAni(NMHDR *pNMHDR, LRESULT *pResult);
};
