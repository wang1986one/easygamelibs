/****************************************************************************/
/*                                                                          */
/*      �ļ���:    D3DVector2.h                                             */
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

class CD3DVector2 :
	public D3DXVECTOR2
{
public:
	CD3DVector2(void):D3DXVECTOR2(){}
	CD3DVector2(const FLOAT * pData):D3DXVECTOR2(pData){}
	CD3DVector2(const D3DXVECTOR2& Vector):D3DXVECTOR2(Vector){}
	CD3DVector2(const D3DXFLOAT16 * pData):D3DXVECTOR2(pData){}
	CD3DVector2(FLOAT x, FLOAT y):D3DXVECTOR2(x,y){}
	CD3DVector2(FLOAT Value);

	~CD3DVector2(void){}

	void SetValue(FLOAT vx, FLOAT vy);

	//��׼��
	void Normalize();
	CD3DVector2 GetNormalize();

	//����
	FLOAT Length();
	FLOAT LengthSq();

	//���
	FLOAT Dot(const D3DXVECTOR2& V2);	

	const CD3DVector2& operator=(FLOAT Value);

};

inline CD3DVector2::CD3DVector2(FLOAT Value)
{
	x=Value;
	y=Value;
}

inline void CD3DVector2::SetValue(FLOAT vx, FLOAT vy)
{
	x=vx;
	y=vy;
}

inline void CD3DVector2::Normalize()
{
	D3DXVec2Normalize(this,this);
}

inline CD3DVector2 CD3DVector2::GetNormalize()
{
	CD3DVector2 Vector;
	D3DXVec2Normalize(&Vector,this);
	return Vector;
}

inline FLOAT CD3DVector2::Length()
{
	return D3DXVec2Length(this);
}

inline FLOAT CD3DVector2::LengthSq()
{
	return D3DXVec2LengthSq(this);
}

inline FLOAT CD3DVector2::Dot(const D3DXVECTOR2& V2)
{
	return D3DXVec2Dot(this,&V2);
}

inline const CD3DVector2& CD3DVector2::operator=(FLOAT Value)
{
	x=Value;
	y=Value;
	return *this;
}

}