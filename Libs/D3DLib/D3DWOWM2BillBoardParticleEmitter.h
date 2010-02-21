#pragma once

namespace D3DLib{

class CD3DWOWM2BillBoardParticleEmitter :
	public CD3DObject
{
protected:
	enum
	{
		MAX_PARTICLE_COUNT=10000,
		MIN_PARTICLE_COUNT=2,
	};
	struct PARTICLE_VERTEX
	{
		struct
		{
			CD3DVector3		Position;
			FLOAT			Life;

			CD3DVector3		Direction;
			FLOAT			LifeSpan;

			FLOAT			Speed;
			FLOAT			SpeedVariation;
			FLOAT			Gravity;		
			FLOAT			Decelerate;

			FLOAT			Slowdown;				
			FLOAT			ParticleRotation;
			FLOAT			ColorLifeMid;
			FLOAT			OpacityLifeMid;

			

			D3DCOLOR		ColorStart;

			D3DCOLOR		ColorMid;

			D3DCOLOR		ColorEnd;

			CD3DVector3		Translation;
			FLOAT			TexColRow;

			CD3DQuaternion	Rotation;
			

			CD3DVector3		Scaling;			
			FLOAT			SizeLifeMid;

			CD3DVector2		SizeStart;
			CD3DVector2		SizeMid;

			CD3DVector2		SizeEnd;
		} Info;
		CD3DVector2	Tex;
	};
	struct PARTICLE_RECT
	{
		PARTICLE_VERTEX		Vertex[4];
	};
	struct PARTICLE_INFO
	{		

		
		CD3DVector3			Scale;					
		
		CD3DWOWM2ModelResource::FAKE_ANIMATION_FRAME<CD3DVector3>	Color;			
		CD3DWOWM2ModelResource::FAKE_ANIMATION_FRAME<FLOAT>			Opacity;		
		CD3DWOWM2ModelResource::FAKE_ANIMATION_FRAME<CD3DVector2>	Size;			
		CD3DWOWM2ModelResource::FAKE_ANIMATION_FRAME<short>			Intensity;
	};
	CD3DWOWM2ModelResource *	m_pModelResource;
	CD3DSubMesh					m_SubMesh;
	UINT						m_EmitterIndex;
	UINT						m_MaxParticleCount;	
	PARTICLE_RECT *				m_pParticleVertexBuffer;
	WORD *						m_pParticleIndexBuffer;
	UINT						m_ParticleCount;
	FLOAT						m_StartTime;
	FLOAT						m_RecentUpdateTime;
	FLOAT						m_EmitterRate;	
	bool						m_EnbaleGravityTransform;
	bool						m_NoTrail;

	DECLARE_CLASS_INFO(CD3DWOWM2BillBoardParticleEmitter)

public:
	CD3DWOWM2BillBoardParticleEmitter(void);
	virtual ~CD3DWOWM2BillBoardParticleEmitter(void);

	virtual void Destory();

	virtual bool Reset();
	virtual bool Restore();

	virtual int GetSubMeshCount();
	virtual CD3DSubMesh * GetSubMesh(int index);

	virtual CD3DBoundingBox * GetBoundingBox();
	virtual CD3DBoundingSphere * GetBoundingSphere();

	virtual bool CloneFrom(CNameObject * pObject,UINT Param=0);

	virtual void PrepareRender(CD3DDevice * pDevice,CD3DSubMesh * pSubMesh,CD3DSubMeshMaterial * pMaterial,CD3DLight ** pLight,CD3DCamera * pCamera);


	virtual void Update(FLOAT Time);

	bool Init(CD3DWOWM2ModelResource * pModelResource,UINT EmitterIndex,UINT MaxParticleCount);
protected:
	void BuildParticle(CD3DWOWM2ModelResource::PARTICLE_EMITTER_INFO * pParticleEmitterInfo,CD3DWOWM2ModelResource::PARTICLE_PARAM * pParam);
};


}