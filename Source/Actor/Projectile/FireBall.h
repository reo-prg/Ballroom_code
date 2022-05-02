#pragma once
#include "../ActorBase.h"

class FireBall :
    public ActorBase
{
public:
	FireBall(const VECTOR& pos, b2World* world, const Vector2<float>& vec);
	~FireBall();

	void Update(float delta)override;
	void Draw(void)override;

	void HitEvent(Collider* myCol, Collider* other)override;
	void ApplyVelocity(void)override;
	void ApplyDamage(int damage)override;
private:
	void Initialize(void);

	Vector2<float> vec_;
};

