#pragma once
#include "../ActorBase.h"
#include "ItemType.h"

class Player;
class ItemBase :
    public ActorBase
{
public:
	ItemBase() = delete;
	ItemBase(const VECTOR& pos, Player* player, b2World* world, const Vector2<float>& vec);

	ItemBase(ItemBase&&) = default;

	virtual ~ItemBase();

	virtual void Update(float delta)override;
	virtual void Draw(void)override;

	virtual void HitEvent(Collider * myCol, Collider * other)override;
	virtual void ApplyVelocity(void)override;
	virtual void ApplyDamage(int damage)override;

	ItemType GetType(void) { return type_; }

	void ResetFallSpeed(void)override;
protected:
	void FallUpdate(float delta);
	void MoveUpdate(float delta);
	void DeleteUpdate(float delta);

	bool Blink(void);

	void PlayGetSound(void);

	Player* playerRef_ = nullptr;

	float currentMoveSpeed_;
	float moveDir_;

	float deleteCounter_;
	float deleteTime_;

	bool isMoving_ = true;

	ItemType type_;

	std::string getSoundKey_;

	static constexpr float itemSize_ = 32.0f;
	static constexpr float friction = 100.0f;
};

