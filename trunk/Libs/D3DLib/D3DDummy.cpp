#include "StdAfx.h"

namespace D3DLib{

IMPLEMENT_CLASS_INFO(CD3DDummy,CD3DObject);

CD3DDummy::CD3DDummy(void):CD3DObject()
{
	SetVisible(false);
	m_pBoundingFrame=NULL;	
}

CD3DDummy::~CD3DDummy(void)
{
}

void CD3DDummy::ShowBoundingFrame(int Operator)
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
				m_pBoundingFrame->CreateFromBBox(m_BoundingBox);
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

bool CD3DDummy::ToSmartStruct(CSmartStruct& Packet,CUSOFile * pUSOFile,UINT Param)
{
	SAFE_RELEASE(m_pBoundingFrame);
	return CD3DObject::ToSmartStruct(Packet,pUSOFile,Param);
}

}