#pragma once
#include "ActorBase.h"
#include <box2d.h>
#include <b2_body.h>
#include <EffekseerForDXLib.h>
#include "../Utility/RingBuffer.h"

class EffekseerEffectData;
class Player;
class ThrowObject
	: public ActorBase
{
public:
	ThrowObject() = delete;
	ThrowObject(const VECTOR& pos, float size, b2World* world);
	ThrowObject(const std::string& modelPath, float scale, const VECTOR& rotate, const VECTOR& pos, float size, b2World* world);
	~ThrowObject();

	virtual void Update(float delta)override;
	virtual void Draw(void)override;

	void SetPosition(const VECTOR& pos)override;
	void SetPosition(const Vector2<float>& pos)override;
	void AddPosition(const Vector2<float>& pos)override;

	float GetSize(void) { return size_; };

	void B2HitEvent(b2Body* mybody, MyUserData* myData,
		b2Body* otherbody, MyUserData* otherData, const b2Vec2& hitNormal)override;

	void Throw(const b2Vec2& velocity_, bool attack = true);

	void SetAttackState(bool attacking) { isAttacking_ = attacking; }
	void SetHeadingEnable(bool flag) { canHeading_ = flag; }

	void SetPhysicsEnable(bool flag);

	void SetVelocity(VECTOR vel)override;
	void StopEffect(void);

	void SetPlayer(Player* player);
private:
	void Initialize(b2World* world);
	void Initialize(const std::string& modelPath, float scale, const VECTOR& rotate, b2World* world);

	b2Body* body_;
	float size_;

	bool isAttacking_ = false;
	bool canThrow_ = true;
	bool canHeading_ = true;

	bool isHit_ = false;

	float time_ = 0.0f;

	std::unique_ptr<EffekseerEffectData> hitEffect_;
	std::unique_ptr<EffekseerEffectData> trailEffect_;

	int modelId_ = -1;

	VECTOR rotate_;

	RingBuffer<float, 10> velHistory_;

	Player* player_;
};

