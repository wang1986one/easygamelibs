/****************************************************************************/
/*                                                                          */
/*      �ļ���:    D3DFrustum.h                                             */
/*      ��������:  2009��09��11��                                           */
/*      ����:      Sagasarate                                               */
/*                                                                          */
/*      �������Ȩ��Sagasarate(sagasarate@sina.com)����                     */
/*      ����Խ�����������κ���ҵ�ͷ���ҵ�����������                      */
/*      ���뱣���˰�Ȩ����                                                  */
/*                                                                          */
/****************************************************************************/
#pragma once

namespace D3DLib{

class CD3DFrustum
{
public:
	enum LOCATION_RELATION
	{
		LR_OUT,
		LR_INTERSECT,
		LR_INCLUDE,
	};
protected:
	CD3DPlane	m_Planes[6];
public:
	CD3DFrustum();
	~CD3DFrustum();

	static CD3DFrustum FromMatrixs(CD3DMatrix ViewMat,CD3DMatrix PrjMat);
	static CD3DFrustum FromCombinedMatrix(CD3DMatrix TransMat);
	static CD3DFrustum FromCustomViewFace(CD3DMatrix ViewMat,CD3DMatrix PrjMat,FLOAT MaxX,FLOAT MaxY,FLOAT MaxZ,FLOAT MinX,FLOAT MinY,FLOAT MinZ);

	int BoxLocation(const D3DXVECTOR3& Min,const D3DXVECTOR3& Max) const;
	int BoxLocation(const CD3DBoundingBox& Box) const;

	int BoxLocation(D3DXVECTOR3 * pVertices) const;
	int PointLocation(const D3DXVECTOR3 & Point) const;
	int SphereLocation(const D3DXVECTOR3 & Center, FLOAT Radius) const;
	int SphereLocation(CD3DBoundingSphere Sphere) const;

	CD3DFrustum operator*(const CD3DMatrix& Mat);
	void operator*=(const CD3DMatrix& Mat);

	//0����ȫ���⡣1���������ڡ�2����ȫ����

};

inline CD3DFrustum::CD3DFrustum()
{
}
inline CD3DFrustum::~CD3DFrustum()
{
}



inline int CD3DFrustum::SphereLocation(CD3DBoundingSphere Sphere) const
{
	return SphereLocation(Sphere.m_Center,Sphere.m_Radius);
}

inline CD3DFrustum CD3DFrustum::operator*(const CD3DMatrix& Mat)
{
	CD3DFrustum OutFrustum;
	for(int i=0;i<6;i++)
	{
		D3DXPlaneTransform(&(OutFrustum.m_Planes[i]),&(m_Planes[i]),&Mat);
	}
	return OutFrustum;
}

inline void CD3DFrustum::operator*=(const CD3DMatrix& Mat)
{
	for(int i=0;i<6;i++)
	{
		D3DXPlaneTransform(&(m_Planes[i]),&(m_Planes[i]),&Mat);
	}
}

}