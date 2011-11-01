#include "stdafx.h"


namespace D3DLib{

LPCTSTR SHARED_LIB_FX=
#include "Shaders\SharedLib.fxt"
;

LPCTSTR SHARED_PARAM_FX=
#include "Shaders\SharedParam.fxt"
;


LPCTSTR M2_MODEL_FX=
#include "Shaders/M2ModelSoftBoneOnly.fxt"
;

LPCTSTR WMO_MODEL_FX=
#include "Shaders/WMOModel.fxt"
;

LPCTSTR ADT_MODEL_20_FX=
#include "Shaders/ADTModel20.fxt"
;

LPCTSTR ADT_MODEL_30_FX=
#include "Shaders/ADTModel30.fxt"
;

LPCTSTR M2_PARTICLE_FX=
#include "Shaders/M2Particle.fxt"
;

LPCTSTR M2_RIBBON_FX=
#include "Shaders/M2Ribbon.fxt"
;

LPCTSTR ADT_LIQUID_FX=
#include "Shaders/ADTLiquid.fxt"
;

LPCTSTR WDL_MODEL_FX=
#include "Shaders/WDLModel.fxt"
;


LPCTSTR DEFAULT_UI_WIN_RECT_FX_NT=
#include "Shaders/UIWinRectNoTexture.fxt"
;
LPCTSTR DEFAULT_UI_WIN_RECT_FX_TF=
#include "Shaders/UIWinRectWithTextureFilter.fxt"
;
LPCTSTR DEFAULT_UI_WIN_RECT_FX=
#include "Shaders/UIWinRectWithTexture.fxt"
;

LPCTSTR INVERSE_COLOR_FX=
#include "Shaders/UIInverseColor.fxt"
;

LPCTSTR TEXT_RECT_FX=
#include "Shaders/UITextRect.fxt"
;

LPCTSTR DEFAULT_BOARD_FX_NT=
#include "Shaders/D3DBoard.fxt"
;


LPCTSTR DEFAULT_LINE_FX_NT=
#include "Shaders/D3DLine.fxt"
;

LPCTSTR DEFAULT_BOUND_FRAME_FX_NT=
#include "Shaders/D3DBoundFrame.fxt"
;

}