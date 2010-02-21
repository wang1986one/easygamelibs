#pragma once

namespace D3DLib{

class CD3DFrustum
{
protected:
	CD3DPlane	m_Planes[6];
public:
	CD3DFrustum();
	~CD3DFrustum();

	static CD3DFrustum FromMatrixs(const CD3DMatrix& WorldMat,const CD3DMatrix& ViewMat,const CD3DMatrix& PrjMat);
	static CD3DFrustum FromCombinedMatrix(CD3DMatrix TransMat);

	int BoxLocation(const D3DXVECTOR3& Min,const D3DXVECTOR3& Max);
	int BoxLocation(const CD3DBoundingBox& Box);

	int BoxLocation(D3DXVECTOR3 * pVertices);
	int PointLocation(const D3DXVECTOR3 & Point);
	int SphereLocation(const D3DXVECTOR3 & Center, FLOAT Radius);
	int SphereLocation(CD3DBoundingSphere Sphere);

	CD3DFrustum operator*(const CD3DMatrix& Mat);
	void operator*=(const CD3DMatrix& Mat);

	//0、完全在外。1、部分在内。2、完全在内

};

inline CD3DFrustum::CD3DFrustum()
{
}
inline CD3DFrustum::~CD3DFrustum()
{
}

inline CD3DFrustum CD3DFrustum::FromMatrixs(const CD3DMatrix& WorldMat,const CD3DMatrix& ViewMat,const CD3DMatrix& PrjMat)
{
	return FromCombinedMatrix(WorldMat*ViewMat*PrjMat);
}

inline int CD3DFrustum::SphereLocation(CD3DBoundingSphere Sphere)
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