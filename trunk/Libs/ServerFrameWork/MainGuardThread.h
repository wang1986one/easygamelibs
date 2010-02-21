#pragma once


class CMainGuardThread :
	public CGuardThread,public CStaticObject<CGuardThread>
{
public:
	CMainGuardThread(void);
	~CMainGuardThread(void);
};
