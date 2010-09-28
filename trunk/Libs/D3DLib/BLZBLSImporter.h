/****************************************************************************/
/*                                                                          */
/*      �ļ���:    BLZBLSImporter.h                                         */
/*      ��������:  2010��07��09��                                           */
/*      ����:      Sagasarate                                               */
/*                                                                          */
/*      �������Ȩ��Sagasarate(sagasarate@sina.com)����                     */
/*      ����Խ�����������κ���ҵ�ͷ���ҵ�����������                      */
/*      ���뱣���˰�Ȩ����                                                  */
/*                                                                          */
/****************************************************************************/

#pragma once

namespace D3DLib{

class CBLZBLSImporter :
	public CNameObject
{
protected:
	BYTE *								m_pData;
	UINT								m_DataSize;
	bool								m_IsSelfRelease;
	int									m_ShaderType;
	CEasyArray<BLZ_BLS_DATA_BLOCK *>	m_Blocks;

	DECLARE_FILE_CHANNEL_MANAGER

public:
	enum SHADER_TYPE
	{
		SHADER_TYPE_NONE,
		SHADER_TYPE_VS,
		SHADER_TYPE_PS,
	};
	CBLZBLSImporter(void);
	~CBLZBLSImporter(void);

	bool Load(LPCTSTR FileName);
	bool Load(IFileAccessor * pFile);
	bool Load(BYTE * pData,UINT DataSize,bool IsSelfRelease);
	void Close();

	UINT GetDataBlockCount();
	BLZ_BLS_DATA_BLOCK * GetDataBlock(UINT Index);
	CEasyString GetD3DXDisassembledShader(UINT Index);
};

}