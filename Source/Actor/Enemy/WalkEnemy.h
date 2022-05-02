#pragma once
#include "../Enemy.h"

class WalkEnemy :
    public Enemy
{
public:
	WalkEnemy() = delete;
	WalkEnemy(Player* ref, b2World* world, ActorManager* actorMRef);
	WalkEnemy(const VECTOR& pos, Player* ref, b2World* world, ActorManager* actorMRef);
	~WalkEnemy();

	void Update(float delta)override;
	void Draw(void)override;

	void HitEvent(Collider* myCol, Collider* other)override;
	void ApplyVelocity(void)override;
	void ApplyDamage(int damage)override;

	void SetMoveEnable(bool flag)override;
private:
	void Initialize(void);

	void MoveUpdate(float delta);
	void FallUpdate(float delta);

	void IdleUpdate(float delta);
	void WalkUpdate(float delta);
	void AttackUpdate(float delta);
	void HitUpdate(float delta);
	void DeadUpdate(float delta);
	void (WalkEnemy::*update_)(float delta);

	float movedur_ = 5.0f;
	float time_ = 0.0f;

	float baseVel_ = 0.0f;
};

