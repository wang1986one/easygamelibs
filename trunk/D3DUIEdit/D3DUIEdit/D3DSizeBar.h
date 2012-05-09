/****************************************************************************/
/*                                                                          */
/*      �ļ���:    D3DSizeBar.h                                             */
/*      ��������:  2011��10��07��                                           */
/*      ����:      Sagasarate                                               */
/*                                                                          */
/*      �������Ȩ��Sagasarate(sagasarate@sina.com)����                     */
/*      ����Խ�����������κ���ҵ�ͷ���ҵ�����������                      */
/*      ���뱣���˰�Ȩ����                                                  */
/*                                                                          */
/****************************************************************************/
#pragma once


#define D3DGUIEDIT_OBJECT_SEC	0x02000000

enum D3DGUI_OBJECT_TYPES
{
	D3DGUIEDIT_OBJECT_NONE=0,
	D3DGUIEDIT_OBJECT_SIZEBAR=D3DGUIEDIT_OBJECT_SEC|0x01,
};

class CObjectPropertyView;
class CObjectTreeView;
class CSizeBarManager;

class CD3DSizeBar :
	public CD3DWnd
{
protected:	
	CObjectPropertyView *	m_pObjectPropertyView;
	CObjectTreeView *		m_pObjectTreeView;
	CSizeBarManager *		m_pManager;

	bool					m_IsControlPress;
	bool					m_IsShiftPress;

	DECLARE_CLASS_INFO(CD3DSizeBar)
public:
	CD3DSizeBar();
	CD3DSizeBar(CD3DGUI *  pGUI);
	virtual ~CD3DSizeBar(void);

	void SetManager(CSizeBarManager * pManager);

	virtual BOOL OnMessage(CD3DWnd * pWnd,UINT msg, WPARAM wParam, LPARAM lParam);
	virtual BOOL HandleMessage(CD3DWnd * pWnd,UINT msg, WPARAM wParam, LPARAM lParam);

	CD3DWnd * GetChild();

	virtual void GetMiniSize(int& Width,int& Height);

	virtual void UpdateRects();

	void SetTreeItem(CObjectTreeView * pTreeView,HTREEITEM item)
	{
		m_pObjectTreeView=pTreeView;
		SetWndData(item);
	}
	void SetObjectPropertyView(CObjectPropertyView * pObjectPropertyView)
	{
		m_pObjectPropertyView=pObjectPropertyView;
	}

	HTREEITEM GetTreeItem()
	{
		return (HTREEITEM)GetWndData();
	}

	void UpdateSizeToChild();

	virtual void SaveToXml(xml_node * pXMLNode);
	virtual bool ToSmartStruct(CSmartStruct& Packet,CUSOResourceManager * pResourceManager,UINT Param=0);
	virtual UINT GetSmartStructSize(UINT Param=0);
	virtual void PickResource(CUSOResourceManager * pResourceManager,UINT Param=0);

	void ClearSelect();



	virtual void SetParent(CD3DWnd * parent,bool IsUpdateParent=true);

	void UpdateTreeItem();

	virtual void SetVisible(bool IsVisible);

};
