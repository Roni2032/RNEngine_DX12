#include"stdafx.h"
#include "project.h"
using namespace RNEngine;
#ifdef _DEBUG
int main() {
#else
#include<Windows.h>
int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int) {
#endif
	Engine* engine = new Engine();
	engine->Init();

	engine->Update();

	engine->Destroy();

	delete engine;
	return 0;
}