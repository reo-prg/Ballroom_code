#include "Collider.h"
#include <algorithm>
#include <vector>
#include "../Actor/ActorBase.h"
#include "../Geometry/ShapeFunc.h"

void Collider::CallHitEvent(Collider* other)
{
	if (owner_)
	{
		owner_->HitEvent(this, other);
	}
}

Collider::Collider()
	:active_(true)
{
}

Collider::Collider(bool active, const Vector2<float>& offset)
	:active_(active), localOffset_(offset)
{
}

// Circle---------------------------------------------------------------

bool CircleCollider::CheckHit(CircleCollider* other)
{
	float slen = (circle_.pos_ - other->circle_.pos_).SquareDistance();
	return slen < powf(circle_.radius_ + other->circle_.radius_, 2.0f);
}

bool CircleCollider::CheckHit(SquareCollider* other)
{
	Vector2<float> cPos = circle_.pos_;
	float cRad = circle_.radius_;
	Vector2<float> sPos = other->square_.pos_;
	Vector2<float> sSize = other->square_.size_;

	if (cPos.x >= sPos.x - cRad && cPos.x <= sPos.x + sSize.x + cRad &&
		cPos.y >= sPos.y && cPos.y <= sPos.y + sSize.y)
	{
		return true;
	}
	if (cPos.x >= sPos.x && cPos.x <= sPos.x + sSize.x &&
		cPos.y >= sPos.y - cRad && cPos.y <= sPos.y + sSize.y + cRad)
	{
		return true;
	}

	float dRad = cRad * cRad;
	return (SquareDistance(cPos, other->square_.LeftUp()) <= dRad) ||
		(SquareDistance(cPos, other->square_.RightUp()) <= dRad) ||
		(SquareDistance(cPos, other->square_.LeftDown()) <= dRad) ||
		(SquareDistance(cPos, other->square_.RightDown()) <= dRad);
}

bool CircleCollider::CheckHit(CapsuleCollider* other)
{
	return (sFunc::GetSquareDistancePointToLineSegment(circle_.pos_, other->capsule_.line_) < 
		powf(circle_.radius_ + other->capsule_.radius_, 2.0f));
}

bool CircleCollider::CheckHit(TriangleCollider* other)
{
	Vector2<float> cPos = circle_.pos_;
	float cRad = circle_.radius_;
	Vector2<float> tPos = other->triangle_.pos_;
	std::vector<Line> tLine(std::begin(other->triangle_.lines_), std::end(other->triangle_.lines_));

	bool r1, r2, r3;
	r1 = sFunc::CheckPointSide(cPos, tLine[0]);
	r2 = sFunc::CheckPointSide(cPos, tLine[1]);
	r3 = sFunc::CheckPointSide(cPos, tLine[2]);
	if ((r1 == r2) && (r2 == r3) && (r3 == r1)) { return true; }

	bool ret = false;
	for (int i = 0; i < 3 && !ret; i++)
	{
		float dist = sFunc::GetSquareDistancePointToLineSegment(cPos, tLine[i]);

		ret = dist < (cRad * cRad);
	}

	return ret;
}

std::tuple<float, Vector2<float>, Vector2<float>> CircleCollider::GetSweepCircleHitDistance(CapsuleCollider* capsule)
{
	return std::tuple<float, Vector2<float>, Vector2<float>>(NAN, Vector2<float>(), Vector2<float>());
}

// Square--------------------------------------------------------------

bool SquareCollider::CheckHit(CircleCollider* other)
{
	return other->CheckHit(this);
}

bool SquareCollider::CheckHit(SquareCollider* other)
{
	return ((square_.pos_.x < other->square_.pos_.x + other->square_.size_.x) &&
		(square_.pos_.x + square_.size_.x > other->square_.pos_.x) &&
		(square_.pos_.y < other->square_.pos_.y + other->square_.size_.y) &&
		(square_.pos_.y + square_.size_.y > other->square_.pos_.y));
}

bool SquareCollider::CheckHit(CapsuleCollider* other)
{
	bool ret = false;

	for (auto it = std::begin(square_.lines_); it != std::end(square_.lines_) && !ret; it++)
	{
		ret = sFunc::IsHitCapsuleToLineSegment(other->capsule_, *it);
		if (ret)
		{
			ret = ret;
		}
	}

	return ret;
}

bool SquareCollider::CheckHit(TriangleCollider* other)
{
	return false;
}

std::tuple<float, Vector2<float>, Vector2<float>> SquareCollider::GetSweepCircleHitDistance(CapsuleCollider* capsule)
{
	if (!CheckHit(capsule)) 
	{ 
		return std::tuple<float, Vector2<float>, Vector2<float>>(NAN, Vector2<float>(), Vector2<float>());
	}

	float dist = 1.0f;
	Vector2<float> result = capsule->capsule_.line_.EndPosition();
	Vector2<float> normal = { 0.0f, 0.0f };

	int vertc = _countof(square_.lines_);
	for (int i = 0; i < vertc; i++)
	{
		//if (Dot(square_.lines_[i].vec_.GetNormalVec(false),
		//	capsule->capsule_.line_.EndPosition() - square_.lines_[i].start_) > 0.0f)
		//{
		//	continue;
		//}
		if (sFunc::IsHitCapsuleToLineSegment(capsule->capsule_, square_.lines_[i]))
		{
			Vector2<float> res;
			float d = sFunc::GetHitPointCapsuleAndLineSegment(capsule->capsule_, square_.lines_[i],
				square_.lines_[(i + vertc - 1) % vertc], square_.lines_[(i + 1) % vertc], &res);
			if (d < dist)
			{
				dist = d;
				result = res;
				normal = square_.lines_[i].vec_.GetNormalVec().NormalizedVec();
			}
		}
	}
	//dist = std::clamp(dist, -1.0f, 1.0f);

	return std::tuple<float, Vector2<float>, Vector2<float>>(dist, result, normal);
}

// Capsule-------------------------------------------------------------

bool CapsuleCollider::CheckHit(CircleCollider* other)
{
	return other->CheckHit(this);
}

bool CapsuleCollider::CheckHit(SquareCollider* other)
{
	return other->CheckHit(this);
}

bool CapsuleCollider::CheckHit(CapsuleCollider* other)
{
	float dist;
	dist = sFunc::GetSquareDistancePointToLineSegment(capsule_.line_.start_, other->capsule_.line_);
	dist = fminf(dist, sFunc::GetSquareDistancePointToLineSegment(capsule_.line_.EndPosition(), other->capsule_.line_));
	dist = fminf(dist, sFunc::GetSquareDistancePointToLineSegment(other->capsule_.line_.start_, capsule_.line_));
	dist = fminf(dist, sFunc::GetSquareDistancePointToLineSegment(other->capsule_.line_.EndPosition(), capsule_.line_));

	return dist < powf(capsule_.radius_ + other->capsule_.radius_, 2.0f);
}

bool CapsuleCollider::CheckHit(TriangleCollider* other)
{
	bool ret = false;

	for (auto it = std::begin(other->triangle_.lines_); it != std::end(other->triangle_.lines_) && !ret; it++)
	{
		ret = sFunc::IsHitCapsuleToLineSegment(capsule_, *it);
		if (ret)
		{
			ret = ret;
		}
	}

	return ret;
}

std::tuple<float, Vector2<float>, Vector2<float>> CapsuleCollider::GetSweepCircleHitDistance(CapsuleCollider* capsule)
{
	return std::tuple<float, Vector2<float>, Vector2<float>>(NAN, Vector2<float>(), Vector2<float>());
}

// Triangle-------------------------------------------------------------

bool TriangleCollider::CheckHit(CircleCollider* other)
{
	return other->CheckHit(this);
}

bool TriangleCollider::CheckHit(SquareCollider* other)
{
	return other->CheckHit(this);
}

bool TriangleCollider::CheckHit(CapsuleCollider* other)
{
	return other->CheckHit(this);
}

bool TriangleCollider::CheckHit(TriangleCollider* other)
{
	return false;
}

std::tuple<float, Vector2<float>, Vector2<float>> TriangleCollider::GetSweepCircleHitDistance(CapsuleCollider* capsule)
{
	if (!CheckHit(capsule)) { 
		return std::tuple<float, Vector2<float>, Vector2<float>>(NAN, Vector2<float>(), Vector2<float>());
	}

	float dist = 1.0f;
	Vector2<float> result = capsule->capsule_.line_.EndPosition();
	Vector2<float> normal = { 0.0f, 0.0f };

	int vertc = _countof(triangle_.lines_);
	for (int i = 0; i < vertc; i++)
	{
		//if (Dot(triangle_.lines_[i].vec_.GetNormalVec(false), 
		//	capsule->capsule_.line_.EndPosition() - triangle_.lines_[i].start_) > 0.0f)
		//{
		//	continue;
		//}
		if (sFunc::IsHitCapsuleToLineSegment(capsule->capsule_, triangle_.lines_[i]))
		{
			Vector2<float> res;
			float d = sFunc::GetHitPointCapsuleAndLineSegment(capsule->capsule_, triangle_.lines_[i], 
				triangle_.lines_[(i + vertc - 1) % vertc], triangle_.lines_[(i + 1) % vertc], &res);
			if (d < dist)
			{
				dist = d;
				result = res;
				normal = triangle_.lines_[i].vec_.GetNormalVec().NormalizedVec();
			}
		}
	}

	if (abs(dist) > 100.0f)
	{
		int a = 0;
	}

	//dist = std::clamp(dist, -1.0f, 1.0f);

	return std::tuple<float, Vector2<float>, Vector2<float>>(dist, result, normal);
}
