#include "HealCrystal.h"
#include "../../Audio/AudioManager.h"
#include "../../Graphic/Animation.h"
#include "../../Graphic/BoardPolygon.h"
#include "../../Collider/Collider.h"
#include "../../Collider/ColliderManager.h"

HealCrystal::HealCrystal(const VECTOR& pos, Player* player, b2World* world, const Vector2<float>& vec)
	: ItemBase(pos, player, world, vec)
{
	Initialize();
}

HealCrystal::~HealCrystal()
{
}

void HealCrystal::Update(float delta)
{
	ItemBase::Update(delta);
	anim_->Update(delta);
	poly_->SetPosition(pos_);
}

void HealCrystal::Draw(void)
{
	if (!Blink()) { return; }
	poly_->SetTexture(anim_->GetImageHandle());
	poly_->Draw();
}
//
//void HealCrystal::HitEvent(Collider* myCol, Collider* other)
//{
//
//}

void HealCrystal::ApplyVelocity(void)
{
	ActorBase::ApplyVelocity();
	GetCollider("body")->SetPosition({ pos_.x, pos_.y });
}

void HealCrystal::ApplyDamage(int damage)
{
}

void HealCrystal::Initialize(void)
{
	anim_ = std::make_unique<Animation>("Resource/Animation/HealCrystalAnim.xml");

	size_ = itemSize_;
	shadowSize_ = size_ * 0.5f;

	poly_->SetScale({ itemSize_, itemSize_ });
	poly_->SetPosition(pos_);

	CircleCollider* c = new CircleCollider(Circle(size_, { pos_.x, pos_.y }), true);
	c->owner_ = this;
	c->tag_ = "body";

	colliders_.push_back(c);
	ColliderMngIns.AddCollider(c);

	//AddCircleBody(itemSize_, "item");

	deleteTime_ = 10.0f;

	type_ = ItemType::Heal;

	getSoundKey_ = "item";
	AudioIns.LoadSound("Resource/Sound/correct.wav", getSoundKey_);
}
