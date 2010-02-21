#pragma once
#include "ifileobjectcreator.h"


class CStandardFileObjectCreator :
	public IFileObjectCreator
{
protected:
	DECLARE_CLASS_INFO_STATIC(CStandardFileObjectCreator);
public:
	CStandardFileObjectCreator(void);
	virtual ~CStandardFileObjectCreator(void);

	virtual IFileAccessor * CreateFileAccessor();
	
};
