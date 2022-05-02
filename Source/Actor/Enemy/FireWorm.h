#pragma once
#include "../Enemy.h"

class ActorManager;
class FireWorm :
    public Enemy
{
public:
	FireWorm() = delete;
	FireWorm(Player* ref, b2World* world, ActorManager* actorMRef);
	FireWorm(const VECTOR& pos, Player* ref, b2World* world, ActorManager* actorMRef);
	~FireWorm();

	void Update(float delta)override;
	void Draw(void)override;

	void HitEvent(Collider* myCol, Collider* other)override;
	void ApplyVelocity(void)override;
	void ApplyDamage(int damage)override;

	void SetMoveEnable(bool flag)override;
	bool CheckPlayerInSight(float delta);
private:
	void Initialize(void);

	void (FireWorm::*update_)(float delta);
	void IdleUpdate(float delta);
	void AttackStartUpdate(float delta);
	void AttackUpdate(float delta);
	void HitUpdate(float delta);
	void DeadUpdate(float delta);


	float time_;
};

