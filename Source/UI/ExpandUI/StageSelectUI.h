#pragma once
#include "../UIIntegrator.h"

class StageSelectScene;
class StageSelectUI :
    public UIIntegrator
{
public:
	StageSelectUI() = delete;
	StageSelectUI(const std::string& filepath, StageSelectScene* scene);
	~StageSelectUI();

	void Update(float delta)override;
	void Draw(void)override;

	enum class State
	{
		NONE,
		GO_TITLE,
		GO_GAME,
	};

	State GetState(void) { return state_; }
	int GetCursorPos(void) { return cursorpos_; }

	void SetCursorPos(int pos);
private:
	void Initialize(void);
	void MakeButtons(void);

	void MoveCursor(void);
	void Select(void);

	void ButtonUpdate(int prev, int next);
	void ButtonScaleUpdate(float delta);

	StageSelectScene* scene_;

	std::vector<UIImageRenderer*> select_;
	std::vector<float> buttonScale_;

	int cursorpos_;
	int count_x;
	int count_y;

	int selectGHandle_;
	int notSelectGHandle_;

	State state_;

	rapidxml::xml_document<> stagedoc_;
};

