#include <Windows.h>
#include "Application.h"

int WINAPI WinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPSTR lpCmdLine, _In_ int nCmdShow)
{
	Application* app = new Application();

	if (!app->Initialize())
	{
		return -1;
	}
	app->Run();
	app->Terminate();

	delete app;

	return 0;
}