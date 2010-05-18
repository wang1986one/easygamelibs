/****************************************************************************/
/*                                                                          */
/*      �ļ���:    D3DVector3.h                                             */
/*      ��������:  2009��09��25��                                           */
/*      ����:      Sagasarate                                               */
/*                                                                          */
/*      �������Ȩ��Sagasarate(sagasarate@sina.com)����                     */
/*      ����Խ�����������κ���ҵ�ͷ���ҵ�����������                      */
/*      ���뱣���˰�Ȩ����                                                  */
/*                                                                          */
/****************************************************************************/
#pragma once

namespace D3DLib{

class CD3DVector3 :
	public D3DXVECTOR3
{
public:
	CD3DVector3(void):D3DXVECTOR3(){}
	CD3DVector3(const FLOAT * pData):D3DXVECTOR3(pData){}
	CD3DVector3(const D3DVECTOR& Vector):D3DXVECTOR3(Vector){}
	CD3DVector3(const D3DXFLOAT16 * pData):D3DXVECTOR3(pData){}
	CD3DVector3(FLOAT x, FLOAT y, FLOAT z):D3DXVECTOR3(x,y,z){}
	CD3DVector3(FLOAT Value);

	~CD3DVector3(void){}

	void SetValue(FLOAT vx, FLOAT vy, FLOAT vz);

	//��׼��
	void Normalize();
	CD3DVector3 GetNormalize();

	//����
	FLOAT Length();
	FLOAT LengthSq();

	//���
	FLOAT Dot(const D3DXVECTOR3& V2);

	//���
	CD3DVector3 Cross(const D3DXVECTOR3& V2);

	//����任
	CD3DVector3 operator*(const D3DXMATRIX& Mat);
	void operator*=(const D3DXMATRIX& Mat);

	CD3DVector3 operator*(FLOAT Len);
	void operator*=(FLOAT Len);
	void operator=(FLOAT Value);

	CD3DVector3 operator+(const CD3DVector3& V2);
	void operator+=(const CD3DVector3& V2);

	CD3DVector3 operator*(const D3DXVECTOR3& V2);

	//��ֵ
	static CD3DVector3 Lerp(const D3DXVECTOR3& v1,const D3DXVECTOR3& v2,FLOAT s);
};

inline CD3DVector3::CD3DVector3(FLOAT Value)
{
	x=Value;
	y=Value;
	z=Value;
}


inline void CD3DVector3::SetValue(FLOAT vx, FLOAT vy, FLOAT vz)
{
	x=vx;
	y=vy;
	z=vz;
}

inline void CD3DVector3::Normalize()
{
	D3DXVec3Normalize(this,this);
}

inline CD3DVector3 CD3DVector3::GetNormalize()
{
	CD3DVector3 Vector;
	D3DXVec3Normalize(&Vector,this);
	return Vector;
}

inline FLOAT CD3DVector3::Length()
{
	return D3DXVec3Length(this);
}

inline FLOAT CD3DVector3::LengthSq()
{
	return D3DXVec3LengthSq(this);
}

inline FLOAT CD3DVector3::Dot(const D3DXVECTOR3& V2)
{
	return D3DXVec3Dot(this,&V2);
}


inline CD3DVector3 CD3DVector3::Cross(const D3DXVECTOR3& V2)
{
	CD3DVector3 RetVec;

	D3DXVec3Cross(&RetVec,this,&V2);

	return RetVec;
}

inline CD3DVector3 CD3DVector3::operator*(const D3DXMATRIX& Mat)
{
	D3DXVECTOR4 RetVec;
	D3DXVec3Transform(&RetVec,this,&Mat);
	return CD3DVector3(RetVec.x/RetVec.w,RetVec.y/RetVec.w,RetVec.z/RetVec.w);
}

inline void CD3DVector3::operator*=(const D3DXMATRIX& Mat)
{
	*this=*this*Mat;
}

inline CD3DVector3 CD3DVector3::operator*(FLOAT Len)
{
	return CD3DVector3(x*Len,y*Len,z*Len);
}

inline void CD3DVector3::operator*=(FLOAT Len)
{
	x*=Len;
	y*=Len;
	z*=Len;
}

inline void CD3DVector3::operator=(FLOAT Value)
{
	x=Value;
	y=Value;
	z=Value;
}

inline CD3DVector3 CD3DVector3::operator+(const CD3DVector3& V2)
{
	CD3DVector3 RetVec;

	D3DXVec3Add(&RetVec,this,&V2);

	return RetVec;
}

inline void CD3DVector3::operator+=(const CD3DVector3& V2)
{
	D3DXVec3Add(this,this,&V2);
}

inline CD3DVector3 CD3DVector3::operator*(const D3DXVECTOR3& V2)
{
	CD3DVector3 RetVec;
	RetVec.x=x*V2.x;
	RetVec.y=y*V2.y;
	RetVec.z=z*V2.z;

	return RetVec;
}

inline CD3DVector3 CD3DVector3::Lerp(const D3DXVECTOR3& v1,const D3DXVECTOR3& v2,FLOAT s)
{
	CD3DVector3 RetVec;

	D3DXVec3Lerp(&RetVec,&v1,&v2,s);

	return RetVec;
}



}