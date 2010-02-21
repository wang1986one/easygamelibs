#pragma once



class CClassifiedID
{
protected:

	union CLASSIFIED_ID
	{
		UINT	ID;
		struct CLASSIFIED_4
		{
			BYTE	Index;			//索引编号
			BYTE	Group;			//分组号
			BYTE	SubType;		//子类型
			BYTE	MainType;		//主类型
		}ClassifiedID_4;
		struct CLASSIFIED_2
		{
			WORD	Index;		//索引编号		
			WORD	Type;		//类型			
		}ClassifiedID_2;
	};

	CLASSIFIED_ID		m_ID;

public:
	CClassifiedID()
	{
		m_ID.ID=0;
	}
	CClassifiedID(unsigned int ID)
	{
		m_ID.ID=ID;
	}
	CClassifiedID(int ID)
	{
		m_ID.ID=(UINT)ID;
	}
	CClassifiedID(const char * IDStr)
	{
		FromStr(IDStr);
	}
	CClassifiedID& operator=(CClassifiedID& ID)
	{
		m_ID.ID=ID.m_ID.ID;
		return *this;
	}
	CClassifiedID& operator=(unsigned int ID)
	{
		m_ID.ID=ID;
		return *this;
	}
	CClassifiedID& operator=(int ID)
	{
		m_ID.ID=(UINT)ID;
		return *this;
	}
	CClassifiedID& operator=(const char * IDStr)
	{
		FromStr(IDStr);
		return *this;
	}
	operator int()
	{
		return (int)m_ID.ID;
	}
	operator unsigned int()
	{
		return m_ID.ID;
	}
	operator const char *()
	{
		return ToStr();
	}	
	void FromStr(const char * IDStr)
	{
		CStringSplitter Split(IDStr,'-');

		if(Split.GetCount()>=4)
		{
			m_ID.ClassifiedID_4.MainType=atoi(Split[0]);
			m_ID.ClassifiedID_4.SubType=atoi(Split[1]);
			m_ID.ClassifiedID_4.Group=atoi(Split[2]);
			m_ID.ClassifiedID_4.Index=atoi(Split[3]);
		}
	}
	const char * ToStr()
	{
		static char Buffer[32];

		sprintf_s(Buffer,32,"%u-%u-%u-%u",
			m_ID.ClassifiedID_4.MainType,
			m_ID.ClassifiedID_4.SubType,
			m_ID.ClassifiedID_4.Group,
			m_ID.ClassifiedID_4.Index);
		return Buffer;
	}
	BYTE& MainType()
	{
		return m_ID.ClassifiedID_4.MainType;
	}
	BYTE& SubType()
	{
		return m_ID.ClassifiedID_4.SubType;
	}
	BYTE& Group()
	{
		return m_ID.ClassifiedID_4.Group;
	}
	BYTE& Index()
	{
		return m_ID.ClassifiedID_4.Index;
	}
	WORD& BigType()
	{
		return m_ID.ClassifiedID_2.Type;
	}
	WORD& BigIndex()
	{
		return m_ID.ClassifiedID_2.Index;
	}
};