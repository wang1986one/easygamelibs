#pragma once

namespace D3DLib{

class CD3DBaseDynamicModel :
	public CD3DObject
{
protected:
	CD3DBoundingFrame *			m_pBoundingFrame;

	DECLARE_CLASS_INFO(CD3DBaseDynamicModel)
public:
	CD3DBaseDynamicModel(void);
	virtual ~CD3DBaseDynamicModel(void);

	virtual void Destory();

	virtual bool Reset();
	virtual bool Restore();

	virtual bool Play(bool IsLoop);	
	virtual bool Stop();
	virtual bool IsPlaying();

	virtual void ShowBoundingFrame(int Operator);
	virtual void UpdateBoundingFrame();
};

}