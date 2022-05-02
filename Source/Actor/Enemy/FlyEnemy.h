#pragma once
#include "../Enemy.h"

class FlyEnemy :
	public Enemy
{
public:
	FlyEnemy() = delete;
	FlyEnemy(Player* ref, b2World* world, ActorManager* actorMRef);
	FlyEnemy(const VECTOR& pos, Player* ref, b2World* world, ActorManager* actorMRef);
	~FlyEnemy();

	void Update(float delta)override;
	void Draw(void)override;

	void HitEvent(Collider* myCol, Collider* other)override;
	void ApplyVelocity(void)override;
	void ApplyDamage(int damage)override;

	void SetMoveEnable(bool flag)override;
private:
	void Initialize(void);

	void (FlyEnemy::* update_)(float delta);
	void RandomMoveUpdate(float delta);
	void StopUpdate(float delta);
	void FollowPlayerUpdate(float delta);
	void AttackUpdate(float delta);
	void HitUpdate(float delta);
	void FallUpdate(float delta);
	void DeadUpdate(float delta);

	void CheckDir(float delta);
	void CheckPlayerInSight(float delta);

	float time_ = 0.0f;

	VECTOR moveTarget_;
	VECTOR moveVec_;
	float moveTime_;

	bool isinSight_ = false;
	static constexpr float sightLength_ = 400.0f;
};

