#pragma once
#include "ActorBase.h"
#include <array>
#include "Item/ItemType.h"
#include "../../RapidXML/rapidxml.hpp"

struct ItemDropData
{
	int dropmin_ = 0;
	int dropmax_ = 0;

	float prob_ = 0.0f;
};

class Player;
class ActorManager;
class Enemy :
    public ActorBase
{
public:
	Enemy() = delete;
	Enemy(Player* ref, b2World* world, ActorManager* actorMRef);
	Enemy(const VECTOR& pos, Player* ref, b2World* world, ActorManager* actorMRef);

	Enemy(Enemy&&) = default;

	virtual ~Enemy();

	virtual void Update(float delta)override;
	virtual void Draw(void)override;

	virtual void HitEvent(Collider* myCol, Collider* other)override;
	virtual void ApplyVelocity(void)override;
	virtual void ApplyDamage(int damage)override;

	bool IsDead(void) { return isDead_; }

	void LoadDropData(rapidxml::xml_node<>* node);

	void DropItem(void);

	void SetInGame(bool flag) { inGame_ = flag; }
protected:
	void Initialize(void);

	void SetDropData(ItemType type, const ItemDropData& data);

	int hp_;
	bool isDead_ = false;

	bool inGame_ = true;

	Player* playerRef_;

	ActorManager* actorMng_;

	std::array<ItemDropData, static_cast<size_t>(ItemType::Max)> drop_;

	std::array<std::function<void(const Vector2<float>&, float)>, static_cast<size_t>(ItemType::Max)> dropFunc_;
};

