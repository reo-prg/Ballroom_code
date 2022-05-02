#pragma once
#include <string>
#include <string_view>
#include <vector>
#include <unordered_map>
#include "../../RapidXML/rapidxml.hpp"
#include "../../RapidXML/rapidxml_utils.hpp"

#define lpAnimManager AnimationManager::GetInstance()

constexpr int loopInfinity = -1;

struct AnimData
{
	std::vector<std::pair<int, double>> imageID_;
	int loop_;
};

struct AnimList
{
	std::string imagePath_;
	std::unordered_map<std::string, AnimData> animation_;
};

class AnimationManager
{
public:
	static void Create(void)
	{
		instance_ = new AnimationManager();
	}

	static AnimationManager& GetInstance(void)
	{
		return *instance_;
	}

	static void Terminate(void)
	{
		delete instance_;
	}

	/// <summary>
	/// �A�j���[�V�����̓ǂݍ���
	/// </summary>
	/// <param name="filename">�A�j���[�V�����f�[�^�̃p�X</param>
	void LoadAnimation(std::string_view filename);

	/// <summary>
	/// �摜�̃p�X�̎擾
	/// </summary>
	/// <param name="filename">�A�j���[�V�����f�[�^�̃p�X</param>
	/// <returns>�摜�̃p�X</returns>
	[[nodiscard]] 
	std::string& GetImagePath(std::string_view filename);

	/// <summary>
	/// ���ׂĂ̏�Ԃ̃A�j���[�V�����f�[�^�̎擾
	/// </summary>
	/// <param name="filename">�A�j���[�V�����f�[�^�̃p�X</param>
	/// <returns>�A�j���[�V�����f�[�^</returns>
	[[nodiscard]]
	std::unordered_map<std::string, AnimData>& GetAnimations(const std::string& filename);

	/// <summary>
	/// ����̏�Ԃ̃A�j���[�V�����f�[�^�̎擾
	/// </summary>
	/// <param name="filename">�A�j���[�V�����f�[�^�̃p�X</param>
	/// <param name="state">���</param>
	/// <returns>��ԂɑΉ�����A�j���[�V�����f�[�^</returns>
	[[nodiscard]]
	AnimData& GetAnimation(std::string_view filename, std::string_view state);
private:
	AnimationManager() = default;
	~AnimationManager() = default;

	static AnimationManager* instance_;

	std::unordered_map<std::string, AnimList> animations_;
};

