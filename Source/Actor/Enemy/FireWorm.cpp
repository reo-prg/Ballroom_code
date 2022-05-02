#include "FireWorm.h"
#include "../Player.h"
#include "../ActorManager.h"
#include "../Projectile/FireBall.h"
#include "../../Defines/PhysicsDefines.h"
#include "../../Graphic/Animation.h"
#include "../../Graphic/BoardPolygon.h"
#include "../../Graphic/EffectMng.h"
#include "../../Collider/Collider.h"
#include "../../Collider/ColliderManager.h"
#include "../../Math/Random_utility.h"

FireWorm::FireWorm(Player* ref, b2World* world, ActorManager* actorMRef)
	: Enemy(ref, world, actorMRef)
{
	Initialize();
}

FireWorm::FireWorm(const VECTOR& pos, Player* ref, b2World* world, ActorManager* actorMRef)
	: Enemy(pos, ref, world, actorMRef)
{
	Initialize();
}

FireWorm::~FireWorm()
{
}

void FireWorm::Update(float delta)
{
	(this->*update_)(delta);
	poly_->SetPosition(pos_);
	anim_->Update(delta);

	OutputDebugStringA(anim_->GetCurrentState().c_str());
}

void FireWorm::Draw(void)
{
	poly_->SetTexture(anim_->GetImageHandle());
	poly_->Draw();
	//DrawSphere3D(pos_, size_, 16, 0xff0000, 0x000000, false);
}

void FireWorm::HitEvent(Collider* myCol, Collider* other)
{
}

void FireWorm::ApplyVelocity(void)
{
}

void FireWorm::ApplyDamage(int damage)
{
	Enemy::ApplyDamage(damage);
	if (isDead_)
	{
		GetCollider("enemy_body")->active_ = false;
		ChangeState("dead");
		DeleteB2Body();
	}
	else
	{
		GetCollider("enemy_body")->active_ = false;
		ChangeState("hit");
	}
}

void FireWorm::SetMoveEnable(bool flag)
{
	ActorBase::SetMoveEnable(flag);
}

bool FireWorm::CheckPlayerInSight(float delta)
{
	auto&& pPos = playerRef_->GetPosition();

	return false;
}

void FireWorm::Initialize(void)
{
	size_ = 60.0f;
	shadowSize_ = 60.0f;

	anim_ = std::make_unique<Animation>("Resource/Animation/WormEnemyAnim.xml");

	constexpr float polySize = 180.0f;
	poly_->SetScale({ polySize, polySize });
	poly_->SetPosition(pos_);

	CircleCollider* c = new CircleCollider(Circle(size_, { pos_.x, pos_.y }));
	c->owner_ = this;
	c->tag_ = "enemy_body";

	colliders_.push_back(c);
	ColliderMngIns.AddCollider(c);

	SetDirection(Direction::Left);

	update_ = &FireWorm::IdleUpdate;

	state_.emplace("idle", StateAnimPair{ "idle", [this]() { time_ = 0.0f; update_ = &FireWorm::IdleUpdate; } });
	state_.emplace("attack_start", StateAnimPair{ "attack_b", [this]() { update_ = &FireWorm::AttackStartUpdate; } });
	state_.emplace("attack", StateAnimPair{ "attack", [this]() { update_ = &FireWorm::AttackUpdate; } });
	state_.emplace("hit", StateAnimPair{ "hit", [this]() { update_ = &FireWorm::HitUpdate; } });
	state_.emplace("dead", StateAnimPair{ "dead", [this]() { update_ = &FireWorm::DeadUpdate; } });

	ChangeState("idle");

	AddCircleBody(size_, "enemy");

	isHitFloor_ = true;

	hp_ = 4;
}

void FireWorm::IdleUpdate(float delta)
{
	time_ += delta;

	if (time_ > 2.0f)
	{
		ChangeState("attack_start");
	}
}

void FireWorm::AttackStartUpdate(float delta)
{
	//OutputDebugStringA(std::to_string(anim_->GetCurrentAnimationProgress()).c_str());
	if (anim_->GetCurrentAnimationProgress() >= 1.0f)
	{
		auto&& pPos = playerRef_->GetPosition();

		actorMng_->Add(new FireBall(pos_, world_, Vector2<float>(pPos.x - pos_.x, pPos.y - pos_.y).NormalizedVec() * 5.0f));
		ChangeState("attack");
	}
}

void FireWorm::AttackUpdate(float delta)
{
	if (anim_->GetCurrentAnimationProgress() >= 1.0f)
	{
		ChangeState("idle");
	}
}

void FireWorm::HitUpdate(float delta)
{
	if (anim_->GetCurrentAnimationProgress() >= 1.0f)
	{
		GetCollider("enemy_body")->active_ = true;
		ChangeState("idle");
	}
}

void FireWorm::DeadUpdate(float delta)
{
	if (anim_->GetCurrentAnimationProgress() >= 1.0f)
	{
		isDeleted_ = true;
	}
}
