#include "Application.h"
#include <EffekseerForDXLib.h>
#include "Collider/ColliderManager.h"
#include "Graphic/AnimationManager.h"
#include "Graphic/Drawer.h"
#include "Graphic/ImageHandleManager.h"
#include "Graphic/EffectMng.h"
#include "Graphic/EffekseerMng.h"
#include "Graphic/FontHandleManager.h"
#include "Graphic/ShaderHandleManager.h"
#include "Input/InputIntegrator.h"
#include "Scene/SceneManager.h"
#include "Audio/AudioManager.h"
#include "Audio/SubmixHandleManager.h"
#include "DebugModule.h"
#include "../resource.h"

#pragma comment(lib, "box2d.lib")

float Application::gameSpeed_ = 1.0f;
float Application::time_ = 0.0f;
float Application::absDelta_ = 0.0f;
bool Application::end_ = false;

using namespace std::chrono;

Application::Application()
{
}

Application::~Application()
{
}

bool Application::Initialize(void)
{
#ifdef _DEBUG
	SetOutApplicationLogValidFlag(true);
#else
	SetOutApplicationLogValidFlag(false);
#endif
	SetGraphMode(screenWidth, screenHeight, 32);
	ChangeWindowMode(true);
	SetMainWindowText("Ballroom");
	SetAlwaysRunFlag(true);
	SetUseDirect3DVersion(DX_DIRECT3D_11);
	SetWindowIconID(IDI_ICON1);
	SetWaitVSyncFlag(true);
	if (DxLib_Init() == -1)
	{
		return false;
	}
	SetDrawScreen(DX_SCREEN_BACK);


	EffekseerMng::Create();
	Drawer::Initialize();
	EffectMng::Create();
	AnimationManager::Create();
	ColliderManager::Create();
	ImageHandleManager::Create();
	FontHandleManager::Create();
	ShaderHandleManager::Create();
	AudioManager::Create();

	InitializeAudio();
	LoadSystemSound();
	LoadEffect();

	scenemng_ = std::make_unique<SceneManager>();


	return true;
}

void Application::Run(void)
{
	prevTime_ = steady_clock::now();
	auto&& effekseerMng = EffekseerMng::GetInstance();

	while (ProcessMessage() == 0 && CheckHitKey(KEY_INPUT_ESCAPE) == 0 && !end_)
	{
		// 経過時間関連
		float delta = GetDeltaTime();
		if (delta > 1.0f) { delta = 0.05f; }
		GameSpeedUpdate(delta);
		absDelta_ = delta;
		delta = delta * gameSpeed_;

		InputIns.Update();
		DebugModule::CheckDebugEnable();

		scenemng_->Update(delta);

		DrawerIns.Update(delta);

		scenemng_->BeforeDraw();
		DrawerIns.StartDraw();
		DrawerIns.Draw(DrawType::BG);
		scenemng_->Draw();
		EffectMng::GetInstance().Draw(delta, { 0.0f, 0.0f });
		effekseerMng.UpdateEffekseer();
		DrawerIns.Draw(DrawType::Normal);
		DrawerIns.Draw(DrawType::UI);
		
		// デバッグ中はfps表示
		if (DebugModule::IsDebugEnable())
		{
			DrawString(10, 10, std::to_string(GetFPS()).c_str(), 0xffffff);
		}

		DrawerIns.EndDraw();
	}
}

void Application::Terminate(void)
{
	EffectMng::Destroy();
	EffekseerMng::Destroy();
	DxLib_End();
}

std::pair<int, int> Application::GetWindowSize(void)
{
	return std::pair<int, int>(screenWidth, screenHeight);
}

void Application::SetGameSpeed(float speed, float time)
{
	gameSpeed_ = speed; 
	time_ = time;
	EffekseerMng::GetInstance().SetPlaySpeed(gameSpeed_);
}

float Application::GetRealDeltaTime(void)
{
	return absDelta_;
}

void Application::ResetDeltaTime(void)
{
	
}

void Application::EndGame(void)
{
	end_ = true;
}

void Application::InitializeAudio(void)
{
	auto&& audio = AudioManager::GetInstance();

	int bgm = audio.CreateSubmix();
	audio.SetVolume(bgm, 0.05f);
	
	int se = audio.CreateSubmix();
	audio.SetVolume(se, 0.05f);

	SubmixHandleMng.Add(bgm, "bgm");
	SubmixHandleMng.Add(se, "se");
	SubmixHandleMng.Add(RootSubmixHandle, "master");
}

void Application::LoadSystemSound(void)
{
	auto&& inst = AudioManager::GetInstance();

	inst.LoadSound("Resource/Sound/System/select.wav", "select");
	inst.LoadSound("Resource/Sound/System/push.wav", "button");
	inst.LoadSound("Resource/Sound/ME025-110124-clear05-wav.wav", "clear");
	inst.LoadSound("Resource/Sound/knocking_a_wall.wav", "clear_exp1");
}

void Application::LoadEffect(void)
{
	auto&& inst = EffectMng::GetInstance();

	inst.LoadEffect("fall_smoke", "Resource/Texture/Smoke/VFX 3 Seprated Frames");
	inst.LoadEffect("dash_smoke", "Resource/Texture/Smoke/VFX 1 Separated Frames");
	inst.LoadEffect("defeat_smoke", "Resource/Texture/Smoke/VFX 2 Separated Frames");
	inst.LoadEffect("clear_exp1", "Resource/Texture/Explosion/round_explosion");
	inst.LoadEffect("clear_spark1", "Resource/Texture/Explosion/vertical_explosion");
}

float Application::GetDeltaTime(void)
{
	steady_clock::time_point current = steady_clock::now();
	duration<float> ret = duration_cast<microseconds>(current - prevTime_);
	prevTime_ = std::move(current);
	return ret.count();
}

void Application::GameSpeedUpdate(float delta)
{
	if (time_ > 0.0f)
	{
		time_ -= delta;
		if (time_ <= 0.0f)
		{
			gameSpeed_ = 1.0f;
			EffekseerMng::GetInstance().SetPlaySpeed(gameSpeed_);
		}
	}
}
