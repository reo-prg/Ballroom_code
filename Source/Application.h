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
	/// ƒQ[ƒ€‘¬“x‚Ìİ’è
	/// </summary>
	/// <param name="speed">‘¬“x‚Ì”{—¦</param>
	/// <param name="time">Œp‘±ŠÔ</param>
	static void SetGameSpeed(float speed, float time);
	static float GetGameSpeed(void) { return gameSpeed_; }

	/// <summary>
	/// ƒQ[ƒ€‘¬“x‚ªŠÖŒW‚È‚¢Œo‰ßŠÔ‚Ìæ“¾
	/// </summary>
	/// <returns>Œo‰ßŠÔ</returns>
	static float GetRealDeltaTime(void);

	static void ResetDeltaTime(void);

	/// <summary>
	/// ƒQ[ƒ€‚ÌI—¹
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

