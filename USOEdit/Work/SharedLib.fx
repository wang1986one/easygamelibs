struct LIGHT_INFO
{
	int		LightType;
	float4	LightDiffuse;
	float4	LightSpecular;
	float4	LightAmbient;
	float3	LightPos;
	float3	LightDir;
	float	LightRange;
	float	LightFalloff;
	float	LightAtn0;
	float	LightAtn1;
	float	LightAtn2;
	float	LightTheta;
	float	LightPhi;
};													
shared bool			IsGlobalLightEnable;
shared LIGHT_INFO	GlobalLight;
shared float4x4		PrjMatrix;
shared float3		CameraPos={0.0f,0.0f,0.0f};
shared float		CameraNear=1.0f;
shared float		CameraFar=1000.0f;
shared float		FogNear=3000.0f;
shared float		FogFar=3000.0f;
shared float3		FogColor={0.5f, 0.5f, 0.5f};

static float4 MaterialAmbient = {1.0f, 1.0f, 1.0f, 1.0f};
static float4 MaterialDiffuse = {1.0f, 1.0f, 1.0f, 1.0f};
static float4 MaterialSpecular = {1.0f, 1.0f, 1.0f, 1.0f};
static float4 MaterialEmissive = {0.0f, 0.0f, 0.0f, 1.0f};
static float  MaterialPower=40.0f;

float4 CaculateDiffuse(float3 Pos,float3 Normal,LIGHT_INFO LightInfo)
{
	float4 Diffuse={0.0f,0.0f,0.0f,0.0f};
	
	if(LightInfo.LightType==1)
	{
		float Dis=length(LightInfo.LightPos-Pos);
		if(Dis<=LightInfo.LightRange)
		{
			float Atten = 1/( LightInfo.LightAtn0 + LightInfo.LightAtn1 * Dis + LightInfo.LightAtn2 * Dis * Dis);
			Diffuse += max(0,dot( Normal, normalize(LightInfo.LightPos-Pos) )) * 
				LightInfo.LightDiffuse * MaterialDiffuse * Atten + 
				LightInfo.LightAmbient * MaterialAmbient;
		}
	}
	else if(LightInfo.LightType==3)
	{
		Diffuse += max(0,dot( Normal, -normalize( LightInfo.LightDir ) )) * 
			LightInfo.LightDiffuse * MaterialDiffuse + 
			LightInfo.LightAmbient * MaterialAmbient;
	}
		
	return Diffuse;
}

float4 CaculateDiffuseAll(float3 Pos,float3 Normal)
{
	if(IsGlobalLightEnable||LightCount>0)
	{
		float4 Diffuse={0.0f,0.0f,0.0f,0.0f};
		
		if(IsGlobalLightEnable)
		{
			Diffuse+=CaculateDiffuse(Pos,Normal,GlobalLight);
		}
	
		for(int i=0;i<LightCount;i++)
		{
			Diffuse+=CaculateDiffuse(Pos,Normal,Lights[i]);
		}
		Diffuse += MaterialEmissive;
		return Diffuse;
	}
	else
	{		
		return 1.0f;
	}	
}

float4 CaculateSpecular(float3 Pos,float3 Normal,LIGHT_INFO LightInfo)
{
	float4 Specular={0.0f,0.0f,0.0f,0.0f};
	float3 EyeDir=Pos-CameraPos;
	for(int i=0;i<LightCount;i++)
	{
		if(LightInfo.LightType==1)
		{
			float Dis=length(LightInfo.LightPos-Pos);
			if(Dis<=LightInfo.LightRange)
			{
				float Atten = 1/( LightInfo.LightAtn0 + LightInfo.LightAtn1 * Dis + LightInfo.LightAtn2 * Dis * Dis);				
				float3 LightRelrect = normalize(reflect(Pos - LightInfo.LightPos,Normal));
				Specular += pow( max( 0,dot(LightRelrect,-normalize(EyeDir) ) ) ,MaterialPower/4 ) * 
					LightInfo.LightSpecular * MaterialSpecular * Atten;
			}				
		}
		else if(LightInfo.LightType==3)
		{
			float3 LightRelrect = normalize(reflect(LightInfo.LightDir,Normal));
			Specular += pow( max( 0,dot(LightRelrect,-normalize(EyeDir) ) ) ,MaterialPower/4 ) * 
				LightInfo.LightSpecular* MaterialSpecular;
		}
	}
	return Specular;
}

float4 CaculateSpecularAll(float3 Pos,float3 Normal)
{
	if(IsGlobalLightEnable||LightCount>0)
	{
		float4 Specular={0.0f,0.0f,0.0f,0.0f};
		
		if(IsGlobalLightEnable)
		{
			Specular+=CaculateSpecular(Pos,Normal,GlobalLight);
		}
	
		for(int i=0;i<LightCount;i++)
		{
			Specular+=CaculateSpecular(Pos,Normal,Lights[i]);
		}		
		return Specular;
	}
	else
	{		
		return 1.0f;
	}	
}