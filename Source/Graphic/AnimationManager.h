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
	/// アニメーションの読み込み
	/// </summary>
	/// <param name="filename">アニメーションデータのパス</param>
	void LoadAnimation(std::string_view filename);

	/// <summary>
	/// 画像のパスの取得
	/// </summary>
	/// <param name="filename">アニメーションデータのパス</param>
	/// <returns>画像のパス</returns>
	[[nodiscard]] 
	std::string& GetImagePath(std::string_view filename);

	/// <summary>
	/// すべての状態のアニメーションデータの取得
	/// </summary>
	/// <param name="filename">アニメーションデータのパス</param>
	/// <returns>アニメーションデータ</returns>
	[[nodiscard]]
	std::unordered_map<std::string, AnimData>& GetAnimations(const std::string& filename);

	/// <summary>
	/// 特定の状態のアニメーションデータの取得
	/// </summary>
	/// <param name="filename">アニメーションデータのパス</param>
	/// <param name="state">状態</param>
	/// <returns>状態に対応するアニメーションデータ</returns>
	[[nodiscard]]
	AnimData& GetAnimation(std::string_view filename, std::string_view state);
private:
	AnimationManager() = default;
	~AnimationManager() = default;

	static AnimationManager* instance_;

	std::unordered_map<std::string, AnimList> animations_;
};

