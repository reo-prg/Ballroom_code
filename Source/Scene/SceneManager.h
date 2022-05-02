#pragma once
#include <memory>
#include <functional>

class Scene;
class SceneManager
{
public:
	SceneManager();
	~SceneManager() = default;

	void Update(float delta);

	void BeforeDraw(void);
	void Draw(void);

	std::shared_ptr<Scene> GetCurrentScene(void) { return currentScene_; };

	/// <summary>
	/// シーンを変える
	/// </summary>
	/// <param name="scene">遷移先のシーン</param>
	void ChangeScene(Scene* scene, bool callInit = true);

	void ChangeScene(const std::shared_ptr<Scene>& scene, bool callInit = true);
private:
	std::shared_ptr<Scene> currentScene_;
	std::function<void(void)> changer_;
};
