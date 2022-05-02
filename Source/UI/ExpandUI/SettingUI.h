#pragma once
#include "../UIIntegrator.h"

class SettingScene;
class SettingUI :
    public UIIntegrator
{
public:
	SettingUI() = delete;
	SettingUI(const std::string& filepath, SettingScene* scene);
	~SettingUI();

	void Update(float delta)override;
	void Draw(void)override;

	enum class State
	{
		NONE,
		BACK,
	};

	State GetState(void) { return state_; }
private:
	void Initialize(void);

	void MoveCursor(void);
	void MoveBar(float delta);
	void Select(void);
	void MoveToNextScene(void);

	SettingScene* scene_;

	UIImageRenderer* cursor_;
	std::vector<UIProgressBar*> select_;

	State state_;

	float time_;

	int cursorpos_;
};

