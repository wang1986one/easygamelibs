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
	enum
	{
		DEFAULT_LOCAL_VARIABLE_COUNT=64,
		DEFAULT_CALL_STACK_SIZE=16,
		LOCAL_VARIABLE_ID_START=0x80000000,
	};
	CESVariableList *	m_pVariableList;
	CESVariableList		m_LocalVariableList;
	CESFunctionList *	m_pFunctionList;
	CESBolanStack *		m_pScript;
	CESBolanStack		m_Stack;
	CEasyArray<UINT>	m_CallStack;

	bool				m_IsInInterrupt;
	int					m_InterruptPos;	
	CEasyTimer			m_InterruptTimer;
	UINT				m_InterrupCode;

	ES_BOLAN			m_Result;
	int					m_ResultCode;
	int					m_LastLine;
public:
	CESThread(void);
	CESThread(UINT MaxLocalVariableCount);
	~CESThread(void);

	void SetVariableList(CESVariableList * pVariableList);
	CESVariableList * GetVariableList();
	CESVariableList * GetLocalVariableList();
	ES_VARIABLE * FindVariable(UINT VarID);
	ES_VARIABLE * FindVariable(LPCTSTR VarName);
	void ClearLocalVariable();

	void SetFunctionList(CESFunctionList * pFunctionList);
	CESFunctionList * GetFunctionList();

	void SetScript(CESBolanStack * pScript);
	CESBolanStack * GetScript();

	CESBolanStack * GetStack();
	void ClearStack();
	bool PushValueToStack(const ES_BOLAN& Value);

	void PushCallStack(UINT Pos);
	int PopCallStack();
	void ClearCallStack();

	void SetInterruptPos(int Pos);
	void ClearInterrupt();
	bool IsInInterrupt();
	int GetInterruptPos();
	bool IsInterruptTimeOut(UINT TimeOut);
	void SetInterruptCode(UINT Code);
	UINT GetInterruptCode();

	void SetResult(const ES_BOLAN& Result);
	void SetResultCode(int ResultCode);
	ES_BOLAN& GetResult();
	int GetResultCode();
	void SetLastLine(int Line);
	int GetLastLine();
	
	void Reset();

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
inline CESVariableList * CESThread::GetLocalVariableList()
{
	return &m_LocalVariableList;
}

inline ES_VARIABLE * CESThread::FindVariable(UINT VarID)
{
	if(VarID>LOCAL_VARIABLE_ID_START)
	{
		return m_LocalVariableList.FindVariable(VarID);
	}
	else
	{
		return m_pVariableList->FindVariable(VarID);
	}
}
inline ES_VARIABLE * CESThread::FindVariable(LPCTSTR VarName)
{
	ES_VARIABLE * pVar=NULL;	
	pVar=m_LocalVariableList.FindVariable(VarName);
	if(pVar==NULL&&m_pVariableList)
	{
		pVar=m_pVariableList->FindVariable(VarName);
	}
	return pVar;
}
inline void CESThread::ClearLocalVariable()
{
	m_LocalVariableList.Clear();
}

inline void CESThread::SetFunctionList(CESFunctionList * pFunctionList)
{
	m_pFunctionList=pFunctionList;
}
inline CESFunctionList * CESThread::GetFunctionList()
{
	return m_pFunctionList;
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
inline void CESThread::ClearStack()
{
	m_Stack.Clear();
}
inline bool CESThread::PushValueToStack(const ES_BOLAN& Value)
{
	return m_Stack.PushValue(&Value);
}
inline void CESThread::PushCallStack(UINT Pos)
{
	m_CallStack.Add(Pos);
}
inline int CESThread::PopCallStack()
{
	if(m_CallStack.GetCount())
	{
		int Pos=m_CallStack[m_CallStack.GetCount()-1];
		m_CallStack.Delete(m_CallStack.GetCount()-1);
		return Pos;
	}
	return -1;
}
inline void CESThread::ClearCallStack()
{
	m_CallStack.Empty();
}
inline void CESThread::SetInterruptPos(int Pos)
{
	m_IsInInterrupt=true;
	m_InterruptPos=Pos;	
	m_InterruptTimer.SaveTime();
}
inline void CESThread::ClearInterrupt()
{
	m_IsInInterrupt=false;
	m_InterruptPos=-1;	
}
inline bool CESThread::IsInInterrupt()
{
	return m_IsInInterrupt;
}
inline int CESThread::GetInterruptPos()
{
	return m_InterruptPos;
}
inline bool CESThread::IsInterruptTimeOut(UINT TimeOut)
{
	if(m_InterruptTimer.IsTimeOut(TimeOut))
		return true;
	return false;
}
inline void CESThread::SetInterruptCode(UINT Code)
{
	m_InterrupCode=Code;
}
inline UINT CESThread::GetInterruptCode()
{
	return m_InterrupCode;
}

inline void CESThread::SetResult(const ES_BOLAN& Result)
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

inline void CESThread::Reset()
{
	ClearStack();
	ClearCallStack();
	ClearInterrupt();
	ClearLocalVariable();
}
inline int CESThread::PushScript(const char * szExpStr)
{
	if(m_pVariableList==NULL)
	{
		m_ResultCode=6001;
		return m_ResultCode;
	}
	if(m_pFunctionList==NULL)
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
	m_ResultCode=m_pScript->PushScript(szExpStr,m_pVariableList,m_pFunctionList,m_LastLine);
	return m_ResultCode;
}