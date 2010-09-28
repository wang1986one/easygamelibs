#pragma once

namespace D3DLib{

class CBLZWDTFile :
	public CNameObject
{
public:
	struct WMO_OBJECT_INFO
	{
		UINT 			ID;
		CEasyString		Name;
		CD3DVector3 	Position;		
		CD3DQuaternion 	Orientation;			
		UINT16 			Flags;
		UINT16 			DoodadSet;
	};

protected:
	MAINMapAreaInfo											m_MapAreaInfo[64][64];
	CEasyArray<WMO_OBJECT_INFO>								m_MapWMOInfo;
	bool													m_IsBigAlphaMask;

	DECLARE_FILE_CHANNEL_MANAGER
public:
	CBLZWDTFile(void);
	~CBLZWDTFile(void);

	bool Load(LPCTSTR FileName);
	bool Load(IFileAccessor * pFile);

	bool IsAreaExist(UINT x,UINT y);
	UINT GetWMOInfoCount();
	WMO_OBJECT_INFO * GetWMOInfo(UINT Index);
	bool IsBigAlphaMask();
};

inline bool CBLZWDTFile::IsAreaExist(UINT x,UINT y)
{
	if(x<64&&y<64)
	{
		return (m_MapAreaInfo[y][x].Flag&1)!=0;
	}
	return false;
}
inline UINT CBLZWDTFile::GetWMOInfoCount()
{
	return m_MapWMOInfo.GetCount();
}
inline CBLZWDTFile::WMO_OBJECT_INFO * CBLZWDTFile::GetWMOInfo(UINT Index)
{
	return m_MapWMOInfo.GetObject(Index);
}
inline bool CBLZWDTFile::IsBigAlphaMask()
{
	return m_IsBigAlphaMask;
}

}