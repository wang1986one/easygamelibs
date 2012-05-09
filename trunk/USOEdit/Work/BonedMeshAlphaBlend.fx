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
};

VS_OUTPUT VShadeWithSkinMesh(VS_INPUT Input)
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
    

  
    //Normal = mul(Normal, (float3x3)ViewMatrix);
    
    Normal = normalize(Normal);
   

	//计算灯光
    float3 L = - LightDir;
    
    float3 R = normalize(2 * dot(Normal, L) * Normal - L);          // reflection vector (view space)
    float3 V = - normalize(Pos);                             // view direction (view space)

    
    Output.Diffuse = MaterialEmissive +LightAmbient * MaterialAmbient + LightDiffuse * MaterialDiffuse * max(0, dot(Normal, L)); // diffuse + ambient
    Output.Specular = LightSpecular * MaterialSpecular * pow(max(0, dot(R, V)), MaterialPower/4);   // specular
    Output.Tex0  = Input.Tex0; 


    return Output;
}


VS_OUTPUT VShadeNormal(VS_INPUT Input)
{
    VS_OUTPUT   Output;
    float3      Pos = 0.0f;
    float3      Normal = 0.0f;      
        
        
	Pos = mul(float4(Input.Pos.xyz, 1.0f), ViewMatrix);
    
    Output.Pos = mul(float4(Pos.xyz, 1.0f), ProjMatrix);
    
    
    Normal = normalize(Input.Normal);

	//计算灯光
    float3 L = - LightDir;
    
    float3 R = normalize(2 * dot(Normal, L) * Normal - L);          // reflection vector (view space)
    float3 V = - normalize(Pos);                             // view direction (view space)

    
    Output.Diffuse = MaterialEmissive +LightAmbient * MaterialAmbient + LightDiffuse * MaterialDiffuse * max(0, dot(Normal, L)); // diffuse + ambient
    Output.Specular = LightSpecular * MaterialSpecular * pow(max(0, dot(R, V)), MaterialPower/4);   // specular
    Output.Tex0  = Input.Tex0; 
    
    


    return Output;
}


technique TecWithSkinMesh
{
    pass p0
    { 	   
		Lighting       = true;
   
		SpecularEnable = false;	
		zenable = true;
		zwriteenable = false;
		CullMode = none;
		fogenable = false;
		
		AlphaBlendEnable = true;
		BlendOp = Add;
		SrcBlend = SrcAlpha;
		DestBlend = InvSrcAlpha;
     
	
		 //Sampler[0] = (Sampler);

		Texture[0] = <TexLay0>; // Use the texture parameter defined above
        ColorOp[0] = SelectArg1;
        ColorArg1[0] = Texture;
        ColorArg2[0] = Diffuse;        
       
		AlphaOp[0] = SelectArg1;
		AlphaArg1[0] = Texture;	
		AlphaArg2[0] = diffuse;	
        
        
        ColorOp[1] = disable;
		AlphaOp[1] = disable;
		AddressU[0] = clamp;
		AddressV[0] = clamp;
               
		MinFilter[0] = Linear;
      	MagFilter[0] = Linear;
        MipFilter[0] = Linear;
        
        VertexShader = compile vs_2_0 VShadeWithSkinMesh();
        PixelShader  = NULL;

    }
}




technique TecNormal
{
    pass p0
    { 	   
		Lighting       = true;
   
		SpecularEnable = false;	
		zenable = true;
		zwriteenable = false;
		CullMode = none;
		fogenable = false;
		
		AlphaBlendEnable = true;
		BlendOp = Add;
		SrcBlend = SrcAlpha;
		DestBlend = InvSrcAlpha;
      
	
		 //Sampler[0] = (Sampler);

		Texture[0] = <TexLay0>; // Use the texture parameter defined above
        ColorOp[0] = SelectArg1;
        ColorArg1[0] = Texture;
        ColorArg2[0] = Diffuse;        
       
		AlphaOp[0] = SelectArg1;
		AlphaArg1[0] = Texture;	
		AlphaArg2[0] = diffuse;	
        
        
        ColorOp[1] = disable;
		AlphaOp[1] = disable;
		AddressU[0] = clamp;
		AddressV[0] = clamp;
               
		MinFilter[0] = Linear;
      	MagFilter[0] = Linear;
        MipFilter[0] = Linear;
        
        VertexShader = compile vs_2_0 VShadeNormal();
        PixelShader  = NULL;

    }
}