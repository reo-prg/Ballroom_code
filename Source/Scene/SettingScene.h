#pragma once
#include "Scene.h"
#include <memory>
#include "../Math/VectorInterface.h"

class SettingUI;
class Camera;
class BackGround;
class Animation;
class SettingScene :
    public Scene
{
public:
	SettingScene();
	~SettingScene() = default;

	void Update(float delta)override;

	void BeforeDraw(void)override;

	void Draw(void)override;

	void Initialize(void)override;
private:
	void StartUpdate(float delta);
	void SelectUpdate(float delta);
	void EndUpdate(float delta);
	void (SettingScene::* update_)(float delta);

	std::unique_ptr<SettingUI> ui_;

	std::unique_ptr<Camera> camera_;
	std::unique_ptr<BackGround> bg_;
	std::unique_ptr<Animation> playerAnim_;
	bool playerGoForward_;

	Vector2<float> playerPos_;

	float time_;

	int screenWidth_;

	float playerSpeed_;

	int bgmHandle_;
};

