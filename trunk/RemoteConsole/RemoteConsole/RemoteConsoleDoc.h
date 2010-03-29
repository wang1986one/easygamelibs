/****************************************************************************/
/*                                                                          */
/*      文件名:    RemoteConsoleDoc.h                                       */
/*      创建日期:  2009年02月09日                                           */
/*      作者:      Sagasarate                                               */
/*                                                                          */
/*      本软件版权归Sagasarate(sagasarate@sina.com)所有                     */
/*      你可以将本软件用于任何商业和非商业软件开发，但                      */
/*      必须保留此版权声明                                                  */
/*                                                                          */
/****************************************************************************/
// RemoteConsoleDoc.h :  CRemoteConsoleDoc 类的接口
//


#pragma once

class CRemoteConsoleDoc : public CDocument
{
protected: // 仅从序列化创建
	CRemoteConsoleDoc();
	DECLARE_DYNCREATE(CRemoteConsoleDoc)

// 属性
public:

// 操作
public:

// 重写
	public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);

// 实现
public:
	virtual ~CRemoteConsoleDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// 生成的消息映射函数
protected:
	DECLARE_MESSAGE_MAP()
};


