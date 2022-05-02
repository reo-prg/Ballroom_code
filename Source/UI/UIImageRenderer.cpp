#include "UIImageRenderer.h"
#include "../Graphic/ImageHandleManager.h"
#include "../Graphic/Square2DPolygon.h"
#include "../Math/MathUtility.h"

UIImageRenderer::UIImageRenderer(const Vector2<float>& pos, const Vector2<float>& size, float angle, const std::string& texpath, int priority)
	: size_(size), angle_(angle), priority_(priority)
{
	pos_ = pos;
	Initialize(texpath);
}

UIImageRenderer::~UIImageRenderer()
{
	poly_.reset(nullptr);
}

void UIImageRenderer::SetDesc(const UIImageDesc& desc)
{
	SetPosition(desc.pos_);
	SetSize(desc.size_);
	SetDrawPriority(static_cast<float>(desc.priority_));
	SetTexture(desc.textureHandle_);
	SetRotateDeg(desc.angle_);
}

void UIImageRenderer::SetPosition(const Vector2<float>& pos)
{
	pos_ = pos;
	poly_->SetPosition(pos_);
}

void UIImageRenderer::SetPositionX(float x)
{
	pos_.x = x;
	poly_->SetPosition(pos_);
}

void UIImageRenderer::SetPositionY(float y)
{
	pos_.y = y;
	poly_->SetPosition(pos_);
}

void UIImageRenderer::SetDrawPriority(float priority)
{
	priority_ = priority;
	poly_->SetDrawPriority(priority_);
}

void UIImageRenderer::SetRotateRad(float angle)
{
	angle_ = RadianToDegree(angle);
	poly_->SetRotateDeg(angle_);
}

void UIImageRenderer::SetRotateDeg(float angle)
{
	angle_ = angle;
	poly_->SetRotateDeg(angle_);
}

void UIImageRenderer::SetSize(const Vector2<float>& size)
{
	size_ = size;
	poly_->SetSize(size_ * scale_);
}

void UIImageRenderer::SetScale(float scale)
{
	scale_ = scale;
	poly_->SetSize(size_ * scale_);
}

void UIImageRenderer::TextureInvertX(bool flag)
{
	poly_->TextureInvertX(flag);
}

void UIImageRenderer::TextureInvertY(bool flag)
{
	poly_->TextureInvertY(flag);
}

void UIImageRenderer::SetTexture(int handle)
{
	textureHandle_ = handle;
	poly_->SetModeTexture(handle);
}

void UIImageRenderer::Draw(void)
{
	if (!visibility_) { return; }
	poly_->Draw();
}

void UIImageRenderer::Initialize(const std::string& texpath)
{
	auto&& inst = ImageHandleManager::GetInstance();
	inst.Load(texpath);
	textureHandle_ = inst.Get(texpath)[0];

	scale_ = 1.0f;

	poly_ = std::make_unique<Square2DPolygon>();

	poly_->SetPosition(pos_);
	poly_->SetSize(size_);
	poly_->SetModeTexture(textureHandle_);
	poly_->SetDrawPriority(priority_);
	poly_->SetRotateDeg(angle_);
	poly_->SetDrawType(DrawType::UI);
}
