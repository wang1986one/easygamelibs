/****************************************************************************/
/*                                                                          */
/*      �ļ���:    DlgParticleEmitterParamViewer.h                          */
/*      ��������:  2011��10��07��                                           */
/*      ����:      Sagasarate                                               */
/*                                                                          */
/*      �������Ȩ��Sagasarate(sagasarate@sina.com)����                     */
/*      ����Խ�����������κ���ҵ�ͷ���ҵ�����������                      */
/*      ���뱣���˰�Ȩ����                                                  */
/*                                                                          */
/****************************************************************************/
#pragma once
#include "afxwin.h"


// CDlgParticleEmitterParamViewer �Ի���

class CDlgParticleEmitterParamViewer : public CPropertyConfigDialog
{
	DECLARE_DYNAMIC(CDlgParticleEmitterParamViewer)
protected:
	CD3DParticleEmitterResource *	m_pModelResource;
	CString							m_Flag;
	CComboBox						m_cbEmitterType;
	float							m_SlowDown;
	CComboBox						m_cbTextureRotation;
	UINT							m_TextureCols;
	UINT							m_TextureRows;
	float							m_ParticleRotation;
	CString							m_FlagDescription;
	CDlgAniFrameViewer				m_DlgAniFrameViewer;

public:
	CDlgParticleEmitterParamViewer(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CDlgParticleEmitterParamViewer();

// �Ի�������
	enum { IDD = IDD_DIALOG_PARTICLE_EMITTER_PARAM_VIEWER };

	void Init(CD3DParticleEmitterResource * pModelResource);

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	
	
	
	afx_msg void OnBnClickedViewParams();
};
