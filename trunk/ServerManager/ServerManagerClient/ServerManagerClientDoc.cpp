/****************************************************************************/
/*                                                                          */
/*      �ļ���:    ServerManagerClientDoc.cpp                               */
/*      ��������:  2010��02��09��                                           */
/*      ����:      Sagasarate                                               */
/*                                                                          */
/*      �������Ȩ��Sagasarate(sagasarate@sina.com)����                     */
/*      ����Խ�����������κ���ҵ�ͷ���ҵ�����������                      */
/*      ���뱣���˰�Ȩ����                                                  */
/*                                                                          */
/****************************************************************************/
// ServerManagerClientDoc.cpp : CServerManagerClientDoc ���ʵ��
//

#include "stdafx.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CServerManagerClientDoc

IMPLEMENT_DYNCREATE(CServerManagerClientDoc, CDocument)

BEGIN_MESSAGE_MAP(CServerManagerClientDoc, CDocument)
END_MESSAGE_MAP()


// CServerManagerClientDoc ����/����

CServerManagerClientDoc::CServerManagerClientDoc()
{
	// TODO: �ڴ����һ���Թ������

}

CServerManagerClientDoc::~CServerManagerClientDoc()
{
}

BOOL CServerManagerClientDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: �ڴ�������³�ʼ������
	// (SDI �ĵ������ø��ĵ�)

	return TRUE;
}




// CServerManagerClientDoc ���л�

void CServerManagerClientDoc::Serialize(CArchive& ar)
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


// CServerManagerClientDoc ���

#ifdef _DEBUG
void CServerManagerClientDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CServerManagerClientDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG


// CServerManagerClientDoc ����
