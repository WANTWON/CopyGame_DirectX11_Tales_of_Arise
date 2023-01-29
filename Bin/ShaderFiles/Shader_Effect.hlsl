
#include "Client_Shader_Defines.hpp"

uint g_iWinX, g_iWinY;

matrix g_WorldMatrix, g_ViewMatrix, g_ProjMatrix;
texture2D g_DiffuseTexture;

/* For Distortion. */
texture2D g_BackBufferCopyTexture;
texture2D g_DistortionTexture;
texture2D g_NoiseTexture;
texture2D g_StrengthTexture;
float g_fDistortionStrength = 1.f;
float g_fDistortionSpeed = 10.f;
float g_fDistortionTimer;

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

	float4 fLerpAlpha = lerp(0, 1, Out.vColor.r);
	Out.vColor.a *= fLerpAlpha;
	Out.vColor.a *= In.fAlpha;

	return Out;
}

PS_OUT PS_DISTORTION(PS_IN In)
{
	PS_OUT Out = (PS_OUT)0;

	float2 vNewTexUV;
	vNewTexUV.x = In.vPosition.x / g_iWinX;
	vNewTexUV.y = In.vPosition.y / g_iWinY;

	float4 vNoise = g_NoiseTexture.Sample(LinearSampler, In.vTexUV + g_fDistortionTimer * g_fDistortionSpeed);
	float4 vFilter = g_StrengthTexture.Sample(LinearSampler, In.vTexUV);
	vNoise *= vFilter;

	float2 vNoisedUVs;
	vNoisedUVs.x = vNewTexUV.x + vNoise.r;
	vNoisedUVs.y = vNewTexUV.y + vNoise.r;

	vNewTexUV = lerp(vNewTexUV, vNoisedUVs, g_fDistortionStrength);

	/*vNewTexUV.x += (cos(vNoise.r * g_fDistortionTimer * g_fDistortionSpeed)) * vFilter * g_fDistortionStrength;
	vNewTexUV.y += (sin(vNoise.r * g_fDistortionTimer * g_fDistortionSpeed)) * vFilter * g_fDistortionStrength;*/

	Out.vColor = g_DistortionTexture.Sample(LinearSampler, vNewTexUV);
	//Out.vColor += 0.2f;

	return Out;
}

technique11 DefaultTechnique
{
	pass Default // 0
	{
		SetRasterizerState(RS_Default);
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