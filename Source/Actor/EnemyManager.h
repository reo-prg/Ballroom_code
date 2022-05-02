#pragma once
#include <memory>
#include <vector>

class Enemy;
class MapCollider;
class Player;
class EnemyManager
{
public:
	EnemyManager();
	~EnemyManager();

	void Update(float delta);
	void CheckMapCollision(MapCollider* mapcol);
	void ApplyVelocity(void);
	void Draw(void);
	void DrawSimpleShadow(int stagemodel);

	void SetPlayer(std::unique_ptr<Player>* player);

	void Add(Enemy* enemy);

	void SetMoveEnable(bool flag);

	void SetGameState(bool flag);
private:

	std::vector<std::unique_ptr<Enemy>> enemies_;

	std::unique_ptr<Player>* player_;

	static constexpr float ActivateDistance = 2000.0f;
	static constexpr float DrawShadowDistance = 1000.0f;
};

