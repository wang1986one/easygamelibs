#include "StdAfx.h"

namespace D3DLib{

IMPLEMENT_CLASS_INFO(CD3DBaseStaticModel,CD3DObject);

CD3DBaseStaticModel::CD3DBaseStaticModel(void)
{
	m_pBoundingFrame=NULL;
}

CD3DBaseStaticModel::~CD3DBaseStaticModel(void)
{
	Destory();
}


void CD3DBaseStaticModel::Destory()
{
	SAFE_RELEASE(m_pBoundingFrame);
	CD3DObject::Destory();
}

bool CD3DBaseStaticModel::Reset()
{	
	return m_pBoundingFrame->Reset()&&CD3DObject::Reset();
}

bool CD3DBaseStaticModel::Restore()
{
	return m_pBoundingFrame->Restore()&&CD3DObject::Reset();
}

void CD3DBaseStaticModel::ShowBoundingFrame(int Operator)
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

void CD3DBaseStaticModel::UpdateBoundingFrame()
{
	if(m_pBoundingFrame)
	{
		m_pBoundingFrame->CreateFromBBox(*GetBoundingBox());
	}
}

}