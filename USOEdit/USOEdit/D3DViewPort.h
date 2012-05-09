/****************************************************************************/
/*                                                                          */
/*      文件名:    D3DViewPort.h                                            */
/*      创建日期:  2011年10月07日                                           */
/*      作者:      Sagasarate                                               */
/*                                                                          */
/*      本软件版权归Sagasarate(sagasarate@sina.com)所有                     */
/*      你可以将本软件用于任何商业和非商业软件开发，但                      */
/*      必须保留此版权声明                                                  */
/*                                                                          */
/****************************************************************************/
/****************************************************************************/
/*                                                                          */
/*      文件名:    D3DViewPort.h                                            */
/*      创建日期:  2010年02月09日                                           */
/*      作者:      Sagasarate                                               */
/*                                                                          */
/*      本软件版权归Sagasarate(sagasarate@sina.com)所有                     */
/*      你可以将本软件用于任何商业和非商业软件开发，但                      */
/*      必须保留此版权声明                                                  */
/*                                                                          */
/****************************************************************************/
#pragma once


// CD3DViewPort
class CUSOEditDoc;

class CD3DViewPort : public CWnd
{
	DECLARE_DYNAMIC(CD3DViewPort)

public:
	CD3DViewPort();
	virtual ~CD3DViewPort();

	BOOL Create(CWnd * pParent);
	

protected:
	DECLARE_MESSAGE_MAP()

protected:
	enum OBJECT_TRANS_TYPE
	{
		OBJECT_TRANS_NONE,
		OBJECT_TRANS_MODE1,
		OBJECT_TRANS_MODE2,
	};
	CD3DDevice *			m_pD3DDevice;
	CD3DUIRender *			m_pUIRender;
	CD3DSceneRender *		m_pRender;
	CD3DGUIObjectCreator *	m_pObjectCreator;
	CD3DGUI *				m_pGUI;
	CD3DRenderThread *		m_pRenderThread;

	int					m_StartX,m_StartY;		
	FLOAT				m_CameraXTransform;
	FLOAT				m_CameraYTransform;
	FLOAT				m_CameraZTransform;
	FLOAT				m_CameraRotationX;
	FLOAT				m_CameraRotationY;
	bool				m_IsRotation;
	bool				m_IsTransform;
	int					m_ObjectTransMode;	
	bool				m_HaveTrans;
	
	CD3DObject *		m_pSelectedObject;

	DWORD				m_UpdateStartTime;

	CEasyTimer			m_FrameRateTimer;
	int					m_FrameCount;
	FLOAT				m_FrameRate;

	CD3DLight *			m_pSystemLight;
	CD3DCamera *		m_pSystemCamera;

	CEasyTimer			m_UpdateControlTimer;
	
	
public:	

	

	void Update();	
	
	virtual BOOL PreTranslateMessage(MSG* pMsg);

	CD3DGUI * GetGUI()
	{
		return m_pGUI;
	}

	CD3DSceneRender * GetRender()
	{
		return m_pRender;
	}
	CD3DRenderThread * GetRenderThread()
	{
		return m_pRenderThread;
	}
	
	CD3DDevice * GetDevice()
	{
		return m_pD3DDevice;
	}
	CD3DLight * GetSystemLight()
	{
		return m_pSystemLight;
	}
	CD3DCamera * GetSystemCamera()
	{
		return m_pSystemCamera;
	}
	void SetSelectedObject(CD3DObject * pObject)
	{
		m_pSelectedObject=pObject;
	}
	CD3DObject * GetSelectObject()
	{
		return m_pSelectedObject;
	}

	CUSOEditDoc * GetDocument();	
	
	void ApplyCameraChange(bool IsReset);
	
	FLOAT GetFrameRate()
	{
		return m_pRenderThread->GetFrameRate();
	}	

	FLOAT GetUpdateRate()
	{
		return m_FrameRate;
	}

	UINT GetRenderObjectCount()
	{
		return m_pRenderThread->GetRenderObjectCount(0);
	}

	UINT GetRenderSubMeshCount()
	{
		return m_pRenderThread->GetRenderSubMeshCount(0);
	}

	UINT GetRenderFaceCount()
	{
		return m_pRenderThread->GetRenderFaceCount(0);
	}

	UINT GetRenderVertexCount()
	{
		return m_pRenderThread->GetRenderVertexCount(0);
	}
	void GetCameraPos(float& PosX,float& PosY,float& PosZ)
	{
		PosX=m_CameraXTransform;
		PosY=m_CameraYTransform;
		PosZ=m_CameraZTransform;
	}
	void SetCameraPos(float PosX,float PosY,float PosZ)
	{
		m_CameraXTransform=PosX;
		m_CameraYTransform=PosY;
		m_CameraZTransform=PosZ;
	}
	void GetCameraRotate(float& RotateX,float& RotateY)
	{
		RotateX=m_CameraRotationX;
		RotateY=m_CameraRotationY;
	}
	void SetCameraRotate(float RotateX,float RotateY)
	{
		m_CameraRotationX=RotateX;
		m_CameraRotationY=RotateY;
	}

	void MoveCamera(FLOAT X,FLOAT Y,FLOAT Z,FLOAT RotateX,FLOAT RotateY);
	void SaveDepthTexture(LPCTSTR szFileName);

	void UpdateCamera();

protected:
	
	
	CD3DBoundingBox GetSceneBoundingBox(CUSOObject * pSceneRoot);

public:
	afx_msg void OnKillFocus(CWnd* pNewWnd);
	afx_msg void OnSetFocus(CWnd* pOldWnd);
};


