#pragma once
#include "../Math/VectorInterface.h"

class UIBase
{
public:
	UIBase() = default;
	virtual ~UIBase() = default;

	virtual void Update(float delta) {};
	virtual void Draw(void) {};

	void SetVisible(bool flag) { visibility_ = flag; };

	Vector2<float> GetPosition(void) { return pos_; }
protected:

	bool visibility_ = true;

	Vector2<float> pos_;
};

