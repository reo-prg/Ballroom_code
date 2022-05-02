#pragma once
#include "ActorBase.h"
#include "Item/ItemType.h"

class Camera;
class HealthUI;
class Animation;
class MainScene;
class ThrowObject;
class EffekseerEffectData;
struct ShakeDescriptor;
class Player :
	public ActorBase
{
public:
	Player(const VECTOR& pos, b2World* world, MainScene* scene);
	~Player();

	void Update(float delta)override;
	void Draw(void)override;

	void SetCamera(Camera* camera);

	void HitEvent(Collider* myCol, Collider* other)override;
	void ApplyVelocity(void)override;
	void ApplyDamage(int damage)override;

	void AddItemCount(ItemType type);

	void SetBGMHandle(int handle);

	static constexpr float playerSize_ = 50.0f;
private:
	void Initialize(void);

	void MoveUpdate(float delta);
	void JumpControlUpdate(float delta);
	void FallUpdate(float delta);
	void ThrowUpdate(float delta);

	void(Player::*update_)(float);

	void WalkUpdate(float delta);
	void JumpUpdate(float delta);
	void JumpFallUpdate(float delta);
	void GrabAnimUpdate(float delta);
	void AttackUpdate(float delta);
	void HitUpdate(float delta);
	void DeadUpdate(float delta);

	void (Player::*grabUpdate_)(float);

	void CannottGrab(float delta);
	void NotGrab(float delta);
	void GrabStartUpdate(float delta);
	void GrabUpdate(float delta);

	void CheckDir(float delta);
	void InvincibleTimerUpdate(float delta)override;

	b2Vec2 GetThrowVec(void);

	void CalculateGrabAnimationVelocity(void);

	Camera* camera_;
	Collider* body_;

	float maxWalkSpeed_ = 300.0f;

	std::unique_ptr<HealthUI> healthUI_;

	MainScene* scene_;

	ThrowObject* grabObject_;

	float time_;
	bool isRestarting_ = false;

	bool isGoingRight_ = true;

	float throwSpeed_ = 8.0f;
	float throwAngle_ = 60.0f;

	Vector2<float> grabAnimVel_;
	float animtime_ = 0.3f;
	float grabCurrentFallSpeed_;

	std::array<int, static_cast<size_t>(ItemType::Max)> itemCount_;

	std::unique_ptr<EffekseerEffectData> HealEffect_;

	ShakeDescriptor* cameraShake_;

	int bgmHandle_;
};

