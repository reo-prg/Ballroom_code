#pragma once
#include "Shapes.h"

namespace sFunc
{
	float GetDistancePointToLineSegment(const Vector2<float>& point, const Line& line);
	float GetDistancePointToLineSegment(const Vector2<float>& point, 
		const Vector2<float>& lineStart, const Vector2<float>& lineEnd);

	float GetSquareDistancePointToLineSegment(const Vector2<float>& point, const Line& line);
	float GetSquareDistancePointToLineSegment(const Vector2<float>& point,
		const Vector2<float>& lineStart, const Vector2<float>& lineEnd);

	float GetProgressPointToLineSegment(const Vector2<float>& point, const Line& line);
	float GetProgressPointToLineSegment(const Vector2<float>& point,
		const Vector2<float>& lineStart, const Vector2<float>& lineEnd);

	bool CheckPointSide(const Vector2<float>& point, const Line& line);
	bool CheckPointSide(const Vector2<float>& point, 
		const Vector2<float>& lineStart, const Vector2<float>& lineEnd);

	bool GetLineCrossPoint(const Line& line1, const Line& line2,
		float* line1CrossPoint = nullptr, float* line2CrossPoint = nullptr);
	bool GetLineCrossPoint(const Vector2<float>& lineStart1, const Vector2<float>& lineEnd1,
		const Vector2<float>& lineStart2, const Vector2<float>& lineEnd2,
		float* line1CrossPoint = nullptr, float* line2CrossPoint = nullptr);

	bool IsHitCapsuleToLineSegment(const Capsule& capsule, const Line& line);
	bool IsHitCapsuleToLineSegment(const Capsule& capsule,
		const Vector2<float>& lineStart, const Vector2<float>& lineEnd);

	float GetHitPointCapsuleAndLineSegment(const Capsule& capsule, 
		const Line& line, Vector2<float>* sweepResult = nullptr);
	float GetHitPointCapsuleAndLineSegment(const Capsule& capsule,
		const Vector2<float>& lineStart, const Vector2<float>& lineEnd, Vector2<float>* sweepResult = nullptr);

	float GetHitPointCapsuleAndLineSegment(const Capsule& capsule,
		const Line& line, const Line& prevLine, const Line& nextLine, Vector2<float>* sweepResult = nullptr);

	Vector2<float> GetDivideLine(const Vector2<float>& vec1, const Vector2<float>& vec2);
}
