matrix g_WorldMatrix, g_ViewMatrix, g_ProjMatrix;
matrix g_ViewMatrixInv, g_ProjMatrixInv;

float g_fWinSizeX = 1280.f;
float g_fWinSizeY = 720.f;

texture2D g_BackBufferTexture;
texture2D g_BlendTexture;
texture2D g_DepthTexture;

/* Distortion */
texture2D g_DistortionTexture;
texture2D g_DistortionNoiseTexture;
float g_fDistortionTimer;
float g_fDistortionSpeed = .5f;
float g_fDistortionStrength = 1.f;

/* Blur */
texture2D g_BlurTexture;
const float Weight[17] = { 0.0561, 0.1353, 0.278, 0.4868, 0.6534, 0.7261, 0.8253, 0.9231, 1, 0.9231, 0.8253, 0.7261, 0.6534, 0.4868, 0.278, 0.1353, 0.0561 };
const float WeightSum = 9.1682;
const int WeightCount = 8; 

/* Fog */
texture2D g_FogTexture;
float3 g_vFogColor = float3(0.2, 0.4, 0.9f);
float g_fFogStrength = 0.6f;
float g_fMinRange = 20.f;
float g_fMaxRange = 70.f;
float3 g_vPlayerPosition;

/* Screen Distortion */
texture2D g_ScreenDistortionTexture;
float g_fScreenDistortionTimer;

/* Depth Of Field */
texture2D g_BlurredBackBufferTexture;
float g_fMinBlurDepth = 100.f;
float g_fMaxBlurDepth = 400.f;

/* Zoom Blur */
float g_fFocusPower;
int g_iFocusDetail;

/* Saturation */
float g_fSaturation = 1.f;

/* Edge Detection */
texture2D g_EdgeDetectionTexture;
texture2D g_EdgeDetectionComputeTexture;

sampler LinearSampler = sampler_state
{
	filter = min_mag_mip_Linear;
	AddressU = wrap;
	AddressV = wrap;
};

sampler DepthSampler = sampler_state
{
	filter = min_mag_Linear_mip_point;
	AddressU = clamp;
	AddressV = clamp;
};

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

BlendState BS_AlphaBlending
{
	BlendEnable[0] = true;

	SrcBlend = src_alpha;
	DestBlend = inv_Src_Alpha;
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

PS_OUT PS_MAIN(PS_IN In) // 0
{
	PS_OUT Out = (PS_OUT)0;

	float4 vBackBufferCopy = g_BackBufferTexture.Sample(LinearSampler, In.vTexUV);
	float4 vBlendTexture = g_BlendTexture.Sample(LinearSampler, In.vTexUV);

	Out.vColor = vBackBufferCopy + vBlendTexture;

	return Out;
}

PS_OUT PS_HORIZONTAL_BLUR(PS_IN In) // 1
{
	PS_OUT Out = (PS_OUT)0;

	float2 vTexUVOffset = 0;
	float texelSizeX = 1 / g_fWinSizeX; /* Get the size of a Texel Horizontally. */

	for (int i = -WeightCount; i < WeightCount; ++i)
	{
		vTexUVOffset = In.vTexUV + float2(texelSizeX * i, 0); /* Get the UV coordinates for the Offsetted Pixel. */
		Out.vColor += Weight[WeightCount + i] * g_BlurTexture.Sample(LinearSampler, vTexUVOffset); /* Multiply the Pixel Color with his corresponding Weight and add it to the final Color. */
	}

	Out.vColor /= WeightSum; /* Average the final Color by the Weight Sum. */
	Out.vColor.a = 0.f;
	return Out;
}

PS_OUT PS_VERTICAL_BLUR(PS_IN In) // 2
{
	PS_OUT Out = (PS_OUT)0;

	float2 vTexUVOffset = 0;
	float texelSizeY = 1 / g_fWinSizeY; /* Get the size of a Texel Vertically. */

	for (int i = -WeightCount; i < WeightCount; ++i)
	{
		vTexUVOffset = In.vTexUV + float2(0, texelSizeY * i); /* Get the UV coordinates for the Offsetted Pixel. */
		Out.vColor += Weight[WeightCount + i] * g_BlurTexture.Sample(LinearSampler, vTexUVOffset); /* Multiply the Pixel Color with his corresponding Weight and add it to the final Color. */
	}

	Out.vColor /= WeightSum; /* Average the final Color by the Weight Sum. */
	Out.vColor.a = 0.f;
	return Out;
}

PS_OUT PS_FOG(PS_IN In) // 3
{
	PS_OUT Out = (PS_OUT)0;

	float4 vDepthTexture = g_DepthTexture.Sample(LinearSampler, In.vTexUV);
	float fViewZ = vDepthTexture.y * 1000.f;

	float fFogPower;

	if (fViewZ == 0.f) /* SkyBox is not computed inside the DepthTexture. */
		fFogPower = 1.f;
	else
	{
		vector vWorldPos = (vector)0.f;
		vWorldPos.x = In.vTexUV.x * 2.f - 1.f;
		vWorldPos.y = In.vTexUV.y * -2.f + 1.f;
		vWorldPos.z = vDepthTexture.r;
		vWorldPos.w = 1.0f;

		vWorldPos *= fViewZ;
		vWorldPos = mul(vWorldPos, g_ProjMatrixInv);
		vWorldPos = mul(vWorldPos, g_ViewMatrixInv);

		float3 vPixelWorldPosition = vWorldPos.xyz;
		float fDistance = length(g_vPlayerPosition - vPixelWorldPosition);
		fFogPower = saturate((fDistance - g_fMinRange) / (g_fMaxRange - g_fMinRange)); // 0 ~ 1
	}

	Out.vColor.rgb = g_vFogColor;
	Out.vColor.a = fFogPower * g_fFogStrength;
	
	return Out;
}

PS_OUT PS_FOG_BLEND(PS_IN In) // 4
{
	PS_OUT Out = (PS_OUT)0;

	float4 vFogTexture = g_FogTexture.Sample(LinearSampler, In.vTexUV);

	Out.vColor = vFogTexture;

	return Out;
}

PS_OUT PS_SCREEN_DISTORTION(PS_IN In) // 5
{
	PS_OUT Out = (PS_OUT)0;

	float4 vBackBufferCopy = g_BackBufferTexture.Sample(LinearSampler, In.vTexUV);
	float4 vScreenDistortionTexture = g_ScreenDistortionTexture.Sample(LinearSampler, In.vTexUV);

	Out.vColor = vBackBufferCopy;

	return Out;
}

PS_OUT PS_DISTORTION(PS_IN In) // 6
{
	PS_OUT Out = (PS_OUT)0;

	float4 vBackBufferCopy = g_BackBufferTexture.Sample(LinearSampler, In.vTexUV);
	float4 vDistortionTexture = g_DistortionTexture.Sample(LinearSampler, In.vTexUV);

	/* Distort */
	if (vDistortionTexture.r > 0)
	{
		float4 vNoise = g_DistortionNoiseTexture.Sample(LinearSampler, In.vTexUV + g_fDistortionTimer * g_fDistortionSpeed);

		float2 vNoisedUVs;
		vNoisedUVs = In.vTexUV + vNoise.r;

		In.vTexUV = lerp(In.vTexUV, vNoisedUVs, g_fDistortionStrength / 100);

		vBackBufferCopy = g_BackBufferTexture.Sample(LinearSampler, In.vTexUV);
	}
	/* No Distort */
	else
		vBackBufferCopy = g_BackBufferTexture.Sample(LinearSampler, In.vTexUV);

	Out.vColor = vBackBufferCopy;

	return Out;
}

PS_OUT PS_DEPTHOFFIELD(PS_IN In) // 7
{
	PS_OUT Out = (PS_OUT)0;

	float4 vBackBufferTexture = g_BackBufferTexture.Sample(LinearSampler, In.vTexUV);
	float4 vBlurredBackBufferTexture = g_BlurredBackBufferTexture.Sample(LinearSampler, In.vTexUV);
	
	float4 vDepthTexture = g_DepthTexture.Sample(LinearSampler, In.vTexUV);
	
	float fViewZ = (vDepthTexture.y * 1000);

	if (fViewZ < g_fMinBlurDepth) /* SkyBox is not computed inside the DepthTexture. */
		Out.vColor = vBackBufferTexture; /* Just render BackBuffer color. */
	else
	{
		/* Lerp between BackBufferTexture and BlurredBackBufferTexture based on Depth. */
		float fInterpFactor = min(((fViewZ - g_fMinBlurDepth) / (g_fMaxBlurDepth - g_fMinBlurDepth)), 1.f);
		float4 vLerpColor = lerp(vBackBufferTexture, vBlurredBackBufferTexture, fInterpFactor);

		Out.vColor = vLerpColor;
	}

	return Out;
}

PS_OUT PS_ZOOMBLUR(PS_IN In) // 8
{
	PS_OUT Out = (PS_OUT)0;

	float2 vFocusPosition = float2(g_fWinSizeX / 2, g_fWinSizeY / 2);
	vFocusPosition.x /= g_fWinSizeX;
	vFocusPosition.y /= g_fWinSizeY;

	float2 vFocus = In.vTexUV - vFocusPosition;

	for (int i = 0; i < g_iFocusDetail; i++) 
	{
		float fPower = 1.0 - g_fFocusPower * (1.0 / g_fWinSizeX) * float(i);
		Out.vColor.rgb += g_BlurTexture.Sample(LinearSampler, vFocus * fPower + vFocusPosition).rgb;
	}

	Out.vColor.rgb *= 1.0 / float(g_iFocusDetail);

	return Out;
}

PS_OUT PS_SATURATE(PS_IN In) // 9
{
	PS_OUT Out = (PS_OUT)0;

	float3 fLuminance = float3(0.3086, 0.6094, 0.0820);
	float fSaturationInverse = 1.0 - g_fSaturation;

	float3 fRed = float3(fLuminance.x * fSaturationInverse, fLuminance.x * fSaturationInverse, fLuminance.x * fSaturationInverse);
	fRed += float3(g_fSaturation, 0, 0);

	float3 fGreen = float3(fLuminance.y * fSaturationInverse, fLuminance.y * fSaturationInverse, fLuminance.y * fSaturationInverse);
	fGreen += float3(0, g_fSaturation, 0);

	float3 fBlue = float3(fLuminance.z * fSaturationInverse, fLuminance.z * fSaturationInverse, fLuminance.z * fSaturationInverse);
	fBlue += float3(0, 0, g_fSaturation);

	float4x4 mSaturationMatrix = float4x4(	fRed, 0,
											fGreen, 0,
											fBlue, 0,
											0, 0, 0, 1	);

	float4 vColor = g_BackBufferTexture.Sample(LinearSampler, In.vTexUV);
	vColor = mul(vColor, mSaturationMatrix);

	Out.vColor = vColor;

	return Out;
}

PS_OUT PS_EDGE_DETECTION(PS_IN In) // 10
{
	PS_OUT Out = (PS_OUT)0;

	float4 vCenter = g_EdgeDetectionTexture.Sample(LinearSampler, In.vTexUV);
	float4 vLeft = g_EdgeDetectionTexture.Sample(LinearSampler, In.vTexUV - float2(1, 0) / float2(g_fWinSizeX, g_fWinSizeY));
	float4 vRight = g_EdgeDetectionTexture.Sample(LinearSampler, In.vTexUV + float2(1, 0) / float2(g_fWinSizeX, g_fWinSizeY));
	float4 vUp = g_EdgeDetectionTexture.Sample(LinearSampler, In.vTexUV - float2(0, 1) / float2(g_fWinSizeX, g_fWinSizeY));
	float4 vDown = g_EdgeDetectionTexture.Sample(LinearSampler, In.vTexUV + float2(0, 1) / float2(g_fWinSizeX, g_fWinSizeY));

	if (vCenter.a == 1 && vLeft.a == 0 ||
		vCenter.a == 1 && vRight.a == 0 ||
		vCenter.a == 1 && vUp.a == 0 ||
		vCenter.a == 1 && vDown.a == 0)
		Out.vColor = float4(0.f, 0.f, 0.f, 1.f);
	else
		Out.vColor = float4(1.f, 1.f, 1.f, 1.f);

	return Out;
}

PS_OUT PS_EDGE_DETECTION_BLEND(PS_IN In) // 11
{
	PS_OUT Out = (PS_OUT)0;

	float4 vEdgeDetectionCompute = g_EdgeDetectionComputeTexture.Sample(LinearSampler, In.vTexUV);

	if (vEdgeDetectionCompute.r == 1 && vEdgeDetectionCompute.g == 1.f && vEdgeDetectionCompute.b == 1.f)
		discard;

	Out.vColor = vEdgeDetectionCompute;

	return Out;
}

technique11 DefaultTechnique
{
	pass Default // 0
	{
		SetRasterizerState(RS_Default);
		SetBlendState(BS_Default, float4(0.f, 0.f, 0.f, 1.f), 0xffffffff);
		SetDepthStencilState(DSS_ZEnable_Disable_ZWrite_Disable, 0);

		VertexShader = compile vs_5_0 VS_MAIN();
		GeometryShader = NULL;
		PixelShader = compile ps_5_0 PS_MAIN();
	}

	pass Horizontal_Blur // 1
	{
		SetRasterizerState(RS_Default);
		SetBlendState(BS_Default, float4(0.f, 0.f, 0.f, 1.f), 0xffffffff);
		SetDepthStencilState(DSS_ZEnable_Disable_ZWrite_Disable, 0);

		VertexShader = compile vs_5_0 VS_MAIN();
		GeometryShader = NULL;
		PixelShader = compile ps_5_0 PS_HORIZONTAL_BLUR();
	}

	pass Vertical_Blur // 2
	{
		SetRasterizerState(RS_Default);
		SetBlendState(BS_Default, float4(0.f, 0.f, 0.f, 1.f), 0xffffffff);
		SetDepthStencilState(DSS_ZEnable_Disable_ZWrite_Disable, 0);

		VertexShader = compile vs_5_0 VS_MAIN();
		GeometryShader = NULL;
		PixelShader = compile ps_5_0 PS_VERTICAL_BLUR();
	}
	
	pass Fog // 3 
	{
		SetRasterizerState(RS_Default);
		SetBlendState(BS_AlphaBlending, float4(0.f, 0.f, 0.f, 1.f), 0xffffffff);
		SetDepthStencilState(DSS_ZEnable_Disable_ZWrite_Disable, 0);

		VertexShader = compile vs_5_0 VS_MAIN();
		GeometryShader = NULL;
		PixelShader = compile ps_5_0 PS_FOG();
	}

	pass Blend_Fog // 4
	{
		SetRasterizerState(RS_Default);
		SetBlendState(BS_AlphaBlending, float4(0.f, 0.f, 0.f, 1.f), 0xffffffff);
		SetDepthStencilState(DSS_ZEnable_Disable_ZWrite_Disable, 0);

		VertexShader = compile vs_5_0 VS_MAIN();
		GeometryShader = NULL;
		PixelShader = compile ps_5_0 PS_FOG_BLEND();
	}

	pass Screen_Distortion // 5
	{
		SetRasterizerState(RS_Default);
		SetBlendState(BS_Default, float4(0.f, 0.f, 0.f, 1.f), 0xffffffff);
		SetDepthStencilState(DSS_ZEnable_Disable_ZWrite_Disable, 0);

		VertexShader = compile vs_5_0 VS_MAIN();
		GeometryShader = NULL;
		PixelShader = compile ps_5_0 PS_SCREEN_DISTORTION();
	}

	pass Distortion // 6
	{
		SetRasterizerState(RS_Default);
		SetBlendState(BS_Default, float4(0.f, 0.f, 0.f, 1.f), 0xffffffff);
		SetDepthStencilState(DSS_ZEnable_Disable_ZWrite_Disable, 0);

		VertexShader = compile vs_5_0 VS_MAIN();
		GeometryShader = NULL;
		PixelShader = compile ps_5_0 PS_DISTORTION();
	}

	pass DepthOfField // 7
	{
		SetRasterizerState(RS_Default);
		SetBlendState(BS_Default, float4(0.f, 0.f, 0.f, 1.f), 0xffffffff);
		SetDepthStencilState(DSS_ZEnable_Disable_ZWrite_Disable, 0);

		VertexShader = compile vs_5_0 VS_MAIN();
		GeometryShader = NULL;
		PixelShader = compile ps_5_0 PS_DEPTHOFFIELD();
	}

	pass Zoom_Blur // 8
	{
		SetRasterizerState(RS_Default);
		SetBlendState(BS_Default, float4(0.f, 0.f, 0.f, 1.f), 0xffffffff);
		SetDepthStencilState(DSS_ZEnable_Disable_ZWrite_Disable, 0);

		VertexShader = compile vs_5_0 VS_MAIN();
		GeometryShader = NULL;
		PixelShader = compile ps_5_0 PS_ZOOMBLUR();
	}

	pass Saturation // 9
	{
		SetRasterizerState(RS_Default);
		SetBlendState(BS_Default, float4(0.f, 0.f, 0.f, 1.f), 0xffffffff);
		SetDepthStencilState(DSS_ZEnable_Disable_ZWrite_Disable, 0);

		VertexShader = compile vs_5_0 VS_MAIN();
		GeometryShader = NULL;
		PixelShader = compile ps_5_0 PS_SATURATE();
	}

	pass EdgeDetection // 10
	{
		SetRasterizerState(RS_Default);
		SetBlendState(BS_Default, float4(0.f, 0.f, 0.f, 1.f), 0xffffffff);
		SetDepthStencilState(DSS_ZEnable_Disable_ZWrite_Disable, 0);

		VertexShader = compile vs_5_0 VS_MAIN();
		GeometryShader = NULL;
		PixelShader = compile ps_5_0 PS_EDGE_DETECTION();
	}

	pass EdgeDetectionBlend // 11
	{
		SetRasterizerState(RS_Default);
		SetBlendState(BS_Default, float4(0.f, 0.f, 0.f, 1.f), 0xffffffff);
		SetDepthStencilState(DSS_ZEnable_Disable_ZWrite_Disable, 0);

		VertexShader = compile vs_5_0 VS_MAIN();
		GeometryShader = NULL;
		PixelShader = compile ps_5_0 PS_EDGE_DETECTION_BLEND();
	}
}