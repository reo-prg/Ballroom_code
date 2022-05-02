struct PS_INPUT
{
	float4 DiffuseColor  : COLOR0;      // ディフューズ
	float4 SpecularColor : COLOR1;      // スペキュラ
	float2 TextureCoord0 : TEXCOORD0;   // テクスチャ座標０
	float2 TextureCoord1 : TEXCOORD1;   // テクスチャ座標１
};

cbuffer postParam : register(b0)
{
	float time;
	float damagestr;
	float fade;
};

Texture2D<float4> tex : register(t0);

sampler smp : register(s0);

static const float PI = 3.14159265f;

float4 main(PS_INPUT input) : SV_TARGET
{
	float4 scene = tex.Sample(smp, input.TextureCoord0);
	float t = frac(time);

	float2 v1 = sin(input.TextureCoord0 * PI);
	v1 = 1.0f - v1;
	v1 = pow(v1, 5.0f);

	float v2 = sin(t * PI) * 0.15f;

	v2 = (v2 + saturate(v1.x + v1.y)) * damagestr;

	float4 dmgResult = lerp(scene, float4(0.72f, 0.0f, 0.0f, 1.0f), v2);

	return lerp(dmgResult, float4(0.0f, 0.0f, 0.0f, 1.0f), fade);
}
