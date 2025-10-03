#include"stdafx.h"
#include "RNEngine.h"
using namespace RNEngine;
#ifdef _DEBUG
int main() {
#else
#include<Windows.h>
int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int) {
#endif
	RnEngine engine = RnEngine();
	engine.Init();



	engine.Destroy();
	return 0;
}