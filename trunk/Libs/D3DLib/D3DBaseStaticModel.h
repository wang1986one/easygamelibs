#pragma once

namespace D3DLib{

class CD3DBaseStaticModel :
	public CD3DObject
{
protected:
protected:
	CD3DBoundingFrame *			m_pBoundingFrame;

	DECLARE_CLASS_INFO(CD3DBaseStaticModel)
public:
	CD3DBaseStaticModel(void);
	~CD3DBaseStaticModel(void);

	virtual void Destory();

	virtual bool Reset();
	virtual bool Restore();
	

	virtual void ShowBoundingFrame(int Operator);
	virtual void UpdateBoundingFrame();
};

}