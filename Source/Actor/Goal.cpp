#include "Goal.h"
#include "../Collider/Collider.h"
#include "../Collider/ColliderManager.h"
#include "../Graphic/Animation.h"
#include "../Graphic/BoardPolygon.h"
#include "../Graphic/ImageHandleManager.h"
#include "../Scene/MainScene.h"

namespace
{
	Capsule GoalCapsuleDef = Capsule(Line({ 0.0f, 120.0f }, { 0.0f, 0.0f }), 20.0f);
}

Goal::Goal(const VECTOR& pos, MainScene* scene)
	:ActorBase(pos), scene_(scene)
{
	Initialize();
}

Goal::~Goal()
{
}

void Goal::Update(float delta)
{
	anim_->Update(delta);
}

void Goal::Draw(void)
{
	//unsigned int color = 0xffff00;
	//if (isCleared_) { color = 0xff0000; }
	//DrawCapsule3D(pos_, VAdd(pos_, { GoalCapsuleDef.line_.vec_.x, GoalCapsuleDef.line_.vec_.y, 0.0f }),
	//	GoalCapsuleDef.radius_, 16, color, 0x000000, false);

	poly_->SetTexture(anim_->GetImageHandle());
	poly_->Draw();
}

void Goal::HitEvent(Collider* myCol, Collider* other)
{
	if (other->tag_ == "player_body" && !isCleared_)
	{
		scene_->StageClear();
		isCleared_ = true;
	}
}

void Goal::Initialize(void)
{
	Capsule c = GoalCapsuleDef;
	c.line_.start_ = { pos_.x, pos_.y };
	CapsuleCollider* col = new CapsuleCollider(c);
	col->owner_ = this;
	col->tag_ = "goal";

	ColliderMngIns.AddCollider(col);
	colliders_.push_back(col);

	anim_ = std::make_unique<Animation>("Resource/Animation/FlagAnim.xml");

	size_ = 80.0f;

	poly_->SetScale({ size_, size_ });
	poly_->SetPosition(Vector2<float>{ pos_.x, pos_.y + size_ * 0.5f });
}
