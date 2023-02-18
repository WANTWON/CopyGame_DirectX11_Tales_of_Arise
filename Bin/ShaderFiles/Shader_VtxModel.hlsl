
#include "Client_Shader_Defines.hpp"

matrix g_WorldMatrix, g_ViewMatrix, g_ProjMatrix;

texture2D g_DiffuseTexture;
texture2D g_NormalTexture;
texture2D g_DepthTexture;

float4 g_vColor;
float g_fAlpha = 1.f;
float g_fTimer = 0.f;

/* Diffuse */
bool g_bDiffuse;

/* Mask */
bool g_bMask;
float g_fMaskSpeed;
float g_fMaskDirectionX;
float g_fMaskDirectionY;
texture2D g_MaskTexture;

/* Noise */
bool g_bNoise;
float g_fNoiseSpeed;
float g_fNoisePower;
float g_fNoiseDirectionX;
float g_fNoiseDirectionY;
texture2D g_NoiseTexture;

/* Dissolve */
bool g_bDissolve;
texture2D g_DissolveTexture;

/* Glow */
float3 g_vGlowColor;
float g_fGlowPower;

/* Water (https://www.youtube.com/watch?v=aVCfVs1oZSY&list=PLv8DnRaQOs5-ST_VDqgbbMRtzMtpK36Hy&index=38&t=1610s) */
float g_fScrollingSpeed = .05f;
float g_fScrollingTimer;
float g_fDissolveAlpha;
float4 g_WaterColorDeep = float4(0.3f, 0.3f, 0.8f, 1.f);
float4 g_WaterColorShallow = float4(0.3f, 0.3f, 0.8f, 1.f);

struct VS_IN
{
	float3 vPosition : POSITION;
	float3 vNormal : NORMAL;
	float2 vTexUV : TEXCOORD0;
	float3 vTangent : TANGENT;
};

struct VS_OUT
{
	float4 vPosition : SV_POSITION;
	float3 vNormal : NORMAL;
	float2 vTexUV : TEXCOORD0;
	float4 vProjPos : TEXCOORD1;
	float3 vTangent : TANGENT;
	float3 vBinormal : BINORMAL;
};

VS_OUT VS_MAIN(VS_IN In)
{
	VS_OUT Out = (VS_OUT)0;

	matrix matWV, matWVP;
	matWV = mul(g_WorldMatrix, g_ViewMatrix);
	matWVP = mul(matWV, g_ProjMatrix);

	vector vNormal = normalize(mul(vector(In.vNormal, 0.f), g_WorldMatrix));

	Out.vPosition = mul(vector(In.vPosition, 1.f), matWVP);
	Out.vNormal = vNormal.xyz;
	Out.vTexUV = In.vTexUV;
	Out.vProjPos = Out.vPosition;
	Out.vTangent = normalize(mul(vector(In.vTangent, 0.f), g_WorldMatrix)).xyz;
	Out.vBinormal = cross(Out.vNormal, Out.vTangent);

	return Out;
}

struct PS_IN
{
	float4 vPosition : SV_POSITION;
	float3 vNormal : NORMAL;
	float2 vTexUV : TEXCOORD0;
	float4 vProjPos : TEXCOORD1;
	float3 vTangent : TANGENT;
	float3 vBinormal : BINORMAL;
};

struct PS_OUT
{
	float4 vDiffuse : SV_TARGET0;
	float4 vNormal : SV_TARGET1;
	float4 vDepth : SV_TARGET2;
};

struct PS_OUT_SHADOW
{
	float4 vLightDepth : SV_TARGET0;
};

struct PS_EFFECT_OUT
{
	float4 vColor : SV_TARGET0;
	float4 vDepth : SV_TARGET2;
};

PS_OUT PS_MAIN(PS_IN In)
{
	PS_OUT Out = (PS_OUT)0;

	//float4 vTextureNormal = g_NormalTexture.Sample(LinearSampler, In.vTexUV);
	//float3 vNormal;

	////vNormal = float3(vTextureNormal.x*2.f-1.f, vTextureNormal.y*2.f-1.f, sqrt(1 - vTextureNormal.x * vTextureNormal.x - vTextureNormal.y * vTextureNormal.y));
	//vNormal = float3(vTextureNormal.x, vTextureNormal.y, sqrt(1 - vTextureNormal.x * vTextureNormal.x - vTextureNormal.y * vTextureNormal.y));
	//vNormal = normalize(vNormal);
	//float3x3 WorldMatrix = float3x3(In.vTangent, In.vBinormal, In.vNormal);
	//vNormal = mul(vNormal, WorldMatrix);

	float3	vNormal = g_NormalTexture.Sample(LinearSampler, In.vTexUV).xyz;

	vNormal = vNormal * 2.f - 1.f;

	float3x3	WorldMatrix = float3x3(In.vTangent, In.vBinormal, In.vNormal);

	vNormal = mul(vNormal, WorldMatrix);

	Out.vDiffuse = g_DiffuseTexture.Sample(LinearSampler, In.vTexUV);
	Out.vNormal = vector(vNormal * 0.5f + 0.5f, 0.f);
	Out.vDepth = vector(In.vProjPos.z / In.vProjPos.w, In.vProjPos.w / 1000.f, 0.f, 0.f);

	if (Out.vDiffuse.a <= 0.0f)
		discard;

	return Out;
}

PS_OUT_SHADOW PS_MAIN_SHADOW(PS_IN In)
{
	PS_OUT_SHADOW Out = (PS_OUT_SHADOW)0;

	Out.vLightDepth.r = In.vProjPos.w / 1000.f;
	Out.vLightDepth.a = 1.f;

	return Out;
}

PS_OUT PS_WATER(PS_IN In)
{
	PS_OUT Out = (PS_OUT)0;

	Out.vDiffuse = g_WaterColorDeep;
	Out.vDiffuse.a = 0.6f;

	float2 texCoord = In.vTexUV * 20 + (g_fScrollingTimer * g_fScrollingSpeed);
	float2 texCoordOffset = (float2(-In.vTexUV.x, In.vTexUV.y) * 20) + (g_fScrollingTimer * g_fScrollingSpeed); /* Wave Motion UVs */

	float4 vTextureNormal = g_NormalTexture.Sample(LinearSampler, texCoord);
	float4 vTextureNormalOffset = g_NormalTexture.Sample(LinearSampler, texCoordOffset);

	float3 vLerpNormal = normalize(lerp(vTextureNormal, vTextureNormalOffset, .5f));

	float3x3 WorldMatrix = float3x3(In.vTangent, In.vBinormal, In.vNormal);
	vLerpNormal = mul(vLerpNormal, WorldMatrix);
	Out.vNormal = vector(vLerpNormal * 0.5f + 0.5f, 0.f);

	Out.vDepth = vector(In.vProjPos.z / In.vProjPos.w, In.vProjPos.w / 1000.f, 0.f, 0.f);

	if (Out.vDiffuse.a <= .3f)
		discard;

	return Out;
}

PS_OUT PS_DISSOLVE(PS_IN In)
{
	PS_OUT Out = (PS_OUT)0;

	//float4 vTextureNormal = g_NormalTexture.Sample(LinearSampler, In.vTexUV);
	//float3 vNormal;

	//vNormal = float3(vTextureNormal.x, vTextureNormal.y, sqrt(1 - vTextureNormal.x * vTextureNormal.x - vTextureNormal.y * vTextureNormal.y));
	////vNormal = normalize(vNormal);
	//float3x3 WorldMatrix = float3x3(In.vTangent, In.vBinormal, In.vNormal);
	//vNormal = mul(vNormal, WorldMatrix);
	

	float3	vNormal = g_NormalTexture.Sample(LinearSampler, In.vTexUV).xyz;

	vNormal = vNormal * 2.f - 1.f;

	float3x3	WorldMatrix = float3x3(In.vTangent, In.vBinormal, In.vNormal);

	vNormal = mul(vNormal, WorldMatrix);


	float4 vDissolve = g_DissolveTexture.Sample(LinearSampler, In.vTexUV);

	Out.vDiffuse = g_DiffuseTexture.Sample(LinearSampler, In.vTexUV);
	Out.vDiffuse.a = Out.vDiffuse.r;
	Out.vNormal = vector(vNormal * 0.5f + 0.5f, 0.f);
	Out.vDepth = vector(In.vProjPos.z / In.vProjPos.w, In.vProjPos.w / 1000.f, 0.f, 0.f);
	
	if (vDissolve.r < g_fDissolveAlpha)
		discard;

	return Out;
}

PS_EFFECT_OUT PS_EFFECT(PS_IN In)
{
	PS_EFFECT_OUT Out = (PS_EFFECT_OUT)0;

	/* Diffuse Texture */
	if (g_bDiffuse)
	{
		Out.vColor = g_DiffuseTexture.Sample(LinearSampler, In.vTexUV);
	}

	/* Mask Texture */
	if (g_bMask)
	{
		float2 vOffsettedUV = In.vTexUV;
		vOffsettedUV.x += g_fMaskDirectionX * g_fMaskSpeed * g_fTimer;
		vOffsettedUV.y += g_fMaskDirectionY * g_fMaskSpeed * g_fTimer;

		float4 vMaskTexture = g_MaskTexture.Sample(LinearSampler, vOffsettedUV);

		Out.vColor.a = vMaskTexture.r;
		Out.vColor *= g_fAlpha;

		Out.vColor.rgb = g_vColor;
	}

	/* Noise Texture */
	if (g_bNoise)
	{
		float2 vOffsettedUV = In.vTexUV;
		vOffsettedUV.x += g_fNoiseDirectionX * g_fNoiseSpeed * g_fTimer;
		vOffsettedUV.y += g_fNoiseDirectionY * g_fNoiseSpeed * g_fTimer;

		float4 vNoiseTexture = g_NoiseTexture.Sample(LinearSampler, vOffsettedUV);

		float lerpValue = 0.5f + (sin(g_fTimer * 5.0f) * 0.5f);
		float vNoise = lerp(vNoiseTexture.r, vNoiseTexture.r * 1.5f, lerpValue);
		vNoise *= g_fNoisePower;

		Out.vColor.a *= vNoise;
	}

	if (Out.vColor.a == 0)
		discard;

	return Out;
}

PS_EFFECT_OUT PS_GLOW(PS_IN In)
{
	PS_EFFECT_OUT Out = (PS_EFFECT_OUT)0;

	float2 vOffsettedUV = In.vTexUV;
	vOffsettedUV.x += g_fMaskDirectionX * g_fMaskSpeed * g_fTimer;
	vOffsettedUV.y += g_fMaskDirectionY * g_fMaskSpeed * g_fTimer;

	Out.vColor = g_MaskTexture.Sample(LinearSampler, vOffsettedUV);
	Out.vColor.gba = Out.vColor.r;

	float3 vGlow = g_vGlowColor * g_fGlowPower;
	Out.vColor.rgb *= vGlow;
	Out.vColor.a *= g_fAlpha;

	if (g_bNoise)
	{
		float2 vOffsettedUV = In.vTexUV;
		vOffsettedUV.x += g_fNoiseDirectionX * g_fNoiseSpeed * g_fTimer;
		vOffsettedUV.y += g_fNoiseDirectionY * g_fNoiseSpeed * g_fTimer;

		float4 vNoiseTexture = g_NoiseTexture.Sample(LinearSampler, vOffsettedUV);

		float lerpValue = 0.5f + (sin(g_fTimer * 5.0f) * 0.5f);
		float vNoise = lerp(vNoiseTexture.r, vNoiseTexture.r * 1.5f, lerpValue);
		vNoise *= g_fNoisePower;

		Out.vColor.a *= vNoise;
	}

	if (Out.vColor.a == 0)
		discard;


	float2	vUV;

	vUV.x = (In.vProjPos.x / In.vProjPos.w) * 0.5f + 0.5f;
	vUV.y = (In.vProjPos.y / In.vProjPos.w) * -0.5f + 0.5f;

	vector	vDepthDesc = g_DepthTexture.Sample(LinearSampler, vUV);

	float	fViewZ = In.vProjPos.w;
	float	fOldViewZ = vDepthDesc.y*1000.f;

	Out.vColor.a = Out.vColor.a * (fOldViewZ - fViewZ);

	Out.vColor.a *= g_fAlpha;

	if (Out.vColor.a <= 0.00f)
		discard;

	return Out;
}

PS_EFFECT_OUT PS_DISTORTION(PS_IN In)
{
	PS_EFFECT_OUT Out = (PS_EFFECT_OUT)0;

	Out.vColor = float4(1.f, 0.f, 0.f, 1.f);

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

	pass DepthShadow // 1
	{
		SetRasterizerState(RS_Default);
		SetBlendState(BS_Default, float4(0.f, 0.f, 0.f, 1.f), 0xffffffff);
		SetDepthStencilState(DSS_Priority, 0);

		VertexShader = compile vs_5_0 VS_MAIN();
		GeometryShader = NULL;
		PixelShader = compile ps_5_0 PS_MAIN_SHADOW();
	}

	pass Water // 2
	{
		SetRasterizerState(RS_Default);
		SetBlendState(BS_AlphaBlending, float4(0.f, 0.f, 0.f, 1.f), 0xffffffff);
		SetDepthStencilState(DSS_Default, 0);

		VertexShader = compile vs_5_0 VS_MAIN();
		GeometryShader = NULL;
		PixelShader = compile ps_5_0 PS_WATER();
	}

	pass Dissolve // 3
	{
		SetRasterizerState(RS_Default);
		SetBlendState(BS_Default, float4(0.f, 0.f, 0.f, 1.f), 0xffffffff);
		SetDepthStencilState(DSS_Default, 0);

		VertexShader = compile vs_5_0 VS_MAIN();
		GeometryShader = NULL;
		PixelShader = compile ps_5_0 PS_DISSOLVE();
	}

	pass Effect // 4
	{
		SetRasterizerState(RS_Default_NoCull);
		SetBlendState(BS_AlphaBlending, float4(0.f, 0.f, 0.f, 1.f), 0xffffffff);
		SetDepthStencilState(DSS_Default, 0);

		VertexShader = compile vs_5_0 VS_MAIN();
		GeometryShader = NULL;
		PixelShader = compile ps_5_0 PS_EFFECT();
	}

	pass Glow // 5
	{
		SetRasterizerState(RS_Default_NoCull);
		SetBlendState(BS_AlphaBlending, float4(0.f, 0.f, 0.f, 1.f), 0xffffffff);
		SetDepthStencilState(DSS_Default, 0);

		VertexShader = compile vs_5_0 VS_MAIN();
		GeometryShader = NULL;
		PixelShader = compile ps_5_0 PS_GLOW();
	}

	pass Distortion // 6
	{
		SetRasterizerState(RS_Default_NoCull);
		SetBlendState(BS_Default, float4(0.f, 0.f, 0.f, 1.f), 0xffffffff);
		SetDepthStencilState(DSS_Default, 0);

		VertexShader = compile vs_5_0 VS_MAIN();
		GeometryShader = NULL;
		PixelShader = compile ps_5_0 PS_DISTORTION();
	}
}