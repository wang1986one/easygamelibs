/****************************************************************************/
/*                                                                          */
/*      文件名:    DlgHeightTest.h                                          */
/*      创建日期:  2011年10月07日                                           */
/*      作者:      Sagasarate                                               */
/*                                                                          */
/*      本软件版权归Sagasarate(sagasarate@sina.com)所有                     */
/*      你可以将本软件用于任何商业和非商业软件开发，但                      */
/*      必须保留此版权声明                                                  */
/*                                                                          */
/****************************************************************************/
#pragma once


// CDlgHeightTest 对话框

class CDlgHeightTest : public CDialog
{
	DECLARE_DYNAMIC(CDlgHeightTest)
protected:
	CD3DViewPort *		m_pViewPort;
public:
	CDlgHeightTest(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CDlgHeightTest();

	void SetView(CD3DViewPort * pViewPort)
	{
		m_pViewPort=pViewPort;
	}

// 对话框数据
	enum { IDD = IDD_DIALOG_HEIGHT_TEST };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	float m_PosX;
	float m_PosZ;
	float m_PosY;
	float m_Height;
	afx_msg void OnBnClickedDoTest();
};
