/****************************************************************************/
/*                                                                          */
/*      �ļ���:    USOObject.h                                              */
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
/*      �ļ���:    USOObject.h                                              */
/*      ��������:  2009��07��06��                                           */
/*      ����:      Sagasarate                                               */
/*                                                                          */
/*      �������Ȩ��Sagasarate(sagasarate@sina.com)����                     */
/*      ����Խ�����������κ���ҵ�ͷ���ҵ�����������                      */
/*      ���뱣���˰�Ȩ����                                                  */
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
