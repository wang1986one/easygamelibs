/****************************************************************************/
/*                                                                          */
/*      �ļ���:    D3DPlane.h                                               */
/*      ��������:  2009��07��06��                                           */
/*      ����:      Sagasarate                                               */
/*                                                                          */
/*      �������Ȩ��Sagasarate(sagasarate@sina.com)����                     */
/*      ����Խ�����������κ���ҵ�ͷ���ҵ�����������                      */
/*      ���뱣���˰�Ȩ����                                                  */
/*                                                                          */
/****************************************************************************/
#pragma once
namespace D3DLib{

class CD3DPlane :
	public D3DXPLANE
{
public:
	CD3DPlane():D3DXPLANE(){}
	CD3DPlane(const FLOAT* pData):D3DXPLANE(pData){}
	CD3DPlane(const D3DXFLOAT16* pData):D3DXPLANE(pData){}
	CD3DPlane(FLOAT a, FLOAT b, FLOAT c, FLOAT d):D3DXPLANE(a,b,c,d){}

	~CD3DPlane(){};

	//���
	void Normalize();
	CD3DPlane GetNormalize();

	//���
	FLOAT Dot(const D3DXVECTOR4& Vector);
	FLOAT DotCoord(const D3DXVECTOR3& Vector);
	FLOAT DotNormal(const D3DXVECTOR3& Vector);

	//����
	void Scale(FLOAT Scale);
	CD3DPlane GetScale(FLOAT Scale);

	//�󽻵�
	CD3DVector3 IntersectLine(const D3DXVECTOR3& Vector1,const D3DXVECTOR3& Vector2);

	static CD3DPlane FromPoints(const D3DXVECTOR3& Vector1,const D3DXVECTOR3& Vector2,const D3DXVECTOR3& Vector3);
	static CD3DPlane FromPointNormal(const D3DXVECTOR3& Vector,const D3DXVECTOR3& Normal);
};



inline void CD3DPlane::Normalize()
{
	D3DXPlaneNormalize(this,this);
}

inline CD3DPlane CD3DPlane::GetNormalize()
{
	CD3DPlane Plane;
	D3DXPlaneNormalize(&Plane,this);
	return Plane;
}


inline FLOAT CD3DPlane::Dot(const D3DXVECTOR4& Vector)
{
	return D3DXPlaneDot(this,&Vector);	
}
inline FLOAT CD3DPlane::DotCoord(const D3DXVECTOR3& Vector)
{
	return D3DXPlaneDotCoord(this,&Vector);
}
inline FLOAT CD3DPlane::DotNormal(const D3DXVECTOR3& Vector)
{
	return D3DXPlaneDotNormal(this,&Vector);
}


inline void CD3DPlane::Scale(FLOAT Scale)
{
	D3DXPlaneScale(this,this,Scale);
}
inline CD3DPlane CD3DPlane::GetScale(FLOAT Scale)
{
	CD3DPlane Plane;
	D3DXPlaneScale(this,this,Scale);
	return Plane;
}


inline CD3DVector3 CD3DPlane::IntersectLine(const D3DXVECTOR3& Vector1,const D3DXVECTOR3& Vector2)
{
	CD3DVector3 Vector;
	D3DXPlaneIntersectLine(&Vector,this,&Vector1,&Vector2);
	return Vector;
}

inline CD3DPlane CD3DPlane::FromPoints(const D3DXVECTOR3& Vector1,const D3DXVECTOR3& Vector2,const D3DXVECTOR3& Vector3)
{
	CD3DPlane Plane;
	D3DXPlaneFromPoints(&Plane,&Vector1,&Vector2,&Vector3);
	return Plane;
}
inline CD3DPlane CD3DPlane::FromPointNormal(const D3DXVECTOR3& Vector,const D3DXVECTOR3& Normal)
{
	CD3DPlane Plane;
	D3DXPlaneFromPointNormal(&Plane,&Vector,&Normal);
	return Plane;
}

}