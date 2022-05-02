#pragma once
#include <string>
#include <vector>
#include <box2d.h>
#include <b2_body.h>
#include "../../RapidXML/rapidxml.hpp"
#include "../Math/VectorInterface.h"

struct Collider;
struct CircleCollider;
struct SquareCollider;
struct CapsuleCollider;
struct TriangleCollider;
class ActorBase;
class MapCollider
{
public:
	MapCollider() = default;
	MapCollider(b2World* world);
	~MapCollider();

	void LoadFile(const std::string& filepath);

	void CheckCollisionDetection(ActorBase* actor, Collider* col);
	void CheckCollisionDetection(ActorBase* actor, CircleCollider* col);
	void CheckCollisionDetection(ActorBase* actor, SquareCollider* col);
	void CheckCollisionDetection(ActorBase* actor, CapsuleCollider* col);
	void CheckCollisionDetection(ActorBase* actor, TriangleCollider* col);
private:
	void CheckVertCount(rapidxml::xml_node<>* node);
	void AddTriangleCollider(rapidxml::xml_node<>* node);
	void AddSquareCollider(rapidxml::xml_node<>* node);

	Collider* FindHitCollision(Collider* col);
	bool SweepCircle(CapsuleCollider* col, Vector2<float>* hitPoint, Vector2<float>* result, Vector2<float>* normal);

	std::vector<Collider*> colliders_;
	std::vector<Collider*> hitcolliders_;

	std::vector<b2Body*> bodys_;

	rapidxml::xml_document<> doc_;

	b2World* world_;
};

