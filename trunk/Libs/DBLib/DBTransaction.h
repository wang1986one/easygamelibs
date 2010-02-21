#pragma once

namespace DBLib
{

class CDBTransaction :
	public CNameObject
{
protected:
	bool		m_IsSucceed;

	DECLARE_CLASS_INFO_STATIC(CDBTransaction)
public:
	CDBTransaction(void);
	~CDBTransaction(void);

	virtual bool OnExecute(IDBConnection * pConnection);
	virtual void OnFinish();

	void SetResult(bool IsSucceed)
	{
		m_IsSucceed=IsSucceed;
	}
};

}