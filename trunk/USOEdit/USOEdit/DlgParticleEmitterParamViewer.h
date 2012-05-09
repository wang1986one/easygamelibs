/****************************************************************************/
/*                                                                          */
/*      文件名:    DlgParticleEmitterParamViewer.h                          */
/*      创建日期:  2011年10月07日                                           */
/*      作者:      Sagasarate                                               */
/*                                                                          */
/*      本软件版权归Sagasarate(sagasarate@sina.com)所有                     */
/*      你可以将本软件用于任何商业和非商业软件开发，但                      */
/*      必须保留此版权声明                                                  */
/*                                                                          */
/****************************************************************************/
#pragma once
#include "afxwin.h"


// CDlgParticleEmitterParamViewer 对话框

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
	CDlgParticleEmitterParamViewer(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CDlgParticleEmitterParamViewer();

// 对话框数据
	enum { IDD = IDD_DIALOG_PARTICLE_EMITTER_PARAM_VIEWER };

	void Init(CD3DParticleEmitterResource * pModelResource);

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	
	
	
	afx_msg void OnBnClickedViewParams();
};
