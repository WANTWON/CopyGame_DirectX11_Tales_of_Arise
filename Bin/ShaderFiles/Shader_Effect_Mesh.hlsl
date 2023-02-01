
#include "Client_Shader_Defines.hpp"

matrix g_WorldMatrix, g_ViewMatrix, g_ProjMatrix;

float4 g_vColor = float4(1.f, 0.f, 0.f, 1.f);
float g_fAlpha = 1.f;

texture2D g_MaskTexture;
texture2D g_NoiseTexture;
texture2D g_DissolveTexture;

struct VS_IN
{
	float3 vPosition : POSITION;
	float2 vTexUV : TEXCOORD0;
};

struct VS_OUT
{
	float4 vPosition : SV_POSITION;
	float2 vTexUV : TEXCOORD0;
	float4 vProjPos : TEXCOORD1;
};

VS_OUT VS_MAIN(VS_IN In)
{
	VS_OUT Out = (VS_OUT)0;

	matrix matWV, matWVP;
	matWV = mul(g_WorldMatrix, g_ViewMatrix);
	matWVP = mul(matWV, g_ProjMatrix);

	Out.vPosition = mul(vector(In.vPosition, 1.f), matWVP);
	Out.vTexUV = In.vTexUV;
	Out.vProjPos = Out.vPosition;

	return Out;
}

struct PS_IN
{
	float4 vPosition : SV_POSITION;
	float2 vTexUV : TEXCOORD0;
	float4 vProjPos : TEXCOORD1;
};

struct PS_OUT
{
	float4 vColor : SV_TARGET0;
};

PS_OUT PS_MAIN(PS_IN In)
{
	PS_OUT Out = (PS_OUT)0;

	Out.vColor = g_MaskTexture.Sample(LinearSampler, In.vTexUV);
	/*Out.vColor.a = Out.vColor.r;
	Out.vColor.rgb = float3(1.f, 1.f, 1.f);
*/
	/* Soft Effect */
	/*float2	vUV;

	vUV.x = (In.vProjPos.x / In.vProjPos.w) * 0.5f + 0.5f;
	vUV.y = (In.vProjPos.y / In.vProjPos.w) * -0.5f + 0.5f;

	vector	vDepthDesc = g_DepthTexture.Sample(LinearSampler, vUV);

	float	fViewZ = In.vProjPos.w;
	float	fOldViewZ = vDepthDesc.y * 500.f;

	Out.vColor.a = Out.vColor.a * (fOldViewZ - fViewZ);*/

	return Out;
}

PS_OUT PS_DISSOLVE(PS_IN In)
{
	PS_OUT Out = (PS_OUT)0;
	
	/*float4 vTextureNormal = g_NormalTexture.Sample(LinearSampler, In.vTexUV);
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
		discard;*/

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

	pass Dissolve // 1
	{
		SetRasterizerState(RS_Default_NoCull);
		SetBlendState(BS_AlphaBlending, float4(0.f, 0.f, 0.f, 1.f), 0xffffffff);
		SetDepthStencilState(DSS_Default, 0);

		VertexShader = compile vs_5_0 VS_MAIN();
		GeometryShader = NULL;
		PixelShader = compile ps_5_0 PS_DISSOLVE();
	}
}