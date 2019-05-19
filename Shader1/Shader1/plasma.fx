#define PI 3.1415926535897932384626433832795

//------------------------------ GLOBAL VARIALBLES -----------------------------
float time;


//------------------------------ TEXTURE PROPERTIES ----------------------------
texture ScreenTexture;

sampler TextureSampler = sampler_state
{
	Texture = <ScreenTexture>;
	MipFilter = LINEAR;
	MinFilter = LINEAR;
	MagFilter = LINEAR;
};

//------------------------ PIXEL SHADER ----------------------------------------
float4 plasma(float2 TextureCoordinate : TEXCOORD0) : COLOR0
{
	float2 k = float2(16,16);

	float v = 0.0;
	float2 c = TextureCoordinate * k - k / 2.0;
	v += sin((c.x + time));
	v += sin((c.y + time) / 2.0);
	v += sin((c.x + c.y + time) / 2.0);
	c += k / 2.0 * float2(sin(time / 3.0), cos(time / 2.0));
	v += sin(sqrt(c.x*c.x + c.y*c.y + 1.0) + time);
	v = v / 2.0;
	float3 col = float3(sin(PI*v), sin(PI*v + 4 * PI / 3), cos(PI*v));

	//to show plasma effect
	float4 color = float4(col, 1);

	//to show refraction effect
	//float4 color = tex2D(TextureSampler, TextureCoordinate + normalize(col*.5).xy*.015);

	return color;
}

//-------------------------- TECHNIQUES ----------------------------------------
technique plasma_technique
{
	pass Pass1
	{
		VertexShader = null;
		PixelShader = compile ps_3_0 plasma();
		ZEnable = false;
	}
}