#include "WalkEnemy.h"
#include "../../Collider/Collider.h"
#include "../../Collider/ColliderManager.h"
#include "../../Graphic/Animation.h"
#include "../../Graphic/BoardPolygon.h"
#include "../../Defines/PhysicsDefines.h"
#include "../../Application.h"
#include "../../Utility/utility.h"

WalkEnemy::WalkEnemy(Player* ref, b2World* world, ActorManager* actorMRef)
	: Enemy(ref, world, actorMRef)
{
	Initialize();
}

WalkEnemy::WalkEnemy(const VECTOR& pos, Player* ref, b2World* world, ActorManager* actorMRef)
	: Enemy(pos, ref, world, actorMRef)
{
	Initialize();
}

WalkEnemy::~WalkEnemy()
{
}

void WalkEnemy::Update(float delta)
{
	(this->*update_)(delta);
	FallUpdate(delta);
	poly_->SetPosition(pos_);
	baseVel_ = velocity_.x;
}

void WalkEnemy::Draw(void)
{
	//MV1SetTextureGraphHandle(model_, 0, anim_->GetImageHandle(), true);
	//MV1DrawModel(model_);

	poly_->SetTexture(anim_->GetImageHandle());
	poly_->Draw();
	
}

void WalkEnemy::HitEvent(Collider* myCol, Collider* other)
{
	if (other->tag_ == "player_body" && anim_->GetCurrentState() != "attack")
	{
		other->owner_->ApplyDamage(40);
		auto&& pPos = other->owner_->GetPosition();
		float dist = pPos.x - pos_.x;
		SetDirection(dist > 0.0f ? Direction::Right : Direction::Left);
		bodyObject_->SetEnabled(false);
		ChangeState("attack");
	}
}

void WalkEnemy::ApplyVelocity(void)
{
	float d = Application::GetRealDeltaTime();
	float s = Application::GetGameSpeed();
	if (anim_->GetCurrentState() == "walk" && velocity_.x / baseVel_ < 0.2f)
	{
		InvDirection();
		time_ = 0.0f;
	}

	Enemy::ApplyVelocity();
	GetCollider("enemy_body")->SetPosition({ pos_.x, pos_.y });
}

void WalkEnemy::ApplyDamage(int damage)
{
	Enemy::ApplyDamage(damage);
	if (IsDead())
	{
		DeleteB2Body();
		ChangeState("dead");
		GetCollider("enemy_body")->active_ = false;
	}
	else
	{
		ChangeState("hit");
		GetCollider("enemy_body")->active_ = false;
	}
}

void WalkEnemy::SetMoveEnable(bool flag)
{
	ActorBase::SetMoveEnable(flag);

	if (!canMove_)
	{
		ChangeState("idle");
	}
}

void WalkEnemy::Initialize(void)
{
	size_ = 60.0f;
	shadowSize_ = 30.0f;

	anim_ = std::make_unique<Animation>("Resource/Animation/WalkEnemyAnim.xml");
	anim_->ChangeState("walk");	

	poly_->SetScale({ size_, size_ });
	poly_->SetPosition(pos_);

	constexpr float sizescale = 0.6f;

	CircleCollider* c = new CircleCollider(Circle(size_ * sizescale, { pos_.x, pos_.y }),
		true, { 0.0f, -size_ * (1.0f - sizescale) });
	c->owner_ = this;
	c->tag_ = "enemy_body";

	colliders_.push_back(c);
	ColliderMngIns.AddCollider(c);

	SetDirection(Direction::Left);

	AddCircleBody(size_ * sizescale, "enemy", { 0.0f, -size_ * (1.0f - sizescale) });

	state_.emplace("idle", StateAnimPair{ "idle", [this]() { update_ = &WalkEnemy::IdleUpdate; } });
	state_.emplace("walk", StateAnimPair{ "walk", [this]() { update_ = &WalkEnemy::WalkUpdate; } });
	state_.emplace("attack", StateAnimPair{ "attack", [this]() { update_ = &WalkEnemy::AttackUpdate; } });
	state_.emplace("hit", StateAnimPair{ "hit",[this]() { update_ = &WalkEnemy::HitUpdate; } });
	state_.emplace("dead", StateAnimPair{ "hit", [this]() { 
		update_ = &WalkEnemy::DeadUpdate;
		time_ = 0.0f;
		} });


	ChangeState("walk");

	hp_ = 1;
}

void WalkEnemy::MoveUpdate(float delta)
{
	time_ += delta;

	constexpr float speed = 300.0f;
	float move = 0.0f;

	if (dir_ == Direction::Left)
	{
		move = -speed * delta;
	}
	else
	{
		move = speed * delta;
	}

	velocity_.x = move;

	if (time_ > movedur_)
	{
		InvDirection();
		time_ = 0.0f;
	}
}

void WalkEnemy::FallUpdate(float delta)
{
	float nextspeed = currentFallSpeed_ + phys_def::gravityAcc * delta;
	nextspeed = std::min(nextspeed, 980.0f);
	velocity_.y = (nextspeed + currentFallSpeed_) * 0.5f * delta;

	currentFallSpeed_ = nextspeed;
}

void WalkEnemy::IdleUpdate(float delta)
{
	anim_->Update(delta);

}

void WalkEnemy::WalkUpdate(float delta)
{
	if (canMove_)
	{
		MoveUpdate(delta);
	}

	anim_->Update(delta);
}

void WalkEnemy::AttackUpdate(float delta)
{
	anim_->Update(delta);
	if (anim_->GetCurrentAnimationProgress() >= 1.0f)
	{
		bodyObject_->SetEnabled(true);
		ChangeState("walk");
	}
}

void WalkEnemy::HitUpdate(float delta)
{
	anim_->Update(delta);
	if (anim_->GetCurrentAnimationProgress() >= 1.0f)
	{
		GetCollider("enemy_body")->active_ = true;
		ChangeState("walk");
	}
}

void WalkEnemy::DeadUpdate(float delta)
{
	anim_->Update(delta);
	if (anim_->GetCurrentAnimationProgress() >= 1.0f)
	{
		isDeleted_ = true;
	}
}
