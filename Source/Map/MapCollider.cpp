#include "MapCollider.h"
#include <algorithm>
#include "../../RapidXML/rapidxml_utils.hpp"
#include "../MyRapidXML_Helper.h"
#include "../Actor/ActorBase.h"
#include "../Collider/Collider.h"
#include "../Defines/Unit_Defines.h"
#include "../MyB2DHelper.h"


MapCollider::MapCollider(b2World* world)
	:world_(world)
{
	
}

MapCollider::~MapCollider()
{
	for (auto& c : colliders_)
	{
		delete c;
	}

	doc_.clear();
}

void MapCollider::LoadFile(const std::string& filepath)
{
	rapidxml::file<> file = filepath.c_str();
	doc_.parse<0>(file.data());

	rapidxml::xml_node<>* root = doc_.first_node();

	for (auto colnode = root->first_node("ColliderData"); colnode != nullptr;
		colnode = colnode->next_sibling())
	{
		CheckVertCount(colnode);
	}

}

void MapCollider::CheckCollisionDetection(ActorBase* actor, Collider* col)
{
	if (!actor || !col) { return; }

	switch (col->GetType())
	{
	case ColliderType::Circle:
		CheckCollisionDetection(actor, dynamic_cast<CircleCollider*>(col));
		break;
	case ColliderType::Square:
		break;
	case ColliderType::Capsule:
		break;
	case ColliderType::Triangle:
		break;
	default:
		break;
	}
}

void MapCollider::CheckCollisionDetection(ActorBase* actor, CircleCollider* col)
{
	auto vel = actor->GetVelocity();
	if (vel.x == 0.0f && vel.y == 0.0f && vel.z == 0.0f) { return; }

	//if (vel.y < 0.0f && vel.y > -1.0f && actor->CanJump())
	//{
	//	vel.y = 0.0f;
	//}
	
	if (vel.y < -10.0)
	{
		actor->SetJump(false);
	}

	CapsuleCollider capsule;
	capsule.capsule_.radius_ = col->circle_.radius_;
	capsule.capsule_.line_.start_ = col->GetPosition();
	capsule.capsule_.line_.vec_ = { vel.x, vel.y };

	Vector2<float> hp;
	Vector2<float> res;
	Vector2<float> norm;
	bool ishitfloor = false;
	bool ishit = false;

	constexpr int executeCount = 2;
	for (int i = 0; i < executeCount; i++)
	{
		bool hit = SweepCircle(&capsule, &hp, &res, &norm);
		if (!hit) { break; }
		if (std::isnan(res.x) || std::isnan(res.y))
		{
			break;
		}
		ishit = true;
		if ((Dot(norm, { 0.0f, 1.0f }) > sinf(DX_PI_F * 0.2f)) && vel.y < 0.0f)
		{
			ishitfloor = true;
		}
		if (Dot(norm, { 0.0f, -1.0f }) > sinf(DX_PI_F * 0.3f))
		{
			actor->ResetFallSpeed();
		}


		//capsule.capsule_.line_.start_ = hp;
		//capsule.capsule_.line_.vec_ = res - hp;

		capsule.capsule_.line_.vec_ = res - capsule.capsule_.line_.start_;

		//if (abs(capsule.capsule_.line_.vec_.x) > 100000.0f)
		//{
		//	int a = 0;
		//}

		/*OutputDebugStringA((std::to_string(capsule.capsule_.line_.vec_.x) + " " + 
			std::to_string(capsule.capsule_.line_.vec_.y) + "\n").c_str());*/
		if (capsule.capsule_.line_.vec_.SquareDistance() < 1.0f)
		{
			break;
		}
	}
	if (ishit)
	{
	}
	if (ishitfloor)
	{
		actor->ResetFallSpeed();
		actor->SetJump(true);
	}
	actor->SetHitFloor(ishitfloor);
	actor->SetHitMap(ishit);

	//res = { std::floor(res.x), std::floor(res.y) };

	actor->SetVelocity(VSub({ res.x, res.y, 0.0f }, 
		{ col->circle_.pos_.x, col->circle_.pos_.y, 0.0f }));

	hitcolliders_.clear();
}

void MapCollider::CheckCollisionDetection(ActorBase* actor, SquareCollider* col)
{
}

void MapCollider::CheckCollisionDetection(ActorBase* actor, CapsuleCollider* col)
{
}

void MapCollider::CheckCollisionDetection(ActorBase* actor, TriangleCollider* col)
{
}

void MapCollider::CheckVertCount(rapidxml::xml_node<>* node)
{
	auto vc = node->first_node("vCount_");
	int vcount = atoi(GetNodeValue(vc));

	constexpr int triangleVertCount = 3;
	constexpr int squareVertCount = 4;

	auto vertnode = node->first_node("vertices_");

	switch (vcount)
	{
	case triangleVertCount:
		AddTriangleCollider(vertnode);
		break;
	case squareVertCount:
		AddSquareCollider(vertnode);
		break;
	default:
		break;
	}
}

void MapCollider::AddTriangleCollider(rapidxml::xml_node<>* node)
{
	TriangleCollider* col = new TriangleCollider();

	std::vector<b2Vec2> b2vert;
	int current = 0;
	for (auto vert = node->first_node("Vector2"); vert != nullptr;
		vert = vert->next_sibling())
	{
		float x = static_cast<float>(atof(GetNodeValue(vert->first_node("x"))));
		float y = static_cast<float>(atof(GetNodeValue(vert->first_node("y"))));

		col->triangle_.vertices_[current].x = x * unit::m_to_cm;
		col->triangle_.vertices_[current].y = y * unit::m_to_cm;

		b2vert.push_back({ x, y });

		current++;
	}
	col->triangle_.ApplyToLines();
	colliders_.push_back(col);

	b2BodyDef bDef;
	bDef.position.Set(b2vert[0].x, b2vert[0].y);
	
	MyUserData* data = new MyUserData();
	data->owner_ = nullptr;
	data->tag_ = "map";
	SetBodyData(&bDef, data);

	b2Body* body = world_->CreateBody(&bDef);

	b2Vec2 org = b2vert[0];
	std::for_each(b2vert.begin(), b2vert.end(), [&org](b2Vec2& vec) { vec -= org; });

	b2PolygonShape bTri;
	bTri.Set(b2vert.data(), static_cast<int32>(b2vert.size()));

	body->CreateFixture(&bTri, 0.0f);

	bodys_.push_back(body);
}

void MapCollider::AddSquareCollider(rapidxml::xml_node<>* node)
{
	SquareCollider* col = new SquareCollider();

	Vector2<float> ldpos;
	Vector2<float> rupos;

	auto vert = node->first_node("Vector2");
	ldpos.x = static_cast<float>(atof(GetNodeValue(vert->first_node("x"))));
	ldpos.y = static_cast<float>(atof(GetNodeValue(vert->first_node("y"))));
	rupos = ldpos;

	for (vert = vert->next_sibling(); vert != nullptr;
		vert = vert->next_sibling())
	{
		float x = static_cast<float>(atof(GetNodeValue(vert->first_node("x"))));
		float y = static_cast<float>(atof(GetNodeValue(vert->first_node("y"))));

		ldpos.x = fminf(ldpos.x, x);
		ldpos.y = fminf(ldpos.y, y);
		rupos.x = fmaxf(rupos.x, x);
		rupos.y = fmaxf(rupos.y, y);
	}

	col->square_.pos_ = ldpos * unit::m_to_cm;
	col->square_.size_ = (rupos - ldpos) * unit::m_to_cm;
	col->square_.ApplyToLines();
	colliders_.push_back(col);

	b2BodyDef bDef;
	bDef.position.Set((ldpos.x + rupos.x) * 0.5f, 
		(ldpos.y + rupos.y) * 0.5f);

	MyUserData* data = new MyUserData();
	data->owner_ = nullptr;
	data->tag_ = "map";
	SetBodyData(&bDef, data);

	b2Body* body = world_->CreateBody(&bDef);

	b2PolygonShape bSquare;
	bSquare.SetAsBox((rupos.x - ldpos.x) * 0.5f, 
		(rupos.y - ldpos.y) * 0.5f);

	body->CreateFixture(&bSquare, 0.0f);

	bodys_.push_back(body);
}

Collider* MapCollider::FindHitCollision(Collider* col)
{
	Collider* ret = nullptr;

	for (auto& c : colliders_)
	{
		if (col->isHit(c))
		{
			ret = c;
			break;
		}
	}

	return ret;
}


bool MapCollider::SweepCircle(CapsuleCollider* col, Vector2<float>* hitPoint, Vector2<float>* result, Vector2<float>* normal)
{
	bool hit = false;
	float dist = 1.0f;
	Vector2<float> res = col->capsule_.line_.EndPosition();
	Vector2<float> norm;

	Collider* hitcol = nullptr;

	for (auto& c : colliders_)
	{
		if (std::find(hitcolliders_.begin(), hitcolliders_.end(), c) != hitcolliders_.end()) 
		{ 
			continue;
		}
		auto&& [d, p, n] = c->GetSweepCircleHitDistance(col);
		if (d < dist)
		{
			dist = d;
			res = p;
			norm = n;
			hit = true;
			hitcol = c;
		}
	}

	if (hitcol)
	{
		hitcolliders_.push_back(hitcol);
	}
	if (result)
	{
		*result = res;
	}
	if (normal)
	{
		*normal = norm;
	}
	if (hitPoint)
	{
		*hitPoint = col->capsule_.line_.start_ +
			(col->capsule_.line_.EndPosition() - col->capsule_.line_.start_) * dist;

		if (dist != 1.0f)
		{
			//OutputDebugStringA((std::to_string(dist) + "\n").c_str());
		}
	}

	return hit;
}
