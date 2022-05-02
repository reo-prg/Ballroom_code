#include "Enemy.h"
#include <algorithm>
#include "../Collider/Collider.h"
#include "../Collider/ColliderManager.h"
#include "../Graphic/Animation.h"
#include "../Graphic/EffectMng.h"
#include "../../RapidXML/rapidxml_utils.hpp"
#include "../MyRapidXML_Helper.h"
#include "../Math/MathUtility.h"
#include "../Math/Random_utility.h"

#include "Item/HealCrystal.h"
#include "ActorManager.h"

Enemy::Enemy(Player* ref, b2World* world, ActorManager* actorMRef)
	: playerRef_(ref), ActorBase(VECTOR{ 0.0f, 0.0f }, world), actorMng_(actorMRef)
{
	Initialize();
}

Enemy::Enemy(const VECTOR& pos, Player* ref, b2World* world, ActorManager* actorMRef)
	: playerRef_(ref), ActorBase(pos, world), actorMng_(actorMRef)
{
	pos_ = pos;
	Initialize();
}

Enemy::~Enemy()
{
	if (isDead_ && inGame_)
	{
		DropItem();

		//EffectIns.PlayEffect("defeat_smoke", Vector3<float>{ pos_.x, pos_.y, pos_.z - 1.0f }, 
		//	0.0f, 1.0f, 0.3f);
	}
}

void Enemy::Update(float delta)
{
}

void Enemy::Draw(void)
{
}

void Enemy::HitEvent(Collider* myCol, Collider* other)
{
}

void Enemy::ApplyVelocity(void)
{
	ActorBase::ApplyVelocity();
	velocity_ = { 0.0f, 0.0f, 0.0f };
}

void Enemy::ApplyDamage(int damage)
{
	hp_ -= damage;
	if (hp_ <= 0)
	{
		isDead_ = true;
	}
}

void Enemy::SetDropData(ItemType type, const ItemDropData& data)
{
	int idx = static_cast<size_t>(type);
	if (idx > static_cast<size_t>(ItemType::Max)) { return; }

	drop_[idx] = data;
}

void Enemy::LoadDropData(rapidxml::xml_node<>* node)
{
	using namespace rapidxml;

	for (auto&& data = node->first_node("data"); data != nullptr; data = data->next_sibling())
	{
		std::string type = GetAttributeValue(data->first_attribute("type"));
		int dmin = atoi(GetAttributeValue(data->first_attribute("min")));
		int dmax = atoi(GetAttributeValue(data->first_attribute("max")));
		float prob = static_cast<float>(atof(GetAttributeValue(data->first_attribute("prob"))));

		auto&& it = std::find(ItemNames.begin(), ItemNames.end(), type);

		SetDropData(static_cast<ItemType>(it - ItemNames.begin()), { dmin, dmax, prob });
	}
}

void Enemy::DropItem(void)
{
	using namespace rand_util;
	//actorMng_->Add(new HealCrystal(pos_, playerRef_, world_, { -200.0f, 400.0f }));
	//actorMng_->Add(new HealCrystal(pos_, playerRef_, world_, { 0.0f, 400.0f }));
	//actorMng_->Add(new HealCrystal(pos_, playerRef_, world_, { 200.0f, 400.0f }));

	std::uniform_real_distribution<float> dist(200.0f, 400.0f);
	std::uniform_real_distribution<float> angle(0.5f, Math_PI - 0.5f);
	std::uniform_real_distribution<float> pr(0.0f, 1.0f);

	std::uniform_real_distribution<float> zd(-5.0f, 5.0f);

	for (int i = 0; i < static_cast<int>(ItemType::Max); i++)
	{
		for (int c = 0; c < drop_[i].dropmax_; c++)
		{
			if (c >= drop_[i].dropmin_)
			{
				if (pr(GetSeed()) > drop_[i].prob_)
				{
					break;
				}
			}
			float d = dist(GetSeed());

			dropFunc_[i](Vector2<float>{ cosf(angle(GetSeed()))* d, sinf(angle(GetSeed()))* d }, zd(GetSeed()));
		}
	}
}

void Enemy::Initialize(void)
{
	dropFunc_[static_cast<size_t>(ItemType::Heal)] =
		[this](const Vector2<float>& vec, float z) 
	{
		auto&& p = pos_;
		p.z += z;
		actorMng_->Add(new HealCrystal(p, playerRef_, world_, vec));
	};

	dropFunc_[static_cast<size_t>(ItemType::Buff)] =
		[this](const Vector2<float>& vec, float z)
	{
	};
}
