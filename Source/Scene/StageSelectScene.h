#pragma once
#include "Scene.h"
#include <memory>
#include "../Math/VectorInterface.h"

class StageSelectUI;
class Camera;
class BackGround;
class Animation;
struct MapData;
class StageSelectScene :
    public Scene
{
public:
	StageSelectScene() = delete;
	StageSelectScene(int stage);
	~StageSelectScene() = default;

	void Update(float delta)override;

	void BeforeDraw(void)override;

	void Draw(void)override;

	void Initialize(void)override;

	void ChangeScene(const std::shared_ptr<Scene>& scene);
private:
	void StartUpdate(float delta);
	void SelectUpdate(float delta);
	void EndUpdate(float delta);
	void (StageSelectScene::* update_)(float delta);

	MapData* CreateMapData(int mapId);

	std::unique_ptr<StageSelectUI> ui_;

	std::unique_ptr<Camera> camera_;
	std::unique_ptr<BackGround> bg_;
	std::unique_ptr<Animation> playerAnim_;
	bool playerGoForward_;

	Vector2<float> playerPos_;

	float bgSpeed_;

	float time_;

	int screenWidth_;

	float playerSpeed_;

	int stage_;

	int bgmHandle_;
};

