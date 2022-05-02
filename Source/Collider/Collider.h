#pragma once
#include <string>
#include "../Geometry/Shapes.h"
#include "../Math/VectorInterface.h"

struct CircleCollider;
struct SquareCollider;
struct CapsuleCollider;
struct TriangleCollider;
class ActorBase;

enum class ColliderType
{
	Circle,
	Square,
	Capsule,
	Triangle,
	None,
};

struct Collider
{
	/// <summary>”»’è‚ª—LŒø‚©</summary>
	bool active_;

	bool delete_ = false;

	ActorBase* owner_ = nullptr;

	std::string tag_;

	Vector2<float> localOffset_;

	virtual bool isHit(Collider* other) = 0;

	virtual bool CheckHit(CircleCollider* other) = 0;
	virtual bool CheckHit(SquareCollider* other) = 0;
	virtual bool CheckHit(CapsuleCollider* other) = 0;
	virtual bool CheckHit(TriangleCollider* other) = 0;

	virtual std::tuple<float, Vector2<float>, Vector2<float>> 
		GetSweepCircleHitDistance(CapsuleCollider* capsule) = 0;

	virtual void SetPosition(const Vector2<float>& pos) = 0;
	virtual Vector2<float> GetPosition(void) = 0;

	ColliderType GetType(void)const { return type_; }

	void CallHitEvent(Collider* other);

	void SetLocalOffset(const Vector2<float>& offset)
	{
		localOffset_ = offset;
		ApplyLocalOffset();
	}

	Collider();
	Collider(bool active, const Vector2<float>& offset);
	virtual ~Collider() = default;

protected:
	ColliderType type_ = ColliderType::None;

	virtual void ApplyLocalOffset(void) = 0;
};


struct CircleCollider :
	public Collider
{
	Circle circle_;

	bool isHit(Collider* other)override { return other->CheckHit(this); };

	bool CheckHit(CircleCollider* other)override;
	bool CheckHit(SquareCollider* other)override;
	bool CheckHit(CapsuleCollider* other)override;
	bool CheckHit(TriangleCollider* other)override;

	std::tuple<float, Vector2<float>, Vector2<float>> GetSweepCircleHitDistance(CapsuleCollider* capsule)override;

	void SetPosition(const Vector2<float>& pos)override
	{
		circle_.pos_ = pos;
		ApplyLocalOffset();
	};

	Vector2<float> GetPosition(void)override
	{
		return circle_.pos_;
	};

	CircleCollider() 
		:circle_() 
	{
		type_ = ColliderType::Circle;
	};

	CircleCollider(const Circle& circle, bool active = true, const Vector2<float>& offset = Vector2<float>::ZeroVector())
		:circle_(circle), Collider(active, offset) 
	{
		type_ = ColliderType::Circle;
		ApplyLocalOffset();
	};


	~CircleCollider() = default;
private:
	void ApplyLocalOffset(void)override
	{
		circle_.pos_ = circle_.pos_ + localOffset_;
	}
};


struct SquareCollider :
	public Collider
{
	Square square_;

	bool isHit(Collider* other)override { return other->CheckHit(this); };

	bool CheckHit(CircleCollider* other)override;
	bool CheckHit(SquareCollider* other)override;
	bool CheckHit(CapsuleCollider* other)override;
	bool CheckHit(TriangleCollider* other)override;

	std::tuple<float, Vector2<float>, Vector2<float>> GetSweepCircleHitDistance(CapsuleCollider* capsule)override;

	void SetPosition(const Vector2<float>& pos)override
	{
		square_.pos_ = pos;
		ApplyLocalOffset();
	};

	Vector2<float> GetPosition(void)override
	{
		return square_.pos_;
	};

	SquareCollider()
		:square_() 
	{
		type_ = ColliderType::Square;
	};

	SquareCollider(const Square& square, bool active = true, const Vector2<float>& offset = Vector2<float>::ZeroVector())
		:square_(square), Collider(active, offset) 
	{
		type_ = ColliderType::Square;
		ApplyLocalOffset();
	};


	~SquareCollider() = default;
private:
	void ApplyLocalOffset(void)override
	{
		square_.pos_ = square_.pos_ + localOffset_;
	}
};


struct CapsuleCollider :
	public Collider
{
	Capsule capsule_;

	bool isHit(Collider* other)override { return other->CheckHit(this); };

	bool CheckHit(CircleCollider* other)override;
	bool CheckHit(SquareCollider* other)override;
	bool CheckHit(CapsuleCollider* other)override;
	bool CheckHit(TriangleCollider* other)override; 

	std::tuple<float, Vector2<float>, Vector2<float>> GetSweepCircleHitDistance(CapsuleCollider* capsule)override;

	void SetPosition(const Vector2<float>& pos)override
	{
		capsule_.line_.start_ = pos;
		ApplyLocalOffset();
	};

	Vector2<float> GetPosition(void)override
	{
		return capsule_.line_.start_;
	};

	CapsuleCollider()
		:capsule_() 
	{
		type_ = ColliderType::Capsule;
	};

	CapsuleCollider(const Capsule& capsule, bool active = true, const Vector2<float>& offset = Vector2<float>::ZeroVector())
		:capsule_(capsule), Collider(active, offset) 
	{
		type_ = ColliderType::Capsule;
	};


	~CapsuleCollider() = default;
private:
	void ApplyLocalOffset(void)override
	{
		capsule_.line_.start_ = capsule_.line_.start_ + localOffset_;
	}
};


struct TriangleCollider :
	public Collider
{
	Triangle triangle_;

	bool isHit(Collider* other)override { return other->CheckHit(this); };

	bool CheckHit(CircleCollider* other)override;
	bool CheckHit(SquareCollider* other)override;
	bool CheckHit(CapsuleCollider* other)override;
	bool CheckHit(TriangleCollider* other)override;

	std::tuple<float, Vector2<float>, Vector2<float>> GetSweepCircleHitDistance(CapsuleCollider* capsule)override;

	void SetPosition(const Vector2<float>& pos)override
	{
		triangle_.pos_ = pos;
		ApplyLocalOffset();
	};

	Vector2<float> GetPosition(void)override
	{
		return triangle_.pos_;
	};

	TriangleCollider()
		:triangle_() 
	{
		type_ = ColliderType::Triangle;
	};

	TriangleCollider(const Triangle& triangle, bool active = true, const Vector2<float>& offset = Vector2<float>::ZeroVector())
		:triangle_(triangle), Collider(active, offset) 
	{
		type_ = ColliderType::Triangle;
	};


	~TriangleCollider() = default;
private:
	void ApplyLocalOffset(void)override
	{
		triangle_.pos_ = triangle_.pos_ + localOffset_;
	}
};
