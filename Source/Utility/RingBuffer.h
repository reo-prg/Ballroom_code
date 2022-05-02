#include <array>
#include <algorithm>
#include <functional>

template<typename Ty, size_t N>
class RingBuffer
{
public:
	/// <summary>
	/// 要素を取得する
	/// </summary>
	/// <param name="n">基準の要素からの差</param>
	/// <returns>要素</returns>
	Ty& operator[](size_t n)
	{
		return vec_[(currentBuffer_ + n) % vec_.size()];
	}

	/// <summary>
	/// 生の配列の取得
	/// </summary>
	std::array<Ty, N>& Data(void)
	{
		return vec_;
	}

	/// <summary>
	/// 配列のサイズの取得
	/// </summary>
	constexpr size_t Size(void)const noexcept
	{
		return vec_.size();
	}

	/// <summary>
	/// 現在の基準の要素番号を1つ進める
	/// </summary>
	void Next(void)noexcept { currentBuffer_ = (currentBuffer_ + 1) % vec_.size(); }
	
	/// <summary>
	/// 現在の基準の要素番号を1つ戻す
	/// </summary>
	void Prev(void)noexcept { currentBuffer_ = (currentBuffer_ + vec_.size() - 1) % vec_.size(); }

	/// <summary>
	/// 特定の要素で埋める
	/// </summary>
	void Fill(const Ty& value)
	{
		std::fill(vec_.begin(), vec_.end(), value);
	}

	/// <summary>
	/// すべての要素が条件を満たしているのかを調べる
	/// </summary>
	/// <param name="func">関数</param>
	/// <returns>すべて満たしていたらtrue</returns>
	bool Allof(const std::function<bool(Ty)>& func)
	{
		return std::all_of(vec_.begin(), vec_.end(), func);
	}
private:
	std::array<Ty, N> vec_;
	size_t currentBuffer_ = 0;
};
