/****************************************************************************/
/*                                                                          */
/*      �ļ���:    D3DUIRender.h                                            */
/*      ��������:  2010��02��09��                                           */
/*      ����:      Sagasarate                                               */
/*                                                                          */
/*      �������Ȩ��Sagasarate(sagasarate@sina.com)����                     */
/*      ����Խ�����������κ���ҵ�ͷ���ҵ�����������                      */
/*      ���뱣���˰�Ȩ����                                                  */
/*                                                                          */
/****************************************************************************/
#pragma once

namespace D3DLib{




class CD3DUIRender :
	public CD3DBaseRender
{
protected:

	CEasyArray<CD3DObject *>		m_ObjectList;	


	DECLARE_CLASS_INFO(CD3DUIRender)
public:
	CD3DUIRender();
	virtual ~CD3DUIRender(void);

	bool Create(CD3DDevice * pDevice);
	virtual void Destory();
	virtual bool Reset();
	virtual bool Restore();	
		
	virtual void Render();
	virtual void RenderDirectly(CD3DObject * pObject);	

	virtual bool AddObject(CD3DObject * pObj,bool IsRecursive=true);
	virtual bool DelObject(CD3DObject * pObj,bool IsRecursive=true);

	virtual void RemoveAllObject();

	bool MoveToTop(CD3DObject * pObj,CD3DObject *pBefore);
	bool MoveToTop(CD3DObject ** ppObj,UINT ObjectCount,CD3DObject *pBefore);

protected:
	
	
};



}