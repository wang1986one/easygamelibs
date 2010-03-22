#include "stdafx.h"


CDialogItemSorter::CDialogItemSorter(void)
{
	m_pParent=NULL;
	m_pItemTree=new SORT_TREE_ITEM;
}

CDialogItemSorter::~CDialogItemSorter(void)
{
	SAFE_DELETE(m_pItemTree);
}

void CDialogItemSorter::SetSortType(UINT ID,int SortType,float SortParam)
{
	SORT_TREE_ITEM * pNode=FindTreeNode(m_pItemTree,ID);
	if(pNode)
	{
		pNode->SortType=SortType;
		pNode->SortParam=SortParam;
	}
}

bool CDialogItemSorter::AddDialogItem(UINT ParentID,UINT ID,int SortType,float SortParam,int DialogItemID,CRect Rect)
{
	SORT_TREE_ITEM * pParent=FindTreeNode(m_pItemTree,ParentID);
	if(pParent)
	{
		SORT_TREE_ITEM * pNewNode=new SORT_TREE_ITEM;
		pNewNode->ID=ID;
		pNewNode->SortType=SortType;
		pNewNode->SortParam=SortParam;
		pNewNode->DialogItemID=DialogItemID;
		pNewNode->Rect=Rect;

		pNewNode->pParent=pParent;
		pParent->ChildList.push_back(pNewNode);
		return true;
	}
	return false;
}

void CDialogItemSorter::DoSort()
{
	if(m_pParent&&::IsWindow(m_pParent->GetSafeHwnd()))
	{
		m_pParent->GetClientRect(&(m_pItemTree->Rect));
		GetDialogItemSize(m_pItemTree);
		ResizeItems(m_pItemTree);
		AlignItems(m_pItemTree);
		SetDialogItemSize(m_pItemTree);
		
	}
}

CDialogItemSorter::SORT_TREE_ITEM * CDialogItemSorter::FindTreeNode(SORT_TREE_ITEM * pRoot,UINT ID)
{
	if(pRoot->ID==ID)
		return pRoot;
	for(size_t i=0;i<pRoot->ChildList.size();i++)
	{
		SORT_TREE_ITEM * pTree=FindTreeNode(pRoot->ChildList[i],ID);
		if(pTree)
			return pTree;
	}
	return NULL;
}

void CDialogItemSorter::ResizeItems(SORT_TREE_ITEM * pRoot)
{
	CRect Rect=pRoot->Rect;
	if(pRoot->SortType&SORT_VERTICAL)
	{
		int FitHeight=Rect.Height();
		int FitItemCount=0;
		for(size_t i=0;i<pRoot->ChildList.size();i++)
		{
			SORT_TREE_ITEM * pChild=pRoot->ChildList[i];			

			if(!(pChild->SortType&RESIZE_FIT_PARENT_HEIGHT))
			{
				FitHeight-=pChild->Rect.Height();				
			}
			else
			{
				FitItemCount++;
			}
			if(pChild->SortType&RESIZE_FIT_PARENT_WIDTH)
			{
				pChild->Rect.left=Rect.left;
				pChild->Rect.right=Rect.right;
			}
		}
		if(FitItemCount)
		{		
			if(pRoot->SortType&PARAM_ALIGN_SPACE)
				FitHeight-=(pRoot->ChildList.size()+1)*pRoot->SortParam;		

			int Height=FitHeight/FitItemCount;
			for(size_t i=0;i<pRoot->ChildList.size();i++)
			{
				SORT_TREE_ITEM * pChild=pRoot->ChildList[i];
				if(pChild->SortType&RESIZE_FIT_PARENT_HEIGHT)
				{
					if(pChild->SortType&PARAM_FIT_RATE)
						pChild->Rect.bottom=pChild->Rect.top+FitHeight*pChild->SortParam;
					else
						pChild->Rect.bottom=pChild->Rect.top+Height;
				}
			}
		}
	}
	else
	{
		int FitWidth=Rect.Width();
		int FitItemCount=0;
		for(size_t i=0;i<pRoot->ChildList.size();i++)
		{
			SORT_TREE_ITEM * pChild=pRoot->ChildList[i];
			if(!(pChild->SortType&RESIZE_FIT_PARENT_WIDTH))
			{
				FitWidth-=pChild->Rect.Width();				
			}
			else
			{
				FitItemCount++;
			}
			if(pChild->SortType&RESIZE_FIT_PARENT_HEIGHT)
			{
				pChild->Rect.top=Rect.top;
				pChild->Rect.bottom=Rect.bottom;
			}
		}
		if(FitItemCount)
		{		
			if(pRoot->SortType&PARAM_ALIGN_SPACE)
				FitWidth-=(pRoot->ChildList.size()+1)*pRoot->SortParam;		

			int Width=FitWidth/FitItemCount;
			for(size_t i=0;i<pRoot->ChildList.size();i++)
			{
				SORT_TREE_ITEM * pChild=pRoot->ChildList[i];
				if(pChild->SortType&RESIZE_FIT_PARENT_WIDTH)
				{
					if(pChild->SortType&PARAM_FIT_RATE)
						pChild->Rect.right=pChild->Rect.left+FitWidth*pChild->SortParam;
					else
						pChild->Rect.right=pChild->Rect.left+Width;
				}
			}
		}
	}
	for(size_t i=0;i<pRoot->ChildList.size();i++)
	{
		ResizeItems(pRoot->ChildList[i]);
	}
}

void CDialogItemSorter::AlignItems(SORT_TREE_ITEM * pRoot)
{
	CRect Rect=pRoot->Rect;
	if(pRoot->SortType&SORT_VERTICAL)
	{
		int Height=0;
		int ItemCount=pRoot->ChildList.size();
		for(size_t i=0;i<pRoot->ChildList.size();i++)
		{
			SORT_TREE_ITEM * pChild=pRoot->ChildList[i];

			Height+=pChild->Rect.Height();
		}
		if(ItemCount)
		{		
			int AlignType=pRoot->SortType&ALIGN_MASK;
			int Pre=0;
			int Space=0;
			if(AlignType==ALIGN_CENTER)
			{
				if(pRoot->SortType&PARAM_ALIGN_SPACE)
				{
					Pre=(Rect.Height()-Height-pRoot->SortParam*(ItemCount-1))/2;			
					Space=pRoot->SortParam;
				}
				else
				{
					Pre=(Rect.Height()-Height)/(ItemCount+1);			
					Space=Pre;
				}				
			}
			else if(AlignType==ALIGN_TOP)
			{
				if(pRoot->SortType&PARAM_ALIGN_SPACE)
				{
					Pre=pRoot->SortParam;	
					Space=pRoot->SortParam;
				}
				else
				{
					Pre=0;
					Space=0;
				}
			}
			else if(AlignType==ALIGN_BOTTOM)
			{
				if(pRoot->SortType&PARAM_ALIGN_SPACE)
				{
					Pre=pRoot->SortParam;	
					Space=pRoot->SortParam;
				}
				else
				{
					Pre=Rect.Height()-Height-pRoot->SortParam*(ItemCount-1);
					Space=0;
				}
			}

			int Top=Rect.top+Pre;
			for(size_t i=0;i<pRoot->ChildList.size();i++)
			{
				SORT_TREE_ITEM * pChild=pRoot->ChildList[i];

				pChild->Rect.bottom=Top+pChild->Rect.Height();
				pChild->Rect.top=Top;
				Top+=pChild->Rect.Height()+Space;

				int Left=Rect.left+(Rect.Width()-pChild->Rect.Width())/2;
				pChild->Rect.right=Left+pChild->Rect.Width();
				pChild->Rect.left=Left;
			}
		}
	}
	else
	{
		int Width=0;
		int ItemCount=pRoot->ChildList.size();
		for(size_t i=0;i<pRoot->ChildList.size();i++)
		{
			SORT_TREE_ITEM * pChild=pRoot->ChildList[i];

			Width+=pChild->Rect.Width();
		}
		if(ItemCount)
		{		
			int AlignType=pRoot->SortType&ALIGN_MASK;
			int Pre=0;
			int Space=0;
			if(AlignType==ALIGN_CENTER)
			{
				if(pRoot->SortType&PARAM_ALIGN_SPACE)
				{
					Pre=(Rect.Width()-Width-pRoot->SortParam*(ItemCount-1))/2;			
					Space=pRoot->SortParam;
				}
				else
				{
					Pre=(Rect.Width()-Width)/(ItemCount+1);			
					Space=Pre;
				}
				
			}
			else if(AlignType==ALIGN_LEFT)
			{
				if(pRoot->SortType&PARAM_ALIGN_SPACE)
				{
					Pre=pRoot->SortParam;	
					Space=pRoot->SortParam;
				}
				else
				{
					Pre=0;
					Space=0;
				}
				
			}
			else if(AlignType==ALIGN_RIGHT)
			{
				if(pRoot->SortType&PARAM_ALIGN_SPACE)
				{
					Pre=pRoot->SortParam;	
					Space=pRoot->SortParam;
				}
				else
				{
					Pre=Rect.Width()-Width-pRoot->SortParam*(ItemCount-1);
					Space=0;
				}
				
			}

			int Left=Rect.left+Pre;
			for(size_t i=0;i<pRoot->ChildList.size();i++)
			{
				SORT_TREE_ITEM * pChild=pRoot->ChildList[i];

				pChild->Rect.right=Left+pChild->Rect.Width();
				pChild->Rect.left=Left;
				Left+=pChild->Rect.Width()+Space;

				int Top=Rect.top+(Rect.Height()-pChild->Rect.Height())/2;
				pChild->Rect.bottom=Top+pChild->Rect.Height();
				pChild->Rect.top=Top;
			}
		}
	}

	for(size_t i=0;i<pRoot->ChildList.size();i++)
	{
		AlignItems(pRoot->ChildList[i]);
	}
}

void CDialogItemSorter::GetDialogItemSize(SORT_TREE_ITEM * pRoot)
{
	CWnd * pDialogItem=m_pParent->GetDlgItem(pRoot->DialogItemID);
	if(pDialogItem)
		pDialogItem->GetWindowRect(&(pRoot->Rect));

	for(size_t i=0;i<pRoot->ChildList.size();i++)
	{
		GetDialogItemSize(pRoot->ChildList[i]);
	}
}
void CDialogItemSorter::SetDialogItemSize(SORT_TREE_ITEM * pRoot)
{
	CWnd * pDialogItem=m_pParent->GetDlgItem(pRoot->DialogItemID);
	if(pDialogItem)
		pDialogItem->MoveWindow(&(pRoot->Rect));

	for(size_t i=0;i<pRoot->ChildList.size();i++)
	{
		SetDialogItemSize(pRoot->ChildList[i]);
	}
}