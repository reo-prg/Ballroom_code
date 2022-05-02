#include "ThrowObject.h"
#include <DxLib.h>
#include "../Audio/AudioManager.h"
#include "../Audio/SubmixHandleManager.h"
#include "../Camera/CameraShake.h"
#include "../Collider/Collider.h"
#include "../Collider/ColliderManager.h"
#include "../Defines/Unit_Defines.h"
#include "../Graphic/Drawer.h"
#include "../Graphic/EffekseerEffectData.h"
#include "../Math/MathUtility.h"
#include "../MyB2DHelper.h"
#include "../Application.h"
#include "Player.h"
#include <string>
#include <algorithm>

namespace
{
	ShakeDescriptor hitShake = {
		Vector3<float>{ 10.0f, 10.0f, 0.0f },
		ShakeType::SinWave,
		0.4f,
		5.0f,
		0.0f,
		0.2f
	};
}

ThrowObject::ThrowObject(const VECTOR& pos, float size, b2World* world)
	: size_(size), ActorBase(pos)
{
	Initialize(world);
}

ThrowObject::ThrowObject(const std::string& modelPath, float scale, const VECTOR& rotate, const VECTOR& pos, float size, b2World* world)
	: size_(size), ActorBase(pos)
{
	Initialize(modelPath, scale, rotate, world);
}

ThrowObject::~ThrowObject()
{
	MV1DeleteModel(modelId_);
}

void ThrowObject::Update(float delta)
{
	b2Vec2 bpos = body_->GetPosition();

	pos_.x = bpos.x * unit::m_to_cm;
	pos_.y = bpos.y * unit::m_to_cm;

	GetCollider("throw_area")->SetPosition({ pos_.x, pos_.y });

	if (player_)
	{
		auto&& pPos = player_->GetPosition();

		if (abs(pPos.x - pos_.x) > 600.0f)
		{
			isAttacking_ = false;
			isHit_ = false;
			trailEffect_->Stop();
		}
	}

	if (isAttacking_)
	{
		//trailEffect_->SetPosition({ pos_.x, pos_.y, pos_.z });
		auto&& vel = body_->GetLinearVelocity();
		auto&& ang = atan2(vel.y, vel.x);
		//trailEffect_->SetRotation({ 0.0f, 0.0f, ang });

		velHistory_.Next();
		velHistory_[0] = vel.LengthSquared();
	}
	trailEffect_->SetPosition({ pos_.x, pos_.y, pos_.z });
	auto&& vel = body_->GetLinearVelocity();
	auto&& ang = atan2(vel.y, vel.x);
	trailEffect_->SetRotation({ 0.0f, 0.0f, ang });

	if (modelId_ != -1)
	{
		MV1SetPosition(modelId_, pos_);
		float ang = body_->GetAngle();
		MV1SetRotationXYZ(modelId_, { rotate_.x, rotate_.y, ang });
	}

	//if (time_ > 0.0f)
	//{
	//	time_ -= delta;
	//	if (time_ <= 0.0f)
	//	{
	//		isAttacking_ = false;
	//		trailEffect_->Stop();
	//	}
	//}

	if (isHit_)
	{
		if (velHistory_.Allof([](float val) { return val < 0.5f; }))
		{
			isAttacking_ = false;
			isHit_ = false;
			trailEffect_->Stop();
		}
	}

	//OutputDebugStringA(("x:" + std::to_string(bpos.x) + " y:" + std::to_string(bpos.y) + "\n").c_str());
}

void ThrowObject::Draw(void)
{
	unsigned int color = 0xffff00;
	COLOR_F colorf = GetColorF(1.0f, 1.0f, 1.0f, 1.0f);
	if (isAttacking_) 
	{ 
		color = 0xff0000; 
		colorf = GetColorF(1.0f, 0.0f, 0.0f, 1.0f);
	}

	if (modelId_ == -1)
	{
		DrawSphere3D(pos_, size_, 12, color, 0x000000, true);
	}
	else
	{
		MV1SetMaterialDifColor(modelId_, 0, colorf);
		MV1DrawModel(modelId_);
	}
}

void ThrowObject::SetPosition(const VECTOR& pos)
{
	float angle = body_->GetAngle();
	body_->SetTransform({ pos.x / unit::m_to_cm, pos.y / unit::m_to_cm }, angle);
	pos_ = pos;
}

void ThrowObject::SetPosition(const Vector2<float>& pos)
{
	float angle = body_->GetAngle();
	body_->SetTransform({ pos.x / unit::m_to_cm, pos.y / unit::m_to_cm }, angle);
	pos_.x = pos.x;
	pos_.y = pos.y;
}

void ThrowObject::AddPosition(const Vector2<float>& pos)
{
	float angle = body_->GetAngle();
	pos_.x += pos.x;
	pos_.y += pos.y;
	body_->SetTransform({ pos_.x / unit::m_to_cm, pos_.y / unit::m_to_cm }, angle);
}

void ThrowObject::B2HitEvent(b2Body* mybody, MyUserData* myData,
	b2Body* otherbody, MyUserData* otherData, const b2Vec2& hitNormal)
{
	if (otherData->tag_ == "enemy" && isAttacking_)
	{
		otherData->owner_->ApplyDamage(1);
		/*isAttacking_ = false;*/
		time_ = 2.0f;
		isHit_ = true;
		hitEffect_->Play();
		hitEffect_->SetPosition({ pos_.x, pos_.y, pos_.z });

		Application::SetGameSpeed(0.2f, 0.2f);

		b2Vec2 lVel = body_->GetLinearVelocity();
		hitShake.strength_ = { lVel.x, lVel.y, 0.0f };
		DrawerIns.StartCameraShake(hitShake);

		int hitsound = AudioIns.PlayTemporary("hit", 1.0f, { SubmixHandleMng.GetHandle("se") });
	}
	if (otherData->tag_ == "map")
	{
		if (b2Dot(hitNormal, { 0.0f, 1.0f }) >= sinf(DegreeToRadian(45.0f)))
		{
			//isAttacking_ = false;
			if (time_ == 0.0f)
			{
				time_ = 1.0f;
			}
			else
			{
				time_ *= 0.5f;
			}
			isHit_ = true;

		}
	}
	if (otherData->tag_ == "throw")
	{
		if (b2Dot(hitNormal, { 0.0f, 1.0f }) >= sinf(DegreeToRadian(45.0f)))
		{
			//isAttacking_ = false;
			time_ = 1.0f;
			isHit_ = true;
		}
	}
	if (otherData->tag_ == "player" && canHeading_)
	{
		if (b2Dot(hitNormal, { 0.0f, 1.0f }) >= sinf(DegreeToRadian(45.0f)))
		{
			b2Vec2 vel = hitNormal;
			vel *= 8.0f;
			Throw(vel);
			time_ = 0.0f;
			isHit_ = false;
		}
	}
}

void ThrowObject::Throw(const b2Vec2& velocity_, bool attack)
{
	if (!canThrow_) { return; }

	body_->SetLinearVelocity(velocity_);
	isAttacking_ = attack;
	velHistory_.Fill(velocity_.LengthSquared());
	time_ = 0.0f;
	trailEffect_->Play();
	trailEffect_->SetPosition({ pos_.x, pos_.y, pos_.z });
}

void ThrowObject::SetPhysicsEnable(bool flag)
{
	body_->SetEnabled(flag);
}

void ThrowObject::SetVelocity(VECTOR vel)
{
	velocity_ = vel;
	body_->SetLinearVelocity({ vel.x / unit::m_to_cm, vel.x / unit::m_to_cm });
}

void ThrowObject::StopEffect(void)
{
	trailEffect_->Stop();
}

void ThrowObject::SetPlayer(Player* player)
{
	player_ = player;
}

void ThrowObject::Initialize(b2World* world)
{
	shadowSize_ = size_;

	b2BodyDef bDef;
	bDef.position.Set(pos_.x / unit::m_to_cm, pos_.y / unit::m_to_cm);
	bDef.type = b2_dynamicBody;
	bDef.allowSleep = false;
	bDef.angularDamping = 3.0f;

	MyUserData* data = new MyUserData();
	data->owner_ = dynamic_cast<ActorBase*>(this);
	data->tag_ = "throw";
	SetBodyData(&bDef, data);

	body_ = world->CreateBody(&bDef);

	b2CircleShape bc;
	bc.m_p.Set(0.0f, 0.0f);
	bc.m_radius = size_ / unit::m_to_cm;
	

	b2FixtureDef fdef;
	fdef.shape = &bc;
	fdef.density = 0.3f;
	fdef.friction = 0.6f;
	fdef.restitution = 0.5f;

	body_->CreateFixture(&fdef);

	CircleCollider* col = new CircleCollider(Circle(size_ + 30.0f));
	col->owner_ = this;
	col->tag_ = "throw_area";

	ColliderMngIns.AddCollider(col);
	colliders_.push_back(col);

	hitEffect_ = std::make_unique<EffekseerEffectData>("Resource/Effect/Hit/Hit.efk", 100.0f);
	trailEffect_ = std::make_unique<EffekseerEffectData>("Resource/Effect/Trail/Ball_Trail.efk", 10.0f);
}

void ThrowObject::Initialize(const std::string& modelPath, float scale, const VECTOR& rotate, b2World* world)
{
	Initialize(world);

	modelId_ = MV1LoadModel(modelPath.c_str());
	MV1SetScale(modelId_, { scale, scale, scale });
	MV1SetPosition(modelId_, pos_);
	MV1SetRotationXYZ(modelId_, rotate);
	rotate_ = rotate;
}
