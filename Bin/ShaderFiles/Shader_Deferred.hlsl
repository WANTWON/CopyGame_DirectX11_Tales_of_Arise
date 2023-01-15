
matrix g_WorldMatrix, g_ViewMatrix, g_ProjMatrix;
matrix g_ViewMatrixInv, g_ProjMatrixInv, g_LightViewMatrix, g_LightProjMatrix;
	   
vector g_vCamPosition;

vector g_vLightDir;
vector g_vLightPos;
float g_fLightRange;
vector g_vLightDiffuse;
vector g_vLightAmbient;
vector g_vLightSpecular;

vector g_vMtrlAmbient = vector(1.f, 1.f, 1.f, 1.f);
vector g_vMtrlSpecular = vector(1.f, 1.f, 1.f, 1.f);

texture2D g_DiffuseTexture;
texture2D g_NormalTexture;
texture2D g_DepthTexture;
texture2D g_ShadeTexture;
texture2D g_SpecularTexture;
texture2D g_ShadowDepthTexture;
texture2D g_GlowTexture;

float g_fWinSizeX = 1280.f;
float g_fWinSizeY = 720.f;

sampler LinearSampler = sampler_state
{
	filter = min_mag_mip_Linear;
};

sampler DepthSampler = sampler_state
{
	filter = min_mag_Linear_mip_point;
	AddressU = clamp;
	AddressV = clamp;
};

struct VS_IN
{
	float3 vPosition : POSITION;
	float2 vTexUV : TEXCOORD0;
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

	Out.vPosition = mul(vector(In.vPosition, 1.f), matWVP);
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

/* 이렇게 만들어진 픽셀을 PS_MAIN함수의 인자로 던진다. */
/* 리턴하는 색은 Target0 == 장치에 0번째에 바인딩되어있는 렌더타겟(일반적으로 백버퍼)에 그린다. */
/* 그래서 백버퍼에 색이 그려진다. */
PS_OUT PS_MAIN(PS_IN In)
{
	PS_OUT Out = (PS_OUT)0;

	Out.vColor = g_DiffuseTexture.Sample(LinearSampler, In.vTexUV);

	return Out;
}

struct PS_OUT_LIGHT
{
	float4 vShade : SV_TARGET0;
	float4 vSpecular : SV_TARGET1;
};

PS_OUT_LIGHT PS_MAIN_LIGHT_DIRECTIONAL(PS_IN In)
{
	PS_OUT_LIGHT Out = (PS_OUT_LIGHT)0;

	/* 0 ~ 1 => -1 ~ 1*/
	vector vNormalDesc = g_NormalTexture.Sample(LinearSampler, In.vTexUV);
	vector vDepthDesc = g_DepthTexture.Sample(LinearSampler, In.vTexUV);

	float fViewZ = vDepthDesc.y * 500.f;

	vector vNormal = vector(vNormalDesc.xyz * 2.f - 1.f, 0.f);

	Out.vShade = g_vLightDiffuse * (saturate(dot(normalize(g_vLightDir) * -1.f, normalize(vNormal))) + (g_vLightAmbient * g_vMtrlAmbient));
	Out.vShade.a = 1.f;

	vector vWorldPos = (vector)0.f;

	/* 투영 공간상의 위치를 구했다. */
	/* 투영 공간 == 로컬점의위치 * 월드행렬 * 뷰행렬 * 투영행렬 / w */
	vWorldPos.x = In.vTexUV.x * 2.f - 1.f;
	vWorldPos.y = In.vTexUV.y * -2.f + 1.f;
	vWorldPos.z = vDepthDesc.r;
	vWorldPos.w = 1.0f;

	/* 로컬점의위치 * 월드행렬 * 뷰행렬 * 투영행렬 */
	vWorldPos *= fViewZ;

	/* 뷰 공간상의 위치르 ㄹ구한다. */
	/* 로컬점의위치 * 월드행렬 * 뷰행렬  */
	vWorldPos = mul(vWorldPos, g_ProjMatrixInv);

	/* 로컬점의위치 * 월드행렬   */
	vWorldPos = mul(vWorldPos, g_ViewMatrixInv);

	vector vReflect = reflect(normalize(g_vLightDir), normalize(vNormal));
	vector vLook = vWorldPos - g_vCamPosition;

	Out.vSpecular = (g_vLightSpecular * g_vMtrlSpecular) * pow(saturate(dot(normalize(vReflect) * -1.f, normalize(vLook))), 30.f);
	Out.vSpecular.a = 0.f;
	return Out;
}

PS_OUT_LIGHT PS_MAIN_LIGHT_POINT(PS_IN In)
{
	PS_OUT_LIGHT Out = (PS_OUT_LIGHT)0;

	/* 0 ~ 1 => -1 ~ 1*/
	vector vNormalDesc = g_NormalTexture.Sample(LinearSampler, In.vTexUV);
	vector vDepthDesc = g_DepthTexture.Sample(LinearSampler, In.vTexUV);

	float fViewZ = vDepthDesc.y * 500.f;

	vector vWorldPos = (vector)0.f;

	/* 투영 공간상의 위치를 구했다. */
	/* 투영 공간 == 로컬점의위치 * 월드행렬 * 뷰행렬 * 투영행렬 / w */
	vWorldPos.x = In.vTexUV.x * 2.f - 1.f;
	vWorldPos.y = In.vTexUV.y * -2.f + 1.f;
	vWorldPos.z = vDepthDesc.r;
	vWorldPos.w = 1.0f;

	/* 로컬점의위치 * 월드행렬 * 뷰행렬 * 투영행렬 */
	vWorldPos *= fViewZ;

	/* 뷰 공간상의 위치르 ㄹ구한다. */
	/* 로컬점의위치 * 월드행렬 * 뷰행렬  */
	vWorldPos = mul(vWorldPos, g_ProjMatrixInv);

	/* 로컬점의위치 * 월드행렬   */
	vWorldPos = mul(vWorldPos, g_ViewMatrixInv);

	vector vNormal = vector(vNormalDesc.xyz * 2.f - 1.f, 0.f);
	vector vLightDir = vWorldPos - g_vLightPos;
	float fDistance = length(vLightDir);
	float fAtt = saturate((g_fLightRange - fDistance) / g_fLightRange);

	Out.vShade = g_vLightDiffuse * (saturate(dot(normalize(vLightDir) * -1.f, normalize(vNormal))) + (g_vLightAmbient * g_vMtrlAmbient));
	Out.vShade *= fAtt;
	Out.vShade.a = 1.f;

	vector vReflect = reflect(normalize(vLightDir), normalize(vNormal));
	vector vLook = vWorldPos - g_vCamPosition;

	Out.vSpecular = (g_vLightSpecular * g_vMtrlSpecular) * pow(saturate(dot(normalize(vReflect) * -1.f, normalize(vLook))), 30.f);
	Out.vSpecular *= fAtt;

	Out.vSpecular.a = 0.f;

	return Out;
}

PS_OUT PS_MAIN_BLEND(PS_IN In)
{
	PS_OUT Out = (PS_OUT)0;

	vector vDiffuse = g_DiffuseTexture.Sample(LinearSampler, In.vTexUV);
	vector vShade = g_ShadeTexture.Sample(LinearSampler, In.vTexUV);
	vector vSpecular = g_SpecularTexture.Sample(LinearSampler, In.vTexUV);
	vector vGlow = g_GlowTexture.Sample(LinearSampler, In.vTexUV);

	Out.vColor = vDiffuse * vShade + vSpecular + vGlow;

	vector vDepthInfo = g_DepthTexture.Sample(DepthSampler, In.vTexUV);
	float fViewZ = vDepthInfo.y * 500.f;

	vector vPosition;
	vPosition.x = (In.vTexUV.x * 2.f - 1.f) * fViewZ;
	vPosition.y = (In.vTexUV.y * -2.f + 1.f) * fViewZ;
	vPosition.z = vDepthInfo.x * fViewZ;
	vPosition.w = fViewZ;

	// View
	vPosition = mul(vPosition, g_ProjMatrixInv);

	// World
	vPosition = mul(vPosition, g_ViewMatrixInv);

	vPosition = mul(vPosition, g_LightViewMatrix);

	vector vUVPos = mul(vPosition, g_LightProjMatrix);
	float2 vNewUV;

	vNewUV.x = (vUVPos.x / vUVPos.w) * 0.5f + 0.5f;
	vNewUV.y = (vUVPos.y / vUVPos.w) * -0.5f + 0.5f;

	vector vShadowDepthInfo = g_ShadowDepthTexture.Sample(DepthSampler, vNewUV);

	if (vPosition.z - 0.2f > vShadowDepthInfo.r * 500.f)
		Out.vColor -= vector(0.1, 0.1f, 0.1f, 0.f);

	if (Out.vColor.a <= 0.f)
		discard;

	return Out;
}

PS_OUT PS_HORIZONTAL_BLUR(PS_IN In)
{
	PS_OUT Out = (PS_OUT)0;

	float weight0, weight1, weight2, weight3, weight4;
	float normalization; 
	float4 color;

	// Create the weights that each neighbor pixel will contribute to the blur.
	weight0 = 1.0f;
	weight1 = 0.9f;
	weight2 = 0.55f;
	weight3 = 0.18f;
	weight4 = 0.1f;

	// Create a normalized value to average the weights out a bit.
	normalization = (weight0 + 2.0f * (weight1 + weight2 + weight3 + weight4));
	
	// Normalize the weights.
	weight0 = weight0 / normalization;
	weight1 = weight1 / normalization;
	weight2 = weight2 / normalization;
	weight3 = weight3 / normalization;
	weight4 = weight4 / normalization;

	// Determine the floating point size of a texel for a screen with this specific width.
	float texelSize = 1.0f / g_fWinSizeX;

	// Create UV coordinates for the pixel and its four horizontal neighbors on either side.
	float2 texCoord1 = In.vTexUV + float2(texelSize * -4.0f, 0.0f);
	float2 texCoord2 = In.vTexUV + float2(texelSize * -3.0f, 0.0f);
	float2 texCoord3 = In.vTexUV + float2(texelSize * -2.0f, 0.0f);
	float2 texCoord4 = In.vTexUV + float2(texelSize * -1.0f, 0.0f);
	float2 texCoord5 = In.vTexUV + float2(texelSize *  0.0f, 0.0f);
	float2 texCoord6 = In.vTexUV + float2(texelSize *  1.0f, 0.0f);
	float2 texCoord7 = In.vTexUV + float2(texelSize *  2.0f, 0.0f);
	float2 texCoord8 = In.vTexUV + float2(texelSize *  3.0f, 0.0f);
	float2 texCoord9 = In.vTexUV + float2(texelSize *  4.0f, 0.0f);

	// Initialize the color to black.
	color = float4(0.0f, 0.0f, 0.0f, 0.0f);

	// Add the nine horizontal pixels to the color by the specific weight of each.
	color += g_GlowTexture.Sample(LinearSampler, texCoord1) * weight4;
	color += g_GlowTexture.Sample(LinearSampler, texCoord2) * weight3;
	color += g_GlowTexture.Sample(LinearSampler, texCoord3) * weight2;
	color += g_GlowTexture.Sample(LinearSampler, texCoord4) * weight1;
	color += g_GlowTexture.Sample(LinearSampler, texCoord5) * weight0;
	color += g_GlowTexture.Sample(LinearSampler, texCoord6) * weight1;
	color += g_GlowTexture.Sample(LinearSampler, texCoord7) * weight2;
	color += g_GlowTexture.Sample(LinearSampler, texCoord8) * weight3;
	color += g_GlowTexture.Sample(LinearSampler, texCoord9) * weight4;

	Out.vColor = color;

	// Set the alpha channel to one.
	color.a = 1.0f;

	return Out;
}

PS_OUT PS_VERTICAL_BLUR(PS_IN In)
{
	PS_OUT Out = (PS_OUT)0;

	float weight0, weight1, weight2, weight3, weight4;
	float normalization;
	float4 color;

	// Create the weights that each neighbor pixel will contribute to the blur.
	weight0 = 1.0f;
	weight1 = 0.9f;
	weight2 = 0.55f;
	weight3 = 0.18f;
	weight4 = 0.1f;

	// Create a normalized value to average the weights out a bit.
	normalization = (weight0 + 2.0f * (weight1 + weight2 + weight3 + weight4));
	
	// Normalize the weights.
	weight0 = weight0 / normalization;
	weight1 = weight1 / normalization;
	weight2 = weight2 / normalization;
	weight3 = weight3 / normalization;
	weight4 = weight4 / normalization;

	// Determine the floating point size of a texel for a screen with this specific width.
	float texelSize = 1.0f / g_fWinSizeX;

	// Create UV coordinates for the pixel and its four horizontal neighbors on either side.
	float2 texCoord1 = In.vTexUV + float2(0.0f, texelSize * -4.0f);
	float2 texCoord2 = In.vTexUV + float2(0.0f, texelSize * -3.0f);
	float2 texCoord3 = In.vTexUV + float2(0.0f, texelSize * -2.0f);
	float2 texCoord4 = In.vTexUV + float2(0.0f, texelSize * -1.0f);
	float2 texCoord5 = In.vTexUV + float2(0.0f, texelSize *  0.0f);
	float2 texCoord6 = In.vTexUV + float2(0.0f, texelSize *  1.0f);
	float2 texCoord7 = In.vTexUV + float2(0.0f, texelSize *  2.0f);
	float2 texCoord8 = In.vTexUV + float2(0.0f, texelSize *  3.0f);
	float2 texCoord9 = In.vTexUV + float2(0.0f, texelSize *  4.0f);

	// Initialize the color to black.
	color = float4(0.0f, 0.0f, 0.0f, 0.0f);

	// Add the nine horizontal pixels to the color by the specific weight of each.
	color += g_GlowTexture.Sample(LinearSampler, texCoord1) * weight4;
	color += g_GlowTexture.Sample(LinearSampler, texCoord2) * weight3;
	color += g_GlowTexture.Sample(LinearSampler, texCoord3) * weight2;
	color += g_GlowTexture.Sample(LinearSampler, texCoord4) * weight1;
	color += g_GlowTexture.Sample(LinearSampler, texCoord5) * weight0;
	color += g_GlowTexture.Sample(LinearSampler, texCoord6) * weight1;
	color += g_GlowTexture.Sample(LinearSampler, texCoord7) * weight2;
	color += g_GlowTexture.Sample(LinearSampler, texCoord8) * weight3;
	color += g_GlowTexture.Sample(LinearSampler, texCoord9) * weight4;

	Out.vColor = color;

	// Set the alpha channel to one.
	color.a = 1.0f;

	return Out;
}

RasterizerState RS_Default
{
	FillMode = solid;
	CullMode = back;
	FrontCounterClockwise = false;
};
BlendState BS_Default
{
	BlendEnable[0] = false;
};

BlendState BS_LightBlending
{
	BlendEnable[0] = true;
	BlendEnable[1] = true;

	SrcBlend = one;
	DestBlend = one;
	BlendOp = add;
};

DepthStencilState DSS_Default
{
	DepthEnable = true;
	DepthWriteMask = all;
	DepthFunc = less_equal;
};

DepthStencilState DSS_ZEnable_Disable_ZWrite_Disable
{
	DepthEnable = false;
	DepthWriteMask = zero;
};

technique11 DefaultTechnique
{
	// 0
	pass DebugTarget	
	{
		SetRasterizerState(RS_Default);
		SetBlendState(BS_Default, float4(0.f, 0.f, 0.f, 1.f), 0xffffffff);
		SetDepthStencilState(DSS_ZEnable_Disable_ZWrite_Disable, 0);

		VertexShader = compile vs_5_0 VS_MAIN();
		GeometryShader = NULL;
		PixelShader = compile ps_5_0 PS_MAIN();
	}

	// 1
	pass Light_Directional
	{
		SetRasterizerState(RS_Default);
		SetBlendState(BS_LightBlending, float4(0.f, 0.f, 0.f, 1.f), 0xffffffff);
		SetDepthStencilState(DSS_ZEnable_Disable_ZWrite_Disable, 0);

		VertexShader = compile vs_5_0 VS_MAIN();
		GeometryShader = NULL;
		PixelShader = compile ps_5_0 PS_MAIN_LIGHT_DIRECTIONAL();
	}

	// 2
	pass Light_Point
	{
		SetRasterizerState(RS_Default);
		SetBlendState(BS_LightBlending, float4(0.f, 0.f, 0.f, 1.f), 0xffffffff);
		SetDepthStencilState(DSS_ZEnable_Disable_ZWrite_Disable, 0);

		VertexShader = compile vs_5_0 VS_MAIN();
		GeometryShader = NULL;
		PixelShader = compile ps_5_0 PS_MAIN_LIGHT_POINT();
	}

	// 3
	pass Blend
	{
		SetRasterizerState(RS_Default);
		SetBlendState(BS_Default, float4(0.f, 0.f, 0.f, 1.f), 0xffffffff);
		SetDepthStencilState(DSS_ZEnable_Disable_ZWrite_Disable, 0);

		VertexShader = compile vs_5_0 VS_MAIN();
		GeometryShader = NULL;
		PixelShader = compile ps_5_0 PS_MAIN_BLEND();
	}

	// 4
	pass Horizontal_Blur
	{
		SetRasterizerState(RS_Default);
		SetBlendState(BS_Default, float4(0.f, 0.f, 0.f, 1.f), 0xffffffff);
		SetDepthStencilState(DSS_ZEnable_Disable_ZWrite_Disable, 0);

		VertexShader = compile vs_5_0 VS_MAIN();
		GeometryShader = NULL;
		PixelShader = compile ps_5_0 PS_HORIZONTAL_BLUR();
	}

	// 5
	pass Vertical_Blur
	{
		SetRasterizerState(RS_Default);
		SetBlendState(BS_Default, float4(0.f, 0.f, 0.f, 1.f), 0xffffffff);
		SetDepthStencilState(DSS_ZEnable_Disable_ZWrite_Disable, 0);

		VertexShader = compile vs_5_0 VS_MAIN();
		GeometryShader = NULL;
		PixelShader = compile ps_5_0 PS_VERTICAL_BLUR();
	}
}