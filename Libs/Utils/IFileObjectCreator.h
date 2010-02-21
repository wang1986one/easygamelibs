#pragma once
#include "IFileAccessor.h"

class IFileObjectCreator:public CNameObject
{
protected:
	DECLARE_CLASS_INFO_STATIC(IFileObjectCreator)
public:
	IFileObjectCreator();
	virtual ~IFileObjectCreator(void);

	virtual IFileAccessor * CreateFileAccessor()=0;
	
};
