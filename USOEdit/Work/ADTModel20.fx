static const int MAX_LIGHT_COUNT = 4;

int		LightCount=0;
int		LightType[MAX_LIGHT_COUNT];
float3	LightPos[MAX_LIGHT_COUNT];
float3	LightDir[MAX_LIGHT_COUNT];
float4	LightAmbient[MAX_LIGHT_COUNT];
float4	LightDiffuse[MAX_LIGHT_COUNT];
float4	LightSpecular[MAX_LIGHT_COUNT];
float	LightRange[MAX_LIGHT_COUNT];
float	LightAtn0[MAX_LIGHT_COUNT];
float	LightAtn1[MAX_LIGHT_COUNT];
float	LightAtn2[MAX_LIGHT_COUNT];
float	LightTheta[MAX_LIGHT_COUNT];
float	LightPhi[MAX_LIGHT_COUNT];
float	LightFalloff[MAX_LIGHT_COUNT];


float4 MaterialAmbient = {1.0f, 1.0f, 1.0f, 1.0f};
float4 MaterialDiffuse = {1.0f, 1.0f, 1.0f, 1.0f};
float4 MaterialSpecular = {1.0f, 1.0f, 1.0f, 1.0f};
float4 MaterialEmissive = {0.0f, 0.0f, 0.0f, 1.0f};
float  MaterialPower=1.0f;

float4x4		WorldMatrix;
float4x4		WorldViewMatrix;
shared float4x4		PrjMatrix;

shared float3		CameraPos;
shared float		CameraNear;
shared float		CameraFar;

shared float		FogNear=30.0f;
shared float		FogFar=300.0f;
shared float3		FogColor={0.25f, 0.25f, 0.5f};

static const float TexRatio=8.0f;
static const float AlphaTexRatio=1.0f;


texture TexLay0 ;
texture TexLay1 ;
texture TexLay2 ;
texture TexLay3 ;
texture TexLay4 ;
texture TexLay5 ;
texture TexLay6 ;
texture TexShadow ;


sampler Sampler0 =
sampler_state
{
    Texture = <TexLay0>;
    MinFilter = Linear;
    MagFilter = Linear;
    MipFilter = Linear;
    AddressU = Wrap;
    AddressV = Wrap;

};

sampler Sampler1 =
sampler_state
{
    Texture = <TexLay1>;
    MinFilter = Linear;
    MagFilter = Linear;
    MipFilter = Linear;
    AddressU = Wrap;
    AddressV = Wrap;

};

sampler Sampler2 =
sampler_state
{
    Texture = <TexLay2>;
    MinFilter = Linear;
    MagFilter = Linear;
    MipFilter = Linear;
    AddressU = Clamp;
    AddressV = Clamp;

};

sampler Sampler3 =
sampler_state
{
    Texture = <TexLay3>;
    MinFilter = Linear;
    MagFilter = Linear;
    MipFilter = Linear;
    AddressU = Wrap;
    AddressV = Wrap;
};

sampler Sampler4 =
sampler_state
{
    Texture = <TexLay4>;
    MinFilter = Linear;
    MagFilter = Linear;
    MipFilter = Linear;
    AddressU = Clamp;
    AddressV = Clamp;
};

sampler Sampler5 =
sampler_state
{
    Texture = <TexLay5>;
    MinFilter = Linear;
    MagFilter = Linear;
    MipFilter = Linear;
    AddressU = Wrap;
    AddressV = Wrap;
};

sampler Sampler6 =
sampler_state
{
    Texture = <TexLay6>;
    MinFilter = Linear;
    MagFilter = Linear;
    MipFilter = Linear;
    AddressU = Clamp;
    AddressV = Clamp;
};

sampler SamplerShadow =
sampler_state
{
    Texture = <TexShadow>;
    MinFilter = Linear;
    MagFilter = Linear;
    MipFilter = Linear;
    AddressU = Clamp;
    AddressV = Clamp;
};

struct VS_INPUT
{
    float4  Pos             : POSITION;
    float4  Normal          : NORMAL;
    float4	Diffuse			: COLOR0;
    float2  Tex0			: TEXCOORD0;    
};

struct VS_OUTPUT
{
    float4  Pos			: POSITION;
    float4  Diffuse		: COLOR0; 
    float4  Specular	: COLOR1; 
    float4  Tex0		: TEXCOORD0;    
};

struct VS_OUTPUT_FOR_DEPTH
{
    float4  Pos			: POSITION;
    float4  Diffuse		: COLOR0; 
};

struct PS_INPUT
{
	float4	Diffuse		: COLOR0;
	float4	Specular	: COLOR1;
    float4	Tex0		: TEXCOORD0;
    
};

struct PS_OUTPUT
{
	float4	Color				: COLOR0;
	float4	ColorForDepthMap	: COLOR1;
};


float4 CaculateDiffuse(float3 Pos,float3 Normal)
{
	if(LightCount>0)
	{
		float4 Diffuse={0.0f,0.0f,0.0f,0.0f};
		for(int i=0;i<LightCount;i++)
		{
			if(LightType[i]==1)
			{
				float Dis=length(LightPos[i]-Pos);
				if(Dis<=LightRange[i])
				{
					float Atten = 1/( LightAtn0[i] + LightAtn1[i] * Dis + LightAtn2[i] * Dis * Dis);
					Diffuse += max(0,dot( Normal, normalize(LightPos[i]-Pos) )) * LightDiffuse[i] * MaterialDiffuse * Atten + 
						LightAmbient[i] * MaterialAmbient;
				}
			}
			else if(LightType[i]==3)
			{
				Diffuse += max(0,dot( Normal, -normalize( LightDir[i] ) )) * LightDiffuse[i] * MaterialDiffuse + 
					LightAmbient[i] * MaterialAmbient;
			}
		}
		Diffuse += MaterialEmissive;
		return Diffuse;
	}
	else
	{		
		return 1.0f;
	}	
}

float4 CaculateSpecular(float3 Pos,float3 Normal)
{
	if(LightCount>0)
	{
		float4 Specular={0.0f,0.0f,0.0f,0.0f};
		float3 EyeDir=Pos-CameraPos;
		for(int i=0;i<LightCount;i++)
		{
			if(LightType[i]==1)
			{
				float Dis=length(LightPos[i]-Pos);
				if(Dis<=LightRange[i])
				{
					float Atten = 1/( LightAtn0[i] + LightAtn1[i] * Dis + LightAtn2[i] * Dis * Dis);				
					float3 LightRelrect = normalize(reflect(Pos - LightPos[i],Normal));
					Specular += pow( max( 0,dot(LightRelrect,-normalize(EyeDir) ) ) ,MaterialPower/4 ) * LightSpecular[i] * MaterialSpecular * Atten;
				}				
			}
			else if(LightType[i]==3)
			{
				float3 LightRelrect = normalize(reflect(LightDir[i],Normal));
				Specular += pow( max( 0,dot(LightRelrect,-normalize(EyeDir) ) ) ,MaterialPower/4 )*LightSpecular[i]* MaterialSpecular;
			}
		}
		return Specular;
	}
	else
	{
		return 0.0f;
	}	
	
	
}

PS_OUTPUT PShaderOneLayer(PS_INPUT Input)
{	
	PS_OUTPUT Output;
	
	Output.Color=tex2D( Sampler0, Input.Tex0.xy*TexRatio );
    Output.Color=Output.Color*Input.Diffuse+Input.Specular*Output.Color.a;
	Output.Color.xyz=Output.Color.xyz*(1.0f-Input.Tex0.z)+FogColor*Input.Tex0.z;
	
	Output.ColorForDepthMap=Input.Tex0.w;
    return Output;
}

PS_OUTPUT PShaderOneLayerWithShadow(PS_INPUT Input)
{	
	PS_OUTPUT Output;
	
	Output.Color=tex2D( Sampler0, Input.Tex0.xy*TexRatio );
	float4 Shadow=tex2D( SamplerShadow, Input.Tex0.xy );
	Output.Color=Output.Color*Shadow.a;
    Output.Color=Output.Color*Input.Diffuse+Input.Specular*Output.Color.a;
	Output.Color.xyz=Output.Color.xyz*(1.0f-Input.Tex0.z)+FogColor*Input.Tex0.z;
	
    Output.ColorForDepthMap=Input.Tex0.w;
    return Output;
}

PS_OUTPUT PShaderTwoLayer(PS_INPUT Input)
{	
	PS_OUTPUT Output;
	
	Output.Color=tex2D( Sampler0, Input.Tex0.xy*TexRatio );
	
	float4 Color=tex2D( Sampler1, Input.Tex0.xy*TexRatio );
	float4 Alpha=tex2D( Sampler2, Input.Tex0.xy*AlphaTexRatio );	
	Output.Color=Output.Color*(1.0f-Alpha.a)+Color*Alpha.a;
	
	Output.Color=Output.Color*Input.Diffuse+Input.Specular*Output.Color.a;
	Output.Color.xyz=Output.Color.xyz*(1.0f-Input.Tex0.z)+FogColor*Input.Tex0.z;
	
    Output.ColorForDepthMap=Input.Tex0.w;
    return Output;
}

PS_OUTPUT PShaderTwoLayerWithShadow(PS_INPUT Input)
{	
	PS_OUTPUT Output;
	
	Output.Color=tex2D( Sampler0, Input.Tex0.xy*TexRatio );
	
	float4 Color=tex2D( Sampler1, Input.Tex0.xy*TexRatio );
	float4 Alpha=tex2D( Sampler2, Input.Tex0.xy*AlphaTexRatio );		
	Output.Color=Output.Color*(1.0f-Alpha.a)+Color*Alpha.a;
	
	float4 Shadow=tex2D( SamplerShadow, Input.Tex0.xy );
	Output.Color=Output.Color*Shadow.a;
	
	Output.Color=Output.Color*Input.Diffuse+Input.Specular*Output.Color.a;
	Output.Color.xyz=Output.Color.xyz*(1.0f-Input.Tex0.z)+FogColor*Input.Tex0.z;
	
	Output.ColorForDepthMap=Input.Tex0.w;
    return Output;
}

PS_OUTPUT PShaderThreeLayer(PS_INPUT Input)
{	
	PS_OUTPUT Output;
	
	Output.Color=tex2D( Sampler0, Input.Tex0.xy*TexRatio );
	
	float4 Color=tex2D( Sampler1, Input.Tex0.xy*TexRatio );
	float4 Alpha=tex2D( Sampler2, Input.Tex0.xy*AlphaTexRatio );	
	Output.Color=Output.Color*(1.0f-Alpha.a)+Color*Alpha.a;
	
	Color=tex2D( Sampler3, Input.Tex0.xy*TexRatio );
	Alpha=tex2D( Sampler4, Input.Tex0.xy*AlphaTexRatio );	
	Output.Color=Output.Color*(1.0f-Alpha.a)+Color*Alpha.a;	
	
	Output.Color=Output.Color*Input.Diffuse+Input.Specular*Output.Color.a;
	Output.Color.xyz=Output.Color.xyz*(1.0f-Input.Tex0.z)+FogColor*Input.Tex0.z;
	
    Output.ColorForDepthMap=Input.Tex0.w;
    return Output;
}

PS_OUTPUT PShaderThreeLayerWithShadow(PS_INPUT Input)
{	
	PS_OUTPUT Output;
	
	Output.Color=tex2D( Sampler0, Input.Tex0.xy*TexRatio );
	
	float4 Color=tex2D( Sampler1, Input.Tex0.xy*TexRatio );
	float4 Alpha=tex2D( Sampler2, Input.Tex0.xy*AlphaTexRatio );	
	Output.Color=Output.Color*(1.0f-Alpha.a)+Color*Alpha.a;
	
	Color=tex2D( Sampler3, Input.Tex0.xy*TexRatio );
	Alpha=tex2D( Sampler4, Input.Tex0.xy*AlphaTexRatio );	
	Output.Color=Output.Color*(1.0f-Alpha.a)+Color*Alpha.a;	
	
	float4 Shadow=tex2D( SamplerShadow, Input.Tex0.xy );
	Output.Color=Output.Color*Shadow.a;
	
	Output.Color=Output.Color*Input.Diffuse+Input.Specular*Output.Color.a;
	Output.Color.xyz=Output.Color.xyz*(1.0f-Input.Tex0.z)+FogColor*Input.Tex0.z;
	
    Output.ColorForDepthMap=Input.Tex0.w;
    return Output;
}

PS_OUTPUT PShaderFourLayer(PS_INPUT Input)
{	
	PS_OUTPUT Output;
	
	Output.Color=tex2D( Sampler0, Input.Tex0.xy*TexRatio );
	
	float4 Color=tex2D( Sampler1, Input.Tex0.xy*TexRatio );
	float4 Alpha=tex2D( Sampler2, Input.Tex0.xy*AlphaTexRatio );	
	Output.Color=Output.Color*(1.0f-Alpha.a)+Color*Alpha.a;
	
	Color=tex2D( Sampler3, Input.Tex0.xy*TexRatio );
	Alpha=tex2D( Sampler4, Input.Tex0.xy*AlphaTexRatio );	
	Output.Color=Output.Color*(1.0f-Alpha.a)+Color*Alpha.a;
	
	Color=tex2D( Sampler5, Input.Tex0.xy*TexRatio );
	Alpha=tex2D( Sampler6, Input.Tex0.xy*AlphaTexRatio );	
	Output.Color=Output.Color*(1.0f-Alpha.a)+Color*Alpha.a;
	
	Output.Color=Output.Color*Input.Diffuse+Input.Specular*Output.Color.a;
	Output.Color.xyz=Output.Color.xyz*(1.0f-Input.Tex0.z)+FogColor*Input.Tex0.z;
	
    Output.ColorForDepthMap=Input.Tex0.w;
    return Output;
}

PS_OUTPUT PShaderFourLayerWithShadow(PS_INPUT Input)
{	
	PS_OUTPUT Output;
	
	Output.Color=tex2D( Sampler0, Input.Tex0.xy*TexRatio );
	
	float4 Color=tex2D( Sampler1, Input.Tex0.xy*TexRatio );
	float4 Alpha=tex2D( Sampler2, Input.Tex0.xy*AlphaTexRatio );	
	Output.Color=Output.Color*(1.0f-Alpha.a)+Color*Alpha.a;
	
	Color=tex2D( Sampler3, Input.Tex0.xy*TexRatio );
	Alpha=tex2D( Sampler4, Input.Tex0.xy*AlphaTexRatio );	
	Output.Color=Output.Color*(1.0f-Alpha.a)+Color*Alpha.a;
	
	Color=tex2D( Sampler5, Input.Tex0.xy*TexRatio );
	Alpha=tex2D( Sampler6, Input.Tex0.xy*AlphaTexRatio );	
	Output.Color=Output.Color*(1.0f-Alpha.a)+Color*Alpha.a;
	
	float4 Shadow=tex2D( SamplerShadow, Input.Tex0.xy );
	Output.Color=Output.Color*Shadow.a;
	
	Output.Color=Output.Color*Input.Diffuse+Input.Specular*Output.Color.a;
	Output.Color.xyz=Output.Color.xyz*(1.0f-Input.Tex0.z)+FogColor*Input.Tex0.z;
	
    Output.ColorForDepthMap=Input.Tex0.w;
    return Output;
}

VS_OUTPUT VShaderNormal(VS_INPUT Input)
{
    VS_OUTPUT   Output;
    
    
    float3 Pos =mul(float4(Input.Pos.xyz, 1.0f), WorldMatrix);
    float3 Normal = normalize(mul(Input.Normal.xyz, WorldMatrix));
    
    
    
    Output.Pos = mul(float4(Input.Pos.xyz, 1.0f), WorldViewMatrix);
    Output.Tex0.z = saturate(((Output.Pos.z-FogNear)/(FogFar-FogNear)));
    Output.Tex0.w = saturate((Output.Pos.z-CameraNear)/(CameraFar-CameraNear));
    Output.Pos = mul(Output.Pos, PrjMatrix);
    
    Output.Diffuse = CaculateDiffuse(Pos,Normal);
    Output.Specular = CaculateSpecular(Pos,Normal);
    Output.Tex0.xy = Input.Tex0;
    

    return Output;
}

float4 PShaderForDepth( float4 Diffuse : COLOR0) : COLOR0
{		
	return Diffuse;
}

VS_OUTPUT_FOR_DEPTH VShaderForDepth(VS_INPUT Input)
{
    VS_OUTPUT_FOR_DEPTH   Output;
    
    Output.Pos = mul(float4(Input.Pos.xyz, 1.0f), WorldViewMatrix);
    Output.Pos = mul(Output.Pos, PrjMatrix);
    Output.Diffuse = saturate((Output.Pos.z-CameraNear)/(CameraFar-CameraNear));
    
    return Output;
}

technique Tec0
{
    pass p0
    { 	   
		zenable = True;
		zwriteenable = True;
		fogenable = False;
		CullMode = ccw;
		
		AlphaBlendEnable = False;		
		AlphaTestEnable = False;
        
			
        
        VertexShader = compile vs_3_0 VShaderNormal();
        PixelShader  = compile ps_3_0 <PSFunction>();

    }
}

technique TechForDepthTexture
{
    pass p0
    { 	   
		zenable = True;
		zwriteenable = True;
		fogenable = False;
		CullMode = ccw;
		
		AlphaBlendEnable = False;		
		AlphaTestEnable = False;
        
			
        
        VertexShader = compile vs_2_0 VShaderForDepth();
        PixelShader  = compile ps_2_0 PShaderForDepth();

    }
}