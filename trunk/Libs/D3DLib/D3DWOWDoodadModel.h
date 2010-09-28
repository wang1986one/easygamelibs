/****************************************************************************/
/*                                                                          */
/*      �ļ���:    D3DWOWDoodadModel.h		                                */
/*      ��������:  2010��08��05��                                           */
/*      ����:      Sagasarate                                               */
/*                                                                          */
/*      �������Ȩ��Sagasarate(sagasarate@sina.com)����                     */
/*      ����Խ�����������κ���ҵ�ͷ���ҵ�����������                      */
/*      ���뱣���˰�Ȩ����                                                  */
/*                                                                          */
/****************************************************************************/
#pragma once

namespace D3DLib{

#pragma comment(linker,"/include:?m_CD3DWOWDoodadModelClassInfoRegister@CD3DWOWDoodadModel@D3DLib@@1VCClassInfoRegister@@A")

class CD3DWOWDoodadModel :
	public CD3DWOWM2Model
{
protected:
	WORD		m_GroupIndex;
	DECLARE_CLASS_INFO(CD3DWOWDoodadModel)
public:
	CD3DWOWDoodadModel(void);
	~CD3DWOWDoodadModel(void);

	virtual void Destory();

	void SetGroupIndex(WORD Index);
	WORD GetGroupIndex();
};

inline void CD3DWOWDoodadModel::SetGroupIndex(WORD Index)
{
	m_GroupIndex=Index;
}

inline WORD CD3DWOWDoodadModel::GetGroupIndex()
{
	return m_GroupIndex;
}
}