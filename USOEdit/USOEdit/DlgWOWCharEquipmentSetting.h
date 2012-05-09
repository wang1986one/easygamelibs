/****************************************************************************/
/*                                                                          */
/*      文件名:    DlgWOWCharEquipmentSetting.h                             */
/*      创建日期:  2011年10月07日                                           */
/*      作者:      Sagasarate                                               */
/*                                                                          */
/*      本软件版权归Sagasarate(sagasarate@sina.com)所有                     */
/*      你可以将本软件用于任何商业和非商业软件开发，但                      */
/*      必须保留此版权声明                                                  */
/*                                                                          */
/****************************************************************************/
/****************************************************************************/
/*                                                                          */
/*      文件名:    DlgWOWCharEquipmentSetting.h                             */
/*      创建日期:  2010年02月09日                                           */
/*      作者:      Sagasarate                                               */
/*                                                                          */
/*      本软件版权归Sagasarate(sagasarate@sina.com)所有                     */
/*      你可以将本软件用于任何商业和非商业软件开发，但                      */
/*      必须保留此版权声明                                                  */
/*                                                                          */
/****************************************************************************/
#pragma once
#include "afxwin.h"


// CDlgWOWCharEquipmentSetting 对话框

class CDlgWOWCharEquipmentSetting : public CDialog
{
protected:
	CD3DWOWM2CharacterModel *	m_pModel;
	CUSOObject *				m_pUSOObject;

	CComboBox					m_cbItemClass;
	CComboBox					m_cbItemSubClass;
	CComboBox					m_cbEquipmentHead;
	CComboBox					m_cbEquipmentShoulder;
	CComboBox					m_cbEquipmentBack;
	CComboBox					m_cbEquipmentShirt;
	CComboBox					m_cbEquipmentBust;
	CComboBox					m_cbEquipmentEnsign;
	CComboBox					m_cbEquipmentWaist;
	CComboBox					m_cbEquipmentLeg;
	CComboBox					m_cbEquipmentFoot;
	CComboBox					m_cbEquipmentWrist;
	CComboBox					m_cbEquipmentHand;
	CComboBox					m_cbEquipmentLeftHand;
	CComboBox					m_cbEquipmentRightHand;

	
	

	CString						m_FilterText;
	bool						m_IsItemDataLoaded;
	

	DECLARE_DYNAMIC(CDlgWOWCharEquipmentSetting)

public:
	CDlgWOWCharEquipmentSetting(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CDlgWOWCharEquipmentSetting();

	bool Init(CD3DWOWM2CharacterModel * pModel,CUSOObject * pUSOObject);
	

// 对话框数据
	enum { IDD = IDD_WOW_CHAR_EQUIPMENT_SETTING };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	
	afx_msg void OnBnClickedFilter();
	
	
	afx_msg void OnCbnSelchangeComboEquipmentHead();
	afx_msg void OnCbnSelchangeComboEquipmentShoulder();
	afx_msg void OnCbnSelchangeComboEquipmentBack();
	afx_msg void OnCbnSelchangeComboEquipmentShirt();
	afx_msg void OnCbnSelchangeComboEquipmentBust();
	afx_msg void OnCbnSelchangeComboEquipmentEnsign();
	afx_msg void OnCbnSelchangeComboEquipmentWaist();
	afx_msg void OnCbnSelchangeComboEquipmentLeg();
	afx_msg void OnCbnSelchangeComboEquipmentFoot();
	afx_msg void OnCbnSelchangeComboEquipmentWrist();
	afx_msg void OnCbnSelchangeComboEquipmentHand();
	afx_msg void OnCbnSelchangeComboEquipmentLeftHand();
	afx_msg void OnCbnSelchangeComboEquipmentRightHand();
	afx_msg void OnCbnSelchangeComboItemClass();
	afx_msg void OnCbnSelchangeComboItemSubClass();
protected:
	void ProcessItemClassChange();
	void ProcessItemSubClassChange();
};
