#include "stdafx.h"
#include "project.h"

namespace RNEngine {

	//面倒だけど書かなあかんやつ
	LRESULT WindowProcedure(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam) {
		if (msg == WM_DESTROY) {//ウィンドウが破棄されたら呼ばれます
			PostQuitMessage(0);//OSに対して「もうこのアプリは終わるんや」と伝える
			return 0;
		}
		return DefWindowProc(hwnd, msg, wparam, lparam);//規定の処理を行う
	}

	void Window::Create(const wstring& appName, UINT width, UINT height) {

		auto result = CoInitializeEx(0, COINIT_MULTITHREADED);
		assert(SUCCEEDED(result));

		WNDCLASSEX w = {
			sizeof(WNDCLASSEX),
			CS_CLASSDC,
			(WNDPROC)WindowProcedure,
			0,0,
			GetModuleHandle(NULL),
			NULL,NULL,NULL,NULL,
			appName.c_str(),
			NULL
		};
		RegisterClassEx(&w);

		m_Hwnd = CreateWindow(
			appName.c_str(), appName.c_str(),
			WS_OVERLAPPEDWINDOW,
			CW_USEDEFAULT, CW_USEDEFAULT,
			width, height,
			NULL, NULL,
			w.hInstance,
			NULL
		);
		ShowWindow(m_Hwnd, SW_SHOW);

		m_AppName = appName;
		m_Instance = w.hInstance;
	}

	bool Window::ProcessMessage() {
		MSG msg{};

		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) {
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}

		return msg.message != WM_QUIT;
	}
}