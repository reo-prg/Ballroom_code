#include "SceneManager.h"
#include "Scene.h"
#include "MainScene.h"
#include "TitleScene.h"

SceneManager::SceneManager()
{
	currentScene_.reset(new TitleScene(false));
	currentScene_->SetManager(this);
	currentScene_->Initialize();

	changer_ = [](){};
}

void SceneManager::Update(float delta)
{
	currentScene_->Update(delta);
	changer_();
	changer_ = []() {};
}

void SceneManager::BeforeDraw(void)
{
	currentScene_->BeforeDraw();
}

void SceneManager::Draw(void)
{
	currentScene_->Draw();
}

void SceneManager::ChangeScene(Scene* scene, bool callInit)
{
	changer_ = [scene, callInit, this]()
	{
		currentScene_.reset(scene);
		currentScene_->SetManager(this);
		if (callInit)
		{
			currentScene_->Initialize();
		}
	};
}

void SceneManager::ChangeScene(const std::shared_ptr<Scene>& scene, bool callInit)
{
	changer_ = [scene, callInit, this]()
	{
		currentScene_ = scene;
		currentScene_->SetManager(this);
		if (callInit)
		{
			currentScene_->Initialize();
		}
	};
}
