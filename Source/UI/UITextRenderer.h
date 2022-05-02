#pragma once
#include "UIBase.h"
#include <string>
#include "../math/VectorInterface.h"

class UITextRenderer :
    public UIBase
{
public:
	UITextRenderer() = delete;
	UITextRenderer(const Vector2<float>& pos, const std::string& font, int size, int thickness,
		const std::string& text, int red, int green, int blue, const std::string& id, int priority);
	UITextRenderer(const Vector2<float>& pos, const std::string& font, const std::string& filepath, int size, int thickness,
		const std::string& text, int red, int green, int blue, const std::string& id, int priority);
	~UITextRenderer();

	void Draw(void)override;

	void SetColor(unsigned int color);

	int GetSize(void) { return size_; }

	void SetPosition(const Vector2<float>& pos);

	float GetCenterPositionY(void) { return pos_.y + size_ * 0.5f; }

	void SetText(const std::string& text) { text_ = text; }
private:
	void Initialize(const std::string& font, int size, int thickness, int red, int green, int blue);
	void Initialize(const std::string& font, const std::string& filepath, int size, int thickness, int red, int green, int blue);

	std::string text_;
	unsigned int color_;
	int priority_;

	int size_;

	std::string id_;
};

