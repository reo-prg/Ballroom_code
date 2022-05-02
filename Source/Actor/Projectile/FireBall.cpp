#include "FireBall.h"
#include "../../Graphic/Animation.h"
#include "../../Graphic/BoardPolygon.h"
#include "../../Graphic/EffectMng.h"
#include "../../Collider/Collider.h"
#include "../../Collider/ColliderManager.h"

FireBall::FireBall(const VECTOR& pos, b2World* world, const Vector2<float>& vec)
	: vec_(vec)
{
	Initialize();
}

FireBall::~FireBall()
{
}

void FireBall::Update(float delta)
{
	if (isHitMap_)
	{
		isDeleted_ = true;
		return;
	}
	anim_->Update(delta);

	velocity_.x = vec_.x * delta;
	velocity_.y = vec_.y * delta;
}

void FireBall::Draw(void)
{
	poly_->SetTexture(anim_->GetImageHandle());
	poly_->Draw();
}

void FireBall::HitEvent(Collider* myCol, Collider* other)
{
}

void FireBall::ApplyVelocity(void)
{
	ActorBase::ApplyVelocity();

	if (!colliders_.empty())
	{
		colliders_[0]->SetPosition({ pos_.x, pos_.y });
	}
}

void FireBall::ApplyDamage(int damage)
{
}

void FireBall::Initialize(void)
{
	size_ = 40.0f;

	anim_ = std::make_unique<Animation>("Resource/Animation/FireBallAnim.xml");

	constexpr float polySize = 180.0f;
	poly_->SetScale({ polySize, polySize });
	poly_->SetPosition(pos_);
	poly_->SetRotate(atan2(vec_.y, vec_.x));

	CircleCollider* c = new CircleCollider(Circle(size_, { pos_.x, pos_.y }));
	c->owner_ = this;
	c->tag_ = "body";

	colliders_.push_back(c);
	ColliderMngIns.AddCollider(c);

	//SetDirection(Direction::Left);

	AddCircleBody(size_, "collision");

	isHitFloor_ = false;
}
