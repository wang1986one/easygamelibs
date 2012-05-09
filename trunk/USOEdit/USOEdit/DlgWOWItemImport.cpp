/****************************************************************************/
/*                                                                          */
/*      �ļ���:    DlgWOWItemImport.cpp                                     */
/*      ��������:  2011��10��07��                                           */
/*      ����:      Sagasarate                                               */
/*                                                                          */
/*      �������Ȩ��Sagasarate(sagasarate@sina.com)����                     */
/*      ����Խ�����������κ���ҵ�ͷ���ҵ�����������                      */
/*      ���뱣���˰�Ȩ����                                                  */
/*                                                                          */
/****************************************************************************/
/****************************************************************************/
/*                                                                          */
/*      �ļ���:    DlgWOWItemImport.cpp                                     */
/*      ��������:  2010��02��09��                                           */
/*      ����:      Sagasarate                                               */
/*                                                                          */
/*      �������Ȩ��Sagasarate(sagasarate@sina.com)����                     */
/*      ����Խ�����������κ���ҵ�ͷ���ҵ�����������                      */
/*      ���뱣���˰�Ȩ����                                                  */
/*                                                                          */
/****************************************************************************/
// DlgWOWItemImport.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "DlgWOWItemImport.h"



// CDlgWOWItemImport �Ի���

IMPLEMENT_DYNAMIC(CDlgWOWItemImport, CDialog)

CDlgWOWItemImport::CDlgWOWItemImport(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgWOWItemImport::IDD, pParent)
	, m_ModelFileName(_T(""))
	, m_SkinFileName(_T(""))	
	, m_ItemHandType(0)
	, m_FilterText(_T(""))
{
}

CDlgWOWItemImport::~CDlgWOWItemImport()
{
}

void CDlgWOWItemImport::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT1, m_ModelFileName);
	DDX_Text(pDX, IDC_EDIT6, m_SkinFileName);
	DDX_Control(pDX, IDC_COMBO_ITEM_CLASS, m_cbItemClass);
	DDX_Control(pDX, IDC_COMBO_ITEM_SUB_CLASS, m_cbItemSubClass);
	DDX_Control(pDX, IDC_COMBO_ITEM, m_cbItem);
	DDX_Control(pDX, IDC_COMBO_ITEM_HAND_TYPE, m_cbItemHandType);
	DDX_CBIndex(pDX, IDC_COMBO_ITEM_HAND_TYPE, m_ItemHandType);
	DDX_Text(pDX, IDC_EDIT_FILTER_TEXT, m_FilterText);
}


BEGIN_MESSAGE_MAP(CDlgWOWItemImport, CDialog)
	ON_BN_CLICKED(IDC_SELECT_MODEL_FILE, &CDlgWOWItemImport::OnBnClickedSelectModelFile)
	ON_BN_CLICKED(IDC_SELECT_SKIN_FILE, &CDlgWOWItemImport::OnBnClickedSelectSkinFile)
	ON_CBN_SELCHANGE(IDC_COMBO_ITEM_CLASS, &CDlgWOWItemImport::OnCbnSelchangeComboItemClass)
	ON_CBN_SELCHANGE(IDC_COMBO_ITEM_SUB_CLASS, &CDlgWOWItemImport::OnCbnSelchangeComboItemSubClass)
	ON_CBN_SELCHANGE(IDC_COMBO_ITEM, &CDlgWOWItemImport::OnCbnSelchangeComboItem)
	ON_CBN_SELCHANGE(IDC_COMBO_ITEM_HAND_TYPE, &CDlgWOWItemImport::OnCbnSelchangeComboItemHandType)
END_MESSAGE_MAP()


// CDlgWOWItemImport ��Ϣ�������

void CDlgWOWItemImport::OnBnClickedSelectModelFile()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CFileDialog dlg(true,_T("*.m2"),_T("*.m2"),OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT|OFN_NOCHANGEDIR,
		_T("Blizzard M2 Model File(*.m2)|*.m2|All Files(*.*)|*.*||"));

	if(dlg.DoModal()==IDOK)
	{
		UpdateData(true);
		m_ModelFileName=dlg.GetPathName();
		UpdateData(false);
	}
}

void CDlgWOWItemImport::OnBnClickedSelectSkinFile()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CFileDialog dlg(true,_T("*.skin"),_T("*.skin"),OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT|OFN_NOCHANGEDIR,
		_T("Blizzard M2 Model Skin File(*.skin)|*.skin|All Files(*.*)|*.*||"));

	if(dlg.DoModal()==IDOK)
	{
		UpdateData(true);
		m_SkinFileName=dlg.GetPathName();
		UpdateData(false);
	}
}

BOOL CDlgWOWItemImport::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  �ڴ���Ӷ���ĳ�ʼ��

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


	m_cbItemHandType.SetRedraw(false);
	m_cbItemHandType.ResetContent();
	m_cbItemHandType.AddString(_T("��"));
	m_cbItemHandType.AddString(_T("��"));
	m_cbItemHandType.SetRedraw(true);
	m_cbItemHandType.SetCurSel(0);	
	ProcessItemClassChange();

	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣: OCX ����ҳӦ���� FALSE
}

void CDlgWOWItemImport::OnCbnSelchangeComboItemClass()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	ProcessItemClassChange();
}

void CDlgWOWItemImport::OnCbnSelchangeComboItemSubClass()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	ProcessItemSubClassChange();
}

void CDlgWOWItemImport::OnCbnSelchangeComboItem()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	ProcessItemChange();
}

void CDlgWOWItemImport::OnCbnSelchangeComboItemHandType()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	UpdateData(true);
	ProcessItemChange();
}

void CDlgWOWItemImport::ProcessItemClassChange()
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
			Index=m_cbItemSubClass.AddString(Temp);
			m_cbItemSubClass.SetItemData(Index,pItemClassInfo->SubClassInfo[i].SubClassID);
		}
		m_cbItemSubClass.SetRedraw(true);
		m_cbItemSubClass.SetCurSel(0);		
		ProcessItemSubClassChange();
	}

}
void CDlgWOWItemImport::ProcessItemSubClassChange()
{
	UpdateData(true);
	int Index=m_cbItemClass.GetCurSel();
	UINT ItemClassID=(UINT)m_cbItemClass.GetItemData(Index);
	Index=m_cbItemSubClass.GetCurSel();
	UINT ItemSubClassID=(UINT)m_cbItemSubClass.GetItemData(Index);
	CBLZWOWDatabase::BLZ_DB_ITEM_SUB_CLASS * pItemSubClassInfo=CBLZWOWDatabase::GetInstance()->GetItemSubClassInfo(ItemClassID,ItemSubClassID);
	if(pItemSubClassInfo)
	{
		m_cbItem.SetRedraw(false);
		m_cbItem.ResetContent();
		for(UINT i=0;i<pItemSubClassInfo->ItemInfo.GetCount();i++)
		{
			if(!m_FilterText.IsEmpty())
			{
				if(pItemSubClassInfo->ItemInfo[i]->Name.Find(m_FilterText)<0)
					continue;
			}
			CEasyString Temp;
			Temp.Format(_T("[%u]%s"),
				pItemSubClassInfo->ItemInfo[i]->ItemID,
				(LPCTSTR)(pItemSubClassInfo->ItemInfo[i]->Name));
			Index=m_cbItem.AddString(Temp);
			m_cbItem.SetItemData(Index,pItemSubClassInfo->ItemInfo[i]->ItemID);
		}
		m_cbItem.SetRedraw(true);
		m_cbItem.SetCurSel(0);		
		ProcessItemChange();
	}
}
void CDlgWOWItemImport::ProcessItemChange()
{
	int Index=m_cbItem.GetCurSel();
	m_ItemID=(UINT)m_cbItem.GetItemData(Index);

	CBLZWOWDatabase::BLZ_DB_ITEM_DATA * pItemData=CBLZWOWDatabase::GetInstance()->GetItemData(m_ItemID);
	if(pItemData)
	{
		m_ItemDisplayID=pItemData->ItemDisplayInfo;
		CBLZWOWDatabase::BLZ_DB_ITEM_DISPLAY_INFO * pItemDisplayInfo=
			CBLZWOWDatabase::GetInstance()->GetItemDisplayInfo(m_ItemDisplayID);
		if(pItemDisplayInfo)
		{
			if(m_ItemHandType==CD3DWOWM2ItemModel::IHT_LEFT)
			{
				if(!pItemDisplayInfo->LeftModel.IsEmpty())
				{
					UpdateData(true);
					CEasyString ModelFileName=ITEM_PATH_BY_SLOT[pItemData->InventorySlotID];
					ModelFileName=ModelFileName+"\\"+pItemDisplayInfo->LeftModel;
					int Pos=ModelFileName.ReverseFind('.');
					if(Pos>=0)
					{
						ModelFileName=ModelFileName.Left(Pos);
					}
					m_ModelFileName=ModelFileName+".m2";
					m_SkinFileName=ModelFileName+"00.skin";
					UpdateData(false);
				}
			}
			else
			{
				if(!pItemDisplayInfo->RightModel.IsEmpty())
				{
					UpdateData(true);
					CEasyString ModelFileName=ITEM_PATH_BY_SLOT[pItemData->InventorySlotID];
					ModelFileName=ModelFileName+"\\"+pItemDisplayInfo->RightModel;
					int Pos=ModelFileName.ReverseFind('.');
					if(Pos>=0)
					{
						ModelFileName=ModelFileName.Left(Pos);
					}
					m_ModelFileName=ModelFileName+".m2";
					m_SkinFileName=ModelFileName+"00.skin";
					UpdateData(false);
				}
			}
			
		}
	}
}

