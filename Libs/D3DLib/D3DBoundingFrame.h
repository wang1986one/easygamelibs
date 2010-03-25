/****************************************************************************/
/*                                                                          */
/*      文件名:    D3DBoundingFrame.h                                       */
/*      创建日期:  2009年09月25日                                           */
/*      作者:      Sagasarate                                               */
/*                                                                          */
/*      本软件版权归Sagasarate(sagasarate@sina.com)所有                     */
/*      你可以将本软件用于任何商业和非商业软件开发，但                      */
/*      必须保留此版权声明                                                  */
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

	bool CreateFromBBox(const CD3DBoundingBox& BBox);

	virtual int GetSubMeshCount();
	virtual CD3DSubMesh * GetSubMesh(int index);
	virtual CD3DBoundingBox * GetBoundingBox();

	virtual bool RayIntersect(const CD3DVector3& Point,const CD3DVector3& Dir,CD3DVector3& IntersectPoint,FLOAT& Distance,bool TestOnly=true);
	virtual bool GetHeightByXZ(FLOAT x,FLOAT z,FLOAT& y);

	virtual bool ToSmartStruct(CSmartStruct& Packet,CUSOFile * pUSOFile,UINT Param=0);
protected:
	void BuildRect(BF_VERTEX * pBuff,CD3DVector3 p1,CD3DVector3 p2,CD3DVector3 p3,CD3DVector3 p4);
	void BuildLine(BF_VERTEX * pBuff,CD3DVector3 p1,CD3DVector3 p2);
};



}