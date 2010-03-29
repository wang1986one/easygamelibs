/****************************************************************************/
/*                                                                          */
/*      文件名:    RemoteConsoleDoc.cpp                                     */
/*      创建日期:  2009年02月09日                                           */
/*      作者:      Sagasarate                                               */
/*                                                                          */
/*      本软件版权归Sagasarate(sagasarate@sina.com)所有                     */
/*      你可以将本软件用于任何商业和非商业软件开发，但                      */
/*      必须保留此版权声明                                                  */
/*                                                                          */
/****************************************************************************/
// RemoteConsoleDoc.cpp :  CRemoteConsoleDoc 类的实现
//

#include "stdafx.h"
#include "RemoteConsole.h"

#include "RemoteConsoleDoc.h"




// CRemoteConsoleDoc

IMPLEMENT_DYNCREATE(CRemoteConsoleDoc, CDocument)

BEGIN_MESSAGE_MAP(CRemoteConsoleDoc, CDocument)
END_MESSAGE_MAP()


// CRemoteConsoleDoc 构造/析构

CRemoteConsoleDoc::CRemoteConsoleDoc()
{
	// TODO: 在此添加一次性构造代码

}

CRemoteConsoleDoc::~CRemoteConsoleDoc()
{
}

BOOL CRemoteConsoleDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: 在此添加重新初始化代码
	// (SDI 文档将重用该文档)

	return TRUE;
}




// CRemoteConsoleDoc 序列化

void CRemoteConsoleDoc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		// TODO: 在此添加存储代码
	}
	else
	{
		// TODO: 在此添加加载代码
	}
}


// CRemoteConsoleDoc 诊断

#ifdef _DEBUG
void CRemoteConsoleDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CRemoteConsoleDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG


// CRemoteConsoleDoc 命令
