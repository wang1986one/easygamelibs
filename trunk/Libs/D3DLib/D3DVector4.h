/****************************************************************************/
/*                                                                          */
/*      �ļ���:    D3DVector4.h                                             */
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

class CD3DVector4 :
	public D3DXVECTOR4
{
public:
	CD3DVector4(void):D3DXVECTOR4(){}
	CD3DVector4(const FLOAT * pData):D3DXVECTOR4(pData){}
	CD3DVector4(const D3DXVECTOR4& Vector):D3DXVECTOR4(Vector){}
	CD3DVector4(const D3DXFLOAT16 * pData):D3DXVECTOR4(pData){}
	CD3DVector4(FLOAT x, FLOAT y, FLOAT z, FLOAT w):D3DXVECTOR4(x,y,z,w){}
	CD3DVector4(const D3DVECTOR& Vec);
	CD3DVector4(const D3DCOLORVALUE& Color);

	~CD3DVector4(void){}

	void SetValue(FLOAT vx, FLOAT vy, FLOAT vz, FLOAT vw);

	//��׼��
	void Normalize();
	CD3DVector4 GetNormalize();

	//����
	FLOAT Length();
	FLOAT LengthSq();

	//���
	FLOAT Dot(const D3DXVECTOR4& V2);

	//���
	CD3DVector4 Cross(const D3DXVECTOR4& V2,const D3DXVECTOR4& V3);
};

inline CD3DVector4::CD3DVector4(const D3DVECTOR& Vec)
{
	x=Vec.x;
	y=Vec.y;
	z=Vec.z;
	w=1.0f;
}

inline CD3DVector4::CD3DVector4(const D3DCOLORVALUE& Color)
{
	x=Color.r;
	y=Color.g;
	z=Color.b;
	w=Color.a;
}

inline void CD3DVector4::SetValue(FLOAT vx, FLOAT vy, FLOAT vz, FLOAT vw)
{
	x=vx;
	y=vy;
	z=vz;
	w=vw;
}

inline void CD3DVector4::Normalize()
{
	D3DXVec4Normalize(this,this);
}

inline CD3DVector4 CD3DVector4::GetNormalize()
{
	CD3DVector4 Vector;
	D3DXVec4Normalize(&Vector,this);
	return Vector;
}

inline FLOAT CD3DVector4::Length()
{
	return D3DXVec4Length(this);
}

inline FLOAT CD3DVector4::LengthSq()
{
	return D3DXVec4LengthSq(this);
}


inline FLOAT CD3DVector4::Dot(const D3DXVECTOR4& V2)
{
	return D3DXVec4Dot(this,&V2);
}


inline CD3DVector4 CD3DVector4::Cross(const D3DXVECTOR4& V2,const D3DXVECTOR4& V3)
{
	CD3DVector4 RetVec;

	D3DXVec4Cross(&RetVec,this,&V2,&V3);

	return RetVec;
}

}