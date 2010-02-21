#pragma once
#include "ifileobjectcreator.h"

class CWinFileAccessorObjectCreator :
	public IFileObjectCreator
{
protected:
	DECLARE_CLASS_INFO_STATIC(CWinFileAccessorObjectCreator);
public:
	CWinFileAccessorObjectCreator(void);
	virtual ~CWinFileAccessorObjectCreator(void);

	virtual IFileAccessor * CreateFileAccessor();
};
