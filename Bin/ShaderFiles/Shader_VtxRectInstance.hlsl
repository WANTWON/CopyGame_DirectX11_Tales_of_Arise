
#include "Client_Shader_Defines.hpp"

matrix g_WorldMatrix, g_ViewMatrix, g_ProjMatrix;
texture2D g_DiffuseTexture;

struct VS_IN
{
	float3 vPosition : POSITION;
	float2 vTexUV : TEXCOORD0;

	float4 vRight : TEXCOORD1;
	float4 vUp : TEXCOORD2;
	float4 vLook : TEXCOORD3;
	float4 vTranslation : TEXCOORD4;
};

struct VS_OUT
{
	float4 vPosition : SV_POSITION;
	float2 vTexUV : TEXCOORD0;
};

VS_OUT VS_MAIN(VS_IN In)
{
	VS_OUT Out = (VS_OUT)0;

	matrix matWV, matWVP;
	matWV = mul(g_WorldMatrix, g_ViewMatrix);
	matWVP = mul(matWV, g_ProjMatrix);

	float4x4 TransformMatrix = float4x4(In.vRight, In.vUp, In.vLook, In.vTranslation);

	vector vPosition = mul(vector(In.vPosition, 1.f), TransformMatrix);
	Out.vPosition = mul(vPosition, matWVP);
	Out.vTexUV = In.vTexUV;

	return Out;
}

struct PS_IN
{
	float4 vPosition : SV_POSITION;
	float2 vTexUV : TEXCOORD0;
};

struct PS_OUT
{
	float4 vColor : SV_TARGET0;
};

PS_OUT PS_MAIN(PS_IN In)
{
	PS_OUT Out = (PS_OUT)0;
	Out.vColor = g_DiffuseTexture.Sample(LinearSampler, In.vTexUV);
	Out.vColor.a = Out.vColor.r;

	return Out;
}

technique11 DefaultTechnique
{
	pass Default
	{
		SetRasterizerState(RS_Default);
		SetBlendState(BS_AlphaBlending, float4(0.f, 0.f, 0.f, 1.f), 0xffffffff);
		SetDepthStencilState(DSS_Default, 0);

		VertexShader = compile vs_5_0 VS_MAIN();
		GeometryShader = NULL;
		PixelShader = compile ps_5_0 PS_MAIN();
	}
}