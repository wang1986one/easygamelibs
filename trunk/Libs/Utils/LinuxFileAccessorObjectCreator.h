#pragma once

class CLinuxFileAccessorObjectCreator :
	public IFileObjectCreator
{
protected:
	DECLARE_CLASS_INFO_STATIC(CLinuxFileAccessorObjectCreator);
public:
	CLinuxFileAccessorObjectCreator(void);
	~CLinuxFileAccessorObjectCreator(void);

	virtual IFileAccessor * CreateFileAccessor();
};
