/****************************************************************************/
/*                                                                          */
/*      文件名:    DlgWOWADTImport.cpp                                      */
/*      创建日期:  2011年11月15日                                           */
/*      作者:      Sagasarate                                               */
/*                                                                          */
/*      本软件版权归Sagasarate(sagasarate@sina.com)所有                     */
/*      你可以将本软件用于任何商业和非商业软件开发，但                      */
/*      必须保留此版权声明                                                  */
/*                                                                          */
/****************************************************************************/
/****************************************************************************/
/*                                                                          */
/*      文件名:    DlgWOWADTImport.cpp                                      */
/*      创建日期:  2010年02月09日                                           */
/*      作者:      Sagasarate                                               */
/*                                                                          */
/*      本软件版权归Sagasarate(sagasarate@sina.com)所有                     */
/*      你可以将本软件用于任何商业和非商业软件开发，但                      */
/*      必须保留此版权声明                                                  */
/*                                                                          */
/****************************************************************************/
// DlgWOWADTImport.cpp : 实现文件
//

#include "stdafx.h"
#include "DlgWOWADTImport.h"




// CDlgWOWADTImport 对话框

IMPLEMENT_DYNAMIC(CDlgWOWADTImport, CDialog)

CDlgWOWADTImport::CDlgWOWADTImport(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgWOWADTImport::IDD, pParent)
	, m_ModelFileName(_T(""))	
	, m_IsImportAllArea(FALSE)
{
	m_pD3DDevice=NULL;
	m_hBMPMiniMap=NULL;
	m_IsBigAlphaMask=false;
	m_MapID=0;
}

CDlgWOWADTImport::~CDlgWOWADTImport()
{
	if(m_hBMPMiniMap)
	{
		//m_stMiniMap.SetBitmap(NULL);
		DeleteObject(m_hBMPMiniMap);
		m_hBMPMiniMap=NULL;
	}
}

void CDlgWOWADTImport::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_MODEL_FILE_NAME, m_ModelFileName);
	DDX_Control(pDX, IDC_COMBO_MAP, m_cbMap);
	DDX_Control(pDX, IDC_COMBO_AREA, m_cbArea);
	DDX_Check(pDX, IDC_CHECK_IMPORT_ALL_AREA, m_IsImportAllArea);
	DDX_Control(pDX, IDC_MINI_MAP, m_stMiniMap);
}


BEGIN_MESSAGE_MAP(CDlgWOWADTImport, CDialog)
	ON_BN_CLICKED(IDC_SELECT_MODEL_FILE, &CDlgWOWADTImport::OnBnClickedSelectModelFile)
	ON_BN_CLICKED(IDOK, &CDlgWOWADTImport::OnBnClickedOk)
	ON_CBN_SELCHANGE(IDC_COMBO_MAP, &CDlgWOWADTImport::OnCbnSelchangeComboMap)
	ON_CBN_SELCHANGE(IDC_COMBO_AREA, &CDlgWOWADTImport::OnCbnSelchangeComboArea)
	ON_BN_CLICKED(ID_SHOW_FULL_MAP, &CDlgWOWADTImport::OnBnClickedShowFullMap)
END_MESSAGE_MAP()


// CDlgWOWADTImport 消息处理程序
void CDlgWOWADTImport::OnBnClickedSelectModelFile()
{
	// TODO: 在此添加控件通知处理程序代码
	CFileDialog dlg(true,_T("*.adt"),_T("*.adt"),OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT|OFN_NOCHANGEDIR,
		_T("Blizzard ADT File(*.adt)|*.adt|All Files(*.*)|*.*||"));

	if(dlg.DoModal()==IDOK)
	{
		UpdateData(true);
		m_ModelFileName=dlg.GetPathName();		
		UpdateData(false);
	}
}

void CDlgWOWADTImport::OnBnClickedOk()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(true);
	if(m_IsImportAllArea)
	{			
		for(int i=0;i<m_cbArea.GetCount();i++)
		{
			CString AreaFileName;				
			m_cbArea.GetLBText(i,AreaFileName);
			int Type=m_cbArea.GetItemData(i);

			if(Type/10000==0)
			{
				int Pos=AreaFileName.Find(' ');
				m_AreaFileNameList.Add((LPCTSTR)AreaFileName.Left(Pos));
			}
		}		
	}
	else
	{
		m_AreaFileNameList.Add((LPCTSTR)m_ModelFileName);
	}
	OnOK();
}

void CDlgWOWADTImport::OnCbnSelchangeComboMap()
{
	// TODO: 在此添加控件通知处理程序代码
	m_cbArea.ResetContent();
	int Sel=m_cbMap.GetCurSel();
	UINT Index=m_cbMap.GetItemData(Sel);
	CBLZWOWDatabase::BLZ_DB_MAP_INFO * pMapInfo=CBLZWOWDatabase::GetInstance()->GetMapInfo(Index);
	if(pMapInfo)
	{
		m_MapID=pMapInfo->ID;
		CEasyString WDTFileName;
		WDTFileName.Format(_T("%s\\%s\\%s.wdt"),
			(LPCTSTR)WOW_MAP_ADT_FILE_DIR,(LPCTSTR)pMapInfo->InternalName,(LPCTSTR)pMapInfo->InternalName);
		m_WDLFileName.Format(_T("%s\\%s\\%s.wdl"),
			(LPCTSTR)WOW_MAP_ADT_FILE_DIR,(LPCTSTR)pMapInfo->InternalName,(LPCTSTR)pMapInfo->InternalName);
		CBLZWDTFile WDTFile;
		if(WDTFile.Load(WDTFileName))
		{
			m_IsBigAlphaMask=WDTFile.IsBigAlphaMask();
			UINT MinX=63,MinY=63;
			UINT MaxX=0,MaxY=0;
			for(UINT x=0;x<64;x++)
			{
				for(UINT y=0;y<64;y++)
				{
					if(WDTFile.IsAreaExist(x,y))
					{
						if(x<MinX)
							MinX=x;
						if(x>MaxX)
							MaxX=x;
						if(y<MinY)
							MinY=y;
						if(y>MaxY)
							MaxY=y;
						CEasyString MiniMapFile;
						MiniMapFile.Format(_T("%s\\map%d_%d.blp"),
							(LPCTSTR)pMapInfo->InternalName,
							x,y);

						CEasyString AreaFile=WOW_MAP_ADT_FILE_DIR;
						AreaFile.Format(_T("%s\\%s\\%s_%d_%d.adt %s"),
							WOW_MAP_ADT_FILE_DIR,
							(LPCTSTR)pMapInfo->InternalName,
							(LPCTSTR)pMapInfo->InternalName,
							x,y,(LPCTSTR)MiniMapFile);
						
						int index=m_cbArea.InsertString(m_cbArea.GetCount(),AreaFile);
						UINT Param=y*100+x;
						m_cbArea.SetItemData(index,Param);
					}
				}
			}
			m_SenceBox.m_Min.x=BLZ_ADT_MAP_AREA_SIZE*MinX-10.0f;
			m_SenceBox.m_Min.y=-BLZ_ADT_MAP_AREA_SIZE*64;
			m_SenceBox.m_Min.z=-BLZ_ADT_MAP_AREA_SIZE*MaxY-10.0f;
			m_SenceBox.m_Max.x=BLZ_ADT_MAP_AREA_SIZE*MaxX+10.0f;
			m_SenceBox.m_Max.y=BLZ_ADT_MAP_AREA_SIZE*64;
			m_SenceBox.m_Max.z=-BLZ_ADT_MAP_AREA_SIZE*MinY+10.0f;


			for(UINT i=0;i<WDTFile.GetWMOInfoCount();i++)
			{
				CBLZWDTFile::WMO_OBJECT_INFO * pInfo=WDTFile.GetWMOInfo(i);
				
				//CEasyString MiniMapFile;
				//MiniMapFile.Format("%s\\map%d_%d.blp",
				//	(LPCTSTR)pMapInfo->InternalName,
				//	x,y);

				CEasyString AreaFile=WOW_MAP_ADT_FILE_DIR;
				AreaFile.Format(_T("%s"),
					(LPCTSTR)pInfo->Name);

				int index=m_cbArea.InsertString(m_cbArea.GetCount(),AreaFile);						
				m_cbArea.SetItemData(index,10000);
			}
		}
		else
		{
			CEasyString Msg;
			Msg.Format(_T("无法打开%s"),WDTFileName);
			AfxMessageBox(Msg);
		}

	}
}

void CDlgWOWADTImport::OnCbnSelchangeComboArea()
{
	// TODO: 在此添加控件通知处理程序代码
	
	CString AreaFileName;
	int Sel=m_cbArea.GetCurSel();
	m_cbArea.GetLBText(Sel,AreaFileName);
	UpdateData(true);
	int Pos=AreaFileName.Find(' ');
	m_ModelFileName=AreaFileName.Left(Pos);
	CEasyString MiniMapFile=(LPCTSTR)AreaFileName.Right(AreaFileName.GetLength()-Pos-1);
	MiniMapFile=_T("\\")+MiniMapFile;
	MiniMapFile=WOW_MINI_MAP_FILE_DIR+MiniMapFile;	
	CD3DTexture * pMiniMapTexture=m_pD3DDevice->GetTextureManager()->LoadTexture(MiniMapFile);
	if(pMiniMapTexture)
	{
		LPD3DXBUFFER pDataBuffer;
		if(D3DXSaveTextureToFileInMemory(&pDataBuffer,D3DXIFF_BMP,pMiniMapTexture->GetD3DTexture(),NULL)==D3D_OK)
		{
			BITMAPFILEHEADER * pBMPHeader=(BITMAPFILEHEADER *)pDataBuffer->GetBufferPointer();
			BITMAPINFO * pBMI=(BITMAPINFO *)((BYTE *)pDataBuffer->GetBufferPointer()+sizeof(BITMAPFILEHEADER));
			BYTE * pBits=(BYTE *)pDataBuffer->GetBufferPointer()+pBMPHeader->bfOffBits;
			if(m_hBMPMiniMap)
			{
				HBITMAP hOld=m_stMiniMap.SetBitmap(NULL);
				DeleteObject(m_hBMPMiniMap);
				m_hBMPMiniMap=NULL;
			}
			m_hBMPMiniMap=CreateDIBitmap(GetDC()->GetSafeHdc(),&pBMI->bmiHeader,CBM_INIT,pBits,pBMI,DIB_RGB_COLORS);
			if(m_hBMPMiniMap)
			{
				m_stMiniMap.SetBitmap(m_hBMPMiniMap);
				m_stMiniMap.Invalidate();
			}
			SAFE_RELEASE(pDataBuffer);
		}
		SAFE_RELEASE(pMiniMapTexture);
	}
	UpdateData(false);
	
}

BOOL CDlgWOWADTImport::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  在此添加额外的初始化
	m_AreaFileNameList.Clear();
	m_cbMap.ResetContent();
	m_cbArea.ResetContent();
	for(UINT i=0;i<CBLZWOWDatabase::GetInstance()->GetMapInfoCount();i++)
	{
		CBLZWOWDatabase::BLZ_DB_MAP_INFO * pMapInfo=CBLZWOWDatabase::GetInstance()->GetMapInfo(i);
		if(pMapInfo)
		{
			m_cbMap.InsertString(i,pMapInfo->Name);
			m_cbMap.SetItemData(i,i);
		}
	}
	

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}

void CDlgWOWADTImport::OnBnClickedShowFullMap()
{
	// TODO: 在此添加控件通知处理程序代码
	int Sel=m_cbMap.GetCurSel();
	UINT Index=m_cbMap.GetItemData(Sel);
	CBLZWOWDatabase::BLZ_DB_MAP_INFO * pMapInfo=CBLZWOWDatabase::GetInstance()->GetMapInfo(Index);
	if(pMapInfo)
	{		
		CEasyString WDTFileName;
		WDTFileName.Format(_T("%s\\%s\\%s.wdt"),
			(LPCTSTR)WOW_MAP_ADT_FILE_DIR,(LPCTSTR)pMapInfo->InternalName,(LPCTSTR)pMapInfo->InternalName);
		CBLZWDTFile WDTFile;
		if(WDTFile.Load(WDTFileName))
		{
			
			UINT MinX=63,MinY=63;
			UINT MaxX=0,MaxY=0;
			for(UINT x=0;x<64;x++)
			{
				for(UINT y=0;y<64;y++)
				{
					if(WDTFile.IsAreaExist(x,y))
					{
						if(x<MinX)
							MinX=x;
						if(x>MaxX)
							MaxX=x;
						if(y<MinY)
							MinY=y;
						if(y>MaxY)
							MaxY=y;
					}
				}
			}

			if(m_DlgMapView.GetMapID()!=pMapInfo->ID)
			{
				if(MinX<=MaxX&&MinY<=MaxY)
				{
					CDC DestDC,SrcDC;
					DestDC.CreateCompatibleDC(GetDC());
					SrcDC.CreateCompatibleDC(GetDC());

					CBitmap FullMapBMP;

					UINT MapWidth=(MaxX-MinX+1)*64;
					UINT MapHeight=(MaxY-MinY+1)*64;

					FullMapBMP.CreateBitmap(MapWidth,MapHeight,1,32,NULL);

					int err=GetLastError();

					CBitmap * pOldBMP=DestDC.SelectObject(&FullMapBMP);
					DestDC.SetStretchBltMode(COLORONCOLOR);


					for(UINT x=0;x<64;x++)
					{
						for(UINT y=0;y<64;y++)
						{
							if(WDTFile.IsAreaExist(x,y))
							{
								CEasyString MiniMapFile;
								MiniMapFile.Format(_T("%s\\%s\\map%d_%d.blp"),
									WOW_MINI_MAP_FILE_DIR,
									(LPCTSTR)pMapInfo->InternalName,
									x,y);

								//MiniMapFile.Format("%s\\%s",
								//	WOW_MINI_MAP_FILE_DIR,
								//	CBLZWOWDatabase::GetInstance()->GetMiniMapRealFileName(MiniMapFile));
								CD3DTexture * pMiniMapTexture=m_pD3DDevice->GetTextureManager()->LoadTexture(MiniMapFile);
								if(pMiniMapTexture)
								{
									LPD3DXBUFFER pDataBuffer;
									if(D3DXSaveTextureToFileInMemory(&pDataBuffer,D3DXIFF_BMP,pMiniMapTexture->GetD3DTexture(),NULL)==D3D_OK)
									{
										BITMAPFILEHEADER * pBMPHeader=(BITMAPFILEHEADER *)pDataBuffer->GetBufferPointer();
										BITMAPINFO * pBMI=(BITMAPINFO *)((BYTE *)pDataBuffer->GetBufferPointer()+sizeof(BITMAPFILEHEADER));
										BYTE * pBits=(BYTE *)pDataBuffer->GetBufferPointer()+pBMPHeader->bfOffBits;									
										HBITMAP hBMPMiniMap=CreateDIBitmap(GetDC()->GetSafeHdc(),&pBMI->bmiHeader,CBM_INIT,pBits,pBMI,DIB_RGB_COLORS);
										SrcDC.SelectObject(hBMPMiniMap);
										if(!DestDC.StretchBlt((x-MinX)*64,(y-MinY)*64,
											pBMI->bmiHeader.biWidth/4,pBMI->bmiHeader.biHeight/4,&SrcDC,
											0,0,pBMI->bmiHeader.biWidth,pBMI->bmiHeader.biHeight,SRCCOPY))
										{
											AfxMessageBox(_T("拼接小地图失败"));
										}
										DeleteObject(hBMPMiniMap);
										SAFE_RELEASE(pDataBuffer);
									}
									SAFE_RELEASE(pMiniMapTexture);
								}							
							}
						}
					}
					DestDC.SelectObject(pOldBMP);
					m_DlgMapView.SetMap(pMapInfo->ID,(HBITMAP)FullMapBMP.GetSafeHandle(),MapWidth,MapHeight,MinX,MinY);
					FullMapBMP.Detach();
				}				
			}
			if(m_DlgMapView.DoModal()==IDOK)
			{
				UINT SelectX=MinX+m_DlgMapView.m_SelectedMapX/64;
				UINT SelectY=MinY+m_DlgMapView.m_SelectedMapY/64;
				for(int i=0;i<m_cbArea.GetCount();i++)
				{
					CString AreaFileName;				
					m_cbArea.GetLBText(i,AreaFileName);
					UINT Param=m_cbArea.GetItemData(i);
					UINT x=Param%100;
					UINT y=(Param/100)%100;
					if(SelectX==x&&SelectY==y)
					{
						m_cbArea.SetCurSel(i);
						OnCbnSelchangeComboArea();
						break;
					}
				}		
			}
		}
	}

}
