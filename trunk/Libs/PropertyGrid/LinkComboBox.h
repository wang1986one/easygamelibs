/****************************************************************************/
/*                                                                          */
/*      �ļ���:    LinkComboBox.h                                           */
/*      ��������:  2009��07��06��                                           */
/*      ����:      Sagasarate                                               */
/*                                                                          */
/*      �������Ȩ��Sagasarate(sagasarate@sina.com)����                     */
/*      ����Խ�����������κ���ҵ�ͷ���ҵ�����������                      */
/*      ���뱣���˰�Ȩ����                                                  */
/*                                                                          */
/****************************************************************************/
#pragma once

namespace PropertyGrid
{
class CLinkComboBox : public CComboBox
{
	DECLARE_DYNAMIC(CLinkComboBox)
protected:
	
public:
	CLinkComboBox();
	virtual ~CLinkComboBox();	
protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnKillFocus(CWnd* pNewWnd);
};
}