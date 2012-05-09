float4 LightDir = {1.0f, 0.0f, 0.0f, 1.0f};    //light Direction 
float4 LightAmbient = {0.1f, 0.1f, 0.1f, 1.0f}; // Light Diffuse
float4 LightDiffuse = {0.6f, 0.6f, 0.6f, 1.0f}; // Light Diffuse
float4 LightSpecular = {1.0f, 1.0f, 1.0f, 1.0f}; // Light Diffuse
float4 MaterialAmbient = {1.0f, 1.0f, 1.0f, 1.0f};
float4 MaterialDiffuse = {1.0f, 1.0f, 1.0f, 1.0f};
float4 MaterialSpecular = {1.0f, 1.0f, 1.0f, 1.0f};
float4 MaterialEmissive = {0.0f, 0.0f, 0.0f, 1.0f};
float  MaterialPower=40.0f;

float4x4    ViewMatrix;
float4x4	ProjMatrix;


// Matrix Pallette
static const int MAX_MATRICES = 75;
float4x3    BoneMatrixArray[MAX_MATRICES];

texture TexLay0 < string name = "test1.jpg"; >;
texture TexLay1 < string name = "test1.jpg"; >;

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
    MinFilter = none;
    MagFilter = none;
    MipFilter = none;
    AddressU = WRAP;
    AddressV = WRAP;

};


///////////////////////////////////////////////////////
struct VS_INPUT
{
    float4  Pos             : POSITION;
    float4  Normal          : NORMAL;
    float2  Tex0            : TEXCOORD0;
    float4  BlendWeights    : BLENDWEIGHT;
    float4	BlendIndices    : BLENDINDICES;
    
};

struct VS_OUTPUT
{
    float4  Pos			: POSITION;
    float4  Diffuse		: COLOR0;
    float4	Specular	: COLOR1;
    float2  Tex0		: TEXCOORD0;
    float3  EnvTex		: TEXCOORD1;
};


float4x4 inverse_nonSAS(float4x4 m)
{
    float4x4 adj;
    
    // Calculate the adjoint matrix
    adj._11 = m._22*m._33*m._44 + m._23*m._34*m._42 + m._24*m._32*m._43 - m._22*m._34*m._43 - m._23*m._32*m._44 - m._24*m._33*m._42;
    adj._12 = m._12*m._34*m._43 + m._13*m._32*m._44 + m._14*m._33*m._42 - m._12*m._33*m._44 - m._13*m._34*m._42 - m._14*m._32*m._43;
    adj._13 = m._12*m._23*m._44 + m._13*m._24*m._42 + m._14*m._22*m._43 - m._12*m._24*m._43 - m._13*m._22*m._44 - m._14*m._23*m._42;
    adj._14 = m._12*m._24*m._33 + m._13*m._22*m._34 + m._14*m._23*m._32 - m._12*m._23*m._34 - m._13*m._24*m._32 - m._14*m._22*m._33;
    
    adj._21 = m._21*m._34*m._43 + m._23*m._31*m._44 + m._24*m._33*m._41 - m._21*m._33*m._44 - m._23*m._34*m._41 - m._24*m._31*m._43;
    adj._22 = m._11*m._33*m._44 + m._13*m._34*m._41 + m._14*m._31*m._43 - m._11*m._34*m._43 - m._13*m._31*m._44 - m._14*m._33*m._41;
    adj._23 = m._11*m._24*m._43 + m._13*m._21*m._44 + m._14*m._23*m._41 - m._11*m._23*m._44 - m._13*m._24*m._41 - m._14*m._21*m._43;
    adj._24 = m._11*m._23*m._34 + m._13*m._24*m._31 + m._14*m._21*m._33 - m._11*m._24*m._33 - m._13*m._21*m._34 - m._14*m._23*m._31;
    
    adj._31 = m._21*m._32*m._44 + m._22*m._34*m._41 + m._24*m._31*m._42 - m._21*m._34*m._42 - m._22*m._31*m._44 - m._24*m._32*m._41;
    adj._32 = m._11*m._34*m._42 + m._12*m._31*m._44 + m._14*m._32*m._41 - m._11*m._32*m._44 - m._12*m._34*m._41 - m._14*m._31*m._42;
    adj._33 = m._11*m._22*m._44 + m._12*m._24*m._41 + m._14*m._21*m._42 - m._11*m._24*m._42 - m._12*m._21*m._44 - m._14*m._22*m._41;
    adj._34 = m._11*m._24*m._32 + m._12*m._21*m._34 + m._14*m._22*m._31 - m._11*m._22*m._34 - m._12*m._24*m._31 - m._14*m._21*m._32;
    
    adj._41 = m._21*m._33*m._42 + m._22*m._31*m._43 + m._23*m._32*m._41 - m._21*m._32*m._43 - m._22*m._33*m._41 - m._23*m._31*m._42;
    adj._42 = m._11*m._32*m._43 + m._12*m._33*m._41 + m._13*m._31*m._42 - m._11*m._33*m._42 - m._12*m._31*m._43 - m._13*m._32*m._41;
    adj._43 = m._11*m._23*m._42 + m._12*m._21*m._43 + m._13*m._22*m._41 - m._11*m._22*m._43 - m._12*m._23*m._41 - m._13*m._21*m._42;
    adj._44 = m._11*m._22*m._33 + m._12*m._23*m._31 + m._13*m._21*m._32 - m._11*m._23*m._32 - m._12*m._21*m._33 - m._13*m._22*m._31;
    
    // Calculate the determinant
    float det = determinant(m);
    
    float4x4 result = 0;
    if( 0.0f != det )
        result = 1.0f/det * adj;
        
    return result;
}


float4 PShade( float4 Diffuse : COLOR0,
           float4 Specular : COLOR1,
           float2 Tex0 : TEXCOORD0,
           float3 EnvTex : TEXCOORD1 ) : COLOR0
{
    return tex2D( Sampler0, Tex0 ) * Diffuse + texCUBE( Sampler1, EnvTex )*2;
}

VS_OUTPUT VShade(VS_INPUT Input)
{
    VS_OUTPUT   Output;
    float3      Pos = 0.0f;
    float3      Normal = 0.0f;      
     
    // Compensate for lack of UBYTE4 on Geforce3   
	//int4 IndexVector = D3DCOLORtoUBYTE4(i.BlendIndices);

    // cast the vectors to arrays for use in the for loop below
    float BlendWeightsArray[4] = (float[4])Input.BlendWeights;
    int   IndexArray[4]        = (int[4])Input.BlendIndices;
    
    // calculate the pos/normal using the "normal" weights 
    //        and accumulate the weights to calculate the last weight
    
    for (int iBone = 0; iBone < 4; iBone++)
    {
			        
		Pos += mul(Input.Pos, BoneMatrixArray[IndexArray[iBone]]) * BlendWeightsArray[iBone];
		Normal += mul(Input.Normal, (float3x3)BoneMatrixArray[IndexArray[iBone]]) * BlendWeightsArray[iBone];
		
    } 
        
    
    Pos = mul(float4(Pos.xyz, 1.0f), ViewMatrix);
    
    Output.Pos = mul(float4(Pos.xyz, 1.0f), ProjMatrix);
    

	Normal = normalize(Normal);
	 
    float3 ViewNormal = normalize(mul(Normal, (float3x3)ViewMatrix));
    
    
   

	//¼ÆËãµÆ¹â
    float3 L = - LightDir;
    
    float3 R = normalize(2 * dot(Normal, L) * Normal - L);          // reflection vector (view space)
    float3 V = - normalize(Pos);                             // view direction (view space)

    
    Output.Diffuse = MaterialEmissive +LightAmbient * MaterialAmbient + LightDiffuse * MaterialDiffuse * max(0, dot(Normal, L)); // diffuse + ambient
    Output.Specular = LightSpecular * MaterialSpecular * pow(max(0, dot(R, V)), MaterialPower/4);   // specular
    Output.Tex0  = Input.Tex0; 
    
    
    //float4x4 ViewInvMat= inverse_nonSAS(ViewMatrix);
    //Output.EnvTex = 2 * dot( -Pos, ViewNormal ) * ViewNormal + Pos;
    //Output.EnvTex = mul( Output.EnvTex, (float3x3)ViewInvMat );   
   
   
    ///float3 vEyeR = -normalize( Pos );
    
    //Output.EnvTex = 2 * dot( vEyeR, ViewNormal ) * ViewNormal - vEyeR;
    
	Output.EnvTex = reflect(ViewNormal,Normal);
	


    return Output;
}


technique tec0
{
    pass p0
    { 	   
		Lighting       = true;
   
		SpecularEnable = false;	
		zenable = true;
		zwriteenable = true;
		CullMode = ccw;
		fogenable = false;
		
		AlphaBlendEnable = false;
		AlphaTestEnable = false;
	
		 //Sampler[0] = (Sampler);

		Texture[0] = <TexLay0>; // Use the texture parameter defined above
        ColorOp[0] = Modulate;
        ColorArg1[0] = Texture;
        ColorArg2[0] = Diffuse;
        
        AlphaOp[0] = Disable;
        
        ColorOp[1] = disable;
		AlphaOp[1] = disable;               
		
        
        VertexShader = compile vs_2_0 VShade();
        PixelShader  = compile ps_2_0 PShade();

    }
}