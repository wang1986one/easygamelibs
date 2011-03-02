/****************************************************************************/
/*                                                                          */
/*      �ļ���:    D3DBoundingFrame.h                                       */
/*      ��������:  2009��09��25��                                           */
/*      ����:      Sagasarate                                               */
/*                                                                          */
/*      �������Ȩ��Sagasarate(sagasarate@sina.com)����                     */
/*      ����Խ�����������κ���ҵ�ͷ���ҵ�����������                      */
/*      ���뱣���˰�Ȩ����                                                  */
/*                                                                          */
/****************************************************************************/
#pragma once


namespace D3DLib{

class CD3DBoundingFrame :
	public CD3DObject
{
protected:
	struct BF_VERTEX
	{
		CD3DVector3 Pos;
		D3DCOLOR	Color;
	};

	CD3DSubMesh * m_pSubMesh;
	CD3DBoundingBox m_BoundingBox;

	DECLARE_CLASS_INFO(CD3DBoundingFrame)
public:
	CD3DBoundingFrame(void);
	virtual ~CD3DBoundingFrame(void);

	virtual void Destory();
	virtual bool Reset();
	virtual bool Restore();

	bool CreateFromBBox(const CD3DBoundingBox& BBox,D3DCOLOR Color=0xFFFF0000);
	bool CreateFromLight(CD3DLight * pLight,D3DCOLOR Color=0xFFFF0000);

	void SetColor(D3DCOLOR Color);

	virtual int GetSubMeshCount();
	virtual CD3DSubMesh * GetSubMesh(UINT index);
	virtual CD3DBoundingBox * GetBoundingBox();
	virtual bool CanDoSubMeshViewCull();

	virtual bool RayIntersect(const CD3DVector3& Point,const CD3DVector3& Dir,CD3DVector3& IntersectPoint,FLOAT& Distance,bool TestOnly=true);
	virtual bool GetHeightByXZ(FLOAT x,FLOAT z,FLOAT& y);

	virtual bool ToSmartStruct(CSmartStruct& Packet,CUSOResourceManager * pResourceManager,UINT Param=0);
	virtual UINT GetSmartStructSize(UINT Param=0);
protected:
	void BuildLine(BF_VERTEX * pBuff,CD3DVector3 p1,CD3DVector3 p2,D3DCOLOR Color);
};



}