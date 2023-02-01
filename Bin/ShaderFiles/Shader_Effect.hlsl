
#include "Client_Shader_Defines.hpp"

uint g_iWinX, g_iWinY;

matrix g_WorldMatrix, g_ViewMatrix, g_ProjMatrix;
texture2D g_DiffuseTexture;

float3 g_vColor;
float g_fAlpha;
float g_fAlphaDiscard;
texture2D g_MaskTexture;
texture2D g_NoiseTexture;
texture2D g_DissolveTexture;
float g_fNoiseSpeed;
float g_fNoisePower;
float g_fTimer = 0.f;

/* For.Distortion */
texture2D g_StrengthTexture;

struct VS_IN
{
	float3 vPosition : POSITION;
	float2 vTexUV : TEXCOORD0;
	float fAlpha : TEXCOORD1;
};

struct VS_OUT
{
	float4 vPosition : SV_POSITION;
	float2 vTexUV : TEXCOORD0;
	float fAlpha : TEXCOORD1;
};

VS_OUT VS_MAIN(VS_IN In)
{
	VS_OUT Out = (VS_OUT)0;

	matrix matWV, matWVP;

	matWV = mul(g_WorldMatrix, g_ViewMatrix);
	matWVP = mul(matWV, g_ProjMatrix);

	Out.vPosition = mul(vector(In.vPosition, 1.f), matWVP);
	Out.vTexUV = In.vTexUV;
	Out.fAlpha = In.fAlpha;

	return Out;
}

struct PS_IN
{
	float4 vPosition : SV_POSITION;
	float2 vTexUV : TEXCOORD0;
	float fAlpha : TEXCOORD1;
};

struct PS_OUT
{
	float4 vColor : SV_TARGET0;
};

PS_OUT PS_MAIN(PS_IN In)
{
	PS_OUT Out = (PS_OUT)0;
	Out.vColor = g_DiffuseTexture.Sample(LinearSampler, In.vTexUV);

	return Out;
}

PS_OUT PS_ALPHAMASK(PS_IN In)
{
	PS_OUT Out = (PS_OUT)0;
	Out.vColor = g_DiffuseTexture.Sample(LinearSampler, In.vTexUV);
	Out.vColor.gba = Out.vColor.r;

	Out.vColor.a *= Out.vColor.r; // Set Alpha based on Alpha Mask
	Out.vColor.rgb *= g_vColor; // Set Color

	Out.vColor.a *= In.fAlpha; // Set Alpha from Input

	if (Out.vColor.a < g_fAlphaDiscard) // Alpha Test
		discard;

	return Out;
}

PS_OUT PS_DISTORTION(PS_IN In)
{
	PS_OUT Out = (PS_OUT)0;
	Out.vColor.r = 1.f;

	float4 vFilter = g_StrengthTexture.Sample(LinearSampler, In.vTexUV);
	Out.vColor.r *= vFilter.r;

	return Out;
}

technique11 DefaultTechnique
{
	pass Default // 0
	{
		SetRasterizerState(RS_Default_NoCull);
		SetBlendState(BS_Default, float4(0.f, 0.f, 0.f, 1.f), 0xffffffff);
		SetDepthStencilState(DSS_Default, 0);

		VertexShader = compile vs_5_0 VS_MAIN();
		GeometryShader = NULL;
		PixelShader = compile ps_5_0 PS_MAIN();
	}

	pass AlphaMask // 1
	{
		SetRasterizerState(RS_Default_NoCull);
		SetBlendState(BS_AlphaBlending, float4(0.f, 0.f, 0.f, 1.f), 0xffffffff);
		SetDepthStencilState(DSS_Default, 0);

		VertexShader = compile vs_5_0 VS_MAIN();
		GeometryShader = NULL;
		PixelShader = compile ps_5_0 PS_ALPHAMASK();
	}

	pass Distortion // 2
	{
		SetRasterizerState(RS_Default_NoCull);
		SetBlendState(BS_Default, float4(0.f, 0.f, 0.f, 1.f), 0xffffffff);
		SetDepthStencilState(DSS_Default, 0);

		VertexShader = compile vs_5_0 VS_MAIN();
		GeometryShader = NULL;
		PixelShader = compile ps_5_0 PS_DISTORTION();
	}
}