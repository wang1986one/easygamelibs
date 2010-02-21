#include "StdAfx.h"

namespace D3DLib{

IMPLEMENT_CLASS_INFO(CD3DBaseDynamicModel,CD3DObject);

CD3DBaseDynamicModel::CD3DBaseDynamicModel(void):CD3DObject()
{
	m_pBoundingFrame=NULL;
}

CD3DBaseDynamicModel::~CD3DBaseDynamicModel(void)
{
	Destory();
}

void CD3DBaseDynamicModel::Destory()
{
	SAFE_RELEASE(m_pBoundingFrame);
	CD3DObject::Destory();
}

bool CD3DBaseDynamicModel::Reset()
{	
	return m_pBoundingFrame->Reset()&&CD3DObject::Reset();
}

bool CD3DBaseDynamicModel::Restore()
{
	return m_pBoundingFrame->Restore()&&CD3DObject::Reset();
}

bool CD3DBaseDynamicModel::Play(bool IsLoop)
{
	return false;
}
bool CD3DBaseDynamicModel::Stop()
{
	return false;
}
bool CD3DBaseDynamicModel::IsPlaying()
{
	return false;
}

void CD3DBaseDynamicModel::ShowBoundingFrame(int Operator)
{
	switch(Operator)
	{
	case DBFO_HIDE:
		{
			if(m_pBoundingFrame)
				m_pBoundingFrame->SetVisible(false);
		}
		break;
	case DBFO_SHOW:
		{
			if(m_pBoundingFrame)
				m_pBoundingFrame->SetVisible(true);
			else
			{
				if(GetRender()==NULL)
					return;
				m_pBoundingFrame=new CD3DBoundingFrame();
				m_pBoundingFrame->SetRender(GetRender());
				m_pBoundingFrame->CreateFromBBox(*GetBoundingBox());
				m_pBoundingFrame->SetParent(this);
				GetRender()->AddObject(m_pBoundingFrame);
			}
		}
		break;
	case DBFO_RELEASE:
		SAFE_RELEASE(m_pBoundingFrame);
		break;
	}
}

void CD3DBaseDynamicModel::UpdateBoundingFrame()
{
	if(m_pBoundingFrame)
	{
		m_pBoundingFrame->CreateFromBBox(*GetBoundingBox());
	}
}

}