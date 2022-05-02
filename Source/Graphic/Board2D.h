#pragma once
#include <DxLib.h>
#include <string>
#include "../Math/VectorInterface.h"

class Board2D
{
public:
	Board2D(const Vector2<float>& pos, const Vector2<float>& size);
	~Board2D();

	void SetPixelShader(const std::string& filepath);
	void SetPixelShader(int handle);

	void SetTextureHandle(int handle) { texture_ = handle; }


	void Draw(void);
private:
	void Initialize(const Vector2<float>& pos, const Vector2<float>& size);
	VERTEX2DSHADER vert_[6];

	int ps_;
	int texture_;
};

