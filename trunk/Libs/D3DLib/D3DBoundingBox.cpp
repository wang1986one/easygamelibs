/****************************************************************************/
/*                                                                          */
/*      �ļ���:    D3DBoundingBox.cpp                                       */
/*      ��������:  2009��09��11��                                           */
/*      ����:      Sagasarate                                               */
/*                                                                          */
/*      �������Ȩ��Sagasarate(sagasarate@sina.com)����                     */
/*      ����Խ�����������κ���ҵ�ͷ���ҵ�����������                      */
/*      ���뱣���˰�Ȩ����                                                  */
/*                                                                          */
/****************************************************************************/
#include "StdAfx.h"

namespace D3DLib{

bool CD3DBoundingBox::RayIntersect(const CD3DVector3& Point,const CD3DVector3& Dir,CD3DVector3& IntersectPoint,FLOAT& Distance,bool TestOnly)
{
	CD3DVector3 p1,p2,p3;
	FLOAT U,V;
	//����
	p1=CD3DVector3(m_Min.x,m_Max.y,m_Max.z);
	p2=CD3DVector3(m_Max.x,m_Max.y,m_Max.z);
	p3=CD3DVector3(m_Min.x,m_Max.y,m_Min.z);
	if(D3DXIntersectTri(&p1,&p2,&p3,&Point,&Dir,&U,&V,&Distance))
	{
		IntersectPoint=p1+(p2-p1)*U+(p3-p1)*V;			
		return true;
	}

	p1=CD3DVector3(m_Max.x,m_Max.y,m_Max.z);
	p2=CD3DVector3(m_Max.x,m_Max.y,m_Min.z);
	p3=CD3DVector3(m_Min.x,m_Max.y,m_Min.z);
	if(D3DXIntersectTri(&p1,&p2,&p3,&Point,&Dir,&U,&V,&Distance))
	{
		IntersectPoint=p1+(p2-p1)*U+(p3-p1)*V;			
		return true;
	}
	//����
	p1=CD3DVector3(m_Min.x,m_Min.y,m_Max.z);
	p2=CD3DVector3(m_Max.x,m_Min.y,m_Max.z);
	p3=CD3DVector3(m_Min.x,m_Min.y,m_Min.z);
	if(D3DXIntersectTri(&p1,&p2,&p3,&Point,&Dir,&U,&V,&Distance))
	{
		IntersectPoint=p1+(p2-p1)*U+(p3-p1)*V;			
		return true;
	}

	p1=CD3DVector3(m_Max.x,m_Min.y,m_Max.z);
	p2=CD3DVector3(m_Max.x,m_Min.y,m_Min.z);
	p3=CD3DVector3(m_Min.x,m_Min.y,m_Min.z);
	if(D3DXIntersectTri(&p1,&p2,&p3,&Point,&Dir,&U,&V,&Distance))
	{
		IntersectPoint=p1+(p2-p1)*U+(p3-p1)*V;			
		return true;
	}
	//ǰ��
	p1=CD3DVector3(m_Min.x,m_Max.y,m_Min.z);
	p2=CD3DVector3(m_Max.x,m_Max.y,m_Min.z);
	p3=CD3DVector3(m_Min.x,m_Min.y,m_Min.z);
	if(D3DXIntersectTri(&p1,&p2,&p3,&Point,&Dir,&U,&V,&Distance))
	{
		IntersectPoint=p1+(p2-p1)*U+(p3-p1)*V;			
		return true;
	}
	p1=CD3DVector3(m_Max.x,m_Max.y,m_Min.z);
	p2=CD3DVector3(m_Max.x,m_Min.y,m_Min.z);
	p3=CD3DVector3(m_Min.x,m_Min.y,m_Min.z);
	if(D3DXIntersectTri(&p1,&p2,&p3,&Point,&Dir,&U,&V,&Distance))
	{
		IntersectPoint=p1+(p2-p1)*U+(p3-p1)*V;			
		return true;
	}
	//����
	p1=CD3DVector3(m_Min.x,m_Max.y,m_Max.z);
	p2=CD3DVector3(m_Max.x,m_Max.y,m_Max.z);
	p3=CD3DVector3(m_Min.x,m_Min.y,m_Max.z);
	if(D3DXIntersectTri(&p1,&p2,&p3,&Point,&Dir,&U,&V,&Distance))
	{
		IntersectPoint=p1+(p2-p1)*U+(p3-p1)*V;			
		return true;
	}
	p1=CD3DVector3(m_Max.x,m_Max.y,m_Max.z);
	p2=CD3DVector3(m_Max.x,m_Min.y,m_Max.z);
	p3=CD3DVector3(m_Min.x,m_Min.y,m_Max.z);
	if(D3DXIntersectTri(&p1,&p2,&p3,&Point,&Dir,&U,&V,&Distance))
	{
		IntersectPoint=p1+(p2-p1)*U+(p3-p1)*V;			
		return true;
	}
	//����
	p1=CD3DVector3(m_Min.x,m_Max.y,m_Max.z);
	p2=CD3DVector3(m_Min.x,m_Max.y,m_Min.z);
	p3=CD3DVector3(m_Min.x,m_Min.y,m_Max.z);
	if(D3DXIntersectTri(&p1,&p2,&p3,&Point,&Dir,&U,&V,&Distance))
	{
		IntersectPoint=p1+(p2-p1)*U+(p3-p1)*V;			
		return true;
	}
	p1=CD3DVector3(m_Min.x,m_Max.y,m_Min.z);
	p2=CD3DVector3(m_Min.x,m_Min.y,m_Min.z);
	p3=CD3DVector3(m_Min.x,m_Min.y,m_Max.z);
	if(D3DXIntersectTri(&p1,&p2,&p3,&Point,&Dir,&U,&V,&Distance))
	{
		IntersectPoint=p1+(p2-p1)*U+(p3-p1)*V;			
		return true;
	}
	//����
	p1=CD3DVector3(m_Max.x,m_Max.y,m_Max.z);
	p2=CD3DVector3(m_Max.x,m_Max.y,m_Min.z);
	p3=CD3DVector3(m_Max.x,m_Min.y,m_Max.z);
	if(D3DXIntersectTri(&p1,&p2,&p3,&Point,&Dir,&U,&V,&Distance))
	{
		IntersectPoint=p1+(p2-p1)*U+(p3-p1)*V;			
		return true;
	}
	p1=CD3DVector3(m_Max.x,m_Max.y,m_Min.z);
	p2=CD3DVector3(m_Max.x,m_Min.y,m_Min.z);
	p3=CD3DVector3(m_Max.x,m_Min.y,m_Max.z);
	if(D3DXIntersectTri(&p1,&p2,&p3,&Point,&Dir,&U,&V,&Distance))
	{
		IntersectPoint=p1+(p2-p1)*U+(p3-p1)*V;			
		return true;
	}
	return false;
}

}