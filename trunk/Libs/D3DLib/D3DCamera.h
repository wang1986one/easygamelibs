/****************************************************************************/
/*                                                                          */
/*      �ļ���:    D3DCamera.h                                              */
/*      ��������:  2009��09��25��                                           */
/*      ����:      Sagasarate                                               */
/*                                                                          */
/*      ��������Ȩ��Sagasarate(sagasarate@sina.com)����                     */
/*      ����Խ������������κ���ҵ�ͷ���ҵ������������                      */
/*      ���뱣���˰�Ȩ����                                                  */
/*                                                                          */
/****************************************************************************/
#pragma once

namespace D3DLib{

enum D3DCAMERA_APPLY_MODE
{
	D3DCAMERA_APPLY_VIEW=1,
	D3DCAMERA_APPLY_PROJECT=2,
	D3DCAMERA_APPLY_ALL=3,
};

class CD3DCamera :
	public CD3DObject
{
protected:	
	CD3DMatrix		m_ViewMatrix;
	CD3DMatrix		m_ProjectMatrix;
	
	//struct STORAGE_STRUCT:public CD3DObject::STORAGE_STRUCT
	//{
	//	CD3DMatrix ProjMatrix;
	//};

	DECLARE_CLASS_INFO(CD3DCamera)

public:
	CD3DCamera();
	virtual ~CD3DCamera(void);

	virtual bool CanRender();

	void SetViewMat(const CD3DMatrix& Mat);
	void SetProjectMat(const CD3DMatrix& Mat);

	CD3DMatrix& GetViewMat();
	CD3DMatrix& GetProjectMat();	

	void LookAt(const CD3DVector3& Eye,const CD3DVector3& At,const CD3DVector3& Up);

	void SetProjectPerspective(FLOAT Fov,FLOAT Aspect,FLOAT Near,FLOAT Far);
	void SetProjectOrtho(FLOAT Width,FLOAT Height,FLOAT Near,FLOAT Far);

	
	void SetPos(const CD3DVector3& Pos);
	void SetRotation(FLOAT Yaw,FLOAT Pitch,FLOAT Roll);

	void Apply(CD3DDevice* pDevice,int Mode);

	void GetPickRay(FLOAT ScreenX,FLOAT ScreenY,CD3DVector3& Point,CD3DVector3& Dir);
	void GetPickRay(int ScreenX,int ScreenY,int ScreenWidth,int ScreenHeight,CD3DVector3& Point,CD3DVector3& Dir);

	virtual void Update(FLOAT Time);

protected:
	//virtual CNameObject::STORAGE_STRUCT * USOCreateHead(UINT Param=0);
	//virtual int USOWriteHead(CNameObject::STORAGE_STRUCT * pHead,CUSOFile * pUSOFile,UINT Param=0);
	//virtual int USOReadHead(CNameObject::STORAGE_STRUCT * pHead,CUSOFile * pUSOFile,UINT Param=0);
	
};

inline bool CD3DCamera::CanRender()
{
	return false;
}

inline void CD3DCamera::SetViewMat(const CD3DMatrix& Mat)
{
	SetLocalMatrix(Mat.GetInverse());
}
inline void CD3DCamera::SetProjectMat(const CD3DMatrix& Mat)
{
	m_ProjectMatrix=Mat;
}

inline CD3DMatrix& CD3DCamera::GetViewMat()
{
	return m_ViewMatrix;
}
inline CD3DMatrix& CD3DCamera::GetProjectMat()
{
	return m_ProjectMatrix;
}

inline void CD3DCamera::LookAt(const CD3DVector3& Eye,const CD3DVector3& At,const CD3DVector3& Up)
{
	//�Ӿ�����һ����׼�Ӿ����һ���任�������

	m_ViewMatrix=CD3DMatrix::FromLookAtLH(Eye,At,Up);
	m_LocalMatrix=m_ViewMatrix.GetInverse();
}

inline void CD3DCamera::SetProjectPerspective(FLOAT Fov,FLOAT Aspect,FLOAT Near,FLOAT Far)
{
	m_ProjectMatrix=CD3DMatrix::FromPerspectiveFovLH(Fov,Aspect,Near,Far);
}
inline void CD3DCamera::SetProjectOrtho(FLOAT Width,FLOAT Height,FLOAT Near,FLOAT Far)
{
	m_ProjectMatrix=CD3DMatrix::FromOrthoLH(Width,Height,Near,Far);
}


inline void CD3DCamera::SetPos(const CD3DVector3& Pos)
{
	m_LocalMatrix.SetTranslation(Pos);
}
inline void CD3DCamera::SetRotation(FLOAT Yaw,FLOAT Pitch,FLOAT Roll)
{
	m_LocalMatrix.SetRotation(CD3DMatrix::FromRotationYawPitchRoll(Yaw,Pitch,Roll));
}

inline void CD3DCamera::Apply(CD3DDevice* pDevice,int Mode)
{
	if(pDevice)
	{			
		m_ViewMatrix=GetWorldMatrix().GetInverse();

		if(Mode&D3DCAMERA_APPLY_VIEW)
			pDevice->GetD3DDevice()->SetTransform(D3DTS_VIEW,&m_ViewMatrix);
		if(Mode&D3DCAMERA_APPLY_PROJECT)
			pDevice->GetD3DDevice()->SetTransform(D3DTS_PROJECTION,&m_ProjectMatrix);
	}

}

}