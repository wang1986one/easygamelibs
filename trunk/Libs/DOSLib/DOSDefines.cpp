/****************************************************************************/
/*                                                                          */
/*      �ļ���:    DOSDefines.cpp                                           */
/*      ��������:  2009��07��06��                                           */
/*      ����:      Sagasarate                                               */
/*                                                                          */
/*      �������Ȩ��Sagasarate(sagasarate@sina.com)����                     */
/*      ����Խ�����������κ���ҵ�ͷ���ҵ�����������                      */
/*      ���뱣���˰�Ȩ����                                                  */
/*                                                                          */
/****************************************************************************/
#include "StdAfx.h"

int FindMinObjectID(OBJECT_ID * pObjectIDs,UINT Count)
{	
	int MinPtr=-1;
	UINT64 MinID=0;
	for(UINT i=0;i<Count;i++)
	{
		if(pObjectIDs[i].ID)
		{
			if(MinPtr<0)
			{
				MinPtr=i;
				MinID=pObjectIDs[i].ID;
			}
			else
			{
				if(pObjectIDs[i].ID<MinID)
				{
					MinPtr=i;
					MinID=pObjectIDs[i].ID;
				}
			}
		}
	}	
	return MinPtr;
}

UINT DistinctObjectID(OBJECT_ID * pObjectIDs,UINT Count)
{
	if(Count)
	{
		int FinishPtr=0;
		int LeftPtr=1;
		while(LeftPtr<(int)Count)
		{
			int MinPtr=FindMinObjectID(pObjectIDs+LeftPtr,(int)Count-LeftPtr);
			if(MinPtr>=0)
			{
				MinPtr+=LeftPtr;
				if(pObjectIDs[FinishPtr].ID>pObjectIDs[MinPtr].ID)
				{
					UINT64 Temp;
					Temp=pObjectIDs[FinishPtr].ID;
					pObjectIDs[FinishPtr].ID=pObjectIDs[MinPtr].ID;
					pObjectIDs[MinPtr].ID=Temp;
				}
				else 
				{
					if(MinPtr>LeftPtr)
					{
						UINT64 Temp;
						Temp=pObjectIDs[LeftPtr].ID;
						pObjectIDs[LeftPtr].ID=pObjectIDs[MinPtr].ID;
						pObjectIDs[MinPtr].ID=Temp;
						MinPtr=LeftPtr;
					}
					if(pObjectIDs[FinishPtr].ID==pObjectIDs[LeftPtr].ID)
						pObjectIDs[LeftPtr].ID=0;
					else
					{
						FinishPtr++;
						if(pObjectIDs[FinishPtr].ID!=pObjectIDs[LeftPtr].ID)
						{
							UINT64 Temp;
							Temp=pObjectIDs[LeftPtr].ID;
							pObjectIDs[LeftPtr].ID=pObjectIDs[FinishPtr].ID;
							pObjectIDs[FinishPtr].ID=Temp;
						}
					}
					LeftPtr++;
				}
			}
			else
			{
				break;
			}
		}
		return FinishPtr+1;
	}
	return 0;
}