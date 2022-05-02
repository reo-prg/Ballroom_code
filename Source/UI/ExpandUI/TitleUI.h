#pragma once
#include "../UIIntegrator.h"
#include <vector>

class TitleScene;
class TitleUI :
    public UIIntegrator
{
public:
	TitleUI() = delete;
	TitleUI(const std::string& filepath, TitleScene* scene);
	~TitleUI();

	void Update(float delta)override;
	void Draw(void)override;

	enum class State
	{
		NONE,
		GO_SELECT,
		GO_SETTING,
		EXIT
	};

	State GetState(void) { return state_; }
	void Reset(void);
private:
	void Initialize(void);

	void MoveCursor(void);
	void Select(void);
	void MoveToNextScene(void);

	UIImageRenderer* cursor_;
	std::vector<UITextRenderer*> select_;

	float time_;

	int cursorpos_;

	TitleScene* scene_;

	State state_ = State::NONE;

	enum class SelectElement
	{
		Start,
		Setting,
		Exit
	};

	static constexpr unsigned int selectColor = 0x6fa2fa;
	static constexpr unsigned int idleColor = 0x000000;
};

