#pragma once
#include "../UIIntegrator.h"
#include <vector>

class ExitUI :
	public UIIntegrator
{
public:
	ExitUI() = delete;
	ExitUI(const std::string& filepath);
	~ExitUI();

	void Update(float delta)override;
	void Draw(void)override;

	enum class State
	{
		NONE,
		YES,
		NO
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

	State state_ = State::NONE;

	enum class SelectElement
	{
		Yes,
		No
	};

	static constexpr unsigned int selectColor = 0x6fa2fa;
	static constexpr unsigned int idleColor = 0x000000;

};
