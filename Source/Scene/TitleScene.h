#pragma once
#include "Scene.h"
#include <memory>
#include "../Math/VectorInterface.h"

class TitleUI;
class ExitUI;
class Camera;
class BackGround;
class Animation;
class TitleScene :
	public Scene
{
public:
	TitleScene() = delete;
	TitleScene(bool isBackFromAnotherScene);
	~TitleScene();

	void Update(float delta)override;

	void BeforeDraw(void)override;

	void Draw(void)override;

	void Initialize(void)override;

	void ChangeScene(const std::shared_ptr<Scene>& scene);
private:
	void StartUpdate(float delta);
	void BackUpdate(float delta);
	void SelectUpdate(float delta);
	void ExitSelectUpdate(float delta);
	void EndUpdate(float delta);
	void (TitleScene::*update_)(float delta);

	std::unique_ptr<TitleUI> ui_;
	std::unique_ptr<ExitUI> exitUI_;
	
	std::unique_ptr<Camera> camera_;
	std::unique_ptr<BackGround> bg_;
	std::unique_ptr<Animation> playerAnim_;
	bool playerGoForward_;

	Vector2<float> playerPos_;

	float bgSpeed_;

	float time_;

	int screenWidth_;

	bool isBack_;

	float playerSpeed_;

	int bgmHandle_;

	bool isExiting_ = false;
};

