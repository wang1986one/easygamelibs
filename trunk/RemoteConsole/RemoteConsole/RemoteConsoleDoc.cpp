/****************************************************************************/
/*                                                                          */
/*      �ļ���:    RemoteConsoleDoc.cpp                                     */
/*      ��������:  2009��02��09��                                           */
/*      ����:      Sagasarate                                               */
/*                                                                          */
/*      �������Ȩ��Sagasarate(sagasarate@sina.com)����                     */
/*      ����Խ�����������κ���ҵ�ͷ���ҵ�����������                      */
/*      ���뱣���˰�Ȩ����                                                  */
/*                                                                          */
/****************************************************************************/
// RemoteConsoleDoc.cpp :  CRemoteConsoleDoc ���ʵ��
//

#include "stdafx.h"
#include "RemoteConsole.h"

#include "RemoteConsoleDoc.h"




// CRemoteConsoleDoc

IMPLEMENT_DYNCREATE(CRemoteConsoleDoc, CDocument)

BEGIN_MESSAGE_MAP(CRemoteConsoleDoc, CDocument)
END_MESSAGE_MAP()


// CRemoteConsoleDoc ����/����

CRemoteConsoleDoc::CRemoteConsoleDoc()
{
	// TODO: �ڴ����һ���Թ������

}

CRemoteConsoleDoc::~CRemoteConsoleDoc()
{
}

BOOL CRemoteConsoleDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: �ڴ�������³�ʼ������
	// (SDI �ĵ������ø��ĵ�)

	return TRUE;
}




// CRemoteConsoleDoc ���л�

void CRemoteConsoleDoc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		// TODO: �ڴ���Ӵ洢����
	}
	else
	{
		// TODO: �ڴ���Ӽ��ش���
	}
}


// CRemoteConsoleDoc ���

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


// CRemoteConsoleDoc ����
