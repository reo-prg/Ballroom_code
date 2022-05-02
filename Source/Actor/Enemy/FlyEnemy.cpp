#include "FlyEnemy.h"
#include "../Player.h"
#include "../../Defines/PhysicsDefines.h"
#include "../../Graphic/Animation.h"
#include "../../Graphic/BoardPolygon.h"
#include "../../Graphic/EffectMng.h"
#include "../../Collider/Collider.h"
#include "../../Collider/ColliderManager.h"
#include "../../Math/Random_utility.h"

namespace
{
	constexpr float movedur = 2.0f;
	constexpr float followdur = 2.0f;

	const Vector2<float> DeadColliderLocalOffset = { 0.0f, -30.0f };
}

FlyEnemy::FlyEnemy(Player* ref, b2World* world, ActorManager* actorMRef)
	: Enemy(ref, world, actorMRef)
{
	Initialize();
}

FlyEnemy::FlyEnemy(const VECTOR& pos, Player* ref, b2World* world, ActorManager* actorMRef)
	: Enemy(pos, ref, world, actorMRef)
{
	Initialize();
}

FlyEnemy::~FlyEnemy()
{
}

void FlyEnemy::Update(float delta)
{
	(this->*update_)(delta);
	poly_->SetPosition(pos_);
	CheckDir(delta);
	anim_->Update(delta);
	if (canMove_)
	{
		CheckPlayerInSight(delta);
	}
}

void FlyEnemy::Draw(void)
{
	poly_->SetTexture(anim_->GetImageHandle());
	poly_->Draw();
}

void FlyEnemy::HitEvent(Collider* myCol, Collider* other)
{
	if (other->tag_ == "player_body" && currentState_ != "attack" && currentState_ != "stop")
	{
		other->owner_->ApplyDamage(30);
		bodyObject_->SetEnabled(false);
		ChangeState("attack");
	}
}

void FlyEnemy::ApplyVelocity(void)
{
	Enemy::ApplyVelocity();
	GetCollider("enemy_body")->SetPosition({ pos_.x, pos_.y });
}

void FlyEnemy::ApplyDamage(int damage)
{
	Enemy::ApplyDamage(damage);
	if (isDead_)
	{
		GetCollider("enemy_body")->active_ = false;
		GetCollider("enemy_body")->SetLocalOffset(DeadColliderLocalOffset);
		DeleteB2Body();
		ChangeState("fall");
	}
	else
	{
		GetCollider("enemy_body")->active_ = false;
		ChangeState("hit");
	}
}

void FlyEnemy::SetMoveEnable(bool flag)
{
	ActorBase::SetMoveEnable(flag);
	ChangeState("stop");
}

void FlyEnemy::Initialize(void)
{
	size_ = 50.0f;
	shadowSize_ = 35.0f;

	anim_ = std::make_unique<Animation>("Resource/Animation/FlyEnemyAnim.xml");

	constexpr float polySize = 250.0f;
	poly_->SetScale({ polySize, polySize });
	poly_->SetPosition(pos_);

	CircleCollider* c = new CircleCollider(Circle(size_, { pos_.x, pos_.y }));
	c->owner_ = this;
	c->tag_ = "enemy_body";

	colliders_.push_back(c);
	ColliderMngIns.AddCollider(c);

	SetDirection(Direction::Left);

	update_ = &FlyEnemy::RandomMoveUpdate;

	moveTarget_ = pos_;
	moveVec_ = { 0.0f, 0.0f, 0.0f };

	state_.emplace("random", StateAnimPair{ "idle", [this]() { 
		update_ = &FlyEnemy::RandomMoveUpdate;
		time_ = 0.0f;
		} });

	state_.emplace("stop", StateAnimPair{ "idle", [this]() { 
		update_ = &FlyEnemy::StopUpdate;
		std::uniform_real_distribution<float> dist(1.0f, 2.0f);
		time_ = dist(rand_util::GetSeed());
		} });

	state_.emplace("follow", StateAnimPair{ "idle", [this]() { 
		update_ = &FlyEnemy::FollowPlayerUpdate; 
		time_ = 0.0f;
		} });
	
	
	state_.emplace("attack", StateAnimPair{ "attack", [this]() { update_ = &FlyEnemy::AttackUpdate; } });
	state_.emplace("hit", StateAnimPair{ "hit", [this]() { update_ = &FlyEnemy::HitUpdate; } });
	state_.emplace("fall", StateAnimPair{ "death_fall", [this]() { 
		update_ = &FlyEnemy::FallUpdate; 
		currentFallSpeed_ = 0.0f;
		} });
	state_.emplace("dead", StateAnimPair{ "death", [this]() { update_ = &FlyEnemy::DeadUpdate; } });

	ChangeState("random");

	AddCircleBody(size_ / 3.0f, "enemy");

	isHitFloor_ = false;

	hp_ = 2;
}

void FlyEnemy::RandomMoveUpdate(float delta)
{
	constexpr VECTOR forwardvec = { 300.0f, 0.0f, 0.0f };

	if (isinSight_)
	{
		ChangeState("follow");
		return;
	}

	if (time_ == 0.0f)
	{
		time_ = time_ - movedur;

		std::uniform_real_distribution<float> dist(0.7f, 1.0f);
		float s = dist(rand_util::GetSeed());
		moveVec_ = VScale(forwardvec, dir_ == Direction::Right ? -s : s);
		moveTarget_ = VAdd(moveTarget_, moveVec_);
	}

	time_ += delta;

	if (time_ > movedur)
	{
		ChangeState("stop");
	}

	velocity_ = VScale(moveVec_, delta / movedur);
}

void FlyEnemy::StopUpdate(float delta)
{
	if (canMove_)
	{
		time_ -= delta;
	}
	if (time_ < 0.0f)
	{
		ChangeState("random");
	}
}

void FlyEnemy::FollowPlayerUpdate(float delta)
{
	constexpr float followSpeed = 150.0f;
	time_ += delta;

	if (isinSight_)
	{
		time_ = 0.0f;
	}

	if (time_ > followdur)
	{
		ChangeState("stop");
		return;
	}

	velocity_ = VScale(moveVec_, followSpeed * delta);

	if (VDot(VSub(moveTarget_, pos_), moveVec_) < 0.0f)
	{
		ChangeState("stop");
	}
}

void FlyEnemy::AttackUpdate(float delta)
{
	constexpr float followSpeed = 250.0f;

	if (anim_->GetCurrentAnimationProgress() >= 1.0f)
	{
		bodyObject_->SetEnabled(true);
		ChangeState("stop");
		time_ *= 0.25f;
	}
	if (VDot(VSub(moveTarget_, pos_), moveVec_) < 0.0f)
	{
		return;
	}
	velocity_ = VScale(moveVec_, followSpeed * delta);
}

void FlyEnemy::HitUpdate(float delta)
{
	if (anim_->GetCurrentAnimationProgress() >= 1.0f)
	{
		GetCollider("enemy_body")->active_ = true;
		ChangeState("stop");
	}
}

void FlyEnemy::FallUpdate(float delta)
{
	if (isHitFloor_)
	{
		EffectIns.PlayEffect("fall_smoke", { pos_.x, pos_.y, pos_.z - 1.0f }, 0.0f, 5.0f, 0.6f);
		ChangeState("dead");
	}
	float nextspeed = currentFallSpeed_ + phys_def::gravityAcc * delta;
	nextspeed = std::min(nextspeed, 980.0f);
	velocity_.y = (nextspeed + currentFallSpeed_) * 0.5f * delta;

	currentFallSpeed_ = nextspeed;
}

void FlyEnemy::DeadUpdate(float delta)
{
	if (anim_->GetCurrentAnimationProgress() >= 1.0f)
	{
		isDeleted_ = true;
	}
}

void FlyEnemy::CheckDir(float delta)
{
	if (velocity_.x != 0.0f)
	{
		SetDirection(velocity_.x >= 0.0f ? Direction::Right : Direction::Left);
	}
}

void FlyEnemy::CheckPlayerInSight(float delta)
{
	VECTOR pPos = playerRef_->GetPosition();
	VECTOR pVec = VSub(pPos, pos_);
	if (VSquareSize(pVec) > sightLength_ * sightLength_)
	{
		isinSight_ = false;
		return;
	}

	if (VDot(pVec, moveVec_) < 0.0f)
	{
		isinSight_ = false;
		return;
	}

	moveTarget_ = pPos;
	VECTOR a = VSub(moveTarget_, pos_);
	moveVec_ = VNorm(a);
	isinSight_ = true;
}
