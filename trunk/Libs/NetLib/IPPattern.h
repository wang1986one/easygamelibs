/****************************************************************************/
/*                                                                          */
/*      �ļ���:    IPPattern.h                                              */
/*      ��������:  2009��07��06��                                           */
/*      ����:      Sagasarate                                               */
/*                                                                          */
/*      �������Ȩ��Sagasarate(sagasarate@sina.com)����                     */
/*      ����Խ�����������κ���ҵ�ͷ���ҵ�����������                      */
/*      ���뱣���˰�Ȩ����                                                  */
/*                                                                          */
/****************************************************************************/
#pragma once

#define GET_IP_SECTION(IP,Section)		((IP>>(Section*8))&0xff)

class CIPPattern
{
protected:
	short	m_Pattern[4];
public:
	CIPPattern()
	{
		m_Pattern[0]=-1;
		m_Pattern[1]=-1;
		m_Pattern[2]=-1;
		m_Pattern[3]=-1;
	}
	CIPPattern(short p1,short p2,short p3,short p4)
	{
		SetPattern(p1,p2,p3,p4);
	}
	CIPPattern(LPCTSTR Pattern)
	{
		SetPattern(Pattern);
	}
	CIPPattern(const CIPPattern& IPPattern)
	{
		memcpy(m_Pattern,IPPattern.m_Pattern,sizeof(m_Pattern));
	}
	~CIPPattern()
	{
	}
	const CIPPattern& operator=(const CIPPattern& IPPattern)
	{
		memcpy(m_Pattern,IPPattern.m_Pattern,sizeof(m_Pattern));
		return *this;
	}
	void SetPattern(short p1,short p2,short p3,short p4)
	{
		m_Pattern[0]=p1;
		m_Pattern[1]=p2;
		m_Pattern[2]=p3;
		m_Pattern[3]=p4;
	}
	void SetPattern(LPCTSTR Pattern)
	{
		if(Pattern)
		{
			CStringSplitter Split(Pattern,'.');

			if(Split.GetCount()>=4)
			{				
				for(int i=0;i<4;i++)
				{				
					if(strchr(Split[i],'*')!=NULL)
						m_Pattern[i]=-1;
					else
						m_Pattern[i]=atoi(Split[i]);
				}
				
			}
		}
	}
	bool IsMatch(CIPAddress& IPAddress)
	{
		return (m_Pattern[0]<0||GET_IP_SECTION(IPAddress.GetSockAddr().sin_addr.s_addr,0)==(BYTE)m_Pattern[0])&&
			(m_Pattern[1]<0||GET_IP_SECTION(IPAddress.GetSockAddr().sin_addr.s_addr,1)==(BYTE)m_Pattern[1])&&
			(m_Pattern[2]<0||GET_IP_SECTION(IPAddress.GetSockAddr().sin_addr.s_addr,2)==(BYTE)m_Pattern[2])&&
			(m_Pattern[3]<0||GET_IP_SECTION(IPAddress.GetSockAddr().sin_addr.s_addr,3)==(BYTE)m_Pattern[3]);
	}
	bool IsMatch(LPCSTR IPStr)
	{
		CIPAddress IPAddress(IPStr,0);
		return IsMatch(IPAddress);
	}	
};