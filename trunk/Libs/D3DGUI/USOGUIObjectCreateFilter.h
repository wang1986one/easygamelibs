#pragma once

namespace D3DGUI{

class CUSOGUIObjectCreateFilter :
	public CUSOObjectCreateFilter
{
protected:
	CD3DGUI *		m_pGUI;

	DECLARE_CLASS_INFO(CUSOGUIObjectCreateFilter)
public:
	CUSOGUIObjectCreateFilter(void);
	CUSOGUIObjectCreateFilter(CD3DGUI * pGUI);
	~CUSOGUIObjectCreateFilter(void);

	void SetGUIManager(CD3DGUI * pGUI);

	virtual bool OnObjectCreate(CNameObject * pObject);
	virtual CNameObject * FindObject(CLASS_INFO * pClassInfo,LPCTSTR ObjectName);
};

}