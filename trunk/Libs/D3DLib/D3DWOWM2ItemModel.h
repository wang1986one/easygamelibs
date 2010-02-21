#pragma once

namespace D3DLib{

#pragma comment(linker,"/include:?m_CD3DWOWM2ItemModelClassInfoRegister@CD3DWOWM2ItemModel@D3DLib@@1VCClassInfoRegister@@A")

class CD3DWOWM2ItemModel :
	public CD3DWOWM2Model
{
public:
	enum ITEM_HAND_TYPE
	{
		IHT_LEFT,
		IHT_RIGHT,
	};
protected:
	//struct STORAGE_STRUCT:CD3DWOWM2Model::STORAGE_STRUCT
	//{
	//	UINT			ItemID;
	//	int				ItemHandType;
	//};

	enum SST_MEMBER_ID
	{
		SST_D3DWMIM_ITEM_ID=SST_D3DWMM_MAX,
		SST_D3DWMIM_ITEM_HAND_TYPE,
		SST_D3DWMIM_MAX=SST_D3DWMM_MAX+50,
	};

	UINT			m_ItemID;
	int				m_ItemHandType;	

	DECLARE_CLASS_INFO(CD3DWOWM2ItemModel)
public:
	CD3DWOWM2ItemModel(void);
	~CD3DWOWM2ItemModel(void);

	void SetItemID(UINT ItemID);
	UINT GetItemID();

	void SetItemHandType(int Type);
	int GetItemHandType();

	bool BuildModel();

	virtual bool CloneFrom(CNameObject * pObject,UINT Param=0);

	virtual bool ToSmartStruct(CSmartStruct& Packet,CUSOFile * pUSOFile,UINT Param=0);
	virtual bool FromSmartStruct(CSmartStruct& Packet,CUSOFile * pUSOFile,UINT Param=0);
	virtual UINT GetSmartStructSize(UINT Param=0);

	virtual void Update(FLOAT Time);

protected:
	//virtual CNameObject::STORAGE_STRUCT * USOCreateHead(UINT Param=0);
	//virtual int USOWriteHead(CNameObject::STORAGE_STRUCT * pHead,CUSOFile * pUSOFile,UINT Param=0);		
	//virtual int USOReadHead(CNameObject::STORAGE_STRUCT * pHead,CUSOFile * pUSOFile,UINT Param=0);
	
};


inline void CD3DWOWM2ItemModel::SetItemID(UINT ItemID)
{
	m_ItemID=ItemID;
}

inline UINT CD3DWOWM2ItemModel::GetItemID()
{
	return m_ItemID;
}

inline void CD3DWOWM2ItemModel::SetItemHandType(int Type)
{
	m_ItemHandType=Type;
}
inline int CD3DWOWM2ItemModel::GetItemHandType()
{
	return m_ItemHandType;
}

}