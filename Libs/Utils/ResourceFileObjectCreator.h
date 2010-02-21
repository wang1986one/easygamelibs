#pragma once


class CResourceFileObjectCreator :
	public IFileObjectCreator
{
protected:
	DECLARE_CLASS_INFO_STATIC(CResourceFileObjectCreator);
public:
	CResourceFileObjectCreator(void);
	virtual ~CResourceFileObjectCreator(void);

	virtual IFileAccessor * CreateFileAccessor();
};
