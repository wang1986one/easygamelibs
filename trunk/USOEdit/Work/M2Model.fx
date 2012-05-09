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
//float	LightTheta[MAX_LIGHT_COUNT];
//float	LightPhi[MAX_LIGHT_COUNT];
//float	LightFalloff[MAX_LIGHT_COUNT];


static float4 MaterialAmbient = {1.0f, 1.0f, 1.0f, 1.0f};
static float4 MaterialDiffuse = {1.0f, 1.0f, 1.0f, 1.0f};
static float4 MaterialSpecular = {1.0f, 1.0f, 1.0f, 1.0f};
static float4 MaterialEmissive = {0.0f, 0.0f, 0.0f, 1.0f};
static float  MaterialPower=40.0f;

float4 GlobalColor=1.0f;
float4x4    UVMatrix0;

float4x4			WorldMatrix;
float4x4			WorldViewMatrix;
shared float4x4		PrjMatrix;


shared float3		CameraPos;
shared float		CameraNear;
shared float		CameraFar;

shared float		FogNear=30.0f;
shared float		FogFar=300.0f;
shared float3		FogColor={0.25f, 0.25f, 0.5f};

// Matrix Pallette
static const int MAX_MATRICES = 62;
float4x3    BoneMatrixArray[MAX_MATRICES];

texture TexLay0 ;
texture TexLay1 ;
texture TexLay2 ;
texture TexLay3 ;


sampler Sampler0 =
sampler_state
{
    Texture = <TexLay0>;
    MinFilter = Linear;
    MagFilter = Linear;
    MipFilter = Linear;
    AddressU = <TextureAddrU0>;
    AddressV = <TextureAddrV0>;

};

sampler Sampler1 =
sampler_state
{
    Texture = <TexLay1>;
    MinFilter = Linear;
    MagFilter = Linear;
    MipFilter = Linear;
    AddressU = <TextureAddrU1>;
    AddressV = <TextureAddrV1>;

};

sampler Sampler2 =
sampler_state
{
    Texture = <TexLay2>;
    MinFilter = Linear;
    MagFilter = Linear;
    MipFilter = Linear;
    AddressU = <TextureAddrU2>;
    AddressV = <TextureAddrV2>;

};


sampler Sampler3 =
sampler_state
{
    Texture = <TexLay3>;
    MinFilter = Linear;
    MagFilter = Linear;
    MipFilter = Linear;
    AddressU = <TextureAddrU3>;
    AddressV = <TextureAddrV3>;

};



///////////////////////////////////////////////////////
struct VS_INPUT
{
    float4  Pos             : POSITION;
    float4  Normal          : NORMAL;
    float2  Tex0            : TEXCOORD0;
    float4	Diffuse			: COLOR0;
    float4  BlendWeights    : BLENDWEIGHT;
    float4	BlendIndices    : BLENDINDICES;
    
};

struct VS_OUTPUT
{
    float4  Pos			: POSITION;
    float4  Diffuse		: COLOR0;  
    float4  Tex0		: TEXCOORD0;
    float3  EnvTex		: TEXCOORD1;
};

struct PS_INPUT
{
	float4	Diffuse		: COLOR0;	
    float4	Tex0		: TEXCOORD0;
    float3  EnvTex		: TEXCOORD1;
    
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

PS_OUTPUT PShaderWithNormal(PS_INPUT Input)
{
	PS_OUTPUT Output;
		
	Output.Color=tex2D( Sampler0, Input.Tex0.xy );
<OtherPSOperation>
	Output.Color=Output.Color<PSDiffuseFunction>;
	Output.Color=Output.Color*GlobalColor;
	Output.Color.xyz=Output.Color.xyz*(1.0f-Input.Tex0.z)+FogColor*Input.Tex0.z;
	
	Output.ColorForDepthMap=Input.Tex0.w;
    return Output;
}


VS_OUTPUT VShaderWithSkinMesh(VS_INPUT Input)
{
    VS_OUTPUT   Output;
    float3      Pos = 0.0f;
    float3      Normal = 0.0f;
      
    float BlendWeightsArray[4] = (float[4])Input.BlendWeights;
    int   IndexArray[4]        = (int[4])Input.BlendIndices;
      
    
    for (int iBone = 0; iBone < 4; iBone++)
    {			        
		Pos += mul(Input.Pos, BoneMatrixArray[IndexArray[iBone]]) * BlendWeightsArray[iBone];
		
		Normal += mul(Input.Normal, (float3x3)BoneMatrixArray[IndexArray[iBone]]) * BlendWeightsArray[iBone];		
		
    } 
    
    Normal = normalize(mul(Normal, WorldMatrix));
    
    Output.Pos = mul(float4(Pos.xyz, 1.0f), WorldViewMatrix);
    Output.Tex0.z = saturate(((Output.Pos.z-FogNear)/(FogFar-FogNear)));
    Output.Tex0.w = saturate((Output.Pos.z-CameraNear)/(CameraFar-CameraNear));
	Output.Pos = mul(Output.Pos, PrjMatrix);
    Output.Diffuse = <VSDiffuseFunction>;
    <VertexAlphaOperation>;
	<TextureUVAni>;     
    Output.EnvTex = reflect(CameraPos,Normal);
	

    return Output;
}

VS_OUTPUT VShaderNormal(VS_INPUT Input)
{
    VS_OUTPUT   Output;
    float3      Pos = Input.Pos.xyz;
    float3      Normal = 0.0f;
        
   
    Normal = normalize(mul(Input.Normal.xyz, WorldMatrix));

	Output.Pos = mul(float4(Pos.xyz, 1.0f), WorldViewMatrix);
	Output.Tex0.z = saturate(((Output.Pos.z-FogNear)/(FogFar-FogNear)));
	Output.Tex0.w = saturate((Output.Pos.z-CameraNear)/(CameraFar-CameraNear));
	Output.Pos = mul(Output.Pos, PrjMatrix);
	Output.Diffuse = <VSDiffuseFunction>;
	<VertexAlphaOperation>;
    <TextureUVAni>; 
	Output.EnvTex = reflect(CameraPos,Normal);

    return Output;
}


technique Tec0
{
    pass p0
    { 	   
		zenable = True;
		zwriteenable = <EnableZWrite>;
		fogenable = False;
		CullMode = <CullMode>;
		
		AlphaBlendEnable = <EnableAlphaBlend>;
		BlendOp = <BlendOp>;
		SrcBlend = <SrcBlend>;
		DestBlend = <DestBlend>;
		
		AlphaTestEnable = <EnableAlphaTest>;
        AlphaRef = 0x80;
        AlphaFunc = GreaterEqual;
			
        
        VertexShader = compile vs_2_0 <VShader>();
        PixelShader  = compile ps_2_0 <PShader>();

    }
}

