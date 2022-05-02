#pragma once
#include "ActorBase.h"
#include <memory>

class MainScene;
class Goal :
	public ActorBase
{
public:
	Goal() = delete;
	Goal(const VECTOR& pos, MainScene* scene);
	~Goal();

	void Update(float delta)override;
	void Draw(void);

	void HitEvent(Collider* myCol, Collider* other)override;
private:
	void Initialize(void);

	MainScene* scene_;
	bool isCleared_ = false;
};

