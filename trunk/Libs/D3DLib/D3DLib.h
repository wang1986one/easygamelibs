/****************************************************************************/
/*                                                                          */
/*      �ļ���:    D3DLib.h                                                 */
/*      ��������:  2010��02��09��                                           */
/*      ����:      Sagasarate                                               */
/*                                                                          */
/*      ��������Ȩ��Sagasarate(sagasarate@sina.com)����                     */
/*      ����Խ������������κ���ҵ�ͷ���ҵ������������                      */
/*      ���뱣���˰�Ȩ����                                                  */
/*                                                                          */
/****************************************************************************/
#pragma once

#define D3DLIB_VERSION	"1.0.0.0"

#if defined(USE_CRT_DETAIL_NEW) && defined(_DEBUG)
#undef new
#endif

#include "D3D9.h"
#include "D3DX9.h"

#if defined(USE_CRT_DETAIL_NEW) && defined(_DEBUG)
#define new new( __FILE__, __LINE__ )
#endif

#include <map>
#include <algorithm>

#include "D3DVector2.h"
#include "D3DVector3.h"
#include "D3DVector4.h"
#include "D3DQuaternion.h"
#include "D3DMatrix.h"
#include "D3DPlane.h"
#include "D3DBoundingBox.h"
#include "D3DBoundingSphere.h"
#include "D3DFrustum.h"

#include "Types.h"

#include "BLZFileDefines.h"

#include "D3DDevice.h"

#include "D3DFX.h"
#include "D3DFXManager.h"

#include "D3DTexture.h"
#include "D3DTextTexture.h"
#include "D3DIFLTexture.h"
#include "D3DTextureManager.h"

#include "D3DSubMeshMaterial.h"
#include "D3DSubMesh.h"

#include "D3DRender.h"
#include "D3DSortedRender.h"

#include "D3DObject.h"

#include "D3DObjectResource.h"
#include "D3DObjectResourceManager.h"

#include "D3DCamera.h"
#include "D3DLight.h"

#include "D3DScene.h"

#include "D3DBoundingFrame.h"
#include "D3DDummy.h"

#include "D3DBaseStaticModel.h"
#include "D3DStaticModelResource.h"
#include "D3DStaticModel.h"

#include "D3DBaseDynamicModel.h"

#include "D3DBonedAniResource.h"
#include "D3DBonedModelResource.h"
#include "D3DBonedDynamicModel.h"

#include "D3DFrameDynamicModel.h"

#include "BLZWOWDatabase.h"
#include "BLZChunkFile.h"
#include "D3DWOWM2ModelResource.h"
#include "D3DWOWM2BillBoardParticleEmitter.h"
#include "D3DWOWM2RibbonEmitter.h"
#include "D3DWOWM2Model.h"
#include "D3DWOWM2CharacterModel.h"
#include "D3DWOWM2ItemModel.h"

#include "D3DWOWWMOModelResource.h"
#include "D3DWOWWMOModel.h"

#include "D3DWOWADTModelResource.h"
#include "D3DWOWADTModel.h"

#include "D3DBaseFont.h"
#include "D3DDX9Font.h"
#include "D3DFontCharClip.h"
#include "D3DFontCN.h"
#include "D3DFontManager.h"

#include "USOD3DObjectCreateFilter.h"



#pragma comment(lib,"dxguid.lib")
#pragma comment(lib,"d3dx9.lib")
#pragma comment(lib,"d3d9.lib")




using namespace D3DLib;