#include "DebugModule.h"
#include "Input/InputIntegrator.h"

bool DebugModule::isDebuging_ = false;

void DebugModule::CheckDebugEnable(void)
{
#ifdef _DEBUG
	if (InputIns.Triggered("debug"))
	{
		isDebuging_ = !isDebuging_;
	}
#endif // DEBUG
}

bool DebugModule::IsDebugEnable(void)
{
#ifdef _DEBUG
	return isDebuging_;
#else
	return false;
#endif // DEBUG
}
