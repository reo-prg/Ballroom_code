#pragma once
#include "UIBase.h"
#include <string>
#include "../math/VectorInterface.h"

class UIProgressBar :
    public UIBase
{
public:
	UIProgressBar() = delete;
	UIProgressBar(const Vector2<float>& pos, const std::string& basePath, const std::string& progressPath,
		const std::string& id, int priority);
	UIProgressBar(const Vector2<float>& pos, const std::string& basePath, const std::string& progressPath, const std::string& divPath,
		const std::string& id, int priority);
	~UIProgressBar();

	void Draw(void)override;

	void AddProgress(float add);
	float GetProgress(void) { return progress_; }
	void SetProgress(float prog);

	float GetCenterPositionY(void) { return pos_.y; }

	void SetProgressHandle(int handle) { progressHandle_ = handle; }
private:
	void Initialize(const std::string& basePath, const std::string& progressPath);
	void Initialize(const std::string& basePath, const std::string& progressPath, const std::string& divPath);

	int baseHandle_ = -1;
	int progressHandle_ = -1;
	int divHandle_ = -1;

	std::string id_;

	int priority_;
	float scale_;

	int barWidth_;
	int barHeight_;

	float progress_ = 0.6f;
};

