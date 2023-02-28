
#include "Client_Shader_Defines.hpp"

matrix			g_WorldMatrix, g_ViewMatrix, g_ProjMatrix;
texture2D		g_DiffuseTexture;
texture2D       g_GradationTexture;
float			g_fAlpha = 1.f;
float           g_fAlpha1 = 1.f;

float           g_fBright = 0.f;

float			g_fMinRange = 100.f;
float			g_fMaxRange = 400.f;
float a = 0.f;
float g_fAngle; 
float g_fAngle1;

float g_fCurrentHp;
float g_fMaxHp;

float4			g_PlayerProjPos;
float2			g_TexUV;

float g_UV_sizeX;   //가로 ;
float g_UV_sizeY;   //세로;
float2 g_MiddlePoint;

float2 g_UVSpriteValue;
float g_SpriteCurTime;
float g_SpriteSpeed;
bool g_SpriteLoop = true;

float2 g_WinXY;

/* Sprite */
float2 g_vSprite;			/* Number of Sprites (Horizontally and Vertically). */
float g_fSpriteTimer;		/* Time elapsed since start of Sprite Animation. */
float g_fSpriteDuration;	/* Duration of entire Sprite Animation in seconds. */

/* Glow */
texture2D g_GlowTexture;
float4 g_vGlowColor;
bool g_bUseDiffuseColor;
float g_fGlowTimer;

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

	if (In.vTexUV.y > (1.4f - In.vTexUV.x) + (In.vTexUV.y))
		discard;
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
    Out.vColor = g_DiffuseTexture.Sample(LinearSampler, In.vTexUV);
	

	float fGradientRadius = 0.5f;
	float fGradientStrength = 0.2f;
	float  fLerp = 0.f;

	/* First Lerp */
	if (In.vTexUV.x > g_fBright - fGradientRadius && In.vTexUV.x < g_fBright)
	{
		float fStart = g_fBright - fGradientRadius;
		float fEnd = g_fBright;
		float fInterpFactor = (In.vTexUV.x - fStart) / (fEnd - fStart);
		fLerp = lerp(0, fGradientStrength, fInterpFactor);
	}
	/* Second Lerp */
	else if (In.vTexUV.x > g_fBright && In.vTexUV.x < g_fBright + fGradientRadius)
	{
		float fStart = g_fBright;
		float fEnd = g_fBright + fGradientRadius;
		float fInterpFactor = (In.vTexUV.x - fStart) / (fEnd - fStart);
		fLerp = lerp(fGradientStrength, 0, fInterpFactor);
	}

	Out.vColor.rgb += fLerp;   //fLerpValue;


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


	float weight = dot(Out.vColor.rgb, yellowRef); //알파만들기
	Out.vColor.a = lerp(0, 1, weight);

	Out.vColor.a = lerp(0, 1, saturate(weight));


	Out.vColor.a -= g_fAlpha;

	//Out.vColor.a = Out.vColor.rgb

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
	if (Out.vColor.a <= 0.1)
		discard;
		
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
	if (Out.vColor.a < 0.2f)
		discard;
	Out.vColor.a *= g_fAlpha;

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

	if (Out.vColor.a <= 0.1)
	discard;

	return Out;
}

PS_OUT PS_UVROTATION(PS_IN In)
{
	PS_OUT      Out = (PS_OUT)0;

	/*Out.vColor = g_DiffuseTexture.Sample(LinearSampler, In.vTexUV);
	if (Out.vColor.r == 0 && Out.vColor.g == 0 && Out.vColor.b && 0)
		return Out;*/
	

	float2x2 rotationMatrix = float2x2(cos(g_fAngle), -sin(g_fAngle), sin(g_fAngle), cos(g_fAngle));

	if (In.vTexUV.x < 0.15f || In.vTexUV.x > 0.85f)
		discard;

	if (In.vTexUV.y < 0.2f || In.vTexUV.y > 0.8f)
		discard;

	float2 rotatedUV = mul(rotationMatrix, In.vTexUV -0.5f) + 0.5f;

	Out.vColor = g_DiffuseTexture.Sample(LinearSampler, rotatedUV);

	Out.vColor.a = Out.vColor.r;

	//if (Out.vColor.r >= 0.2f)
		Out.vColor.rgb = float3(0.5882352941176471f, 0.5372549019607843f, 0.3686274509803922f);

		


	Out.vColor.a -= 0.2f;

	return Out;
}

PS_OUT PS_INVENICON(PS_IN In)
{
	PS_OUT		Out = (PS_OUT)0;

	


	Out.vColor = g_DiffuseTexture.Sample(LinearSampler, In.vTexUV);

	
	if (Out.vColor.r < 0.53f || Out.vColor.g < 0.5f || Out.vColor.b < 0.5f)
		discard;
	else
	{

		Out.vColor.r = 0.9372549019607843f;
		Out.vColor.g = 0.8745098039215686f;
		Out.vColor.b = 0.7647058823529412f;
	}

	float fGradientRadius = 0.6f;
	float fGradientStrength = 0.25f;
	float fLerp = 0.f;

	/* First Lerp */
	if (In.vTexUV.x > g_fBright - fGradientRadius && In.vTexUV.x < g_fBright)
	{
		float fStart = g_fBright - fGradientRadius;
		float fEnd = g_fBright;
		float fInterpFactor = (In.vTexUV.x - fStart) / (fEnd - fStart);
		fLerp = lerp(0, fGradientStrength, fInterpFactor);
	}
	/* Second Lerp */
	else if (In.vTexUV.x > g_fBright && In.vTexUV.x < g_fBright + fGradientRadius)
	{
		float fStart = g_fBright;
		float fEnd = g_fBright + fGradientRadius;
		float fInterpFactor = (In.vTexUV.x - fStart) / (fEnd - fStart);
		fLerp = lerp(fGradientStrength, 0, fInterpFactor);
	}



	Out.vColor.rgb += fLerp;   //fLerpValue;
	
		
		
		
	Out.vColor.a -= 0.3f;
	

	return Out;
}

PS_OUT PS_INVENBACK(PS_IN In)
{
	PS_OUT		Out = (PS_OUT)0;




	Out.vColor = g_DiffuseTexture.Sample(LinearSampler, In.vTexUV);

	Out.vColor.a = Out.vColor.r;
	Out.vColor.rgb = float3(0.5882352941176471f, 0.5372549019607843f, 0.3686274509803922f);
	//if (Out.vColor.r < 0.53f || Out.vColor.g < 0.5f || Out.vColor.b < 0.5f)
	//	discard;
	//else
	//{

	//	Out.vColor.r = 0.9372549019607843f;
	//	Out.vColor.g = 0.8745098039215686f;
	//	Out.vColor.b = 0.7647058823529412f;
	//	//	Out.vColor.r = max(0.9372549019607843f, Out.vColor.r);
	//	//	Out.vColor.g = max(0.8745098039215686f, Out.vColor.g);
	//	//	Out.vColor.b = max(0.7647058823529412f, Out.vColor.b);
	//}




	Out.vColor.a -= 0.1f;


	return Out;
}

PS_OUT PS_MENULINE(PS_IN In)
{
	PS_OUT      Out = (PS_OUT)0;


	float4 origincolor = g_DiffuseTexture.Sample(LinearSampler, In.vTexUV);
	float4 maskcolor = g_GradationTexture.Sample(LinearSampler, In.vTexUV);

	
	
	//origincolor.a = maskcolor.a;
	//Out.vColor = origincolor;

	float4 lerpcolor = lerp(float4(0.701f, 0.784f, 0.545f, 1.f), float4(0.7882f, 0.8352f, 0.647f, 1.f), maskcolor);

	Out.vColor = lerpcolor;
	Out.vColor.a = maskcolor.a;
	


		if (Out.vColor.a == 0.f)
			discard;
	/*if (Out.vColor.a<0.3f)
		discard;*/

	return Out;
}


PS_OUT PS_INVENTOPBOTTOM(PS_IN In)
{
	PS_OUT		Out = (PS_OUT)0;




	Out.vColor = g_DiffuseTexture.Sample(LinearSampler, In.vTexUV);


	Out.vColor.r = 0.f;
	Out.vColor.g = 0.f;
	Out.vColor.b = 0.f;
	Out.vColor.a = 1.f;

	Out.vColor.a -= 0.4f;


	return Out;
}

PS_OUT PS_INVENTOPBOTTOMALPHA(PS_IN In)
{
	PS_OUT		Out = (PS_OUT)0;




	Out.vColor = g_DiffuseTexture.Sample(LinearSampler, In.vTexUV);


	Out.vColor.r = 0.f;
	Out.vColor.g = 0.f;
	Out.vColor.b = 0.f;
	Out.vColor.a = 0.6f;

	Out.vColor.a -= g_fAlpha1;


	return Out;
}

PS_OUT PS_GALDBACK(PS_IN In)
{
	PS_OUT		Out = (PS_OUT)0;

	Out.vColor = g_DiffuseTexture.Sample(LinearSampler, In.vTexUV);

	Out.vColor.a -= Out.vColor.r + 0.2f;

	Out.vColor.rgb -= 0.2f;
	/*if (Out.vColor.a <= 1.f)
		discard;*/

	Out.vColor.a *= g_fAlpha;

	return Out;
}

PS_OUT PS_UVROTATIONSTRONG(PS_IN In)
{
	PS_OUT      Out = (PS_OUT)0;

	/*Out.vColor = g_DiffuseTexture.Sample(LinearSampler, In.vTexUV);
	if (Out.vColor.r == 0 && Out.vColor.g == 0 && Out.vColor.b && 0)
	return Out;*/


	float2x2 rotationMatrix = float2x2(cos(g_fAngle), -sin(g_fAngle1), sin(g_fAngle1), cos(g_fAngle1));

	if (In.vTexUV.x < 0.15f || In.vTexUV.x > 0.85f)
		discard;

	if (In.vTexUV.y < 0.2f || In.vTexUV.y > 0.8f)
		discard;

	float2 rotatedUV = mul(rotationMatrix, In.vTexUV - 0.5f) + 0.5f;

	Out.vColor = g_DiffuseTexture.Sample(LinearSampler, rotatedUV);

	Out.vColor.a = Out.vColor.r;

	//if (Out.vColor.r >= 0.2f)
	Out.vColor.rgb = float3(0.6f, 0.5f, 0.3f);//(0.5882352941176471f, 0.5372549019607843f, 0.3686274509803922f);




	Out.vColor.a += 0.15f;

	return Out;


}

PS_OUT PS_USINGITEMPORTRAIT(PS_IN In)
{
	PS_OUT      Out = (PS_OUT)0;



	Out.vColor = g_DiffuseTexture.Sample(LinearSampler, In.vTexUV);

	////Out.vColor.a = Out.vColor.r;

	////if (Out.vColor.r >= 0.2f)
	//Out.vColor.rgb = (0.5882352941176471f, 0.5372549019607843f, 0.3686274509803922f);

	float4 lerpcolor = lerp(float4(0.5882352941176471f, 0.5372549019607843f, 0.3686274509803922f, 1.f), float4(0.9f, 0.8352f, 0.9f, 1.f), Out.vColor);


	
	Out.vColor = lerpcolor;
	Out.vColor.a -= 0.5f;
	return Out;


}

PS_OUT PS_LIGHTEFFECT(PS_IN In)
{
	PS_OUT      Out = (PS_OUT)0;



	Out.vColor = g_DiffuseTexture.Sample(LinearSampler, In.vTexUV);
	Out.vColor.a = Out.vColor.g;

	Out.vColor.rgb = float3(0.5882352941176471f, 0.5372549019607843f, 0.5f);
	//float4 lerpcolor = lerp(float4(0.5882352941176471f, 0.5372549019607843f, 0.3686274509803922f, 1.f), float4(0.9f, 0.8352f, 0.9f, 1.f), Out.vColor);

	Out.vColor.a -= 0.25f;
	
	//Out.vColor.rgb = lerpcolor.rgb;
	
	return Out;


}

PS_OUT PS_Bright(PS_IN In)
{
	PS_OUT Out = (PS_OUT)0;

	Out.vColor = g_DiffuseTexture.Sample(LinearSampler, In.vTexUV);


	float fGradientRadius = 0.5f;
	float fGradientStrength = 0.3f;
	float fLerp = 0.f;

	/* First Lerp */
	if (In.vTexUV.x > g_fBright - fGradientRadius && In.vTexUV.x < g_fBright)
	{
		float fStart = g_fBright - fGradientRadius;
		float fEnd = g_fBright;
		float fInterpFactor = (In.vTexUV.x - fStart) / (fEnd - fStart);
		fLerp = lerp(0, fGradientStrength, fInterpFactor);
	}
	/* Second Lerp */
	else if (In.vTexUV.x > g_fBright && In.vTexUV.x < g_fBright + fGradientRadius)
	{
		float fStart = g_fBright;
		float fEnd = g_fBright + fGradientRadius;
		float fInterpFactor = (In.vTexUV.x - fStart) / (fEnd - fStart);
		fLerp = lerp(fGradientStrength, 0, fInterpFactor);
	}

	Out.vColor.rgb += fLerp;   //fLerpValue;


	/*if (Out.vColor.a<0.3f)
		discard;*/

	Out.vColor.a *= g_fAlpha;

	return Out;

	
}

PS_OUT PS_HPbarBLUE(PS_IN In)
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
		Out.vColor = g_DiffuseTexture.Sample(LinearSampler, In.vTexUV);


	float fGradientRadius = 0.5f;
	float fGradientStrength = 0.2f;
	float fLerp = 0.f;

	/* First Lerp */
	if (In.vTexUV.x > g_fBright - fGradientRadius && In.vTexUV.x < g_fBright)
	{
		float fStart = g_fBright - fGradientRadius;
		float fEnd = g_fBright;
		float fInterpFactor = (In.vTexUV.x - fStart) / (fEnd - fStart);
		fLerp = lerp(0, fGradientStrength, fInterpFactor);
	}
	/* Second Lerp */
	else if (In.vTexUV.x > g_fBright && In.vTexUV.x < g_fBright + fGradientRadius)
	{
		float fStart = g_fBright;
		float fEnd = g_fBright + fGradientRadius;
		float fInterpFactor = (In.vTexUV.x - fStart) / (fEnd - fStart);
		fLerp = lerp(fGradientStrength, 0, fInterpFactor);
	}
	Out.vColor.b = 1.f;
	Out.vColor.r -= 0.13f;
	Out.vColor.g += 0.05f;
	//Out.vColor.g += 0.2f;
	Out.vColor.rgb += fLerp;   //fLerpValue;


	if (Out.vColor.a<0.3f)
		discard;

	return Out;
}

PS_OUT PS_Dialoguebox(PS_IN In)
{
	PS_OUT      Out = (PS_OUT)0;

	

	Out.vColor = g_DiffuseTexture.Sample(LinearSampler, In.vTexUV);

	Out.vColor.a = Out.vColor.g;

	if (Out.vColor.r == 0)
		discard;
	

	Out.vColor.rgb = 0.f;
	Out.vColor.a *= g_fAlpha;


	return Out;


}
PS_OUT PS_DialogueLINE(PS_IN In)
{
	PS_OUT      Out = (PS_OUT)0;


    Out.vColor = g_DiffuseTexture.Sample(LinearSampler, In.vTexUV);
	Out.vColor.a = Out.vColor.r;

	if (Out.vColor.r == 0)
		discard;

    float4 lerpcolor = lerp(float4(0.9019607843137255f, 0.8235294117647059f, 0.6588235294117647f, 1.f), float4(0.7882f, 0.8352f, 0.647f, 1.f), Out.vColor);

	Out.vColor.rgb = lerpcolor.rgb;
	Out.vColor.a *= g_fAlpha;

	return Out;
}

PS_OUT PS_DIALOGUECURSOR(PS_IN In)
{
	PS_OUT      Out = (PS_OUT)0;

	if (1 - g_fCurrentHp / g_fMaxHp > In.vTexUV.y)
		discard;

	float4 origincolor = g_DiffuseTexture.Sample(LinearSampler, In.vTexUV);
	if (origincolor.r == 0)
		discard;

		float4 maskcolor = g_GradationTexture.Sample(LinearSampler, In.vTexUV);

		float4 lerpcolor = lerp(float4(0.701f, 0.784f, 0.545f, 1.f), float4(0.7882f, 0.8352f, 0.647f, 1.f), maskcolor);

		Out.vColor = lerpcolor;

		Out.vColor.a *= g_fAlpha;

	return Out;
}
PS_OUT PS_DIALOGUECURSORNOTMOVE(PS_IN In)
{
	PS_OUT      Out = (PS_OUT)0;


	float4 origincolor = g_DiffuseTexture.Sample(LinearSampler, In.vTexUV);
	if (origincolor.r == 0)
		discard;

	float4 maskcolor = g_GradationTexture.Sample(LinearSampler, In.vTexUV);

	float4 lerpcolor = lerp(float4(0.701f, 0.784f, 0.545f, 1.f), float4(0.7882f, 0.8352f, 0.647f, 1.f), maskcolor);

	Out.vColor = lerpcolor;

	Out.vColor.a *= g_fAlpha;

	return Out;
}


PS_OUT PS_ALPHATESTSET(PS_IN In)
{
//	PS_OUT		Out = (PS_OUT)0;
//
//	Out.vColor = g_DiffuseTexture.Sample(LinearSampler, In.vTexUV);
//	if(Out.vColor.r <= 0.f)
//		discard;
////	float2 vNewUv = In.vTexUV * 0.5f;
//	float4 gradtexture = g_GradationTexture.Sample(LinearSampler, vNewUv);
//	
//	Out.vColor.a = gradtexture.a;
//
//	
//
//	/*if (Out.vColor.r <= 1.5f)
//		discard;*/
//	Out.vColor.a *= g_fAlpha;
//
//	return Out;

	PS_OUT		Out = (PS_OUT)0;

	Out.vColor = g_DiffuseTexture.Sample(LinearSampler, In.vTexUV);
	/*if (Out.vColor.a <= 0.3f)
	discard;

	if (Out.vColor.r == 0.f&&Out.vColor.g == 0.f&& Out.vColor.b ==0.f )
	discard;

	Out.vColor.r += 0.929f;
	Out.vColor.g += 0.8f;
	Out.vColor.b += 0.486f;*/
//	float3 yellowRef = normalize(float3(.69f, .62f, .42f)); //텍스쳐색


//	float weight = dot(Out.vColor.rgb, yellowRef); //알파만들기
//	Out.vColor.a = lerp(0, 1, weight);

	Out.vColor.a = Out.vColor.rgb;//lerp(0, 1, saturate(weight));


	Out.vColor.a *= g_fAlpha;

	return Out;
}

PS_OUT PS_BRIGHTFORBLACK(PS_IN In)
{
	

	PS_OUT		Out = (PS_OUT)0;

	Out.vColor = g_DiffuseTexture.Sample(LinearSampler, In.vTexUV);
	

	Out.vColor.a = Out.vColor.rgb;


	float fGradientRadius = 0.5f;
	float fGradientStrength = 0.5f;
	float fLerp = 0.f;

	/* First Lerp */
	if (In.vTexUV.x > g_fBright - fGradientRadius && In.vTexUV.x < g_fBright)
	{
		float fStart = g_fBright - fGradientRadius;
		float fEnd = g_fBright;
		float fInterpFactor = (In.vTexUV.x - fStart) / (fEnd - fStart);
		fLerp = lerp(0, fGradientStrength, fInterpFactor);
	}
	/* Second Lerp */
	else if (In.vTexUV.x > g_fBright && In.vTexUV.x < g_fBright + fGradientRadius)
	{
		float fStart = g_fBright;
		float fEnd = g_fBright + fGradientRadius;
		float fInterpFactor = (In.vTexUV.x - fStart) / (fEnd - fStart);
		fLerp = lerp(fGradientStrength, 0, fInterpFactor);
	}

	Out.vColor.rgb += fLerp;   //fLerpValue;


	Out.vColor.a *= g_fAlpha;

	return Out;
}

PS_OUT PS_BRIGHTDialogueLINE(PS_IN In)
{
	PS_OUT      Out = (PS_OUT)0;


	Out.vColor = g_DiffuseTexture.Sample(LinearSampler, In.vTexUV);
	Out.vColor.a = Out.vColor.r;

	if (Out.vColor.r == 0)
		discard;

	float4 lerpcolor = lerp(float4(0.9019607843137255f, 0.8235294117647059f, 0.6588235294117647f, 1.f), float4(0.7882f, 0.8352f, 0.647f, 1.f), Out.vColor);

	Out.vColor.rgb = lerpcolor.rgb;

	float fGradientRadius = 0.5f;
	float fGradientStrength = 0.3f;
	float fLerp = 0.f;

	/* First Lerp */
	if (In.vTexUV.x > g_fBright - fGradientRadius && In.vTexUV.x < g_fBright)
	{
		float fStart = g_fBright - fGradientRadius;
		float fEnd = g_fBright;
		float fInterpFactor = (In.vTexUV.x - fStart) / (fEnd - fStart);
		fLerp = lerp(0, fGradientStrength, fInterpFactor);
	}
	/* Second Lerp */
	else if (In.vTexUV.x > g_fBright && In.vTexUV.x < g_fBright + fGradientRadius)
	{
		float fStart = g_fBright;
		float fEnd = g_fBright + fGradientRadius;
		float fInterpFactor = (In.vTexUV.x - fStart) / (fEnd - fStart);
		fLerp = lerp(fGradientStrength, 0, fInterpFactor);
	}

	Out.vColor.rgb += fLerp;   //fLerpValue;


	Out.vColor.a *= g_fAlpha;
	

	return Out;
}

PS_OUT PS_OUTLINE(PS_IN In)
{
	PS_OUT      Out = (PS_OUT)0;

	//vector Color_rgb;
	//Out.vColor = g_DiffuseTexture.Sample(LinearSampler, In.vTexUV);

	//float fRight_Line = (g_MiddlePoint.x + (g_UV_sizeX*0.5f));
	//float fUp_Line = (g_MiddlePoint.y - (g_UV_sizeY*0.5f));
	//float fDown_Line = (g_MiddlePoint.y + (g_UV_sizeY*0.5f));
	//float fLeft_Line = (g_MiddlePoint.x - (g_UV_sizeX*0.5f));

	////float fRight_Line = In.vTexUV.x + (In.vTexUV.x *0.5f);
	////float fUp_Line =   In.vTexUV.y + (In.vTexUV.y *0.5f);
	////float fDown_Line = In.vTexUV.y - (In.vTexUV.y *0.5f);
	////float fLeft_Line = In.vTexUV.x - (In.vTexUV.x *0.5f);


	//if (fLeft_Line < g_MiddlePoint.x && g_MiddlePoint.x < fLeft_Line + 12.f ||
	//	fRight_Line > g_MiddlePoint.x && g_MiddlePoint.x > fRight_Line - 12.f ||
	//	fDown_Line > g_MiddlePoint.y&& g_MiddlePoint.y > fDown_Line - 12.f ||
	//	fUp_Line < g_MiddlePoint.y && g_MiddlePoint.y < fUp_Line + 12.f)
	//{
	//	
	//	Out.vColor = float4(1.f, 1.f, 1.f, 1.f);
	//	Out.vColor.a *= 0.5f;
	//}
	//else
	//{
	//	Out.vColor = g_DiffuseTexture.Sample(LinearSampler, In.vTexUV);
	//	//Out.vColor = float4(1.f, 1.f, 1.f, 1.f);
	//	Out.vColor.a *= 0.5f;
	//}
	Out.vColor = g_DiffuseTexture.Sample(LinearSampler, In.vTexUV);
	if (In.vTexUV.x < 0.015f || In.vTexUV.x > 0.985f)
		Out.vColor.rgb = 0.2f;// (0.2f, 0.2f, 0.2f,


	if (In.vTexUV.y < 0.02f || In.vTexUV.y > 0.98f)
		Out.vColor.rgb = 0.2f;

		Out.vColor.a *= g_fAlpha;

	return Out;
}

PS_OUT PS_OUTLINE2(PS_IN In)
{
	PS_OUT      Out = (PS_OUT)0;

	//vector Color_rgb;
	//Out.vColor = g_DiffuseTexture.Sample(LinearSampler, In.vTexUV);

	//float fRight_Line = (g_MiddlePoint.x + (g_UV_sizeX*0.5f));
	//float fUp_Line = (g_MiddlePoint.y - (g_UV_sizeY*0.5f));
	//float fDown_Line = (g_MiddlePoint.y + (g_UV_sizeY*0.5f));
	//float fLeft_Line = (g_MiddlePoint.x - (g_UV_sizeX*0.5f));

	////float fRight_Line = In.vTexUV.x + (In.vTexUV.x *0.5f);
	////float fUp_Line =   In.vTexUV.y + (In.vTexUV.y *0.5f);
	////float fDown_Line = In.vTexUV.y - (In.vTexUV.y *0.5f);
	////float fLeft_Line = In.vTexUV.x - (In.vTexUV.x *0.5f);


	//if (fLeft_Line < g_MiddlePoint.x && g_MiddlePoint.x < fLeft_Line + 12.f ||
	//	fRight_Line > g_MiddlePoint.x && g_MiddlePoint.x > fRight_Line - 12.f ||
	//	fDown_Line > g_MiddlePoint.y&& g_MiddlePoint.y > fDown_Line - 12.f ||
	//	fUp_Line < g_MiddlePoint.y && g_MiddlePoint.y < fUp_Line + 12.f)
	//{
	//	
	//	Out.vColor = float4(1.f, 1.f, 1.f, 1.f);
	//	Out.vColor.a *= 0.5f;
	//}
	//else
	//{
	//	Out.vColor = g_DiffuseTexture.Sample(LinearSampler, In.vTexUV);
	//	//Out.vColor = float4(1.f, 1.f, 1.f, 1.f);
	//	Out.vColor.a *= 0.5f;
	//}
	Out.vColor = g_DiffuseTexture.Sample(LinearSampler, In.vTexUV);
	if (In.vTexUV.x < 0.01f || In.vTexUV.x > 0.99f)
		Out.vColor.rgb = 0.2f;// (0.2f, 0.2f, 0.2f,


	if (In.vTexUV.y < 0.04f || In.vTexUV.y > 0.96f)
		Out.vColor.rgb = 0.2f;

	Out.vColor.a *= g_fAlpha;

	return Out;
}

PS_OUT PS_BoostGuage(PS_IN In)
{
	PS_OUT      Out = (PS_OUT)0;

	if (g_fCurrentHp / g_fMaxHp < In.vTexUV.x)
		discard;

	Out.vColor = g_DiffuseTexture.Sample(LinearSampler, In.vTexUV);

	//Out.vColor.a *= -g_fAlpha;



	return Out;
}

PS_OUT PS_TODOCOMPLETE(PS_IN In)
{
	PS_OUT      Out = (PS_OUT)0;


	Out.vColor = g_DiffuseTexture.Sample(LinearSampler, In.vTexUV);

	Out.vColor.b = 1.f;



	return Out;
}




PS_OUT PS_BATTLESTART(PS_IN In)
{
	PS_OUT Out = (PS_OUT)0;

	float fSpriteCount = g_vSprite.x * g_vSprite.y;
	float fSpriteWidth = 1.0 / g_vSprite.x;
	float fSpriteHeight = 1.0 / g_vSprite.y;

	float fFullSprites = fSpriteCount - 1.0;
	float fTimePerFullSprite = g_fSpriteDuration / fFullSprites;
	float fTimePerLastSprite = fTimePerFullSprite * fSpriteWidth * (1.0 / fSpriteHeight);
	float fTimePerSprite = (fTimePerFullSprite * fFullSprites + fTimePerLastSprite) / fSpriteCount;

	float fSpriteIndex = floor(g_fSpriteTimer / fTimePerSprite);
	float fRow = floor(fSpriteIndex / g_vSprite.x);
	float fCol = fSpriteIndex % g_vSprite.x;

	float u = fCol * fSpriteWidth;
	float v = fRow * fSpriteHeight;
	float2 vSpriteUV = float2(u + In.vTexUV.x * fSpriteWidth, v + In.vTexUV.y * fSpriteHeight);

	Out.vColor = g_DiffuseTexture.Sample(LinearSampler, vSpriteUV);
	Out.vColor.a = Out.vColor.r;

	float3 vFirstColor = float3(.96f, .87f, .3f);
	float3 vSecondColor = float3(.7f, .21f, .18f);

	float fInterpFactor = (Out.vColor.a - .5f) / (1.f - .5f);
	float3 vLerpColor = lerp(vSecondColor, vFirstColor, fInterpFactor);
	Out.vColor.rgb = vLerpColor;

	return Out;
}


PS_OUT PS_PROGRESSBAR(PS_IN In)
{
	PS_OUT      Out = (PS_OUT)0;
	float4 DiffuseTexture = g_DiffuseTexture.Sample(LinearSampler, In.vTexUV);
	//float duration = 500.f;

	float progress = g_fCurrentHp / g_fMaxHp;

	float innerRadius = 0.12f;
	float outerRadius = 0.18f;

	float middleRadius = 0.5f * (innerRadius + outerRadius);
	float halfWidth = 0.5f * (outerRadius - innerRadius);

	float2 pos = In.vTexUV.xy - 0.5f * g_WinXY.xy;
	//float2 pos = In.vTexUV.xy;
	float radius = length(pos.xy);

	float fr = halfWidth - abs(radius - middleRadius) + 1.f;
	/*if(fr < 0.0)
	discard;*/
	fr = saturate(fr);

	float angle = degrees(atan2(-pos.x, pos.y) + 0.f) + 180.f;
	float fa = radians(angle - progress * 360.f) * radius + 1.f;

	fa = saturate(fa);
	if (fa != 1.f)
		discard;
	vector color = vector(0.f, 0.f, 0.f, 1);
	vector col = lerp(color, DiffuseTexture, fa);
	//   col.a *= fr;

	//col = col * col2;//DiffuseTexture;

	Out.vColor = col;

	if (Out.vColor.a < 0.2f)
		discard;

	Out.vColor.a = 1.f;

	Out.vColor.a *= g_fAlpha;

	return Out;



//	return Out;
}



PS_OUT PS_BOSSHPBAR(PS_IN In)
{

	PS_OUT      Out = (PS_OUT)0;

	if (In.vTexUV.y > (1.4f - In.vTexUV.x) + (In.vTexUV.y))
		discard;
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
		Out.vColor = g_DiffuseTexture.Sample(LinearSampler, In.vTexUV);


	float fGradientRadius = 0.5f;
	float fGradientStrength = 0.2f;
	float  fLerp = 0.f;

	/* First Lerp */
	if (In.vTexUV.x > g_fBright - fGradientRadius && In.vTexUV.x < g_fBright)
	{
		float fStart = g_fBright - fGradientRadius;
		float fEnd = g_fBright;
		float fInterpFactor = (In.vTexUV.x - fStart) / (fEnd - fStart);
		fLerp = lerp(0, fGradientStrength, fInterpFactor);
	}
	/* Second Lerp */
	else if (In.vTexUV.x > g_fBright && In.vTexUV.x < g_fBright + fGradientRadius)
	{
		float fStart = g_fBright;
		float fEnd = g_fBright + fGradientRadius;
		float fInterpFactor = (In.vTexUV.x - fStart) / (fEnd - fStart);
		fLerp = lerp(fGradientStrength, 0, fInterpFactor);
	}

	Out.vColor.rgb += fLerp;   //fLerpValue;

	Out.vColor.r += 0.1f;
	Out.vColor.b -= 0.2f;

	if (Out.vColor.a<0.3f)
		discard;

	return Out;

	
}

PS_OUT PS_BOSSHPBARFULL(PS_IN In)
{
	PS_OUT      Out = (PS_OUT)0;

	if (In.vTexUV.y > (1.4f - In.vTexUV.x) + (In.vTexUV.y))
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


	float fGradientRadius = 0.5f;
	float fGradientStrength = 0.2f;
	float  fLerp = 0.f;

	/* First Lerp */
	if (In.vTexUV.x > g_fBright - fGradientRadius && In.vTexUV.x < g_fBright)
	{
		float fStart = g_fBright - fGradientRadius;
		float fEnd = g_fBright;
		float fInterpFactor = (In.vTexUV.x - fStart) / (fEnd - fStart);
		fLerp = lerp(0, fGradientStrength, fInterpFactor);
	}
	/* Second Lerp */
	else if (In.vTexUV.x > g_fBright && In.vTexUV.x < g_fBright + fGradientRadius)
	{
		float fStart = g_fBright;
		float fEnd = g_fBright + fGradientRadius;
		float fInterpFactor = (In.vTexUV.x - fStart) / (fEnd - fStart);
		fLerp = lerp(fGradientStrength, 0, fInterpFactor);
	}

	Out.vColor.rgb += fLerp;   //fLerpValue;
	Out.vColor.r += 0.05f;
	Out.vColor.b -= 0.2f;
	Out.vColor.g -= 0.05f;

	if (Out.vColor.a<0.3f)
		discard;

	return Out;
}

PS_OUT PS_BOSSBACKBLACK(PS_IN In)
{
	PS_OUT      Out = (PS_OUT)0;


	Out.vColor = g_DiffuseTexture.Sample(LinearSampler, In.vTexUV);
	if (Out.vColor.a < 0.5f)
		discard;
	Out.vColor.a = 0.5f;



	return Out;
}

PS_OUT PS_BOSSBACKWHITE(PS_IN In)
{
	PS_OUT      Out = (PS_OUT)0;


	Out.vColor = g_DiffuseTexture.Sample(LinearSampler, In.vTexUV);
	if (Out.vColor.a < 0.5f)
		discard;

	Out.vColor.rgb = float3(0.9686274509803922f, 0.8705882352941176f, 0.6823529411764706f);
	Out.vColor.a = 0.5f;



	return Out;
}

PS_OUT PS_EFFECTSCREEN(PS_IN In)
{
	PS_OUT Out = (PS_OUT)0;

	return Out;
}

PS_OUT PS_UI_GLOW(PS_IN In)
{
	PS_OUT Out = (PS_OUT)0;

	Out.vColor = g_GlowTexture.Sample(LinearSampler, In.vTexUV);

	if (g_bUseDiffuseColor)
	{
		float4 vDiffuse = g_DiffuseTexture.Sample(LinearSampler, In.vTexUV);
		Out.vColor = vDiffuse;
	}
	else
	{
		Out.vColor.a = Out.vColor.r;
		Out.vColor.rgb = g_vGlowColor;

		Out.vColor *=  min(cos(g_fGlowTimer * 4) + 1.5f, 1.f);
	}

	Out.vColor.a *= g_fAlpha;
	if (Out.vColor.a == 0)
		discard;

	

	return Out;
}

PS_OUT PS_EXPBAR(PS_IN In)
{
	PS_OUT      Out = (PS_OUT)0;
	float4 DiffuseTexture = g_DiffuseTexture.Sample(LinearSampler, In.vTexUV);
	//float duration = 500.f;

	float progress = g_fCurrentHp / g_fMaxHp;

	float innerRadius = 0.12f;
	float outerRadius = 0.18f;

	float middleRadius = 0.5f * (innerRadius + outerRadius);
	float halfWidth = 0.5f * (outerRadius - innerRadius);

	float2 pos = In.vTexUV.xy - 0.5f * g_WinXY.xy;
	//float2 pos = In.vTexUV.xy;
	float radius = length(pos.xy);

	float fr = halfWidth - abs(radius - middleRadius) + 1.f;
	/*if(fr < 0.0)
	discard;*/
	fr = saturate(fr);

	float angle = degrees(atan2(pos.x, pos.y) + 0.f) + 180.f;
	float fa = radians(angle - progress * 360.f) * radius + 1.f;

	fa = saturate(fa);
	if (fa != 1.f)
		discard;
	vector color = vector(0.f, 0.f, 0.f, 1.f);
	vector col = lerp(color, DiffuseTexture, fa);
	//   col.a *= fr;

	//col = col * col2;//DiffuseTexture;
	if (Out.vColor.a < 0.2f)
		discard;
	Out.vColor = lerp(float4(0.801f, 0.784f, 0.545f, 1.f), float4(0.9882f, 0.8352f, 0.647f, 1.f), 0.2f);

	/*

	Out.vColor.a += 0.6f;*/

	return Out;



	//	return Out;
}

PS_OUT PS_STRIKEEFFECT(PS_IN In)
{
	PS_OUT      Out = (PS_OUT)0;

	int TotalIndex = 5;
	int CurIndex = g_SpriteCurTime / g_SpriteSpeed;

	if (g_SpriteLoop && CurIndex >= TotalIndex - 1)
	{
		CurIndex = 0;
	}
	else if (CurIndex >= TotalIndex - 1)
	{
		CurIndex = TotalIndex - 1;
	}

	//UV가로세로 개수
	int SpriteU = (int)g_UVSpriteValue.x;
	int SpriteV = (int)g_UVSpriteValue.y;

	//밸류당 UV
	float ValueX = 1.f / SpriteU;
	float ValueY = 1.f / SpriteV;

	//현재 인덱스의 XY
	float CurX = CurIndex % SpriteU;
	float CurY = CurIndex / SpriteU;

	float2 NewUV = float2((In.vTexUV.x * ValueX) + (ValueX * CurX), (In.vTexUV.y * ValueY) + (ValueY * CurY));

	Out.vColor = g_DiffuseTexture.Sample(LinearSampler, NewUV);

	Out.vColor.a = Out.vColor.rgb;





	return Out;
}

PS_OUT PS_UI_SPRITE_GLOW(PS_IN In)
{
	PS_OUT Out = (PS_OUT)0;

	float fSpriteCount = g_vSprite.x * g_vSprite.y;
	float fSpriteWidth = 1.0 / g_vSprite.x;
	float fSpriteHeight = 1.0 / g_vSprite.y;

	float fFullSprites = fSpriteCount - 1.0;
	float fTimePerFullSprite = g_fSpriteDuration / fFullSprites;
	float fTimePerLastSprite = fTimePerFullSprite * fSpriteWidth * (1.0 / fSpriteHeight);
	float fTimePerSprite = (fTimePerFullSprite * fFullSprites + fTimePerLastSprite) / fSpriteCount;

	float fSpriteIndex = floor(g_fSpriteTimer / fTimePerSprite);
	float fRow = floor(fSpriteIndex / g_vSprite.x);
	float fCol = fSpriteIndex % g_vSprite.x;

	float u = fCol * fSpriteWidth;
	float v = fRow * fSpriteHeight;
	float2 vSpriteUV = float2(u + In.vTexUV.x * fSpriteWidth, v + In.vTexUV.y * fSpriteHeight);
	 
	Out.vColor = g_GlowTexture.Sample(LinearSampler, vSpriteUV);
	Out.vColor.a = Out.vColor.r;

	float3 vFirstColor = float3(.96f, .87f, .3f);
	float3 vSecondColor = float3(.7f, .21f, .18f);

	float fInterpFactor = (Out.vColor.a - .5f) / (1.f - .5f);
	float3 vLerpColor = lerp(vSecondColor, vFirstColor, fInterpFactor);
	Out.vColor.rgb = vLerpColor;

	if (Out.vColor.a == 0)
		discard;

	return Out;
}


PS_OUT PS_Recoverhpfont(PS_IN In)
{
	PS_OUT      Out = (PS_OUT)0;

	/*if (In.vTexUV.y > (1.4f - In.vTexUV.x) + (In.vTexUV.y))
		discard;*/
	float4 origincolor = g_DiffuseTexture.Sample(LinearSampler, In.vTexUV);

	if (origincolor.a > 0.4f)
	{
		float4 maskcolor = g_GradationTexture.Sample(LinearSampler, In.vTexUV);

		float4 lerpcolor = lerp(float4(0.701f, 0.784f, 0.545f, 1.f), float4(0.7882f, 0.8352f, 0.647f, 1.f), maskcolor);

		Out.vColor = lerpcolor;
	}
	else
		discard;
	//	Out.vColor = g_DiffuseTexture.Sample(LinearSampler, In.vTexUV);



	if (Out.vColor.a<0.3f)
		discard;

	Out.vColor.a *= g_fAlpha;

	return Out;
}

PS_OUT PS_Hithpfont(PS_IN In)
{
	PS_OUT      Out = (PS_OUT)0;

	/*if (In.vTexUV.y > (1.4f - In.vTexUV.x) + (In.vTexUV.y))
	discard;*/
	float4 origincolor = g_DiffuseTexture.Sample(LinearSampler, In.vTexUV);

	if (origincolor.g != 0.f)
	{
		if (origincolor.a > 0.4f)
		{
			float4 maskcolor = g_GradationTexture.Sample(LinearSampler, In.vTexUV);

			float4 lerpcolor = lerp(float4(0.9372549019607843f, 0.5725490196078431f, 0.5725490196078431f, 1.f), float4(0.9607843137254902f, 0.3098039215686275f, 0.5529411764705882f, 1.f), maskcolor);

			Out.vColor = lerpcolor;
		}
		else
			discard;
	}
	else
		Out.vColor = origincolor;

	
	//	Out.vColor = g_DiffuseTexture.Sample(LinearSampler, In.vTexUV);



	if (Out.vColor.a<0.3f)
	discard;

	Out.vColor.a *= g_fAlpha;

	return Out;
}

PS_OUT PS_JUSTDODGE(PS_IN In)
{
	

	PS_OUT		Out = (PS_OUT)0;

	Out.vColor = g_DiffuseTexture.Sample(LinearSampler, In.vTexUV);
	

	Out.vColor.a = Out.vColor.rgb;

	Out.vColor.rg -= 0.3f;

	Out.vColor.a *= g_fAlpha;

	return Out;
}


PS_OUT PS_ResistDamagefont(PS_IN In)
{


	PS_OUT      Out = (PS_OUT)0;

	/*if (In.vTexUV.y > (1.4f - In.vTexUV.x) + (In.vTexUV.y))
	discard;*/
	float4 origincolor = g_DiffuseTexture.Sample(LinearSampler, In.vTexUV);

	if (origincolor.g != 0.f)
	{
		if (origincolor.a > 0.4f)
		{
			float4 maskcolor = g_GradationTexture.Sample(LinearSampler, In.vTexUV);

			float4 lerpcolor = lerp(float4(0.5764705882352941f, 0.6666666666666667f, 0.8274509803921569f, 1.f), float4(0.99f, 0.99f, 1.f, 1.f), maskcolor);

			Out.vColor = lerpcolor;
		}
		else
			discard;
	}
	else
		Out.vColor = origincolor;


	if (Out.vColor.a<0.3f)
		discard;
	Out.vColor.a *= g_fAlpha;

	return Out;
}


PS_OUT PS_CRITICALDAMAGE(PS_IN In)
{


	//PS_OUT		Out = (PS_OUT)0;

	//Out.vColor = g_DiffuseTexture.Sample(LinearSampler, In.vTexUV);


	////Out.vColor.a = Out.vColor.rgb;

	//Out.vColor.b = 0.2745098039215686f;
	//if (Out.vColor.a<0.3f)
	//	discard;
	//Out.vColor.a *= g_fAlpha;

	//return Out;


	PS_OUT      Out = (PS_OUT)0;

	/*if (In.vTexUV.y > (1.4f - In.vTexUV.x) + (In.vTexUV.y))
	discard;*/
	float4 origincolor = g_DiffuseTexture.Sample(LinearSampler, In.vTexUV);

	if (origincolor.g != 0.f)
	{
		if (origincolor.a > 0.4f)
		{
			float4 maskcolor = g_GradationTexture.Sample(LinearSampler, In.vTexUV);

			float4 lerpcolor = lerp(float4(0.99f, 0.89f, 0.42f, 1.f), float4(0.99f, 0.99f, 1.f, 1.f), maskcolor);

			Out.vColor = lerpcolor;
		}
		else
			discard;
	}
	else
		Out.vColor = origincolor;


	if (Out.vColor.a<0.3f)
		discard;
	Out.vColor.a *= g_fAlpha;

	return Out;
}


technique11 DefaultTechnique
{
	pass Default
	{
		SetRasterizerState(RS_Default);
		SetBlendState(BS_AlphaBlending, float4(0.f, 0.f, 0.f, 1.f), 0xffffffff);
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

	pass UVROT
	{
		SetRasterizerState(RS_Default);
		SetBlendState(BS_AlphaBlending, float4(0.f, 0.f, 0.f, 1.f), 0xffffffff);
		SetDepthStencilState(DSS_Priority, 0);

		VertexShader = compile vs_5_0 VS_MAIN();
		GeometryShader = NULL;
		PixelShader = compile ps_5_0 PS_UVROTATION();
	}

	pass INVENICON
	{
		SetRasterizerState(RS_Default);
		SetBlendState(BS_AlphaBlending, float4(0.f, 0.f, 0.f, 1.f), 0xffffffff);
		SetDepthStencilState(DSS_Priority, 0);

		VertexShader = compile vs_5_0 VS_MAIN();
		GeometryShader = NULL;
		PixelShader = compile ps_5_0 PS_INVENICON();
	}

	pass INVENBACK
	{
		SetRasterizerState(RS_Default);
		SetBlendState(BS_AlphaBlending, float4(0.f, 0.f, 0.f, 1.f), 0xffffffff);
		SetDepthStencilState(DSS_Priority, 0);

		VertexShader = compile vs_5_0 VS_MAIN();
		GeometryShader = NULL;
		PixelShader = compile ps_5_0 PS_INVENBACK();
	}

	pass MENULINE
	{
		SetRasterizerState(RS_Default);
		SetBlendState(BS_AlphaBlending, float4(0.f, 0.f, 0.f, 1.f), 0xffffffff);
		SetDepthStencilState(DSS_Priority, 0);

		VertexShader = compile vs_5_0 VS_MAIN();
		GeometryShader = NULL;
		PixelShader = compile ps_5_0 PS_MENULINE();
	}

	pass INVENTOPBOTTOM
	{
		SetRasterizerState(RS_Default);
		SetBlendState(BS_AlphaBlending, float4(0.f, 0.f, 0.f, 1.f), 0xffffffff);
		SetDepthStencilState(DSS_Priority, 0);

		VertexShader = compile vs_5_0 VS_MAIN();
		GeometryShader = NULL;
		PixelShader = compile ps_5_0 PS_INVENTOPBOTTOM();
	}

	pass INVENTOPBOTTOMALPHA
	{
		SetRasterizerState(RS_Default);
		SetBlendState(BS_AlphaBlending, float4(0.f, 0.f, 0.f, 1.f), 0xffffffff); //23
		SetDepthStencilState(DSS_Priority, 0);

		VertexShader = compile vs_5_0 VS_MAIN();
		GeometryShader = NULL;
		PixelShader = compile ps_5_0 PS_INVENTOPBOTTOMALPHA();
	}

	pass GALDBACK
	{
		SetRasterizerState(RS_Default);
		SetBlendState(BS_AlphaBlending, float4(0.f, 0.f, 0.f, 1.f), 0xffffffff);  //24
		SetDepthStencilState(DSS_Priority, 0);

		VertexShader = compile vs_5_0 VS_MAIN();
		GeometryShader = NULL;
		PixelShader = compile ps_5_0 PS_GALDBACK();
	}
	
	pass UVROTATIONSTRONG
	{
		SetRasterizerState(RS_Default);
		SetBlendState(BS_AlphaBlending, float4(0.f, 0.f, 0.f, 1.f), 0xffffffff);  //25
		SetDepthStencilState(DSS_Priority, 0);

		VertexShader = compile vs_5_0 VS_MAIN();
		GeometryShader = NULL;
		PixelShader = compile ps_5_0 PS_UVROTATIONSTRONG();
	}
	
	pass USINGITEMPORTRAIT
	{
		SetRasterizerState(RS_Default);
		SetBlendState(BS_AlphaBlending, float4(0.f, 0.f, 0.f, 1.f), 0xffffffff);  //26
		SetDepthStencilState(DSS_Priority, 0);

		VertexShader = compile vs_5_0 VS_MAIN();
		GeometryShader = NULL;
		PixelShader = compile ps_5_0 PS_USINGITEMPORTRAIT();
	}

	pass LIGHTEFFECT
	{
		SetRasterizerState(RS_Default);
		SetBlendState(BS_AlphaBlending, float4(0.f, 0.f, 0.f, 1.f), 0xffffffff);  //27
		SetDepthStencilState(DSS_Priority, 0);

		VertexShader = compile vs_5_0 VS_MAIN();
		GeometryShader = NULL;
		PixelShader = compile ps_5_0 PS_LIGHTEFFECT();
	}

	pass Bright
	{
		SetRasterizerState(RS_Default);
		SetBlendState(BS_AlphaBlending, float4(0.f, 0.f, 0.f, 1.f), 0xffffffff); //28
		SetDepthStencilState(DSS_Priority, 0);

		VertexShader = compile vs_5_0 VS_MAIN();
		GeometryShader = NULL;
		PixelShader = compile ps_5_0 PS_Bright();
	}

	pass BLUEHPBAR
	{
		SetRasterizerState(RS_Default);
		SetBlendState(BS_AlphaBlending, float4(0.f, 0.f, 0.f, 1.f), 0xffffffff);
		SetDepthStencilState(DSS_Priority, 0);

		VertexShader = compile vs_5_0 VS_MAIN();
		GeometryShader = NULL;
		PixelShader = compile ps_5_0 PS_HPbarBLUE();                //29
	}

	pass Dialoguebox
	{
		SetRasterizerState(RS_Default);
		SetBlendState(BS_AlphaBlending, float4(0.f, 0.f, 0.f, 1.f), 0xffffffff);
		SetDepthStencilState(DSS_Priority, 0);

		VertexShader = compile vs_5_0 VS_MAIN();
		GeometryShader = NULL;
		PixelShader = compile ps_5_0 PS_Dialoguebox();                //30
	}

	pass Dialogueline
	{
		SetRasterizerState(RS_Default);
		SetBlendState(BS_AlphaBlending, float4(0.f, 0.f, 0.f, 1.f), 0xffffffff);
		SetDepthStencilState(DSS_Priority, 0);

		VertexShader = compile vs_5_0 VS_MAIN();
		GeometryShader = NULL;
		PixelShader = compile ps_5_0 PS_DialogueLINE();                //31
	}
	
	pass DIALOGUECURSOR
	{
		SetRasterizerState(RS_Default);
		SetBlendState(BS_AlphaBlending, float4(0.f, 0.f, 0.f, 1.f), 0xffffffff);
		SetDepthStencilState(DSS_Priority, 0);

		VertexShader = compile vs_5_0 VS_MAIN();
		GeometryShader = NULL;
		PixelShader = compile ps_5_0 PS_DIALOGUECURSOR();                //32
	}

	pass DIALOGUECURSORNONMOVE
	{
		SetRasterizerState(RS_Default);
		SetBlendState(BS_AlphaBlending, float4(0.f, 0.f, 0.f, 1.f), 0xffffffff);
		SetDepthStencilState(DSS_Priority, 0);

		VertexShader = compile vs_5_0 VS_MAIN();
		GeometryShader = NULL;
		PixelShader = compile ps_5_0 PS_DIALOGUECURSORNOTMOVE();                //33
	}
	
	pass ALPHASETTEST
	{
		SetRasterizerState(RS_Default);
		SetBlendState(BS_AlphaBlending, float4(0.f, 0.f, 0.f, 1.f), 0xffffffff);
		SetDepthStencilState(DSS_Priority, 0);

		VertexShader = compile vs_5_0 VS_MAIN();
		GeometryShader = NULL;
		PixelShader = compile ps_5_0 PS_ALPHATESTSET();                //34
	}
	
	pass BRIGHTBLACK
	{
		SetRasterizerState(RS_Default);
		SetBlendState(BS_AlphaBlending, float4(0.f, 0.f, 0.f, 1.f), 0xffffffff);
		SetDepthStencilState(DSS_Priority, 0);

		VertexShader = compile vs_5_0 VS_MAIN();
		GeometryShader = NULL;
		PixelShader = compile ps_5_0 PS_BRIGHTFORBLACK();                //35
	}

	pass BRIGHTDialogueLine
	{
		SetRasterizerState(RS_Default);
		SetBlendState(BS_AlphaBlending, float4(0.f, 0.f, 0.f, 1.f), 0xffffffff);
		SetDepthStencilState(DSS_Priority, 0);

		VertexShader = compile vs_5_0 VS_MAIN();
		GeometryShader = NULL;
		PixelShader = compile ps_5_0 PS_BRIGHTDialogueLINE();                //36
	}

	pass OUTLINE
	{
		SetRasterizerState(RS_Default);
		SetBlendState(BS_AlphaBlending, float4(0.f, 0.f, 0.f, 1.f), 0xffffffff);
		SetDepthStencilState(DSS_Priority, 0);

		VertexShader = compile vs_5_0 VS_MAIN();
		GeometryShader = NULL;
		PixelShader = compile ps_5_0 PS_OUTLINE();                //37
	}

	pass OUTLINE2
	{
		SetRasterizerState(RS_Default);
		SetBlendState(BS_AlphaBlending, float4(0.f, 0.f, 0.f, 1.f), 0xffffffff);
		SetDepthStencilState(DSS_Priority, 0);

		VertexShader = compile vs_5_0 VS_MAIN();
		GeometryShader = NULL;
		PixelShader = compile ps_5_0 PS_OUTLINE2();                //38
	}

	pass BoostGuage
	{
		SetRasterizerState(RS_Default);
		SetBlendState(BS_AlphaBlending, float4(0.f, 0.f, 0.f, 1.f), 0xffffffff);
		SetDepthStencilState(DSS_Priority, 0);

		VertexShader = compile vs_5_0 VS_MAIN();
		GeometryShader = NULL;
		PixelShader = compile ps_5_0 PS_BoostGuage();                //39
	}

	pass TODOCOMPLETE
	{
		SetRasterizerState(RS_Default);
		SetBlendState(BS_AlphaBlending, float4(0.f, 0.f, 0.f, 1.f), 0xffffffff);
		SetDepthStencilState(DSS_Priority, 0);

		VertexShader = compile vs_5_0 VS_MAIN();
		GeometryShader = NULL;
		PixelShader = compile ps_5_0 PS_TODOCOMPLETE();                //40
	}

	pass BATTLESTART
	{
		SetRasterizerState(RS_Default);
		SetBlendState(BS_AlphaBlending, float4(0.f, 0.f, 0.f, 1.f), 0xffffffff);
		SetDepthStencilState(DSS_Priority, 0);

		VertexShader = compile vs_5_0 VS_MAIN();
		GeometryShader = NULL;
		PixelShader = compile ps_5_0 PS_BATTLESTART();                //41
	}
	
	pass PROGRESSBAR
	{
		SetRasterizerState(RS_Default);
		SetBlendState(BS_AlphaBlending, float4(0.f, 0.f, 0.f, 1.f), 0xffffffff);
		SetDepthStencilState(DSS_Priority, 0);

		VertexShader = compile vs_5_0 VS_MAIN();
		GeometryShader = NULL;
		PixelShader = compile ps_5_0 PS_PROGRESSBAR();                //42
	}

	pass BOSSHP
	{
		SetRasterizerState(RS_Default);
		SetBlendState(BS_AlphaBlending, float4(0.f, 0.f, 0.f, 1.f), 0xffffffff);
		SetDepthStencilState(DSS_Priority, 0);

		VertexShader = compile vs_5_0 VS_MAIN();
		GeometryShader = NULL;
		PixelShader = compile ps_5_0 PS_BOSSHPBAR();                //43
	}

	pass BOSSHPFULL
	{
		SetRasterizerState(RS_Default);
		SetBlendState(BS_AlphaBlending, float4(0.f, 0.f, 0.f, 1.f), 0xffffffff);
		SetDepthStencilState(DSS_Priority, 0);

		VertexShader = compile vs_5_0 VS_MAIN();
		GeometryShader = NULL;
		PixelShader = compile ps_5_0 PS_BOSSHPBARFULL();                //44
	}

	pass BOSSHPBBLACK
	{
		SetRasterizerState(RS_Default);
		SetBlendState(BS_AlphaBlending, float4(0.f, 0.f, 0.f, 1.f), 0xffffffff);
		SetDepthStencilState(DSS_Priority, 0);

		VertexShader = compile vs_5_0 VS_MAIN();
		GeometryShader = NULL;
		PixelShader = compile ps_5_0 PS_BOSSBACKBLACK();                //45
	}

	pass BOSSHPBWHITE
	{
		SetRasterizerState(RS_Default);
		SetBlendState(BS_AlphaBlending, float4(0.f, 0.f, 0.f, 1.f), 0xffffffff);
		SetDepthStencilState(DSS_Priority, 0);

		VertexShader = compile vs_5_0 VS_MAIN();
		GeometryShader = NULL;
		PixelShader = compile ps_5_0 PS_BOSSBACKWHITE();                //46
	}

	pass UI_EFFECTSCREEN
	{
		SetRasterizerState(RS_Default);
		SetBlendState(BS_AlphaBlending, float4(0.f, 0.f, 0.f, 1.f), 0xffffffff);
		SetDepthStencilState(DSS_Priority, 0);

		VertexShader = compile vs_5_0 VS_MAIN();
		GeometryShader = NULL;
		PixelShader = compile ps_5_0 PS_EFFECTSCREEN();                //47
	}
	
	pass UI_GLOW // 48
	{
		SetRasterizerState(RS_Default);
		SetBlendState(BS_AlphaBlending, float4(0.f, 0.f, 0.f, 1.f), 0xffffffff);
		SetDepthStencilState(DSS_Priority, 0);

		VertexShader = compile vs_5_0 VS_MAIN();
		GeometryShader = NULL;
		PixelShader = compile ps_5_0 PS_UI_GLOW();
	}

	pass EXPBAR // 49
	{
		SetRasterizerState(RS_Default);
		SetBlendState(BS_AlphaBlending, float4(0.f, 0.f, 0.f, 1.f), 0xffffffff);
		SetDepthStencilState(DSS_Priority, 0);

		VertexShader = compile vs_5_0 VS_MAIN();
		GeometryShader = NULL;
		PixelShader = compile ps_5_0 PS_EXPBAR();
	}

	pass STRIKEEFFECT // 50
	{
		SetRasterizerState(RS_Default);
		SetBlendState(BS_AlphaBlending, float4(0.f, 0.f, 0.f, 1.f), 0xffffffff);
		SetDepthStencilState(DSS_Priority, 0);

		VertexShader = compile vs_5_0 VS_MAIN();
		GeometryShader = NULL;
		PixelShader = compile ps_5_0 PS_STRIKEEFFECT();
	}

	pass UI_SPRITE_GLOW // 51
	{
		SetRasterizerState(RS_Default);
		SetBlendState(BS_AlphaBlending, float4(0.f, 0.f, 0.f, 1.f), 0xffffffff);
		SetDepthStencilState(DSS_Priority, 0);

		VertexShader = compile vs_5_0 VS_MAIN();
		GeometryShader = NULL;
		PixelShader = compile ps_5_0 PS_UI_SPRITE_GLOW();
	}

	pass UI_Recovoerfont // 52
	{
		SetRasterizerState(RS_Default);
		SetBlendState(BS_AlphaBlending, float4(0.f, 0.f, 0.f, 1.f), 0xffffffff);
		SetDepthStencilState(DSS_Priority, 0);

		VertexShader = compile vs_5_0 VS_MAIN();
		GeometryShader = NULL;
		PixelShader = compile ps_5_0 PS_Recoverhpfont();
	}

	pass UI_PLAYERHIT // 53
	{
		SetRasterizerState(RS_Default);
		SetBlendState(BS_AlphaBlending, float4(0.f, 0.f, 0.f, 1.f), 0xffffffff);
		SetDepthStencilState(DSS_Priority, 0);

		VertexShader = compile vs_5_0 VS_MAIN();
		GeometryShader = NULL;
		PixelShader = compile ps_5_0 PS_Hithpfont();
	}

	pass UI_JUSTDODGE // 54
	{
		SetRasterizerState(RS_Default);
		SetBlendState(BS_AlphaBlending, float4(0.f, 0.f, 0.f, 1.f), 0xffffffff);
		SetDepthStencilState(DSS_Priority, 0);

		VertexShader = compile vs_5_0 VS_MAIN();
		GeometryShader = NULL;
		PixelShader = compile ps_5_0 PS_JUSTDODGE();
	}
	

	pass UI_RESISTDAMAGEFONT // 55
	{
		SetRasterizerState(RS_Default);
		SetBlendState(BS_AlphaBlending, float4(0.f, 0.f, 0.f, 1.f), 0xffffffff);
		SetDepthStencilState(DSS_Priority, 0);

		VertexShader = compile vs_5_0 VS_MAIN();
		GeometryShader = NULL;
		PixelShader = compile ps_5_0 PS_ResistDamagefont();
	}

	pass UI_CRITICALDAMAGE // 56
	{
		SetRasterizerState(RS_Default);
		SetBlendState(BS_AlphaBlending, float4(0.f, 0.f, 0.f, 1.f), 0xffffffff);
		SetDepthStencilState(DSS_Priority, 0);

		VertexShader = compile vs_5_0 VS_MAIN();
		GeometryShader = NULL;
		PixelShader = compile ps_5_0 PS_CRITICALDAMAGE();
	}
	

	
}