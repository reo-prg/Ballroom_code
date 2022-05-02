#pragma once
#include <EffekseerForDXLib.h>
#include <functional>
#include <memory>
#include <string>
#include <unordered_map>
#include <vector>
#include <b2_body.h>
#include "../Math/VectorInterface.h"
#include "../MyB2DHelper.h"

enum class Direction
{
	Left,
	Right,
};

struct Collider;
class BoardPolygon;
class Animation;
class MyListener;
class ActorBase
{
public:
	ActorBase();
	ActorBase(const VECTOR& pos);
	ActorBase(const VECTOR& pos, b2World* world);
	virtual ~ActorBase();

	virtual void Update(float delta);
	virtual void Draw(void);

	/// <summary>
	/// 丸影を描画する
	/// </summary>
	/// <param name="stagemodel">マップのモデルのハンドル</param>
	void DrawSimpleShadow(int stagemodel);

	virtual void SetPosition(const VECTOR& pos);
	virtual void SetPosition(const Vector2<float>& pos);

	/// <summary>
	/// 奥行きの位置(Z)の設定
	/// </summary>
	virtual void SetDepth(float pos);

	virtual void AddPosition(const Vector2<float>& pos);

	VECTOR GetPosition(void)const { return pos_; }

	/// <summary>
	/// 当たり判定が当たった時に呼び出される
	/// </summary>
	/// <param name="myCol">自分の判定</param>
	/// <param name="other">相手の判定</param>
	virtual void HitEvent(Collider* myCol, Collider* other) {};
	
	/// <summary>
	/// Box2Dの判定が衝突した時に呼び出される
	/// </summary>
	/// <param name="mybody">自分の判定</param>
	/// <param name="myData">自分の判定のデータ</param>
	/// <param name="otherbody">相手の判定</param>
	/// <param name="otherData">相手の判定のデータ</param>
	/// <param name="hitNormal">当たった面の法線方向のベクトル</param>
	virtual void B2HitEvent(b2Body* mybody, MyUserData* myData,
		b2Body* otherbody, MyUserData* otherData, const b2Vec2& hitNormal) {};

	/// <summary>
	/// ダメージを与える
	/// </summary>
	/// <param name="damage">与えるダメージ量</param>
	virtual void ApplyDamage(int damage) {};

	/// <summary>
	/// 移動を適用する
	/// </summary>
	virtual void ApplyVelocity(void);

	/// <summary>
	/// 速度の取得
	/// </summary>
	VECTOR GetVelocity(void)const { return velocity_; };

	/// <summary>
	/// 速度の設定
	/// </summary>
	virtual void SetVelocity(VECTOR vel);

	/// <summary>
	/// 当たり判定の取得
	/// </summary>
	/// <param name="tag">探したい判定のタグ</param>
	/// <returns>判定</returns>
	Collider* GetCollider(const std::string& tag);

	/// <summary>
	/// 落下速度のリセット
	/// </summary>
	virtual void ResetFallSpeed(void) { currentFallSpeed_ = 0.0f; }
	void SetJump(bool flag) { canJump_ = flag; }

	bool CanJump(void) { return canJump_; }

	/// <summary>
	/// 移動できるようにするか
	/// </summary>
	virtual void SetMoveEnable(bool flag) { canMove_ = flag; }

	/// <summary>
	/// 速度をBox2Dの判定に適用する
	/// </summary>
	/// <param name=""></param>
	void ApplyVelocityToB2Body(void);

	void SetHitFloor(bool flag) { isHitFloor_ = flag; }
	void SetHitMap(bool flag) { isHitMap_ = flag; }

	/// <summary>
	/// 死亡しているか
	/// </summary>
	bool IsDead(void);

	/// <summary>
	/// 無敵か
	/// </summary>
	bool IsInvincible(void);

	/// <summary>
	/// Box2Dの判定を削除する
	/// </summary>
	void DeleteB2Body(void);

	/// <summary>
	/// 削除されているか
	/// </summary>
	bool IsDeleted(void) { return isDeleted_; }

	/// <summary>
	/// マップとの判定が有効か
	/// </summary>
	bool IsMapCollisionEnable(void) { return isCheckMapCollision_; }
protected:
	void SetDirection(Direction dir);
	void InvDirection(void);

	/// <summary>
	/// Box2Dの円の判定の追加
	/// </summary>
	/// <param name="size">半径</param>
	/// <param name="tag">タグ</param>
	/// <param name="offset">座標からのオフセット位置</param>
	/// <param name="isBody">体の判定か</param>
	void AddCircleBody(float size, const std::string& tag, const Vector2<float>& offset = { 0.0f, 0.0f }, bool isBody = true);
	void ApplyPositionToB2Body(void);

	void ChangeState(const std::string& state);

	/// <summary>
	/// 無敵時間の更新
	/// </summary>
	/// <param name="delta"></param>
	virtual void InvincibleTimerUpdate(float delta);

	struct StateAnimPair
	{
		std::string anim_;
		std::function<void(void)> func_;
	};

	std::unordered_map<std::string, StateAnimPair> state_;
	std::unique_ptr<Animation> anim_;
	std::string currentState_;

	VECTOR pos_;
	VECTOR velocity_;
	bool canJump_;

	int model_;
	float size_ = 64.0f;
	VECTOR angle_;
	bool rightIsPositiveDir_ = true;

	std::vector<Collider*> colliders_;

	float currentFallSpeed_ = 0.0f;
	Direction dir_ = Direction::Right;

	std::unique_ptr<BoardPolygon> poly_;

	std::vector<b2Body*> b2bodys_;
	b2Body* bodyObject_;
	int ps_;

	bool canMove_ = true;
	bool isHitFloor_ = true;

	b2World* world_;

	int maxHealth_;
	int health_;

	float invincibleTimer_ = 0.0f;

	bool isDeleted_ = false;
	bool isHitMap_ = false;

	bool isCheckMapCollision_ = true;

	float shadowSize_ = 50.0f;
private:
	void Initialize(void);

	MATERIALPARAM matparam_;
};