
#include "Client_Shader_Defines.hpp"

matrix g_WorldMatrix, g_ViewMatrix, g_ProjMatrix;

float g_fMinRange = 5.f;
float g_fMaxRange = 10.f;
float4 g_PlayerPosition;

matrix g_BoneMatrices[400];

vector g_vColor = vector(1.f, 1.f, 1.f, 1);

texture2D g_DiffuseTexture;
texture2D g_NormalTexture;

/* Glow */
texture2D g_GlowTexture;
float4 g_vGlowColor;
bool g_bUseDiffuseColor;
float g_fGlowUpTimer;
float g_fGlowUpLifespan;

/* Dissolve */
texture2D g_DissolveTexture;
float g_DissolveTimer;
float g_DissolveLifespan;
vector g_DissolveColor = vector(1.f, .95f, .6f, 1.f);
vector g_DissolveHighlight = vector(.92f, .36f, .2f, 1);

/* VTF */
texture2D g_BoneTexture;

sampler PointVTFSampler = sampler_state
{
	Filter = MIN_MAG_MIP_POINT;
};

struct VS_IN
{
	float3 vPosition : POSITION;
	float3 vNormal : NORMAL;
	float2 vTexUV : TEXCOORD0;
	float3 vTangent : TANGENT;
	uint4 vBlendIndex : BLENDINDEX;
	float4 vBlendWeight : BLENDWEIGHT;
};

struct VS_OUT
{
	float4 vPosition : SV_POSITION;
	float3 vNormal : NORMAL;
	float2 vTexUV : TEXCOORD0;
	float4 vProjPos : TEXCOORD1; 
	float3 vTangent : TANGENT;
	float3 vBinormal : BINORMAL;
	float4 vWorldPos : TEXCOORD2;
};

VS_OUT VS_MAIN(VS_IN In)
{
	VS_OUT Out = (VS_OUT)0;

	matrix matWV, matWVP;
	matWV = mul(g_WorldMatrix, g_ViewMatrix);
	matWVP = mul(matWV, g_ProjMatrix);

	float fW = 1.f - (In.vBlendWeight.x + In.vBlendWeight.y + In.vBlendWeight.z);

	float2 vBoneTexUV;
	/*uint BoneTexWidth, BoneTexHeight;
	g_BoneTexture.GetDimensions(BoneTexWidth, BoneTexHeight);*/

	vBoneTexUV.x = (In.vBlendIndex.x / 16) / 64;
	vBoneTexUV.y = ((In.vBlendIndex.x % 16) * 4) / 64;
	
	float4x4 BoneMatrixX = float4x4(g_BoneTexture.SampleLevel(PointVTFSampler, vBoneTexUV, 0),
		g_BoneTexture.SampleLevel(PointVTFSampler, float2(vBoneTexUV.x, vBoneTexUV.y + 1), 0),
		g_BoneTexture.SampleLevel(PointVTFSampler, float2(vBoneTexUV.x, vBoneTexUV.y + 2), 0),
		g_BoneTexture.SampleLevel(PointVTFSampler, float2(vBoneTexUV.x, vBoneTexUV.y + 3), 0));

	vBoneTexUV.x = In.vBlendIndex.y / 16;
	vBoneTexUV.y = (In.vBlendIndex.y % 16) * 4;

	float4x4 BoneMatrixY = float4x4(g_BoneTexture.SampleLevel(PointVTFSampler, vBoneTexUV, 0),
		g_BoneTexture.SampleLevel(PointVTFSampler, float2(vBoneTexUV.x, vBoneTexUV.y + 1), 0),
		g_BoneTexture.SampleLevel(PointVTFSampler, float2(vBoneTexUV.x, vBoneTexUV.y + 2), 0),
		g_BoneTexture.SampleLevel(PointVTFSampler, float2(vBoneTexUV.x, vBoneTexUV.y + 3), 0));

	vBoneTexUV.x = In.vBlendIndex.z / 16;
	vBoneTexUV.y = (In.vBlendIndex.z % 16) * 4;

	float4x4 BoneMatrixZ = float4x4(g_BoneTexture.SampleLevel(PointVTFSampler, vBoneTexUV, 0),
		g_BoneTexture.SampleLevel(PointVTFSampler, float2(vBoneTexUV.x, vBoneTexUV.y + 1), 0),
		g_BoneTexture.SampleLevel(PointVTFSampler, float2(vBoneTexUV.x, vBoneTexUV.y + 2), 0),
		g_BoneTexture.SampleLevel(PointVTFSampler, float2(vBoneTexUV.x, vBoneTexUV.y + 3), 0));

	vBoneTexUV.x = In.vBlendIndex.w / 16;
	vBoneTexUV.y = (In.vBlendIndex.w % 16) * 4;

	float4x4 BoneMatrixW = float4x4(g_BoneTexture.SampleLevel(PointVTFSampler, vBoneTexUV, 0),
		g_BoneTexture.SampleLevel(PointVTFSampler, float2(vBoneTexUV.x, vBoneTexUV.y + 1), 0),
		g_BoneTexture.SampleLevel(PointVTFSampler, float2(vBoneTexUV.x, vBoneTexUV.y + 2), 0),
		g_BoneTexture.SampleLevel(PointVTFSampler, float2(vBoneTexUV.x, vBoneTexUV.y + 3), 0));

	matrix BoneMatrix = BoneMatrixX * In.vBlendWeight.x +
		BoneMatrixY * In.vBlendWeight.y +
		BoneMatrixZ * In.vBlendWeight.z +
		BoneMatrixW * fW;

	/*matrix BoneMatrix = g_BoneMatrices[In.vBlendIndex.x] * In.vBlendWeight.x +
		g_BoneMatrices[In.vBlendIndex.y] * In.vBlendWeight.y +
		g_BoneMatrices[In.vBlendIndex.z] * In.vBlendWeight.z +
		g_BoneMatrices[In.vBlendIndex.w] * fW;*/

	vector vPosition = mul(vector(In.vPosition, 1.f), BoneMatrix);
	vector vNormal = mul(vector(In.vNormal, 0.f), BoneMatrix);
	vNormal = normalize(mul(vNormal, g_WorldMatrix));

	/* 정점의 위치에 월드 뷰 투영행렬을 곱한다. 현재 정점은 ViewSpace에 존재하낟. */
	/* 투영행렬까지 곱하면 정점위치의 w에 뷰스페이스 상의 z를 보관한다. == Out.vPosition이 반드시 float4이어야하는 이유. */
	Out.vPosition = mul(vPosition, matWVP);
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

	float4 vWorldPos : TEXCOORD2;
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

struct PS_OUT_GLOW
{
	float4 vGlow : SV_TARGET0;
};

PS_OUT PS_MAIN(PS_IN In)
{
	PS_OUT Out = (PS_OUT)0;
	float4 vTextureNormal = g_NormalTexture.Sample(LinearSampler, In.vTexUV);
	float3 vNormal;

	vNormal = float3(vTextureNormal.x*2.f-1.f, vTextureNormal.y*2.f-1.f, sqrt(1 - vTextureNormal.x * vTextureNormal.x - vTextureNormal.y * vTextureNormal.y));

	float3x3 WorldMatrix = float3x3(In.vTangent, In.vBinormal, In.vNormal);
	vNormal = mul(vNormal, WorldMatrix);

	Out.vDiffuse = g_DiffuseTexture.Sample(LinearSampler, In.vTexUV);
	Out.vNormal = vector(vNormal * 0.5f + 0.5f, 0.f);
	Out.vDepth = vector(In.vProjPos.z / In.vProjPos.w, In.vProjPos.w / 1000.f, 0.f, 0.f);
	
	if (Out.vDiffuse.a <= 0.3f)
		discard;

	return Out;
}

PS_OUT_SHADOW PS_SHADOWDEPTH(PS_IN In)
{
	PS_OUT_SHADOW Out = (PS_OUT_SHADOW)0;

	Out.vLightDepth.r = In.vProjPos.w / 1000.f;
	Out.vLightDepth.a = 1.f;

	return Out;
}

PS_OUT PS_DISSOLVE(PS_IN In)
{
	PS_OUT Out = (PS_OUT)0;

	float4 vTextureNormal = g_NormalTexture.Sample(LinearSampler, In.vTexUV);
	float3 vNormal;
	vNormal = float3(vTextureNormal.x * 2.f - 1.f, vTextureNormal.y * 2.f - 1.f, sqrt(1 - vTextureNormal.x * vTextureNormal.x - vTextureNormal.y * vTextureNormal.y));

	float3x3 WorldMatrix = float3x3(In.vTangent, In.vBinormal, In.vNormal);
	vNormal = mul(vNormal, WorldMatrix);

	Out.vDiffuse = g_DiffuseTexture.Sample(LinearSampler, In.vTexUV);
	Out.vNormal = vector(vNormal * 0.5f + 0.5f, 0.f);
	Out.vDepth = vector(In.vProjPos.z / In.vProjPos.w, In.vProjPos.w / 1000.f, 0.f, 0.f);

	/* Dissolve with Highlight */
	float dissolveFactor = lerp(0, 1, g_DissolveTimer / g_DissolveLifespan); /* If dissolveFactor:
																			 == 0:	Should not Dissolve
																			 == 1:	Should Dissolve Everything. */
	float4 dissolveColor = g_DissolveTexture.Sample(LinearSampler, In.vTexUV);
	dissolveColor.a = dissolveColor.y;
	dissolveColor.yz = dissolveColor.x;

	float dissolveValue = dissolveColor.r - dissolveFactor; /* If dissolveValue:
															> .1:		No Dissolve
															0 ~ .1f:	Highlight
															<= 0:		Dissolve. */

	if (dissolveValue <= 0)
		discard;
	else if (dissolveValue < .1f)
	{
		float3 lerpColor = lerp(g_DissolveColor, g_DissolveHighlight, dissolveValue / .1f);
		Out.vDiffuse.rgb = lerpColor;
	}

	return Out;
}

PS_OUT_GLOW PS_GLOW(PS_IN In)
{
	PS_OUT_GLOW Out = (PS_OUT_GLOW)0;

	Out.vGlow = g_GlowTexture.Sample(LinearSampler, In.vTexUV);

	if (g_bUseDiffuseColor)
	{
		float4 vDiffuse = g_DiffuseTexture.Sample(LinearSampler, In.vTexUV);
		float4 vLerpGlow = lerp(vDiffuse, float4(1.f, 1.f, 1.f, 1.f), g_fGlowUpTimer / g_fGlowUpLifespan);
		Out.vGlow = vLerpGlow;
	}
	else
	{
		Out.vGlow.gba = Out.vGlow.r;
		Out.vGlow.rgb *= g_vGlowColor;
	}

	if (Out.vGlow.a == 0)
		discard;

	return Out;
}

PS_OUT_GLOW PS_GLOW_DISSOLVE(PS_IN In)
{
	PS_OUT_GLOW Out = (PS_OUT_GLOW)0;

	Out.vGlow = g_GlowTexture.Sample(LinearSampler, In.vTexUV);

	if (g_bUseDiffuseColor)
	{
		float4 vDiffuse = g_DiffuseTexture.Sample(LinearSampler, In.vTexUV);
		float4 vLerpGlow = lerp(vDiffuse, float4(1.f, 1.f, 1.f, 1.f), g_fGlowUpTimer / g_fGlowUpLifespan);
		Out.vGlow = vLerpGlow;
	}
	else
	{
		Out.vGlow.gba = Out.vGlow.r;
		Out.vGlow.rgb *= g_vGlowColor;
	}

	if (Out.vGlow.a == 0)
		discard;

	/* Dissolve Glow with Highlight */
	float dissolveFactor = lerp(0, 1, g_DissolveTimer / g_DissolveLifespan); /* If dissolveFactor:
																			 == 0:	Should not Dissolve
																			 == 1:	Should Dissolve Everything. */
	float4 dissolveColor = g_DissolveTexture.Sample(LinearSampler, In.vTexUV);
	dissolveColor.a = dissolveColor.y;
	dissolveColor.yz = dissolveColor.x;

	float dissolveValue = dissolveColor.r - dissolveFactor; /* If dissolveValue:
															> .1:		No Dissolve
															0 ~ .1f:	Highlight
															<= 0:		Dissolve. */

	if (dissolveValue <= 0)
		discard;
	else if (dissolveValue < .1f)
	{
		float3 lerpColor = lerp(g_DissolveColor, g_DissolveHighlight, dissolveValue / .1f);
		Out.vGlow.rgb = lerpColor;
	}

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

	pass Shadow_Depth // 1
	{
		SetRasterizerState(RS_Default);
		SetBlendState(BS_Default, float4(0.f, 0.f, 0.f, 1.f), 0xffffffff);
		SetDepthStencilState(DSS_Priority, 0);

		VertexShader = compile vs_5_0 VS_MAIN();
		GeometryShader = NULL;
		PixelShader = compile ps_5_0 PS_SHADOWDEPTH();
	}

	pass Dissolve // 2
	{
		SetRasterizerState(RS_Default);
		SetBlendState(BS_Default, float4(0.f, 0.f, 0.f, 1.f), 0xffffffff);
		SetDepthStencilState(DSS_Default, 0);

		VertexShader = compile vs_5_0 VS_MAIN();
		GeometryShader = NULL;
		PixelShader = compile ps_5_0 PS_DISSOLVE();
	}

	pass Glow // 3
	{
		SetRasterizerState(RS_Default);
		SetBlendState(BS_AlphaBlending, float4(0.f, 0.f, 0.f, 1.f), 0xffffffff);
		SetDepthStencilState(DSS_Default, 0);

		VertexShader = compile vs_5_0 VS_MAIN();
		GeometryShader = NULL;
		PixelShader = compile ps_5_0 PS_GLOW();
	}

	pass Glow_Dissolve // 4
	{
		SetRasterizerState(RS_Default);
		SetBlendState(BS_AlphaBlending, float4(0.f, 0.f, 0.f, 1.f), 0xffffffff);
		SetDepthStencilState(DSS_Default, 0);

		VertexShader = compile vs_5_0 VS_MAIN();
		GeometryShader = NULL;
		PixelShader = compile ps_5_0 PS_GLOW_DISSOLVE();
	}
}