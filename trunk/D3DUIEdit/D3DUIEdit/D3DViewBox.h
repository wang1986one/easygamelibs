/****************************************************************************/
/*                                                                          */
/*      �ļ���:    D3DViewBox.h                                             */
/*      ��������:  2011��10��07��                                           */
/*      ����:      Sagasarate                                               */
/*                                                                          */
/*      �������Ȩ��Sagasarate(sagasarate@sina.com)����                     */
/*      ����Խ�����������κ���ҵ�ͷ���ҵ�����������                      */
/*      ���뱣���˰�Ȩ����                                                  */
/*                                                                          */
/****************************************************************************/
#pragma once



// CD3DViewBox ��ͼ

class CD3DViewBox : public CScrollView
{
	DECLARE_DYNCREATE(CD3DViewBox)

protected:
	CD3DViewBox();           // ��̬������ʹ�õ��ܱ����Ĺ��캯��
	virtual ~CD3DViewBox();

public:
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:
	CWnd  m_ContentWnd;
	CD3DDevice *		m_pD3DDevice;
	CD3DUIRender *		m_pRender;
	CD3DGUIObjectCreator *	m_pObjectCreator;
	CD3DGUI *			m_pGUI;
	

	virtual void OnDraw(CDC* pDC);      // ��д�Ի��ƴ���ͼ
	virtual void OnInitialUpdate();     // �����ĵ�һ��

	DECLARE_MESSAGE_MAP()
public:
	afx_msg int OnMouseActivate(CWnd* pDesktopWnd, UINT nHitTest, UINT message);
	afx_msg void OnTimer(UINT nIDEvent);

	void Init()
	{
		OnInitialUpdate();
	}

	CD3DGUIObjectCreator * GetObjectCreator()
	{
		return m_pObjectCreator;
	}
	CD3DDevice * GetDevice()
	{
		return m_pD3DDevice;
	}
	CD3DGUI * GetGUI()
	{
		return m_pGUI;
	}
	void SetContentSize(int Width,int Height);
	virtual BOOL PreTranslateMessage(MSG* pMsg);
};


