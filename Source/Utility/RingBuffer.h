#include <array>
#include <algorithm>
#include <functional>

template<typename Ty, size_t N>
class RingBuffer
{
public:
	/// <summary>
	/// �v�f���擾����
	/// </summary>
	/// <param name="n">��̗v�f����̍�</param>
	/// <returns>�v�f</returns>
	Ty& operator[](size_t n)
	{
		return vec_[(currentBuffer_ + n) % vec_.size()];
	}

	/// <summary>
	/// ���̔z��̎擾
	/// </summary>
	std::array<Ty, N>& Data(void)
	{
		return vec_;
	}

	/// <summary>
	/// �z��̃T�C�Y�̎擾
	/// </summary>
	constexpr size_t Size(void)const noexcept
	{
		return vec_.size();
	}

	/// <summary>
	/// ���݂̊�̗v�f�ԍ���1�i�߂�
	/// </summary>
	void Next(void)noexcept { currentBuffer_ = (currentBuffer_ + 1) % vec_.size(); }
	
	/// <summary>
	/// ���݂̊�̗v�f�ԍ���1�߂�
	/// </summary>
	void Prev(void)noexcept { currentBuffer_ = (currentBuffer_ + vec_.size() - 1) % vec_.size(); }

	/// <summary>
	/// ����̗v�f�Ŗ��߂�
	/// </summary>
	void Fill(const Ty& value)
	{
		std::fill(vec_.begin(), vec_.end(), value);
	}

	/// <summary>
	/// ���ׂĂ̗v�f�������𖞂����Ă���̂��𒲂ׂ�
	/// </summary>
	/// <param name="func">�֐�</param>
	/// <returns>���ׂĖ������Ă�����true</returns>
	bool Allof(const std::function<bool(Ty)>& func)
	{
		return std::all_of(vec_.begin(), vec_.end(), func);
	}
private:
	std::array<Ty, N> vec_;
	size_t currentBuffer_ = 0;
};
