#include"stdafx.h"
#include "project.h"
using namespace RNEngine;
#ifdef _DEBUG
int main() {
#else
#include<Windows.h>
int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int) {
#endif
	RnEngine engine = RnEngine();
	engine.Init();

	engine.Update();

	engine.Destroy();
	return 0;
}