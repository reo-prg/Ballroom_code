#include "ActorManager.h"
#include <algorithm>
#include "ActorBase.h"
#include "Player.h"
#include "../Map/MapCollider.h"

ActorManager::ActorManager()
{
}

ActorManager::~ActorManager()
{
}

void ActorManager::Update(float delta)
{
	isProcessing_ = true;

	auto it = std::remove_if(actors_.begin(), actors_.end(),
		[](const std::unique_ptr<ActorBase>& en) { return en->IsDeleted(); });
	actors_.erase(it, actors_.end());

	auto&& pPos = (*player_)->GetPosition();
	for (auto& act : actors_)
	{
		float dist = abs(pPos.x - act->GetPosition().x);
		if (dist > ActivateDistance) { continue; }
		act->Update(delta);
	}

	for (auto& tmp : tmpActors_)
	{
		actors_.push_back(std::move(tmp));
	}

	isProcessing_ = false;
}

void ActorManager::CheckMapCollision(MapCollider* mapcol)
{
	auto&& pPos = (*player_)->GetPosition();
	for (auto& act : actors_)
	{
		float dist = abs(pPos.x - act->GetPosition().x);
		if (dist > ActivateDistance) { continue; }
		if (act->IsMapCollisionEnable())
		{
 			mapcol->CheckCollisionDetection(act.get(), act->GetCollider("body"));
		}
	}
}

void ActorManager::ApplyVelocity(void)
{
	auto&& pPos = (*player_)->GetPosition();
	for (auto& act : actors_)
	{
		float dist = abs(pPos.x - act->GetPosition().x);
		if (dist > ActivateDistance) { continue; }
		act->ApplyVelocity();
	}
}

void ActorManager::Draw(void)
{
	auto&& pPos = (*player_)->GetPosition();
	for (auto& act : actors_)
	{
		float dist = abs(pPos.x - act->GetPosition().x);
		if (dist > ActivateDistance) { continue; }
		act->Draw();
	}
}

void ActorManager::DrawSimpleShadow(int stagemodel)
{
	auto&& pPos = (*player_)->GetPosition();
	for (auto& act : actors_)
	{
		float dist = abs(pPos.x - act->GetPosition().x);
		if (dist > DrawShadowDistance) { continue; }
		act->DrawSimpleShadow(stagemodel);
	}
}

void ActorManager::Add(ActorBase* actor)
{
	if (!actor) { return; }
	if (isProcessing_)
	{
		tmpActors_.push_back(std::move(std::unique_ptr<ActorBase>(actor)));
	}
	else
	{
		actors_.push_back(std::move(std::unique_ptr<ActorBase>(actor)));
	}
}

void ActorManager::SetMoveEnable(bool flag)
{
	for (auto& act : actors_)
	{
		act->SetMoveEnable(flag);
	}
}

void ActorManager::SetPlayer(std::unique_ptr<Player>* player)
{
	player_ = player;
}
