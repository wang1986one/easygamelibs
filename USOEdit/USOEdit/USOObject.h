/****************************************************************************/
/*                                                                          */
/*      文件名:    USOObject.h                                              */
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
/*      文件名:    USOObject.h                                              */
/*      创建日期:  2009年07月06日                                           */
/*      作者:      Sagasarate                                               */
/*                                                                          */
/*      本软件版权归Sagasarate(sagasarate@sina.com)所有                     */
/*      你可以将本软件用于任何商业和非商业软件开发，但                      */
/*      必须保留此版权声明                                                  */
/*                                                                          */
/****************************************************************************/
#pragma once
#include <vector>



class CUSOObject :
	public CTreeObject
{
protected:	
	CNameObject *				m_pBindObject;	
	CTreeCtrl *					m_pTreeCtrl;
	HTREEITEM					m_hTreeItem;

	DECLARE_CLASS_INFO(CUSOObject)
public:
	CUSOObject(void);
	virtual ~CUSOObject(void);

	void SetBindObject(CNameObject * pBindObject,bool CreateChild=true);
	CNameObject * GetBindObject();

	

	virtual void SetParent(CUSOObject * pParent);
	CUSOObject * GetParent();	
	CUSOObject * GetChildByIndex(int Index);

	void SetTreeCtrl(CTreeCtrl * pTreeCtrl);

	HTREEITEM GetTreeItem();
	CTreeCtrl * GetTreeCtrl();
	
	void ReBindChilds();
	void RebulidTreeItem();
	void UpdateTreeItem(bool IsRecursive=false);
	CString BulidObjectName();
	CString GetTypeName();

protected:
	void BindChilds();	
};

inline CUSOObject * CUSOObject::GetParent()
{
	return (CUSOObject *)CTreeObject::GetParent();
}

inline CTreeCtrl * CUSOObject::GetTreeCtrl()
{
	return m_pTreeCtrl;
}



inline CUSOObject * CUSOObject::GetChildByIndex(int Index)
{
	return (CUSOObject *)CTreeObject::GetChildByIndex(Index);
}
