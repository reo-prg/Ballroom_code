#pragma once
#include "ItemBase.h"

class HealCrystal :
    public ItemBase
{
public:
	HealCrystal() = delete;
	HealCrystal(const VECTOR& pos, Player* player, b2World* world, const Vector2<float>& vec = Vector2<float>::ZeroVector());

	HealCrystal(HealCrystal&&) = default;

	virtual ~HealCrystal();

	virtual void Update(float delta)override;
	virtual void Draw(void)override;

	//virtual void HitEvent(Collider * myCol, Collider * other)override;
	virtual void ApplyVelocity(void)override;
	virtual void ApplyDamage(int damage)override;

private:
	void Initialize(void);


};

