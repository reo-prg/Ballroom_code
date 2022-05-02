struct PS_INPUT
{
	float4 DiffuseColor  : COLOR0;      // ディフューズカラー
	float4 SpecularColor : COLOR1;      // スペキュラカラー
	float2 TextureCoord0 : TEXCOORD0;   // テクスチャ座標０
	float2 TextureCoord1 : TEXCOORD1;   // テクスチャ座標１
};

cbuffer fadeParam : register(b0)
{
	float4 maincolor;
	float4 subcolor;
	float4 param1;
	float4 param2;
	float4 param3;
	float4 param4;
	float progress;
};

Texture2D<float4> tex : register(t0);

sampler smp : register(s0);

float BoxSDFWithFMod(float2 pos, float distance, float2 size)
{
	float2 v = abs(fmod(pos, distance) - distance * 0.5f) - size;
	return length(max(v, 0.0f)) + min(max(v.x, v.y), 0.0f);
};

float SphereSDFWithFMod(float2 pos, float distance, float radius)
{
	return length(fmod(pos, distance) - distance * 0.5f) - radius;
};

float4 main(PS_INPUT input) : SV_TARGET
{
	float br = 0.0f;

	float w, h, level;
	tex.GetDimensions(0, w, h, level);

	float2 uv = abs(param1.x - input.TextureCoord0);
	uv.x = uv.x * w / h;
	//float v1 = floor(abs(param1.x - (input.TextureCoord0.x + input.TextureCoord0.y) / 2.0f) * 10.0f) / 10.0f;
	float2 v = floor(uv * 16.0f) / 16.0f;
	float v1 = (v.x + v.y) * 0.5f;
	float v2 = progress * 1.7f;

	if (v1 < v2)
	{
		br = 1.0f;
		if (v1 + 0.5f > v2)
		{
			br = 1.0f - ((v1 + 0.5f - v2) / 0.5f);
		}
	}

	br = pow(br, 2.0f);
	//float b = SphereSDFWithFMod(float2(input.TextureCoord0.x * w / h, input.TextureCoord0.y), 0.0625f, br * 0.045f);
	float b = BoxSDFWithFMod(float2(input.TextureCoord0.x * w / h, input.TextureCoord0.y), 0.0625f, float2(br * 0.03125f, br * 0.03125f));

	float4 col = maincolor;

	float c = 0.0f;
	if (b < 0.0f)
	{
		c = 1.0f;
	}

	return lerp(tex.Sample(smp, input.TextureCoord0), col, c);
}