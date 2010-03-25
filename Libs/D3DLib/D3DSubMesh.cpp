/****************************************************************************/
/*                                                                          */
/*      文件名:    D3DSubMesh.cpp                                           */
/*      创建日期:  2009年09月25日                                           */
/*      作者:      Sagasarate                                               */
/*                                                                          */
/*      本软件版权归Sagasarate(sagasarate@sina.com)所有                     */
/*      你可以将本软件用于任何商业和非商业软件开发，但                      */
/*      必须保留此版权声明                                                  */
/*                                                                          */
/****************************************************************************/
#include "StdAfx.h"
namespace D3DLib{

IMPLEMENT_CLASS_INFO(CD3DSubMesh,CNameObject);
CD3DSubMesh::CD3DSubMesh(void)
{
	ZeroMemory(&m_VertexFormat,sizeof(m_VertexFormat));
	m_PrimitiveType=D3DPT_TRIANGLELIST;
	m_PrimitiveCount=0;
	m_pVertexs=NULL;
	m_pDXVertexBuffer=NULL;
	m_IsVertexsSelfDelete=false;
	m_IsDXVertexBufferSelfRelease=false;
	m_VertexCount=0;
	m_StartVertex=0;
	m_pIndexs=NULL;
	m_pDXIndexBuffer=NULL;
	m_IsIndexsSelfDelete=false;
	m_IsDXIndexBufferSelfRelease=false;
	m_IndexCount=0;	
	m_StartIndex=0;
	m_Property=0;
	m_Flag=0;
	SetVisible(true);

}

CD3DSubMesh::~CD3DSubMesh(void)
{
	Destory();
}


void CD3DSubMesh::Destory()
{
	if(m_IsVertexsSelfDelete)
	{
		SAFE_DELETE_ARRAY(m_pVertexs);
	}
	if(m_IsDXVertexBufferSelfRelease)
	{
		SAFE_RELEASE(m_pDXVertexBuffer);
	}
	if(m_IsIndexsSelfDelete)
	{
		SAFE_DELETE_ARRAY(m_pIndexs);
	}
	if(m_IsDXIndexBufferSelfRelease)
	{
		SAFE_RELEASE(m_pDXIndexBuffer);
	}
	SAFE_RELEASE(m_VertexFormat.pVertexDeclaration);
	m_Material.Destory();
	CNameObject::Destory();
}

bool CD3DSubMesh::Reset()
{
	return m_Material.Reset();
}
bool CD3DSubMesh::Restore()
{
	return m_Material.Restore();
}

void CD3DSubMesh::CreateBounding()
{
	if(m_VertexCount)
	{
		if(m_pDXVertexBuffer)
		{				
			BYTE *pBuff;
			m_pDXVertexBuffer->Lock(0,0,(LPVOID *)&pBuff,0);
			m_BoundingBox.ComputeFromVertex(
				pBuff+m_StartVertex*m_VertexFormat.VertexSize,
				m_VertexCount,
				m_VertexFormat.VertexSize);
			m_BoundingSphere.ComputeFromVertex(
				pBuff+m_StartVertex*m_VertexFormat.VertexSize,
				m_VertexCount,
				m_VertexFormat.VertexSize);			
			m_pDXVertexBuffer->Unlock();
		}
		else
		{
			m_BoundingBox.ComputeFromVertex(
				m_pVertexs+m_StartVertex*m_VertexFormat.VertexSize,
				m_VertexCount,
				m_VertexFormat.VertexSize);
			m_BoundingSphere.ComputeFromVertex(
				m_pVertexs+m_StartVertex*m_VertexFormat.VertexSize,
				m_VertexCount,
				m_VertexFormat.VertexSize);			
		}
	}
	
}

bool CD3DSubMesh::RayIntersect(const CD3DMatrix& WorldMatrix,const CD3DVector3& Point,const CD3DVector3& Dir,CD3DVector3& IntersectPoint,FLOAT& Distance,bool TestOnly)
{
	BYTE * pVertexBuff=NULL;
	BYTE * pIndexBuff=NULL;
	bool IsIntersect=false;

	
	CD3DVector3 InterPoint;

	CD3DBoundingBox BBox=m_BoundingBox*WorldMatrix;

	if(!BBox.RayIntersect(Point,Dir,IntersectPoint,Distance))
		return IsIntersect;

	Distance=3.4E+38f;

	if(m_pDXVertexBuffer)
	{
		m_pDXVertexBuffer->Lock(0,0,(LPVOID *)&pVertexBuff,D3DLOCK_READONLY);
		if(m_pDXIndexBuffer)
			m_pDXIndexBuffer->Lock(0,0,(LPVOID *)&pIndexBuff,D3DLOCK_READONLY);
	}
	else
	{				
		pVertexBuff=m_pVertexs;
		pIndexBuff=m_pIndexs;
	}
	for(UINT t=0;t<m_PrimitiveCount;t++)
	{
		CD3DVector3 p1;
		CD3DVector3 p2;
		CD3DVector3 p3;

		if(pIndexBuff)
		{
			if(m_VertexFormat.IndexSize==2)
			{
				WORD * IndexList=(WORD *)pIndexBuff;
				p1=*(CD3DVector3 *)(pVertexBuff+IndexList[t*3]*m_VertexFormat.VertexSize);
				p2=*(CD3DVector3 *)(pVertexBuff+IndexList[t*3+1]*m_VertexFormat.VertexSize);
				p3=*(CD3DVector3 *)(pVertexBuff+IndexList[t*3+2]*m_VertexFormat.VertexSize);
			}
			else
			{
				DWORD * IndexList=(DWORD *)pIndexBuff;
				p1=*(CD3DVector3 *)(pVertexBuff+IndexList[t*3]*m_VertexFormat.VertexSize);
				p2=*(CD3DVector3 *)(pVertexBuff+IndexList[t*3+1]*m_VertexFormat.VertexSize);
				p3=*(CD3DVector3 *)(pVertexBuff+IndexList[t*3+2]*m_VertexFormat.VertexSize);
			}
		}
		else
		{
			p1=*(CD3DVector3 *)(pVertexBuff+(t*3)*m_VertexFormat.VertexSize);
			p2=*(CD3DVector3 *)(pVertexBuff+(t*3+1)*m_VertexFormat.VertexSize);
			p3=*(CD3DVector3 *)(pVertexBuff+(t*3+2)*m_VertexFormat.VertexSize);
		}
		
		

		p1*=WorldMatrix;
		p2*=WorldMatrix;
		p3*=WorldMatrix;

		FLOAT U,V;
		FLOAT Dis;

		if(D3DXIntersectTri(&p1,&p2,&p3,&Point,&Dir,&U,&V,&Dis))
		{			
			InterPoint=p1+(p2-p1)*U+(p3-p1)*V;
			IsIntersect=true;
			//CD3DVector3 DistanceVector=InterPoint-Point;
			//FLOAT Dis=DistanceVector.Length();
			if(Dis<Distance)
			{
				Distance=Dis;
				IntersectPoint=InterPoint;
			}
			if(TestOnly)
			{
				break;
			}
		}
	}
	if(m_pDXVertexBuffer)
	{
		m_pDXVertexBuffer->Unlock();
		if(m_pDXIndexBuffer)
			m_pDXIndexBuffer->Unlock();
	}
	return IsIntersect;
}

bool CD3DSubMesh::DeclareVertexFormat(CD3DDevice * pDevice,D3DVERTEXELEMENT9* pVertexElements,WORD VertexSize,WORD IndexSize)
{
	SAFE_RELEASE(m_VertexFormat.pVertexDeclaration);
	if(pDevice->GetD3DDevice()->CreateVertexDeclaration(pVertexElements,&(m_VertexFormat.pVertexDeclaration))==D3D_OK)
	{
		m_VertexFormat.VertexSize=VertexSize;
		m_VertexFormat.IndexSize=IndexSize;
		return true;
	}
	return false;
}

void CD3DSubMesh::AllocVertexs()
{
	m_pVertexs=new BYTE[m_VertexFormat.VertexSize*m_VertexCount];	
	m_IsVertexsSelfDelete=true;
}
bool CD3DSubMesh::AllocDXVertexBuffer(CD3DDevice * pDevice,DWORD Usage,D3DPOOL Pool)
{
	if(pDevice->GetD3DDevice()->CreateVertexBuffer( 
		m_VertexFormat.VertexSize*m_VertexCount,
		Usage, m_VertexFormat.FVF,
		Pool, &m_pDXVertexBuffer, NULL )==D3D_OK)
	{
		m_IsDXVertexBufferSelfRelease=true;
		return true;
	}
	return false;
	
}

void CD3DSubMesh::AllocIndexs()
{
	m_pIndexs=new BYTE[m_VertexFormat.IndexSize*m_IndexCount];	
	m_IsIndexsSelfDelete=true;
}

bool CD3DSubMesh::AllocDXIndexBuffer(CD3DDevice * pDevice,DWORD Usage,D3DPOOL Pool)
{
	D3DFORMAT IndexType=D3DFMT_INDEX16;
	if(m_VertexFormat.IndexSize==sizeof(DWORD))
		IndexType=D3DFMT_INDEX32;
	if(pDevice->GetD3DDevice()->CreateIndexBuffer(
		m_VertexFormat.IndexSize*m_IndexCount,
		Usage,			
		IndexType,
		Pool,
		&m_pDXIndexBuffer,
		NULL)==D3D_OK)
	{
		m_IsDXIndexBufferSelfRelease=true;
		return true;
	}
	return false;
}

bool CD3DSubMesh::SortByName(CD3DSubMesh * pSubMesh1,CD3DSubMesh * pSubMesh2)
{
	if(strcmp(pSubMesh1->GetName(),pSubMesh2->GetName())<0)
		return true;
	else
		return false;
}

bool CD3DSubMesh::SortByRender(CD3DSubMesh * pSubMesh1,CD3DSubMesh * pSubMesh2)
{
	BYTE RenderOrder1=pSubMesh1->GetProperty()>>56;
	BYTE RenderOrder2=pSubMesh2->GetProperty()>>56;

	return RenderOrder1<=RenderOrder2;
}

//bool CD3DSubMesh::ToUSOFile(CUSOFile * pUSOFile,UINT Param)
//{
//	if(pUSOFile==NULL)
//		return false;	
//
//	IFileAccessor * pFile=pUSOFile->GetFile();
//	if(pFile==NULL)
//		return false;
//
//	STORAGE_STRUCT Data;
//
//	ZeroMemory(&Data,sizeof(Data));
//
//	strncpy_0(Data.ObjectHead.Type,USO_FILE_MAX_TYPE_LEN,GetClassInfo().ClassName,USO_FILE_MAX_TYPE_LEN);
//	strncpy_0(Data.ObjectHead.Name,USO_FILE_MAX_OBJECT_NAME,GetName(),USO_FILE_MAX_OBJECT_NAME);
//	Data.ObjectHead.Size=sizeof(STORAGE_STRUCT)+m_VertexCount*m_VertexFormat.VertexSize+m_IndexCount*m_VertexFormat.IndexSize;
//	Data.ObjectHead.StorageID=0;
//
//	Data.VertexFormat=m_VertexFormat;
//	Data.PrimitiveType=m_PrimitiveType;
//	Data.PrimitiveCount=m_PrimitiveCount;
//	Data.VertexCount=m_VertexCount;
//	Data.IndexCount=m_IndexCount;
//	Data.BoundingBox=m_BoundingBox;
//	Data.BoundingSphere=m_BoundingSphere;
//	Data.Property=m_Property;	
//	
//	if(pFile->Write(&Data,sizeof(STORAGE_STRUCT))<sizeof(STORAGE_STRUCT))		
//		return false;
//
//	if(m_VertexCount&&m_pDXVertexBuffer)
//	{
//		BYTE *pBuff;
//		m_pDXVertexBuffer->Lock(0,0,(LPVOID *)&pBuff,0);
//		UINT64 DataSize=m_VertexCount*m_VertexFormat.VertexSize;
//		UINT64 WriteSize=pFile->Write(pBuff+m_StartVertex*m_VertexFormat.VertexSize,DataSize);			
//		m_pDXVertexBuffer->Unlock();
//		if(WriteSize<DataSize)
//			return false;
//	}
//	if(m_IndexCount&&m_pDXIndexBuffer)
//	{
//		BYTE *pBuff;
//		m_pDXIndexBuffer->Lock(0,0,(LPVOID *)&pBuff,0);
//		UINT64 DataSize=m_IndexCount*m_VertexFormat.IndexSize;
//		UINT64 WriteSize=pFile->Write(pBuff+m_StartIndex*m_VertexFormat.IndexSize,DataSize);
//		m_pDXIndexBuffer->Unlock();
//		if(WriteSize<DataSize)
//			return false;
//	}
//
//	return m_Material.ToUSOFile(pUSOFile,Param);
//}
//
//bool CD3DSubMesh::FromUSOFile(CUSOFile * pUSOFile,UINT Param)
//{
//	if(pUSOFile==NULL)
//		return false;
//
//	CD3DDevice * pDevice=(CD3DDevice *)Param;
//	if(pDevice==NULL)
//		return false;
//
//	IFileAccessor * pFile=pUSOFile->GetFile();
//	if(pFile==NULL)
//		return false;
//
//	STORAGE_STRUCT * pData;
//	BYTE * pBuff;
//	UINT Size;
//
//	pFile->Read(&Size,sizeof(UINT));
//	pBuff=new BYTE[Size];
//	pFile->Read(pBuff+sizeof(UINT),Size-sizeof(UINT));
//	pData=(STORAGE_STRUCT *)pBuff;
//	pData->ObjectHead.Size=Size;
//
//
//	if((!GetClassInfo().IsKindOf(pData->ObjectHead.Type))||
//		pData->ObjectHead.Size<sizeof(STORAGE_STRUCT))
//	{	
//		delete[] pBuff;
//		return false;
//	}
//
//	Destory();
//
//	pData->ObjectHead.Name[USO_FILE_MAX_OBJECT_NAME-1]=0;
//	SetName(pData->ObjectHead.Name);
//
//	m_VertexFormat=pData->VertexFormat;
//	m_PrimitiveType=pData->PrimitiveType;
//	m_PrimitiveCount=pData->PrimitiveCount;
//	m_VertexCount=pData->VertexCount;
//	m_IndexCount=pData->IndexCount;
//	m_BoundingBox=pData->BoundingBox;
//	m_BoundingSphere=pData->BoundingSphere;
//	m_Property=pData->Property;		
//	m_pVertexs=NULL;
//	m_pDXVertexBuffer=NULL;
//	m_IsVertexsSelfDelete=false;
//	m_IsDXVertexBufferSelfRelease=false;	
//	m_StartVertex=0;
//	m_pIndexs=NULL;
//	m_pDXIndexBuffer=NULL;
//	m_IsIndexsSelfDelete=false;
//	m_IsDXIndexBufferSelfRelease=false;	
//	m_StartIndex=0;	
//	m_Flag=SMF_IS_VISIBLE;
//
//	BYTE * pCurData=pBuff+sizeof(STORAGE_STRUCT);
//
//
//	if(m_VertexCount)
//	{
//		AllocDXVertexBuffer(pDevice);
//
//		int DataSize=m_VertexCount*m_VertexFormat.VertexSize;		
//		BYTE *pDataBuff;
//		m_pDXVertexBuffer->Lock(0,0,(LPVOID *)&pDataBuff,0);
//		memcpy(	pDataBuff,pCurData,DataSize);
//		m_pDXVertexBuffer->Unlock();
//		pCurData+=DataSize;
//	}
//	if(m_IndexCount)
//	{
//		AllocDXIndexBuffer(pDevice);
//		int DataSize=m_IndexCount*m_VertexFormat.IndexSize;
//		BYTE *pDataBuff;
//		m_pDXIndexBuffer->Lock(0,0,(LPVOID *)&pDataBuff,0);
//		memcpy(	pDataBuff,pCurData,DataSize);
//		m_pDXIndexBuffer->Unlock();
//		pCurData+=DataSize;
//	}
//
//	delete[] pBuff;
//	return m_Material.FromUSOFile(pUSOFile,Param);
//}

bool CD3DSubMesh::ToSmartStruct(CSmartStruct& Packet,CUSOFile * pUSOFile,UINT Param)
{
	if(!CNameObject::ToSmartStruct(Packet,pUSOFile,Param))
		return false;
	if(m_VertexFormat.pVertexDeclaration)
	{
		UINT ElementCount;
		m_VertexFormat.pVertexDeclaration->GetDeclaration(NULL,&ElementCount);
		D3DVERTEXELEMENT9 * pElements=new D3DVERTEXELEMENT9[ElementCount];
		m_VertexFormat.pVertexDeclaration->GetDeclaration(pElements,&ElementCount);
		bool Ret=Packet.AddMember(SST_D3DSM_VERTEX_DECLARATION,(char *)pElements,sizeof(D3DVERTEXELEMENT9)*ElementCount);
		delete[] pElements;
		if(!Ret)
			return false;
	}
	else
	{
		CHECK_SMART_STRUCT_ADD_AND_RETURN(Packet.AddMember(SST_D3DSM_FVF,m_VertexFormat.FVF));
	}
	CHECK_SMART_STRUCT_ADD_AND_RETURN(Packet.AddMember(SST_D3DSM_VERTEX_SIZE,m_VertexFormat.VertexSize));
	CHECK_SMART_STRUCT_ADD_AND_RETURN(Packet.AddMember(SST_D3DSM_INDEX_SIZE,m_VertexFormat.IndexSize));
	CHECK_SMART_STRUCT_ADD_AND_RETURN(Packet.AddMember(SST_D3DSM_PRIMITIVE_TYPE,m_PrimitiveType));
	CHECK_SMART_STRUCT_ADD_AND_RETURN(Packet.AddMember(SST_D3DSM_PRIMITIVE_COUNT,m_PrimitiveCount));
	CHECK_SMART_STRUCT_ADD_AND_RETURN(Packet.AddMember(SST_D3DSM_VERTEX_COUNT,m_VertexCount));
	CHECK_SMART_STRUCT_ADD_AND_RETURN(Packet.AddMember(SST_D3DSM_INDEX_COUNT,m_IndexCount));
	CHECK_SMART_STRUCT_ADD_AND_RETURN(Packet.AddMember(SST_D3DSM_BOUNDING_BOX,(char *)&m_BoundingBox,sizeof(m_BoundingBox)));
	CHECK_SMART_STRUCT_ADD_AND_RETURN(Packet.AddMember(SST_D3DSM_BOUNDING_SPHERE,(char *)&m_BoundingSphere,sizeof(m_BoundingSphere)));
	CHECK_SMART_STRUCT_ADD_AND_RETURN(Packet.AddMember(SST_D3DSM_PROPERTY,m_Property));

	if(m_VertexCount)
	{
		UINT DataSize=m_VertexCount*m_VertexFormat.VertexSize;
		bool Ret;
		if(m_pDXVertexBuffer)
		{
			BYTE *pBuff;
			m_pDXVertexBuffer->Lock(0,0,(LPVOID *)&pBuff,0);			
			Ret=Packet.AddMember(SST_D3DSM_VERTEX,(char *)pBuff,DataSize);
			m_pDXVertexBuffer->Unlock();
			
		}
		else
		{
			Ret=Packet.AddMember(SST_D3DSM_VERTEX,(char *)m_pVertexs,DataSize);
		}
		if(!Ret)
			return false;
	}
	if(m_IndexCount)
	{
		UINT DataSize=m_IndexCount*m_VertexFormat.IndexSize;
		bool Ret;
		if(m_pDXIndexBuffer)
		{
			BYTE *pBuff;
			m_pDXIndexBuffer->Lock(0,0,(LPVOID *)&pBuff,0);			
			Ret=Packet.AddMember(SST_D3DSM_INDEX,(char *)pBuff,DataSize);
			m_pDXIndexBuffer->Unlock();
			
		}
		if(!Ret)
			return false;
	}

	{
		UINT BufferSize;
		void * pBuffer=Packet.PrepareMember(BufferSize);
		CSmartStruct SubPacket(pBuffer,BufferSize,true);
		if(!m_Material.ToSmartStruct(SubPacket,pUSOFile,Param))
			return false;
		if(!Packet.FinishMember(SST_D3DSM_MATERIAL,SubPacket.GetDataLen()))
			return false;
	}
	return true;
}
bool CD3DSubMesh::FromSmartStruct(CSmartStruct& Packet,CUSOFile * pUSOFile,UINT Param)
{
	CD3DDevice * pDevice=(CD3DDevice *)Param;
	if(pDevice==NULL)
		return false;

	if(!CNameObject::FromSmartStruct(Packet,pUSOFile,Param))
		return false;

	void * Pos=Packet.GetFirstMemberPosition();
	while(Pos)
	{
		WORD MemberID;
		CSmartValue Value=Packet.GetNextMember(Pos,MemberID);
		switch(MemberID)
		{
		case SST_D3DSM_FVF:
			m_VertexFormat.FVF=Value;
			break;
		case SST_D3DSM_VERTEX_DECLARATION:
			{
				D3DVERTEXELEMENT9* pVertexElements=(D3DVERTEXELEMENT9*)((LPCTSTR)Value);				
				if(pDevice->GetD3DDevice()->CreateVertexDeclaration(pVertexElements,&(m_VertexFormat.pVertexDeclaration))!=D3D_OK)
					return false;
			}
			break;
		case SST_D3DSM_VERTEX_SIZE:
			m_VertexFormat.VertexSize=Value;
			break;
		case SST_D3DSM_INDEX_SIZE:
			m_VertexFormat.IndexSize=Value;
			break;
		case SST_D3DSM_PRIMITIVE_TYPE:
			m_PrimitiveType=Value;
			break;
		case SST_D3DSM_PRIMITIVE_COUNT:
			m_PrimitiveCount=Value;
			break;
		case SST_D3DSM_VERTEX_COUNT:
			m_VertexCount=Value;
			break;
		case SST_D3DSM_INDEX_COUNT:
			m_IndexCount=Value;
			break;
		case SST_D3DSM_BOUNDING_BOX:
			memcpy(&m_BoundingBox,(LPCTSTR)Value,sizeof(m_BoundingBox));
			break;
		case SST_D3DSM_BOUNDING_SPHERE:
			memcpy(&m_BoundingSphere,(LPCTSTR)Value,sizeof(m_BoundingSphere));
			break;
		case SST_D3DSM_PROPERTY:
			m_Property=Value;
			break;
		case SST_D3DSM_VERTEX:
			if(m_VertexCount)
			{
				AllocDXVertexBuffer(pDevice);

				int DataSize=m_VertexCount*m_VertexFormat.VertexSize;		
				BYTE *pDataBuff;
				m_pDXVertexBuffer->Lock(0,0,(LPVOID *)&pDataBuff,0);
				memcpy(	pDataBuff,(LPCTSTR)Value,DataSize);
				m_pDXVertexBuffer->Unlock();				
			}		
			break;
		case SST_D3DSM_INDEX:
			if(m_IndexCount)
			{
				AllocDXIndexBuffer(pDevice);
				int DataSize=m_IndexCount*m_VertexFormat.IndexSize;
				BYTE *pDataBuff;
				m_pDXIndexBuffer->Lock(0,0,(LPVOID *)&pDataBuff,0);
				memcpy(	pDataBuff,(LPCTSTR)Value,DataSize);
				m_pDXIndexBuffer->Unlock();				
			}
			break;
		case SST_D3DSM_MATERIAL:
			{
				CSmartStruct SubPacket=Value;
				if(!m_Material.FromSmartStruct(SubPacket,pUSOFile,Param))
					return false;
			}
			break;
		}
	}
	return true;
}

UINT CD3DSubMesh::GetSmartStructSize(UINT Param)
{
	UINT Size=CNameObject::GetSmartStructSize(Param);

	if(m_VertexFormat.pVertexDeclaration)
	{
		UINT ElementCount;
		m_VertexFormat.pVertexDeclaration->GetDeclaration(NULL,&ElementCount);
		Size+=SMART_STRUCT_STRING_MEMBER_SIZE(sizeof(D3DVERTEXELEMENT9)*ElementCount);
	}
	else
	{
		Size+=SMART_STRUCT_FIX_MEMBER_SIZE(sizeof(m_VertexFormat.FVF));
	}
	Size+=SMART_STRUCT_FIX_MEMBER_SIZE(sizeof(m_VertexFormat.VertexSize));
	Size+=SMART_STRUCT_FIX_MEMBER_SIZE(sizeof(m_VertexFormat.IndexSize));
	Size+=SMART_STRUCT_FIX_MEMBER_SIZE(sizeof(m_PrimitiveType));
	Size+=SMART_STRUCT_FIX_MEMBER_SIZE(sizeof(m_PrimitiveCount));
	Size+=SMART_STRUCT_FIX_MEMBER_SIZE(sizeof(m_VertexCount));
	Size+=SMART_STRUCT_FIX_MEMBER_SIZE(sizeof(m_IndexCount));
	Size+=SMART_STRUCT_STRING_MEMBER_SIZE(sizeof(m_BoundingBox));
	Size+=SMART_STRUCT_STRING_MEMBER_SIZE(sizeof(m_BoundingSphere));
	Size+=SMART_STRUCT_FIX_MEMBER_SIZE(sizeof(m_Property));
	Size+=SMART_STRUCT_STRING_MEMBER_SIZE(m_VertexCount*m_VertexFormat.VertexSize);
	Size+=SMART_STRUCT_STRING_MEMBER_SIZE(m_IndexCount*m_VertexFormat.IndexSize);

	Size+=SMART_STRUCT_STRUCT_MEMBER_SIZE(m_Material.GetSmartStructSize(Param));
	
	return Size;
}

//CNameObject::STORAGE_STRUCT * CD3DSubMesh::USOCreateHead(UINT Param)
//{
//	STORAGE_STRUCT * pHead=new STORAGE_STRUCT;
//	ZeroMemory(pHead,sizeof(STORAGE_STRUCT));
//	pHead->Size=sizeof(STORAGE_STRUCT);
//	return pHead;
//}
//
//int CD3DSubMesh::USOWriteHead(CNameObject::STORAGE_STRUCT * pHead,CUSOFile * pUSOFile,UINT Param)
//{
//	int HeadSize=CNameObject::USOWriteHead(pHead,pUSOFile,Param);
//	if(HeadSize<0)
//		return -1;
//
//	STORAGE_STRUCT * pLocalHead=(STORAGE_STRUCT *)pHead;
//
//	pLocalHead->Size+=m_VertexCount*m_VertexFormat.VertexSize+m_IndexCount*m_VertexFormat.IndexSize;
//	
//	pLocalHead->VertexFormat.FVF=m_VertexFormat.FVF;
//	pLocalHead->VertexFormat.VertexSize=m_VertexFormat.VertexSize;
//	pLocalHead->VertexFormat.IndexSize=m_VertexFormat.IndexSize;
//	pLocalHead->PrimitiveType=m_PrimitiveType;
//	pLocalHead->PrimitiveCount=m_PrimitiveCount;
//	pLocalHead->VertexCount=m_VertexCount;
//	pLocalHead->IndexCount=m_IndexCount;
//	pLocalHead->BoundingBox=m_BoundingBox;
//	pLocalHead->BoundingSphere=m_BoundingSphere;
//	pLocalHead->Property=m_Property;	
//
//	return sizeof(STORAGE_STRUCT);
//}
//
//bool CD3DSubMesh::USOWriteData(CNameObject::STORAGE_STRUCT * pHead,CUSOFile * pUSOFile,UINT Param)
//{
//	if(!CNameObject::USOWriteData(pHead,pUSOFile,Param))
//		return false;
//
//	if(pUSOFile==NULL)
//		return false;	
//
//	IFileAccessor * pFile=pUSOFile->GetFile();
//	if(pFile==NULL)
//		return false;
//
//	if(m_VertexCount&&m_pDXVertexBuffer)
//	{
//		BYTE *pBuff;
//		m_pDXVertexBuffer->Lock(0,0,(LPVOID *)&pBuff,0);
//		UINT64 DataSize=m_VertexCount*m_VertexFormat.VertexSize;
//		UINT64 WriteSize=pFile->Write(pBuff+m_StartVertex*m_VertexFormat.VertexSize,DataSize);			
//		m_pDXVertexBuffer->Unlock();
//		if(WriteSize<DataSize)
//			return false;
//	}
//	if(m_IndexCount&&m_pDXIndexBuffer)
//	{
//		BYTE *pBuff;
//		m_pDXIndexBuffer->Lock(0,0,(LPVOID *)&pBuff,0);
//		UINT64 DataSize=m_IndexCount*m_VertexFormat.IndexSize;
//		UINT64 WriteSize=pFile->Write(pBuff+m_StartIndex*m_VertexFormat.IndexSize,DataSize);
//		m_pDXIndexBuffer->Unlock();
//		if(WriteSize<DataSize)
//			return false;
//	}
//	return true;
//}
//
//bool CD3DSubMesh::USOWriteChild(CNameObject::STORAGE_STRUCT * pHead,CUSOFile * pUSOFile,UINT Param)
//{
//	if(!CNameObject::USOWriteChild(pHead,pUSOFile,Param))
//		return false;
//
//	return m_Material.ToUSOFile(pUSOFile,Param);
//
//}
//
//int CD3DSubMesh::USOReadHead(CNameObject::STORAGE_STRUCT * pHead,CUSOFile * pUSOFile,UINT Param)
//{	
//	int ReadSize=CNameObject::USOReadHead(pHead,pUSOFile,Param);
//	if(ReadSize<0)
//		return -1;
//
//	if(pHead->Size<sizeof(STORAGE_STRUCT))
//		return false;
//
//	STORAGE_STRUCT * pLocalHead=(STORAGE_STRUCT *)pHead;
//
//
//	m_VertexFormat.FVF=pLocalHead->VertexFormat.FVF;
//	m_VertexFormat.VertexSize=pLocalHead->VertexFormat.VertexSize;
//	m_VertexFormat.IndexSize=pLocalHead->VertexFormat.IndexSize;
//	m_PrimitiveType=pLocalHead->PrimitiveType;
//	m_PrimitiveCount=pLocalHead->PrimitiveCount;
//	m_VertexCount=pLocalHead->VertexCount;
//	m_IndexCount=pLocalHead->IndexCount;
//	m_BoundingBox=pLocalHead->BoundingBox;
//	m_BoundingSphere=pLocalHead->BoundingSphere;
//	m_Property=pLocalHead->Property;		
//	m_pVertexs=NULL;
//	m_pDXVertexBuffer=NULL;
//	m_IsVertexsSelfDelete=false;
//	m_IsDXVertexBufferSelfRelease=false;	
//	m_StartVertex=0;
//	m_pIndexs=NULL;
//	m_pDXIndexBuffer=NULL;
//	m_IsIndexsSelfDelete=false;
//	m_IsDXIndexBufferSelfRelease=false;	
//	m_StartIndex=0;	
//	m_Flag=0;
//
//	return sizeof(STORAGE_STRUCT);
//}
//
//int CD3DSubMesh::USOReadData(CNameObject::STORAGE_STRUCT * pHead,CUSOFile * pUSOFile,BYTE * pData,int DataSize,UINT Param)
//{
//	int ReadSize=CNameObject::USOReadData(pHead,pUSOFile,pData,DataSize,Param);
//
//	CD3DDevice * pDevice=(CD3DDevice *)Param;
//	if(pDevice==NULL)
//		return false;
//
//	pData+=ReadSize;
//	DataSize-=ReadSize;
//
//	STORAGE_STRUCT * pLocalHead=(STORAGE_STRUCT *)pHead;
//
//	if(m_VertexCount)
//	{
//		AllocDXVertexBuffer(pDevice);
//
//		int DataSize=m_VertexCount*m_VertexFormat.VertexSize;		
//		BYTE *pDataBuff;
//		m_pDXVertexBuffer->Lock(0,0,(LPVOID *)&pDataBuff,0);
//		memcpy(	pDataBuff,pData,DataSize);
//		m_pDXVertexBuffer->Unlock();
//		pData+=DataSize;
//		ReadSize+=DataSize;
//	}
//	if(m_IndexCount)
//	{
//		AllocDXIndexBuffer(pDevice);
//		int DataSize=m_IndexCount*m_VertexFormat.IndexSize;
//		BYTE *pDataBuff;
//		m_pDXIndexBuffer->Lock(0,0,(LPVOID *)&pDataBuff,0);
//		memcpy(	pDataBuff,pData,DataSize);
//		m_pDXIndexBuffer->Unlock();
//		pData+=DataSize;
//		ReadSize+=DataSize;
//	}
//	return ReadSize;
//}
//
//bool CD3DSubMesh::USOReadChild(CNameObject::STORAGE_STRUCT * pHead,CUSOFile * pUSOFile,UINT Param)
//{	
//	if(!CNameObject::USOReadChild(pHead,pUSOFile,Param))
//		return false;
//
//	//PrintImportantLog(0,"CD3DSubMesh需要读取一个CD3DSubMeshMaterial");
//
//	return m_Material.FromUSOFile(pUSOFile,Param);	
//}

//bool CD3DSubMesh::USOReadFinish(CNameObject::STORAGE_STRUCT * pHead,UINT Param)
//{
//	if(!CNameObject::USOReadFinish(pHead,Param))
//		return false;
//
//	if(m_Material.GetFX())
//	{
//		int TransparentFactor;
//
//		if(m_Material.GetFX()->GetInt("TransparentFactor",TransparentFactor))
//		{
//			if(TransparentFactor>=4)
//				SetTransparent(true);
//		}
//	}
//	SetVisible(true);
//
//	return true;
//}




}