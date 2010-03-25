/****************************************************************************/
/*                                                                          */
/*      文件名:    D3DScene.cpp                                             */
/*      创建日期:  2009年10月20日                                           */
/*      作者:      Sagasarate                                               */
/*                                                                          */
/*      本软件版权归Sagasarate(sagasarate@sina.com)所有                     */
/*      你可以将本软件用于任何商业和非商业软件开发，但                      */
/*      必须保留此版权声明                                                  */
/*                                                                          */
/****************************************************************************/
#include "StdAfx.h"

namespace D3DLib{

#define SMDL_EXTPROP_LEN	12

IMPLEMENT_FILE_CHANNEL_MANAGER(CD3DScene)

IMPLEMENT_CLASS_INFO(CD3DScene,CD3DObject);

CD3DScene::CD3DScene(void):CD3DObject()
{
	SetVisible(false);	
}

CD3DScene::~CD3DScene(void)
{
}

bool CD3DScene::LoadFromSN2(LPCTSTR FileName)
{
	IFileAccessor * pFile;

	if(GetDevice()==NULL)
		return false;

	pFile=CFileSystemManager::GetInstance()->CreateFileAccessor(m_FileChannel);
	
	if(pFile==NULL)
		return false;

	if(!pFile->Open(FileName,IFileAccessor::modeRead))
	{
		pFile->Release();
		return false;
	}

	m_CurNameTable=NULL;
	while(!pFile->IsEOF())
	{
		SN2_DATA_BLOCK_HEADER BlockHeader;
		int ReadSize=(int)pFile->Read(&BlockHeader,sizeof(BlockHeader));
		if(ReadSize!=sizeof(BlockHeader))
			break;
		ReadBlock(pFile,BlockHeader);
	}
	pFile->Release();
	SAFE_DELETE_ARRAY(m_CurNameTable);
	return true;
}

CD3DObject * CD3DScene::PickObject(CD3DVector3 Point,CD3DVector3 Dir)
{
	FLOAT MinDistance=MAX_HEIGHT;
	CD3DObject * pObject=NULL;

	for(UINT i=0;i<GetChildCount();i++)
	{
		CD3DVector3 IntersectPoint;
		FLOAT Distance;

		if((!GetChildByIndex(i)->IsVisible())||(GetChildByIndex(i)->IsCulled()))
			continue;

		if(GetChildByIndex(i)->RayIntersect(Point,Dir,IntersectPoint,Distance,false))
		{
			if(Distance<MinDistance)
			{
				MinDistance=Distance;
				pObject=GetChildByIndex(i);
			}
		}
	}
	return pObject;
}

bool CD3DScene::GetHeightByXZ(FLOAT x,FLOAT z,FLOAT& y)
{
	float h=-MAX_HEIGHT;
	for(UINT i=0;i<GetChildCount();i++)
	{
		if(GetChildByIndex(i)->IsKindOf(GET_CLASS_INFO(CD3DStaticModel)))
		{
			CD3DStaticModel * pModel=(CD3DStaticModel *)GetChildByIndex(i);
			if(pModel->GetProperty()&CD3DStaticModel::PV_IS_GROUND)
			{
				FLOAT Height;
				if(pModel->GetHeightByXZ(x,z,Height))
				{
					if(Height>h)
						h=Height;
				}
			}
		}
	}
	if(h>-MAX_HEIGHT)
		y=h;
	return true;
}

void CD3DScene::ReadBlock(IFileAccessor * pFile,SN2_DATA_BLOCK_HEADER& BlockHeader)
{
	switch(BlockHeader.Type)
	{
	case sck_Scene:
		break;
	case sck_Thing:
		{
			SN2_DATA_BLOCK_HEADER SubBlock;

			UINT ReadLen=0;
			while(ReadLen<BlockHeader.Size)
			{
				int ReadSize=(int)pFile->Read(&SubBlock,sizeof(SubBlock));
				ReadLen+=ReadSize;
				ReadLen+=SubBlock.Size;
				ReadBlock(pFile,SubBlock);
			}
			//if(m_CurObjectFileName.Find("圆盘.model")<0&&
			//	m_CurObjectFileName.Find("历史_ 木灯.model")<0)
			//	break;
			//if(strcmp(m_CurObjectFileName,"Root/Scene/m004/m004_植物4.model")==0)
			//	int err=0;
			CD3DStaticModel * pModel=new CD3DStaticModel();
			pModel->SetDevice(GetDevice());
			if(pModel->LoadFromSMDL(m_CurObjectFileName))
			{
				pModel->SetName(m_CurObjectFileName);
				pModel->SetID(m_CurObjectUID);
				pModel->SetLocalMatrix(m_CurObjectMatrix);
				pModel->SetParent(this);
				pModel->Update(0.0f);				
#ifdef _DEBUG
				{
					char Msg[1000];
					sprintf_s(Msg,1000,"装入场景模型<%s>\r\n",(LPCTSTR)m_CurObjectFileName);
					OutputDebugString(Msg);
				}
#endif
				
				
			}
			else
			{
				pModel->Release();
#ifdef _DEBUG
				{
					char Msg[1000];
					sprintf_s(Msg,1000,"装入场景模型失败<%s>\r\n",(LPCTSTR)m_CurObjectFileName);
					OutputDebugString(Msg);
				}
#endif
			}
		}
		break;
	case sck_extprop:
		{
			BYTE ExtProp[SMDL_EXTPROP_LEN];
			pFile->Read(ExtProp,SMDL_EXTPROP_LEN);
		}
		break;
	case sck_FileName:
		{
			if(m_CurNameTable)
			{
				int NameOffset;
				pFile->Read(&NameOffset,sizeof(int));
				strncpy_0(m_CurObjectFileName,MAX_PATH,m_CurNameTable+NameOffset,MAX_PATH);
			}
			else
			{
				pFile->Read(m_CurObjectFileName,BlockHeader.Size);;
				m_CurObjectFileName[BlockHeader.Size-1]=0;
			}
		}
		break;
	case sck_Matrix:
		{
			pFile->Read(&m_CurObjectMatrix,BlockHeader.Size);
		}
		break;
	
	case sck_NameTable:
		{
			m_CurNameTable=new char[BlockHeader.Size];
			pFile->Read(m_CurNameTable,BlockHeader.Size);
		}
		break;
	case sck_UID:
		{
			pFile->Read(&m_CurObjectUID,sizeof(DWORD));
		}
		break;
	case sck_Rotat:
	case sck_Data:
	case sck_EnvTextureInfo:
	case sck_ObjCount:
	case sck_LodItem:
	case sck_ZoneSizeInfo:
	case sck_EnvTextureInfo2:
	case sck_SizeInfo:
	default:
		pFile->Seek(BlockHeader.Size,IFileAccessor::seekCurrent);
		break;
	}
}



}