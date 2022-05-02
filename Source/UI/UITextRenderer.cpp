#include "UITextRenderer.h"
#include <EffekseerForDXLib.h>
#include "../Graphic/Drawer.h"
#include "../Graphic/FontHandleManager.h"

UITextRenderer::UITextRenderer(const Vector2<float>& pos, const std::string& font,
	int size, int thickness, const std::string& text, int red, int green, int blue,
	const std::string& id, int priority)
	: size_(size), text_(text), id_(id), priority_(priority)
{
	pos_ = pos;
	Initialize(font, size, thickness, red, green, blue);
}

UITextRenderer::UITextRenderer(const Vector2<float>& pos, const std::string& font, 
	const std::string& filepath, int size, int thickness, const std::string& text, 
	int red, int green, int blue, const std::string& id, int priority)
	: size_(size), text_(text), id_(id), priority_(priority)
{
	pos_ = pos;
	Initialize(font, filepath, size, thickness, red, green, blue);
}

UITextRenderer::~UITextRenderer()
{
	FontMngIns.DeleteFont(id_);
}

void UITextRenderer::Draw(void)
{
	if (!visibility_) { return; }
	DrawerIns.Add2DDraw(
		[this]() {
			DrawStringToHandle(static_cast<int>(pos_.x), static_cast<int>(pos_.y), text_.c_str(),
				color_, FontMngIns.Get(id_));
		}, priority_, DrawType::UI);
}

void UITextRenderer::SetColor(unsigned int color)
{
	color_ = color;
}

void UITextRenderer::SetPosition(const Vector2<float>& pos)
{
}

void UITextRenderer::Initialize(const std::string& font, int size, int thickness, int red, int green, int blue)
{
	color_ = (red << 16) + (green << 8) + blue;

	FontMngIns.Load(id_, font, size, thickness);
}

void UITextRenderer::Initialize(const std::string& font, const std::string& filepath, int size, int thickness, int red, int green, int blue)
{
	color_ = (red << 16) + (green << 8) + blue;

	FontMngIns.Load(id_.c_str(), font.c_str(), filepath, size, thickness);

	int w = GetDrawStringWidthToHandle(text_.c_str(), static_cast<unsigned int>(text_.size()), FontMngIns.Get(id_));
	pos_.x = pos_.x - static_cast<float>(w * 0.5f);
	pos_.y = pos_.y - size_ * 0.5f;
}
