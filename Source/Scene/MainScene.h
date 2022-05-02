#pragma once
#include "Scene.h"
#include <memory>
#include <string>
#include <unordered_map>
#include <vector>
#include <box2d.h>
#include "../../RapidXML/rapidxml.hpp"
#include <functional>

class Camera;
class Map;
class MapCollider;
class Player;
class BackGround;
class EnemyManager;
class Goal;
class ThrowObjManager;
class MyListener;
struct MapData;
class Enemy;
class ActorManager;
class MainScene :
    public Scene
{
public:
	MainScene() = delete;
    MainScene(MapData* map);
    ~MainScene();

    void Update(float delta)override;

	void BeforeDraw(void)override;
    void Draw(void)override;

	void Initialize(void)override;

	void PlayerDead(void);
	void StageClear(void);
private:
	void StartUpdate(float delta);
	void GameUpdate(float delta);
	void ClearUpdate(float delta);
	void DeadUpdate(float delta);
	void (MainScene::*update_)(float delta);

	void B2DUpdate(float delta);

	void InitializeActors(void);
	void InitializePlayer(rapidxml::xml_node<>* node);
	void InitializeEnemy(rapidxml::xml_node<>* node);
	void InitializeGoal(rapidxml::xml_node<>* node);
	void InitializeBall(rapidxml::xml_node<>* node);

	void SpawnExplosion(void);

	void GetDeadScreen(void);
	void DeadDraw(void);

    std::unique_ptr<Map> map_;
    std::unique_ptr<MapCollider> mapcol_;
    std::unique_ptr<Camera> camera_;
    std::unique_ptr<Player> player_;
	std::unique_ptr<BackGround> bg_;
	std::unique_ptr<Goal> goal_;

	std::unique_ptr<EnemyManager> enemyMng_;
	std::unique_ptr<ActorManager> actorMng_;

	std::unique_ptr<b2World> world_;
	std::unique_ptr<MyListener> listener_;

	std::unique_ptr<ThrowObjManager> throwMng_;

	std::unordered_map<std::string, std::function<Enemy*(float, float)>> enemyFunc_;

	MapData* mapmeta_;

	float time_ = 0.0f;

	int bgmHandle_;

	int clearP_;
	float cTime_ = 0.0f;
	int deadScreen_;

	int screenWidth_;
	int screenHeight_;

	bool isDrawEnable_ = true;
	bool isPlayerDead_ = false;
};

