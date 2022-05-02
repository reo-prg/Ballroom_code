#pragma once
#include <Windows.h>
#include <memory>
#include <unordered_map>
#include <string>
#include <vector>

#define InputIns InputIntegrator::GetInstance()

enum class InputType
{
	XInput,
	Keyboard,
};

struct Input
{
	InputType type_;
	unsigned int key_;
};

class XInputState;
class KeyboardState;
class InputIntegrator
{
public:
	static InputIntegrator& GetInstance(void);

	void Update(void);

	/// <summary>
	/// 押されているか
	/// </summary>
	bool Pressed(const std::string& key);

	/// <summary>
	/// 押したか
	/// </summary>
	bool Triggered(const std::string& key);

	/// <summary>
	/// 離したか
	/// </summary>
	bool Released(const std::string& key);

	/// <summary>
	/// どれかを押しているか
	/// </summary>
	bool AnyPressed(void);

	/// <summary>
	/// どれかを押したか
	/// </summary>
	bool AnyTriggered(void);

	void SetWindowHandle(HWND hwnd);

	/// <summary>
	/// マウスカーソルの座標取得
	/// </summary>
	std::pair<float, float> GetMouseCursorPosition(void);
private:
	InputIntegrator();
	InputIntegrator(const InputIntegrator&) = delete;
	InputIntegrator operator=(const InputIntegrator&) = delete;
	~InputIntegrator();

	void Initialize(void);
	void Register(const std::string& key, const std::vector<Input>& list);

	std::unique_ptr<XInputState> xinput_;
	std::unique_ptr<KeyboardState> key_;

	std::unordered_map<std::string, std::vector<Input>> keyConfig_;

	POINT cursorPos_ = {};
	HWND hwnd_;
};

