#pragma once
#include <memory>
#include <vector>

class MapCollider;
class ActorBase;
class Player;
class ActorManager
{
public:
	ActorManager();
	~ActorManager();

	void Update(float delta);
	void CheckMapCollision(MapCollider* mapcol);
	void ApplyVelocity(void);
	void Draw(void);
	void DrawSimpleShadow(int stagemodel);

	void Add(ActorBase* actor);

	void SetMoveEnable(bool flag);

	void SetPlayer(std::unique_ptr<Player>* player);
private:
	std::vector<std::unique_ptr<ActorBase>> actors_;
	std::vector<std::unique_ptr<ActorBase>> tmpActors_;

	std::unique_ptr<Player>* player_;

	bool isProcessing_ = false;

	static constexpr float ActivateDistance = 2000.0f;
	static constexpr float DrawShadowDistance = 1000.0f;
};

