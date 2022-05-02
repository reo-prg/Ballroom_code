#include "Transition.h"
#include <EffekseerForDXLib.h>
#include "../../Application.h"
#include "../../Graphic/Drawer.h"

Transition::Transition(const std::shared_ptr<Scene>& prevScene, const std::shared_ptr<Scene>& nextScene)
	: prev_(prevScene), next_(nextScene)
{
	auto [w, h] = Application::GetWindowSize();

	screenWidth_ = w;
	screenHeight_ = h;
}

Transition::~Transition()
{
	DeleteGraph(prevScreen_);
	DeleteGraph(nextScreen_);
}

void Transition::GetPrevScreen(void)
{
	prevScreen_ = MakeScreen(screenWidth_, screenHeight_);
	GetScreen(prevScreen_, prev_);
}

void Transition::GetNextScreen(void)
{
	nextScreen_ = MakeScreen(screenWidth_, screenHeight_);
	GetScreen(nextScreen_, next_);
}

void Transition::GetScreen(int handle, const std::shared_ptr<Scene>& scene)
{
	scene->BeforeDraw();
	DrawerIns.StartDraw(handle);
	DrawerIns.Draw(DrawType::BG);
	scene->Draw();
	DrawerIns.Draw(DrawType::Normal);
	DrawerIns.Draw(DrawType::UI);
	DrawerIns.EndDraw();
}
