#include "Square2DPolygon.h"
#include "Drawer.h"
#include "../Math/MathUtility.h"

Square2DPolygon::Square2DPolygon()
{
	Initialize();
}

Square2DPolygon::~Square2DPolygon()
{
	int a = 0;
}

void Square2DPolygon::SetPosition(const Vector2<float>& pos)
{
	pos_ = pos;
	translate_ = TranslateMat(pos_.x, pos_.y);
	isMatChanged_ = true;
}

void Square2DPolygon::SetDrawPriority(int priority)
{
	zPriority_ = priority;
}

void Square2DPolygon::SetRotateRad(float angle)
{
	angle_ = angle;
	rotate_ = RotateMat(angle_);
	isMatChanged_ = true;
}

void Square2DPolygon::SetRotateDeg(float angle)
{
	angle_ = DegreeToRadian(angle);
	rotate_ = RotateMat(angle_);
	isMatChanged_ = true;
}

void Square2DPolygon::SetSize(const Vector2<float>& scale)
{
	size_ = scale;
	scale_ = ScaleMat(size_.x, size_.y);
	isMatChanged_ = true;
}

void Square2DPolygon::TextureInvertX(bool flag)
{
	texinvx_ = flag;
	ApplyUVToVert();
}

void Square2DPolygon::TextureInvertY(bool flag)
{
	texinvy_ = flag;
	ApplyUVToVert();
}

void Square2DPolygon::SetModeTexture(int handle)
{
	mode_ = BoardDrawMode::Texture;
	textureHandle_ = handle;
}

void Square2DPolygon::SetModeColor(unsigned int color)
{
	mode_ = BoardDrawMode::Color;

	for (auto& v : vertices_)
	{
		v.dif = { (color >> 16) & 0xff, (color >> 8) & 0xff, color & 0xff, 0xff };
	}
}

void Square2DPolygon::Draw(void)
{
	ApplyMatrixToVert();
	switch (mode_)
	{
	case BoardDrawMode::Color:
		DrawerIns.Add2DDraw(
			[this]() {
				DrawPrimitiveIndexed2D(vertices_.data(), vertices_.size(),
					index_.data(), static_cast<int>(index_.size()), DX_PRIMTYPE_TRIANGLELIST, DX_NONE_GRAPH, true);
			}, zPriority_, type_);
		break;
	case BoardDrawMode::Texture:
		DrawerIns.Add2DDraw(
			[this]() {

				DrawPolygonIndexed2D(vertices_.data(), vertices_.size(),
					index_.data(), static_cast<int>(index_.size()) / 3, textureHandle_, true);
			}, zPriority_, type_);
		break;
	default:
		break;
	}
}

void Square2DPolygon::Initialize(void)
{
	for (int i = 0; i < vertices_.size(); i++)
	{
		basevertices_[i].dif = { 0xff, 0xff, 0xff, 0xff };
		basevertices_[i].rhw = 1.0f;

		basevertices_[i].pos = { i & 0b10 ? 0.5f : -0.5f, i & 0b01 ? -0.5f : 0.5f, 0.0f };
		basevertices_[i].u = i & 0b10 ? 1.0f : 0.0f;
		basevertices_[i].v = i & 0b01 ? 0.0f : 1.0f;
	}

	vertices_ = basevertices_;

	index_ = { 0, 2, 1, 1, 2, 3 };
}

void Square2DPolygon::ApplyUVToVert(void)
{
	for (int i = 0; i < vertices_.size(); i++)
	{
		vertices_[i].u = i & 0b10 ? 1.0f : 0.0f;
		if (texinvx_) { vertices_[i].u = 1.0f - vertices_[i].u; }

		vertices_[i].v = i & 0b01 ? 0.0f : 1.0f;
		if (texinvy_) { vertices_[i].v = 1.0f - vertices_[i].v; }
	}
}

void Square2DPolygon::ApplyMatrixToVert(void)
{
	if (!isMatChanged_) { return; }
	isMatChanged_ = false;

	for (int i = 0; i < vertices_.size(); i++)
	{
		Vector2<float> p = MultipleVec(MultipleMat(translate_, MultipleMat(rotate_, scale_)), 
			Vector2<float>{ basevertices_[i].pos.x, basevertices_[i].pos.y });
		vertices_[i].pos.x = p.x;
		vertices_[i].pos.y = p.y;
	}
}
