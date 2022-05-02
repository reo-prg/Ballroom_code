#pragma once
#include <EffekseerForDXLib.h>
#include <array>
#include "../Math/VectorInterface.h"
#include "../Math/Matrix33.h"

class BoardPolygon
{
public:
	BoardPolygon();
	~BoardPolygon();

	/// <summary>
	/// xy平面上で座標設定
	/// </summary>
	/// <param name="pos">座標</param>
	void SetPosition(const Vector2<float>& pos);
	
	/// <summary>
	/// 奥行き(z座標)の設定
	/// </summary>
	/// <param name="zpos">座標</param>
	void SetZ(float zpos);

	/// <summary>
	/// 3Dの座標設定
	/// </summary>
	/// <param name="pos">座標</param>
	void SetPosition(const VECTOR& pos);

	/// <summary>
	/// 回転の設定
	/// </summary>
	/// <param name="angle">角度(ラジアン)</param>
	void SetRotate(float angle);

	/// <summary>
	/// 回転の加算
	/// </summary>
	/// <param name="angle">角度(ラジアン)</param>
	void AddRotate(float angle);

	/// <summary>
	/// 拡大の設定
	/// </summary>
	/// <param name="scale">拡大率</param>
	void SetScale(const Vector2<float>& scale);

	/// <summary>
	/// 画像の左右を反転させるか
	/// </summary>
	void TextureInvertX(bool flag);

	/// <summary>
	/// 画像の上下を反転させるか
	/// </summary>
	void TextureInvertY(bool flag);

	/// <summary>
	/// 画像を設定する
	/// </summary>
	/// <param name="handle">画像ハンドル</param>
	void SetTexture(int handle);

	void SetVertexShader(int handle);
	void SetPixelShader(int handle);

	void Draw(void);
private:
	void Initialize(void);

	void ApplyPosToVert(void);
	void ApplyUvToVert(void);

	static constexpr int BoardVertNum = 4;
	static constexpr int BoardIndexNum = 6;

	std::array<VERTEX3D, BoardVertNum> vert_;
	std::array<unsigned short, BoardIndexNum> index_;

	VECTOR centerPos_;
	Vector2<float> scale_;

	float angle_;
	Matrix angleMat_;

	bool texinvx_;
	bool texinvy_;

	int vertexShaderHandle_ = -1;
	int pixelShaderHandle_ = -1;
	
	int textureHandle_ = -1;

	MATERIALPARAM matparam_;
};

