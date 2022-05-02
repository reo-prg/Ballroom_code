#pragma once
#include <box2d.h>
#include <vector>
#include "../MyB2DHelper.h"

class ActorBase;
struct HitPair
{
	b2Body* bodyA_;
	MyUserData* dataA_;
	b2Body* bodyB_;
	MyUserData* dataB_;
	b2Vec2 normal_;
	bool touch_;
};

class MyListener :
	public b2ContactListener
{
public:
	std::vector<HitPair> list_;

	std::vector<b2Body*> deletebody_;

	void BeginContact(b2Contact* contact)override
	{
		HitPair hit;
		hit.bodyA_ = contact->GetFixtureA()->GetBody();
		hit.bodyB_ = contact->GetFixtureB()->GetBody();
		hit.dataA_ = GetBodyData(hit.bodyA_);
		hit.dataB_ = GetBodyData(hit.bodyB_);

		hit.dataA_->count_++;
		hit.dataB_->count_--;

		b2WorldManifold mn;
		contact->GetWorldManifold(&mn);
		hit.normal_ = mn.normal;

		hit.touch_ = contact->IsTouching();

		list_.push_back(std::move(hit));
	};

	void EndContact(b2Contact* contact)override
	{
		HitPair hit;
		hit.bodyA_ = contact->GetFixtureA()->GetBody();
		hit.bodyB_ = contact->GetFixtureB()->GetBody();
		hit.dataA_ = GetBodyData(hit.bodyA_);
		hit.dataB_ = GetBodyData(hit.bodyB_);

		hit.dataA_->count_--;
		hit.dataB_->count_--;
	};

	void DeleteB2Body(b2Body* body)
	{
		deletebody_.push_back(body);
	}
};
