#include "ShapeFunc.h"
#include <algorithm>
#include <limits>
#include "../Math/MathUtility.h"
#include <Windows.h>
#include <string>

namespace sFunc
{
	constexpr float epsilon = 0.0f;

	float GetDistancePointToLineSegment(const Vector2<float>& point, const Line& line)
	{
		return sqrt(GetSquareDistancePointToLineSegment(point, line));
	}

	float GetDistancePointToLineSegment(const Vector2<float>& point, 
		const Vector2<float>& lineStart, const Vector2<float>& lineEnd)
	{
		return sqrt(GetSquareDistancePointToLineSegment(point, lineStart, lineEnd));
	}

	float GetSquareDistancePointToLineSegment(const Vector2<float>& point, const Line& line)
	{
		Vector2<float> clVec = point - line.start_;
		Vector2<float> lVec = line.vec_;

		float dist = Dot(lVec.NormalizedVec(), clVec);
		dist = std::clamp(dist / lVec.Distance(), epsilon, 1.0f - epsilon);

		Vector2<float> lPoint = line.start_ + lVec * dist;

		return SquareDistance(lPoint, point);
	}

	float GetSquareDistancePointToLineSegment(const Vector2<float>& point, 
		const Vector2<float>& lineStart, const Vector2<float>& lineEnd)
	{
		Vector2<float> clVec = point - lineStart;
		Vector2<float> lVec = lineEnd - lineStart;

		float dist = Dot(lVec.NormalizedVec(), clVec);
		dist = std::clamp(dist / lVec.Distance(), epsilon, 1.0f - epsilon);

		Vector2<float> lPoint = lineStart + lVec * dist;

		return SquareDistance(lPoint, point);
	}

	float GetProgressPointToLineSegment(const Vector2<float>& point, const Line& line)
	{
		Vector2<float> clVec = point - line.start_;
		Vector2<float> lVec = line.vec_;

		float dist = Dot(lVec.NormalizedVec(), clVec);
		dist = std::clamp(dist / lVec.Distance(), 0.0f, 1.0f);
		return dist;
	}

	float GetProgressPointToLineSegment(const Vector2<float>& point, const Vector2<float>& lineStart, const Vector2<float>& lineEnd)
	{
		Vector2<float> clVec = point - lineStart;
		Vector2<float> lVec = lineEnd - lineStart;

		float dist = Dot(lVec.NormalizedVec(), clVec);
		dist = std::clamp(dist / lVec.Distance(), 0.0f, 1.0f);
		return dist;
	}

	bool CheckPointSide(const Vector2<float>& point, const Line& line)
	{
		return CheckPointSide(point, line.start_, line.EndPosition());
	}

	bool CheckPointSide(const Vector2<float>& point, const Vector2<float>& lineStart, const Vector2<float>& lineEnd)
	{
		Vector2<float> norm{ (lineEnd.y - lineStart.y) * -1.0f, lineEnd.x - lineStart.x };
		float d = Dot(norm, point - lineStart);
		return IsPositive(d);
	}

	bool GetLineCrossPoint(const Line& line1, const Line& line2, float* line1CrossPoint, float* line2CrossPoint)
	{
		return GetLineCrossPoint(line1.start_, line1.EndPosition(), line2.start_, line2.EndPosition(), line1CrossPoint, line2CrossPoint);
	}

	bool GetLineCrossPoint(const Vector2<float>& lineStart1, const Vector2<float>& lineEnd1, 
		const Vector2<float>& lineStart2, const Vector2<float>& lineEnd2, float* line1CrossPoint, float* line2CrossPoint)
	{
		Vector2<float> vec1 = lineEnd1 - lineStart1;
		Vector2<float> vec2 = lineEnd2 - lineStart2;

		float cross_v1v2 = Cross(vec1, vec2);
		if (cross_v1v2 == 0.0f)
		{
			return false;
		}

		float cross_v1 = Cross(lineStart2 - lineStart1, vec1) / cross_v1v2;
		float cross_v2 = Cross(lineStart2 - lineStart1, vec2) / cross_v1v2;

		if (line1CrossPoint) { *line1CrossPoint = cross_v1; }
		if (line2CrossPoint) { *line2CrossPoint = cross_v2; }

		return (cross_v1 >= 0.0f && cross_v1 <= 1.0f && cross_v2 >= 0.0f && cross_v2 <= 1.0f);
	}

	bool IsHitCapsuleToLineSegment(const Capsule& capsule, const Line& line)
	{
		float dist;
		dist = GetSquareDistancePointToLineSegment(capsule.line_.start_, line);
		dist = fminf(dist, GetSquareDistancePointToLineSegment(capsule.line_.EndPosition(), line));
		dist = fminf(dist, GetSquareDistancePointToLineSegment(line.start_, capsule.line_));
		dist = fminf(dist, GetSquareDistancePointToLineSegment(line.EndPosition(), capsule.line_));

		return dist < capsule.radius_ * capsule.radius_;
	}

	bool IsHitCapsuleToLineSegment(const Capsule& capsule, const Vector2<float>& lineStart, const Vector2<float>& lineEnd)
	{
		return IsHitCapsuleToLineSegment(capsule, Line(lineEnd - lineStart, lineStart));
	}

	float GetHitPointCapsuleAndLineSegment(const Capsule& capsule, const Line& line, Vector2<float>* sweepResult)
	{
		Vector2<float> norm = line.vec_.GetNormalVec().NormalizedVec();
		Vector2<float> pVec = capsule.line_.EndPosition() - line.start_;

		float dist = Dot(pVec, norm);

		float prog = GetProgressPointToLineSegment(capsule.line_.EndPosition(), line);

		Vector2<float> pbVec;

		float pushlen = capsule.radius_ - dist + 1.0f;
		pbVec = norm * pushlen;

		Vector2<float> pbPos = capsule.line_.EndPosition() + pbVec;
		Vector2<float> rVec;
		float len;
		float offset;
		if (prog == 0.0f)
		{
			rVec = pbPos - line.start_;
			pVec = -line.vec_.NormalizedVec();
			len = Dot(rVec, pVec);
			len = std::clamp(len / capsule.radius_, -1.0f, 1.0f);
			offset = -(1.0f - sinf(acosf(len))) * capsule.radius_;
			pbPos += norm * offset;
			pushlen += offset;
		}
		else if (prog == 1.0f)
		{
			rVec = pbPos - line.EndPosition();
			pVec = line.vec_.NormalizedVec();
			len = Dot(rVec, pVec);
			len = std::clamp(len / capsule.radius_, -1.0f, 1.0f);
			offset = -(1.0f - sinf(acosf(len))) * capsule.radius_;
			pbPos += norm * -(1.0f - sinf(acosf(len))) * capsule.radius_;
			pushlen += offset;
		}

		float v = Dot(-capsule.line_.vec_, norm);

		if (sweepResult)
		{
			*sweepResult = pbPos;
		}

		if (v == 0.0f)
		{
			return 1.0f;
		}
		if (v == 1.0f)
		{
			return 0.0f;
		}


		if (abs(1.0f - (pushlen / v)) > 100.0f)
		{
			int a = 0;
		}

		return 1.0f - (pushlen / v);
	}

	float GetHitPointCapsuleAndLineSegment(const Capsule& capsule, const Vector2<float>& lineStart, const Vector2<float>& lineEnd, Vector2<float>* sweepResult)
	{
		return GetHitPointCapsuleAndLineSegment(capsule, Line(lineEnd - lineStart, lineStart), sweepResult);
	}
	float GetHitPointCapsuleAndLineSegment(const Capsule& capsule, const Line& line, const Line& prevLine, const Line& nextLine, Vector2<float>* sweepResult)
	{
		if (!IsHitCapsuleToLineSegment(capsule, line))
		{
			return 1.0f;
		}

		Vector2<float> norm = line.vec_.GetNormalVec().NormalizedVec();
		Vector2<float> pVec = capsule.line_.EndPosition() - line.start_;

		float dist = Dot(pVec, norm);

		float prog = GetProgressPointToLineSegment(capsule.line_.EndPosition(), line);

		Vector2<float> pbVec;

		float pushlen = capsule.radius_ - dist + 1.0f;
		pbVec = norm * pushlen;

		Vector2<float> pbPos = capsule.line_.EndPosition() + pbVec;
		Vector2<float> rVec;
		float len;
		float offset;

		if (CheckPointSide(pbPos, line.start_, line.start_ + -GetDivideLine(line.vec_, -prevLine.vec_)))
		{
			return 1.0f;
		}
		if (!CheckPointSide(pbPos, line.EndPosition(), line.EndPosition() + -GetDivideLine(-line.vec_, nextLine.vec_)))
		{
			return 1.0f;
		}

		float v = Dot(-capsule.line_.vec_, norm);

		prog = GetProgressPointToLineSegment(pbPos, line);
		if (prog == 0.0f)
		{
			rVec = pbPos - line.start_;
			pVec = -line.vec_.NormalizedVec();
			len = Dot(rVec, pVec);
			len = std::clamp(len / capsule.radius_, 0.0f, 1.0f);
			offset = -(1.0f - sinf(acosf(len))) * capsule.radius_;
			pbPos += norm * offset;
			pbPos += pVec * len * capsule.line_.vec_.Distance();

			pushlen += offset;

			//otherVec = -line.vec_.NormalizedVec() + prevLine.vec_.NormalizedVec();

		}
		else if (prog == 1.0f)
		{
			rVec = pbPos - line.EndPosition();
			pVec = line.vec_.NormalizedVec();
			len = Dot(rVec, pVec);
			len = std::clamp(len / capsule.radius_, 0.0f, 1.0f);
			offset = -(1.0f - sinf(acosf(len))) * capsule.radius_;
			pbPos += norm * offset;
			pbPos += pVec * len * capsule.line_.vec_.Distance();
			pushlen += offset;

			//otherVec = line.vec_.NormalizedVec() - nextLine.vec_.NormalizedVec();

		}

		float sl = Dot(norm, capsule.line_.vec_.NormalizedVec());

		if (v == 0.0f)
		{
			float d = GetDistancePointToLineSegment(capsule.line_.EndPosition(), line);
			if (CheckPointSide(capsule.line_.EndPosition(), line))
			{
				*sweepResult = capsule.line_.EndPosition() + norm * (capsule.radius_ - d);
			}
			else
			{
				*sweepResult = capsule.line_.EndPosition() + norm * (capsule.radius_ + d);
			}
			return 1.0f - std::numeric_limits<float>::epsilon();
		}

		if (sweepResult)
		{
			*sweepResult = pbPos;
		}

		return 1.0f - (pushlen / v);
	}

	Vector2<float> GetDivideLine(const Vector2<float>& vec1, const Vector2<float>& vec2)
	{
		float len1 = vec1.Distance();
		float len2 = vec2.Distance();
		Vector2<float> floorVec = vec2 - vec1;
		Vector2<float> pos = vec1 + floorVec * (len1 / (len1 + len2));

		return pos;
	}
}