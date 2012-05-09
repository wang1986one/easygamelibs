/****************************************************************************/
/*                                                                          */
/*      文件名:    DlgWOWCharEquipmentSetting.cpp                           */
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
/*      文件名:    DlgWOWCharEquipmentSetting.cpp                           */
/*      创建日期:  2010年02月09日                                           */
/*      作者:      Sagasarate                                               */
/*                                                                          */
/*      本软件版权归Sagasarate(sagasarate@sina.com)所有                     */
/*      你可以将本软件用于任何商业和非商业软件开发，但                      */
/*      必须保留此版权声明                                                  */
/*                                                                          */
/****************************************************************************/
// DlgWOWCharEquipmentSetting.cpp : 实现文件
//

#include "stdafx.h"
#include "DlgWOWCharEquipmentSetting.h"



// CDlgWOWCharEquipmentSetting 对话框

IMPLEMENT_DYNAMIC(CDlgWOWCharEquipmentSetting, CDialog)

CDlgWOWCharEquipmentSetting::CDlgWOWCharEquipmentSetting(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgWOWCharEquipmentSetting::IDD, pParent)
	, m_FilterText(_T(""))
{
	m_pModel=NULL;
	m_pUSOObject=NULL;	
	m_IsItemDataLoaded=false;

}

CDlgWOWCharEquipmentSetting::~CDlgWOWCharEquipmentSetting()
{
}

void CDlgWOWCharEquipmentSetting::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_COMBO_ITEM_CLASS, m_cbItemClass);
	DDX_Control(pDX, IDC_COMBO_ITEM_SUB_CLASS, m_cbItemSubClass);	
	DDX_Control(pDX, IDC_COMBO_EQUIPMENT_HEAD,		m_cbEquipmentHead);
	DDX_Control(pDX, IDC_COMBO_EQUIPMENT_SHOULDER,	m_cbEquipmentShoulder);
	DDX_Control(pDX, IDC_COMBO_EQUIPMENT_CAPE,		m_cbEquipmentBack);
	DDX_Control(pDX, IDC_COMBO_EQUIPMENT_SHIRT,		m_cbEquipmentShirt);
	DDX_Control(pDX, IDC_COMBO_EQUIPMENT_BUST,		m_cbEquipmentBust);
	DDX_Control(pDX, IDC_COMBO_EQUIPMENT_ENSIGN,	m_cbEquipmentEnsign);
	DDX_Control(pDX, IDC_COMBO_EQUIPMENT_BELT,		m_cbEquipmentWaist);
	DDX_Control(pDX, IDC_COMBO_EQUIPMENT_LEG,		m_cbEquipmentLeg);
	DDX_Control(pDX, IDC_COMBO_EQUIPMENT_BOOT,		m_cbEquipmentFoot);
	DDX_Control(pDX, IDC_COMBO_EQUIPMENT_WRIST,		m_cbEquipmentWrist);
	DDX_Control(pDX, IDC_COMBO_EQUIPMENT_HAND,		m_cbEquipmentHand);
	DDX_Control(pDX, IDC_COMBO_EQUIPMENT_LEFT_HAND, m_cbEquipmentLeftHand);
	DDX_Control(pDX, IDC_COMBO_EQUIPMENT_RIGHT_HAND,m_cbEquipmentRightHand);

	DDX_Text(pDX, IDC_EDIT_FILTER_TEXT, m_FilterText);
}


BEGIN_MESSAGE_MAP(CDlgWOWCharEquipmentSetting, CDialog)
	ON_BN_CLICKED(IDC_FILTER, &CDlgWOWCharEquipmentSetting::OnBnClickedFilter)
	ON_CBN_SELCHANGE(IDC_COMBO_EQUIPMENT_HEAD, &CDlgWOWCharEquipmentSetting::OnCbnSelchangeComboEquipmentHead)
	ON_CBN_SELCHANGE(IDC_COMBO_EQUIPMENT_SHOULDER, &CDlgWOWCharEquipmentSetting::OnCbnSelchangeComboEquipmentShoulder)
	ON_CBN_SELCHANGE(IDC_COMBO_EQUIPMENT_CAPE, &CDlgWOWCharEquipmentSetting::OnCbnSelchangeComboEquipmentBack)
	ON_CBN_SELCHANGE(IDC_COMBO_EQUIPMENT_SHIRT, &CDlgWOWCharEquipmentSetting::OnCbnSelchangeComboEquipmentShirt)
	ON_CBN_SELCHANGE(IDC_COMBO_EQUIPMENT_BUST, &CDlgWOWCharEquipmentSetting::OnCbnSelchangeComboEquipmentBust)
	ON_CBN_SELCHANGE(IDC_COMBO_EQUIPMENT_ENSIGN, &CDlgWOWCharEquipmentSetting::OnCbnSelchangeComboEquipmentEnsign)
	ON_CBN_SELCHANGE(IDC_COMBO_EQUIPMENT_BELT, &CDlgWOWCharEquipmentSetting::OnCbnSelchangeComboEquipmentWaist)
	ON_CBN_SELCHANGE(IDC_COMBO_EQUIPMENT_LEG, &CDlgWOWCharEquipmentSetting::OnCbnSelchangeComboEquipmentLeg)
	ON_CBN_SELCHANGE(IDC_COMBO_EQUIPMENT_BOOT, &CDlgWOWCharEquipmentSetting::OnCbnSelchangeComboEquipmentFoot)
	ON_CBN_SELCHANGE(IDC_COMBO_EQUIPMENT_WRIST, &CDlgWOWCharEquipmentSetting::OnCbnSelchangeComboEquipmentWrist)
	ON_CBN_SELCHANGE(IDC_COMBO_EQUIPMENT_HAND, &CDlgWOWCharEquipmentSetting::OnCbnSelchangeComboEquipmentHand)
	ON_CBN_SELCHANGE(IDC_COMBO_EQUIPMENT_LEFT_HAND, &CDlgWOWCharEquipmentSetting::OnCbnSelchangeComboEquipmentLeftHand)
	ON_CBN_SELCHANGE(IDC_COMBO_EQUIPMENT_RIGHT_HAND, &CDlgWOWCharEquipmentSetting::OnCbnSelchangeComboEquipmentRightHand)
	ON_CBN_SELCHANGE(IDC_COMBO_ITEM_CLASS, &CDlgWOWCharEquipmentSetting::OnCbnSelchangeComboItemClass)
	ON_CBN_SELCHANGE(IDC_COMBO_ITEM_SUB_CLASS, &CDlgWOWCharEquipmentSetting::OnCbnSelchangeComboItemSubClass)
END_MESSAGE_MAP()


// CDlgWOWCharEquipmentSetting 消息处理程序


bool CDlgWOWCharEquipmentSetting::Init(CD3DWOWM2CharacterModel * pModel,CUSOObject * pUSOObject)
{
	m_pModel=pModel;
	m_pUSOObject=pUSOObject;

	if(!m_IsItemDataLoaded)
	{
		m_IsItemDataLoaded=true;
		m_cbItemClass.SetRedraw(false);
		m_cbItemClass.ResetContent();
		for(UINT i=0;i<CBLZWOWDatabase::GetInstance()->GetItemClassCount();i++)
		{
			CBLZWOWDatabase::BLZ_DB_ITEM_CLASS * pItemClassInfo=CBLZWOWDatabase::GetInstance()->GetItemClassInfoByIndex(i);
			CEasyString Temp;
			Temp.Format(_T("[%u]%s"),
				pItemClassInfo->ClassID,
				(LPCTSTR)(pItemClassInfo->Name));
			int Index=m_cbItemClass.AddString(Temp);
			m_cbItemClass.SetItemData(Index,(DWORD_PTR)(pItemClassInfo->ClassID));
		}
		m_cbItemClass.SetRedraw(true);
		m_cbItemClass.SetCurSel(0);	

		ProcessItemClassChange();
	}
	for(UINT i=0;i<CD3DWOWM2CharacterModel::CES_MAX;i++)
	{
		CBLZWOWDatabase::BLZ_DB_ITEM_DATA * pItemData=CBLZWOWDatabase::GetInstance()->
			GetItemData(m_pModel->GetEquipment(i)->ItemID);
		if(pItemData)
		{
			CEasyString Temp;
			Temp.Format(_T("[%u]%s"),
				pItemData->ItemID,
				(LPCTSTR)(pItemData->Name));
			CWnd * pTextWnd=NULL;
			switch(i)
			{
			case CD3DWOWM2CharacterModel::CES_HEAD:
				pTextWnd=GetDlgItem(IDC_EDIT_EQUIPMENT_HEAD);
				break;
			case CD3DWOWM2CharacterModel::CES_SHOULDER:
				pTextWnd=GetDlgItem(IDC_EDIT_EQUIPMENT_SHOULDER);
				break;
			case CD3DWOWM2CharacterModel::CES_LEFT_HAND:
				pTextWnd=GetDlgItem(IDC_EDIT_EQUIPMENT_LEFT_HAND);
				break;
			case CD3DWOWM2CharacterModel::CES_RIGHT_HAND:
				pTextWnd=GetDlgItem(IDC_EDIT_EQUIPMENT_RIGHT_HAND);
				break;
			case CD3DWOWM2CharacterModel::CES_SHIELD:
				pTextWnd=GetDlgItem(IDC_EDIT_EQUIPMENT_LEFT_HAND);
				break;
			case CD3DWOWM2CharacterModel::CES_CAPE:
				pTextWnd=GetDlgItem(IDC_EDIT_EQUIPMENT_CAPE);
				break;
			case CD3DWOWM2CharacterModel::CES_SHIRT:
				pTextWnd=GetDlgItem(IDC_EDIT_EQUIPMENT_SHIRT);
				break;
			case CD3DWOWM2CharacterModel::CES_LEG:
				pTextWnd=GetDlgItem(IDC_EDIT_EQUIPMENT_LEG);
				break;
			case CD3DWOWM2CharacterModel::CES_BOOT:
				pTextWnd=GetDlgItem(IDC_EDIT_EQUIPMENT_BOOT);
				break;
			case CD3DWOWM2CharacterModel::CES_BUST:
				pTextWnd=GetDlgItem(IDC_EDIT_EQUIPMENT_BUST);
				break;
			case CD3DWOWM2CharacterModel::CES_WRIST:
				pTextWnd=GetDlgItem(IDC_EDIT_EQUIPMENT_WRIST);
				break;
			case CD3DWOWM2CharacterModel::CES_HAND:
				pTextWnd=GetDlgItem(IDC_EDIT_EQUIPMENT_HAND);
				break;
			case CD3DWOWM2CharacterModel::CES_BELT:
				pTextWnd=GetDlgItem(IDC_EDIT_EQUIPMENT_BELT);
				break;
			case CD3DWOWM2CharacterModel::CES_ENSIGN:
				pTextWnd=GetDlgItem(IDC_EDIT_EQUIPMENT_ENSIGN);
				break;
			}
			if(pTextWnd)
				pTextWnd->SetWindowText(Temp);
		}		
	}
	return true;
}
void CDlgWOWCharEquipmentSetting::OnBnClickedFilter()
{
	// TODO: 在此添加控件通知处理程序代码
	ProcessItemSubClassChange();
}

void CDlgWOWCharEquipmentSetting::OnCbnSelchangeComboEquipmentHead()
{
	// TODO: 在此添加控件通知处理程序代码
	CWnd * pTextWnd=GetDlgItem(IDC_EDIT_EQUIPMENT_HEAD);
	CEasyString Temp;
	int Sel=m_cbEquipmentHead.GetCurSel();
	UINT ItemID=m_cbEquipmentHead.GetItemData(Sel);
	m_pUSOObject->ClearAllChild();
	CBLZWOWDatabase::BLZ_DB_ITEM_DATA * pItemData=CBLZWOWDatabase::GetInstance()->GetItemData(ItemID);
	if(pItemData)
	{
		
		Temp.Format(_T("[%u]%s"),
			pItemData->ItemID,
			(LPCTSTR)(pItemData->Name));
		m_pModel->SetEquipment(CD3DWOWM2CharacterModel::CES_HEAD,ItemID);		
	}
	else
	{
		m_pModel->SetEquipment(CD3DWOWM2CharacterModel::CES_HEAD,0);
	}
	m_pModel->BuildModel();
	m_pModel->GetRender()->AddObject(m_pModel);
	m_pUSOObject->ReBindChilds();
	pTextWnd->SetWindowText(Temp);
	
}

void CDlgWOWCharEquipmentSetting::OnCbnSelchangeComboEquipmentShoulder()
{
	// TODO: 在此添加控件通知处理程序代码
	CWnd * pTextWnd=GetDlgItem(IDC_EDIT_EQUIPMENT_SHOULDER);
	CEasyString Temp;
	int Sel=m_cbEquipmentShoulder.GetCurSel();
	m_pUSOObject->ClearAllChild();
	UINT ItemID=m_cbEquipmentShoulder.GetItemData(Sel);
	CBLZWOWDatabase::BLZ_DB_ITEM_DATA * pItemData=CBLZWOWDatabase::GetInstance()->GetItemData(ItemID);
	if(pItemData)
	{		
		Temp.Format(_T("[%u]%s"),
			pItemData->ItemID,
			(LPCTSTR)(pItemData->Name));
		m_pModel->SetEquipment(CD3DWOWM2CharacterModel::CES_SHOULDER,ItemID);		
	}
	else
	{
		m_pModel->SetEquipment(CD3DWOWM2CharacterModel::CES_SHOULDER,0);
	}
	m_pModel->BuildModel();
	m_pModel->GetRender()->AddObject(m_pModel);
	m_pUSOObject->ReBindChilds();
	pTextWnd->SetWindowText(Temp);
}

void CDlgWOWCharEquipmentSetting::OnCbnSelchangeComboEquipmentBack()
{
	// TODO: 在此添加控件通知处理程序代码
	CWnd * pTextWnd=GetDlgItem(IDC_EDIT_EQUIPMENT_CAPE);
	CEasyString Temp;
	int Sel=m_cbEquipmentBack.GetCurSel();
	UINT ItemID=m_cbEquipmentBack.GetItemData(Sel);
	CBLZWOWDatabase::BLZ_DB_ITEM_DATA * pItemData=CBLZWOWDatabase::GetInstance()->GetItemData(ItemID);
	if(pItemData)
	{
		
		Temp.Format(_T("[%u]%s"),
			pItemData->ItemID,
			(LPCTSTR)(pItemData->Name));
		m_pModel->SetEquipment(CD3DWOWM2CharacterModel::CES_CAPE,ItemID);
	}
	else
	{
		m_pModel->SetEquipment(CD3DWOWM2CharacterModel::CES_CAPE,0);
	}
	m_pModel->BuildModel();
	m_pModel->GetRender()->AddObject(m_pModel);
	m_pUSOObject->ReBindChilds();
	pTextWnd->SetWindowText(Temp);
}

void CDlgWOWCharEquipmentSetting::OnCbnSelchangeComboEquipmentShirt()
{
	// TODO: 在此添加控件通知处理程序代码
	CWnd * pTextWnd=GetDlgItem(IDC_EDIT_EQUIPMENT_SHIRT);
	CEasyString Temp;
	int Sel=m_cbEquipmentShirt.GetCurSel();
	UINT ItemID=m_cbEquipmentShirt.GetItemData(Sel);
	CBLZWOWDatabase::BLZ_DB_ITEM_DATA * pItemData=CBLZWOWDatabase::GetInstance()->GetItemData(ItemID);
	if(pItemData)
	{
		
		Temp.Format(_T("[%u]%s"),
			pItemData->ItemID,
			(LPCTSTR)(pItemData->Name));
		m_pModel->SetEquipment(CD3DWOWM2CharacterModel::CES_SHIRT,ItemID);
	}
	else
	{
		m_pModel->SetEquipment(CD3DWOWM2CharacterModel::CES_SHIRT,0);
	}
	m_pModel->BuildModel();
	m_pModel->GetRender()->AddObject(m_pModel);
	m_pUSOObject->ReBindChilds();
	pTextWnd->SetWindowText(Temp);
}

void CDlgWOWCharEquipmentSetting::OnCbnSelchangeComboEquipmentBust()
{
	// TODO: 在此添加控件通知处理程序代码
	CWnd * pTextWnd=GetDlgItem(IDC_EDIT_EQUIPMENT_BUST);
	CEasyString Temp;
	int Sel=m_cbEquipmentBust.GetCurSel();
	UINT ItemID=m_cbEquipmentBust.GetItemData(Sel);
	CBLZWOWDatabase::BLZ_DB_ITEM_DATA * pItemData=CBLZWOWDatabase::GetInstance()->GetItemData(ItemID);
	if(pItemData)
	{
		
		Temp.Format(_T("[%u]%s"),
			pItemData->ItemID,
			(LPCTSTR)(pItemData->Name));
		m_pModel->SetEquipment(CD3DWOWM2CharacterModel::CES_BUST,ItemID);
	}
	else
	{
		m_pModel->SetEquipment(CD3DWOWM2CharacterModel::CES_BUST,0);
	}
	m_pModel->BuildModel();
	m_pModel->GetRender()->AddObject(m_pModel);
	m_pUSOObject->ReBindChilds();
	pTextWnd->SetWindowText(Temp);
}

void CDlgWOWCharEquipmentSetting::OnCbnSelchangeComboEquipmentEnsign()
{
	// TODO: 在此添加控件通知处理程序代码
	CWnd * pTextWnd=GetDlgItem(IDC_EDIT_EQUIPMENT_ENSIGN);
	CEasyString Temp;
	int Sel=m_cbEquipmentEnsign.GetCurSel();
	UINT ItemID=m_cbEquipmentEnsign.GetItemData(Sel);
	CBLZWOWDatabase::BLZ_DB_ITEM_DATA * pItemData=CBLZWOWDatabase::GetInstance()->GetItemData(ItemID);
	if(pItemData)
	{
		
		Temp.Format(_T("[%u]%s"),
			pItemData->ItemID,
			(LPCTSTR)(pItemData->Name));
		m_pModel->SetEquipment(CD3DWOWM2CharacterModel::CES_ENSIGN,ItemID);
	}
	else
	{
		m_pModel->SetEquipment(CD3DWOWM2CharacterModel::CES_ENSIGN,0);
	}
	m_pModel->BuildModel();
	m_pModel->GetRender()->AddObject(m_pModel);
	m_pUSOObject->ReBindChilds();
	pTextWnd->SetWindowText(Temp);
}

void CDlgWOWCharEquipmentSetting::OnCbnSelchangeComboEquipmentWaist()
{
	// TODO: 在此添加控件通知处理程序代码
	CWnd * pTextWnd=GetDlgItem(IDC_EDIT_EQUIPMENT_BELT);
	CEasyString Temp;
	int Sel=m_cbEquipmentWaist.GetCurSel();
	UINT ItemID=m_cbEquipmentWaist.GetItemData(Sel);
	CBLZWOWDatabase::BLZ_DB_ITEM_DATA * pItemData=CBLZWOWDatabase::GetInstance()->GetItemData(ItemID);
	if(pItemData)
	{
		
		Temp.Format(_T("[%u]%s"),
			pItemData->ItemID,
			(LPCTSTR)(pItemData->Name));
		m_pModel->SetEquipment(CD3DWOWM2CharacterModel::CES_BELT,ItemID);
	}
	else
	{
		m_pModel->SetEquipment(CD3DWOWM2CharacterModel::CES_BELT,0);
	}
	m_pModel->BuildModel();
	m_pModel->GetRender()->AddObject(m_pModel);
	m_pUSOObject->ReBindChilds();
	pTextWnd->SetWindowText(Temp);
}

void CDlgWOWCharEquipmentSetting::OnCbnSelchangeComboEquipmentLeg()
{
	// TODO: 在此添加控件通知处理程序代码
	CWnd * pTextWnd=GetDlgItem(IDC_EDIT_EQUIPMENT_LEG);
	CEasyString Temp;
	int Sel=m_cbEquipmentLeg.GetCurSel();
	UINT ItemID=m_cbEquipmentLeg.GetItemData(Sel);
	CBLZWOWDatabase::BLZ_DB_ITEM_DATA * pItemData=CBLZWOWDatabase::GetInstance()->GetItemData(ItemID);
	if(pItemData)
	{
		
		Temp.Format(_T("[%u]%s"),
			pItemData->ItemID,
			(LPCTSTR)(pItemData->Name));
		m_pModel->SetEquipment(CD3DWOWM2CharacterModel::CES_LEG,ItemID);
	}
	else
	{
		m_pModel->SetEquipment(CD3DWOWM2CharacterModel::CES_LEG,0);
	}
	m_pModel->BuildModel();
	m_pModel->GetRender()->AddObject(m_pModel);
	m_pUSOObject->ReBindChilds();
	pTextWnd->SetWindowText(Temp);
}

void CDlgWOWCharEquipmentSetting::OnCbnSelchangeComboEquipmentFoot()
{
	// TODO: 在此添加控件通知处理程序代码
	CWnd * pTextWnd=GetDlgItem(IDC_EDIT_EQUIPMENT_BOOT);
	CEasyString Temp;
	int Sel=m_cbEquipmentFoot.GetCurSel();
	UINT ItemID=m_cbEquipmentFoot.GetItemData(Sel);
	CBLZWOWDatabase::BLZ_DB_ITEM_DATA * pItemData=CBLZWOWDatabase::GetInstance()->GetItemData(ItemID);
	if(pItemData)
	{
		
		Temp.Format(_T("[%u]%s"),
			pItemData->ItemID,
			(LPCTSTR)(pItemData->Name));
		m_pModel->SetEquipment(CD3DWOWM2CharacterModel::CES_BOOT,ItemID);
	}
	else
	{
		m_pModel->SetEquipment(CD3DWOWM2CharacterModel::CES_BOOT,0);
	}
	m_pModel->BuildModel();
	m_pModel->GetRender()->AddObject(m_pModel);
	m_pUSOObject->ReBindChilds();
	pTextWnd->SetWindowText(Temp);
}

void CDlgWOWCharEquipmentSetting::OnCbnSelchangeComboEquipmentWrist()
{
	// TODO: 在此添加控件通知处理程序代码
	CWnd * pTextWnd=GetDlgItem(IDC_EDIT_EQUIPMENT_WRIST);
	CEasyString Temp;
	int Sel=m_cbEquipmentWrist.GetCurSel();
	UINT ItemID=m_cbEquipmentWrist.GetItemData(Sel);
	CBLZWOWDatabase::BLZ_DB_ITEM_DATA * pItemData=CBLZWOWDatabase::GetInstance()->GetItemData(ItemID);
	if(pItemData)
	{
		
		Temp.Format(_T("[%u]%s"),
			pItemData->ItemID,
			(LPCTSTR)(pItemData->Name));
		m_pModel->SetEquipment(CD3DWOWM2CharacterModel::CES_WRIST,ItemID);
	}
	else
	{
		m_pModel->SetEquipment(CD3DWOWM2CharacterModel::CES_WRIST,0);
	}
	m_pModel->BuildModel();
	m_pModel->GetRender()->AddObject(m_pModel);
	m_pUSOObject->ReBindChilds();
	pTextWnd->SetWindowText(Temp);
}

void CDlgWOWCharEquipmentSetting::OnCbnSelchangeComboEquipmentHand()
{
	// TODO: 在此添加控件通知处理程序代码
	CWnd * pTextWnd=GetDlgItem(IDC_EDIT_EQUIPMENT_HAND);
	CEasyString Temp;
	int Sel=m_cbEquipmentHand.GetCurSel();
	UINT ItemID=m_cbEquipmentHand.GetItemData(Sel);
	CBLZWOWDatabase::BLZ_DB_ITEM_DATA * pItemData=CBLZWOWDatabase::GetInstance()->GetItemData(ItemID);
	if(pItemData)
	{
		
		Temp.Format(_T("[%u]%s"),
			pItemData->ItemID,
			(LPCTSTR)(pItemData->Name));
		m_pModel->SetEquipment(CD3DWOWM2CharacterModel::CES_HAND,ItemID);
	}
	else
	{
		m_pModel->SetEquipment(CD3DWOWM2CharacterModel::CES_HAND,0);
	}
	m_pModel->BuildModel();
	m_pModel->GetRender()->AddObject(m_pModel);
	m_pUSOObject->ReBindChilds();
	pTextWnd->SetWindowText(Temp);
}

void CDlgWOWCharEquipmentSetting::OnCbnSelchangeComboEquipmentLeftHand()
{
	// TODO: 在此添加控件通知处理程序代码
	CWnd * pTextWnd=GetDlgItem(IDC_EDIT_EQUIPMENT_LEFT_HAND);
	CEasyString Temp;
	int Sel=m_cbEquipmentLeftHand.GetCurSel();
	m_pUSOObject->ClearAllChild();
	UINT ItemID=m_cbEquipmentLeftHand.GetItemData(Sel);
	m_pModel->SetEquipment(CD3DWOWM2CharacterModel::CES_SHIELD,0);
	m_pModel->SetEquipment(CD3DWOWM2CharacterModel::CES_LEFT_HAND,0);
	CBLZWOWDatabase::BLZ_DB_ITEM_DATA * pItemData=CBLZWOWDatabase::GetInstance()->GetItemData(ItemID);
	if(pItemData)
	{
		
		Temp.Format(_T("[%u]%s"),
			pItemData->ItemID,
			(LPCTSTR)(pItemData->Name));
		if(pItemData->InventorySlotID==IISI_SHIELD)
			m_pModel->SetEquipment(CD3DWOWM2CharacterModel::CES_SHIELD,ItemID);
		else
			m_pModel->SetEquipment(CD3DWOWM2CharacterModel::CES_LEFT_HAND,ItemID);
	}
	m_pModel->BuildModel();
	m_pModel->GetRender()->AddObject(m_pModel);
	m_pUSOObject->ReBindChilds();
	pTextWnd->SetWindowText(Temp);
}

void CDlgWOWCharEquipmentSetting::OnCbnSelchangeComboEquipmentRightHand()
{
	// TODO: 在此添加控件通知处理程序代码
	CWnd * pTextWnd=GetDlgItem(IDC_EDIT_EQUIPMENT_RIGHT_HAND);
	CEasyString Temp;
	int Sel=m_cbEquipmentRightHand.GetCurSel();
	m_pUSOObject->ClearAllChild();
	UINT ItemID=m_cbEquipmentRightHand.GetItemData(Sel);
	CBLZWOWDatabase::BLZ_DB_ITEM_DATA * pItemData=CBLZWOWDatabase::GetInstance()->GetItemData(ItemID);
	if(pItemData)
	{
		
		Temp.Format(_T("[%u]%s"),
			pItemData->ItemID,
			(LPCTSTR)(pItemData->Name));
		m_pModel->SetEquipment(CD3DWOWM2CharacterModel::CES_RIGHT_HAND,ItemID);
	}
	else
	{
		m_pModel->SetEquipment(CD3DWOWM2CharacterModel::CES_RIGHT_HAND,0);
	}
	m_pModel->BuildModel();
	m_pModel->GetRender()->AddObject(m_pModel);
	m_pUSOObject->ReBindChilds();
	pTextWnd->SetWindowText(Temp);
}

void CDlgWOWCharEquipmentSetting::OnCbnSelchangeComboItemClass()
{
	// TODO: 在此添加控件通知处理程序代码
	ProcessItemClassChange();
}

void CDlgWOWCharEquipmentSetting::OnCbnSelchangeComboItemSubClass()
{
	// TODO: 在此添加控件通知处理程序代码
	ProcessItemSubClassChange();
}

void CDlgWOWCharEquipmentSetting::ProcessItemClassChange()
{
	int Index=m_cbItemClass.GetCurSel();
	UINT ItemClassID=(UINT)m_cbItemClass.GetItemData(Index);
	CBLZWOWDatabase::BLZ_DB_ITEM_CLASS * pItemClassInfo=CBLZWOWDatabase::GetInstance()->GetItemClassInfo(ItemClassID);
	if(pItemClassInfo)
	{
		m_cbItemSubClass.SetRedraw(false);
		m_cbItemSubClass.ResetContent();
		for(UINT i=0;i<pItemClassInfo->SubClassInfo.GetCount();i++)
		{
			CEasyString Temp;
			Temp.Format(_T("[%u]%s %s"),
				pItemClassInfo->SubClassInfo[i].SubClassID,
				(LPCTSTR)(pItemClassInfo->SubClassInfo[i].Name),
				(LPCTSTR)(pItemClassInfo->SubClassInfo[i].UniquePlural));
			int Index=m_cbItemSubClass.AddString(Temp);
			m_cbItemSubClass.SetItemData(Index,pItemClassInfo->SubClassInfo[i].SubClassID);
		}
		m_cbItemSubClass.SetRedraw(true);
		m_cbItemSubClass.SetCurSel(0);		
		ProcessItemSubClassChange();
	}

}

void CDlgWOWCharEquipmentSetting::ProcessItemSubClassChange()
{
	UpdateData(true);
	int Index=m_cbItemClass.GetCurSel();
	UINT ItemClassID=(UINT)m_cbItemClass.GetItemData(Index);
	Index=m_cbItemSubClass.GetCurSel();
	UINT ItemSubClassID=(UINT)m_cbItemSubClass.GetItemData(Index);
	CBLZWOWDatabase::BLZ_DB_ITEM_SUB_CLASS * pItemSubClassInfo=CBLZWOWDatabase::GetInstance()->GetItemSubClassInfo(ItemClassID,ItemSubClassID);
	if(pItemSubClassInfo)
	{
		m_cbEquipmentHead.SetRedraw(false);
		m_cbEquipmentShoulder.SetRedraw(false);
		m_cbEquipmentBack.SetRedraw(false);
		m_cbEquipmentShirt.SetRedraw(false);
		m_cbEquipmentBust.SetRedraw(false);
		m_cbEquipmentEnsign.SetRedraw(false);
		m_cbEquipmentWaist.SetRedraw(false);
		m_cbEquipmentLeg.SetRedraw(false);
		m_cbEquipmentFoot.SetRedraw(false);
		m_cbEquipmentWrist.SetRedraw(false);
		m_cbEquipmentHand.SetRedraw(false);
		m_cbEquipmentLeftHand.SetRedraw(false);
		m_cbEquipmentRightHand.SetRedraw(false);

		m_cbEquipmentHead.ResetContent();		
		m_cbEquipmentShoulder.ResetContent();
		m_cbEquipmentBack.ResetContent();
		m_cbEquipmentShirt.ResetContent();
		m_cbEquipmentBust.ResetContent();
		m_cbEquipmentEnsign.ResetContent();
		m_cbEquipmentWaist.ResetContent();
		m_cbEquipmentLeg.ResetContent();
		m_cbEquipmentFoot.ResetContent();
		m_cbEquipmentWrist.ResetContent();
		m_cbEquipmentHand.ResetContent();
		m_cbEquipmentLeftHand.ResetContent();
		m_cbEquipmentRightHand.ResetContent();

		m_cbEquipmentHead.AddString(_T("无"));		
		m_cbEquipmentShoulder.AddString(_T("无"));
		m_cbEquipmentBack.AddString(_T("无"));
		m_cbEquipmentShirt.AddString(_T("无"));
		m_cbEquipmentBust.AddString(_T("无"));
		m_cbEquipmentEnsign.AddString(_T("无"));
		m_cbEquipmentWaist.AddString(_T("无"));
		m_cbEquipmentLeg.AddString(_T("无"));
		m_cbEquipmentFoot.AddString(_T("无"));
		m_cbEquipmentWrist.AddString(_T("无"));
		m_cbEquipmentHand.AddString(_T("无"));
		m_cbEquipmentLeftHand.AddString(_T("无"));
		m_cbEquipmentRightHand.AddString(_T("无"));
		
		for(UINT i=0;i<pItemSubClassInfo->ItemInfo.GetCount();i++)
		{
			CEasyString Temp;
			Temp.Format(_T("[%u]%s"),
				pItemSubClassInfo->ItemInfo[i]->ItemID,
				(LPCTSTR)(pItemSubClassInfo->ItemInfo[i]->Name));

			if(!m_FilterText.IsEmpty())
			{
				if(pItemSubClassInfo->ItemInfo[i]->Name.Find(m_FilterText)<0)
					continue;
			}

			switch(pItemSubClassInfo->ItemInfo[i]->InventorySlotID)
			{			
			case IISI_HEAD:
				{
					int Index=m_cbEquipmentHead.AddString(Temp);
					m_cbEquipmentHead.SetItemData(Index,pItemSubClassInfo->ItemInfo[i]->ItemID);
				}
				break;
			case IISI_SHOULDERS:
				{
					int Index=m_cbEquipmentShoulder.AddString(Temp);
					m_cbEquipmentShoulder.SetItemData(Index,pItemSubClassInfo->ItemInfo[i]->ItemID);
				}
				break;
			case IISI_SHIRT:
				{
					int Index=m_cbEquipmentShirt.AddString(Temp);
					m_cbEquipmentShirt.SetItemData(Index,pItemSubClassInfo->ItemInfo[i]->ItemID);
				}
				break;
			case IISI_VEST:		
				{
					int Index=m_cbEquipmentBust.AddString(Temp);
					m_cbEquipmentBust.SetItemData(Index,pItemSubClassInfo->ItemInfo[i]->ItemID);
				}
				break;
			case IISI_WAIST:
				{
					int Index=m_cbEquipmentWaist.AddString(Temp);
					m_cbEquipmentWaist.SetItemData(Index,pItemSubClassInfo->ItemInfo[i]->ItemID);
				}
				break;
			case IISI_LEGS:
				{
					int Index=m_cbEquipmentLeg.AddString(Temp);
					m_cbEquipmentLeg.SetItemData(Index,pItemSubClassInfo->ItemInfo[i]->ItemID);
				}
				break;
			case IISI_FEET:
				{
					int Index=m_cbEquipmentFoot.AddString(Temp);
					m_cbEquipmentFoot.SetItemData(Index,pItemSubClassInfo->ItemInfo[i]->ItemID);
				}
				break;
			case IISI_WRIST:
				{
					int Index=m_cbEquipmentWrist.AddString(Temp);
					m_cbEquipmentWrist.SetItemData(Index,pItemSubClassInfo->ItemInfo[i]->ItemID);
				}
				break;
			case IISI_HANDS:
				{
					int Index=m_cbEquipmentHand.AddString(Temp);
					m_cbEquipmentHand.SetItemData(Index,pItemSubClassInfo->ItemInfo[i]->ItemID);
				}
				break;				
			case IISI_ONE_HAND:
				{
					int Index=m_cbEquipmentRightHand.AddString(Temp);
					m_cbEquipmentRightHand.SetItemData(Index,pItemSubClassInfo->ItemInfo[i]->ItemID);
				}
			case IISI_SHIELD:
			case IISI_BOW:
				{
					int Index=m_cbEquipmentLeftHand.AddString(Temp);
					m_cbEquipmentLeftHand.SetItemData(Index,pItemSubClassInfo->ItemInfo[i]->ItemID);
				}
				break;	
			case IISI_BACK:
				{
					int Index=m_cbEquipmentBack.AddString(Temp);
					m_cbEquipmentBack.SetItemData(Index,pItemSubClassInfo->ItemInfo[i]->ItemID);
				}
				break;	
			case IISI_TWO_HAND:
				{
					int Index=m_cbEquipmentRightHand.AddString(Temp);
					m_cbEquipmentRightHand.SetItemData(Index,pItemSubClassInfo->ItemInfo[i]->ItemID);
				}
				break;				
			case IISI_TABARD:
				{
					int Index=m_cbEquipmentEnsign.AddString(Temp);
					m_cbEquipmentEnsign.SetItemData(Index,pItemSubClassInfo->ItemInfo[i]->ItemID);
				}
				break;
			case IISI_ROBE:
				{
					int Index=m_cbEquipmentBust.AddString(Temp);
					m_cbEquipmentBust.SetItemData(Index,pItemSubClassInfo->ItemInfo[i]->ItemID);
				}
				break;
			case IISI_MAIN_HAND:
				{
					int Index=m_cbEquipmentRightHand.AddString(Temp);
					m_cbEquipmentRightHand.SetItemData(Index,pItemSubClassInfo->ItemInfo[i]->ItemID);
				}
				break;
			case IISI_OFF_HAND:
			case IISI_HELD:
				{
					int Index=m_cbEquipmentLeftHand.AddString(Temp);
					m_cbEquipmentLeftHand.SetItemData(Index,pItemSubClassInfo->ItemInfo[i]->ItemID);
				}
				break;			
			case IISI_RANGED:
				{
					int Index=m_cbEquipmentRightHand.AddString(Temp);
					m_cbEquipmentRightHand.SetItemData(Index,pItemSubClassInfo->ItemInfo[i]->ItemID);
				}
				break;
			}
			
		}
		m_cbEquipmentHead.SetRedraw(true);
		m_cbEquipmentShoulder.SetRedraw(true);
		m_cbEquipmentBack.SetRedraw(true);
		m_cbEquipmentShirt.SetRedraw(true);
		m_cbEquipmentBust.SetRedraw(true);
		m_cbEquipmentEnsign.SetRedraw(true);
		m_cbEquipmentWaist.SetRedraw(true);
		m_cbEquipmentLeg.SetRedraw(true);
		m_cbEquipmentFoot.SetRedraw(true);
		m_cbEquipmentWrist.SetRedraw(true);
		m_cbEquipmentHand.SetRedraw(true);
		m_cbEquipmentLeftHand.SetRedraw(true);
		m_cbEquipmentRightHand.SetRedraw(true);		

		m_cbEquipmentHead.SetCurSel(0);
		m_cbEquipmentShoulder.SetCurSel(0);
		m_cbEquipmentBack.SetCurSel(0);
		m_cbEquipmentShirt.SetCurSel(0);
		m_cbEquipmentBust.SetCurSel(0);
		m_cbEquipmentEnsign.SetCurSel(0);
		m_cbEquipmentWaist.SetCurSel(0);
		m_cbEquipmentLeg.SetCurSel(0);
		m_cbEquipmentFoot.SetCurSel(0);
		m_cbEquipmentWrist.SetCurSel(0);
		m_cbEquipmentHand.SetCurSel(0);
		m_cbEquipmentLeftHand.SetCurSel(0);
		m_cbEquipmentRightHand.SetCurSel(0);
		
	}
}