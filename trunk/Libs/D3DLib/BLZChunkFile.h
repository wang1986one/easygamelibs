#pragma once

namespace D3DLib{

class CBLZChunkFile :
	public CNameObject
{
public:
	typedef CEasyArray<BLZ_CHUNK_HEADER *>	CChunkList;
protected:
	BYTE *										m_pData;
	UINT										m_DataSize;	
	bool										m_IsSelfRelease;
	CEasyMap<UINT,CChunkList>					m_Chunks;
public:
	CBLZChunkFile(void);
	~CBLZChunkFile(void);

	BOOL Load(IFileAccessor * pFile);
	BOOL Load(BYTE * pData,UINT DataSize);
	void Close();	

	CChunkList * GetChunks(UINT ChunkID);
	BLZ_CHUNK_HEADER * GetFirstChunk(UINT ChunkID);
	BLZ_CHUNK_HEADER * GetChunkByOffset(UINT ChunkID,UINT Offset);


};




}