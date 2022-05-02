#pragma once
#include "../../RapidXML/rapidxml.hpp"
#include "../../RapidXML/rapidxml_utils.hpp"
#include <string_view>

struct AnimData;

class Animation
{
public:
	Animation() = delete;
	~Animation() = default;

	Animation(const Animation&) = default;
	Animation& operator=(const Animation&) = default;

	Animation(Animation&&) = default;
	Animation& operator=(Animation&&) = default;

	/// <summary>
	/// �R���X�g���N�^
	/// </summary>
	/// <param name="filename">�A�j���[�V�����f�[�^�̃p�X</param>
	explicit Animation(std::string_view filename);

	/// <summary>
	/// <para>�V�����A�j���[�V�����f�[�^��ݒ肷��</para>
	/// <para>���݂̃A�j���[�V�����͎g���Ȃ��Ȃ�</para>
	/// </summary>
	/// <param name="filename">�p�X</param>
	void SetAnimationData(std::string_view filename);

	/// <summary>
	/// �A�j���[�V�����̍X�V
	/// </summary>
	/// <param name="deltaTime">�o�߂�������</param>
	void Update(double deltaTime);

	/// <summary>
	/// ��Ԃ̕ω�
	/// </summary>
	/// <param name="state">���</param>
	void ChangeState(std::string_view state);
	
	/// <summary>
	/// ���݂̉摜�̃n���h���̎擾
	/// </summary>
	/// <returns>�n���h��</returns>
	int GetImageHandle(void)const;

	/// <summary>
	/// ���݂̍Đ��ʒu�̐i�x���擾
	/// </summary>
	/// <returns>�Đ��ʒu(0.0����1.0)</returns>
	double GetCurrentAnimationProgress(void)const;

	/// <summary>
	/// ���݂̍Đ��ʒu�̎��Ԃ��擾
	/// </summary>
	/// <returns>�Đ��ʒu(0.0�ȏ�)</returns>
	double GetCurrentAnimationTime(void)const;

	/// <summary>
	/// ���݂̏�Ԃ̎擾
	/// </summary>
	/// <returns>���</returns>
	const std::string& GetCurrentState(void)const;

	/// <summary>
	/// �摜�̃p�X�̎擾
	/// </summary>
	/// <returns>�摜�̃p�X</returns>
	const std::string& GetImagePath(void)const;

	/// <summary>
	///	<para>���ݍĐ����Ă���A�j���[�V�����̃��[�v�񐔂̏㏑��</para>
	/// <para>�f�[�^������������킯�ł͂Ȃ��̂ŁA</para>
	/// <para>������x�Đ��������̃��[�v�񐔂̓f�t�H���g�ɖ߂��Ă���</para>
	/// </summary>
	/// <param name="count">��</param>
	void SetLoopCount(int count);
private:
	std::pair<int, double>* currentData_;
	int imageHandle_;
	double currentTime_;
	double finalTime_;
	std::string currentState_;
	std::string imagePath_;

	std::string filePath_;
	int loopCount_;
};

