#pragma once
#include <tuple>
#include <chrono>
#include <memory>

class SceneManager;
class Application
{
public:
	Application();
	~Application();

	bool Initialize(void);
	void Run(void);
	void Terminate(void);

	static std::pair<int, int> GetWindowSize(void);

	/// <summary>
	/// ゲーム速度の設定
	/// </summary>
	/// <param name="speed">速度の倍率</param>
	/// <param name="time">継続時間</param>
	static void SetGameSpeed(float speed, float time);
	static float GetGameSpeed(void) { return gameSpeed_; }

	/// <summary>
	/// ゲーム速度が関係ない経過時間の取得
	/// </summary>
	/// <returns>経過時間</returns>
	static float GetRealDeltaTime(void);

	static void ResetDeltaTime(void);

	/// <summary>
	/// ゲームの終了
	/// </summary>
	static void EndGame(void);
private:
	void InitializeAudio(void);
	void LoadSystemSound(void);
	void LoadEffect(void);

	float GetDeltaTime(void);

	static void GameSpeedUpdate(float delta);

	static constexpr int screenWidth = 1080;
	static constexpr int screenHeight = 720;

	std::chrono::steady_clock::time_point prevTime_;

	std::unique_ptr<SceneManager> scenemng_;

	static float absDelta_;
	static float gameSpeed_;
	static float time_;

	static bool end_;
};

