#pragma once
#include "../Math/VectorInterface.h"


struct Circle
{
	/// <summary>半径</summary>
	float radius_;

	/// <summary>座標</summary>
	Vector2<float> pos_;


	/// <summary</summary>
	/// <param name="rad">半径</param>
	/// <param name="pos">座標</param>
	Circle(float rad, const Vector2<float>& pos = Vector2<float>::ZeroVector())
		:radius_(rad), pos_(pos) {};

	Circle() 
		:radius_(1.0f), pos_(Vector2<float>::ZeroVector()){};


	~Circle() = default;
};


struct Line
{
	/// <summary>始点から終点へのベクトル</summary>
	Vector2<float> vec_;

	/// <summary>始点座標</summary>
	Vector2<float> start_;


	/// <summary></summary>
	/// <param name="vec">終点へのベクトル</param>
	/// <param name="start_pos">始点座標</param>
	Line(const Vector2<float>& vec, const Vector2<float>& start_pos = Vector2<float>::ZeroVector())
		:vec_(vec), start_(start_pos) 
	{};

	Line() 
		:vec_(1.0f, 0.0f), start_(Vector2<float>::ZeroVector()) {};

	Vector2<float> EndPosition(void)const { return start_ + vec_; }


	~Line() = default;
};


struct Square
{
	/// <summary>大きさ</summary>
	Vector2<float> size_;

	/// <summary>座標</summary>
	Vector2<float> pos_;

	Line lines_[4];

	/// <summary></summary>
	/// <param name="size">大きさ(幅、高さ)</param>
	/// <param name="pos">座標</param>
	Square(const Vector2<float>& size, const Vector2<float>& pos = Vector2<float>::ZeroVector())
		:size_(size), pos_(pos)
	{
		ApplyToLines();
	};

	void ApplyToLines(void)
	{
		lines_[0].start_ = pos_;
		lines_[1].start_ = { pos_.x, pos_.y + size_.y };
		lines_[2].start_ = pos_ + size_;
		lines_[3].start_ = { pos_.x + size_.x, pos_.y };

		lines_[0].vec_ = lines_[1].start_ - lines_[0].start_;
		lines_[1].vec_ = lines_[2].start_ - lines_[1].start_;
		lines_[2].vec_ = lines_[3].start_ - lines_[2].start_;
		lines_[3].vec_ = lines_[0].start_ - lines_[3].start_;
	}

	Square()
		:size_(1.0f, 1.0f), pos_(Vector2<float>::ZeroVector()) {};

	Vector2<float> LeftUp(void) { return { pos_.x, pos_.y + size_.y }; }
	Vector2<float> RightUp(void) { return pos_ + size_; }
	Vector2<float> LeftDown(void) { return pos_; }
	Vector2<float> RightDown(void) { return { pos_.x + size_.x, pos_.y }; }

	~Square() = default;
};


struct Triangle
{
	/// <summary>座標に対する頂点座標のオフセット</summary>
	Vector2<float> vertices_[3];

	Line lines_[3];

	/// <summary>座標</summary>
	Vector2<float> pos_;


	/// <summary></summary>
	/// <param name="vert1">頂点座標1</param>
	/// <param name="vert2">頂点座標2</param>
	/// <param name="vert3">頂点座標3</param>
	/// <param name="pos_">座標</param>
	Triangle(const Vector2<float>& vert1,
		const Vector2<float>& vert2,
		const Vector2<float>& vert3,
		const Vector2<float>& pos = Vector2<float>::ZeroVector())
		:vertices_{ vert1, vert2, vert3 }, pos_(pos) 
	{
		ApplyToLines();
	};

	void ApplyToLines(void)
	{
		lines_[0].start_ = vertices_[0];
		lines_[1].start_ = vertices_[1];
		lines_[2].start_ = vertices_[2];

		lines_[0].vec_ = vertices_[1] - vertices_[0];
		lines_[1].vec_ = vertices_[2] - vertices_[1];
		lines_[2].vec_ = vertices_[0] - vertices_[2];
	}

	Triangle()
		:vertices_{ { 0.0f, 0.0f }, { 0.0f, 1.0f }, { 1.0f, 0.0f } }, pos_(Vector2<float>::ZeroVector()) {};

	~Triangle() = default;
};


struct Capsule
{
	/// <summary>線分</summary>
	Line line_;

	/// <summary>厚さ(半径)</summary>
	float radius_;


	/// <summary></summary>
	/// <param name="line">線分</param>
	/// <param name="rad">厚さ(半径)</param>
	Capsule(const Line& line, float rad = 1.0f)
		:line_(line), radius_(rad) {};

	Capsule() :line_(), radius_(1.0f) {};


	~Capsule() = default;
};


