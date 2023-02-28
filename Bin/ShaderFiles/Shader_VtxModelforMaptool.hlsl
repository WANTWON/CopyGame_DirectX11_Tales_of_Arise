
#include "Client_Shader_Defines.hpp"

matrix g_WorldMatrix, g_ViewMatrix, g_ProjMatrix;

texture2D g_DiffuseTexture;
texture2D g_NormalTexture;
texture2D g_SpecularTexture;

float4 g_vColor = float4(1.f,1.f,0.f,1.f);
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
	float3 vNormal : SV_TARGET1;
	float4 vDepth : SV_TARGET2;
	vector vLightDepth : SV_TARGET4;
};

struct PS_EFFECT_OUT
{
	float4 vColor : SV_TARGET0;
};

PS_OUT PS_MAIN(PS_IN In)
{
	PS_OUT Out = (PS_OUT)0;

	float3	vNormal = g_NormalTexture.Sample(LinearSampler, In.vTexUV).xyz;

	vNormal = vNormal * 2.f - 1.f;

	float3x3	WorldMatrix = float3x3(In.vTangent, In.vBinormal, In.vNormal);

	vNormal = mul(vNormal, WorldMatrix);

	Out.vDiffuse = g_DiffuseTexture.Sample(LinearSampler, In.vTexUV);
	Out.vNormal = vector(vNormal * 0.5f + 0.5f, 0.f);
	Out.vDepth = vector(In.vProjPos.z / In.vProjPos.w, In.vProjPos.w / 1000.f, 0.f, 0.f);
	Out.vDiffuse.a *= g_fAlpha;

	if (Out.vDiffuse.a <= 0.0f)
		discard;

	return Out;
}

PS_OUT PS_PICKED(PS_IN In)
{
	PS_OUT Out = (PS_OUT)0;

	float3	vNormal = g_NormalTexture.Sample(LinearSampler, In.vTexUV).xyz;

	vNormal = vNormal * 2.f - 1.f;

	float3x3	WorldMatrix = float3x3(In.vTangent, In.vBinormal, In.vNormal);

	vNormal = mul(vNormal, WorldMatrix);

	Out.vDiffuse = g_DiffuseTexture.Sample(LinearSampler, In.vTexUV);
	Out.vNormal = vector(vNormal * 0.5f + 0.5f, 0.f);
	Out.vDepth = vector(In.vProjPos.z / In.vProjPos.w, In.vProjPos.w / 1000.f, 0.f, 0.f);
	
	Out.vDiffuse.rgb += 0.2f;
	Out.vDiffuse.a *= g_fAlpha;

	if (Out.vDiffuse.a <= 0.0f)
		discard;

	return Out;
}

PS_OUT PS_SYMBOL(PS_IN In)
{
	PS_OUT Out = (PS_OUT)0;
	Out.vNormal = vector(In.vNormal.xyz * 0.5f + 0.5f, 0.f);
	Out.vDiffuse = g_vColor;

	return Out;
}

/* Used in Effect Tool (for Mesh Effect) */
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
		//Out.vColor.rgb /= Out.vColor.a;

		
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

	Out.vColor.a *= g_fAlpha;

	if (Out.vColor.a == 0)
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
		SetRasterizerState(RS_Default_NoCull);
		SetBlendState(BS_Default, float4(0.f, 0.f, 0.f, 1.f), 0xffffffff);
		SetDepthStencilState(DSS_Default, 0);

		VertexShader = compile vs_5_0 VS_MAIN();
		GeometryShader = NULL;
		PixelShader = compile ps_5_0 PS_MAIN();
	}

	pass Wire // 1
	{
		SetRasterizerState(RS_Wireframe);
		SetBlendState(BS_Default, float4(0.f, 0.f, 0.f, 1.f), 0xffffffff);
		SetDepthStencilState(DSS_Default, 0);

		VertexShader = compile vs_5_0 VS_MAIN();
		GeometryShader = NULL;
		PixelShader = compile ps_5_0 PS_MAIN();
	}

	pass Picked // 2
	{
		SetRasterizerState(RS_Default_NoCull);
		SetBlendState(BS_Default, float4(0.f, 0.f, 0.f, 1.f), 0xffffffff);
		SetDepthStencilState(DSS_Default, 0);

		VertexShader = compile vs_5_0 VS_MAIN();
		GeometryShader = NULL;
		PixelShader = compile ps_5_0 PS_PICKED();
	}

	pass PickingSymbol // 3
	{
		SetRasterizerState(RS_Default);
		SetBlendState(BS_Default, float4(0.f, 0.f, 0.f, 1.f), 0xffffffff);
		SetDepthStencilState(DSS_Default, 0);

		VertexShader = compile vs_5_0 VS_MAIN();
		GeometryShader = NULL;
		PixelShader = compile ps_5_0 PS_SYMBOL();
	}

	pass Effect // 4
	{
		SetRasterizerState(RS_Default_NoCull);
		SetBlendState(BS_AlphaBlending, float4(0.f, 0.f, 0.f, 1.f), 0xffffffff);
		SetDepthStencilState(DSS_NoWrite, 0);

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

	pass Distortion
	{
		SetRasterizerState(RS_Default_NoCull);
		SetBlendState(BS_Default, float4(0.f, 0.f, 0.f, 1.f), 0xffffffff);
		SetDepthStencilState(DSS_Default, 0);

		VertexShader = compile vs_5_0 VS_MAIN();
		GeometryShader = NULL;
		PixelShader = compile ps_5_0 PS_DISTORTION();
	}
}