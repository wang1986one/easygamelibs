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

texture TexLay0 ;
texture TexLay1 ;

float4 WaterColorLight={0.4f,0.8f,0.4f,1.0f};
float4 WaterColorDark={0.1f,0.2f,0.1f,1.0f};


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
    AddressU = Clamp;
    AddressV = Clamp;

};



struct VS_INPUT
{
    float4  Pos             : POSITION;
    float4	Diffuse			: COLOR0;
    float2  Tex0			: TEXCOORD0;    
};

struct VS_OUTPUT
{
    float4  Pos			: POSITION;
    float4  Diffuse		: COLOR0;  
    float4  Specular	: COLOR1;
    float2  Tex0		: TEXCOORD0;
    float3  Tex1		: TEXCOORD1;
};
struct PS_INPUT
{
	float4	Diffuse		: COLOR0;
	float4  Specular	: COLOR1;
	float2	Tex0		: TEXCOORD0;
	float3	Tex1		: TEXCOORD1;
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

float4 PShader( PS_INPUT Input) : COLOR0
{
	float FogFactor=saturate(((Input.Tex1.z-FogNear)/(FogFar-FogNear)));
	float4 DepthTex = mul(float4(Input.Tex1.xyz, 1.0f),PrjMatrix);
	DepthTex.x = (DepthTex.x/DepthTex.w+1.0f)/2;
    DepthTex.y = (1.0f-DepthTex.y/DepthTex.w)/2;
    DepthTex.z = DepthTex.z/(CameraFar-CameraNear);
    
	float Depth=tex2D( Sampler1, DepthTex.xy ).r;
	Depth=saturate((Depth-DepthTex.z)*(CameraFar-CameraNear)/5+0.3)*0.95;
	
	float4 TexColor=tex2D( Sampler0, Input.Tex0*8.0f );
	
	//Input.Diffuse.a=Input.Diffuse.a*1.5f+0.3f;
	
	float DepthAlpha=TexColor.a*Depth;
	
	float4 Specular=Input.Specular*TexColor.a;
	
	float4 OutColor=lerp(WaterColorDark,WaterColorLight,TexColor.a);
	
	//OutColor=OutColor+TexColor;
	
	OutColor=OutColor*(Input.Diffuse+Specular);
	
	OutColor.a=Depth;
	
	OutColor.xyz=OutColor.xyz*(1.0f-FogFactor)+FogColor*FogFactor;
	
    return OutColor;
}



VS_OUTPUT VShaderNormal(VS_INPUT Input)
{
    VS_OUTPUT   Output;
    float3      Pos = 0.0f;
        
    Pos = mul(float4(Input.Pos.xyz, 1.0f), WorldMatrix);   
    float3 Normal = normalize(mul(float3(0.0f, 1.0f, 0.0f), WorldMatrix)); 
   
    
    Output.Pos = mul(float4(Input.Pos.xyz, 1.0f), WorldViewMatrix);
    Output.Tex1 = Output.Pos;
    Output.Pos = mul(Output.Pos, PrjMatrix);

	Output.Diffuse = CaculateDiffuse(Pos,Normal);
	Output.Diffuse.a = Input.Diffuse.a;
	Output.Specular = CaculateSpecular(Pos,Normal);
    Output.Tex0 = Input.Tex0; 	
    
    

    return Output;
}

technique Tec0
{
    pass p0
    { 	   
		zenable = True;
		zwriteenable = True;
		fogenable = False;
		CullMode = none;
		
		AlphaBlendEnable = True;		
		AlphaTestEnable = False;
		
		BlendOp = Add;
		SrcBlend = SrcAlpha;
		DestBlend = InvSrcAlpha;
        
			
        
        VertexShader = compile vs_2_0 VShaderNormal();
        PixelShader  = compile ps_2_0 PShader();

    }
}

