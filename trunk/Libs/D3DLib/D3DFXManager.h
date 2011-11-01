/****************************************************************************/
/*                                                                          */
/*      �ļ���:    D3DFXManager.h                                           */
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

class CD3DFXManager :
	public CNameObject
{
protected:
	struct FX_INFO
	{
		CD3DFX *	pFX;
		bool		IsPrepareRelease;
		CEasyTimer	ReleaseTimer;
	};
	CD3DDevice *									m_pD3DDevice;
	LPD3DXEFFECTPOOL								m_pEffectPool;
	CD3DBuildInFXIncludeParser						m_BuildInFXIncludeParser;
	CThreadSafeStaticMap<CEasyString,FX_INFO>		m_FXStorage;
	UINT											m_DelayReleaseTime;

	DECLARE_CLASS_INFO_STATIC(CD3DFXManager)
public:
	CD3DFXManager(CD3DDevice * pD3DDevice,int StorageSize);
	virtual ~CD3DFXManager(void);

	bool Reset();
	bool Restore();		

	CD3DDevice * GetDevice();

	CD3DFX * GetFX(UINT ID);

	CD3DFX * GetFX(LPCTSTR FXName);

	bool AddFX(CD3DFX * pFX,LPCTSTR Name);

	bool DeleteFX(UINT ID);
	bool DeleteFX(LPCTSTR Name);

	CD3DFX * LoadFX(LPCTSTR FileName);
	CD3DFX * LoadFXFromMemory(LPCTSTR Name,const void * pData,int DataSize);

	int GetCount();
	LPVOID GetFirstPos();
	LPVOID GetLastPos();
	CD3DFX * GetNext(LPVOID& Pos);
	CD3DFX * GetPrev(LPVOID& Pos);

	LPD3DXEFFECTPOOL GetEffectPool();

	CD3DBuildInFXIncludeParser * GetBuildInFXIncludeParser();

	int Update(FLOAT Time);

	void SetDelayReleaseTime(UINT Time);
};

inline CD3DDevice * CD3DFXManager::GetDevice()
{
	return m_pD3DDevice;
}

inline CD3DFX * CD3DFXManager::GetFX(UINT ID)
{
	FX_INFO * pFXInfo=m_FXStorage.GetObject(ID);
	if(pFXInfo)
		return pFXInfo->pFX;
	else
		return NULL;
}

inline CD3DFX * CD3DFXManager::GetFX(LPCTSTR FXName)
{
	CEasyString Key=FXName;
	Key.MakeUpper();
	FX_INFO * pFXInfo=m_FXStorage.Find(Key);
	if(pFXInfo)
		return pFXInfo->pFX;
	else
		return NULL;
}

inline LPD3DXEFFECTPOOL CD3DFXManager::GetEffectPool()
{
	return m_pEffectPool;
}

inline CD3DBuildInFXIncludeParser * CD3DFXManager::GetBuildInFXIncludeParser()
{
	return &m_BuildInFXIncludeParser;
}

inline void CD3DFXManager::SetDelayReleaseTime(UINT Time)
{
	m_DelayReleaseTime=Time;
}

}