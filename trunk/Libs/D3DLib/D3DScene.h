/****************************************************************************/
/*                                                                          */
/*      �ļ���:    D3DScene.h                                               */
/*      ��������:  2009��09��11��                                           */
/*      ����:      Sagasarate                                               */
/*                                                                          */
/*      �������Ȩ��Sagasarate(sagasarate@sina.com)����                     */
/*      ����Խ�����������κ���ҵ�ͷ���ҵ�����������                      */
/*      ���뱣���˰�Ȩ����                                                  */
/*                                                                          */
/****************************************************************************/
#pragma once

namespace D3DLib{

#pragma comment(linker,"/include:?m_CD3DSceneClassInfoRegister@CD3DScene@D3DLib@@1VCClassInfoRegister@@A")

class CD3DScene :
	public CD3DObject
{
protected:
	struct SN2_DATA_BLOCK_HEADER
	{
		DWORD Type;
		DWORD Size;
	};
	enum SN2_BLOCK_TYPE
	{
		sck_Scene,
			sck_Thing,
				sck_extprop,
				sck_FileName,
				sck_Matrix,
				sck_Data,
			sck_NameTable,		
				sck_UID,
				sck_Rotat,
		sck_EnvTextureInfo,
		sck_ObjCount,
		sck_LodItem,
		sck_ZoneSizeInfo,
		sck_EnvTextureInfo2,
		sck_SizeInfo,
	};

	char		m_CurObjectFileName[MAX_PATH];
	CD3DMatrix	m_CurObjectMatrix;
	DWORD		m_CurObjectUID;
	char *		m_CurNameTable;

	DECLARE_FILE_CHANNEL_MANAGER

	DECLARE_CLASS_INFO(CD3DScene)
public:
	CD3DScene(void);
	virtual ~CD3DScene(void);

	bool LoadFromSN2(LPCTSTR FileName);

	CD3DObject * PickObject(CD3DVector3 Point,CD3DVector3 Dir);

	bool GetHeightByXZ(FLOAT x,FLOAT z,FLOAT& y);
	
protected:
	void ReadBlock(IFileAccessor * pFile,SN2_DATA_BLOCK_HEADER& BlockHeader);
};

}