
#include "Client_Shader_Defines.hpp"

matrix g_WorldMatrix, g_ViewMatrix, g_ProjMatrix;
texture2D g_DiffuseTexture;
texture2D g_NormalTexture;
texture2D g_DissolveTexture;

/* Water (https://www.youtube.com/watch?v=aVCfVs1oZSY&list=PLv8DnRaQOs5-ST_VDqgbbMRtzMtpK36Hy&index=38&t=1610s) */
float g_fScrollingSpeed = .05f;
float g_fScrollingTimer;
float g_fDissolveAlpha;
float4 g_WaterColorDeep = float4(.1f, .62f, .81f, 1.f);
float4 g_WaterColorShallow = float4(.1, .63f, .81f, 1.f);


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
	float4 vGlow : SV_TARGET3;
	float4 vAmbient : SV_TARGET4;
};

struct PS_OUT_SHADOW
{
	float4 vLightDepth : SV_TARGET0;
};

PS_OUT PS_MAIN(PS_IN In)
{
	PS_OUT Out = (PS_OUT)0;

	float4 vTextureNormal = g_NormalTexture.Sample(LinearSampler, In.vTexUV);
	float3 vNormal;

	vNormal = float3(vTextureNormal.x*2.f-1.f, vTextureNormal.y*2.f-1.f, sqrt(1 - vTextureNormal.x * vTextureNormal.x - vTextureNormal.y * vTextureNormal.y));
	vNormal = normalize(vNormal);
	float3x3 WorldMatrix = float3x3(In.vTangent, In.vBinormal, In.vNormal);
	vNormal = mul(vNormal, WorldMatrix);

	Out.vDiffuse = g_DiffuseTexture.Sample(LinearSampler, In.vTexUV);
	Out.vNormal = vector(vNormal * 0.5f + 0.5f, 0.f);
	Out.vDepth = vector(In.vProjPos.z / In.vProjPos.w, In.vProjPos.w / 1000.f, 0.f, 0.f);
	Out.vAmbient = float4(1.f, 1.f, 1.f, 1.f);

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
	Out.vAmbient = float4(0.f, 0.f, 0.f, 0.f);

	if (Out.vDiffuse.a <= .3f)
		discard;

	return Out;
}

PS_OUT PS_DISSOLVE(PS_IN In)
{
	PS_OUT Out = (PS_OUT)0;

	float4 vTextureNormal = g_NormalTexture.Sample(LinearSampler, In.vTexUV);
	float3 vNormal;

	vNormal = float3(vTextureNormal.x*2.f - 1.f, vTextureNormal.y*2.f - 1.f, sqrt(1 - vTextureNormal.x * vTextureNormal.x - vTextureNormal.y * vTextureNormal.y));
	vNormal = normalize(vNormal);
	float3x3 WorldMatrix = float3x3(In.vTangent, In.vBinormal, In.vNormal);
	vNormal = mul(vNormal, WorldMatrix);
	

	float4 vDissolve = g_DissolveTexture.Sample(LinearSampler, In.vTexUV);




	Out.vDiffuse = g_DiffuseTexture.Sample(LinearSampler, In.vTexUV);
	Out.vNormal = vector(vNormal * 0.5f + 0.5f, 0.f);
	Out.vDepth = vector(In.vProjPos.z / In.vProjPos.w, In.vProjPos.w / 1000.f, 0.f, 0.f);
	Out.vAmbient = float4(1.f, 1.f, 1.f, 1.f);
	if (vDissolve.r < g_fDissolveAlpha)
		discard;

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
}