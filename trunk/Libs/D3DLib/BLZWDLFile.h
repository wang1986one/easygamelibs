#pragma once

namespace D3DLib{

class CBLZWDLFile :
	public CNameObject
{
protected:

	DECLARE_FILE_CHANNEL_MANAGER
public:
	CBLZWDLFile(void);
	~CBLZWDLFile(void);

	bool Load(LPCTSTR FileName);
	bool Load(IFileAccessor * pFile);
};

}