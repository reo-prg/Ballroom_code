#include "EnemyManager.h"
#include <algorithm>
#include "Enemy.h"
#include "Player.h"
#include "../Map/MapCollider.h"

EnemyManager::EnemyManager()
{
}

EnemyManager::~EnemyManager()
{
}

void EnemyManager::Update(float delta)
{
	auto it = std::remove_if(enemies_.begin(), enemies_.end(),
		[](const std::unique_ptr<Enemy>& en) { return en->IsDeleted(); });
	enemies_.erase(it, enemies_.end());

	auto&& pPos = (*player_)->GetPosition();
	for (auto& e : enemies_)
	{
		float dist = abs(pPos.x - e->GetPosition().x);
		if (dist > ActivateDistance) { continue; }
		e->Update(delta);
	}
}

void EnemyManager::CheckMapCollision(MapCollider* mapcol)
{
	auto&& pPos = (*player_)->GetPosition();
	for (auto& e : enemies_)
	{
		float dist = abs(pPos.x - e->GetPosition().x);
		if (dist > ActivateDistance) { continue; }
		mapcol->CheckCollisionDetection(e.get(), e->GetCollider("enemy_body"));
	}
}

void EnemyManager::ApplyVelocity(void)
{
	auto&& pPos = (*player_)->GetPosition();
	for (auto& e : enemies_)
	{
		float dist = abs(pPos.x - e->GetPosition().x);
		if (dist > ActivateDistance) { continue; }
		e->ApplyVelocity();
	}
}

void EnemyManager::Draw(void)
{
	auto&& pPos = (*player_)->GetPosition();
	for (auto& e : enemies_)
	{
		float dist = abs(pPos.x - e->GetPosition().x);
		if (dist > DrawShadowDistance) { continue; }
		e->Draw();
	}
}

void EnemyManager::DrawSimpleShadow(int stagemodel)
{
	auto&& pPos = (*player_)->GetPosition();
	for (auto& e : enemies_)
	{
		float dist = abs(pPos.x - e->GetPosition().x);
		if (dist > DrawShadowDistance) { continue; }
		e->DrawSimpleShadow(stagemodel);
	}
}

void EnemyManager::SetPlayer(std::unique_ptr<Player>* player)
{
	player_ = player;
}

void EnemyManager::Add(Enemy* enemy)
{
	if (!enemy) { return; }
	enemies_.push_back(std::move(std::unique_ptr<Enemy>(enemy)));
}

void EnemyManager::SetMoveEnable(bool flag)
{
	for (auto& e : enemies_)
	{
		e->SetMoveEnable(flag);
	}
}

void EnemyManager::SetGameState(bool flag)
{
	for (auto&& e : enemies_)
	{
		e->SetInGame(flag);
	}
}
