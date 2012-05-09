/****************************************************************************/
/*                                                                          */
/*      �ļ���:    USOObject.cpp                                            */
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
/*      �ļ���:    USOObject.cpp                                            */
/*      ��������:  2009��09��25��                                           */
/*      ����:      Sagasarate                                               */
/*                                                                          */
/*      �������Ȩ��Sagasarate(sagasarate@sina.com)����                     */
/*      ����Խ�����������κ���ҵ�ͷ���ҵ�����������                      */
/*      ���뱣���˰�Ȩ����                                                  */
/*                                                                          */
/****************************************************************************/
#include "stdafx.h"

IMPLEMENT_CLASS_INFO(CUSOObject,CTreeObject);

CUSOObject::CUSOObject(void):CTreeObject()
{
	m_pParent=NULL;
	m_pBindObject=NULL;
	m_pTreeCtrl=NULL;
	m_hTreeItem=NULL;		
}

CUSOObject::~CUSOObject(void)
{
	Destory();
	//for(int i=(int)m_ChildList.size()-1;i>=0;i--)
	//{
	//	SAFE_RELEASE(m_ChildList[i]);
	//}
	//m_ChildList.clear();

	if(m_pTreeCtrl&&m_hTreeItem)
	{
		if(::IsWindow(m_pTreeCtrl->GetSafeHwnd()))
			m_pTreeCtrl->DeleteItem(m_hTreeItem);
		m_hTreeItem=NULL;
	}
	//if(m_pParent)
	//	m_pParent->DeleteChild(this);

	
	SAFE_RELEASE(m_pBindObject);
	
}

void CUSOObject::SetBindObject(CNameObject * pBindObject,bool CreateChild)
{
	SAFE_RELEASE(m_pBindObject);
	m_pBindObject=pBindObject;		
	m_pBindObject->SetParam(this);
	//m_pBindObject->AddUseRef();
	RebulidTreeItem();
	if(CreateChild)
		BindChilds();	
}

CNameObject * CUSOObject::GetBindObject()
{
	return m_pBindObject;
}


void CUSOObject::SetParent(CUSOObject * pParent)
{
	CTreeObject::SetParent(pParent);

	if(m_pTreeCtrl&&m_hTreeItem)
		m_pTreeCtrl->DeleteItem(m_hTreeItem);
	
	RebulidTreeItem();
	
}

void CUSOObject::SetTreeCtrl(CTreeCtrl * pTreeCtrl)
{
	if(m_pTreeCtrl&&m_hTreeItem)
	{
		m_pTreeCtrl->DeleteItem(m_hTreeItem);
		m_hTreeItem=NULL;
	}
	m_pTreeCtrl=pTreeCtrl;
	RebulidTreeItem();	
}

HTREEITEM CUSOObject::GetTreeItem()
{
	return m_hTreeItem;
}



void CUSOObject::RebulidTreeItem()
{
	
	if(m_pTreeCtrl&&m_hTreeItem)
	{
		m_pTreeCtrl->DeleteItem(m_hTreeItem);
		m_hTreeItem=NULL;
	}
	if(m_pTreeCtrl)
	{
		CString Name=BulidObjectName();
		if(m_pParent)
			m_hTreeItem=m_pTreeCtrl->InsertItem(Name,0,0,GetParent()->GetTreeItem());
		else
			m_hTreeItem=m_pTreeCtrl->InsertItem(Name,0,0,NULL);
		m_pTreeCtrl->SetItemData(m_hTreeItem,(DWORD_PTR)this);
	}
	for(UINT i=0;i<GetChildCount();i++)
	{
		GetChildByIndex(i)->SetTreeCtrl(m_pTreeCtrl);
	}
}

void CUSOObject::UpdateTreeItem(bool IsRecursive)
{
	if(m_pTreeCtrl&&m_hTreeItem)
	{
		CString Name=BulidObjectName();
		m_pTreeCtrl->SetItemText(m_hTreeItem,Name);		
	}
	if(IsRecursive)
	{
		for(UINT i=0;i<GetChildCount();i++)
		{
			GetChildByIndex(i)->UpdateTreeItem(IsRecursive);
		}
	}
}

CString CUSOObject::BulidObjectName()
{
	CString Name;	
	Name=GetTypeName();
	if(_tcslen(GetName()))
		Name=Name+'['+GetName()+']';
	if(m_pBindObject)
		Name=Name+'<'+m_pBindObject->GetName()+'>';
	return Name;
}

CString CUSOObject::GetTypeName()
{
	CString Name;
	
	if(m_pBindObject==NULL)
		return Name;

	Name.Format(_T("<%s>"),
		m_pBindObject->GetClassInfo().ClassName);

		
	if(m_pBindObject->IsKindOf(GET_CLASS_INFO(CD3DDummy)))
		Name=_T("Dummy");
		
	if(m_pBindObject->IsKindOf(GET_CLASS_INFO(CD3DCamera)))
		Name=_T("�����");
		
	if(m_pBindObject->IsKindOf(GET_CLASS_INFO(CD3DLight)))
		Name=_T("�ƹ�");
		
	if(m_pBindObject->IsKindOf(GET_CLASS_INFO(CD3DScene)))
		Name=_T("����");
			
	if(m_pBindObject->IsKindOf(GET_CLASS_INFO(CD3DObjectResource)))
		Name="ģ����Դ";
		
	if(m_pBindObject->IsKindOf(GET_CLASS_INFO(CD3DWOWM2Model)))
		Name="WOWM2ģ��";

	if(m_pBindObject->IsKindOf(GET_CLASS_INFO(CD3DWOWM2CharacterModel)))
		Name="WOWM2��ɫģ��";

	if(m_pBindObject->IsKindOf(GET_CLASS_INFO(CD3DWOWM2ModelResource)))
		Name="WOWM2ģ����Դ";
		
	if(m_pBindObject->IsKindOf(GET_CLASS_INFO(CD3DTexture)))
		Name="����";
		
	if(m_pBindObject->IsKindOf(GET_CLASS_INFO(CD3DTextTexture)))
		Name="��������";
		
	if(m_pBindObject->IsKindOf(GET_CLASS_INFO(CD3DIFLTexture)))
		Name="��������";
		
	if(m_pBindObject->IsKindOf(GET_CLASS_INFO(CD3DFX)))
		Name="FX";

	if(m_pBindObject->IsKindOf(GET_CLASS_INFO(CD3DBaseFont)))
		Name="����";
		
	if(m_pBindObject->IsKindOf(GET_CLASS_INFO(CD3DFontCN)))
		Name="��������";
		
	if(m_pBindObject->IsKindOf(GET_CLASS_INFO(CD3DDX9Font)))
		Name="DX9����";
		
	if(m_pBindObject->IsKindOf(GET_CLASS_INFO(CD3DSubMesh)))
		Name="SubMesh";
		
	
	return Name;
}

void CUSOObject::ReBindChilds()
{
	ClearAllChild();
	//for(int i=(int)GetChildCount()-1;i>=0;i--)
	//{
	//	if(m_ChildList[i])
	//	{
	//		m_ChildList[i]->Release();
	//	}
	//}
	//m_ChildList.clear();
	BindChilds();
}

void CUSOObject::BindChilds()
{
	CEasyMap<LPVOID,LPVOID> PtrMap;

	//�������Ӷ���󶨵Ķ���ָ����룬��ֹ�ظ�
	for(UINT i=0;i<GetChildCount();i++)
	{
		PtrMap[GetChildByIndex(i)->GetBindObject()]=GetChildByIndex(i)->GetBindObject();
	}	

	if((m_pBindObject->IsKindOf(GET_CLASS_INFO(CD3DObject))))
	{
		CD3DObject * pD3DObject=(CD3DObject *)m_pBindObject;
		
		pD3DObject->ShowBoundingFrame(DBFO_RELEASE);

		CUSOObject * pParent=this;
		//����Դ
				
		if(pD3DObject->IsKindOf(GET_CLASS_INFO(CD3DWOWM2Model)))
		{
			CD3DWOWM2ModelResource * pResource=((CD3DWOWM2Model *)pD3DObject)->GetModelResource();

			if(pResource)
			{
				pResource->AddUseRef();
				CUSOObject * pObject=new CUSOObject();			
				pObject->SetBindObject(pResource,true);
				pObject->SetParent(pParent);
				pObject->SetTreeCtrl(m_pTreeCtrl);
				//pParent=pObject;
			}			
		}
		else if(pD3DObject->IsKindOf(GET_CLASS_INFO(CD3DWOWWMOModel)))
		{
			CD3DWOWWMOModelResource * pResource=((CD3DWOWWMOModel *)pD3DObject)->GetModelResource();

			if(pResource)
			{
				pResource->AddUseRef();
				CUSOObject * pObject=new CUSOObject();			
				pObject->SetBindObject(pResource,true);
				pObject->SetParent(pParent);
				pObject->SetTreeCtrl(m_pTreeCtrl);
				//pParent=pObject;
			}			
		}
		else if(pD3DObject->IsKindOf(GET_CLASS_INFO(CD3DWOWADTModel)))
		{
			CD3DWOWADTModelResource * pResource=((CD3DWOWADTModel *)pD3DObject)->GetModelResource();

			if(pResource)
			{
				pResource->AddUseRef();
				CUSOObject * pObject=new CUSOObject();			
				pObject->SetBindObject(pResource,true);
				pObject->SetParent(pParent);
				pObject->SetTreeCtrl(m_pTreeCtrl);
				//pParent=pObject;
			}			
		}
		
				
		for(int i=0;i<pD3DObject->GetSubMeshCount();i++)
		{
			CD3DSubMesh * pSubMesh=pD3DObject->GetSubMesh(i);
			if(pSubMesh)
			{
				CUSOObject * pParent=new CUSOObject();					
							
				pSubMesh->AddUseRef();
				pParent->SetBindObject(pSubMesh,false);

				pParent->SetParent(this);
				pParent->SetTreeCtrl(m_pTreeCtrl);		


				CD3DFX * pFX=pD3DObject->GetSubMeshMaterial(i)->GetFX();
				if(pFX)
				{					
					pFX->AddUseRef();
					CUSOObject * pObject=new CUSOObject();
					pObject->SetBindObject(pFX,false);
					pObject->SetParent(pParent);
					pObject->SetTreeCtrl(m_pTreeCtrl);

				}
				for(int t=0;t<pD3DObject->GetSubMeshMaterial(i)->GetTextureLayerCount();t++)
				{
					CD3DTexture * pTexture=pD3DObject->GetSubMeshMaterial(i)->GetTexture(t);
					if(pTexture)
					{

						pTexture->AddUseRef();
						CUSOObject * pObject=new CUSOObject();
						pObject->SetBindObject(pTexture,false);
						pObject->SetParent(pParent);
						pObject->SetTreeCtrl(m_pTreeCtrl);

					}
				}
			}
		}		

		//���Ӷ���


		for(UINT i=0;i<pD3DObject->GetChildCount();i++)
		{
			if(PtrMap.Find(pD3DObject->GetChildByIndex(i))==NULL)
			{
				PtrMap[pD3DObject->GetChildByIndex(i)]=pD3DObject->GetChildByIndex(i);
				CUSOObject * pObject=new CUSOObject();
				pD3DObject->GetChildByIndex(i)->AddUseRef();
				pObject->SetBindObject(pD3DObject->GetChildByIndex(i),true);
				pObject->SetParent(this);
				pObject->SetTreeCtrl(m_pTreeCtrl);

			}
		}
	}

	if((m_pBindObject->IsKindOf(GET_CLASS_INFO(CD3DObjectResource))))
	{
		CD3DObjectResource * pResource=(CD3DObjectResource *)m_pBindObject;
		for(int i=0;i<pResource->GetSubMeshCount();i++)
		{
			CD3DSubMesh * pSubMesh=pResource->GetSubMesh(i);			
			if(pSubMesh)
			{
				CUSOObject * pParent=new CUSOObject();				
				pSubMesh->AddUseRef();
				pParent->SetBindObject(pSubMesh,true);
				
				pParent->SetParent(this);
				pParent->SetTreeCtrl(m_pTreeCtrl);				
			}
		}
	}
	if((m_pBindObject->IsKindOf(GET_CLASS_INFO(CD3DSubMesh))))
	{
		CD3DSubMesh * pSubMesh=(CD3DSubMesh *)m_pBindObject;
		CD3DFX * pFX=pSubMesh->GetMaterial().GetFX();
		if(pFX)
		{					
			pFX->AddUseRef();
			CUSOObject * pObject=new CUSOObject();
			pObject->SetBindObject(pFX,false);
			pObject->SetParent(this);
			pObject->SetTreeCtrl(m_pTreeCtrl);

		}
		for(int t=0;t<pSubMesh->GetMaterial().GetTextureLayerCount();t++)
		{
			CD3DTexture * pTexture=pSubMesh->GetMaterial().GetTexture(t);
			if(pTexture)
			{

				pTexture->AddUseRef();
				CUSOObject * pObject=new CUSOObject();
				pObject->SetBindObject(pTexture,false);
				pObject->SetParent(this);
				pObject->SetTreeCtrl(m_pTreeCtrl);

			}
		}
	}
}