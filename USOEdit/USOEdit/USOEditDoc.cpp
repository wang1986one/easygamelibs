/****************************************************************************/
/*                                                                          */
/*      文件名:    USOEditDoc.cpp                                           */
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
/*      文件名:    USOEditDoc.cpp                                           */
/*      创建日期:  2010年02月09日                                           */
/*      作者:      Sagasarate                                               */
/*                                                                          */
/*      本软件版权归Sagasarate(sagasarate@sina.com)所有                     */
/*      你可以将本软件用于任何商业和非商业软件开发，但                      */
/*      必须保留此版权声明                                                  */
/*                                                                          */
/****************************************************************************/
// USOEditDoc.cpp :  CUSOEditDoc 类的实现
//

#include "stdafx.h"
#include "USOEditDoc.h"




// CUSOEditDoc

IMPLEMENT_DYNCREATE(CUSOEditDoc, CDocument)

BEGIN_MESSAGE_MAP(CUSOEditDoc, CDocument)	
	ON_COMMAND(ID_IMPORT_X, OnImportX)
	ON_COMMAND(ID_IMPORT_USO, OnImportUso)
	ON_COMMAND(ID_DELETE_OBJ, OnDeleteObj)
	ON_COMMAND(ID_CLEAR_RESOURCE, OnClearResource)	
	ON_COMMAND(ID_EDIT_CUT, OnEditCut)
	ON_COMMAND(ID_EDIT_COPY, OnEditCopy)
	ON_COMMAND(ID_EDIT_PASTE, OnEditPaste)
	ON_COMMAND(ID_IMPORT_TEXTURE, OnImportTexture)
	ON_COMMAND(ID_IMPORT_FX, OnImportFx)
	ON_COMMAND(ID_IMPORT_M2, &CUSOEditDoc::OnImportM2)
	ON_COMMAND(ID_SWITCH_VISIBLE, &CUSOEditDoc::OnSwitchVisible)
	ON_COMMAND(ID_HIDE_ALL_SUBMESH, &CUSOEditDoc::OnHideAllSubmesh)
	ON_COMMAND(ID_IMPORT_M2_CHARACTER, &CUSOEditDoc::OnImportM2Character)
	ON_COMMAND(ID_SHOW_RANDER_PERFORMANCE, &CUSOEditDoc::OnShowRanderPerformance)
	ON_COMMAND(ID_IMPORT_M2_ITEM, &CUSOEditDoc::OnImportM2Item)
	ON_COMMAND(ID_IMPORT_WMO, &CUSOEditDoc::OnImportWmo)
	ON_COMMAND(ID_CAMERA_SETTING, &CUSOEditDoc::OnCameraSetting)
	ON_COMMAND(ID_IMPORT_ADT, &CUSOEditDoc::OnImportAdt)
	ON_COMMAND(ID_LOOK_OBJECT, &CUSOEditDoc::OnLookObject)
	ON_COMMAND(ID_RESOURCE_STAT, &CUSOEditDoc::OnResourceStat)
	ON_COMMAND(ID_SHOW_ALL_SUBMESH, &CUSOEditDoc::OnShowAllSubmesh)
	ON_COMMAND(ID_VIEW_TEXTURE, &CUSOEditDoc::OnViewTexture)
	ON_COMMAND(ID_SYSTEM_LIGHT_SETTING, &CUSOEditDoc::OnSystemLightSetting)
	ON_COMMAND(ID_FILE_VIEWER, &CUSOEditDoc::OnFileViewer)
	ON_COMMAND(ID_SAVE_DEPTH_TEXTURE, &CUSOEditDoc::OnSaveDepthTexture)
	ON_COMMAND(ID_IMPORT_M2_CREATURE, &CUSOEditDoc::OnImportM2Creature)
	ON_COMMAND(ID_CREATE_SCENE, &CUSOEditDoc::OnCreateScene)
	ON_COMMAND(ID_EXPORT_TO_DIR, &CUSOEditDoc::OnExportToDir)
	ON_COMMAND(ID_SENCE_TREE_VIEW, &CUSOEditDoc::OnSenceTreeView)
	ON_COMMAND(ID_IMPORT_PREVIEW_OBJECT, &CUSOEditDoc::OnImportPreviewObject)
	ON_COMMAND(ID_HEIGHT_TEST, &CUSOEditDoc::OnHeightTest)
	ON_COMMAND(ID_IMPORT_WDL, &CUSOEditDoc::OnImportWdl)
END_MESSAGE_MAP()


// CUSOEditDoc 构造/析构

CUSOEditDoc::CUSOEditDoc()
{
	// TODO: 在此添加一次性构造代码
	m_pUSOTreeView=NULL;
	m_pUSOPropertyView=NULL;
	m_pD3DView=NULL;
	m_pControlPanelView=NULL;
	m_pD3DViewPort=NULL;

	m_pObjects=NULL;
	m_pResources=NULL;
	m_pWOWM2ModelResource=NULL;
	m_pTextureResource=NULL;
	m_pFXResource=NULL;
	m_pFontResource=NULL;
	m_pPreview=NULL;
	m_pSystemObjects=NULL;

	m_pPreviewUSOObject=NULL;

	m_CurMapID=0;


	m_USOClipFormat=RegisterClipboardFormat(_T("Universal System Object"));

	m_USOFile.AddObjectCreateFilter(&m_D3DObjectCreateFilter);	

	
}

CUSOEditDoc::~CUSOEditDoc()
{
	
}

bool CUSOEditDoc::Init()
{
	SAFE_RELEASE(m_pResources);	
	SAFE_RELEASE(m_pObjects);	

	m_pObjects=new CUSOObject();
	m_pObjects->SetName(_T("对象"));
	m_pObjects->SetTreeCtrl(&m_pUSOTreeView->GetTreeCtrl());

	m_pPreview=new CUSOObject();
	m_pPreview->SetName(_T("对象预览"));
	m_pPreview->SetTreeCtrl(&m_pUSOTreeView->GetTreeCtrl());

	
	m_pSystemObjects=new CUSOObject();
	m_pSystemObjects->SetName(_T("系统对象"));
	m_pSystemObjects->SetTreeCtrl(&m_pUSOTreeView->GetTreeCtrl());

	m_pResources=new CUSOObject();
	m_pResources->SetName(_T("资源"));	

	m_pWOWM2ModelResource=new CUSOObject();
	m_pWOWM2ModelResource->SetName(_T("WOWM2模型资源"));
	m_pWOWM2ModelResource->SetParent(m_pResources);

	m_pTextureResource=new CUSOObject();
	m_pTextureResource->SetName(_T("材质资源"));
	m_pTextureResource->SetParent(m_pResources);

	m_pFXResource=new CUSOObject();
	m_pFXResource->SetName(_T("FX资源"));
	m_pFXResource->SetParent(m_pResources);

	m_pFontResource=new CUSOObject();
	m_pFontResource->SetName(_T("字体资源"));
	m_pFontResource->SetParent(m_pResources);

	m_pOtherResource=new CUSOObject();
	m_pOtherResource->SetName(_T("其他资源"));
	m_pOtherResource->SetParent(m_pResources);

	m_pResources->SetTreeCtrl(&m_pUSOTreeView->GetTreeCtrl());

	m_USOFile.Destory();

	m_TextureViewer.SetRenderThread(m_pD3DViewPort->GetRenderThread());

	m_RenderPerformance.Create(CDlgRenderPerformance::IDD,AfxGetMainWnd());
	m_CameraSetting.Create(m_CameraSetting.IDD,AfxGetMainWnd());
	m_TextureViewer.Create(m_TextureViewer.IDD,AfxGetMainWnd());
	m_SystemLightSetting.Create(m_SystemLightSetting.IDD,AfxGetMainWnd());
	m_SenceTreeView.Create(m_SenceTreeView.IDD,AfxGetMainWnd());
	m_HeightTest.Create(m_HeightTest.IDD,AfxGetMainWnd());
	

	CUSOEditApp::GetInstance()->GetFileViewer().SetDocument(this);

	CUSOObject * pUSOObject=AddObject(m_pSystemObjects,m_pD3DViewPort->GetSystemLight(),NULL,false,false,_T(""));
	m_pD3DViewPort->GetSystemLight()->AddUseRef();
	pUSOObject->SetTreeCtrl(&m_pUSOTreeView->GetTreeCtrl());

	pUSOObject=AddObject(m_pSystemObjects,m_pD3DViewPort->GetSystemCamera(),NULL,false,false,_T(""));
	m_pD3DViewPort->GetSystemCamera()->AddUseRef();
	pUSOObject->SetTreeCtrl(&m_pUSOTreeView->GetTreeCtrl());

	m_D3DObjectCreateFilter.SetDevice(m_pD3DViewPort->GetDevice());

	m_SenceTreeView.Init(this,m_pD3DViewPort);
	m_HeightTest.SetView(m_pD3DViewPort);

	m_SystemLightSetting.SetDocument(this);
	
	return true;
}






// CUSOEditDoc 序列化

void CUSOEditDoc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		// TODO: 在此添加存储代码
	}
	else
	{
		// TODO: 在此添加加载代码
	}
}


// CUSOEditDoc 诊断

#ifdef _DEBUG
void CUSOEditDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CUSOEditDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG


// CUSOEditDoc 命令

void CUSOEditDoc::OnChangedViewList()
{
	// TODO: 在此添加专用代码和/或调用基类
	POSITION Pos=this->GetFirstViewPosition();
	while(Pos)
	{
		CView * pView=this->GetNextView(Pos);
		if(pView->IsKindOf(RUNTIME_CLASS(CUSOTreeView)))
			m_pUSOTreeView=(CUSOTreeView *)pView;
		if(pView->IsKindOf(RUNTIME_CLASS(CUSOPropertyView)))
			m_pUSOPropertyView=(CUSOPropertyView *)pView;
		if(pView->IsKindOf(RUNTIME_CLASS(CD3DView)))
		{
			m_pD3DView=(CD3DView *)pView;
			m_pD3DViewPort=m_pD3DView->GetViewPort();
			if(m_pD3DViewPort)
			{
				m_D3DObjectCreateFilter.SetDevice(m_pD3DViewPort->GetDevice());
			}
		}
		if(pView->IsKindOf(RUNTIME_CLASS(CControlPanelView)))
			m_pControlPanelView=(CControlPanelView *)pView;		
	}
	m_RenderPerformance.SetViewPort(m_pD3DViewPort);
	m_CameraSetting.SetD3DView(m_pD3DViewPort);
	CDocument::OnChangedViewList();
}


BOOL CUSOEditDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: 在此添加重新初始化代码
	// (SDI 文档将重用该文档)
	Init();


	return TRUE;
}

BOOL CUSOEditDoc::OnOpenDocument(LPCTSTR lpszPathName)
{	
	// TODO:  在此添加您专用的创建代码
	Init();

	m_USOFile.Destory();

	if(m_USOFile.Import(lpszPathName))
	{
		for(int i=0;i<m_USOFile.GetObjectCount();i++)
		{		
			CUSOObject * pObject=new CUSOObject();
			CD3DObject * pD3DObject=(CD3DObject *)m_USOFile.GetObjectByIndex(i);
			pD3DObject->AddUseRef();			
			pObject->SetBindObject(pD3DObject);
			pObject->SetParent(m_pObjects);
			pObject->SetTreeCtrl(&m_pUSOTreeView->GetTreeCtrl());
			if(pD3DObject->IsKindOf(GET_CLASS_INFO(CD3DWOWADTModel))||pD3DObject->IsKindOf(GET_CLASS_INFO(CD3DScene)))
			{				
				pD3DObject->Update(0);				
				GetD3DViewPort()->GetRender()->AddSceneObject((CD3DObject *)pD3DObject);
			}			
			else if(pD3DObject->IsKindOf(GET_CLASS_INFO(CD3DObject)))
			{
				GetD3DViewPort()->GetRender()->AddObject((CD3DObject *)pD3DObject);
			}
			
		}
		UpdateResources();
		return true;
	}
	else
	{
		AfxGetMainWnd()->MessageBox(_T("装载失败！"));
		return false;
	}
}

BOOL CUSOEditDoc::OnSaveDocument(LPCTSTR lpszPathName)
{
	// TODO: 在此添加专用代码和/或调用基类
	
	RePickResource();
	if(m_USOFile.Export(lpszPathName))
	{
		SetModifiedFlag(false);
		return true;
	}
	else
	{
		AfxGetMainWnd()->MessageBox(_T("保存失败！"));
		return false;
	}
}


void CUSOEditDoc::OnCloseDocument()
{
	// TODO: 在此添加专用代码和/或调用基类
	m_pPreviewUSOObject=NULL;
	SAFE_RELEASE(m_pPreview);
	SAFE_RELEASE(m_pSystemObjects);

	m_pWOWM2ModelResource=NULL;
	m_pTextureResource=NULL;
	m_pFXResource=NULL;
	m_pFontResource=NULL;
	SAFE_RELEASE(m_pResources);	
	SAFE_RELEASE(m_pObjects);	
	m_USOFile.Destory();
	CDocument::OnCloseDocument();
}

void CUSOEditDoc::UpdateAllTreeItem()
{
	m_pObjects->UpdateTreeItem(true);
	m_pResources->UpdateTreeItem(true);
}

void CUSOEditDoc::GetFXList(CFXPtrArray& FXPtrArray)
{
	if(m_pFXResource==NULL)
		return;
	FXPtrArray.clear();
	for(UINT i=0;i<m_pFXResource->GetChildCount();i++)
	{
		FXPtrArray.push_back((CD3DFX *)m_pFXResource->GetChildByIndex(i)->GetBindObject());
	}
}


CD3DObject * CUSOEditDoc::PickObject(CD3DVector3 Point,CD3DVector3 Dir)
{
	FLOAT MinDistance=MAX_HEIGHT;
	CD3DObject * pPickObject=NULL;

	if(m_pPreviewUSOObject)
	{
		CD3DObject * pD3DObject=(CD3DObject *)m_pPreviewUSOObject->GetBindObject();
		if(pD3DObject)
		{
			FLOAT ObjDis;
			CD3DObject * pObj=pD3DObject->PickObject(Point,Dir,ObjDis);			
			if(pObj&&ObjDis<MinDistance)
			{
				MinDistance=ObjDis;
				pPickObject=pObj;
			}
		}
	}

	for(UINT i=0;i<m_pObjects->GetChildCount();i++)
	{
		CD3DObject * pD3DObject=(CD3DObject *)m_pObjects->GetChildByIndex(i)->GetBindObject();
		if(pD3DObject->IsKindOf(GET_CLASS_INFO(CD3DObject)))
		{
			FLOAT ObjDis;
			CD3DObject * pObj=pD3DObject->PickObject(Point,Dir,ObjDis);			
			if(pObj&&ObjDis<MinDistance)
			{
				MinDistance=ObjDis;
				pPickObject=pObj;
			}
		}
	}
	CD3DVector3 P;
	FLOAT D,O;
	CD3DObject * pObject;
	m_pD3DViewPort->GetRender()->RayIntersect(Point,Dir,P,D,O,&pObject,RITM_INCLUDE_NO_COLLIDE_FACE);
	return pPickObject;
}

bool CUSOEditDoc::GetHeightByXZ(CD3DVector3 Pos,FLOAT& Height)
{
	for(UINT i=0;i<m_pObjects->GetChildCount();i++)
	{
		CNameObject * pObject=m_pObjects->GetChildByIndex(i)->GetBindObject();
		if(pObject->IsKindOf(GET_CLASS_INFO(CD3DWOWADTModel)))
		{
			CD3DWOWADTModel * pModel=(CD3DWOWADTModel *)pObject;
			FLOAT H,WH;
			if(pModel->GetHeightByXZ(Pos,Pos.y,10,H,WH))
			{
				Height=H;
				if(_finite(WH))
				{
					if(WH>H)
						Height=WH;
				}
				return true;
			}
		}
		else if(pObject->IsKindOf(GET_CLASS_INFO(CD3DScene)))
		{
			CD3DScene * pModel=(CD3DScene *)pObject;
			FLOAT H,WH;
			if(pModel->GetHeightByXZ(Pos,Pos.y,10,H,WH))
			{
				Height=H;
				if(_finite(WH))
				{
					if(WH>Height)
						Height=WH;
				}
				return true;
			}
		}
	}
	return false;
}

CUSOObject * CUSOEditDoc::AddObject(CUSOObject * pParent,CNameObject * pObject,CLASS_INFO * pClassInfo,bool IsAddToUSO,bool IsResource,LPCTSTR ObjectName,LPCTSTR ExtObjectName)
{	
	CUSOObject * pUSOObject=NULL;

	CNameObject * pParentObject=pParent->GetBindObject();


	if(pParentObject)
	{		
		if(!pParentObject->IsKindOf(GET_CLASS_INFO(CD3DObject)))
		{
			AfxGetMainWnd()->MessageBox(_T("不可绑定的父对象！"));
			return NULL;
		}		
	}
	if(pObject==NULL)
	{
		if(pClassInfo)
		{
			pObject=dynamic_cast<CTreeObject *>(m_USOFile.CreateObject(pClassInfo->ClassName,ObjectName,true));

			if(pObject==NULL)
			{
				AfxGetMainWnd()->MessageBox(_T("创建对象失败！"));
				return NULL;
			}
		}
		else
		{
			return NULL;
		}
	}

	if(pParentObject)
	{	
		if(pObject->IsKindOf(GET_CLASS_INFO(CD3DObject)))
		{
			((CD3DObject *)pObject)->SetParent((CD3DObject *)pParentObject);	
			//pObject->AddUseRef();
		}
	}
	else if(IsAddToUSO)
	{
		if(IsResource)
		{
			m_USOFile.AddResource(pObject);
		}
		else
		{
			m_USOFile.AddObject(pObject);
		}
	}

	if(pObject->IsKindOf(GET_CLASS_INFO(CD3DWOWADTModel)))
	{
		m_pD3DViewPort->GetRender()->AddSceneObject((CD3DObject *)pObject);
	}	
	else if(pObject->IsKindOf(GET_CLASS_INFO(CD3DScene)))
	{
		m_pD3DViewPort->GetRender()->AddSceneObject((CD3DObject *)pObject);
	}
	else if(pObject->IsKindOf(GET_CLASS_INFO(CD3DObject)))
	{
		m_pD3DViewPort->GetRender()->AddObject((CD3DObject *)pObject);
	}
	
	if(pUSOObject==NULL)
	{
		pUSOObject=new CUSOObject();
		pUSOObject->SetParent(pParent);
		pUSOObject->SetBindObject(pObject);
	}	
	
	SetModifiedFlag(true);
	return pUSOObject;
}


CD3DObject * CUSOEditDoc::PickChildObject(CD3DObject * pObject,CD3DVector3 Point,CD3DVector3 Dir,FLOAT& MinDistance)
{
	MinDistance=MAX_HEIGHT;
	CD3DObject * pPickObject=NULL;

	for(UINT i=0;i<pObject->GetChildCount();i++)
	{
		CD3DObject * pD3DObject=pObject->GetChildByIndex(i);

		CD3DVector3 IntersectPoint;
		FLOAT Distance;
		if(pD3DObject->RayIntersect(Point,Dir,IntersectPoint,Distance,false))
		{
			if(Distance<MinDistance)
			{
				MinDistance=Distance;
				pPickObject=pD3DObject;
			}
		}
		CD3DObject * pChildObject=PickChildObject(pD3DObject,Point,Dir,Distance);
		if(pChildObject&&Distance<MinDistance)
		{
			MinDistance=Distance;
			pPickObject=pChildObject;
		}

	}
	return pPickObject;
}

void CUSOEditDoc::UpdateResources()
{
	CEasyMap<LPVOID,LPVOID> PtrMap;
	HTREEITEM item;

	//获取已有的资源

	//纹理
	item=m_pUSOTreeView->GetTreeCtrl().GetChildItem(m_pTextureResource->GetTreeItem());
	while(item)
	{
		CUSOObject * pObject=(CUSOObject *)m_pUSOTreeView->GetTreeCtrl().GetItemData(item);
		if(m_USOFile.FindResourceKey(pObject->GetBindObject()))
			PtrMap[pObject->GetBindObject()]=pObject->GetBindObject();
		else
			pObject->Release();
		item=m_pUSOTreeView->GetTreeCtrl().GetNextSiblingItem(item);
	}

	//FX
	item=m_pUSOTreeView->GetTreeCtrl().GetChildItem(m_pFXResource->GetTreeItem());
	while(item)
	{
		CUSOObject * pObject=(CUSOObject *)m_pUSOTreeView->GetTreeCtrl().GetItemData(item);
		if(m_USOFile.FindResourceKey(pObject->GetBindObject()))
			PtrMap[pObject->GetBindObject()]=pObject->GetBindObject();
		else
			pObject->Release();
		item=m_pUSOTreeView->GetTreeCtrl().GetNextSiblingItem(item);
	}
	

	//WOWM2模型
	item=m_pUSOTreeView->GetTreeCtrl().GetChildItem(m_pWOWM2ModelResource->GetTreeItem());
	while(item)
	{
		CUSOObject * pObject=(CUSOObject *)m_pUSOTreeView->GetTreeCtrl().GetItemData(item);
		if(m_USOFile.FindResourceKey(pObject->GetBindObject()))
			PtrMap[pObject->GetBindObject()]=pObject->GetBindObject();
		else
			pObject->Release();
		item=m_pUSOTreeView->GetTreeCtrl().GetNextSiblingItem(item);
	}

	

	//字体
	item=m_pUSOTreeView->GetTreeCtrl().GetChildItem(m_pFontResource->GetTreeItem());
	while(item)
	{
		CUSOObject * pObject=(CUSOObject *)m_pUSOTreeView->GetTreeCtrl().GetItemData(item);
		if(m_USOFile.FindResourceKey(pObject->GetBindObject()))
			PtrMap[pObject->GetBindObject()]=pObject->GetBindObject();
		else
			pObject->Release();
		item=m_pUSOTreeView->GetTreeCtrl().GetNextSiblingItem(item);
	}

	//其他
	item=m_pUSOTreeView->GetTreeCtrl().GetChildItem(m_pOtherResource->GetTreeItem());
	while(item)
	{
		CUSOObject * pObject=(CUSOObject *)m_pUSOTreeView->GetTreeCtrl().GetItemData(item);
		if(m_USOFile.FindResourceKey(pObject->GetBindObject()))
			PtrMap[pObject->GetBindObject()]=pObject->GetBindObject();
		else
			pObject->Release();
		item=m_pUSOTreeView->GetTreeCtrl().GetNextSiblingItem(item);
	}


	//添加新资源
	void * Pos=m_USOFile.GetFirstResourcePos();
	while(Pos)
	{
		CNameObject * pResource=m_USOFile.GetNextResource(Pos);
		if(PtrMap.Find(pResource)==NULL)
		{
			PtrMap[pResource]=pResource;
			CUSOObject * pObject=new CUSOObject();
			pObject->SetBindObject(pResource);
			pResource->AddUseRef();

			if(pResource->IsKindOf(GET_CLASS_INFO(CD3DTexture)))
			{
				pObject->SetParent(m_pTextureResource);
			}
			else if(pResource->IsKindOf(GET_CLASS_INFO(CD3DFX)))			
			{
				pObject->SetParent(m_pFXResource);
			}			
			else if(pResource->IsKindOf(GET_CLASS_INFO(CD3DWOWM2ModelResource)))
			{
				pObject->SetParent(m_pWOWM2ModelResource);
			}
			else if(pResource->IsKindOf(GET_CLASS_INFO(CD3DBaseFont)))
			{
				pObject->SetParent(m_pFontResource);				
			}
			else
			{	
				pObject->SetParent(m_pOtherResource);
			}
			pObject->SetTreeCtrl(&m_pUSOTreeView->GetTreeCtrl());
		}
	}
	
	m_pUSOTreeView->GetTreeCtrl().RedrawWindow();
	
	
	
}

void CUSOEditDoc::RePickResource()
{
	m_USOFile.ClearAllResource();
	for(int i=0;i<m_USOFile.GetObjectCount();i++)
	{
		m_USOFile.PickResource(m_USOFile.GetObjectByIndex(i));
	}
}



void CUSOEditDoc::OnImportX()
{
	// TODO: 在此添加命令处理程序代码
	CUSOObject * pSelectedUSOObject=m_pUSOTreeView->GetSelectedObject();
	CD3DObject * pParentObject=NULL;

	if(pSelectedUSOObject==NULL)
	{
		AfxGetMainWnd()->MessageBox(_T("请选择一个对象！"));
		return;
	}
	
	m_ImportDlg.m_DefualtFile=_T("*.x");
	m_ImportDlg.m_FileFliter=_T("D3D X File(*.x)|*.x|All Files(*.*)|*.*||");
	m_ImportDlg.m_ImportType=IMP_X;
	if(m_ImportDlg.DoModal()==IDOK)
	{
		
		CD3DWOWM2Model * pModel=new CD3DWOWM2Model();
		pModel->SetDevice(m_pD3DViewPort->GetDevice());

		if(m_ImportDlg.m_UseTextureDir)
		{
			CD3DTexture::ClearPath();
			CD3DTexture::AddPath(m_ImportDlg.m_TextureDir);
		}
		if(m_ImportDlg.m_UseFxDir)
		{

			CD3DFX::ClearPath();
			CD3DFX::AddPath(m_ImportDlg.m_FxDir);
		}
		if(pModel->LoadFromXFile(m_ImportDlg.m_ImportFilePath))
		{
			CUSOObject * pUSOObject=AddObject(pSelectedUSOObject,pModel,&GET_CLASS_INFO(CD3DWOWM2Model),true,false,m_ImportDlg.m_ImportFilePath);		

			if(pUSOObject==pSelectedUSOObject)
				pSelectedUSOObject->ReBindChilds();
			else
				pUSOObject->SetTreeCtrl(&m_pUSOTreeView->GetTreeCtrl());

			m_USOFile.PickResource(pModel);
			UpdateResources();
		}
		else
		{
			SAFE_RELEASE(pModel);
			AfxGetMainWnd()->MessageBox(_T("导入失败！"));				
		}

		if(m_ImportDlg.m_UseTextureDir)
		{
			CD3DTexture::ClearPath();
			CD3DTexture::AddPathList(((CUSOEditApp *)AfxGetApp())->GetTexturePathList());
		}
		if(m_ImportDlg.m_UseFxDir)
		{
			CD3DFX::ClearPath();
			CD3DFX::AddPathList(((CUSOEditApp *)AfxGetApp())->GetFXPathList());
		}
	}
}

//void CUSOEditDoc::OnImportMda()
//{
//	// TODO: 在此添加命令处理程序代码
//	CUSOObject * pSelectedUSOObject=m_pUSOTreeView->GetSelectedObject();
//	CD3DObject * pParentObject=NULL;
//
//	if(pSelectedUSOObject==NULL)
//	{
//		AfxGetMainWnd()->MessageBox(_T("请选择一个对象！"));
//		return;
//	}	
//
//	m_ImportDlg.m_DefualtFile="*.mda";
//	m_ImportDlg.m_FileFliter="MDA File(*.mda)|*.mda|All Files(*.*)|*.*||";
//	m_ImportDlg.m_ImportType=IMP_SKN;
//	if(m_ImportDlg.DoModal()==IDOK)
//	{
//		CUSOObject * pUSOObject=AddObject(pSelectedUSOObject,NULL,&GET_CLASS_INFO(CD3DFrameDynamicModel),false,m_ImportDlg.m_ImportFilePath);
//		CD3DFrameDynamicModel * pModel=(CD3DFrameDynamicModel *)pUSOObject->GetBindObject();
//		if(pUSOObject)
//		{
//			if(m_ImportDlg.m_UseTextureDir)
//			{
//
//				CD3DTexture::ClearPath();
//				CD3DTexture::AddPath(m_ImportDlg.m_TextureDir);
//			}
//			if(m_ImportDlg.m_UseFxDir)
//			{
//
//				CD3DFX::ClearPath();
//				CD3DFX::AddPath(m_ImportDlg.m_FxDir);
//			}		
//
//			if(!pModel->LoadFromMDA(m_ImportDlg.m_ImportFilePath))
//			{
//				AfxGetMainWnd()->MessageBox("导入失败！");
//				if(pUSOObject!=pSelectedUSOObject)
//				{
//					SAFE_RELEASE(pUSOObject);
//				}
//				return;
//			}
//
//			if(pUSOObject==pSelectedUSOObject)
//				pSelectedUSOObject->ReBindChilds();
//			else
//				pUSOObject->SetTreeCtrl(&m_pUSOTreeView->GetTreeCtrl());
//
//			m_USOFile.PickResource(pModel);
//			UpdateResources();
//
//			if(m_ImportDlg.m_UseTextureDir)
//			{			
//				CD3DTexture::AssignPathList(((CUSOEditApp *)AfxGetApp())->GetTexturePathList());
//			}
//			if(m_ImportDlg.m_UseFxDir)
//			{			
//				CD3DFX::AssignPathList(((CUSOEditApp *)AfxGetApp())->GetFXPathList());
//			}
//		}		
//	}
//}

//void CUSOEditDoc::OnImportMdl()
//{
//	// TODO: 在此添加命令处理程序代码
//	CUSOObject * pSelectedUSOObject=m_pUSOTreeView->GetSelectedObject();	
//
//	if(pSelectedUSOObject==NULL)
//	{
//		AfxGetMainWnd()->MessageBox(_T("请选择一个对象！"));
//		return;
//	}
//	
//	m_ImportDlg.m_DefualtFile="*.mdl";
//	m_ImportDlg.m_FileFliter="Model File(*.mdl)|*.mdl|All Files(*.*)|*.*||";
//	m_ImportDlg.m_ImportType=IMP_MDL;
//	if(m_ImportDlg.DoModal()==IDOK)
//	{
//		CUSOObject * pUSOObject=AddObject(pSelectedUSOObject,NULL,&GET_CLASS_INFO(CD3DStaticModel),false,m_ImportDlg.m_ImportFilePath);		
//		CD3DStaticModel * pModel=(CD3DStaticModel *)pUSOObject->GetBindObject();
//		if(pModel)
//		{
//			if(m_ImportDlg.m_UseTextureDir)
//			{
//
//				CD3DTexture::ClearPath();
//				CD3DTexture::AddPath(m_ImportDlg.m_TextureDir);
//			}
//			if(m_ImportDlg.m_UseFxDir)
//			{
//
//				CD3DFX::ClearPath();
//				CD3DFX::AddPath(m_ImportDlg.m_FxDir);
//			}
//			if(!pModel->LoadFromMDL(m_ImportDlg.m_ImportFilePath))
//			{
//				AfxGetMainWnd()->MessageBox("导入失败！");
//				if(pUSOObject!=pSelectedUSOObject)
//				{
//					SAFE_RELEASE(pUSOObject);
//				}
//				return;
//			}	
//
//			if(pUSOObject==pSelectedUSOObject)
//				pSelectedUSOObject->ReBindChilds();
//			else
//				pUSOObject->SetTreeCtrl(&m_pUSOTreeView->GetTreeCtrl());
//			m_USOFile.PickResource(pModel);
//			UpdateResources();
//
//			if(m_ImportDlg.m_UseTextureDir)
//			{			
//				CD3DTexture::AssignPathList(((CUSOEditApp *)AfxGetApp())->GetTexturePathList());
//			}
//			if(m_ImportDlg.m_UseFxDir)
//			{			
//				CD3DFX::AssignPathList(((CUSOEditApp *)AfxGetApp())->GetFXPathList());
//			}
//		}
//	}
//}


void CUSOEditDoc::OnImportUso()
{
	// TODO: 在此添加命令处理程序代码
	CUSOObject * pSelectedUSOObject=m_pUSOTreeView->GetSelectedObject();
	CD3DObject * pParentObject=NULL;

	if(pSelectedUSOObject==NULL)
	{
		AfxGetMainWnd()->MessageBox(_T("请选择一个对象！"));
		return;
	}
	
	m_ImportDlg.m_DefualtFile=_T("*.uso");
	m_ImportDlg.m_FileFliter=_T("Universal System Object Files(*.uso)|*.uso|All Files(*.*)|*.*||");
	m_ImportDlg.m_ImportType=IMP_USO;
	if(m_ImportDlg.DoModal()==IDOK)
	{		
		CUSOResourceManager UsoFile;
		UsoFile.AddObjectCreateFilter(&m_D3DObjectCreateFilter);

		if(UsoFile.Import(m_ImportDlg.m_ImportFilePath))
		{
			ImportUSO(pSelectedUSOObject,&UsoFile);
		}
		else
		{
			AfxGetMainWnd()->MessageBox(_T("导入失败！"));
		}
		
	}
}

void CUSOEditDoc::OnDeleteObj()
{
	// TODO: 在此添加命令处理程序代码

	CUSOObject * pSelectedUSOObject=m_pUSOTreeView->GetSelectedObject();
	CD3DObject * pObject=NULL;
	

	if(pSelectedUSOObject==NULL)
	{
		AfxGetMainWnd()->MessageBox(_T("请选择一个对象！"));
		return;
	}

	CUSOObject * pParentUSOObject=pSelectedUSOObject;
	while(pParentUSOObject->GetParent())
	{
		pParentUSOObject=pParentUSOObject->GetParent();
	}

	if(pParentUSOObject!=m_pObjects)
	{
		AfxGetMainWnd()->MessageBox(_T("选中的对象不可删除！"));
		return;
	}
	

	pObject=(CD3DObject *)pSelectedUSOObject->GetBindObject();
	if(pObject)
	{
		
		CString Msg;
		Msg.Format(_T("是否要删除对象<%s>？"),pObject->GetName());
		if(AfxGetMainWnd()->MessageBox(Msg,NULL,MB_YESNOCANCEL)==IDYES)
		{
			bool IsRootObject=false;
			if(pSelectedUSOObject->GetParent()==NULL||
				pSelectedUSOObject->GetParent()->GetBindObject()==NULL)
			{
				IsRootObject=true;
			}			
			
			if(pObject->IsKindOf(GET_CLASS_INFO(CD3DObject)))
			{
				pSelectedUSOObject->Release();
				if(!IsRootObject)
					pObject->Release();	

				//if(pObject->IsKindOf(GET_CLASS_INFO(CD3DWOWADTModel)))
				//{
				//	m_pD3DViewPort->GetRender()->UpdateSceneTree();
				//}

				SetModifiedFlag(true);
			}	
			else
			{
				AfxGetMainWnd()->MessageBox(_T("选中的对象不可删除！"));
				return;
			}

			if(IsRootObject)
			{
				if(m_USOFile.DeleteObject(pObject))
				{
					SetModifiedFlag(true);
				}
				else
				{
					AfxGetMainWnd()->MessageBox(_T("从USO中删除对象失败！"));
				}
			}
		}
	}
}

void CUSOEditDoc::OnSwitchVisible()
{
	// TODO: 在此添加命令处理程序代码
	CUSOObject * pSelectedUSOObject=m_pUSOTreeView->GetSelectedObject();	
	if(pSelectedUSOObject==NULL)
	{		
		return;
	}
	CNameObject * pBindObject=pSelectedUSOObject->GetBindObject();
	if(pBindObject)
	{
		if(pBindObject->IsKindOf(GET_CLASS_INFO(CD3DObject)))
		{
			CD3DObject * pD3DObject=(CD3DObject *)pBindObject;
			pD3DObject->SetVisible(!pD3DObject->IsVisible());
			m_pUSOPropertyView->RefreshObjectProperty();
		}
		else if(pBindObject->IsKindOf(GET_CLASS_INFO(CD3DSubMesh)))
		{
			CD3DSubMesh * pD3DSubMesh=(CD3DSubMesh *)pBindObject;
			pD3DSubMesh->SetVisible(!pD3DSubMesh->IsVisible());
			m_pUSOPropertyView->RefreshObjectProperty();
		}
	}
	
}

void CUSOEditDoc::OnHideAllSubmesh()
{
	// TODO: 在此添加命令处理程序代码
	// TODO: 在此添加命令处理程序代码
	CUSOObject * pSelectedUSOObject=m_pUSOTreeView->GetSelectedObject();	
	if(pSelectedUSOObject==NULL)
	{		
		return;
	}
	CNameObject * pBindObject=pSelectedUSOObject->GetBindObject();
	if(pBindObject)
	{
		if(pBindObject->IsKindOf(GET_CLASS_INFO(CD3DObject)))
		{
			CD3DObject * pD3DObject=(CD3DObject *)pBindObject;
			for(int i=0;i<pD3DObject->GetSubMeshCount();i++)
			{
				pD3DObject->GetSubMesh(i)->SetVisible(false);
			}
		}
		
	}
}

void CUSOEditDoc::OnShowAllSubmesh()
{
	// TODO: 在此添加命令处理程序代码
	CUSOObject * pSelectedUSOObject=m_pUSOTreeView->GetSelectedObject();	
	if(pSelectedUSOObject==NULL)
	{		
		return;
	}
	CNameObject * pBindObject=pSelectedUSOObject->GetBindObject();
	if(pBindObject)
	{
		if(pBindObject->IsKindOf(GET_CLASS_INFO(CD3DObject)))
		{
			CD3DObject * pD3DObject=(CD3DObject *)pBindObject;
			for(int i=0;i<pD3DObject->GetSubMeshCount();i++)
			{
				pD3DObject->GetSubMesh(i)->SetVisible(true);
			}
		}

	}
}


void CUSOEditDoc::OnClearResource()
{
	// TODO: 在此添加命令处理程序代码
	//清理未用的资源
	for(UINT j=0;j<m_pResources->GetChildCount();j++)
	{
		CUSOObject * pResourceTree=m_pResources->GetChildByIndex(j);
		for(int i=pResourceTree->GetChildCount()-1;i>=0;i--)
		{
			CUSOObject * pUSOObject=pResourceTree->GetChildByIndex(i);
			CNameObject * pObject=pUSOObject->GetBindObject();
			if(pObject)
			{
				if(pObject->GetUseRef()<=2)
				{
					if(!m_USOFile.DeleteResource(pObject->GetName()))
					{
						AfxGetMainWnd()->MessageBox(_T("从USO中删除资源失败！"));
					}
					pUSOObject->Release();
				}
			}
		}
	}	
}



void CUSOEditDoc::OnEditCut()
{
	// TODO: 在此添加命令处理程序代码
	OnEditCopy();
	OnDeleteObj();
}

void CUSOEditDoc::OnEditCopy()
{
	// TODO: 在此添加命令处理程序代码
	CUSOObject * pSelectedUSOObject=m_pUSOTreeView->GetSelectedObject();
	CD3DObject * pObject=NULL;


	if(pSelectedUSOObject==NULL)
	{
		AfxGetMainWnd()->MessageBox(_T("请选择一个对象！"));
		return;
	}

	pObject=(CD3DObject *)pSelectedUSOObject->GetBindObject();
	if(pObject&&pObject->IsKindOf(GET_CLASS_INFO(CD3DObject)))
	{
		CMemFileAccessor MemFile;
		CUSOResourceManager UsoFile;

		UsoFile.AddObject(pObject);
		UsoFile.Export(&MemFile);
		
		int Size=MemFile.GetWriteSize();		
		MemFile.AllocBuffer(Size);
		MemFile.Seek(0,IFileAccessor::seekBegin);
		UsoFile.Export(&MemFile);
		
		MemFile.CopyToClipBroad(m_USOClipFormat);
	}
	else
	{
		AfxGetMainWnd()->MessageBox(_T("请选择一个对象！"));
	}
}

void CUSOEditDoc::OnEditPaste()
{
	// TODO: 在此添加命令处理程序代码
	CUSOObject * pSelectedUSOObject=m_pUSOTreeView->GetSelectedObject();
	CD3DObject * pObject=NULL;


	if(pSelectedUSOObject==NULL)
	{
		AfxGetMainWnd()->MessageBox(_T("请选择一个对象！"));
		return;
	}
	CMemFileAccessor MemFile;
	CUSOResourceManager UsoFile;

	UsoFile.AddObjectCreateFilter(&m_D3DObjectCreateFilter);

	MemFile.CopyFromClipBroad(m_USOClipFormat);
	if(MemFile.GetSize()>0)
	{
		UsoFile.Import(&MemFile);
		ImportUSO(pSelectedUSOObject,&UsoFile);
	}

}

bool CUSOEditDoc::ImportUSO(CUSOObject * pParent,CUSOResourceManager * pResourceManager)
{
	for(int i=0;i<pResourceManager->GetObjectCount();i++)
	{
		CNameObject * pObject=pResourceManager->GetObjectByIndex(i);

		CNameObject * pExistObject=m_USOFile.GetObjectRecursive(pObject->GetName());
		if(pExistObject)
		{
			CEasyString Msg;
			Msg.Format(_T("对象[%s]已经存在，是否要合并？"),pObject->GetName());
			int MergeResult=AfxGetMainWnd()->MessageBox(Msg,NULL,MB_YESNOCANCEL);

			if(MergeResult==IDYES)
			{
				pObject->CloneFrom(pExistObject);
			}	
		}

		CUSOObject * pUSOObject=AddObject(pParent,pObject,&(pObject->GetClassInfo()),true,false,pObject->GetName());		

		if(pUSOObject)
		{				
			pObject->AddUseRef();
			if(pUSOObject==pParent)
				pParent->ReBindChilds();
			else
				pUSOObject->SetTreeCtrl(&m_pUSOTreeView->GetTreeCtrl());

			m_USOFile.PickResource(pObject);
			UpdateResources();
		}
	}
	return true;
}
void CUSOEditDoc::OnImportM2()
{
	// TODO: 在此添加命令处理程序代码
	CUSOObject * pSelectedUSOObject=m_pUSOTreeView->GetSelectedObject();	

	if(pSelectedUSOObject==NULL)
	{
		AfxGetMainWnd()->MessageBox(_T("请选择一个对象！"));
		return;
	}	

	if(m_WOWM2ImportDlg.DoModal()==IDOK)
	{
		
		CD3DWOWM2Model * pModel=new CD3DWOWM2Model();
		if(pModel)
		{		
			pModel->SetDevice(m_pD3DViewPort->GetDevice());
			if(!pModel->LoadFromFile(m_WOWM2ImportDlg.m_ModelFileName))
			{
				AfxGetMainWnd()->MessageBox(_T("导入失败！"));
				SAFE_RELEASE(pModel);
				return;
			}			
					
			pModel->Play(0,0,0,true);


			CUSOObject * pUSOObject=AddObject(pSelectedUSOObject,pModel,&GET_CLASS_INFO(CD3DWOWM2Model),true,false,m_WOWM2ImportDlg.m_ModelFileName);		
			pUSOObject->SetTreeCtrl(&m_pUSOTreeView->GetTreeCtrl());
			//m_USOFile.PickResource(pModel);
			UpdateResources();				
		}
	}
}
void CUSOEditDoc::OnImportM2Character()
{
	// TODO: 在此添加命令处理程序代码
	// TODO: 在此添加命令处理程序代码
	CUSOObject * pSelectedUSOObject=m_pUSOTreeView->GetSelectedObject();	

	if(pSelectedUSOObject==NULL)
	{
		AfxGetMainWnd()->MessageBox(_T("请选择一个对象！"));
		return;
	}	

	if(m_WOWCharImportDlg.DoModal()==IDOK)
	{
		
		CD3DWOWM2CharacterModel * pModel=new CD3DWOWM2CharacterModel();
		if(pModel)
		{			
			pModel->SetDevice(m_pD3DViewPort->GetDevice());
			if(!pModel->LoadCharacter(m_WOWCharImportDlg.m_Race+1,m_WOWCharImportDlg.m_Sex))
			{
				AfxGetMainWnd()->MessageBox(_T("导入失败！"));				
				SAFE_RELEASE(pModel);
				return;
			}	

			pModel->Play(0,0,0,true);	
				

			CUSOObject * pUSOObject=AddObject(pSelectedUSOObject,pModel,&GET_CLASS_INFO(CD3DWOWM2CharacterModel),true,false,pModel->GetName());		
			pUSOObject->SetTreeCtrl(&m_pUSOTreeView->GetTreeCtrl());
			UpdateResources();	
			
		}
	}
}

void CUSOEditDoc::OnImportM2Creature()
{
	// TODO: 在此添加命令处理程序代码
	CUSOObject * pSelectedUSOObject=m_pUSOTreeView->GetSelectedObject();	

	if(pSelectedUSOObject==NULL)
	{
		AfxGetMainWnd()->MessageBox(_T("请选择一个对象！"));
		return;
	}	

	if(m_WOWCreatureImport.DoModal()==IDOK)
	{

		CD3DWOWM2CharacterModel * pModel=new CD3DWOWM2CharacterModel();
		if(pModel)
		{			
			pModel->SetDevice(m_pD3DViewPort->GetDevice());
			if(!pModel->LoadCreature(m_WOWCreatureImport.m_CreatureDisplayID))
			{
				AfxGetMainWnd()->MessageBox(_T("导入失败！"));				
				SAFE_RELEASE(pModel);
				return;
			}	
			
			pModel->Play(0,0,0,true);	


			CUSOObject * pUSOObject=AddObject(pSelectedUSOObject,pModel,&GET_CLASS_INFO(CD3DWOWM2CharacterModel),true,false,pModel->GetName());		
			pUSOObject->SetTreeCtrl(&m_pUSOTreeView->GetTreeCtrl());
			UpdateResources();	

		}
	}
}

void CUSOEditDoc::OnImportM2Item()
{
	// TODO: 在此添加命令处理程序代码

	CUSOObject * pSelectedUSOObject=m_pUSOTreeView->GetSelectedObject();	

	if(pSelectedUSOObject==NULL)
	{
		AfxGetMainWnd()->MessageBox(_T("请选择一个对象！"));
		return;
	}	

	if(m_WOWItemImportDlg.DoModal()==IDOK)
	{

		
		CD3DWOWM2ItemModel * pModel=new CD3DWOWM2ItemModel();
		if(pModel)
		{			
			pModel->SetDevice(m_pD3DViewPort->GetDevice());
			if(!pModel->LoadFromFile(m_WOWItemImportDlg.m_ModelFileName))
			{
				AfxGetMainWnd()->MessageBox(_T("导入失败！"));
				SAFE_RELEASE(pModel);
				return;
			}	

			pModel->SetItemDisplayID(m_WOWItemImportDlg.m_ItemDisplayID);
			pModel->SetItemHandType(m_WOWItemImportDlg.m_ItemHandType);
			pModel->BuildModel();
		
			pModel->Play(0,0,0,true);

			CUSOObject * pUSOObject=AddObject(pSelectedUSOObject,pModel,&GET_CLASS_INFO(CD3DWOWM2ItemModel),true,false,m_WOWItemImportDlg.m_ModelFileName);		
			pUSOObject->SetTreeCtrl(&m_pUSOTreeView->GetTreeCtrl());
			UpdateResources();	
		}

	}
	
}


void CUSOEditDoc::OnImportWmo()
{
	// TODO: 在此添加命令处理程序代码
	CUSOObject * pSelectedUSOObject=m_pUSOTreeView->GetSelectedObject();	

	if(pSelectedUSOObject==NULL)
	{
		AfxGetMainWnd()->MessageBox(_T("请选择一个对象！"));
		return;
	}	

	if(m_WOWWMOImportDlg.DoModal()==IDOK)
	{


		CD3DWOWWMOModel * pModel=new CD3DWOWWMOModel();
		if(pModel)
		{			
			pModel->SetDevice(m_pD3DViewPort->GetDevice());
			if(!pModel->LoadFromFile(m_WOWWMOImportDlg.m_ModelFileName))
			{
				AfxGetMainWnd()->MessageBox(_T("导入失败！"));
				SAFE_RELEASE(pModel);
				return;
			}	

			
		
			//pModel->Play(0,0,0,true);

			CUSOObject * pUSOObject=AddObject(pSelectedUSOObject,pModel,&GET_CLASS_INFO(CD3DWOWM2ItemModel),true,false,m_WOWWMOImportDlg.m_ModelFileName);		
			pUSOObject->SetTreeCtrl(&m_pUSOTreeView->GetTreeCtrl());
			UpdateResources();	
		}

	}
}

void CUSOEditDoc::OnImportAdt()
{
	// TODO: 在此添加命令处理程序代码
	CUSOObject * pSelectedUSOObject=m_pUSOTreeView->GetSelectedObject();	

	if(pSelectedUSOObject==NULL)
	{
		AfxGetMainWnd()->MessageBox(_T("请选择一个对象！"));
		return;
	}	

	m_WOWADTImportDlg.SetD3DDevice(m_pD3DViewPort->GetDevice());
	if(m_WOWADTImportDlg.DoModal()==IDOK)
	{

		CEasyArray<CEasyString>& FileNameList=m_WOWADTImportDlg.m_AreaFileNameList;
	

		m_CurMapID=m_WOWADTImportDlg.m_MapID;

		for(UINT i=0;i<FileNameList.GetCount();i++)
		{
			CD3DWOWADTModel * pModel=new CD3DWOWADTModel();
			if(pModel)
			{		
				pModel->SetDevice(m_pD3DViewPort->GetDevice());
				if(pModel->LoadFromFile(FileNameList[i],m_WOWADTImportDlg.m_IsBigAlphaMask))
				{
					CD3DVector3 Pos=pModel->GetBoundingBox()->GetCenter();

					CUSOObject * pUSOObject=AddObject(pSelectedUSOObject,pModel,
						&GET_CLASS_INFO(CD3DWOWM2ItemModel),true,false,FileNameList[i]);		
					pUSOObject->SetTreeCtrl(&m_pUSOTreeView->GetTreeCtrl());
				}
				else
				{
					CEasyString Msg;
					Msg.Format(_T("[%s]导入失败"),
						(LPCTSTR)FileNameList[i]);
					AfxGetMainWnd()->MessageBox(Msg);
					SAFE_RELEASE(pModel);					
				}					
			}
		}

		//m_pD3DViewPort->GetRender()->UpdateSceneTree();
		UpdateResources();
	}
}

void CUSOEditDoc::OnImportWdl()
{
	// TODO: 在此添加命令处理程序代码
	CUSOObject * pSelectedUSOObject=m_pUSOTreeView->GetSelectedObject();	

	if(pSelectedUSOObject==NULL)
	{
		AfxGetMainWnd()->MessageBox(_T("请选择一个对象！"));
		return;
	}	

	m_WOWADTImportDlg.SetD3DDevice(m_pD3DViewPort->GetDevice());
	if(m_WOWADTImportDlg.DoModal()==IDOK)
	{
	
		m_CurMapID=m_WOWADTImportDlg.m_MapID;

		
		CD3DWOWWDLModel * pModel=new CD3DWOWWDLModel();
		if(pModel)
		{		
			pModel->SetDevice(m_pD3DViewPort->GetDevice());
			if(pModel->LoadFromFile(m_WOWADTImportDlg.m_WDLFileName))
			{
				CD3DVector3 Pos=pModel->GetBoundingBox()->GetCenter();

				CUSOObject * pUSOObject=AddObject(pSelectedUSOObject,pModel,
					&GET_CLASS_INFO(CD3DWOWM2ItemModel),true,false,m_WOWADTImportDlg.m_WDLFileName);		
				pUSOObject->SetTreeCtrl(&m_pUSOTreeView->GetTreeCtrl());
			}
			else
			{
				CEasyString Msg;
				Msg.Format(_T("[%s]导入失败"),
					(LPCTSTR)m_WOWADTImportDlg.m_WDLFileName);
				AfxGetMainWnd()->MessageBox(Msg);
				SAFE_RELEASE(pModel);					
			}					
		}
		

		//m_pD3DViewPort->GetRender()->UpdateSceneTree();
		UpdateResources();
	}
}


void CUSOEditDoc::OnImportTexture()
{
	// TODO: 在此添加命令处理程序代码
}

void CUSOEditDoc::OnImportFx()
{
	// TODO: 在此添加命令处理程序代码
	CUSOObject * pSelectedUSOObject=m_pFXResource;
	

	m_ImportDlg.m_DefualtFile=_T("*.fx");
	m_ImportDlg.m_FileFliter=_T("Effect Files(*.fx)|*.fx|All Files(*.*)|*.*||");
	m_ImportDlg.m_ImportType=IMP_FX;
	if(m_ImportDlg.DoModal()==IDOK)
	{	

		CD3DFX * pFX=m_pD3DViewPort->GetDevice()->GetFXManager()->LoadFX(m_ImportDlg.m_ImportFilePath);
		if(pFX)
		{
			CUSOObject * pUSOObject=AddObject(pSelectedUSOObject,pFX,&(pFX->GetClassInfo()),true,true,m_ImportDlg.m_ImportFilePath);
			if(pUSOObject)
			{				
				pUSOObject->SetTreeCtrl(&m_pUSOTreeView->GetTreeCtrl());
			}
			else
			{
				SAFE_RELEASE(pFX);
				AfxGetMainWnd()->MessageBox(_T("添加对象失败！"));
			}
		}		
		else
		{
			AfxGetMainWnd()->MessageBox(_T("导入失败！"));
		}

	}
}

void CUSOEditDoc::OnImportPreviewObject()
{
	// TODO: 在此添加命令处理程序代码
	if(m_pPreviewUSOObject)
	{
		CUSOObject * pSelectedUSOObject=m_pUSOTreeView->GetSelectedObject();	

		if(pSelectedUSOObject==NULL)
		{
			AfxGetMainWnd()->MessageBox(_T("请选择一个对象！"));
			return;
		}	
		CNameObject * pPreviewObject=m_pPreviewUSOObject->GetBindObject();
		if(pPreviewObject)
		{
			CD3DObject * pObject=(CD3DObject *)pPreviewObject->GetClassInfo().CreateObject();
			pObject->SetDevice(m_pD3DViewPort->GetDevice());
			if(pObject->CloneFrom(pPreviewObject))
			{
				CUSOObject * pUSOObject=AddObject(pSelectedUSOObject,pObject,
					&(pPreviewObject->GetClassInfo()),true,false,_T(""));
				pUSOObject->SetTreeCtrl(&m_pUSOTreeView->GetTreeCtrl());
				UpdateResources();
			}
			else
			{
				SAFE_RELEASE(pObject);
				AfxMessageBox(_T("导入失败"));
			}
			
		}
		
	}
	else
	{
		AfxMessageBox(_T("当前没有预览中的对象"));
	}
}

void CUSOEditDoc::OnCreateScene()
{
	// TODO: 在此添加命令处理程序代码
	CUSOObject * pSelectedUSOObject=m_pUSOTreeView->GetSelectedObject();	

	if(pSelectedUSOObject==NULL)
	{
		AfxGetMainWnd()->MessageBox(_T("请选择一个对象！"));
		return;
	}	

	CD3DScene * pModel=new CD3DScene();
	if(pModel)
	{			
		pModel->SetDevice(m_pD3DViewPort->GetDevice());
				

		CUSOObject * pUSOObject=AddObject(pSelectedUSOObject,pModel,
			&GET_CLASS_INFO(CD3DWOWM2ItemModel),true,false,_T("场景"));		
		pUSOObject->SetTreeCtrl(&m_pUSOTreeView->GetTreeCtrl());
					
	}
}




void CUSOEditDoc::OnShowRanderPerformance()
{
	// TODO: 在此添加命令处理程序代码
	m_RenderPerformance.ShowWindow(SW_SHOW);
}




void CUSOEditDoc::OnCameraSetting()
{
	// TODO: 在此添加命令处理程序代码
	m_CameraSetting.ShowWindow(SW_SHOW);
}



void CUSOEditDoc::OnLookObject()
{
	// TODO: 在此添加命令处理程序代码
	CUSOObject * pSelectedUSOObject=m_pUSOTreeView->GetSelectedObject();	
	if(pSelectedUSOObject==NULL)
	{		
		return;
	}
	if(CUSOEditApp::GetInstance()->GetCameraSetting().CameraMode!=CAMERA_MODE_SCENE)
	{
		AfxMessageBox(_T("必须在场景模式使用"));
		return;
	}
	CNameObject * pBindObject=pSelectedUSOObject->GetBindObject();
	if(pBindObject)
	{
		if(pBindObject->IsKindOf(GET_CLASS_INFO(CD3DObject)))
		{
			CD3DObject * pD3DObject=(CD3DObject *)pBindObject;

			CD3DBoundingBox * pBBox=pD3DObject->GetBoundingBox();
			if(pBBox&&m_pD3DViewPort)
			{
				CD3DBoundingBox BBox=(*pBBox)*pD3DObject->GetWorldMatrix();
				FLOAT X=(BBox.m_Max.x+BBox.m_Min.x)/2;
				FLOAT Y=(BBox.m_Max.y+BBox.m_Min.y)/2;
				FLOAT Z=(BBox.m_Max.z+BBox.m_Min.z)/2;
				CD3DVector3 Size=BBox.GetSize();
				FLOAT Alpha=D3DX_PI*47/180;
				FLOAT Rate=(float)CUSOEditApp::GetInstance()->GetViewportSetting().ViewportWidth/
					(float)CUSOEditApp::GetInstance()->GetViewportSetting().ViewportHeight;
				FLOAT LenY=(Size.y/2)/tan(Alpha/2);
				FLOAT LenX=(Size.x/2/Rate)/tan(Alpha/2);
				Z-=Size.z/2+max(LenX,LenY);
				m_pD3DViewPort->MoveCamera(X,Y,Z,0,0);
			}
			
		}

	}
}

void CUSOEditDoc::OnResourceStat()
{
	// TODO: 在此添加命令处理程序代码
	//UINT DataSize=0;
	//UINT ObjectCount=0;
	//for(int i=0;i<m_USOFile.GetResourceCount();i++)
	//{
	//	CNameObject * pResource=m_USOFile.GetResourceByIndex(i);
	//	
	//	if(pResource->IsKindOf(GET_CLASS_INFO(CD3DWOWM2ModelResource)))
	//	{
	//		ObjectCount++;
	//		DataSize+=((CD3DWOWM2ModelResource *)pResource)->CaculateDataSize();
	//	}			
	//}
	//CEasyString Msg;
	//Msg.Format("一共有CD3DWOWM2ModelResource对象%u个,数据大小%u",
	//	ObjectCount,DataSize);
	//AfxMessageBox(Msg);
}


void CUSOEditDoc::OnViewTexture()
{
	CUSOObject * pSelectedUSOObject=m_pUSOTreeView->GetSelectedObject();	
	if(pSelectedUSOObject)
	{		
		CNameObject * pBindObject=pSelectedUSOObject->GetBindObject();
		if(pBindObject)
		{
			if(pBindObject->IsKindOf(GET_CLASS_INFO(CD3DTexture)))
			{
				CD3DTexture * pTexture=(CD3DTexture *)pBindObject;

				m_TextureViewer.ShowTexture(pTexture);
				m_TextureViewer.ShowWindow(SW_SHOW);
			}
		}
	}	
}
void CUSOEditDoc::OnSystemLightSetting()
{
	// TODO: 在此添加命令处理程序代码
	if(m_pD3DViewPort)
		m_SystemLightSetting.SetLight(m_pD3DViewPort->GetSystemLight());
	m_SystemLightSetting.ShowWindow(SW_SHOW);
}

void CUSOEditDoc::OnFileViewer()
{
	// TODO: 在此添加命令处理程序代码
	CUSOEditApp::GetInstance()->GetFileViewer().ShowWindow(SW_SHOW);
}


void CUSOEditDoc::SetPreviewObject(LPCTSTR szFileName,int Type)
{
	CD3DObject * pPreviewObject=NULL;
	SAFE_RELEASE(m_pPreviewUSOObject);	
	switch(Type)
	{
	case CDlgFileViewer::ITEM_TYPE_M2:
		{
			CD3DWOWM2Model * pModel=new CD3DWOWM2Model();
			pModel->SetDevice(m_pD3DViewPort->GetDevice());

			if(!pModel->LoadFromFile(szFileName))
			{
				CEasyString Msg;
				Msg.Format(_T("%s打开失败"),szFileName);
				AfxGetMainWnd()->MessageBox(Msg);
				SAFE_RELEASE(pModel);
				return;
			}	
			
			pModel->Play(0,0,0,true);
			pPreviewObject=pModel;
		}
		break;
	case CDlgFileViewer::ITEM_TYPE_WMO:
		{
			CD3DWOWWMOModel * pModel=new CD3DWOWWMOModel();
			pModel->SetDevice(m_pD3DViewPort->GetDevice());

			if(!pModel->LoadFromFile(szFileName))
			{
				CEasyString Msg;
				Msg.Format(_T("%s打开失败"),szFileName);
				AfxGetMainWnd()->MessageBox(Msg);
				SAFE_RELEASE(pModel);
				return;
			}			

			
			pPreviewObject=pModel;
		}
		break;
	}
	if(pPreviewObject)
	{
		m_pPreviewUSOObject=new CUSOObject();
		m_pPreviewUSOObject->SetParent(m_pPreview);
		m_pPreviewUSOObject->SetBindObject(pPreviewObject);
		m_pPreviewUSOObject->SetTreeCtrl(&m_pUSOTreeView->GetTreeCtrl());
		m_pD3DViewPort->GetRender()->AddObject(pPreviewObject);
	}
}

CUSOObject * CUSOEditDoc::GetSelectedObject()
{
	if(m_pUSOTreeView)
	{
		return m_pUSOTreeView->GetSelectedObject();
	}
	return NULL;
}

void CUSOEditDoc::OnSaveDepthTexture()
{
	// TODO: 在此添加命令处理程序代码
	CFileDialog Dlg(false,_T("*.png"),_T("Depth.png"));

	if(Dlg.DoModal()==IDOK)
	{
		m_pD3DViewPort->SaveDepthTexture(Dlg.GetPathName());
	}
}





void CUSOEditDoc::OnExportToDir()
{
	// TODO: 在此添加命令处理程序代码
	BROWSEINFO bi;
	TCHAR FilePath[MAX_PATH];
	LPITEMIDLIST pidl;

	ZeroMemory(&bi,sizeof(bi));

	//bi.pszDisplayName=FilePath;
	bi.lpszTitle=_T("请选择目标目录");
	bi.ulFlags=BIF_RETURNONLYFSDIRS|BIF_USENEWUI;
	pidl=::SHBrowseForFolder(&bi);
	if(pidl!=NULL)
	{
		::SHGetPathFromIDList(pidl,FilePath);
		IMalloc *pMalloc=NULL;
		::SHGetMalloc(&pMalloc);
		pMalloc->Free(pidl);
		pMalloc->Release();
		pMalloc=NULL;	

		RePickResource();
		if(!m_USOFile.ExportToDir(FilePath))		
		{
			AfxGetMainWnd()->MessageBox(_T("导出失败！"));
		}
		
	}	
}

void CUSOEditDoc::OnSenceTreeView()
{
	// TODO: 在此添加命令处理程序代码
	m_SenceTreeView.ShowWindow(SW_SHOW);
}



void CUSOEditDoc::OnHeightTest()
{
	// TODO: 在此添加命令处理程序代码
	m_HeightTest.ShowWindow(SW_SHOW);
}


