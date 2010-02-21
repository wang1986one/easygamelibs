#pragma once

namespace D3DLib{


#define MAX_2D_VERTICES			50*6

// Font creation flags
#define D3DFONT_BOLD        0x0001
#define D3DFONT_ITALIC      0x0002
#define D3DFONT_ZENABLE     0x0004

// Font rendering flags
#define TEXT_ALIGN_LEFT		1
#define TEXT_ALIGN_RIGHT	2
#define TEXT_ALIGN_TOP		4
#define TEXT_ALIGN_BOTTOM	8
#define TEXT_ALIGN_XCENTER	0
#define TEXT_ALIGN_YCENTER	0

#define MAX_TEXTURE_STORAGE	4096
#define MAX_FX_STORAGE		1024
#define MAX_FONT_STORAGE	1024

#define MAX_TEXTURE_LAYER	8
#define MAX_LIGHT			8
#define MAX_BONE_NUM		70

#define MAX_HEIGHT			1.0e10f

enum MAX_VERTEX_BONE
{
	MAX_VERTEX_BONE_BIND=4,
};


struct RECTVERTEX { D3DXVECTOR4 p;   DWORD color;     FLOAT tu, tv; };


#define D3DFVF_RECTVERTEX (D3DFVF_XYZRHW|D3DFVF_DIFFUSE|D3DFVF_TEX1)

inline RECTVERTEX InitRECTVertex( const D3DXVECTOR4& p, D3DCOLOR color,
                                      FLOAT tu, FLOAT tv )
{
    RECTVERTEX v;   v.p = p;   v.color = color;   v.tu = tu;   v.tv = tv;
    return v;
}

struct D3D_A8B8G8R8_PIXEL
{		
	BYTE Blue;
	BYTE Green;
	BYTE Red;
	BYTE Alpha;	
};

const LPCTSTR DEFAULT_MODEL_FX=
"texture TexLay0 < string name = \"test.jpg\"; >;		\r\n"
"texture TexLay1 < string name = \"test1.jpg\"; >;		\r\n"
"technique tec0											\r\n"
"{														\r\n"
"    pass p0											\r\n"
"    {													\r\n"
"		MultiSampleAntialias = FALSE;					\r\n"	
"		SpecularEnable = false;							\r\n"
"		zenable = true;									\r\n"
"		zwriteenable = true;							\r\n"
"		CullMode = ccw;									\r\n"
"		fogenable = false;								\r\n"
"		lighting = true;								\r\n"
"		Texture[0] = <TexLay0>;							\r\n"
"		AlphaTestEnable = false;						\r\n"
"		AlphaBlendEnable = true;						\r\n"
"		SrcBlend = SrcAlpha;							\r\n"
"		DestBlend = InvSrcAlpha;						\r\n"
"		Texture[0] = <TexLay0>;							\r\n"
"     	ColorOp[0] = Modulate;							\r\n"
"       ColorArg1[0] = Texture;							\r\n"
"       ColorArg2[0] = Diffuse;							\r\n"      	
"       AlphaOp[0] = Modulate;							\r\n"
"       AlphaArg1[0] = Texture;							\r\n"
"       AlphaArg2[0] = diffuse;							\r\n"
"		ColorOp[1] = disable;							\r\n"
"		AlphaOp[1] = disable;							\r\n"
"		AddressU[0] = wrap;								\r\n"
"		AddressV[0] = wrap;								\r\n"      
"		MinFilter[0] = linear;							\r\n"
"       MagFilter[0] = linear;							\r\n"
"       MipFilter[0] = linear;							\r\n"
"		VertexShader = NULL;							\r\n"
"		PixelShader  = NULL;							\r\n"
"    }"
"}";

const LPCTSTR DEFAULT_MODEL_FX_NT=
"texture TexLay0 < string name = \"test.jpg\"; >;		\r\n"
"texture TexLay1 < string name = \"test1.jpg\"; >;		\r\n"
"technique tec0											\r\n"
"{														\r\n"
"    pass p0											\r\n"
"    {													\r\n"
"		MultiSampleAntialias = FALSE;					\r\n"	
"		SpecularEnable = false;							\r\n"
"		zenable = true;									\r\n"
"		zwriteenable = true;							\r\n"
"		lighting = true;								\r\n"
"		CullMode = none;								\r\n"
"		FillMode = WireFrame;							\r\n"
"		fogenable = false;								\r\n"
"		Texture[0] = <TexLay0>;							\r\n"
"		AlphaTestEnable = false;						\r\n"
"		AlphaBlendEnable = false;						\r\n"
"		SrcBlend = SrcAlpha;							\r\n"
"		DestBlend = InvSrcAlpha;						\r\n"
"     	ColorOp[0] = SelectArg1;						\r\n"
"       ColorArg1[0] = Diffuse;							\r\n"      	
"       AlphaOp[0] = SelectArg1;						\r\n"
"       AlphaArg1[0] = diffuse;							\r\n"
"		ColorOp[1] = disable;							\r\n"
"		AlphaOp[1] = disable;							\r\n"
"		VertexShader = NULL;							\r\n"
"		PixelShader  = NULL;							\r\n"
"    }"
"}";

const LPCTSTR DEFAULT_MODEL_FX_NT_WIRE_FRAME=
"texture TexLay0 < string name = \"test.jpg\"; >;		\r\n"
"texture TexLay1 < string name = \"test1.jpg\"; >;		\r\n"
"technique tec0											\r\n"
"{														\r\n"
"    pass p0											\r\n"
"    {													\r\n"
"		MultiSampleAntialias = FALSE;					\r\n"	
"		SpecularEnable = false;							\r\n"
"		zenable = true;									\r\n"
"		zwriteenable = true;							\r\n"
"		FillMode = WireFrame;							\r\n"
"		CullMode = ccw;									\r\n"
"		fogenable = false;								\r\n"
"		Texture[0] = <TexLay0>;							\r\n"
"		AlphaTestEnable = false;						\r\n"
"		AlphaBlendEnable = false;						\r\n"
"		SrcBlend = SrcAlpha;							\r\n"
"		DestBlend = InvSrcAlpha;						\r\n"
"     	ColorOp[0] = SelectArg1;						\r\n"
"       ColorArg1[0] = Diffuse;							\r\n"      	
"       AlphaOp[0] = SelectArg1;						\r\n"
"       AlphaArg1[0] = diffuse;							\r\n"
"		ColorOp[1] = disable;							\r\n"
"		AlphaOp[1] = disable;							\r\n"
"		VertexShader = NULL;							\r\n"
"		PixelShader  = NULL;							\r\n"
"    }"
"}";

}

