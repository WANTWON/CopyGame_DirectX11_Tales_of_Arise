
sampler LinearSampler = sampler_state
{
	Filter = MIN_MAG_MIP_LINEAR;
	AddressU = wrap;
	AddressV = wrap;
};

sampler PointSampler = sampler_state
{
	Filter = MIN_MAG_MIP_POINT;
	AddressU = wrap;
	AddressV = wrap;
};

RasterizerState RS_Default
{
	FillMode = Solid;
	CullMode = Back;
	FrontCounterClockWise = false;
};

RasterizerState RS_WireFrame
{
	FillMode = WireFrame;
	CullMode = Back;
	FrontCounterClockWise = false;
};

RasterizerState RS_SkyBox
{
	FillMode = Solid;
	CullMode = Front;
	FrontCounterClockWise = false;
};

BlendState BS_Default
{
	BlendEnable[0] = false;
};

BlendState BS_AlphaBlending
{
	BlendEnable[0] = true;

	SrcBlend = Src_Alpha;
	DestBlend = Inv_Src_Alpha;
	BlendOP = Add;
};

BlendState BS_AddBlending
{
	BlendEnable[0] = true;

	SrcBlend = One;
	DestBlend = One;
	BlendOP = Add;
};

DepthStencilState DSS_Default
{
	DepthEnable = true;
	DepthWriteMask = All;
	DepthFunc = Less_Equal;
};

DepthStencilState DSS_Priority
{
	DepthEnable = false;
	DepthWriteMask = Zero;
};