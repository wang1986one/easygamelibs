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
	CD3DBoundingBox				m_BoundingBox;

	DECLARE_CLASS_INFO(CD3DScene)
public:
	CD3DScene(void);
	virtual ~CD3DScene(void);

	virtual bool CanRender();

	virtual CD3DBoundingBox * GetBoundingBox();

	bool GetHeightByXZ(const CD3DVector3 Pos,FLOAT MinHeight,FLOAT MaxHeight,FLOAT& Height,FLOAT& WaterHeight);
	

	virtual bool AddChild(CTreeObject* pChild,bool CheckExist=false);

};

inline bool CD3DScene::CanRender()
{
	return false;
}

inline CD3DBoundingBox * CD3DScene::GetBoundingBox()
{
	return &m_BoundingBox;
}

}