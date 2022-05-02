#include "Player.h"
#include <box2d.h>
#include "../Audio/AudioManager.h"
#include "../Audio/SubmixHandleManager.h"
#include "../Camera/CameraShake.h"
#include "../Collider/Collider.h"
#include "../Collider/ColliderManager.h"
#include "../Graphic/Animation.h"
#include "../Graphic/BoardPolygon.h"
#include "../Graphic/Camera.h"
#include "../Graphic/Drawer.h"
#include "../Graphic/EffectMng.h"
#include "../Graphic/EffekseerEffectData.h"
#include "../Graphic/ImageHandleManager.h"
#include "../Input/InputIntegrator.h"
#include "../Math/MathUtility.h"
#include "../Scene/MainScene.h"
#include "../Defines/PhysicsDefines.h"
#include "../UI/HealthUI.h"
#include "../Utility/utility.h"
#include "../MyB2DHelper.h"
#include "ThrowObject.h"
#include "../DebugModule.h"
#include "../Application.h"

namespace
{
	constexpr float GrabAnimationDuration = 0.5f;

	const Vector2<float> GrabOffsetPosition = { 0.0f, Player::playerSize_ };

	const Vector2<float> GrabAnimTopPositionOffset = { 0.0f, Player::playerSize_ + 30.0f };

	constexpr float GrabObjectGravityScale = 10.0f;

	constexpr float InvincibleDuration = 3.0f;
}

Player::Player(const VECTOR& pos, b2World* world, MainScene* scene)
	: ActorBase(pos, world), scene_(scene)
{
	Initialize();
}

Player::~Player()
{
	delete cameraShake_;
}

void Player::Update(float delta)
{
	InvincibleTimerUpdate(delta);
	(this->*update_)(delta);
	(this->*grabUpdate_)(delta);

	poly_->SetPosition(pos_);
	CheckDir(delta);
	anim_->Update(delta);

	healthUI_->Update(delta);
}

void Player::Draw(void)
{
	if (invincibleTimer_ == 0.0f || fmodf(invincibleTimer_, 0.4f) > 0.2f)
	{
		poly_->SetTexture(anim_->GetImageHandle());
		poly_->Draw();
	}

	healthUI_->Draw();

	if (DebugModule::IsDebugEnable())
	{
		DrawerIns.Add2DDraw([this]() {
			DrawString(10, 120, ("x:" + std::to_string(pos_.x) + "  y:" + std::to_string(pos_.y)).c_str(), 0xffffff);
			}, 1000);
	}
}

void Player::SetCamera(Camera* camera)
{
	camera_ = camera;

	auto cPos = camera_->GetCameraPosition();

	camera_->SetCameraPosition(pos_.x, cPos.y, cPos.z);
}

void Player::HitEvent(Collider* myCol, Collider* other)
{

}

void Player::ApplyVelocity(void)
{
	ActorBase::ApplyVelocity();
	if (camera_)
	{
		camera_->SetPositionX(pos_.x);
	}
	if (body_)
	{
		body_->SetPosition({ pos_.x, pos_.y });
	}
	velocity_ = { 0.0f, 0.0f, 0.0f };

	if (pos_.y < -1000.0f)
	{
		scene_->PlayerDead();
	}
}

void Player::ApplyDamage(int damage)
{
	health_ = std::max(health_ - damage, 0);
	healthUI_->SetHealth(health_);
	
	float p = static_cast<float>(health_) / static_cast<float>(maxHealth_);
	p = Saturate((1.0f - p) - 0.5f) * 2.0f;
	DrawerIns.SetDamageEffect(pow(p, 2.0f));

	DrawerIns.StartCameraShake(*cameraShake_);

	bodyObject_->SetEnabled(false);
	if (grabObject_)
	{
		grabObject_->Throw({ 0.0f, 0.0f }, false);
		grabObject_->SetHeadingEnable(true);
		grabObject_->SetPhysicsEnable(true);
		grabObject_ = nullptr;
	}
	grabUpdate_ = &Player::CannottGrab;
	body_->active_ = false;
	if (IsDead())
	{
		AudioIns.PlayTemporary("hit", 0.8f, { SubmixHandleMng.GetHandle("se") });
		AudioIns.PlayTemporary("player_voice", 0.5f, { SubmixHandleMng.GetHandle("se") });
		time_ = 0.0f;
		ChangeState("dead");
	}
	else
	{
		AudioIns.SetFilter(bgmHandle_, XAUDIO2_FILTER_TYPE::LowPassFilter, 0.05f, 0.8f);
		AudioIns.PlayTemporary("hit", 0.8f, { SubmixHandleMng.GetHandle("se") });
		AudioIns.PlayTemporary("player_voice", 0.5f, { SubmixHandleMng.GetHandle("se") });
		ChangeState("hit");
	}
}

void Player::AddItemCount(ItemType type)
{
	size_t t = static_cast<size_t>(type);
	itemCount_[0] = itemCount_[0] + 1;

	switch (type)
	{
	case ItemType::Heal:
		if (itemCount_[t] > 0)
		{
			healthUI_->SetHealth(health_ + itemCount_[t] * 2);
			health_ = healthUI_->GetHealth();

			float p = static_cast<float>(health_) / static_cast<float>(maxHealth_);
			p = Saturate((1.0f - p) - 0.5f) * 2.0f;
			DrawerIns.SetDamageEffect(pow(p, 2.0f));

			itemCount_[t] = 0;
			HealEffect_->Play();
			HealEffect_->SetPosition({pos_.x, pos_.y, pos_.z});
		}
		break;
	case ItemType::Buff:
		break;
	default:
		break;
	}
}

void Player::SetBGMHandle(int handle)
{
	bgmHandle_ = handle;
}

void Player::Initialize(void)
{
	AudioIns.LoadSound("Resource/Sound/player_voice.wav", "player_voice");
	AudioIns.LoadSound("Resource/Sound/jump.wav", "player_jump");

	anim_ = std::make_unique<Animation>("Resource/Animation/PlayerAnim.xml");

	dir_ = Direction::Left;
	size_ = playerSize_;
	rightIsPositiveDir_ = false;
	SetDirection(Direction::Right);

	poly_->SetTexture(anim_->GetImageHandle());
	poly_->SetScale({ size_, size_ });
	poly_->SetPosition(pos_);

	angle_.x = -DX_PI_F * 0.5f;

	constexpr float sizescale = 0.55f;

	body_ = new CircleCollider(Circle(size_ * sizescale, { pos_.x, pos_.y }),
		true, { 0.0f, -size_ * (1.0f - sizescale) });
	body_->owner_ = this;
	body_->tag_ = "player_body";

	ColliderMngIns.AddCollider(body_);

	maxHealth_ = 100;
	health_ = 100;
	healthUI_ = std::make_unique<HealthUI>(maxHealth_, health_);

	colliders_.push_back(body_);

	AddCircleBody(size_ * sizescale, "player", { 0.0f, -size_ * (1.0f - sizescale) });

	state_.emplace("walk", StateAnimPair{ "idle", [this]() { update_ = &Player::WalkUpdate; } });
	state_.emplace("jump", StateAnimPair{ "jump", [this]() { update_ = &Player::JumpUpdate; } });
	state_.emplace("fall", StateAnimPair{ "jumpfall", [this]() { update_ = &Player::JumpFallUpdate; } });
	state_.emplace("grab", StateAnimPair{ "idle", [this]() { update_ = &Player::GrabAnimUpdate; } });
	state_.emplace("attack", StateAnimPair{ "attack", [this]() { update_ = &Player::AttackUpdate; } });
	state_.emplace("hit", StateAnimPair{ "hit", [this]() { update_ = &Player::HitUpdate; } });
	state_.emplace("dead", StateAnimPair{ "death", [this]() { update_ = &Player::DeadUpdate; } });

	ChangeState("walk");

	shadowSize_ = 30.0f;

	grabUpdate_ = &Player::NotGrab;

	itemCount_.fill(0);

	int a = itemCount_[0];

	HealEffect_ = std::make_unique<EffekseerEffectData>("Resource/Effect/Trail/Heal.efk", 20.0f);

	cameraShake_ = new ShakeDescriptor(CameraShake::MakeShakeDescriptor({ 5.0f, 0.0f, 0.0f }, 
		ShakeType::SinWave, 
		1.0f, 10.0f, 0.0f, 0.3f));
}

void Player::MoveUpdate(float delta)
{
	constexpr float xacc = 2000.0f;

	float move = 0.0f;

	if (InputIns.Pressed("left"))
	{
		move -= maxWalkSpeed_ * delta;
		isGoingRight_ = false;
	}
	if (InputIns.Pressed("right"))
	{
		move += maxWalkSpeed_ * delta;
		isGoingRight_ = true;
	}

	velocity_.x = move;

	if (camera_)
	{
		camera_->AddCameraPosition(move, 0.0f, 0.0f);
	}
}

void Player::JumpControlUpdate(float delta)
{
	if (InputIns.Triggered("jump") && canJump_)
	{
		currentFallSpeed_ = 700.0f;
		canJump_ = false;
		AudioIns.PlayTemporary("player_jump", 0.5f, { SubmixHandleMng.GetHandle("se") });
		ChangeState("jump");
	}
}

void Player::FallUpdate(float delta)
{
	float nextspeed = currentFallSpeed_ + phys_def::gravityAcc * delta;
	nextspeed = std::min(nextspeed, 980.0f);
	velocity_.y = (nextspeed + currentFallSpeed_) * 0.5f * delta;

	currentFallSpeed_ = nextspeed;
}

void Player::ThrowUpdate(float delta)
{
	if (grabObject_) { return; }
	if (InputIns.Pressed("throw"))
	{
		ThrowObject* nearobj = nullptr;
		float dist = FLT_MAX;
		auto&& hitCol = ColliderMngIns.CheckCollision(body_, "throw_area");

		if (hitCol.size() == 0) { return; }
		for (auto& c : hitCol)
		{
			ThrowObject* tobj = dynamic_cast<ThrowObject*>(c->owner_);
			if (!tobj) { continue; }

			VECTOR tPos = tobj->GetPosition();
			float d = SquareDistance(pos_.x, pos_.y, tPos.x, tPos.y);
			//tobj->Throw(GetThrowVec());
			if (d < dist)
			{
				dist = d;
				nearobj = tobj;
			}
		}
		if (nearobj)
		{
			grabObject_ = nearobj;
			grabObject_->SetAttackState(false);
			grabObject_->SetHeadingEnable(false);
			grabObject_->SetVelocity({ 0.0f, 0.0f, 0.0f });
			grabObject_->SetPhysicsEnable(false);
			grabObject_->StopEffect();
			grabObject_->SetPlayer(this);
			time_ = 0.0f;
			CalculateGrabAnimationVelocity();
			Application::SetGameSpeed(0.0f, animtime_);
			AudioIns.PlayTemporary("get_ball", 1.0f, { SubmixHandleMng.GetHandle("se") });
			ChangeState("grab");
			grabUpdate_ = &Player::GrabStartUpdate;
		}
	}
}

void Player::WalkUpdate(float delta)
{
	if (canMove_)
	{
		MoveUpdate(delta);
	}
	if (currentFallSpeed_ < -600.0f)
	{
		FallUpdate(delta);
		ChangeState("jump");
		return;
	}
	JumpControlUpdate(delta);
	FallUpdate(delta);
}

void Player::JumpUpdate(float delta)
{
	if (canMove_)
	{
		MoveUpdate(delta);
	}

	FallUpdate(delta);
	if (isHitFloor_)
	{
		auto&& eInst = EffectMng::GetInstance();

		eInst.PlayEffect("fall_smoke", { pos_.x, pos_.y - size_ * 0.5f, pos_.z - 1.0f }, 0.0, 1.5f, 0.2f);
		ChangeState("fall");
	}
}

void Player::JumpFallUpdate(float delta)
{
	if (canMove_)
	{
		MoveUpdate(delta);
	}

	FallUpdate(delta);
	if (anim_->GetCurrentAnimationProgress() >= 1.0f)
	{
		ChangeState("walk");
	}
}

void Player::GrabAnimUpdate(float delta)
{
	// なにもしない
}

void Player::AttackUpdate(float delta)
{
	FallUpdate(delta);
	if (anim_->GetCurrentAnimationProgress() >= 1.0f)
	{
		ChangeState("walk");
	}
}

void Player::HitUpdate(float delta)
{
	float prog = anim_->GetCurrentAnimationProgress();

	if (prog >= 1.0f)
	{
		invincibleTimer_ = InvincibleDuration;
		grabUpdate_ = &Player::NotGrab;
		bodyObject_->SetEnabled(true);
		ChangeState("walk");
	}
	FallUpdate(delta);
}

void Player::DeadUpdate(float delta)
{
	time_ += delta;
	if (time_ >= 2.0f && !isRestarting_)
	{
		isRestarting_ = true;
		scene_->PlayerDead();
	}
	FallUpdate(delta);
}

void Player::CannottGrab(float delta)
{
	// なにもしない
}

void Player::NotGrab(float delta)
{
	ThrowUpdate(delta);
}

// ボールを持つアニメーションを再生している
void Player::GrabStartUpdate(float delta)
{
	float rDelta = Application::GetRealDeltaTime();
	time_ += rDelta;

	float nextspeed = grabCurrentFallSpeed_ + phys_def::gravityAcc * GrabObjectGravityScale * rDelta;
	grabAnimVel_.y = (nextspeed + grabCurrentFallSpeed_) * 0.5f * rDelta;

	grabCurrentFallSpeed_ = nextspeed;

	grabObject_->AddPosition({ grabAnimVel_.x * rDelta, grabAnimVel_.y });

	if (time_ > animtime_)
	{
		ChangeState("walk");
		grabUpdate_ = &Player::GrabUpdate;
	}
}

void Player::GrabUpdate(float delta)
{
	grabObject_->SetPosition(Vector2<float>{ pos_.x, pos_.y } + GrabOffsetPosition);
	if (!InputIns.Pressed("throw"))
	{
		grabObject_->Throw(GetThrowVec());
		grabObject_->SetHeadingEnable(true);
		grabObject_->SetPhysicsEnable(true);

		ChangeState("attack");
		AudioIns.PlayTemporary("throw_ball", 1.0f, { SubmixHandleMng.GetHandle("se") });

		grabObject_ = nullptr;
		grabUpdate_ = &Player::NotGrab;
	}
}

// 向いている方向をチェック
void Player::CheckDir(float delta)
{
	if (velocity_.x != 0.0f)
	{
		SetDirection(velocity_.x >= 0.0f ? Direction::Right : Direction::Left);
	}
}

void Player::InvincibleTimerUpdate(float delta)
{
	if (invincibleTimer_ > 0.0f)
	{
		AudioIns.SetFilter(bgmHandle_, XAUDIO2_FILTER_TYPE::LowPassFilter,
			std::min(0.05f + pow(1.0f - invincibleTimer_ / InvincibleDuration, 3.0f), 1.0f), 0.8f);

		invincibleTimer_ = fmaxf(invincibleTimer_ - delta, 0.0f);
		if (invincibleTimer_ == 0.0f)
		{
			body_->active_ = true;
		}
	}
}

b2Vec2 Player::GetThrowVec(void)
{
	b2Vec2 vel = { 0.0f, 0.0f };

	if (InputIns.Pressed("down"))
	{
		vel = { isGoingRight_ ? 1.0f : -1.0f, 0.0f };
	}
	else if (InputIns.Pressed("up"))
	{
		vel = { 0.0f, 1.0f };
	}
	else
	{
		vel = { sinf(DegreeToRadian(isGoingRight_ ? throwAngle_ : -throwAngle_)), cosf(DegreeToRadian(throwAngle_)) };
	}

	vel *= throwSpeed_;
	return vel;
}

// 持ち上げる時のボールの動きを計算
void Player::CalculateGrabAnimationVelocity(void)
{
	auto&& gPos = grabObject_->GetPosition();
	Vector2<float> curPos = { gPos.x, gPos.y };
	Vector2<float> topPos = Vector2<float>{ pos_.x, pos_.y } + GrabAnimTopPositionOffset;
	Vector2<float> target = Vector2<float>{ pos_.x, pos_.y } + GrabOffsetPosition;

	float topDiff, yDiff = topPos.y - curPos.y;
	float v0, tv0;

	// ボールが自分より下の時
	if (yDiff > 0.0f)
	{
		v0 = 2.0f * phys_def::gravityAcc * GrabObjectGravityScale * yDiff * -1.0f;
		v0 = sqrt(v0);

		animtime_ = -v0 / (phys_def::gravityAcc * GrabObjectGravityScale);

		topDiff = target.y - topPos.y;

		animtime_ += sqrt(topDiff / (0.5f * phys_def::gravityAcc * GrabObjectGravityScale));
	}
	// 上の時
	else
	{
		v0 = 0.0f;
		topDiff = target.y - curPos.y;

		animtime_ = sqrt(topDiff / (0.5f * phys_def::gravityAcc * GrabObjectGravityScale));
	}

	grabAnimVel_ = { (target.x - curPos.x) / animtime_, v0 };
	grabCurrentFallSpeed_ = v0;
}
