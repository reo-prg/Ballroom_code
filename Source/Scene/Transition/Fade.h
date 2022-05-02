#pragma once
#include "Transition.h"

class Fade :
    public Transition
{
public:
	Fade() = delete;
	Fade(const std::shared_ptr<Scene>& prevScene, const std::shared_ptr<Scene>& nextScene,
			float fadeoutDuration, float fadeinDuration);
	~Fade();

	void Update(float delta)override;

	void Draw(void)override;

	void Initialize(void)override;
private:
	void FadeOut(float delta);
	void FadeIn(float delta);
	void (Fade::*update_)(float);

	void PrevDraw(void);
	void NextDraw(void);
	void (Fade::*draw_)(void);

	float timer_;
	float outDur_;
	float inDur_;
	float progress_ = 0.0f;
	
	bool resetTime_ = false;
};

