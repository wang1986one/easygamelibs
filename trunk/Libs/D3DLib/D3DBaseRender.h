/****************************************************************************/
/*                                                                          */
/*      �ļ���:    D3DBaseRender.h                                          */
/*      ��������:  2011��01��14��				                            */
/*      ����:      Sagasarate                                               */
/*                                                                          */
/*      �������Ȩ��Sagasarate(sagasarate@sina.com)����                     */
/*      ����Խ�����������κ���ҵ�ͷ���ҵ�����������                      */
/*      ���뱣���˰�Ȩ����                                                  */
/*                                                                          */
/****************************************************************************/
#pragma once

namespace D3DLib{

class CD3DCamera;
class CD3DLight;
class CD3DObject;

#define D3DLOCK_FOR_OBJECT_ADD \
	CAutoLockEx RenderLock;\
	if(CD3DDevice::IsUseMultiThreadRender())\
	{\
		if(GetRender())\
		{\
			RenderLock.Lock(GetRender()->GetRenderLock());\
		}\
	}

#define D3DLOCK_FOR_OBJECT_MODIFY \
	CAutoLockEx ObjectLock;\
	if(CD3DDevice::IsUseMultiThreadRender())\
	{\
		ObjectLock.Lock(GetRenderLock());\
	}


class CD3DBaseRender :
	public CNameObject
{
protected:
	CD3DDevice *					m_pDevice;
	CEasyCriticalSection			m_RenderLock;

	CEasyArray<CD3DObject *>		m_RootObjectList;
	int								m_PrepareRenderDataFailCount;

	int								m_ObjectCount;
	int								m_SubMeshCount;
	int								m_FaceCount;
	int								m_VertexCount;

	DECLARE_CLASS_INFO(CD3DBaseRender)
public:
	CD3DBaseRender(void);
	~CD3DBaseRender(void);

	virtual bool Reset();
	virtual bool Restore();	
	
	virtual void Render();
	virtual void Update(FLOAT Time);
	

	virtual bool AddObject(CD3DObject * pObj,bool IsRecursive=true);
	virtual bool AddRootObject(CD3DObject * pObj);
	virtual bool DelObject(CD3DObject * pObj);
	virtual bool DelRootObject(CD3DObject * pObj);


	int GetRenderObjectCount();
	int	GetRenderSubMeshCount();
	int GetRenderFaceCount();
	int GetRenderVertexCount();


	CD3DDevice * GetDevice();
	CEasyCriticalSection& GetRenderLock();

protected:
	void RenderSubMesh(CD3DSubMesh * pSubMesh,CD3DFX * pRenderFX,LPCTSTR RenderTech);
	void PrepareRenderData(CD3DObject * pObject);
};

inline int CD3DBaseRender::GetRenderObjectCount()
{
	return m_ObjectCount;
}
inline int CD3DBaseRender::GetRenderSubMeshCount()
{
	return m_SubMeshCount;
}
inline int CD3DBaseRender::GetRenderFaceCount()
{
	return m_FaceCount;
}
inline int CD3DBaseRender::GetRenderVertexCount()
{
	return m_VertexCount;
}

inline CD3DDevice * CD3DBaseRender::GetDevice()
{
	return m_pDevice;
}

inline CEasyCriticalSection& CD3DBaseRender::GetRenderLock()
{
	return m_RenderLock;
}


}