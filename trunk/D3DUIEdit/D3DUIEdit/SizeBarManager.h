/****************************************************************************/
/*                                                                          */
/*      �ļ���:    SizeBarManager.h                                         */
/*      ��������:  2011��10��07��                                           */
/*      ����:      Sagasarate                                               */
/*                                                                          */
/*      �������Ȩ��Sagasarate(sagasarate@sina.com)����                     */
/*      ����Խ�����������κ���ҵ�ͷ���ҵ�����������                      */
/*      ���뱣���˰�Ȩ����                                                  */
/*                                                                          */
/****************************************************************************/
#pragma once
#include "d3dsizebar.h"
#include <vector>

class CSizeBarManager :
	public CNameObject
{
protected:
	std::vector<CD3DSizeBar *>	m_pSizeBarList;
	std::vector<CD3DSizeBar *>	m_pSelectedSizeBarList;
	bool						m_IsControlPress;
public:
	CSizeBarManager(void);
	virtual ~CSizeBarManager(void);

	void AddObject(CD3DSizeBar * pObject);
	void DeleteObject(CD3DSizeBar * pObject);


	bool AddSelectedObject(CD3DSizeBar * pObject);
	void DeleteSelectedObject(CD3DSizeBar * pObject);

	int GetSelectedObjectCount()
	{
		return (int)m_pSelectedSizeBarList.size();
	}
	
	CD3DSizeBar * GetSelectedObject(int Index)
	{
		if(Index>=0&&Index<(int)m_pSelectedSizeBarList.size())
			return m_pSelectedSizeBarList[Index];
		return NULL;
	}

	void ClearSelectedObject()
	{
		for(int i=0;i<(int)m_pSelectedSizeBarList.size();i++)
		{			
			m_pSelectedSizeBarList[i]->ClearSelect();
		}
		m_pSelectedSizeBarList.clear();
	}

	void DeleteAllSelectedObject()
	{
		if(AfxMessageBox(_T("�Ƿ�Ҫɾ��ѡ�ж���?"),MB_YESNO)==IDYES)
		{
			std::vector<CD3DSizeBar *> TempList;
			TempList.assign(m_pSelectedSizeBarList.begin(),m_pSelectedSizeBarList.end());
			m_pSelectedSizeBarList.clear();
			for(int i=(int)TempList.size()-1;i>=0;i--)
			{			
				TempList[i]->Release();
			}		
		}
	}

	void SetControlPress(bool Press)
	{
		m_IsControlPress=Press;
	}

	bool IsControlPress()
	{
		return m_IsControlPress;
	}


	void DoMove(int dx,int dy);
	void DoResize(int dx,int dy);	
};
