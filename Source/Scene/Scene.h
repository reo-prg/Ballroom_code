#pragma once

class SceneManager;
class Scene
{
public:
	Scene();
	virtual ~Scene() = default;

	virtual void Update(float delta) = 0;

	virtual void BeforeDraw(void) {};

	virtual void Draw(void) {};

	virtual void Initialize(void) {};
	/// <summary>
	/// SceneManager‚ðƒZƒbƒg‚·‚é
	/// </summary>
	void SetManager(SceneManager* mng);
protected:
	SceneManager* mng_;
};

