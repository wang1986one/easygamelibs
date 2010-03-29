/****************************************************************************/
/*                                                                          */
/*      �ļ���:    DialogItemSorter.h                                       */
/*      ��������:  2010��02��09��                                           */
/*      ����:      Sagasarate                                               */
/*                                                                          */
/*      �������Ȩ��Sagasarate(sagasarate@sina.com)����                     */
/*      ����Խ�����������κ���ҵ�ͷ���ҵ�����������                      */
/*      ���뱣���˰�Ȩ����                                                  */
/*                                                                          */
/****************************************************************************/
#pragma once

class CDialogItemSorter
{
public:
	enum DIALOG_ITEM_SORT_TYPE
	{
		SORT_MASK=0x01,
		SORT_HORIZONTAL=0x00,				//ˮƽ����
		SORT_VERTICAL=0x01,					//��ֱ����
		RESIZE_MASK=0x06,
		RESIZE_FIT_PARENT_WIDTH=0x02,
		RESIZE_FIT_PARENT_HEIGHT=0x04,
		ALIGN_MASK=0x18,
		ALIGN_LEFT=0x08,
		ALIGN_TOP=0x08,
		ALIGN_RIGHT=0x10,
		ALIGN_BOTTOM=0x10,
		ALIGN_CENTER=0x00,
		PARAM_FIT_RATE=0x20,
		PARAM_ALIGN_SPACE=0x40,
		PARAM_ALIGN_PRE_SPACE=0x80,
	};
protected:
	struct SORT_TREE_ITEM
	{
		UINT						ID;
		int							SortType;		
		float						SortParam;
		SORT_TREE_ITEM *			pParent;
		vector<SORT_TREE_ITEM *>	ChildList;
		CRect						Rect; 
		int							DialogItemID;

		SORT_TREE_ITEM()
		{
			ID=0;
			SortType=0;
			SortParam=0;
			pParent=NULL;
			DialogItemID=0;
		}
		~SORT_TREE_ITEM()
		{
			for(size_t i=0;i<ChildList.size();i++)
			{
				SAFE_DELETE(ChildList[i]);
			}
			ChildList.clear();
		}
	};
	CWnd *				m_pParent;
	SORT_TREE_ITEM *	m_pItemTree;
public:
	CDialogItemSorter(void);
	~CDialogItemSorter(void);

	void SetDialog(CWnd * pParent)
	{
		m_pParent=pParent;
	}
	void SetSortType(UINT ID,int SortType,float SortParam);

	bool AddDialogItem(UINT ParentID,UINT ID,int SortType,float SortParam,int DialogItemID,CRect Rect);

	void DoSort();
protected:
	CDialogItemSorter::SORT_TREE_ITEM * FindTreeNode(SORT_TREE_ITEM * pRoot,UINT ID);
	void ResizeItems(SORT_TREE_ITEM * pRoot);
	void AlignItems(SORT_TREE_ITEM * pRoot);
	void GetDialogItemSize(SORT_TREE_ITEM * pRoot);
	void SetDialogItemSize(SORT_TREE_ITEM * pRoot);
};
