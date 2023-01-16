
#include "Client_Shader_Defines.hpp"

matrix g_WorldMatrix, g_ViewMatrix, g_ProjMatrix;
texture2D g_DiffuseTexture;
vector g_vCamPosition;

struct VS_IN
{
	float3 vPosition : POSITION;
	float2 vPSize : PSIZE;

	float4 vRight : TEXCOORD1;
	float4 vUp : TEXCOORD2;
	float4 vLook : TEXCOORD3;
	float4 vTranslation : TEXCOORD4;
};

struct VS_OUT
{
	float3 vPosition : POSITION;
	float2 vPSize : PSIZE;
};

VS_OUT VS_MAIN(VS_IN In)
{
	VS_OUT Out = (VS_OUT)0;
	
	float4x4 TransformMatrix = float4x4(In.vRight, In.vUp, In.vLook, In.vTranslation);

	vector vPosition = mul(vector(In.vPosition, 1.f), mul(TransformMatrix, g_WorldMatrix));
	Out.vPosition = vPosition.xyz;
	Out.vPSize.x = In.vPSize.x * length(In.vRight.xyz);
	Out.vPSize.y = In.vPSize.y * length(In.vUp.xyz);

	return Out;
}

struct GS_IN
{
	float3 vPosition : POSITION;
	float2 vPSize : PSIZE;
};

struct GS_OUT
{
	float4 vPosition : SV_POSITION;
	float2 vTexUV : TEXCOORD0;
};

/* Geometry Shader = Create Vertices */

/* Called depending on the primitive you want to draw. */
/* Point: Once per vertex. */
/* Line: Once every 2 vertices. */
/* Triangle: Once every 3 vertices. */
[maxvertexcount(20)]
void GS_MAIN(point GS_IN In[1], inout TriangleStream<GS_OUT> DataStream)
{
	GS_OUT Out[4];	

	float3 vLook = g_vCamPosition.xyz - In[0].vPosition;
	float3 vRight = normalize(cross(float3(0.f, 1.f, 0.f), vLook)) * (In[0].vPSize.x * 0.5f);
	float3 vUp = normalize(cross(vLook, vRight)) * (In[0].vPSize.y * 0.5f);

	matrix matVP;
	matVP = mul(g_ViewMatrix, g_ProjMatrix);

	Out[0].vPosition = float4(In[0].vPosition + vRight + vUp, 1.f);
	Out[0].vPosition = mul(Out[0].vPosition, matVP);
	Out[0].vTexUV = float2(0.f, 0.f);

	Out[1].vPosition = float4(In[0].vPosition - vRight + vUp, 1.f);
	Out[1].vPosition = mul(Out[1].vPosition, matVP);
	Out[1].vTexUV = float2(1.f, 0.f);

	Out[2].vPosition = float4(In[0].vPosition - vRight - vUp, 1.f);
	Out[2].vPosition = mul(Out[2].vPosition, matVP);
	Out[2].vTexUV = float2(1.f, 1.f);

	Out[3].vPosition = float4(In[0].vPosition + vRight - vUp, 1.f);
	Out[3].vPosition = mul(Out[3].vPosition, matVP);
	Out[3].vTexUV = float2(0.f, 1.f);

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
};

struct PS_OUT
{
	float4 vColor : SV_TARGET0;
};

PS_OUT PS_MAIN(PS_IN In)
{
	PS_OUT Out = (PS_OUT)0;

	Out.vColor = g_DiffuseTexture.Sample(LinearSampler, In.vTexUV);
	Out.vColor.rgb = float3(1.f, 0.f, 0.f);
		
	if (Out.vColor.a < 0.5f)
		discard;

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
		GeometryShader = compile gs_5_0 GS_MAIN();
		PixelShader = compile ps_5_0 PS_MAIN();
	}
}