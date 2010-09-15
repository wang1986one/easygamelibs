/****************************************************************************/
/*                                                                          */
/*      �ļ���:    D3DMatrix.h                                              */
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

class CD3DMatrix :
	public D3DXMATRIX
{
public:
	CD3DMatrix():D3DXMATRIX(){}
	CD3DMatrix(const FLOAT * pData):D3DXMATRIX(pData){}
	CD3DMatrix(const D3DMATRIX& Mat):D3DXMATRIX(Mat){}
	CD3DMatrix(const D3DXFLOAT16 * pMat):D3DXMATRIX(pMat){}
	CD3DMatrix(	FLOAT _11, FLOAT _12, FLOAT _13, FLOAT _14,
				FLOAT _21, FLOAT _22, FLOAT _23, FLOAT _24,
				FLOAT _31, FLOAT _32, FLOAT _33, FLOAT _34,
				FLOAT _41, FLOAT _42, FLOAT _43, FLOAT _44)
		:D3DXMATRIX(_11, _12, _13, _14,
					_21, _22, _23, _24,
					_31, _32, _33, _34,
					_41, _42, _43, _44){}

	~CD3DMatrix(){}


	const CD3DMatrix& operator=(const CD3DMatrix& Mat);

	//��Ϊ��λ����
	void SetIdentity();	
	//�Ƿ�λ����
	BOOL IsIdentity() const;
	//ת��
	void Transpose();
	//���ת��
	CD3DMatrix GetTranspose() const;
	//����
	void Invert();
	//�����
	CD3DMatrix GetInverse() const;
	//�����
	CD3DVector4 GetRow4v(int row) const;
	//�����
	CD3DVector4 GetCol4v(int col) const;
	//������
	void SetRow4v(int row, const D3DXVECTOR4& Vector);
	//������
	void SetCol4v(int col, const D3DXVECTOR4& Vector);

	//�����
	CD3DVector3 GetRow3v(int row) const;
	//�����
	CD3DVector3 GetCol3v(int col) const;
	//������
	void SetRow3v(int row, const D3DVECTOR& Vector);
	//������
	void SetCol3v(int col, const D3DVECTOR& Vector);

	//���λ��
	CD3DVector3 GetTranslation() const;
	//����λ��
	void SetTranslation(const D3DVECTOR& Translation);
	void SetTranslation(FLOAT x,FLOAT y,FLOAT z);

	//�����ת
	CD3DMatrix GetRotation() const;
	CD3DQuaternion GetRotationOnQuaternion() const;

	CD3DMatrix GetScaleRotation() const;
	//������ת
	void SetRotation(const D3DXMATRIX& Rotation);	

	//�������
	CD3DVector3 GetScale() const;
	//��������
	void SetScale(const D3DVECTOR& Scale);	

	void SetTransformation(const D3DXVECTOR3& Scale,const D3DXQUATERNION& Rotation,const D3DXVECTOR3& Translation);

	//��λ�ƽ�������
	static CD3DMatrix FromTranslation(FLOAT x,FLOAT y,FLOAT z);
	static CD3DMatrix FromTranslation(const D3DVECTOR& Translation);

	//����X����ת����
	static CD3DMatrix FromRotationX(FLOAT Angle);
	//����Y����ת����
	static CD3DMatrix FromRotationY(FLOAT Angle);
	//����Y����ת����
	static CD3DMatrix FromRotationZ(FLOAT Angle);
	//������������ת����
	static CD3DMatrix FromRotationAxis(const D3DXVECTOR3& Axis,FLOAT Angle);
	//��ŷ���ǽ�����ת����
	static CD3DMatrix FromRotationYawPitchRoll(FLOAT Yaw,FLOAT Pitch,FLOAT Roll);
	//����Ԫ������ת����
	static CD3DMatrix FromRotationQuaternion(const D3DXQUATERNION& Quaternion);

	//�����Ž�������
	static CD3DMatrix FromScale(FLOAT x,FLOAT y,FLOAT z);
	static CD3DMatrix FromScale(const D3DVECTOR& Scale);

	//�����任����
	static CD3DMatrix FromTransformation(const D3DXVECTOR3& Scale,const D3DXQUATERNION& Rotation,const D3DXVECTOR3& Translation);

	//�����Ӿ���
	static CD3DMatrix FromLookAtLH(const D3DXVECTOR3& Eye,const D3DXVECTOR3& At,const D3DXVECTOR3& Up);
	static CD3DMatrix FromLookAtRH(const D3DXVECTOR3& Eye,const D3DXVECTOR3& At,const D3DXVECTOR3& Up);

	//����͸��ͶӰ����
	static CD3DMatrix FromPerspectiveFovLH(FLOAT Fov,FLOAT Aspect,FLOAT Near,FLOAT Far);
	static CD3DMatrix FromPerspectiveFovRH(FLOAT Fov,FLOAT Aspect,FLOAT Near,FLOAT Far);

	static CD3DMatrix FromPerspectiveOffCenterLH(FLOAT Left,FLOAT Right,FLOAT Top,FLOAT Bottom,FLOAT Near,FLOAT Far);
	static CD3DMatrix FromPerspectiveOffCenterRH(FLOAT Left,FLOAT Right,FLOAT Top,FLOAT Bottom,FLOAT Near,FLOAT Far);

	//��������ͶӰ����
	static CD3DMatrix FromOrthoLH(FLOAT Width,FLOAT Height,FLOAT Near,FLOAT Far);
	static CD3DMatrix FromOrthoRH(FLOAT Width,FLOAT Height,FLOAT Near,FLOAT Far);

	static CD3DMatrix FromOrthoOffCenterLH(FLOAT Left,FLOAT Right,FLOAT Top,FLOAT Bottom,FLOAT Near,FLOAT Far);
	static CD3DMatrix FromOrthoOffCenterRH(FLOAT Left,FLOAT Right,FLOAT Top,FLOAT Bottom,FLOAT Near,FLOAT Far);

	

};

inline const CD3DMatrix& CD3DMatrix::operator=(const CD3DMatrix& Mat)
{
	memcpy((FLOAT*)(*this),(CONST FLOAT*)Mat,sizeof(D3DXMATRIX));
	return *this;
}

inline void CD3DMatrix::SetIdentity()
{
	//D3DXMatrixIdentity(this);
	ZeroMemory(this, sizeof(CD3DMatrix));	
	_11 = _22 = _33 = _44 = 1.0f;
}

inline BOOL CD3DMatrix::IsIdentity() const
{
	return	D3DXMatrixIsIdentity(this);
}

inline void CD3DMatrix::Transpose()
{
	D3DXMatrixTranspose(this,this);
}

inline CD3DMatrix CD3DMatrix::GetTranspose() const
{
	CD3DMatrix Mat;
	D3DXMatrixTranspose(&Mat,this);
	return Mat;
}

inline void CD3DMatrix::Invert()
{
	D3DXMatrixInverse(this,NULL,this);
}

inline CD3DMatrix CD3DMatrix::GetInverse() const
{
	CD3DMatrix Mat;
	D3DXMatrixInverse(&Mat,NULL,this);
	return Mat;
}

inline CD3DVector4 CD3DMatrix::GetRow4v(int row) const
{
	CD3DVector4 Vector;

	Vector.x=m[row][0];
	Vector.y=m[row][1];
	Vector.z=m[row][2];
	Vector.w=m[row][3];
	return Vector;
}

inline CD3DVector4 CD3DMatrix::GetCol4v(int col) const
{
	CD3DVector4 Vector;

	Vector.x=m[0][col];
	Vector.y=m[1][col];
	Vector.z=m[2][col];
	Vector.w=m[3][col];
	return Vector;
}

inline void CD3DMatrix::SetRow4v(int row, const D3DXVECTOR4& Vector)
{
	m[row][0]=Vector.x;
	m[row][1]=Vector.y;
	m[row][2]=Vector.z;
	m[row][3]=Vector.w;
}

inline void CD3DMatrix::SetCol4v(int col, const D3DXVECTOR4& Vector)
{
	m[0][col]=Vector.x;
	m[1][col]=Vector.y;
	m[2][col]=Vector.z;
	m[3][col]=Vector.w;
}

inline CD3DVector3 CD3DMatrix::GetRow3v(int row) const
{
	CD3DVector3 Vector;

	Vector.x=m[row][0];
	Vector.y=m[row][1];
	Vector.z=m[row][2];	
	return Vector;
}

inline CD3DVector3 CD3DMatrix::GetCol3v(int col) const
{
	CD3DVector3 Vector;

	Vector.x=m[0][col];
	Vector.y=m[1][col];
	Vector.z=m[2][col];	
	return Vector;
}

inline void CD3DMatrix::SetRow3v(int row, const D3DVECTOR& Vector)
{
	m[row][0]=Vector.x;
	m[row][1]=Vector.y;
	m[row][2]=Vector.z;
}

inline void CD3DMatrix::SetCol3v(int col, const D3DVECTOR& Vector)
{
	m[0][col]=Vector.x;
	m[1][col]=Vector.y;
	m[2][col]=Vector.z;
}

inline CD3DVector3 CD3DMatrix::GetTranslation() const
{
	return CD3DVector3(_41, _42, _43);
}

inline void CD3DMatrix::SetTranslation(const D3DVECTOR& Translation)
{
	_41 = Translation.x;
	_42 = Translation.y;
	_43 = Translation.z;
}

inline void CD3DMatrix::SetTranslation(FLOAT x,FLOAT y,FLOAT z)
{
	_41 = x;
	_42 = y;
	_43 = z;
}

inline CD3DMatrix CD3DMatrix::GetRotation() const
{
	CD3DMatrix Mat;
	Mat.SetIdentity();
	CD3DVector3 v;
	for(int i = 0; i < 3; ++i)
	{
		v.x = (*this)(i,0); 
		v.y = (*this)(i,1);
		v.z = (*this)(i,2);
		v.Normalize();
		Mat(i, 0) = v.x;
		Mat(i, 1) = v.y;
		Mat(i, 2) = v.z;
	}
	return Mat;
}

inline CD3DQuaternion CD3DMatrix::GetRotationOnQuaternion() const
{
	CD3DQuaternion Quaternion;
	D3DXQuaternionRotationMatrix(&Quaternion,&GetRotation());
	return Quaternion;
}

inline CD3DMatrix CD3DMatrix::GetScaleRotation() const
{
	CD3DMatrix Mat(*this);

	Mat._41=0.0f;
	Mat._42=0.0f;
	Mat._43=0.0f;

	return Mat;
}

inline void CD3DMatrix::SetRotation(const D3DXMATRIX& Rotation)
{
	CD3DVector3 Scale = GetScale();
	CD3DVector3 Translation = GetTranslation();
	*this = Rotation * FromScale(Scale);
	SetTranslation(Translation);
}

inline CD3DVector3 CD3DMatrix::GetScale() const
{
	CD3DVector3 l1(_11, _12, _13), l2(_21, _22, _23), l3(_31, _32, _33);
	return CD3DVector3(l1.Length(), l2.Length(), l3.Length());
}

inline void CD3DMatrix::SetScale(const D3DVECTOR& Scale)
{	
	CD3DMatrix Rotation  = GetRotation();
	CD3DVector3 Trans = GetTranslation();
	*this = CD3DMatrix::FromScale(Scale) * Rotation;
	SetTranslation(Trans);
}

inline void CD3DMatrix::SetTransformation(const D3DXVECTOR3& Scale,const D3DXQUATERNION& Rotation,const D3DXVECTOR3& Translation)
{
	D3DXMatrixTransformation(this,NULL,NULL,&Scale,NULL,&Rotation,&Translation);
}

//��λ�ƽ�������
inline CD3DMatrix CD3DMatrix::FromTranslation(FLOAT x,FLOAT y,FLOAT z)
{
	CD3DMatrix Mat;
	D3DXMatrixTranslation(&Mat,x,y,z);
	return Mat;
}

inline CD3DMatrix CD3DMatrix::FromTranslation(const D3DVECTOR& Translation)
{
	return FromTranslation(Translation.x,Translation.y,Translation.z);
}

//����X����ת����
inline CD3DMatrix CD3DMatrix::FromRotationX(FLOAT Angle)
{
	CD3DMatrix Mat;	
	D3DXMatrixRotationX(&Mat,Angle);
	return Mat;
}
//����Y����ת����
inline CD3DMatrix CD3DMatrix::FromRotationY(FLOAT Angle)
{
	CD3DMatrix Mat;	
	D3DXMatrixRotationY(&Mat,Angle);
	return Mat;
}
//����Y����ת����
inline CD3DMatrix CD3DMatrix::FromRotationZ(FLOAT Angle)
{
	CD3DMatrix Mat;	
	D3DXMatrixRotationZ(&Mat,Angle);
	return Mat;
}
//������������ת����
inline CD3DMatrix CD3DMatrix::FromRotationAxis(const D3DXVECTOR3& Axis,FLOAT Angle)
{
	CD3DMatrix Mat;	
	D3DXMatrixRotationAxis(&Mat,&Axis,Angle);
	return Mat;
}
//��ŷ���ǽ�����ת����
inline CD3DMatrix CD3DMatrix::FromRotationYawPitchRoll(FLOAT Yaw,FLOAT Pitch,FLOAT Roll)
{
	CD3DMatrix Mat;
	D3DXMatrixRotationYawPitchRoll(&Mat,Yaw,Pitch,Roll);
	return Mat;
}
//����Ԫ������ת����
inline CD3DMatrix CD3DMatrix::FromRotationQuaternion(const D3DXQUATERNION& Quaternion)
{
	CD3DMatrix Mat;	
	D3DXMatrixRotationQuaternion(&Mat,&Quaternion);
	return Mat;
}

//�����Ž�������
inline CD3DMatrix CD3DMatrix::FromScale(FLOAT x,FLOAT y,FLOAT z)
{
	CD3DMatrix Mat;	
	D3DXMatrixScaling(&Mat,x,y,z);
	return Mat;
}

inline CD3DMatrix CD3DMatrix::FromScale(const D3DVECTOR& Scale)
{
	return FromScale(Scale.x,Scale.y,Scale.z);
}


inline CD3DMatrix CD3DMatrix::FromTransformation(const D3DXVECTOR3& Scale,const D3DXQUATERNION& Rotation,const D3DXVECTOR3& Translation)
{
	CD3DMatrix Mat;
	D3DXMatrixTransformation(&Mat,NULL,NULL,&Scale,NULL,&Rotation,&Translation);
	return Mat;
}

//�����Ӿ���
inline CD3DMatrix CD3DMatrix::FromLookAtLH(const D3DXVECTOR3& Eye,const D3DXVECTOR3& At,const D3DXVECTOR3& Up)
{
	CD3DMatrix Mat;	
	D3DXMatrixLookAtLH(&Mat,&Eye,&At,&Up);
	return Mat;
}
inline CD3DMatrix CD3DMatrix::FromLookAtRH(const D3DXVECTOR3& Eye,const D3DXVECTOR3& At,const D3DXVECTOR3& Up)
{
	CD3DMatrix Mat;	
	D3DXMatrixLookAtRH(&Mat,&Eye,&At,&Up);
	return Mat;
}



//����͸��ͶӰ����
inline CD3DMatrix CD3DMatrix::FromPerspectiveFovLH(FLOAT Fov,FLOAT Aspect,FLOAT Near,FLOAT Far)
{
	CD3DMatrix Mat;	
	D3DXMatrixPerspectiveFovLH(&Mat,Fov,Aspect,Near,Far);
	return Mat;
}
inline CD3DMatrix CD3DMatrix::FromPerspectiveFovRH(FLOAT Fov,FLOAT Aspect,FLOAT Near,FLOAT Far)
{
	CD3DMatrix Mat;	
	D3DXMatrixPerspectiveFovRH(&Mat,Fov,Aspect,Near,Far);
	return Mat;
}

inline CD3DMatrix CD3DMatrix::FromPerspectiveOffCenterLH(FLOAT Left,FLOAT Right,FLOAT Top,FLOAT Bottom,FLOAT Near,FLOAT Far)
{
	CD3DMatrix Mat;	
	D3DXMatrixPerspectiveOffCenterLH(&Mat,Left,Right,Top,Bottom,Near,Far);
	return Mat;
}
inline CD3DMatrix CD3DMatrix::FromPerspectiveOffCenterRH(FLOAT Left,FLOAT Right,FLOAT Top,FLOAT Bottom,FLOAT Near,FLOAT Far)
{
	CD3DMatrix Mat;	
	D3DXMatrixPerspectiveOffCenterRH(&Mat,Left,Right,Top,Bottom,Near,Far);
	return Mat;
}


//��������ͶӰ����
inline CD3DMatrix CD3DMatrix::FromOrthoLH(FLOAT Width,FLOAT Height,FLOAT Near,FLOAT Far)
{
	CD3DMatrix Mat;	
	D3DXMatrixOrthoLH(&Mat,Width,Height,Near,Far);
	return Mat;
}
inline CD3DMatrix CD3DMatrix::FromOrthoRH(FLOAT Width,FLOAT Height,FLOAT Near,FLOAT Far)
{
	CD3DMatrix Mat;	
	D3DXMatrixOrthoRH(&Mat,Width,Height,Near,Far);
	return Mat;
}

inline CD3DMatrix CD3DMatrix::FromOrthoOffCenterLH(FLOAT Left,FLOAT Right,FLOAT Top,FLOAT Bottom,FLOAT Near,FLOAT Far)
{
	CD3DMatrix Mat;	
	D3DXMatrixOrthoOffCenterLH(&Mat,Left,Right,Top,Bottom,Near,Far);
	return Mat;
}
inline CD3DMatrix CD3DMatrix::FromOrthoOffCenterRH(FLOAT Left,FLOAT Right,FLOAT Top,FLOAT Bottom,FLOAT Near,FLOAT Far)
{
	CD3DMatrix Mat;	
	D3DXMatrixOrthoOffCenterRH(&Mat,Left,Right,Top,Bottom,Near,Far);
	return Mat;
}


}

