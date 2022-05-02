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

float4 main(PS_INPUT input) : SV_TARGET
{
	return lerp(tex.Sample(smp, input.TextureCoord0), maincolor, progress);
}
