#include "ItemBase.h"
#include "../../Audio/AudioManager.h"
#include "../../Audio/SubmixHandleManager.h"
#include "../../Collider/Collider.h"
#include "../../Collider/ColliderManager.h"
#include "../../Defines/PhysicsDefines.h"
#include "../../Utility/utility.h"
#include "../Player.h"

namespace
{
	constexpr float BlinkDuration = 3.0f;
	constexpr float BlinkInterval = 0.125f;
}

ItemBase::ItemBase(const VECTOR& pos, Player* player, b2World* world, const Vector2<float>& vec)
	: ActorBase(pos, world), playerRef_(player), deleteCounter_(0.0f)
{
	//currentMoveSpeed_ = abs(vec.x);
	//moveDir_ = sign<float>(vec.x);
	currentMoveSpeed_ = vec.x;
	currentFallSpeed_ = vec.y;
}

ItemBase::~ItemBase()
{
}

void ItemBase::Update(float delta)
{
	if (isMoving_)
	{
		FallUpdate(delta);
		MoveUpdate(delta);
	}
	else
	{
		velocity_ = { 0.0f, 0.0f };
	}
	DeleteUpdate(delta);
}

void ItemBase::Draw(void)
{
}

void ItemBase::HitEvent(Collider* myCol, Collider* other)
{
	if (other->tag_ == "player_body")
	{
		playerRef_->AddItemCount(type_);
		PlayGetSound();
		isDeleted_ = true;
	}
}

void ItemBase::ApplyVelocity(void)
{
	ActorBase::ApplyVelocity();
}

void ItemBase::ApplyDamage(int damage)
{
}

void ItemBase::ResetFallSpeed(void)
{
	currentFallSpeed_ = currentFallSpeed_ * -0.3f;
	currentMoveSpeed_ = currentMoveSpeed_ * 0.3f;
	if (abs(currentMoveSpeed_) < 10.0f)
	{
		currentFallSpeed_ = 0.0f;
		currentMoveSpeed_ = 0.0f;
		velocity_.x = 0.0f;
		velocity_.y = 0.0f;
		isCheckMapCollision_ = false;
		isMoving_ = false;
	}
}

void ItemBase::FallUpdate(float delta)
{
	float nextspeed = currentFallSpeed_ + phys_def::gravityAcc * delta;
	nextspeed = std::min(nextspeed, 980.0f);
	velocity_.y = (nextspeed + currentFallSpeed_) * 0.5f * delta;

	currentFallSpeed_ = nextspeed;
}

void ItemBase::MoveUpdate(float delta)
{
	//float nextspeed = currentMoveSpeed_ - friction * delta;
	//nextspeed = std::max(nextspeed, 0.0f);
	//velocity_.x = (nextspeed + currentMoveSpeed_) * 0.5f * delta * moveDir_;

	//currentMoveSpeed_ = nextspeed;

	velocity_.x = currentMoveSpeed_ * delta;
}

void ItemBase::DeleteUpdate(float delta)
{
	deleteCounter_ += delta;

	if (deleteCounter_ > deleteTime_)
	{
		isDeleted_ = true;
	}
}

bool ItemBase::Blink(void)
{
	float v = deleteTime_ - BlinkDuration;
	if (v > deleteCounter_)
	{
		return true;
	}

	float b = std::fmodf(deleteCounter_ - v, BlinkInterval);
	return b > (BlinkInterval * 0.5f);
}

void ItemBase::PlayGetSound(void)
{
	if (getSoundKey_.empty()) { return; }

	AudioIns.PlayTemporary(getSoundKey_, 1.0f, { SubmixHandleMng.GetHandle("se") });
}
