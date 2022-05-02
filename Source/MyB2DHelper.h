#pragma once
#include <box2d.h>
#include <string>

class ActorBase;

struct MyUserData
{
	std::string tag_;
	ActorBase* owner_ = nullptr;
	bool isValid_ = true;
	int count_ = 0;
};

namespace
{
	MyUserData* GetBodyData(b2Body* body)
	{
		MyUserData* d = reinterpret_cast<MyUserData*>(body->GetUserData().pointer);
		return d;
	}

	b2Body* GetOtherBody(b2Contact* contact, b2Body* self)
	{
		b2Body* a = contact->GetFixtureA()->GetBody();
		b2Body* b = contact->GetFixtureB()->GetBody();

		b2Body* other = (self == a) ? b : a;

		return other;
	}

	void SetBodyData(b2BodyDef* bodydef, MyUserData* tag)
	{
		bodydef->userData.pointer = reinterpret_cast<uintptr_t>(tag);
	}
}
