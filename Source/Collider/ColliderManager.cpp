#include "ColliderManager.h"
#include <algorithm>
#include "Collider.h"
#include "../Actor/ActorBase.h"

ColliderManager* ColliderManager::instance_ = nullptr;

void ColliderManager::Create(void)
{
	instance_ = new ColliderManager();
}

ColliderManager& ColliderManager::GetInstance(void)
{
	return *instance_;
}

void ColliderManager::Terminate(void)
{
	delete instance_;
}

void ColliderManager::AddCollider(Collider* collider)
{
	colliders_.push_back(collider);
}

void ColliderManager::RemoveCollider(Collider* collider)
{
	auto it = std::remove_if(colliders_.begin(), colliders_.end(), [&collider](Collider* col) { return collider == col; });
	if (it == colliders_.end()) { return; }
	colliders_.erase(it, colliders_.end());
}

void ColliderManager::CheckCollision(void)
{
	auto it = std::remove_if(colliders_.begin(), colliders_.end(), [](Collider* col) 
		{ 
			return col->delete_;
		});
	colliders_.erase(it, colliders_.end());

	for (auto cola = colliders_.begin(); cola != colliders_.end(); cola++)
	{
		if (!(*cola)->active_) { continue; }
		for (auto colb = cola + 1; colb != colliders_.end(); colb++)
		{
			if (!(*colb)->active_) { continue; }

			bool hit = (*cola)->isHit(*colb);

			if (hit)
			{
				(*cola)->CallHitEvent(*colb);
				(*colb)->CallHitEvent(*cola);
			}
		}
	}
}

std::vector<Collider*> ColliderManager::CheckCollision(Collider* collider)
{
	std::vector<Collider*> hitList;
	for (auto col = colliders_.begin(); col != colliders_.end(); col++)
	{
		if ((*col) == collider) { continue; }
		if (!(*col)->active_) { continue; }

		bool hit = collider->isHit(*col);
		if (hit)
		{
			hitList.push_back(*col);
		}
	}
	return hitList;
}

std::vector<Collider*> ColliderManager::CheckCollision(Collider* collider, const std::string& tag)
{
	std::vector<Collider*> hitList;
	for (auto col = colliders_.begin(); col != colliders_.end(); col++)
	{
		if ((*col) == collider) { continue; }
		if ((*col)->tag_ != tag) { continue; }
		if (!(*col)->active_) { continue; }

		bool hit = collider->isHit(*col);
		if (hit)
		{
			hitList.push_back(*col);
		}
	}
	return hitList;
}

void ColliderManager::Clear(void)
{
	colliders_.clear();
}
