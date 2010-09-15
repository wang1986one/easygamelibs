/****************************************************************************/
/*                                                                          */
/*      �ļ���:    D3DBoundingBox.h                                         */
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

class CD3DBoundingBox
{
public:
	CD3DVector3		m_Min;
	CD3DVector3		m_Max;

	enum RELATION_TYPE
	{
		RELATION_TYPE_OUT,					//����
		RELATION_TYPE_INCLUDE,				//����
		RELATION_TYPE_BE_INCLUDE,			//������
		RELATION_TYPE_INTERSECT,			//�ཻ
	};
public:
	CD3DBoundingBox();
	CD3DBoundingBox(const CD3DBoundingBox& Box);
	CD3DBoundingBox(FLOAT MinX,FLOAT MinY,FLOAT MinZ,FLOAT MaxX,FLOAT MaxY,FLOAT MaxZ);
	CD3DBoundingBox(D3DXVECTOR3 Min,D3DXVECTOR3 Max);
	~CD3DBoundingBox(){}

	void ComputeFromVertex(LPVOID pVertext,int VertexCount,int VertexSize);
	void AppendFromVertex(LPVOID pVertext,int VertexCount,int VertexSize);

	CD3DBoundingBox operator+(const CD3DBoundingBox& Box);
	CD3DBoundingBox& operator+=(const CD3DBoundingBox& Box);

	CD3DVector3 GetBoxPoint(int Index);
	CD3DVector3 GetSize();

	CD3DBoundingBox operator*(const CD3DMatrix& Mat);
	void operator*=(const CD3DMatrix& Mat);

	bool RayIntersect(const CD3DVector3& Point,const CD3DVector3& Dir,CD3DVector3& IntersectPoint,FLOAT& Distance,bool TestOnly=true);
	void Rebuild();
	int CheckRelation(const CD3DBoundingBox& BBox);
	int CheckRelation(const CD3DVector3& Point);
	void Merge(const CD3DBoundingBox& BBox);
	bool IsValid();
};

inline CD3DBoundingBox::CD3DBoundingBox()
{
	m_Min.x=0;
	m_Min.y=0;
	m_Min.z=0;
	m_Max.x=0;
	m_Max.y=0;
	m_Max.z=0;
}

inline CD3DBoundingBox::CD3DBoundingBox(const CD3DBoundingBox& Box)
{
	m_Min=Box.m_Min;
	m_Max=Box.m_Max;
}

inline CD3DBoundingBox::CD3DBoundingBox(FLOAT MinX,FLOAT MinY,FLOAT MinZ,FLOAT MaxX,FLOAT MaxY,FLOAT MaxZ)
{
	m_Min.x=MinX;
	m_Min.y=MinY;
	m_Min.z=MinZ;
	m_Max.x=MaxX;
	m_Max.y=MaxY;
	m_Max.z=MaxZ;
}

inline CD3DBoundingBox::CD3DBoundingBox(D3DXVECTOR3 Min,D3DXVECTOR3 Max)
{
	m_Min=Min;
	m_Max=Max;
}

inline void CD3DBoundingBox::ComputeFromVertex(LPVOID pVertext,int VertexCount,int VertexSize)
{
	D3DXComputeBoundingBox((D3DXVECTOR3 *)pVertext,VertexCount,VertexSize,&m_Min,&m_Max);
}
inline void CD3DBoundingBox::AppendFromVertex(LPVOID pVertext,int VertexCount,int VertexSize)
{
	CD3DVector3 Min,Max;
	D3DXComputeBoundingBox((D3DXVECTOR3 *)pVertext,VertexCount,VertexSize,&Min,&Max);
	if(Min.x<m_Min.x)
		m_Min.x=Min.x;
	if(Min.y<m_Min.y)
		m_Min.y=Min.y;
	if(Min.z<m_Min.z)
		m_Min.z=Min.z;

	if(Max.x>m_Max.x)
		m_Max.x=Max.x;
	if(Max.y>m_Max.y)
		m_Max.y=Max.y;
	if(Max.z>m_Max.z)
		m_Max.z=Max.z;
}

inline CD3DBoundingBox CD3DBoundingBox::operator+(const CD3DBoundingBox& Box)
{
	return CD3DBoundingBox(
		Box.m_Min.x<m_Min.x?Box.m_Min.x:m_Min.x,
		Box.m_Min.y<m_Min.y?Box.m_Min.y:m_Min.y,
		Box.m_Min.z<m_Min.z?Box.m_Min.z:m_Min.z,
		Box.m_Max.x>m_Max.x?Box.m_Max.x:m_Max.x,
		Box.m_Max.y>m_Max.y?Box.m_Max.y:m_Max.y,
		Box.m_Max.z>m_Max.z?Box.m_Max.z:m_Max.z
		);

}

inline CD3DBoundingBox& CD3DBoundingBox::operator+=(const CD3DBoundingBox& Box)
{
	if(Box.m_Min.x<m_Min.x)
		m_Min.x=Box.m_Min.x;
	if(Box.m_Min.y<m_Min.y)
		m_Min.y=Box.m_Min.y;
	if(Box.m_Min.z<m_Min.z)
		m_Min.z=Box.m_Min.z;

	if(Box.m_Max.x>m_Max.x)
		m_Max.x=Box.m_Max.x;
	if(Box.m_Max.y>m_Max.y)
		m_Max.y=Box.m_Max.y;
	if(Box.m_Max.z>m_Max.z)
		m_Max.z=Box.m_Max.z;

	return *this;
}

inline CD3DVector3 CD3DBoundingBox::GetBoxPoint(int Index)
{
	CD3DVector3 Point;

	Point.x=(Index&1)?m_Min.x:m_Max.x;
	Point.y=(Index&2)?m_Min.y:m_Max.y;
	Point.z=(Index&4)?m_Min.z:m_Max.z;
	return Point;
}

inline CD3DVector3 CD3DBoundingBox::GetSize()
{
	return m_Max-m_Min;
}

inline CD3DBoundingBox CD3DBoundingBox::operator*(const CD3DMatrix& Mat)
{
	CD3DVector3	Points[8];

	for(int i=0;i<8;i++)
	{
		Points[i]=GetBoxPoint(i)*Mat;

	}
	CD3DBoundingBox BBox;
	BBox.ComputeFromVertex(Points,8,sizeof(CD3DVector3));
	return BBox;

	//CD3DBoundingBox BBox;
	//BBox.m_Min=m_Min*Mat;
	//BBox.m_Max=m_Max*Mat;
	//if(BBox.m_Min.x>BBox.m_Max.x)
	//	Swap(BBox.m_Min.x,BBox.m_Max.x);
	//if(BBox.m_Min.y>BBox.m_Max.y)
	//	Swap(BBox.m_Min.y,BBox.m_Max.y);
	//if(BBox.m_Min.z>BBox.m_Max.z)
	//	Swap(BBox.m_Min.z,BBox.m_Max.z);
	//return BBox;
}

inline void CD3DBoundingBox::operator*=(const CD3DMatrix& Mat)
{
	CD3DVector3	Points[8];

	for(int i=0;i<8;i++)
	{
		Points[i]=GetBoxPoint(i)*Mat;

	}	
	ComputeFromVertex(Points,8,sizeof(CD3DVector3));

	//m_Min*=Mat;
	//m_Max*=Mat;
	//if(m_Min.x>m_Max.x)
	//	Swap(m_Min.x,m_Max.x);
	//if(m_Min.y>m_Max.y)
	//	Swap(m_Min.y,m_Max.y);
	//if(m_Min.z>m_Max.z)
	//	Swap(m_Min.z,m_Max.z);
}

inline void CD3DBoundingBox::Rebuild()
{
	CD3DVector3	Points[8];

	for(int i=0;i<8;i++)
	{
		Points[i]=GetBoxPoint(i);

	}	
	ComputeFromVertex(Points,8,sizeof(CD3DVector3));
}

inline int CD3DBoundingBox::CheckRelation(const CD3DBoundingBox& BBox)
{
	if(m_Max.x<BBox.m_Min.x||
		m_Max.y<BBox.m_Min.y||
		m_Max.z<BBox.m_Min.z||
		m_Min.x>BBox.m_Max.x||
		m_Min.y>BBox.m_Max.y||
		m_Min.z>BBox.m_Max.z)
	{
		return RELATION_TYPE_OUT;
	}

	if(m_Max.x>=BBox.m_Max.x&&
		m_Max.y>=BBox.m_Max.y&&
		m_Max.z>=BBox.m_Max.z&&
		m_Min.x<=BBox.m_Min.x&&
		m_Min.y<=BBox.m_Min.y&&
		m_Min.z<=BBox.m_Min.z)
	{
		return RELATION_TYPE_INCLUDE;
	}

	if(m_Max.x<=BBox.m_Max.x&&
		m_Max.y<=BBox.m_Max.y&&
		m_Max.z<=BBox.m_Max.z&&
		m_Min.x>=BBox.m_Min.x&&
		m_Min.y>=BBox.m_Min.y&&
		m_Min.z>=BBox.m_Min.z)
	{
		return RELATION_TYPE_BE_INCLUDE;
	}

	return RELATION_TYPE_INTERSECT;
}

inline int CD3DBoundingBox::CheckRelation(const CD3DVector3& Point)
{
	if(Point.x<=m_Max.x&&Point.x>=m_Min.x&&
		Point.y<=m_Max.y&&Point.y>=m_Min.y&&
		Point.z<=m_Max.z&&Point.z>=m_Min.z)
	{
		return RELATION_TYPE_INCLUDE;
	}
	return RELATION_TYPE_OUT;
}

inline void CD3DBoundingBox::Merge(const CD3DBoundingBox& BBox)
{
	m_Max.x=max(m_Max.x,BBox.m_Max.x);
	m_Max.y=max(m_Max.y,BBox.m_Max.y);
	m_Max.z=max(m_Max.z,BBox.m_Max.z);
	m_Min.x=min(m_Min.x,BBox.m_Min.x);
	m_Min.y=min(m_Min.y,BBox.m_Min.y);
	m_Min.z=min(m_Min.z,BBox.m_Min.z);
}

inline bool CD3DBoundingBox::IsValid()
{
	return m_Max.IsValid()&&m_Min.IsValid();
}

}