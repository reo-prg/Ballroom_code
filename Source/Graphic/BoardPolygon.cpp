#include "BoardPolygon.h"

BoardPolygon::BoardPolygon()
	:centerPos_({ 0.0f, 0.0f, 0.0f }), scale_(1.0f, 1.0f), texinvx_(false), texinvy_(false)
{
	Initialize();
}

BoardPolygon::~BoardPolygon()
{
}

void BoardPolygon::SetPosition(const Vector2<float>& pos)
{
	centerPos_.x = pos.x;
	centerPos_.y = pos.y;
	ApplyPosToVert();
}

void BoardPolygon::SetZ(float zpos)
{
	centerPos_.z = zpos;
	ApplyPosToVert();
}

void BoardPolygon::SetPosition(const VECTOR& pos)
{
	centerPos_ = pos;
	ApplyPosToVert();
}

void BoardPolygon::SetRotate(float angle)
{
	angle_ = angle;
	angleMat_ = RotateMat(angle_);
	ApplyPosToVert();
}

void BoardPolygon::AddRotate(float angle)
{
	angle_ += angle;
	angleMat_ = RotateMat(angle_);
	ApplyPosToVert();
}

void BoardPolygon::SetScale(const Vector2<float>& scale)
{
	scale_ = scale;
	ApplyPosToVert();
}

void BoardPolygon::TextureInvertX(bool flag)
{
	texinvx_ = flag;
	ApplyUvToVert();
}

void BoardPolygon::TextureInvertY(bool flag)
{
	texinvy_ = flag;
	ApplyUvToVert();
}

void BoardPolygon::SetTexture(int handle)
{
	textureHandle_ = handle;
}

void BoardPolygon::SetVertexShader(int handle)
{
	vertexShaderHandle_ = handle;
}

void BoardPolygon::SetPixelShader(int handle)
{
	pixelShaderHandle_ = handle;
}

void BoardPolygon::Initialize(void)
{
	angle_ = 0.0f;
	angleMat_ = IdentityMat();

	// 頂点の設定
	for (int i = 0; i < vert_.size(); i++)
	{
		vert_[i].dif = { 0xff, 0xff, 0xff, 0xff };
		vert_[i].spc = { 0x00, 0x00, 0x00, 0x00 };
		vert_[i].norm = { 0.0f, 0.0f, -1.0f };

		vert_[i].pos = { i & 0b10 ? 1.0f : -1.0f, i & 0b01 ? -1.0f : 1.0f, 0.0f };
		vert_[i].u = vert_[i].su = i & 0b10 ? 1.0f : 0.0f;
		vert_[i].v = vert_[i].sv = i & 0b01 ? 1.0f : 0.0f;
	}

	index_ = { 0, 2, 1, 1, 2, 3 };

	// マテリアルの設定
	matparam_.Ambient = { 1.0f, 1.0f, 1.0f, 1.0f };
	matparam_.Diffuse = { 1.0f, 1.0f, 1.0f, 1.0f };
	matparam_.Emissive = { 0.0f, 0.0f, 0.0f, 0.0f };
	matparam_.Specular = { 0.0f, 0.0f, 0.0f, 0.0f };
	matparam_.Power = 0.0f;
}

void BoardPolygon::Draw(void)
{
	SetMaterialParam(matparam_);
	DrawPolygonIndexed3D(vert_.data(), static_cast<int>(vert_.size()), index_.data(), 2, textureHandle_, true);
}

void BoardPolygon::ApplyPosToVert(void)
{
	for (int i = 0; i < vert_.size(); i++)
	{
		Vector2<float> v = { i & 0b10 ? 1.0f : -1.0f, i & 0b01 ? -1.0f : 1.0f };

		v = MultipleVec(angleMat_, v);
		v *= scale_;
		vert_[i].pos = VAdd(VECTOR{ v.x, v.y, 0.0f }, centerPos_);
	}
}

void BoardPolygon::ApplyUvToVert(void)
{
	for (int i = 0; i < vert_.size(); i++)
	{
		vert_[i].u = vert_[i].su = i & 0b10 ? 1.0f : 0.0f;
		if (texinvx_) { vert_[i].u = vert_[i].su = 1.0f - vert_[i].u; }

		vert_[i].v = vert_[i].sv = i & 0b01 ? 1.0f : 0.0f;
		if (texinvy_) { vert_[i].v = vert_[i].sv = 1.0f - vert_[i].v; }
	}
}
