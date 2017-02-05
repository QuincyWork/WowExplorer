
//struct PSConstBuffer
//{
//	mediump vec4	AmbientColor;
//	mediump vec4	FogColor;
//};

//uniform PSConstBuffer g_psbuffer;

const int AmbientColor = 0;
const int FogColor = 1;

const int PSBUFFER_SIZE = 2;

uniform mediump vec4 g_psbuffer[PSBUFFER_SIZE];

uniform sampler2D	g_TexSampler0;
uniform sampler2D	g_TexSampler1;

varying mediump vec4 v_Diffuse;
varying mediump vec3 v_Tex0;
varying mediump vec2 v_Tex1;

void main(void)
{
	mediump vec4 col;
	mediump vec4 tex0 = texture2D(g_TexSampler0, v_Tex0.xy);
	mediump vec4 tex1 = texture2D(g_TexSampler1, v_Tex1.xy);
	
	col.rgb = tex0.rgb * v_Diffuse.a + tex1.rgb * (1.0 - v_Diffuse.a);
	col.rgb = col.rgb * v_Diffuse.rgb;
	
	col.a = 1.0;
		
	//fog
	mediump float fogAlpha = v_Tex0.z;
	col.rgb = col.rgb * (1.0 - fogAlpha) + g_psbuffer[FogColor].rgb * fogAlpha;

	// Copy the primary color
    gl_FragColor = col;
}