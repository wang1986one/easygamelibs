/****************************************************************************/
/*                                                                          */
/*      �ļ���:    D3DGUIObjectCreator.h                                    */
/*      ��������:  2009��09��11��                                           */
/*      ����:      Sagasarate                                               */
/*                                                                          */
/*      �������Ȩ��Sagasarate(sagasarate@sina.com)����                     */
/*      ����Խ�����������κ���ҵ�ͷ���ҵ�����������                      */
/*      ���뱣���˰�Ȩ����                                                  */
/*                                                                          */
/****************************************************************************/
#pragma once
#include "D3DGUIWndRect.h"
#include "D3DGUITexture.h"
#include "D3DGUITextRect.h"

namespace D3DGUI{

class CD3DGUIObjectCreator :
	public CNameObject,public IUIObjectCreator
{
protected:
	CD3DDevice *	m_pDevice;
	CD3DUIRender *	m_pRender;

	DECLARE_CLASS_INFO_STATIC(CD3DGUIObjectCreator)
public:
	CD3DGUIObjectCreator(CD3DUIRender *	pRender);
	virtual ~CD3DGUIObjectCreator(void);

	virtual IUIWndRect * CreateWndRect(FLOAT_RECT * pRect);
	virtual IUITextRect * CreateTextRect(FLOAT_RECT * pRect,const LOGFONT* pLogFont);
	virtual IUITexture * CreateTextureFromFile(LPCTSTR FileName);
	virtual void SetTextureDir(LPCTSTR Dir);

	virtual void Release()
	{
		CNameObject::Release();
	}
};

}