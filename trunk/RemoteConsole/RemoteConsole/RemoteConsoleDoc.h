// RemoteConsoleDoc.h :  CRemoteConsoleDoc ��Ľӿ�
//


#pragma once

class CRemoteConsoleDoc : public CDocument
{
protected: // �������л�����
	CRemoteConsoleDoc();
	DECLARE_DYNCREATE(CRemoteConsoleDoc)

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
	virtual ~CRemoteConsoleDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// ���ɵ���Ϣӳ�亯��
protected:
	DECLARE_MESSAGE_MAP()
};


