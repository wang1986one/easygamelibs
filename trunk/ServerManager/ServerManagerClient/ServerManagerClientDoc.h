/****************************************************************************/
/*                                                                          */
/*      �ļ���:    ServerManagerClientDoc.h                                 */
/*      ��������:  2010��02��09��                                           */
/*      ����:      Sagasarate                                               */
/*                                                                          */
/*      �������Ȩ��Sagasarate(sagasarate@sina.com)����                     */
/*      ����Խ�����������κ���ҵ�ͷ���ҵ�����������                      */
/*      ���뱣���˰�Ȩ����                                                  */
/*                                                                          */
/****************************************************************************/
// ServerManagerClientDoc.h : CServerManagerClientDoc ��Ľӿ�
//


#pragma once


class CServerManagerClientDoc : public CDocument
{
protected: // �������л�����
	CServerManagerClientDoc();
	DECLARE_DYNCREATE(CServerManagerClientDoc)

// ����
public:

// ����
public:

// ��д
public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);

// ʵ��
public:
	virtual ~CServerManagerClientDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// ���ɵ���Ϣӳ�亯��
protected:
	DECLARE_MESSAGE_MAP()
};


