#include "MainScene.h"
#include <EffekseerForDXLib.h>
#include <algorithm>
#include "../../RapidXML/rapidxml_utils.hpp"
#include "../MyRapidXML_Helper.h"
#include "../Actor/Enemy.h"
#include "../Actor/EnemyManager.h"
#include "../Actor/Goal.h"
#include "../Actor/ThrowObject.h"
#include "../Actor/ThrowObjManager.h"
#include "../Actor/Player.h"
#include "../Actor/ActorManager.h"
#include "../Audio/AudioManager.h"
#include "../Collider/ColliderManager.h"
#include "../Graphic/Camera.h"
#include "../Graphic/Drawer.h"
#include "../Graphic/ImageHandleManager.h"
#include "../Graphic/EffectMng.h"
#include "../Input/InputIntegrator.h"
#include "../Map/BackGround.h"
#include "../Map/Map.h"
#include "../Map/MapData.h"
#include "../Map/MapCollider.h"
#include "../Math/Random_utility.h"
#include "../Collider/Collider.h"
#include "../Defines/PhysicsDefines.h"
#include "../Defines/Unit_Defines.h"
#include "SceneManager.h"
#include "StageSelectScene.h"
#include "Transition/Fade.h"
#include "../Box2DModule/MyListener.h"
#include "../Application.h"

#include "../Actor/Enemy/WalkEnemy.h"
#include "../Actor/Enemy/FlyEnemy.h"
#include "../Actor/Enemy/FireWorm.h"
#include "../Audio/AudioManager.h"
#include "../Audio/SubmixHandleManager.h"

namespace
{
	constexpr float StartDuration = 2.0f;
	constexpr float ExpStartTime = 2.0f;
	constexpr float ClearDuration = 5.0f;
	constexpr float DeadDuration = 1.0f;
}

MainScene::MainScene(MapData* map)
	: mapmeta_(map)
{

}

MainScene::~MainScene()
{
	world_.reset();
	enemyMng_.reset();
}

void MainScene::Update(float delta)
{
	(this->*update_)(delta);
	B2DUpdate(delta);
}

void MainScene::BeforeDraw(void)
{
	if (!isDrawEnable_) {
		DrawerIns.Add2DDraw([this]() { DrawGraph(0, 0, deadScreen_, true); }, 100, DrawType::BG);
	}
	bg_->Draw();
}

void MainScene::Draw(void)
{
	if (!isDrawEnable_) 
	{ 
		return;
	}
	DrawerIns.DrawShadowStart();
	map_->Draw();
	//player_->Draw();
	//enemyMng_->Draw();
	//actorMng_->Draw();
	//throwMng_->Draw();
	goal_->Draw();
	DrawerIns.DrawWithShadow();
	map_->Draw();
	player_->Draw();
	player_->DrawSimpleShadow(map_->GetModelID());
	enemyMng_->Draw();
	enemyMng_->DrawSimpleShadow(map_->GetModelID());
	actorMng_->Draw();
	actorMng_->DrawSimpleShadow(map_->GetModelID());
	throwMng_->Draw();
	throwMng_->DrawSimpleShadow(map_->GetModelID());
	goal_->Draw();
}

void MainScene::Initialize(void)
{
	listener_ = std::make_unique<MyListener>();

	world_ = std::make_unique<b2World>(b2Vec2(0.0f, phys_def::gravityAcc / unit::m_to_cm));
	world_->SetContactListener(listener_.get());

	map_ = std::make_unique<Map>();
	map_->LoadMap(mapmeta_->modelPath_);

	mapcol_ = std::make_unique<MapCollider>(world_.get());
	mapcol_->LoadFile(mapmeta_->collisionPath_);

	camera_ = std::make_unique<Camera>();

	camera_->SetCameraPosition(400.0f, 300.0f, -800.0f);
	camera_->AddAngleDeg(0.0f, 0.0f, 0.0f);

	enemyMng_ = std::make_unique<EnemyManager>();

	actorMng_ = std::make_unique<ActorManager>();

	throwMng_ = std::make_unique<ThrowObjManager>();

	InitializeActors();

	enemyMng_->SetPlayer(&player_);
	actorMng_->SetPlayer(&player_);

	bg_ = std::make_unique<BackGround>();
	bg_->Load(mapmeta_->bgPath_);
	bg_->SetCamera(camera_.get());

	DrawerIns.SetCamera(camera_.get());

	auto [w, h] = Application::GetWindowSize();
	screenWidth_ = w;
	screenHeight_ = h;
	deadScreen_ = MakeScreen(w, h);

	ImageMngIns.Load("simple_shadow", "Resource/Texture/Shadow2.png");

	auto&& adInst = AudioManager::GetInstance();
	adInst.LoadSound(mapmeta_->bgmPath_, "game_bgm");
	adInst.LoadSound("Resource/Sound/Hit.wav", "hit");

	bgmHandle_ = adInst.PlayLoop("game_bgm", 0.0f, 0.0f, XAUDIO2_LOOP_INFINITE, 
		0.5f, true, { SubmixHandleMng.GetHandle("bgm") });

	player_->SetBGMHandle(bgmHandle_);

	adInst.LoadSound("Resource/Sound/powerdown07.wav", "dead");
	adInst.LoadSound("Resource/Sound/item.wav", "item");
	adInst.LoadSound("Resource/Sound/getball.wav", "get_ball");
	adInst.LoadSound("Resource/Sound/throwball.wav", "throw_ball");

	update_ = &MainScene::StartUpdate;
}

void MainScene::PlayerDead(void)
{
	player_->SetMoveEnable(false);
	enemyMng_->SetMoveEnable(false);
	enemyMng_->SetGameState(false);
	actorMng_->SetMoveEnable(false);

	//isPlayerDead_ = true;
	update_ = &MainScene::DeadUpdate;
	time_ = 0.0f;

	AudioIns.PlayTemporary("dead", 1.0f, { SubmixHandleMng.GetHandle("bgm") });
}

void MainScene::StageClear(void)
{
	player_->SetMoveEnable(false);
	enemyMng_->SetMoveEnable(false);
	enemyMng_->SetGameState(false);
	actorMng_->SetMoveEnable(false);
	time_ = 0.0f;
	AudioIns.Stop(bgmHandle_);
	AudioIns.PlayTemporary("clear", 1.0f, { SubmixHandleMng.GetHandle("bgm") });
	update_ = &MainScene::ClearUpdate;

	auto&& efkins = EffectMng::GetInstance();
	auto&& gPos = goal_->GetPosition();

	efkins.PlayEffect("clear_spark1", { gPos.x - 130.0f, gPos.y + 20.0f, gPos.z }, 0.0f, 3.0f, 1.0f);
	efkins.PlayEffect("clear_spark1", { gPos.x + 130.0f, gPos.y + 20.0f, gPos.z }, 0.0f, 3.0f, 1.0f);
	clearP_ = 25;
}

void MainScene::StartUpdate(float delta)
{
	GameUpdate(delta);

	time_ += delta;
	AudioIns.SetVolume(bgmHandle_, std::clamp(0.5f * (time_ / StartDuration) + 0.5f, 0.0f, 1.0f));

	if (time_ > StartDuration)
	{
		AudioIns.SetVolume(bgmHandle_, 1.0f);
		update_ = &MainScene::GameUpdate;
	}
}

void MainScene::GameUpdate(float delta)
{
	player_->Update(delta);
	enemyMng_->Update(delta);
	actorMng_->Update(delta);
	throwMng_->Update(delta);
	goal_->Update(delta);

	player_->ApplyVelocityToB2Body();

	mapcol_->CheckCollisionDetection(player_.get(), player_->GetCollider("player_body"));
	enemyMng_->CheckMapCollision(mapcol_.get());
	actorMng_->CheckMapCollision(mapcol_.get());
	ColliderMngIns.CheckCollision();

	player_->ApplyVelocity();
	enemyMng_->ApplyVelocity();
	actorMng_->ApplyVelocity();

	//if (isPlayerDead_)
	//{
	//	GetDeadScreen();
	//	//isDrawEnable_ = false;

	//	time_ = 0.0f;
	//	update_ = &MainScene::DeadFadeUpdate;
	//}
}

void MainScene::ClearUpdate(float delta)
{
	player_->Update(delta);
	enemyMng_->Update(delta);
	actorMng_->Update(delta);
	throwMng_->Update(delta);
	goal_->Update(delta);

	mapcol_->CheckCollisionDetection(player_.get(), player_->GetCollider("player_body"));
	enemyMng_->CheckMapCollision(mapcol_.get());
	actorMng_->CheckMapCollision(mapcol_.get());
	ColliderMngIns.CheckCollision();

	player_->ApplyVelocity();
	enemyMng_->ApplyVelocity();
	actorMng_->ApplyVelocity();

	if (time_ > ExpStartTime && time_ < ClearDuration - 0.5f)
	{
		cTime_ += delta;
		if (cTime_ > 0.01f)
		{
			std::uniform_int_distribution<int> rand(0, clearP_);

			if (rand(rand_util::GetSeed()) == 0)
			{
				SpawnExplosion();
				clearP_ = 25;
			}
			else
			{
				clearP_--;
			}
			cTime_ = 0.0f;
		}
	}

	time_ += delta;
	//AudioIns.SetVolume(bgmHandle_, 1.0f - time_ / ClearDuration);
	if (time_ > ClearDuration)
	{
		mng_->ChangeScene(new Fade(mng_->GetCurrentScene(), std::make_shared<StageSelectScene>(mapmeta_->stage_), 2.0f, 1.0f));
		delete mapmeta_;
		ColliderMngIns.Clear();
		//AudioIns.Stop(bgmHandle_);
		AudioIns.Unload("game_bgm");
		return;
	}

	//DrawerIns.Add2DDraw([this]() { 
	//	DrawString(10, 150, "‚±‚±‚ÉƒS[ƒ‹‰‰o‚ð“ü‚ê‚é", 0xffffff);
	//	DrawString(10, 170, ("time : " + std::to_string(time_)).c_str(), 0xffffff);
	//	}, 1800);
}

void MainScene::DeadUpdate(float delta)
{
	player_->Update(delta);
	enemyMng_->Update(delta);
	actorMng_->Update(delta);
	throwMng_->Update(delta);
	goal_->Update(delta);

	mapcol_->CheckCollisionDetection(player_.get(), player_->GetCollider("player_body"));
	enemyMng_->CheckMapCollision(mapcol_.get());
	actorMng_->CheckMapCollision(mapcol_.get());
	ColliderMngIns.CheckCollision();

	player_->ApplyVelocity();
	enemyMng_->ApplyVelocity();
	actorMng_->ApplyVelocity();

	time_ += delta;
	AudioIns.SetVolume(bgmHandle_, 1.0f - time_ / DeadDuration);
	if (time_ > DeadDuration)
	{
		mng_->ChangeScene(new Fade(mng_->GetCurrentScene(), std::make_shared<MainScene>(mapmeta_), 1.0f, 1.0f));
		ColliderMngIns.Clear();
		AudioIns.Stop(bgmHandle_);
		AudioIns.Unload("game_bgm");
		return;
	}
}

void MainScene::B2DUpdate(float delta)
{
	listener_->list_.clear();
	world_->Step(delta, 8, 3);
	for (auto& l : listener_->list_)
	{
		if (!l.bodyA_ || !l.bodyB_) { continue; }
		if (l.dataA_->owner_)
		{
			l.dataA_->owner_->B2HitEvent(l.bodyA_, l.dataA_, l.bodyB_, l.dataB_, -l.normal_);
		}
		if (l.dataB_->owner_)
		{
			l.dataB_->owner_->B2HitEvent(l.bodyB_, l.dataB_, l.bodyA_, l.dataA_, l.normal_);
		}
	}

	std::vector<b2Body*> deletebody;
	for (b2Body* body = world_->GetBodyList(); body != nullptr; body = body->GetNext())
	{
		if (!GetBodyData(body)->isValid_ && GetBodyData(body)->count_ == 0)
		{
			deletebody.push_back(body);
		}
	}
	for (auto& b : deletebody)
	{
		world_->DestroyBody(b);
	}
}

void MainScene::InitializeActors(void)
{
	using namespace rapidxml;

	xml_document<>* doc = new xml_document<>();
	file<>* fl = new file<>(mapmeta_->actorPath_.c_str());
	doc->parse<0>(fl->data());

	xml_node<>* root = doc->first_node();

	InitializePlayer(root->first_node("playerPosition_"));
	InitializeEnemy(root->first_node("enemy_"));
	InitializeGoal(root->first_node("goal_"));
	InitializeBall(root->first_node("ball_"));
}

void MainScene::InitializePlayer(rapidxml::xml_node<>* node)
{
	float x = static_cast<float>(atof(GetNodeValue(node->first_node("x")))) * unit::m_to_cm;
	float y = static_cast<float>(atof(GetNodeValue(node->first_node("y")))) * unit::m_to_cm;

	player_ = std::make_unique<Player>(VECTOR{ x, y }, world_.get(), this);
	player_->SetCamera(camera_.get());
}

void MainScene::InitializeEnemy(rapidxml::xml_node<>* node)
{
	using namespace rapidxml;

	enemyFunc_.emplace("Walk", [this](float x, float y) 
		{
			return new WalkEnemy(VECTOR{ x, y, 0.0f }, player_.get(), world_.get(), actorMng_.get());
		});
	enemyFunc_.emplace("Fly", [this](float x, float y) 
		{
			return new FlyEnemy(VECTOR{ x, y, 0.0f }, player_.get(), world_.get(), actorMng_.get());
		});
	enemyFunc_.emplace("Worm", [this](float x, float y)
		{
			return new FireWorm(VECTOR{ x, y, 0.0f }, player_.get(), world_.get(), actorMng_.get());
		});

	for (auto data = node->first_node("EnemyData"); data != nullptr;
		data = data->next_sibling("EnemyData"))
	{
		xml_node<>* pos = data->first_node("pos_");
		float x, y;
		x = static_cast<float>(atof(GetNodeValue(pos->first_node("x")))) * unit::m_to_cm;
		y = static_cast<float>(atof(GetNodeValue(pos->first_node("y")))) * unit::m_to_cm;

		std::string type = GetNodeValue(data->first_node("type_"));

		xml_node<>* drop = data->first_node("drop");

		if (enemyFunc_.contains(type))
		{
			auto&& en = enemyFunc_.at(type)(x, y);

			if (drop != nullptr)
			{
				en->LoadDropData(drop);
			}

			enemyMng_->Add(en);
		}
	}
}

void MainScene::InitializeGoal(rapidxml::xml_node<>* node)
{
	float x = static_cast<float>(atof(GetNodeValue(node->first_node("x")))) * unit::m_to_cm;
	float y = static_cast<float>(atof(GetNodeValue(node->first_node("y")))) * unit::m_to_cm;

	goal_ = std::make_unique<Goal>(VECTOR{ x, y, 0.0f }, this);
}

void MainScene::InitializeBall(rapidxml::xml_node<>* node)
{
	for (auto data = node->first_node("Vector2"); data != nullptr;
		data = data->next_sibling("Vector2"))
	{
		float x = static_cast<float>(atof(GetNodeValue(data->first_node("x")))) * unit::m_to_cm;
		float y = static_cast<float>(atof(GetNodeValue(data->first_node("y")))) * unit::m_to_cm;

		throwMng_->Add(new ThrowObject(VECTOR{ x, y, 0.0f }, 30.0f, world_.get()));
	}
}

void MainScene::SpawnExplosion(void)
{
	auto&& efkins = EffectMng::GetInstance();

	auto&& pPos = player_->GetPosition();

	constexpr float posDist = 600.0f;
	constexpr float sizeMin = 2.0f;
	constexpr float sizeMax = 5.0f;

	std::uniform_real_distribution<float> xdist(-posDist, posDist);
	std::uniform_real_distribution<float> ydist(0.0f, posDist);
	std::uniform_real_distribution<float> sc(sizeMin, sizeMax);

	efkins.PlayEffect("clear_exp1", 
		{ pPos.x + xdist(rand_util::GetSeed()), pPos.y + ydist(rand_util::GetSeed()), pPos.z + 500.0f },
		0.0f, sc(rand_util::GetSeed()), 0.8f);

	AudioIns.PlayTemporary("clear_exp1", 1.0f, { SubmixHandleMng.GetHandle("se") });
}

void MainScene::GetDeadScreen(void)
{
	MainScene::BeforeDraw();
	DrawerIns.StartDraw(deadScreen_);
	DrawerIns.Draw(DrawType::BG);
	MainScene::Draw();
	DrawerIns.Draw(DrawType::Normal);
	DrawerIns.Draw(DrawType::UI);
	DrawerIns.EndDraw();
}

void MainScene::DeadDraw(void)
{
	DrawerIns.Add2DDraw([this]() {
		SetDrawBlendMode(DX_BLENDMODE_ALPHA, static_cast<int>(100.0f * (time_ / DeadDuration)));
		DrawBox(0, 0, screenWidth_, screenHeight_, 0x000000, true);
		SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 255);
		}, 1000, DrawType::Normal);
}
