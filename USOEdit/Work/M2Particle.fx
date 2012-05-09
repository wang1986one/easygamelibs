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

bool	TransformGravity=true;
int		TextureTileRotation=0;

float4x4    WorldRotation;
float3		WorldScale;
float3		WorldTranslation;
float4x4    ViewMatrix;

float4x4		WorldMatrix;
shared float4x4		PrjMatrix;

shared float		FogNear=30.0f;
shared float		FogFar=300.0f;
shared float3		FogColor={0.25f, 0.25f, 0.5f};

texture TexLay0 ;
texture TexLay1 ;
texture TexLay2 ;

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



///////////////////////////////////////////////////////
struct VS_INPUT
{
    float4  Pos		        : POSITION;    
    float4  Dir				: NORMAL;
    float4	Param1			: COLOR0;
    float4	Param2			: COLOR1;    
    float4  ColorStart		: TEXCOORD0;
    float4  ColorMid		: TEXCOORD1;
    float4  ColorEnd		: TEXCOORD2;
    float4	Translation		: TEXCOORD3;
    float4	Rotation		: TEXCOORD4;
    float4	Scaling			: TEXCOORD5;
    float4	Size1			: TEXCOORD6;
    float4	Size2			: TEXCOORD7;
};

struct VS_OUTPUT
{
    float4  Pos			: POSITION;
    float4  Diffuse		: COLOR0;  
    float3  Tex0		: TEXCOORD0;   
};

float4x4 QuaternionToMatrix(float4 q)
{
	float4x4 Mat;
	Mat._11 = 1.0f - 2.0f * q.y * q.y - 2.0f * q.z * q.z;
	Mat._12 = 2.0f * q.x * q.y + 2.0f * q.w * q.z;
	Mat._13 = 2.0f * q.x * q.z - 2.0f * q.w * q.y;
	Mat._21 = 2.0f * q.x * q.y - 2.0f * q.w * q.z;
	Mat._22 = 1.0f - 2.0f * q.x * q.x - 2.0f * q.z * q.z;
	Mat._23 = 2.0f * q.y * q.z + 2.0f * q.w * q.x;
	Mat._31 = 2.0f * q.x * q.z + 2.0f * q.w * q.y;
	Mat._32 = 2.0f * q.y * q.z - 2.0f * q.w * q.x;
	Mat._33 = 1.0f - 2.0f * q.x * q.x - 2.0f * q.y * q.y;
	Mat._14 = 0.0f;
	Mat._24 = 0.0f;
	Mat._34 = 0.0f;
	Mat._41 = 0.0f;
	Mat._42 = 0.0f;
	Mat._43 = 0.0f;
	Mat._44 = 1.0f;
	return Mat;
}

float4 CaculateDiffuse(float3 Pos)
{
	if(LightCount>0)
	{
		float4 Diffuse={0.0f,0.0f,0.0f,0.0f};
		for(int i=0;i<LightCount;i++)
		{
			Diffuse += LightAmbient[i] * MaterialAmbient;
			/*
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
				Diffuse += 0.5f*LightDiffuse[i] * MaterialDiffuse + LightAmbient[i] * MaterialAmbient;
			}
			*/
			
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
   
    
    
    float S=Input.Pos.w/Input.Dir.w;
    
    if(S<Input.Param2.z)
    {
		Output.Diffuse.rgb=lerp(Input.ColorStart.rgb,Input.ColorMid.rgb,S/Input.Param2.z);
    }
    else
    {
		Output.Diffuse.rgb=lerp(Input.ColorMid.rgb,Input.ColorEnd.rgb,(S-Input.Param2.z)/(1.0f-Input.Param2.z));
    }
    
			
	if(S<Input.Param2.w)
	{
		Output.Diffuse.a=lerp(Input.ColorStart.a,Input.ColorMid.a,S/Input.Param2.w);
	}
	else
	{
		Output.Diffuse.a=lerp(Input.ColorMid.a,Input.ColorEnd.a,(S-Input.Param2.w)/(1.0f-Input.Param2.w));
	}	
	
	
			
	float2 SizeStart=Input.Size1.xy;
	float2 SizeMid=Input.Size1.zw;
	float2 SizeEnd=Input.Size2;
	
	float2 Size;
	
	if(S<Input.Scaling.w)
	{
		Size=lerp(SizeStart,SizeMid,S/Input.Scaling.w);
	}
	else
	{
		Size=lerp(SizeMid,SizeEnd,(S-Input.Scaling.w)/(1.0f-Input.Scaling.w));
	}
		
	float4x4 Rotation=mul(QuaternionToMatrix(Input.Rotation),WorldRotation);
	
	float3 Translation=Input.Translation.xyz+WorldTranslation;
	float3 Scaling=Input.Scaling.xyz*WorldScale;
	
	float3 Pos=mul(float4(Input.Pos.xyz,1.0f), Rotation);
	float3 Dir=mul(float4(Input.Dir.xyz,1.0f), Rotation);
	
	Dir=normalize(Dir);
	
	float3 GravityDir={0.0f,-1.0f,0.0f};
	
	if(TransformGravity)
	{
		GravityDir=mul(float4(GravityDir,1.0f), Rotation);
		GravityDir=normalize(GravityDir);
	}
		
	float MSpeed=1.0f;
	if(Input.Param2.x>0.0f)
	{
		MSpeed=exp(-1.0f*Input.Param2.x*Input.Pos.w);
	}
    
    Pos=Pos+Dir*MSpeed*Input.Param1.x*Input.Pos.w+
		0.5f*MSpeed*Dir*Input.Param1.y*Input.Pos.w*Input.Pos.w-
		0.5f*MSpeed*Dir*Input.Param1.w*Input.Pos.w*Input.Pos.w+
		0.5f*MSpeed*GravityDir*Input.Param1.z*Input.Pos.w*Input.Pos.w;
    
   
    
    Pos = Pos*Scaling+Translation;    
    
    Output.Diffuse = Output.Diffuse * <DiffuseFunction>;
    
    Pos = mul(float4(Pos, 1.0f), ViewMatrix);
    
    Size=Size*Scaling.xy;   
    
    float Angle=S*Input.Param2.y;
    
    float4x4	ParticleRotation={	1.0f,0.0f,0.0f,0.0f,
									0.0f,1.0f,0.0f,0.0f,
									0.0f,0.0f,1.0f,0.0f,
									0.0f,0.0f,0.0f,1.0f};
									
	ParticleRotation._11=cos(Angle);
	ParticleRotation._21=sin(Angle);
	ParticleRotation._22=ParticleRotation._11;
	ParticleRotation._12=-ParticleRotation._21;
    
   
    float2 Rect=(float2(0.5f,0.5f)-Input.Size2.zw)*Size;
    
    Rect = mul(float4(Rect,0,1.0f), ParticleRotation);
    
    Pos.xy=Pos.xy+Rect;   	
    
    Output.Tex0.z = saturate(((Pos.z-FogNear)/(FogFar-FogNear)));
    
    Output.Pos = mul(float4(Pos.xyz, 1.0f), PrjMatrix);
    
    
    
    float TexRows=round(Input.Translation.w/100.0f);
    float TexCols=round(Input.Translation.w-TexRows*100.0f);
    
    float Grid=TexCols*TexRows*S;
    
    float Row=floor(Grid/TexCols);
    float Col=floor(Grid - Row*TexCols);
    
    if(TextureTileRotation==-1)
    {
		Output.Tex0.x=(Row/TexRows) + (1.0f/TexRows*Input.Size2.w);
		Output.Tex0.y=(Col/TexCols) + (1.0f/TexCols*Input.Size2.z);
    }
    else if(TextureTileRotation==1)
    {
		Output.Tex0.x=1.0f-((Row/TexRows) + (1.0f/TexRows*Input.Size2.w));
		Output.Tex0.y=(Col/TexCols) + (1.0f/TexCols*Input.Size2.z);
    }
    else
    {
		Output.Tex0.x=(Col/TexCols) + (1.0f/TexCols*Input.Size2.z);
		Output.Tex0.y=(Row/TexRows) + (1.0f/TexRows*Input.Size2.w);
    }
	

    return Output;
}


technique Tec0
{
    pass p0
    { 	   
		zenable = True;
		zwriteenable = <EnableZWrite>;
		fogenable = False;
		CullMode = None;
		
		AlphaBlendEnable = <EnableAlphaBlend>;
		BlendOp = <BlendOp>;
		SrcBlend = <SrcBlend>;
		DestBlend = <DestBlend>;
		
		AlphaTestEnable = <EnableAlphaTest>;
        AlphaRef = 0x80;
        AlphaFunc = GreaterEqual;
			
        
        VertexShader = compile vs_2_0 VShaderNormal();
        PixelShader  = compile ps_2_0 PShaderWithNormal();

    }
}

