#pragma once
#include "../Scene.h"
#include <memory>

class Transition :
    public Scene
{
public:
	Transition() = delete;
	Transition(const std::shared_ptr<Scene>& prevScene, const std::shared_ptr<Scene>& nextScene);
	virtual ~Transition();

	virtual void Update(float delta) = 0;

	virtual void Draw(void) = 0;

	virtual void Initialize(void) = 0;
protected:
	void GetPrevScreen(void);
	void GetNextScreen(void);

	std::shared_ptr<Scene> prev_;
	std::shared_ptr<Scene> next_;

	int screenWidth_;
	int screenHeight_;

	int prevScreen_ = -1;
	int nextScreen_ = -1;

	void GetScreen(int handle, const std::shared_ptr<Scene>& scene);
private:
};

