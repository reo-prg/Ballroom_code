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
	/// �ۉe��`�悷��
	/// </summary>
	/// <param name="stagemodel">�}�b�v�̃��f���̃n���h��</param>
	void DrawSimpleShadow(int stagemodel);

	virtual void SetPosition(const VECTOR& pos);
	virtual void SetPosition(const Vector2<float>& pos);

	/// <summary>
	/// ���s���̈ʒu(Z)�̐ݒ�
	/// </summary>
	virtual void SetDepth(float pos);

	virtual void AddPosition(const Vector2<float>& pos);

	VECTOR GetPosition(void)const { return pos_; }

	/// <summary>
	/// �����蔻�肪�����������ɌĂяo�����
	/// </summary>
	/// <param name="myCol">�����̔���</param>
	/// <param name="other">����̔���</param>
	virtual void HitEvent(Collider* myCol, Collider* other) {};
	
	/// <summary>
	/// Box2D�̔��肪�Փ˂������ɌĂяo�����
	/// </summary>
	/// <param name="mybody">�����̔���</param>
	/// <param name="myData">�����̔���̃f�[�^</param>
	/// <param name="otherbody">����̔���</param>
	/// <param name="otherData">����̔���̃f�[�^</param>
	/// <param name="hitNormal">���������ʂ̖@�������̃x�N�g��</param>
	virtual void B2HitEvent(b2Body* mybody, MyUserData* myData,
		b2Body* otherbody, MyUserData* otherData, const b2Vec2& hitNormal) {};

	/// <summary>
	/// �_���[�W��^����
	/// </summary>
	/// <param name="damage">�^����_���[�W��</param>
	virtual void ApplyDamage(int damage) {};

	/// <summary>
	/// �ړ���K�p����
	/// </summary>
	virtual void ApplyVelocity(void);

	/// <summary>
	/// ���x�̎擾
	/// </summary>
	VECTOR GetVelocity(void)const { return velocity_; };

	/// <summary>
	/// ���x�̐ݒ�
	/// </summary>
	virtual void SetVelocity(VECTOR vel);

	/// <summary>
	/// �����蔻��̎擾
	/// </summary>
	/// <param name="tag">�T����������̃^�O</param>
	/// <returns>����</returns>
	Collider* GetCollider(const std::string& tag);

	/// <summary>
	/// �������x�̃��Z�b�g
	/// </summary>
	virtual void ResetFallSpeed(void) { currentFallSpeed_ = 0.0f; }
	void SetJump(bool flag) { canJump_ = flag; }

	bool CanJump(void) { return canJump_; }

	/// <summary>
	/// �ړ��ł���悤�ɂ��邩
	/// </summary>
	virtual void SetMoveEnable(bool flag) { canMove_ = flag; }

	/// <summary>
	/// ���x��Box2D�̔���ɓK�p����
	/// </summary>
	/// <param name=""></param>
	void ApplyVelocityToB2Body(void);

	void SetHitFloor(bool flag) { isHitFloor_ = flag; }
	void SetHitMap(bool flag) { isHitMap_ = flag; }

	/// <summary>
	/// ���S���Ă��邩
	/// </summary>
	bool IsDead(void);

	/// <summary>
	/// ���G��
	/// </summary>
	bool IsInvincible(void);

	/// <summary>
	/// Box2D�̔�����폜����
	/// </summary>
	void DeleteB2Body(void);

	/// <summary>
	/// �폜����Ă��邩
	/// </summary>
	bool IsDeleted(void) { return isDeleted_; }

	/// <summary>
	/// �}�b�v�Ƃ̔��肪�L����
	/// </summary>
	bool IsMapCollisionEnable(void) { return isCheckMapCollision_; }
protected:
	void SetDirection(Direction dir);
	void InvDirection(void);

	/// <summary>
	/// Box2D�̉~�̔���̒ǉ�
	/// </summary>
	/// <param name="size">���a</param>
	/// <param name="tag">�^�O</param>
	/// <param name="offset">���W����̃I�t�Z�b�g�ʒu</param>
	/// <param name="isBody">�̂̔��肩</param>
	void AddCircleBody(float size, const std::string& tag, const Vector2<float>& offset = { 0.0f, 0.0f }, bool isBody = true);
	void ApplyPositionToB2Body(void);

	void ChangeState(const std::string& state);

	/// <summary>
	/// ���G���Ԃ̍X�V
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