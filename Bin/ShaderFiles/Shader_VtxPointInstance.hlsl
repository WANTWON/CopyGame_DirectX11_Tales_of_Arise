
#include "Client_Shader_Defines.hpp"

matrix g_WorldMatrix, g_ViewMatrix, g_ProjMatrix;
vector g_vCamPosition;

texture2D g_DiffuseTexture;
float g_fAlphaDiscard;
float3 g_vGlowColor;

struct VS_IN
{
	/* Vertex */
	float3 vPosition : POSITION;
	float2 vPSize : PSIZE;

	/* Instance */
	float4 vRight : TEXCOORD0;
	float4 vUp : TEXCOORD1;
	float4 vLook : TEXCOORD2;
	float4 vTranslation : TEXCOORD3;
	float fAlpha : TEXCOORD4;
	float3 vColor : TEXCOORD5;
};

struct VS_OUT
{
	float3 vPosition : POSITION;
	float2 vPSize : PSIZE;
	float fAlpha : TEXCOORD0;
	float3 vColor : TEXCOORD1;
};

VS_OUT VS_MAIN(VS_IN In)
{
	VS_OUT Out = (VS_OUT)0;
	
	float4x4 TransformMatrix = float4x4(In.vRight, In.vUp, In.vLook, In.vTranslation);
	float4x4 InstanceWorldMatrix = mul(TransformMatrix, g_WorldMatrix);

	/* Transform to World Space in Vertex Shader. 
	(View and Projection Transforms are applied in the Geometry Shader). */
	vector vPosition = mul(vector(In.vPosition, 1.f), InstanceWorldMatrix);
	
	Out.vPosition = vPosition.xyz;
	Out.vPSize.x = In.vPSize.x * length(In.vRight.xyz);
	Out.vPSize.y = In.vPSize.y * length(In.vUp.xyz);
	Out.fAlpha = In.fAlpha;
	Out.vColor = In.vColor;

	return Out;
}

/* Geometry Shader -> Creates Vertices
Called depending on the primitive you want to draw.
- Point: Once per Vertex.
- Line: Twice Vertex.
- Triangle: 3 times Vertex */
struct GS_IN
{
	float3 vPosition : POSITION;
	float2 vPSize : PSIZE;
	float fAlpha : TEXCOORD0;
	float3 vColor : TEXCOORD1;
};

struct GS_OUT
{
	float4 vPosition : SV_POSITION;
	float2 vTexUV : TEXCOORD0;
	float fAlpha : TEXCOORD1;
	float3 vColor : TEXCOORD2;
};

[maxvertexcount(20)]
void GS_MAIN(point GS_IN In[1], inout TriangleStream<GS_OUT> DataStream)
{
	GS_OUT Out[4];	
		   
	float3 vLook = g_vCamPosition.xyz - In[0].vPosition;
	float3 vRight = normalize(cross(float3(0.f, 1.f, 0.f), vLook)) * (In[0].vPSize.x * 0.5f);
	float3 vUp = normalize(cross(vLook, vRight)) * (In[0].vPSize.y * 0.5f);
		   
	matrix matVP;
	matVP = mul(g_ViewMatrix, g_ProjMatrix);

	Out[0].vPosition = float4(In[0].vPosition - vRight + vUp, 1.f);
	Out[0].vPosition = mul(Out[0].vPosition, matVP);
	Out[0].vTexUV = float2(0.f, 0.f);
	Out[0].fAlpha = In[0].fAlpha;
	Out[0].vColor = In[0].vColor;

	Out[1].vPosition = float4(In[0].vPosition + vRight + vUp, 1.f);
	Out[1].vPosition = mul(Out[1].vPosition, matVP);
	Out[1].vTexUV = float2(1.f, 0.f);
	Out[1].fAlpha = In[0].fAlpha;
	Out[1].vColor = In[0].vColor;

	Out[2].vPosition = float4(In[0].vPosition + vRight - vUp, 1.f);
	Out[2].vPosition = mul(Out[2].vPosition, matVP);
	Out[2].vTexUV = float2(1.f, 1.f);
	Out[2].fAlpha = In[0].fAlpha;
	Out[2].vColor = In[0].vColor;

	Out[3].vPosition = float4(In[0].vPosition - vRight - vUp, 1.f);
	Out[3].vPosition = mul(Out[3].vPosition, matVP);
	Out[3].vTexUV = float2(0.f, 1.f);
	Out[3].fAlpha = In[0].fAlpha;
	Out[3].vColor = In[0].vColor;

	DataStream.Append(Out[0]);
	DataStream.Append(Out[1]);
	DataStream.Append(Out[2]);
	DataStream.RestartStrip();

	DataStream.Append(Out[0]);
	DataStream.Append(Out[2]);
	DataStream.Append(Out[3]);
	DataStream.RestartStrip();
}

struct PS_IN
{
	float4 vPosition : SV_POSITION;
	float2 vTexUV : TEXCOORD0;
	float fAlpha : TEXCOORD1;
	float3 vColor : TEXCOORD2;
};

struct PS_OUT
{
	float4 vColor : SV_TARGET0;
};

PS_OUT PS_MAIN(PS_IN In)
{
	PS_OUT Out = (PS_OUT)0;

	Out.vColor = g_DiffuseTexture.Sample(LinearSampler, In.vTexUV);
	Out.vColor.gba = Out.vColor.r;

	Out.vColor.a *= Out.vColor.r; /* Set Alpha based on Alpha Mask */
	Out.vColor.rgb *= In.vColor; /* Set Color */
	Out.vColor.rgb /= Out.vColor.a;

	Out.vColor.a *= In.fAlpha; /* Set Alpha from Input */

	if (Out.vColor.a <= g_fAlphaDiscard) /* Alpha Test */
		discard;

	return Out;
}

PS_OUT PS_GLOW(PS_IN In)
{
	PS_OUT Out = (PS_OUT)0;

	Out.vColor = g_DiffuseTexture.Sample(LinearSampler, In.vTexUV);
	Out.vColor.gba = Out.vColor.r;

	Out.vColor.a *= Out.vColor.r; /* Set Alpha based on Alpha Mask */
	Out.vColor.rgb *= g_vGlowColor;
	Out.vColor.rgb /= Out.vColor.a;

	Out.vColor.a *= In.fAlpha;

	if (Out.vColor.a == 0)
		discard;

	return Out;
}

technique11 DefaultTechnique
{
	pass Default
	{
		SetRasterizerState(RS_Default_NoCull);
		SetBlendState(BS_AlphaBlending, float4(0.f, 0.f, 0.f, 1.f), 0xffffffff);
		SetDepthStencilState(DSS_NoWrite, 0);

		VertexShader = compile vs_5_0 VS_MAIN();
		GeometryShader = compile gs_5_0 GS_MAIN();
		PixelShader = compile ps_5_0 PS_MAIN();
	}

	pass Glow
	{
		SetRasterizerState(RS_Default_NoCull);
		SetBlendState(BS_AlphaBlending, float4(0.f, 0.f, 0.f, 1.f), 0xffffffff);
		SetDepthStencilState(DSS_NoWrite, 0);

		VertexShader = compile vs_5_0 VS_MAIN();
		GeometryShader = compile gs_5_0 GS_MAIN();
		PixelShader = compile ps_5_0 PS_GLOW();
	}
}