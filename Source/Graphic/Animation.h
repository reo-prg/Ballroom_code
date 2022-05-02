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
	/// コンストラクタ
	/// </summary>
	/// <param name="filename">アニメーションデータのパス</param>
	explicit Animation(std::string_view filename);

	/// <summary>
	/// <para>新しくアニメーションデータを設定する</para>
	/// <para>現在のアニメーションは使えなくなる</para>
	/// </summary>
	/// <param name="filename">パス</param>
	void SetAnimationData(std::string_view filename);

	/// <summary>
	/// アニメーションの更新
	/// </summary>
	/// <param name="deltaTime">経過した時間</param>
	void Update(double deltaTime);

	/// <summary>
	/// 状態の変化
	/// </summary>
	/// <param name="state">状態</param>
	void ChangeState(std::string_view state);
	
	/// <summary>
	/// 現在の画像のハンドルの取得
	/// </summary>
	/// <returns>ハンドル</returns>
	int GetImageHandle(void)const;

	/// <summary>
	/// 現在の再生位置の進度を取得
	/// </summary>
	/// <returns>再生位置(0.0から1.0)</returns>
	double GetCurrentAnimationProgress(void)const;

	/// <summary>
	/// 現在の再生位置の時間を取得
	/// </summary>
	/// <returns>再生位置(0.0以上)</returns>
	double GetCurrentAnimationTime(void)const;

	/// <summary>
	/// 現在の状態の取得
	/// </summary>
	/// <returns>状態</returns>
	const std::string& GetCurrentState(void)const;

	/// <summary>
	/// 画像のパスの取得
	/// </summary>
	/// <returns>画像のパス</returns>
	const std::string& GetImagePath(void)const;

	/// <summary>
	///	<para>現在再生しているアニメーションのループ回数の上書き</para>
	/// <para>データを書き換えるわけではないので、</para>
	/// <para>もう一度再生した時のループ回数はデフォルトに戻っている</para>
	/// </summary>
	/// <param name="count">回数</param>
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

