// <effectEd direct3D="9" profile="fx_2_0" shaderFlags="None, WarningsAreErrors" />

//--------------------------------------------------------------------------------------
// Global variables
//--------------------------------------------------------------------------------------
struct ConstantBuffer
{
	half4 		AmbientColor;
	half4 		FogColor;
};

ConstantBuffer g_cbuffer : register(c0);

sampler2D	g_TexSampler0;
sampler2D	g_TexSampler1;

struct PS_INPUT
{
	half4 Diffuse : COLOR0;
	half3 Tex0 : TEXCOORD0;		//z: fog alpha	
	half2 Tex1 : TEXCOORD1;		
};

float4 main( PS_INPUT i ) : COLOR0
{
	float fogAlpha = i.Tex0.z;
	
	float4 color;
	float4 tex0 = tex2D(g_TexSampler0, i.Tex0);
	float4 tex1 = tex2D(g_TexSampler1, i.Tex1);
	
	color = tex0 * i.Diffuse + i.Diffuse * tex1 * tex0.w;
	
	//fog
	color.rgb = color.rgb * (1.0f-fogAlpha) + g_cbuffer.FogColor.rgb * fogAlpha;

	return color;
}