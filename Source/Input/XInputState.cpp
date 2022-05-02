#include "XInputState.h"
#include <cassert>
#include <cmath>
#include "../Geometry/MathUtility.h"
#include "../Geometry/Matrix.h"

namespace
{
	constexpr float RIGHT_DEADZONE = 14000;
	constexpr float LEFT_DEADZONE = 14000;

}

bool XInputState::TriggerPressed(unsigned int button, unsigned int buffer)
{
	bool rtn = false;
	
	if (button & XINPUT_GAMEPAD_LEFT_TRIGGER)
	{
		if (TriggerTransformToBoolean(state_[buffer].Gamepad.bLeftTrigger))
		{
			rtn = true;
		}
	}
	if (button & XINPUT_GAMEPAD_RIGHT_TRIGGER)
	{
		if (TriggerTransformToBoolean(state_[buffer].Gamepad.bRightTrigger))
		{
			rtn = true;
		}
	}

	return rtn;
}

bool XInputState::TriggerTransformToBoolean(unsigned char value)
{
	return value >= TRIGGER_DEADZONE;
}

bool XInputState::ThumbTilted(unsigned int button, unsigned int buffer)
{
	bool rtn = false;

	if (button & (XINPUT_GAMEPAD_LEFT_THUMB_UP | XINPUT_GAMEPAD_LEFT_THUMB_DOWN | 
		XINPUT_GAMEPAD_LEFT_THUMB_LEFT | XINPUT_GAMEPAD_LEFT_THUMB_RIGHT))
	{
		short tx = state_[buffer].Gamepad.sThumbLX;
		short ty = state_[buffer].Gamepad.sThumbLY;

		if (sqrt(tx * tx + ty * ty) >= LEFT_DEADZONE)
		{
			//Matrix3 tv = TranslationMatrix(tx, ty) * *rotation_;
			Vector2<float> t = MultipleVec(*rotation_, { static_cast<float>(tx), static_cast<float>(ty) });
			
			unsigned int m = XINPUT_GAMEPAD_LEFT_THUMB_UP << ThumbDirection(t.x, t.y);
			rtn = rtn | (button & m);
		}
	}
	if (button & (XINPUT_GAMEPAD_RIGHT_THUMB_UP | XINPUT_GAMEPAD_RIGHT_THUMB_DOWN |
		XINPUT_GAMEPAD_RIGHT_THUMB_LEFT | XINPUT_GAMEPAD_RIGHT_THUMB_RIGHT))
	{
		short tx = state_[buffer].Gamepad.sThumbRX;
		short ty = state_[buffer].Gamepad.sThumbRY;
		if (sqrt(tx * tx + ty * ty) >= RIGHT_DEADZONE)
		{
			//Matrix3 tv = TranslationMatrix(tx, ty) * *rotation_;
			Vector2<float> t = MultipleVec(*rotation_, { static_cast<float>(tx), static_cast<float>(ty) });

			unsigned int m = XINPUT_GAMEPAD_RIGHT_THUMB_UP << ThumbDirection(t.x, t.y);
			rtn = rtn | (button & m);
		}
	}

	return rtn;
}

size_t XInputState::ThumbDirection(float x, float y)
{
	if (IsPositive(x)	&& IsPositive(y))	{ return 0; }		// ã
	else if (!IsPositive(x)	&& IsPositive(y))	{ return 2; }	// ¶
	else if (!IsPositive(x)	&& !IsPositive(y))	{ return 1; }	// ‰º
	else { return 3; }											// ‰E
}

XInputState::XInputState()
{
	RtlZeroMemory(state_, sizeof(XINPUT_STATE) * 2);
	currentBuffer_ = 0;

	rotation_ = std::make_shared<Matrix3>(RotationMatrix(THUMB_ROTATION_ANGLE));
}

XInputState::~XInputState()
{
	
}

void XInputState::Update(void)
{
	currentBuffer_ = 1 - currentBuffer_;
	RtlZeroMemory(&state_[currentBuffer_], sizeof(XINPUT_STATE));
	DWORD result;
	result = XInputGetState(0, &state_[currentBuffer_]);
	if (result != ERROR_SUCCESS)
	{
		RtlZeroMemory(&state_[currentBuffer_], sizeof(XINPUT_STATE));
		assert(result == ERROR_DEVICE_NOT_CONNECTED);
	}
}

bool XInputState::Pressed(unsigned int button, unsigned int buffer)
{
	bool rtn = false;

	if (buffer != defaultBuffer_ && buffer >= _countof(state_))
	{
		assert(false);
		return false;
	}
	size_t buf = buffer == defaultBuffer_ ? currentBuffer_ : buffer;

	unsigned short mask = button & 0xffff;

	int bm = 1;
	while (mask != 0)
	{
		unsigned int b = mask & bm;
		rtn = rtn || (state_[buf].Gamepad.wButtons & b);
		mask -= b;
		bm = bm << 1;
	}

	rtn = rtn || TriggerPressed(button, static_cast<unsigned int>(buf));
	rtn = rtn || ThumbTilted(button, static_cast<unsigned int>(buf));

	return rtn;
}

bool XInputState::Triggered(unsigned int button)
{
	bool rtn = false;

	int bm = 1;
	while (button != 0)
	{
		unsigned int b = button & bm;
		if (Pressed(b, static_cast<unsigned int>(currentBuffer_)) 
			&& !Pressed(b, 1 - static_cast<unsigned int>(currentBuffer_)))
		{
			rtn = true;
			break;
		}
		button -= b;
		bm = bm << 1;
	}

	return rtn;
}

bool XInputState::Released(unsigned int button)
{
	bool rtn = false;

	int bm = 1;
	while (button != 0)
	{
		unsigned int b = button & bm;
		if (!Pressed(b, static_cast<unsigned int>(currentBuffer_)) 
			&& Pressed(b, 1 - static_cast<unsigned int>(currentBuffer_)))
		{
			rtn = true;
			break;
		}
		button -= b;
		bm = bm << 1;
	}

	return rtn;
}
