
#include "Client_Shader_Defines.hpp"

matrix			g_WorldMatrix, g_ViewMatrix, g_ProjMatrix;
texture2D		g_DiffuseTexture;
vector			g_vCamPosition;
float g_fCurrentMp;
float g_fMaxMp;

vector camlook;


//float g_fUVx;

struct VS_IN
{
	float3		vPosition : POSITION;
	float2		vPSize : PSIZE;

	float4		vRight : TEXCOORD0;
	float4		vUp : TEXCOORD1;
	float4		vLook : TEXCOORD2;
	float4		vTranslation : TEXCOORD3;
};

struct VS_OUT
{
	float3		vPosition : POSITION;
	float2		vPSize : PSIZE;
};

VS_OUT VS_MAIN(VS_IN In)
{
	VS_OUT		Out = (VS_OUT)0;

	float4x4	TransformMatrix = float4x4(In.vRight, In.vUp, In.vLook, In.vTranslation);

	vector		vPosition = mul(vector(In.vPosition, 1.f), mul(TransformMatrix, g_WorldMatrix));

	Out.vPosition = vPosition.xyz;
	Out.vPSize.x = In.vPSize.x * length(In.vRight.xyz);
	Out.vPSize.y = In.vPSize.y * length(In.vUp.xyz);

	return Out;
}

struct GS_IN
{
	float3		vPosition : POSITION;
	float2		vPSize : PSIZE;
};

struct GS_OUT
{
	float4		vPosition : SV_POSITION;
	float2		vTexUV : TEXCOORD0;
};

/* 지오메트리셰이더 == 기하셰이더. */
/* 정점을 생성한다. */

/* 그리고자하는 프리미티브에 의존하여 호출된다. */
/* Point : 정점 한개당 1번호출. */
/* Line : 정점 두개당 1번호출. */
/* Triangle : 정점세개당 한번호출 */
[maxvertexcount(20)]
void GS_MPguage(point GS_IN In[1], inout TriangleStream<GS_OUT> DataStream)
{
	GS_OUT			Out[4];

	float3			vLook = camlook;
	float3			vRight = normalize(cross(float3(0.f, 1.f, 0.f), vLook)) * (In[0].vPSize.x * 0.3f);
	float3			vUp = normalize(cross(vRight, vLook)) * (In[0].vPSize.y * 0.3f);
	
	matrix			matVP;

	matVP = mul(g_ViewMatrix, g_ProjMatrix);

	Out[0].vPosition = float4(In[0].vPosition + (vRight * g_fMaxMp) - vUp, 1.f);
	Out[0].vPosition = mul(Out[0].vPosition, matVP);
	Out[0].vTexUV = float2(0.f, 0.f);

	Out[1].vPosition = float4(In[0].vPosition - (vRight * g_fMaxMp) - vUp, 1.f);
	Out[1].vPosition = mul(Out[1].vPosition, matVP);
	Out[1].vTexUV = float2(g_fMaxMp, 0.f);

	Out[2].vPosition = float4(In[0].vPosition - (vRight * g_fMaxMp) + vUp, 1.f);
	Out[2].vPosition = mul(Out[2].vPosition, matVP);
	Out[2].vTexUV = float2(g_fMaxMp, 1.f);

	Out[3].vPosition = float4(In[0].vPosition + (vRight * g_fMaxMp) + vUp, 1.f);
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
	float4		vPosition : SV_POSITION;
	float2		vTexUV : TEXCOORD0;
};

struct PS_OUT
{
	float4		vColor : SV_TARGET0;
};

/* 이렇게 만들어진 픽셀을 PS_MAIN함수의 인자로 던진다. */
/* 리턴하는 색은 Target0 == 장치에 0번째에 바인딩되어있는 렌더타겟(일반적으로 백버퍼)에 그린다. */
/* 그래서 백버퍼에 색이 그려진다. */
PS_OUT PS_MAIN(PS_IN In)
{
	PS_OUT		Out = (PS_OUT)0;

	Out.vColor = g_DiffuseTexture.Sample(LinearSampler, In.vTexUV);

	/*Out.vColor.rgb = float3(1.f, 0.f, 0.f);*/

	/*if (Out.vColor.a < 0.3f)
	discard;*/

	Out.vColor.r = 0.f;
	Out.vColor.g = 0.f;
	Out.vColor.b = 0.f;

	if (Out.vColor.a < 0.5f)
		discard;

	return Out;
}


PS_OUT PS_MPGUAGE(PS_IN In)
{
	PS_OUT		Out = (PS_OUT)0;

	if (g_fCurrentMp / g_fMaxMp < In.vTexUV.x)
		discard;

	Out.vColor = g_DiffuseTexture.Sample(LinearSampler, In.vTexUV);

	//Out.vColor.a -= g_fAlpha;

	Out.vColor.r = 0.0589019607843137f;
	Out.vColor.g = 0.942708f;
	Out.vColor.b = 0.83441f;

	if (Out.vColor.a < 0.5f)
		discard;

	return Out;
}

technique11 DefaultTechnique
{
	pass MPGuageblue
	{
		SetRasterizerState(RS_Default);
		SetBlendState(BS_Default, float4(0.f, 0.f, 0.f, 1.f), 0xffffffff);
		SetDepthStencilState(DSS_Default, 0);

		VertexShader = compile vs_5_0 VS_MAIN();
		GeometryShader = compile gs_5_0 GS_MPguage();
		PixelShader = compile ps_5_0 PS_MPGUAGE();
	}

	pass MPGuageblack
	{
		SetRasterizerState(RS_Default);
		SetBlendState(BS_Default, float4(0.f, 0.f, 0.f, 1.f), 0xffffffff);
		SetDepthStencilState(DSS_Default, 0);

		VertexShader = compile vs_5_0 VS_MAIN();
		GeometryShader = compile gs_5_0 GS_MPguage();
		PixelShader = compile ps_5_0 PS_MAIN();
	}

	
}