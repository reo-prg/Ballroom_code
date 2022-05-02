#include "ActorBase.h"
#include <algorithm>
#include <box2d.h>
#include "../Collider/Collider.h"
#include "../Collider/ColliderManager.h"
#include "../Defines/Unit_Defines.h"
#include "../Graphic/Animation.h"
#include "../Graphic/BoardPolygon.h"
#include "../Graphic/ImageHandleManager.h"
#include "../Graphic/ShaderHandleManager.h"
#include "../Math/MathUtility.h"
#include "../MyB2DHelper.h"

ActorBase::ActorBase()
	: pos_(), velocity_()
{
	Initialize();
}

ActorBase::ActorBase(const VECTOR& pos)
	:pos_(pos), world_(nullptr)
{
	Initialize();
}

ActorBase::ActorBase(const VECTOR& pos, b2World* world)
	:pos_(pos), world_(world)
{
	Initialize();
}

ActorBase::~ActorBase()
{
	for (auto& c : colliders_)
	{
		ColliderMngIns.RemoveCollider(c);
		delete c;
	}
	colliders_.clear();
}

void ActorBase::Update(float delta)
{
}

void ActorBase::Draw(void)
{
	
}

void ActorBase::DrawSimpleShadow(int stagemodel)
{
	int i;
	MV1_COLL_RESULT_POLY_DIM HitResDim;
	MV1_COLL_RESULT_POLY* HitRes;
	VERTEX3D Vertex[3];
	VECTOR SlideVec;

	//// ライティングを無効にする
	SetUseLighting(FALSE);

	//// テクスチャアドレスモードを CLAMP にする( テクスチャの端より先は端のドットが延々続く )
	SetTextureAddressMode(DX_TEXADDRESS_BORDER);

	float shadowHeight = shadowSize_ * 5.0f;

	VECTOR sPos = pos_;
	sPos.z = sPos.z + shadowSize_ * 0.1f;

	// プレイヤーの直下に存在する地面のポリゴンを取得
	HitResDim = MV1CollCheck_Capsule(stagemodel, -1, sPos, 
		VAdd(sPos, VGet(0.0f, -shadowHeight, 0.0f)), shadowSize_);

	//DrawCapsule3D(pos_, VAdd(pos_, VGet(0.0f, -shadowHeight, 0.0f)), shadowSize_, 8, 0xff0000, 0x000000, false);

	// 頂点データで変化が無い部分をセット
	Vertex[0].dif = GetColorU8(255, 255, 255, 255);
	Vertex[0].spc = GetColorU8(0, 0, 0, 0);
	Vertex[0].su = 0.0f;
	Vertex[0].sv = 0.0f;
	Vertex[1] = Vertex[0];
	Vertex[2] = Vertex[0];

	// 球の直下に存在するポリゴンの数だけ繰り返し
	HitRes = HitResDim.Dim;
	for (i = 0; i < HitResDim.HitNum; i++, HitRes++)
	{
		if (VDot(HitRes->Normal, VECTOR{ 0.0f, 1.0f, 0.0f }) <= 0.0f)
		{
			continue;
		}

		// ポリゴンの座標は地面ポリゴンの座標
		Vertex[0].pos = HitRes->Position[0];
		Vertex[1].pos = HitRes->Position[1];
		Vertex[2].pos = HitRes->Position[2];

		// ちょっと持ち上げて重ならないようにする
		SlideVec = VScale(HitRes->Normal, 0.5f);
		Vertex[0].pos = VAdd(Vertex[0].pos, SlideVec);
		Vertex[1].pos = VAdd(Vertex[1].pos, SlideVec);
		Vertex[2].pos = VAdd(Vertex[2].pos, SlideVec);

		// ポリゴンの不透明度を設定する
		Vertex[0].dif.a = 0;
		Vertex[1].dif.a = 0;
		Vertex[2].dif.a = 0;
		float size[3] = { shadowSize_, shadowSize_, shadowSize_ };

		if (HitRes->Position[0].y > sPos.y - shadowHeight)
		{
			Vertex[0].dif.a += static_cast<BYTE>(255.0f * (1.0f - fabs(HitRes->Position[0].y - sPos.y) / shadowHeight));
			//size[0] = (1.0f - Saturate(fabs(HitRes->Position[0].y - sPos.y) / shadowHeight)) * shadowSize_;
		}
		if (HitRes->Position[1].y > sPos.y - shadowHeight)
		{
			Vertex[1].dif.a += static_cast<BYTE>(255.0f * (1.0f - fabs(HitRes->Position[1].y - sPos.y) / shadowHeight));
			//size[1] = (1.0f - Saturate(fabs(HitRes->Position[1].y - sPos.y) / shadowHeight)) * shadowSize_;
		}
		if (HitRes->Position[2].y > sPos.y - shadowHeight)
		{
			Vertex[2].dif.a += static_cast<BYTE>(255.0f * (1.0f - fabs(HitRes->Position[2].y - sPos.y) / shadowHeight));
			//size[2] = (1.0f - Saturate(fabs(HitRes->Position[2].y - sPos.y) / shadowHeight)) * shadowSize_;
		}

		// ＵＶ値は地面ポリゴンとプレイヤーの相対座標から割り出す

		Vertex[0].u = (HitRes->Position[0].x - sPos.x) / (shadowSize_ * 2.0f) + 0.5f;
		Vertex[0].v = (HitRes->Position[0].z - sPos.z) / (shadowSize_ * 2.0f) + 0.5f;
		Vertex[1].u = (HitRes->Position[1].x - sPos.x) / (shadowSize_ * 2.0f) + 0.5f;
		Vertex[1].v = (HitRes->Position[1].z - sPos.z) / (shadowSize_ * 2.0f) + 0.5f;
		Vertex[2].u = (HitRes->Position[2].x - sPos.x) / (shadowSize_ * 2.0f) + 0.5f;
		Vertex[2].v = (HitRes->Position[2].z - sPos.z) / (shadowSize_ * 2.0f) + 0.5f;

		// 影ポリゴンを描画
		SetMaterialParam(matparam_);
		DrawPolygon3D(Vertex, 1, ImageMngIns.Get("simple_shadow")[0], true);
	}

	// 検出した地面ポリゴン情報の後始末
	MV1CollResultPolyDimTerminate(HitResDim);

	// ライティングを有効にする
	SetUseLighting(TRUE);
}

void ActorBase::SetPosition(const VECTOR& pos)
{
	pos_ = pos;
	ApplyPositionToB2Body();
}

void ActorBase::SetPosition(const Vector2<float>& pos)
{
	pos_.x = pos.x;
	pos_.y = pos.y;
	ApplyPositionToB2Body();
}

void ActorBase::SetDepth(float pos)
{
	pos_.z = pos;
	poly_->SetZ(pos);
}

void ActorBase::AddPosition(const Vector2<float>& pos)
{
	pos_.x += pos.x;
	pos_.y += pos.y;
	ApplyPositionToB2Body();
}

void ActorBase::ApplyVelocity(void)
{
	pos_ = VAdd(pos_, velocity_);
	ApplyPositionToB2Body();
	if (poly_)
	{
		poly_->SetPosition(pos_);
	}
}

void ActorBase::SetVelocity(VECTOR vel)
{
	velocity_ = vel;
}

Collider* ActorBase::GetCollider(const std::string& tag)
{
	auto it = std::find_if(colliders_.begin(), colliders_.end(),
		[&tag](Collider* col) { return col->tag_ == tag; });
	if (it == colliders_.end())
	{
		return nullptr;
	}
	else
	{
		return *it;
	}
}

bool ActorBase::IsDead(void)
{
	return health_ <= 0;
}

bool ActorBase::IsInvincible(void)
{
	return invincibleTimer_ > 0.0f;
}

void ActorBase::DeleteB2Body(void)
{
	for (auto& b : b2bodys_)
	{
		GetBodyData(b)->isValid_ = false;
		b->SetEnabled(false);
	}
	b2bodys_.clear();
}

void ActorBase::SetDirection(Direction dir)
{
	if (dir_ == dir) { return; }
	dir_ = dir;
	//float xscale = size_;
	bool isback = false;
	if ((rightIsPositiveDir_ && dir_ == Direction::Left) || 
		(!rightIsPositiveDir_ && dir == Direction::Right))
	{
		isback = true;
		//xscale *= -1.0f;
	}
	poly_->TextureInvertX(isback);

	//MV1SetScale(model_, { xscale, size_, size_ });

	//int mNum = MV1GetMeshNum(model_);

	//for (int i = 0; i < mNum; i++)
	//{
	//	// 背面カリングを元々行わない設定のメッシュは設定を変更しない
	//	if (MV1GetMeshBackCulling(model_, i) != DX_CULLING_NONE)
	//	{
	//		// 背面カリングの設定をデフォルトの DX_CULLING_LEFT とは逆の DX_CULLING_RIGHT を設定
	//		MV1SetMeshBackCulling(model_, i, isback ? DX_CULLING_RIGHT : DX_CULLING_LEFT);
	//	}
	//}
}

void ActorBase::InvDirection(void)
{
	SetDirection(static_cast<Direction>(1 - static_cast<int>(dir_)));
}

void ActorBase::AddCircleBody(float size, const std::string& tag, const Vector2<float>& offset, bool isBody)
{
	b2BodyDef bDef;
	bDef.position.Set(pos_.x / unit::m_to_cm, pos_.y / unit::m_to_cm);
	bDef.gravityScale = 0.0f;

	MyUserData* data = new MyUserData();
	data->owner_ = this;
	data->tag_ = tag;
	SetBodyData(&bDef, data);

	b2Body* body = world_->CreateBody(&bDef);

	b2CircleShape bc;
	bc.m_p.Set(offset.x / unit::m_to_cm, offset.y / unit::m_to_cm);
	bc.m_radius = size / unit::m_to_cm;

	b2FixtureDef fdef;
	fdef.shape = &bc;
	fdef.density = 2.0f;
	fdef.friction = 0.0f;

	body->CreateFixture(&fdef);

	b2bodys_.push_back(body);
	if (isBody)
	{
		bodyObject_ = body;
	}
}

void ActorBase::ApplyPositionToB2Body(void)
{
	for (auto& b : b2bodys_)
	{
		b->SetTransform({ pos_.x / unit::m_to_cm, pos_.y / unit::m_to_cm }, 0.0f);
	}
}

void ActorBase::ChangeState(const std::string& state)
{
	state_.at(state).func_();
	anim_->ChangeState(state_.at(state).anim_);
	currentState_ = state;
}

void ActorBase::InvincibleTimerUpdate(float delta)
{
	invincibleTimer_ = fminf(invincibleTimer_ - delta, 0.0f);
}

void ActorBase::ApplyVelocityToB2Body(void)
{
	for (auto& b : b2bodys_)
	{
		b->SetLinearVelocity({ velocity_.x / unit::m_to_cm, velocity_.y / unit::m_to_cm });
	}
}

void ActorBase::Initialize(void)
{
	ps_ = ShaderMngIns.LoadPS("Resource/Shader/Board_ps.pso");

	poly_ = std::make_unique<BoardPolygon>();
	poly_->SetPixelShader(ps_);

	matparam_.Ambient = { 1.0f, 1.0f, 1.0f, 1.0f };
	matparam_.Diffuse = { 1.0f, 1.0f, 1.0f, 1.0f };
	matparam_.Emissive = { 0.0f, 0.0f, 0.0f, 0.0f };
	matparam_.Specular = { 0.0f, 0.0f, 0.0f, 0.0f };
	matparam_.Power = 0.0f;
}
