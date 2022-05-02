#pragma once
#include <array>
#include <list>
#include <memory>

template<typename Ty, size_t Size>
class HandleArray
{
public:
	/// <summary>
	/// 要素の取得
	/// </summary>
	/// <param name="n">ハンドル</param>
	/// <returns>要素</returns>
	std::unique_ptr<Ty>& operator[](size_t n)
	{
		return buffer_[n].first;
	}

	/// <summary>
	/// 要素の追加
	/// </summary>
	/// <param name="elem">追加する要素</param>
	/// <returns>ハンドル</returns>
	int Add(Ty* elem)
	{
		if (num_ + 1 >= buffer_.size()) { return -1; }

		int index;
		if (num_ > 0)
		{
			if (areaMax_ + 1 < buffer_.size())
			{
				areaMax_++;
				index = areaMax_;
			}
			else if (areaMin_ - 1 >= 0)
			{
				areaMin_--;
				index = areaMin_;
			}
			else
			{
				for (index = 0; index < buffer_.size() && buffer_[index].first; index++) {}
				if (index >= buffer_.size())
				{
					return -1;
				}
			}
		}
		else
		{
			areaMax_ = 0;
			areaMin_ = 0;
			num_ = 1;
			index = 0;
		}

		buffer_[index].first.reset(elem);
		buffer_[index].second = handleList_.insert(handleList_.begin(), index);
		num_++;

		return index;
	}

	/// <summary>
	/// ハンドルに対応する要素の削除
	/// </summary>
	/// <param name="handle">ハンドル</param>
	void Remove(int handle)
	{
		if (num_ <= 0) { return; }
		if (!buffer_[handle].first) { return; }

		buffer_[handle].first.reset();
		handleList_.erase(buffer_[handle].second);

		if (num_ > 1)
		{
			if (handle == areaMax_)
			{
				while (!buffer_[areaMax_].first && areaMax_ > 0) { areaMax_--; }
			}
			else if (handle == areaMin_)
			{
				while (!buffer_[areaMin_].first && areaMin_ < buffer_.size() - 1) { areaMin_++; }
			}
		}
		else
		{
			areaMax_ = 0;
			areaMin_ = 0;
		}
		num_--;
	}

	/// <summary>
	/// 全要素削除
	/// </summary>
	void Clear(void)
	{
		for (auto& h : handleList_)
		{
			buffer_[h].first.reset();
		}
	}

	/// <summary>
	/// 生のリストの取得
	/// </summary>
	const std::list<int>& GetHandleList(void)
	{
		return handleList_;
	}
private:
	std::array<std::pair<std::unique_ptr<Ty>, std::list<int>::iterator>, Size> buffer_;

	std::list<int> handleList_;

	int areaMax_ = 0;
	int areaMin_ = 0;
	int num_ = 0;
};
