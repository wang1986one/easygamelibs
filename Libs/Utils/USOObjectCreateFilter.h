#pragma once

class CUSOObjectCreateFilter :
	public CNameObject
{
protected:

	DECLARE_CLASS_INFO(CUSOObjectCreateFilter)
public:
	CUSOObjectCreateFilter(void);
	~CUSOObjectCreateFilter(void);

	virtual bool OnObjectCreate(CNameObject * pObject);
	virtual CNameObject * FindObject(CLASS_INFO * pClassInfo,LPCTSTR ObjectName);
};
