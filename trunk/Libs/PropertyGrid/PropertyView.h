/****************************************************************************/
/*                                                                          */
/*      �ļ���:    PropertyView.h                                           */
/*      ��������:  2009��07��06��                                           */
/*      ����:      Sagasarate                                               */
/*                                                                          */
/*      �������Ȩ��Sagasarate(sagasarate@sina.com)����                     */
/*      ����Խ�����������κ���ҵ�ͷ���ҵ�����������                      */
/*      ���뱣���˰�Ȩ����                                                  */
/*                                                                          */
/****************************************************************************/
#pragma once


// CPropertyView ��ͼ
namespace PropertyGrid
{

class CPropertyView : public CView
{
	DECLARE_DYNCREATE(CPropertyView)

protected:
	CPropertyView();           // ��̬������ʹ�õ��ܱ����Ĺ��캯��
	virtual ~CPropertyView();

	CPropertyGrid m_PropertyGrid;

public:
	virtual void OnDraw(CDC* pDC);      // ��д�Ի��ƴ���ͼ
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:
	DECLARE_MESSAGE_MAP()

	virtual afx_msg LRESULT OnPropertyChanged( WPARAM wParam, LPARAM lParam);
public:
	virtual afx_msg void OnSize(UINT nType, int cx, int cy);
	virtual void OnInitialUpdate();

	CPropertyGrid& GetPropertyCtrl()
	{
		return m_PropertyGrid;
	}
};


}