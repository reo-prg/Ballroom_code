#pragma once

class DebugModule
{
public:
	static void CheckDebugEnable(void);

	static bool IsDebugEnable(void);
private:
	static bool isDebuging_;
};
