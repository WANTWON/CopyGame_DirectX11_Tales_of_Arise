
#include "Client_Shader_Defines.hpp"

matrix			g_WorldMatrix, g_ViewMatrix, g_ProjMatrix;

vector			g_vCamPosition;

/* For.Material */
texture2D		g_DiffuseTexture[4];
texture2D		g_NormalTexture[4];

texture2D		g_BrushTexture;
float4			g_vBrushPos = float4(5.f, 0.f, 5.f, 1.f);
float			g_fBrushRange = 2.f;


texture2D		g_FilterTexture;

struct VS_IN
{
	float3		vPosition : POSITION;
	float3		vNormal : NORMAL;
	float2		vTexUV : TEXCOORD0;
	float3		vTangent : TANGENT;
};

struct VS_OUT
{
	float4		vPosition : SV_POSITION;
	float3		vNormal : NORMAL;
	float2		vTexUV : TEXCOORD0;
	float4		vWorldPos : TEXCOORD1;
	float4		vProjPos : TEXCOORD2;

	float3		vTangent : TANGENT;
	float3		vBinormal : BINORMAL;
};

VS_OUT VS_MAIN(VS_IN In)
{
	VS_OUT		Out = (VS_OUT)0;

	matrix		matWV, matWVP;

	matWV = mul(g_WorldMatrix, g_ViewMatrix);
	matWVP = mul(matWV, g_ProjMatrix);

	vector		vWorldNormal = mul(vector(In.vNormal, 0.f), g_WorldMatrix);
	vector		vWorldPos = mul(vector(In.vPosition, 1.f), g_WorldMatrix);

	Out.vPosition = mul(vector(In.vPosition, 1.f), matWVP);
	Out.vNormal = normalize(vWorldNormal).xyz;
	Out.vTexUV = In.vTexUV;
	Out.vWorldPos = vWorldPos;
	Out.vProjPos = Out.vPosition;

	Out.vTangent = normalize(mul(vector(In.vTangent, 0.f), g_WorldMatrix)).xyz;
	Out.vBinormal = cross(Out.vNormal, Out.vTangent);

	return Out;
}

struct PS_IN
{
	float4		vPosition : SV_POSITION;
	float3		vNormal : NORMAL;
	float2		vTexUV : TEXCOORD0;
	float4		vWorldPos : TEXCOORD1;
	float4		vProjPos : TEXCOORD2;

	float3		vTangent : TANGENT;
	float3		vBinormal : BINORMAL;
};

struct PS_OUT
{
	float4		vDiffuse : SV_TARGET0;
	float4		vNormal : SV_TARGET1;
	float4		vDepth : SV_TARGET2;
};

PS_OUT PS_MAIN(PS_IN In)
{
	PS_OUT		Out = (PS_OUT)0;

	vector		vDiffuse1 = g_DiffuseTexture[0].Sample(LinearSampler, In.vTexUV * 40.f);
	vector		vDiffuse2 = g_DiffuseTexture[1].Sample(LinearSampler, In.vTexUV * 30.f);
	vector		vDiffuse3 = g_DiffuseTexture[2].Sample(LinearSampler, In.vTexUV * 30.f);
	vector		vDiffuse4 = g_DiffuseTexture[3].Sample(LinearSampler, In.vTexUV * 30.f);

	vector		vFilter = g_FilterTexture.Sample(LinearSampler, In.vTexUV);

	vector		vMtrlDiffuse = vDiffuse2 * vFilter.r + vDiffuse3 * vFilter.g + vDiffuse4 * vFilter.b;
	Out.vDiffuse = (1 - vFilter.r - vFilter.g - vFilter.b)*vDiffuse1 + vMtrlDiffuse ;


	Out.vDiffuse.a = 1.f;

	/* -1 ~ 1 => 0 ~ 1*/

	float fFilterSum = min((vFilter.r + vFilter.g + vFilter.b), 1.f);
	float fFilterStrength = 1 - fFilterSum;


	vector		vNormal1 = g_NormalTexture[0].Sample(LinearSampler, In.vTexUV*fFilterStrength * 40.f);
	vector		vNormal2 = g_NormalTexture[1].Sample(LinearSampler, In.vTexUV*vFilter.r * 30.f);
	vector		vNormal3 = g_NormalTexture[2].Sample(LinearSampler, In.vTexUV*vFilter.g * 30.f);
	vector		vNormal4 = g_NormalTexture[3].Sample(LinearSampler, In.vTexUV*vFilter.b * 30.f);

	float3 vNormalA = normalize(float4(vNormal1.x*2.f - 1.f, vNormal1.y*2.f - 1.f, sqrt(1 - vNormal1.x * vNormal1.x - vNormal1.y * vNormal1.y), 0.f)).xyz;
	float3 vNormalB = normalize(float4(vNormal2.x*2.f - 1.f, vNormal2.y*2.f - 1.f, sqrt(1 - vNormal2.x * vNormal2.x - vNormal2.y * vNormal2.y), 0.f)).xyz;
	float3 vNormalC = normalize(float4(vNormal3.x*2.f - 1.f, vNormal3.y*2.f - 1.f, sqrt(1 - vNormal3.x * vNormal3.x - vNormal3.y * vNormal3.y), 0.f)).xyz;
	float3 vNormalD = normalize(float4(vNormal4.x*2.f - 1.f, vNormal4.y*2.f - 1.f, sqrt(1 - vNormal4.x * vNormal4.x - vNormal4.y * vNormal4.y), 0.f)).xyz;

	
	float3x3 WorldMatrix = float3x3(In.vTangent, In.vBinormal, In.vNormal);
	vNormalA = mul(vNormalA, WorldMatrix);
	vNormalB = mul(vNormalB, WorldMatrix);
	vNormalC = mul(vNormalC, WorldMatrix);
	vNormalD = mul(vNormalD, WorldMatrix);


	//float3 finalNormal = lerp(vNormalB, vFilter, vFilter.r) + lerp(vNormalC, vFilter, vFilter.g) + lerp(vNormalD, vFilter, vFilter.b);
	//vNormalA = lerp(vNormalA, vFilter, fFilterStrength);

	//Out.vNormal.xyz = vNormalA + finalNormal;
	//Out.vNormal = vector(Out.vNormal.xyz * 0.5f + 0.5f, 0.f);


	float3 vMtrlNormal = vNormalB * vFilter.r + vNormalC * vFilter.g + vNormalD * vFilter.b;
	vNormalA *= fFilterStrength;
	Out.vNormal.xyz = vMtrlNormal + vNormalA;
	Out.vNormal = vector(Out.vNormal.xyz * 0.5f + 0.5f, 0.f);

	/* near ~ far */
	/* 0 ~ far */
	/* 0 ~ 1 */
	Out.vDepth = vector(In.vProjPos.z / In.vProjPos.w, In.vProjPos.w / 1000.f, 0.f, 0.f);

	return Out;
}


technique11 DefaultTechnique
{
	pass Default
	{
		SetRasterizerState(RS_Default);
		SetBlendState(BS_Default, float4(0.f, 0.f, 0.f, 1.f), 0xffffffff);
		SetDepthStencilState(DSS_Default, 0);

		VertexShader = compile vs_5_0 VS_MAIN();
		GeometryShader = NULL;
		PixelShader = compile ps_5_0 PS_MAIN();
	}

}