
#include "Client_Shader_Defines.hpp"

matrix			g_WorldMatrix, g_ViewMatrix, g_ProjMatrix;

vector			g_vCamPosition;


texture2D		g_DiffuseTexture;
texture2D		g_OcculsionTexture;

float4			g_vLightDiffuse = float4(1.f, 1.f, 1.f, 1.f);
float4			g_vLightAmbient = float4(0.3f, 0.3f, 0.3f, 1.f);
float4			g_vLightSpecular = float4(1.f, 1.f, 1.f, 1.f);

/* For.Directional */
float4			g_vLightDir = float4(1.f, -1.f, 1.f, 0.f);
float4			g_vMtrlAmbient = float4(1.f, 1.f, 1.f, 1.f);


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
	float4		vNormal : NORMAL;
	float2		vTexUV : TEXCOORD0;
	float4		vWorldPos : TEXCOORD1;
	float		fSpecular : COLOR1;
	float		fShade : COLOR0;
};

/* DrawIndexed함수를 호출하면. */
/* 인덱스버퍼에 담겨있는 인덱스번째의 정점을 VS_MAIN함수에 인자로 던진다. VS_IN에 저장된다. */
/* 일반적으로 TriangleList로 그릴경우, 정점 세개를 각각 VS_MAIN함수의 인자로 던진다. */
VS_OUT VS_MAIN(VS_IN In)
{
	VS_OUT		Out = (VS_OUT)0;

	matrix		matWV, matWVP;

	matWV = mul(g_WorldMatrix, g_ViewMatrix);
	matWVP = mul(matWV, g_ProjMatrix);

	/* 정점의 위치에 월드 뷰 투영행렬을 곱한다. 현재 정점은 ViewSpace에 존재하낟. */
	/* 투영행렬까지 곱하면 정점위치의 w에 뷰스페이스 상의 z를 보관한다. == Out.vPosition이 반드시 float4이어야하는 이유. */
	Out.vPosition = mul(vector(In.vPosition, 1.f), matWVP);
	Out.vTexUV = In.vTexUV;

	vector		vWorldNormal = mul(vector(In.vNormal, 0.f), g_WorldMatrix);
	vector		vWorldPos = mul(vector(In.vPosition, 1.f), g_WorldMatrix);
	vector		vLook = vWorldPos - g_vCamPosition;
	vector		vReflect = reflect(normalize(g_vLightDir), normalize(vWorldNormal));

	Out.fSpecular = pow(max(dot(normalize(vLook) * -1.f, normalize(vReflect)), 0.f), 20);
	Out.vWorldPos = vWorldPos;
	Out.vNormal = vWorldNormal;
	Out.fShade = max(dot(normalize(g_vLightDir) * -1.f, normalize(vWorldNormal)), 0.f);

	return Out;
}


struct PS_IN
{
	float4		vPosition : SV_POSITION;
	float4		vNormal : NORMAL;
	float2		vTexUV : TEXCOORD0;
	float4		vWorldPos : TEXCOORD1;
	float		fSpecular : COLOR1;
	float		fShade : COLOR0;
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

	vector		vMtrlDiffuse = g_DiffuseTexture.Sample(LinearSampler, In.vTexUV);
	vector		vMtrlOcculsion = g_OcculsionTexture.Sample(LinearSampler, In.vTexUV);

	Out.vColor = (g_vLightDiffuse * vMtrlDiffuse) *saturate(In.fShade + g_vLightAmbient * g_vMtrlAmbient)
		+ (g_vLightSpecular * vMtrlOcculsion) * In.fSpecular;


	if (Out.vColor.a <= 0.3f)
		discard;

	return Out;
}

PS_OUT PS_PICKED(PS_IN In)
{
	PS_OUT		Out = (PS_OUT)0;

	Out.vColor = g_DiffuseTexture.Sample(LinearSampler, In.vTexUV);

	Out.vColor.rgb += 0.2f;

	if (Out.vColor.a <= 0.3f)
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
		GeometryShader = NULL;
		PixelShader = compile ps_5_0 PS_MAIN();
	}

	pass Wire
	{
		SetRasterizerState(RS_Wireframe);
		SetBlendState(BS_Default, float4(0.f, 0.f, 0.f, 1.f), 0xffffffff);
		SetDepthStencilState(DSS_Default, 0);

		VertexShader = compile vs_5_0 VS_MAIN();
		GeometryShader = NULL;
		PixelShader = compile ps_5_0 PS_MAIN();
	}

	pass Picked
	{
		SetRasterizerState(RS_Default);
		SetBlendState(BS_Default, float4(0.f, 0.f, 0.f, 1.f), 0xffffffff);
		SetDepthStencilState(DSS_Default, 0);

		VertexShader = compile vs_5_0 VS_MAIN();
		GeometryShader = NULL;
		PixelShader = compile ps_5_0 PS_PICKED();
	}

}