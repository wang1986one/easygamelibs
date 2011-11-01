/****************************************************************************/
/*                                                                          */
/*      �ļ���:    D3DWOWWDLModelResource.h                                 */
/*      ��������:  2011��06��08��                                           */
/*      ����:      Sagasarate                                               */
/*                                                                          */
/*      �������Ȩ��Sagasarate(sagasarate@sina.com)����                     */
/*      ����Խ�����������κ���ҵ�ͷ���ҵ�����������                      */
/*      ���뱣���˰�Ȩ����                                                  */
/*                                                                          */
/****************************************************************************/
#pragma once

namespace D3DLib{

class CD3DWOWWDLModelResource :
	public CD3DObjectResource
{
protected:
	struct MODEL_VERTEXT
	{
		CD3DVector3			Pos;
	};

	DECLARE_CLASS_INFO(CD3DWOWWDLModelResource)
public:
	CD3DWOWWDLModelResource(void);
	CD3DWOWWDLModelResource(CD3DObjectResourceManager* pManager);
	~CD3DWOWWDLModelResource(void);

	virtual void Destory();

	virtual bool Reset();
	virtual bool Restore();

	bool LoadFromFile(LPCTSTR ModelFileName);

public:	
	virtual void PickResource(CUSOResourceManager * pResourceManager,UINT Param=0);
	virtual bool ToSmartStruct(CSmartStruct& Packet,CUSOResourceManager * pResourceManager,UINT Param=0);
	virtual bool FromSmartStruct(CSmartStruct& Packet,CUSOResourceManager * pResourceManager,UINT Param=0);
	virtual UINT GetSmartStructSize(UINT Param=0);
};

}