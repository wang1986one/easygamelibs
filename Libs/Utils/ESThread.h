/****************************************************************************/
/*                                                                          */
/*      文件名:    ESThread.h			                                    */
/*      创建日期:  2010年08月26日                                           */
/*      作者:      Sagasarate                                               */
/*                                                                          */
/*      本软件版权归Sagasarate(sagasarate@sina.com)所有                     */
/*      你可以将本软件用于任何商业和非商业软件开发，但                      */
/*      必须保留此版权声明                                                  */
/*                                                                          */
/****************************************************************************/
#pragma once

class CESThread
{
protected:
	CESVariableList *	m_pVariableList;
	CESFactionList *	m_pFactionList;
	CESBolanStack *		m_pScript;
	CESBolanStack		m_Stack;

	bool				m_IsInInterrupt;
	int					m_InterruptPos;	
	int					m_InterruptRecentControlPos;
	CEasyTimer			m_InterruptTimer;
	UINT				m_InterrupParam;

	ES_BOLAN			m_Result;
	int					m_ResultCode;
	int					m_LastLine;
public:
	CESThread(void);
	~CESThread(void);

	void SetVariableList(CESVariableList * pVariableList);
	CESVariableList * GetVariableList();

	void SetFactionList(CESFactionList * pFactionList);
	CESFactionList * GetFactionList();

	void SetScript(CESBolanStack * pScript);
	CESBolanStack * GetScript();

	CESBolanStack * GetStack();

	void SetInterruptPos(int Pos);
	void SetInterruptRecentControlPos(int RecentControlPos);
	void ClearInterrupt();
	bool IsInInterrupt();
	int GetInterruptPos();
	int GetInterruptRecentControlPos();
	bool IsInterruptTimeOut(UINT TimeOut);

	void SetResult(ES_BOLAN& Result);
	void SetResultCode(int ResultCode);
	ES_BOLAN& GetResult();
	int GetResultCode();
	void SetLastLine(int Line);
	int GetLastLine();
	

	int PushScript(const char * szExpStr);
};

inline void CESThread::SetVariableList(CESVariableList * pVariableList)
{
	m_pVariableList=pVariableList;
}
inline CESVariableList * CESThread::GetVariableList()
{
	return m_pVariableList;
}

inline void CESThread::SetFactionList(CESFactionList * pFactionList)
{
	m_pFactionList=pFactionList;
}
inline CESFactionList * CESThread::GetFactionList()
{
	return m_pFactionList;
}

inline void CESThread::SetScript(CESBolanStack * pScript)
{
	m_pScript=pScript;
}
inline CESBolanStack * CESThread::GetScript()
{
	return m_pScript;
}
inline CESBolanStack * CESThread::GetStack()
{
	return &m_Stack;
}
inline void CESThread::SetInterruptPos(int Pos)
{
	m_IsInInterrupt=true;
	m_InterruptPos=Pos;	
	m_InterruptTimer.SaveTime();
}
inline void CESThread::SetInterruptRecentControlPos(int RecentControlPos)
{
	m_InterruptRecentControlPos=RecentControlPos;
}
inline void CESThread::ClearInterrupt()
{
	m_IsInInterrupt=false;
	m_InterruptPos=0;
	m_InterruptRecentControlPos=0;
}
inline bool CESThread::IsInInterrupt()
{
	return m_IsInInterrupt;
}
inline int CESThread::GetInterruptPos()
{
	return m_InterruptPos;
}
inline int CESThread::GetInterruptRecentControlPos()
{
	return m_InterruptRecentControlPos;
}
inline bool CESThread::IsInterruptTimeOut(UINT TimeOut)
{
	return m_InterruptTimer.IsTimeOut(TimeOut);
}
inline void CESThread::SetResult(ES_BOLAN& Result)
{
	m_Result=Result;
	m_Result.Type=BOLAN_TYPE_VALUE;
}
inline void CESThread::SetResultCode(int ResultCode)
{
	m_ResultCode=ResultCode;
}

inline ES_BOLAN& CESThread::GetResult()
{
	return m_Result;
}
inline int CESThread::GetResultCode()
{
	return m_ResultCode;
}
inline void CESThread::SetLastLine(int Line)
{
	m_LastLine=Line;
}
inline int CESThread::GetLastLine()
{
	return m_LastLine;
}
inline int CESThread::PushScript(const char * szExpStr)
{
	if(m_pVariableList==NULL)
	{
		m_ResultCode=6001;
		return m_ResultCode;
	}
	if(m_pFactionList==NULL)
	{
		m_ResultCode=6002;
		return m_ResultCode;
	}
	if(m_pScript==NULL)
	{
		m_ResultCode=6003;
		return m_ResultCode;
	}
	m_pScript->Clear();
	m_ResultCode=m_pScript->PushScript(szExpStr,m_pVariableList,m_pFactionList,m_LastLine);
	return m_ResultCode;
}