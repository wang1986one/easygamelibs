#pragma once

namespace D3DLib{

class CD3DDummy :
	public CD3DObject
{
protected:
	CD3DBoundingFrame *			m_pBoundingFrame;
	CD3DBoundingBox				m_BoundingBox;

	DECLARE_CLASS_INFO(CD3DDummy)
public:
	CD3DDummy(void);
	virtual ~CD3DDummy(void);

	virtual bool CanRender();

	virtual void ShowBoundingFrame(int Operator);	

	void SetBoundingBox(CD3DBoundingBox& BoundingBox);

	virtual bool ToSmartStruct(CSmartStruct& Packet,CUSOFile * pUSOFile,UINT Param=0);
};

inline bool CD3DDummy::CanRender()
{
	return false;
}

inline void CD3DDummy::SetBoundingBox(CD3DBoundingBox& BoundingBox)
{
	m_BoundingBox=BoundingBox;
}

}