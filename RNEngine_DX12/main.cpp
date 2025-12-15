#include "RNEngine.h"
using namespace RNEngine;

#ifdef _DEBUG
int main() {
#else
#include<Windows.h>
int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int) {
#endif
	SetProcessDpiAwarenessContext(DPI_AWARENESS_CONTEXT_PER_MONITOR_AWARE_V2);

	Engine* engine = new Engine();
	engine->Init();

	engine->Update();

	engine->Destroy();

	delete engine;
	return 0;
}