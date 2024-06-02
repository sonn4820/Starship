#define WIN32_LEAN_AND_MEAN		// Always #define this before #including <windows.h>
#include <windows.h>			// #include this (massive, platform-specific) header in VERY few places (and .CPPs only)
#include "Game/App.hpp"
#include "Game/GameExtern.hpp"

//-----------------------------------------------------------------------------------------------
int WINAPI WinMain( _In_ HINSTANCE applicationInstanceHandle, _In_opt_ HINSTANCE previousInstance, _In_ LPSTR commandLineString, _In_ int nShpwCmd)
{
	UNUSED(applicationInstanceHandle);
	UNUSED(previousInstance);
	UNUSED(commandLineString);		
	UNUSED(nShpwCmd);		

	g_theApp = new App();
	g_theApp->Startup();
	g_theApp->Run();
	g_theApp->Shutdown();
	delete g_theApp;
	g_theApp = nullptr;

	return 0;
}