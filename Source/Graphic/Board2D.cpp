#include "Board2D.h"

Board2D::Board2D(const Vector2<float>& pos, const Vector2<float>& size)
{
	Initialize(pos, size);
}

Board2D::~Board2D()
{
}

void Board2D::SetPixelShader(const std::string& filepath)
{
	ps_ = LoadPixelShader(filepath.c_str());
}

void Board2D::SetPixelShader(int handle)
{
	ps_ = handle;
}

void Board2D::Draw(void)
{
	SetUsePixelShader(ps_);
	SetUseTextureToShader(0, texture_);

	DrawPolygon2DToShader(vert_, 2);

	SetUsePixelShader(-1);
	SetUseTextureToShader(0, -1);
}

void Board2D::Initialize(const Vector2<float>& pos, const Vector2<float>& size)
{
	vert_[0].pos = { pos.x, pos.y };
	vert_[0].rhw = 1.0f;
	vert_[0].dif = GetColorU8(255, 255, 255, 255);
	vert_[0].spc = GetColorU8(0, 0, 0, 0);
	vert_[0].u = 0.0f;
	vert_[0].v = 0.0f;
	vert_[0].su = vert_[0].u;
	vert_[0].sv = vert_[0].v;

	vert_[1].pos = { pos.x + size.x, pos.y };
	vert_[1].rhw = 1.0f;
	vert_[1].dif = GetColorU8(255, 255, 255, 255);
	vert_[1].spc = GetColorU8(0, 0, 0, 0);
	vert_[1].u = 1.0f;
	vert_[1].v = 0.0f;
	vert_[1].su = vert_[1].u;
	vert_[1].sv = vert_[1].v;

	vert_[2].pos = { pos.x, pos.y + size.y };
	vert_[2].rhw = 1.0f;
	vert_[2].dif = GetColorU8(255, 255, 255, 255);
	vert_[2].spc = GetColorU8(0, 0, 0, 0);
	vert_[2].u = 0.0f;
	vert_[2].v = 1.0f;
	vert_[2].su = vert_[2].u;
	vert_[2].sv = vert_[2].v;

	vert_[3].pos = { pos.x, pos.y + size.y };
	vert_[3].rhw = 1.0f;
	vert_[3].dif = GetColorU8(255, 255, 255, 255);
	vert_[3].spc = GetColorU8(0, 0, 0, 0);
	vert_[3].u = 0.0f;
	vert_[3].v = 1.0f;
	vert_[3].su = vert_[3].u;
	vert_[3].sv = vert_[3].v;

	vert_[4].pos = { pos.x + size.x, pos.y };
	vert_[4].rhw = 1.0f;
	vert_[4].dif = GetColorU8(255, 255, 255, 255);
	vert_[4].spc = GetColorU8(0, 0, 0, 0);
	vert_[4].u = 1.0f;
	vert_[4].v = 0.0f;
	vert_[4].su = vert_[4].u;
	vert_[4].sv = vert_[4].v;

	vert_[5].pos = { pos.x + size.x, pos.y + size.y };
	vert_[5].rhw = 1.0f;
	vert_[5].dif = GetColorU8(255, 255, 255, 255);
	vert_[5].spc = GetColorU8(0, 0, 0, 0);
	vert_[5].u = 1.0f;
	vert_[5].v = 1.0f;
	vert_[5].su = vert_[5].u;
	vert_[5].sv = vert_[5].v;
}
