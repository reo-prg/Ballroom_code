#include "InputIntegrator.h"
#include "XInputState.h"
#include "KeyboardState.h"

InputIntegrator& InputIntegrator::GetInstance(void)
{
	static InputIntegrator i;
	return i;
}

InputIntegrator::InputIntegrator()
{
	xinput_.reset(new XInputState());
	key_.reset(new KeyboardState());
	Initialize();
}

InputIntegrator::~InputIntegrator()
{
}

void InputIntegrator::Update(void)
{
	xinput_->Update();
	key_->Update();
	GetCursorPos(&cursorPos_);
	ScreenToClient(hwnd_, &cursorPos_);

	WINDOWINFO info = {};
	GetWindowInfo(hwnd_, &info);
}

bool InputIntegrator::Pressed(const std::string& key)
{
	if (keyConfig_.find(key) == keyConfig_.end())
	{
		return false;
	}

	bool rtn = false;
	for (const auto& k : keyConfig_.at(key))
	{
		switch (k.type_)
		{
		case InputType::Keyboard:
			rtn |= key_->Pressed(k.key_);
			break;
		case InputType::XInput:
			rtn |= xinput_->Pressed(k.key_);
			break;
		default:
			break;
		}
	}
	return rtn;
}

bool InputIntegrator::Triggered(const std::string& key)
{
	if (keyConfig_.find(key) == keyConfig_.end())
	{
		return false;
	}

	bool rtn = false;
	for (const auto& k : keyConfig_.at(key))
	{
		switch (k.type_)
		{
		case InputType::Keyboard:
			rtn |= key_->Triggered(k.key_);
			break;
		case InputType::XInput:
			rtn |= xinput_->Triggered(k.key_);
			break;
		default:
			break;
		}
	}
	return rtn;
}

bool InputIntegrator::Released(const std::string& key)
{
	if (keyConfig_.find(key) == keyConfig_.end())
	{
		return false;
	}

	bool rtn = false;
	for (const auto& k : keyConfig_.at(key))
	{
		switch (k.type_)
		{
		case InputType::Keyboard:
			rtn |= key_->Released(k.key_);
			break;
		case InputType::XInput:
			rtn |= xinput_->Released(k.key_);
			break;
		default:
			break;
		}
	}
	return rtn;
}

bool InputIntegrator::AnyPressed(void)
{
	if (key_->AnyPressed()) { return true; }
	//if (xinput_->Pressed(0x3ffffff)) { return true; }
	return false;
}

bool InputIntegrator::AnyTriggered(void)
{
	if (key_->AnyTriggered()) { return true; }
	//if (xinput_->Triggered(0x3ffffff)) { return true; }
	return false;
}

void InputIntegrator::SetWindowHandle(HWND hwnd)
{
	hwnd_ = hwnd;
}

std::pair<float, float> InputIntegrator::GetMouseCursorPosition(void)
{
	return std::pair<float, float>(static_cast<float>(cursorPos_.x), static_cast<float>(cursorPos_.y));
}

void InputIntegrator::Initialize(void)
{
	Register("lclick", { { InputType::Keyboard, VK_LBUTTON } });
	Register("left", { { InputType::Keyboard, VK_LEFT }, { InputType::XInput, XINPUT_GAMEPAD_LEFT_THUMB_LEFT } });
	Register("right",		{ { InputType::Keyboard, VK_RIGHT }, { InputType::XInput, XINPUT_GAMEPAD_LEFT_THUMB_RIGHT } });
	Register("up",			{ { InputType::Keyboard, VK_UP }, { InputType::XInput, XINPUT_GAMEPAD_LEFT_THUMB_UP } });
	Register("down",		{ { InputType::Keyboard, VK_DOWN }, { InputType::XInput, XINPUT_GAMEPAD_LEFT_THUMB_DOWN } });
	Register("jump",		{ { InputType::Keyboard, 0x5a }, { InputType::XInput, XINPUT_GAMEPAD_B },
		{ InputType::XInput, XINPUT_GAMEPAD_A } });
	Register("throw",		{ { InputType::Keyboard, 0x58 }, { InputType::XInput, XINPUT_GAMEPAD_LEFT_TRIGGER }, { InputType::XInput, XINPUT_GAMEPAD_RIGHT_TRIGGER }, });
	Register("ok",			{ { InputType::Keyboard, 0x5a }, { InputType::XInput, XINPUT_GAMEPAD_B } });
	Register("back",		{ { InputType::Keyboard, 0x58 }, { InputType::XInput, XINPUT_GAMEPAD_A } });
#ifdef _DEBUG
	Register("debug",		{ { InputType::Keyboard, VK_F3 } });
#endif // DEBUG
	GetCursorPos(&cursorPos_);
}

void InputIntegrator::Register(const std::string& key, const std::vector<Input>& list)
{
	if (keyConfig_.find(key) == keyConfig_.end())
	{
		keyConfig_.emplace(key, list);
	}
	else
	{
		for (const auto& l : list)
		{
			keyConfig_.at(key).emplace_back(l);
		}
	}
}
