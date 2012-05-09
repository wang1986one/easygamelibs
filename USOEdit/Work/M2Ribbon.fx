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
float  MaterialPower=40.0f;



float4x4    ViewMatrix;
shared float4x4	PrjMatrix;

shared float3 CameraPos;

shared float		FogNear=30.0f;
shared float		FogFar=300.0f;
shared float3		FogColor={0.25f, 0.25f, 0.5f};

texture TexLay0 ;
texture TexLay1 ;


sampler Sampler0 =
sampler_state
{
    Texture = <TexLay0>;
    MinFilter = Linear;
    MagFilter = Linear;
    MipFilter = Linear;
    AddressU = WRAP;
    AddressV = WRAP;

};

sampler Sampler1 =
sampler_state
{
    Texture = <TexLay1>;
    MinFilter = Linear;
    MagFilter = Linear;
    MipFilter = Linear;
    AddressU = WRAP;
    AddressV = WRAP;

};



///////////////////////////////////////////////////////
struct VS_INPUT
{
    float4  Pos             : POSITION;
    float4  Diffuse			: COLOR0;
    float2  Tex0            : TEXCOORD0;    
};

struct VS_OUTPUT
{
    float4  Pos			: POSITION;
    float4  Diffuse		: COLOR0;
    float3  Tex0		: TEXCOORD0;   
};

float4 CaculateDiffuse(float3 Pos)
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
					Diffuse += LightDiffuse[i] * MaterialDiffuse * Atten + 
						LightAmbient[i] * MaterialAmbient;
				}
			}
			else if(LightType[i]==3)
			{
				Diffuse += LightDiffuse[i] * MaterialDiffuse + LightAmbient[i] * MaterialAmbient;
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

float4 PShaderWithNormal( float4 Diffuse : COLOR0,           
           float3 Tex0 : TEXCOORD0) : COLOR0
{	
	float4 Color=tex2D( Sampler0, Tex0.xy )*Diffuse;
    Color.xyz=Color.xyz*(1.0f-Tex0.z)+FogColor*Tex0.z;
	return Color;
}


VS_OUTPUT VShaderNormal(VS_INPUT Input)
{
    VS_OUTPUT   Output;
    
    float3 Pos = Input.Pos.xyz;
    
    Output.Diffuse = Input.Diffuse * <DiffuseFunction>;
    
    Pos = mul(float4(Pos.xyz, 1.0f), ViewMatrix);
    
    Output.Tex0.z = saturate(((Pos.z-FogNear)/(FogFar-FogNear)));
    
    Output.Pos = mul(float4(Pos.xyz, 1.0f), PrjMatrix);    

	
    Output.Tex0.xy  = Input.Tex0; 

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
        AlphaRef = 0x10;
        AlphaFunc = GreaterEqual;
			
        
        VertexShader = compile vs_2_0 VShaderNormal();
        PixelShader  = compile ps_2_0 PShaderWithNormal();

    }
}

