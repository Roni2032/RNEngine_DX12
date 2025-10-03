#include "stdafx.h"
#include "RNEngine.h"

namespace RNEngine {

	//面倒だけど書かなあかんやつ
	LRESULT WindowProcedure(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam) {
		if (msg == WM_DESTROY) {//ウィンドウが破棄されたら呼ばれます
			PostQuitMessage(0);//OSに対して「もうこのアプリは終わるんや」と伝える
			return 0;
		}
		return DefWindowProc(hwnd, msg, wparam, lparam);//規定の処理を行う
	}

	HWND RnEngine::InitWindow(const TCHAR* appName) {
		HINSTANCE hInstance = GetModuleHandle(nullptr);

		WNDCLASSEX w = {
			sizeof(WNDCLASSEX),
			CS_CLASSDC,
			(WNDPROC)WindowProcedure,
			0,0,
			GetModuleHandle(NULL),
			NULL,NULL,NULL,NULL,
			appName,
			NULL
		};
		RegisterClassEx(&w);

		int x = 0, y = 0;

		HWND hwnd = CreateWindow(
			appName,appName,
			WS_OVERLAPPED,
			x,y,
			FRAME_BUFFER_W
			)
        return S_OK;
    }
	void RnEngine::Init() {

	}
	void RnEngine::Destroy() {

	}
}
