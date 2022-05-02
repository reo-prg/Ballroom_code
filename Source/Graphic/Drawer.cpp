#include "Drawer.h"
#include <algorithm>
#include "Board2D.h"
#include "Camera.h"
#include "../Camera/CameraShake.h"
#include "../Application.h"
#include "../Utility/utility.h"
#include "ShaderHandleManager.h"

Drawer* Drawer::instance_ = nullptr;

void Drawer::Initialize(void)
{
	if (instance_) { return; }
	
	instance_ = new Drawer();
	instance_->Init();
}

Drawer& Drawer::GetInstance(void)
{
	if (!instance_)
	{
		Drawer::Initialize();
	}
	return *instance_;
}

void Drawer::Terminate(void)
{
	delete instance_;
}

void Drawer::Update(float delta)
{
	cameraShake_->Update(delta);

	postParam_->time_ += delta;

	if (targetDamageStr_ < postParam_->damage_)
	{
		postParam_->damage_ -= delta;
		postParam_->damage_ = std::max(targetDamageStr_, postParam_->damage_);
	}
}

void Drawer::Draw(void)
{
	//std::sort(drawlist_.begin(), drawlist_.end(),
	//	[](const std::unique_ptr<DrawData2D>& a, const std::unique_ptr<DrawData2D>& b)
	//	{ return a->zBuffer_ > b->zBuffer_; });

	//for (auto& dl : drawlist_)
	//{
	//	if (dl->isUseFunc_)
	//	{
	//		dl->func_();
	//	}
	//	else
	//	{
	//		DrawRotaGraph(static_cast<int>(dl->posx_), static_cast<int>(dl->posy_),
	//			dl->scale_, dl->rotate_, dl->handle_, dl->trans_);
	//	}
	//}

	//drawlist_.clear();
}

void Drawer::Draw(DrawType type)
{
	std::sort(drawlist_[type].begin(), drawlist_[type].end(),
		[](const std::unique_ptr<DrawData2D>& a, const std::unique_ptr<DrawData2D>& b)
		{ return a->zBuffer_ > b->zBuffer_; });

	for (auto& dl : drawlist_[type])
	{
		if (dl->isUseFunc_)
		{
			dl->func_();
		}
		else
		{
			DrawRotaGraph(static_cast<int>(dl->posx_), static_cast<int>(dl->posy_),
				dl->scale_, dl->rotate_, dl->handle_, dl->trans_, dl->reverse_x, dl->reverse_y);
		}
	}

	drawlist_[type].clear();
}

void Drawer::StartDraw(int screenHandle)
{
	if (screenHandle == -1)
	{
		screenHandle = pass1_;
	}
	currentScreenHandle_ = screenHandle;
	SetDrawScreen(screenHandle);
	ClsDrawScreen();

	
	camera_->SetCamera(cameraShake_->GetOffset());

	auto cPos = camera_->GetCameraPosition();

	SetShadowMapDrawArea(shadowMap_, VAdd(cPos, smAreaMin_), VAdd(cPos, smAreaMax_));
}

void Drawer::EndDraw(void)
{
	SetUseShadowMap(0, -1);

	if (currentScreenHandle_ == pass1_)
	{
		SetDrawScreen(fadepass_);
		ClsDrawScreen();

		SetUseZBuffer3D(false);
		UpdateShaderConstantBuffer(postHandle_);
		SetShaderConstantBuffer(postHandle_, DX_SHADERTYPE_PIXEL, 0);
		postProcess_->Draw();

		SetDrawScreen(DX_SCREEN_BACK);
		ClsDrawScreen();

		UpdateShaderConstantBuffer(fadeHandle_);
		SetShaderConstantBuffer(fadeHandle_, DX_SHADERTYPE_PIXEL, 0);
		fadePoly_->Draw();

		ScreenFlip();

		SetUseZBuffer3D(true);
	}
}

void Drawer::DrawShadowStart(void)
{
	ShadowMap_DrawSetup(shadowMap_);
}

void Drawer::DrawWithShadow(void)
{
	ShadowMap_DrawEnd();
	SetUseShadowMap(0, shadowMap_);
}

void Drawer::SetCamera(Camera* camera)
{
	camera_ = camera;
	cameraShake_->SetCamera(camera);
}

void Drawer::StartCameraShake(const ShakeDescriptor& desc)
{
	cameraShake_->StartShake(desc);
}

void Drawer::Add2DDraw(int grHandle, float x, float y, double angle, double scale, bool trans, 
	int zBuffer, DrawType type, bool rev_x, bool rev_y)
{
	DrawData2D* data = new DrawData2D();
	data->isUseFunc_ = false;
	data->zBuffer_ = zBuffer;
	data->handle_ = grHandle;
	data->posx_ = x;
	data->posy_ = y;
	data->rotate_ = angle;
	data->scale_ = scale;
	data->trans_ = trans;
	data->reverse_x = rev_x;
	data->reverse_y = rev_y;

	switch (type)
	{
	case DrawType::Normal:
		drawlist_[DrawType::Normal].emplace_back(data);
		break;
	case DrawType::BG:
		drawlist_[DrawType::BG].emplace_back(data);
		break;
	case DrawType::UI:
		drawlist_[DrawType::UI].emplace_back(data);
		break;
	default:
		break;
	}
}

void Drawer::Add2DDraw(const std::function<void(void)>& func, int zBuffer, DrawType type)
{
	DrawData2D* data = new DrawData2D();
	data->isUseFunc_ = true;
	data->zBuffer_ = zBuffer;
	data->func_ = func;

	switch (type)
	{
	case DrawType::Normal:
		drawlist_[DrawType::Normal].emplace_back(data);
		break;
	case DrawType::BG:
		drawlist_[DrawType::BG].emplace_back(data);
		break;
	case DrawType::UI:
		drawlist_[DrawType::UI].emplace_back(data);
		break;
	default:
		break;
	}
}

void Drawer::SetDamageEffect(float strength, bool absolute)
{
	if (!postParam_) { return; }

	if (postParam_->damage_ < strength || absolute)
	{
		postParam_->damage_ = strength;
		targetDamageStr_ = strength;
	}
	else
	{
		targetDamageStr_ = strength;
	}
}

void Drawer::SetFadeMainColor(const FLOAT4& color)
{
	if (!fadeParam_) { return; }
	fadeParam_->maincolor_ = color;
}

void Drawer::SetFadeSubColor(const FLOAT4& color)
{
	if (!fadeParam_) { return; }
	fadeParam_->subcolor_ = color;
}

void Drawer::SetFadeParameter(const FLOAT4& value, int parameterIndex_)
{
	if (!fadeParam_) { return; }
	fadeParam_->param_[parameterIndex_][0] = value.x;
	fadeParam_->param_[parameterIndex_][1] = value.y;
	fadeParam_->param_[parameterIndex_][2] = value.z;
	fadeParam_->param_[parameterIndex_][3] = value.w;
}

void Drawer::SetFadeParameter(float value, int parameterIndex_, int subIndex_)
{
	if (!fadeParam_) { return; }
	fadeParam_->param_[parameterIndex_][subIndex_] = value;
}

void Drawer::SetFadeProgress(float progress)
{
	if (!fadeParam_) { return; }
	fadeParam_->progress_ = progress;
}

void Drawer::SetFadeShader(int handle)
{
	if (!fadePoly_) { return; }
	fadePoly_->SetPixelShader(handle);
}

Drawer::Drawer()
{
}

Drawer::~Drawer()
{
}

void Drawer::Init(void)
{
	ShaderHandleManager::Create();

	SetUse3DFlag(true);
	SetUseZBuffer3D(true);
	SetWriteZBuffer3D(true);

	SetUsePixelLighting(true);
	
	ChangeLightTypeDir({ 0.3f, -0.8f, 0.7f });
	SetGlobalAmbientLight({ 0.0f, 0.0f, 0.0f, 1.0f });

	SetBackgroundColor(0, 255, 0);

	camera_ = new Camera();

	drawlist_.reserve(256);

	shadowMap_ = MakeShadowMap(1024, 1024);
	SetShadowMapLightDirection(shadowMap_, { 0.3f, -0.6f, 0.8f });
	

	smAreaMin_ = { -1200.0f, -700.0f, 800.0f };
	smAreaMax_ = { 1200.0f, 300.0f, 1200.0f };
	
	cameraShake_ = std::make_unique<CameraShake>();

	auto [w, h] = Application::GetWindowSize();
	postProcess_ = std::make_unique<Board2D>(Vector2<float>{ 0.0f, 0.0f }, 
		Vector2<float>{ static_cast<float>(w), static_cast<float>(h) });

	pass1_ = MakeScreen(w, h, true);
	
	postProcess_->SetPixelShader("Resource/Shader/Board2D_ps.pso");
	postProcess_->SetTextureHandle(pass1_);

	auto si = AlignTo(sizeof(PostProcessParameter), sizeof(float) * 4);
	postHandle_ = CreateShaderConstantBuffer(si);
	postParam_ = static_cast<PostProcessParameter*>(GetBufferShaderConstantBuffer(postHandle_));

	postParam_->time_ = 0.0f;
	postParam_->damage_ = 0.0f;

	targetDamageStr_ = postParam_->damage_;



	fadepass_ = MakeScreen(w, h, true);

	fadePoly_ = std::make_unique<Board2D>(Vector2<float>{ 0.0f, 0.0f },
		Vector2<float>{ static_cast<float>(w), static_cast<float>(h) });


	int ph = ShaderMngIns.LoadPS("default_fade", "Resource/Shader/DefaultFade.pso");
	fadePoly_->SetPixelShader(ph);
	fadePoly_->SetTextureHandle(fadepass_);

	si = AlignTo(sizeof(FadeParameter), sizeof(float) * 4);
	fadeHandle_ = CreateShaderConstantBuffer(si);
	fadeParam_ = static_cast<FadeParameter*>(GetBufferShaderConstantBuffer(fadeHandle_));

	fadeParam_->maincolor_ = { 0.0f, 0.0f, 0.0f, 1.0f };
	fadeParam_->subcolor_ = { 0.75f, 0.25f, 0.15f, 1.0f };
	for (int i = 0; i < 16; i++)
	{
		fadeParam_->param_[i / 4][i % 4] = 0.0f;
	}
	fadeParam_->progress_ = 0.0f;
}
