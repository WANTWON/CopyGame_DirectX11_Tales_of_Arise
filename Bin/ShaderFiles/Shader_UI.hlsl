
#include "Client_Shader_Defines.hpp"

matrix			g_WorldMatrix, g_ViewMatrix, g_ProjMatrix;
texture2D		g_DiffuseTexture;
texture2D       g_GradationTexture;
float			g_fAlpha = 1.f;

float			g_fMinRange = 100.f;
float			g_fMaxRange = 400.f;
float a = 0.f;

float g_fCurrentHp;
float g_fMaxHp;

float4			g_PlayerProjPos;
float2			g_TexUV;



struct VS_IN
{
	float3		vPosition : POSITION;
	float2		vTexUV : TEXCOORD0;
};

struct VS_OUT
{
	float4		vPosition : SV_POSITION;
	float2		vTexUV : TEXCOORD0;
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

	return Out;
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

	if (Out.vColor.a <= 0.3f)
		discard;

	return Out;
}

PS_OUT PS_ALPHA(PS_IN In)
{
	PS_OUT		Out = (PS_OUT)0;

	Out.vColor = g_DiffuseTexture.Sample(LinearSampler, In.vTexUV);
	Out.vColor.a *= g_fAlpha;

	return Out;
}

PS_OUT PS_PICKED(PS_IN In)
{
	PS_OUT		Out = (PS_OUT)0;

	Out.vColor = g_DiffuseTexture.Sample(LinearSampler, In.vTexUV);
	Out.vColor.rgb += 0.1f;
	Out.vColor.rg += 0.1f;
	Out.vColor.a *= g_fAlpha;
	return Out;
}


PS_OUT PS_SCREEN(PS_IN In)
{
	PS_OUT		Out = (PS_OUT)0;

	Out.vColor.rgb = 0.f;
	Out.vColor.a = g_fAlpha;

	return Out;
}

PS_OUT PS_FADEOUT(PS_IN In)
{
	PS_OUT		Out = (PS_OUT)0;
	Out.vColor = g_DiffuseTexture.Sample(LinearSampler, In.vTexUV);

	Out.vColor.a -= g_fAlpha;

	return Out;
}

PS_OUT PS_HPbar(PS_IN In)
{
	PS_OUT      Out = (PS_OUT)0;

	if (g_fCurrentHp / g_fMaxHp < In.vTexUV.x)
		discard;
	
	float4 origincolor = g_DiffuseTexture.Sample(LinearSampler, In.vTexUV);

	if (origincolor.a > 0.4f)
	{
		float4 maskcolor = g_GradationTexture.Sample(LinearSampler, In.vTexUV);

		float4 lerpcolor = lerp(float4(0.701f, 0.784f, 0.545f, 1.f), float4(0.7882f, 0.8352f, 0.647f, 1.f), maskcolor);

		Out.vColor = lerpcolor;
	}
	else
    Out.vColor = g_DiffuseTexture.Sample(PointSampler, In.vTexUV);
	
	

	if (Out.vColor.a<0.3f)
		discard;

	return Out;
}

PS_OUT PS_COMBOLINE(PS_IN In)
{
	PS_OUT		Out = (PS_OUT)0;

	Out.vColor = g_DiffuseTexture.Sample(LinearSampler, In.vTexUV);
	/*if (Out.vColor.a <= 0.3f)
		discard;

	if (Out.vColor.r == 0.f&&Out.vColor.g == 0.f&& Out.vColor.b ==0.f )
		discard;

	Out.vColor.r += 0.929f;
	Out.vColor.g += 0.8f;
	Out.vColor.b += 0.486f;*/
	float3 yellowRef = normalize(float3(.69f, .62f, .42f)); //텍스쳐색


	float weight = dot(Out.vColor, yellowRef); //알파만들기
	Out.vColor.a = lerp(0, 1, weight);

	Out.vColor.a = lerp(0, 1, saturate(weight));


	Out.vColor.a -= g_fAlpha;

	return Out;
}

PS_OUT PS_Golden(PS_IN In)
{
	PS_OUT      Out = (PS_OUT)0;

	/*if ((g_fCurrentHp / g_fMaxHp) < In.vTexUV.x)
		discard;*/

	float4 origincolor = g_DiffuseTexture.Sample(LinearSampler, In.vTexUV);

	if (origincolor.r >= 0.7f && origincolor.g >= 0.7f && origincolor.b >= 0.7f)//if (origincolor.a > 0.2f)// //
	{
		float4 maskcolor = g_GradationTexture.Sample(LinearSampler, In.vTexUV);

		float4 lerpcolor = lerp(float4(0.9882f, 0.9843f, 0.9176f, 1.f), float4(0.9764f, 0.9921f, 0.8941f, 1.f), maskcolor);

		Out.vColor = lerpcolor;
	}
	else
		Out.vColor = g_DiffuseTexture.Sample(LinearSampler, In.vTexUV);



	if (Out.vColor.a < 0.2f)
		discard;

	Out.vColor.a -= g_fAlpha;

	return Out;
}

PS_OUT PS_CPguage(PS_IN In)
{
	PS_OUT      Out = (PS_OUT)0;

	if (1 - g_fCurrentHp / g_fMaxHp > In.vTexUV.y)
		discard;

	float4 origincolor = g_DiffuseTexture.Sample(LinearSampler, In.vTexUV);

	if (origincolor.a > 0.4f)
	{
		float4 maskcolor = g_GradationTexture.Sample(LinearSampler, In.vTexUV);

		float4 lerpcolor = lerp(float4(0.701f, 0.784f, 0.545f, 1.f), float4(0.7882f, 0.8352f, 0.647f, 1.f), maskcolor);

		Out.vColor = lerpcolor;
	}
	else
		Out.vColor = g_DiffuseTexture.Sample(LinearSampler, In.vTexUV);



	/*if (Out.vColor.a<0.1f)
		discard;*/

	return Out;
}


PS_OUT PS_CPguageblack(PS_IN In)
{
	PS_OUT      Out = (PS_OUT)0;

	//if (1 - g_fCurrentHp / g_fMaxHp > In.vTexUV.y)// && Out.vColor.r == 0 && Out.vColor.g == 0  && Out.vColor.b ==0)
	//	discard;

	/*float4 origincolor = g_DiffuseTexture.Sample(LinearSampler, In.vTexUV);

	if (origincolor.a > 0.4f)
	{
		float4 maskcolor = g_GradationTexture.Sample(LinearSampler, In.vTexUV);

		float4 lerpcolor = lerp(float4(0.701f, 0.784f, 0.545f, 1.f), float4(0.7882f, 0.8352f, 0.647f, 1.f), maskcolor);

		Out.vColor = lerpcolor;
	}
	else
		*/



	/*if (Out.vColor.a<0.1f)
	discard;*/
	Out.vColor = g_DiffuseTexture.Sample(LinearSampler, In.vTexUV);
	return Out;
}

PS_OUT PS_MPGUAGE(PS_IN In)
{
	PS_OUT		Out = (PS_OUT)0;

	if (g_fCurrentHp / 1.f < In.vTexUV.x)
		discard;


	Out.vColor = g_DiffuseTexture.Sample(LinearSampler, In.vTexUV);

	//Out.vColor.a -= g_fAlpha;
	Out.vColor.r = 0.0589019607843137f;
	Out.vColor.g = 0.942708f;
	Out.vColor.b = 0.83441f;
		
	return Out;
}

PS_OUT PS_BLACKCOLOR(PS_IN In)
{
	PS_OUT		Out = (PS_OUT)0;
	Out.vColor = g_DiffuseTexture.Sample(LinearSampler, In.vTexUV);

	//Out.vColor.a -= g_fAlpha;
	Out.vColor.r = 0;
	Out.vColor.g = 0;
	Out.vColor.b = 0;

	return Out;
}

PS_OUT PS_PORTRAITDARK(PS_IN In)
{
	PS_OUT		Out = (PS_OUT)0;

	Out.vColor = g_DiffuseTexture.Sample(LinearSampler, In.vTexUV);


	if (Out.vColor.a <= 0.3f)
		discard;

	Out.vColor.a -= g_fAlpha;
	Out.vColor.r -= 0.15f;
	Out.vColor.g -= 0.15f;
	Out.vColor.b -= 0.15f;

	

	return Out;
}

PS_OUT PS_POREADY(PS_IN In)
{
	PS_OUT		Out = (PS_OUT)0;

	/*if (g_fCurrentHp / g_fMaxHp < In.vTexUV.x)
		discard;*/


	Out.vColor = g_DiffuseTexture.Sample(LinearSampler, In.vTexUV);
	Out.vColor.rgb += a;

	Out.vColor.a -= g_fAlpha;
	Out.vColor.r = max(0.9372549019607843f, Out.vColor.r);
	Out.vColor.g = max(0.8745098039215686f, Out.vColor.g); 
	Out.vColor.b = max(0.7647058823529412f, Out.vColor.b); 


	/*if (Out.vColor.a == 0)
		discard;*/
	return Out;
}

PS_OUT PS_ALLBLUE(PS_IN In)
{
	PS_OUT		Out = (PS_OUT)0;

//	if (g_fCurrentHp / g_fMaxHp < In.vTexUV.x)
//		discard;


	Out.vColor = g_DiffuseTexture.Sample(LinearSampler, In.vTexUV);

	//Out.vColor.a -= g_fAlpha;
	Out.vColor.r = 0.0589019607843137f;
	Out.vColor.g = 0.942708f;
	Out.vColor.b = 0.83441f;

	return Out;
}


PS_OUT PS_RUNECOLOR(PS_IN In)
{
	PS_OUT		Out = (PS_OUT)0;

	//	if (g_fCurrentHp / g_fMaxHp < In.vTexUV.x)
	//		discard;


	Out.vColor = g_DiffuseTexture.Sample(LinearSampler, In.vTexUV);

	//Out.vColor.a -= g_fAlpha;
	Out.vColor.r = 0.9803921568627451f;
	Out.vColor.g = 0.9019607843137255f;
	Out.vColor.b = 0.7607843137254902f;


	if (Out.vColor.a < 0.2f)
		discard;

	Out.vColor.a -= g_fAlpha;

	return Out;
}


PS_OUT PS_REVERSELOCKON(PS_IN In)
{
	PS_OUT		Out = (PS_OUT)0;

	if (1-(g_fCurrentHp / 1.f )> In.vTexUV.x)
		discard;


	Out.vColor = g_DiffuseTexture.Sample(LinearSampler, In.vTexUV);

	//Out.vColor.a -= g_fAlpha;
	Out.vColor.r = 0.0589019607843137f;
	Out.vColor.g = 0.942708f;
	Out.vColor.b = 0.83441f;

	return Out;
}




technique11 DefaultTechnique
{
	pass Default
	{
		SetRasterizerState(RS_Default);
		SetBlendState(BS_Default, float4(0.f, 0.f, 0.f, 1.f), 0xffffffff);
		SetDepthStencilState(DSS_Priority, 0);

		VertexShader = compile vs_5_0 VS_MAIN();
		GeometryShader = NULL;
		PixelShader = compile ps_5_0 PS_MAIN();
	}

	pass Alpha_Blending
	{
		SetRasterizerState(RS_Default);
		SetBlendState(BS_AlphaBlending, float4(0.f, 0.f, 0.f, 1.f), 0xffffffff);
		SetDepthStencilState(DSS_Priority, 0);

		VertexShader = compile vs_5_0 VS_MAIN();
		GeometryShader = NULL;
		PixelShader = compile ps_5_0 PS_MAIN();
	}

	pass Picked
	{
		SetRasterizerState(RS_Default);
		SetBlendState(BS_AlphaBlending, float4(0.f, 0.f, 0.f, 1.f), 0xffffffff);
		SetDepthStencilState(DSS_Priority, 0);

		VertexShader = compile vs_5_0 VS_MAIN();
		GeometryShader = NULL;
		PixelShader = compile ps_5_0 PS_PICKED();
	}

	pass Alpha_Set
	{
		SetRasterizerState(RS_Default);
		SetBlendState(BS_AlphaBlending, float4(0.f, 0.f, 0.f, 1.f), 0xffffffff);
		SetDepthStencilState(DSS_Priority, 0);

		VertexShader = compile vs_5_0 VS_MAIN();
		GeometryShader = NULL;
		PixelShader = compile ps_5_0 PS_ALPHA();
	}

	pass Alpha_Screen
	{
		SetRasterizerState(RS_Default);
		SetBlendState(BS_AlphaBlending, float4(0.f, 0.f, 0.f, 1.f), 0xffffffff);
		SetDepthStencilState(DSS_Priority, 0);

		VertexShader = compile vs_5_0 VS_MAIN();
		GeometryShader = NULL;
		PixelShader = compile ps_5_0 PS_SCREEN();
	}

	pass FADEOUT
	{
		SetRasterizerState(RS_Default);
		SetBlendState(BS_AlphaBlending, float4(0.f, 0.f, 0.f, 1.f), 0xffffffff);
		SetDepthStencilState(DSS_Priority, 0);

		VertexShader = compile vs_5_0 VS_MAIN();
		GeometryShader = NULL;
		PixelShader = compile ps_5_0 PS_FADEOUT();
	}

	pass HPbar
	{
		SetRasterizerState(RS_Default);
		SetBlendState(BS_AlphaBlending, float4(0.f, 0.f, 0.f, 1.f), 0xffffffff);
		SetDepthStencilState(DSS_Priority, 0);

		VertexShader = compile vs_5_0 VS_MAIN();
		GeometryShader = NULL;
		PixelShader = compile ps_5_0 PS_HPbar();
	}

	pass COMBOLINE
	{
		SetRasterizerState(RS_Default);
		SetBlendState(BS_AlphaBlending, float4(0.f, 0.f, 0.f, 1.f), 0xffffffff);
		SetDepthStencilState(DSS_Priority, 0);

		VertexShader = compile vs_5_0 VS_MAIN();
		GeometryShader = NULL;
		PixelShader = compile ps_5_0 PS_COMBOLINE();
	}

	pass Golden
	{
		SetRasterizerState(RS_Default);
		SetBlendState(BS_AlphaBlending, float4(0.f, 0.f, 0.f, 1.f), 0xffffffff);
		SetDepthStencilState(DSS_Priority, 0);

		VertexShader = compile vs_5_0 VS_MAIN();
		GeometryShader = NULL;
		PixelShader = compile ps_5_0 PS_Golden();
	}

	pass CPguage
	{
		SetRasterizerState(RS_Default);
		SetBlendState(BS_AlphaBlending, float4(0.f, 0.f, 0.f, 1.f), 0xffffffff);
		SetDepthStencilState(DSS_Priority, 0);

		VertexShader = compile vs_5_0 VS_MAIN();
		GeometryShader = NULL;
		PixelShader = compile ps_5_0 PS_CPguage();
	}

	pass CPguageblack
	{
		SetRasterizerState(RS_Default);
		SetBlendState(BS_AlphaBlending, float4(0.f, 0.f, 0.f, 1.f), 0xffffffff);
		SetDepthStencilState(DSS_Priority, 0);

		VertexShader = compile vs_5_0 VS_MAIN();
		GeometryShader = NULL;
		PixelShader = compile ps_5_0 PS_CPguageblack();
	}

	pass MPGUAGE
	{
		SetRasterizerState(RS_Default);
		SetBlendState(BS_AlphaBlending, float4(0.f, 0.f, 0.f, 1.f), 0xffffffff);
		SetDepthStencilState(DSS_Priority, 0);

		VertexShader = compile vs_5_0 VS_MAIN();
		GeometryShader = NULL;
		PixelShader = compile ps_5_0 PS_MPGUAGE();
	}

	pass BLACKCOLOR
	{
		SetRasterizerState(RS_Default);
		SetBlendState(BS_AlphaBlending, float4(0.f, 0.f, 0.f, 1.f), 0xffffffff);
		SetDepthStencilState(DSS_Priority, 0);

		VertexShader = compile vs_5_0 VS_MAIN();
		GeometryShader = NULL;
		PixelShader = compile ps_5_0 PS_BLACKCOLOR();
	}

	pass PORTRAITDARK
	{
		SetRasterizerState(RS_Default);
		SetBlendState(BS_AlphaBlending, float4(0.f, 0.f, 0.f, 1.f), 0xffffffff);
		SetDepthStencilState(DSS_Priority, 0);

		VertexShader = compile vs_5_0 VS_MAIN();
		GeometryShader = NULL;
		PixelShader = compile ps_5_0 PS_PORTRAITDARK();
	}

	pass POREADY
	{
		SetRasterizerState(RS_Default);
		SetBlendState(BS_AlphaBlending, float4(0.f, 0.f, 0.f, 1.f), 0xffffffff);
		SetDepthStencilState(DSS_Priority, 0);

		VertexShader = compile vs_5_0 VS_MAIN();
		GeometryShader = NULL;
		PixelShader = compile ps_5_0 PS_POREADY();
	}

	pass PS_ALLBLUE
	{
		SetRasterizerState(RS_Default);
		SetBlendState(BS_AlphaBlending, float4(0.f, 0.f, 0.f, 1.f), 0xffffffff);
		SetDepthStencilState(DSS_Priority, 0);

		VertexShader = compile vs_5_0 VS_MAIN();
		GeometryShader = NULL;
		PixelShader = compile ps_5_0 PS_ALLBLUE();
	}

	pass RUNECOLOR
	{
		SetRasterizerState(RS_Default);
		SetBlendState(BS_AlphaBlending, float4(0.f, 0.f, 0.f, 1.f), 0xffffffff);
		SetDepthStencilState(DSS_Priority, 0);

		VertexShader = compile vs_5_0 VS_MAIN();
		GeometryShader = NULL;
		PixelShader = compile ps_5_0 PS_RUNECOLOR();
	}


	pass REVERSELOCKON
	{
		SetRasterizerState(RS_Default);
		SetBlendState(BS_AlphaBlending, float4(0.f, 0.f, 0.f, 1.f), 0xffffffff);
		SetDepthStencilState(DSS_Priority, 0);

		VertexShader = compile vs_5_0 VS_MAIN();
		GeometryShader = NULL;
		PixelShader = compile ps_5_0 PS_REVERSELOCKON();
	}

	
	
	
}


