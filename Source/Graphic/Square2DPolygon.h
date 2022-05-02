#pragma once
#include <EffekseerForDXLib.h>
#include <array>
#include "DrawEnum.h"
#include "../Math/Matrix33.h"
#include "../Math/VectorInterface.h"

class Square2DPolygon
{
public:
	Square2DPolygon();
	~Square2DPolygon();

	void SetPosition(const Vector2<float>& pos);
	void SetDrawPriority(int priority);

	
	void SetRotateRad(float angle);
	void SetRotateDeg(float angle);
	void SetSize(const Vector2<float>& scale);

	/// <summary>
	/// 画像の左右を反転させるか
	/// </summary>
	void TextureInvertX(bool flag);
	
	/// <summary>
	/// 画像の上下を反転させるか
	/// </summary>
	void TextureInvertY(bool flag);

	/// <summary>
	/// 描画モードを画像にする
	/// </summary>
	/// <param name="handle">画像ハンドル</param>
	void SetModeTexture(int handle);

	/// <summary>
	/// 描画モードを単色にする
	/// </summary>
	/// <param name="color">色</param>
	void SetModeColor(unsigned int color);


	void SetDrawType(DrawType type) { type_ = type; }

	void Draw(void);
private:
	void Initialize(void);
	
	void ApplyUVToVert(void);
	void ApplyMatrixToVert(void);

	static constexpr int vertexcount = 4;
	static constexpr int indexarraycount = 6;

	std::array<VERTEX2D, vertexcount> basevertices_;

	std::array<VERTEX2D, vertexcount> vertices_;
	std::array<unsigned short, indexarraycount> index_;

	int textureHandle_ = -1;
	unsigned int color_ = 0x00ffff;

	BoardDrawMode mode_ = BoardDrawMode::Color;

	DrawType type_ = DrawType::Normal;

	Vector2<float> pos_;
	float angle_;
	int zPriority_;
	Vector2<float> size_;

	Matrix translate_;
	Matrix rotate_;
	Matrix scale_;

	bool isMatChanged_ = true;

	bool texinvx_ = false;
	bool texinvy_ = false;
};

