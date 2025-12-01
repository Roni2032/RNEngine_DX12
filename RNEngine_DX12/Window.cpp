#include "stdafx.h"
#include "project.h"

extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
namespace RNEngine {

	//ñ ì|ÇæÇØÇ«èëÇ©Ç»Ç†Ç©ÇÒÇ‚Ç¬
	LRESULT WindowProcedure(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam) {

		if (ImGui_ImplWin32_WndProcHandler(hwnd, msg, wparam, lparam)) {
			return true;
		}

		switch (msg)
		{
		case WM_DESTROY:
			PostQuitMessage(0);
			break;
		case WM_KEYDOWN:
			WindowKeyDownProcedure(hwnd, wparam);
			break;
		default:
			break;
		}
		
		return DefWindowProc(hwnd, msg, wparam, lparam);//ãKíËÇÃèàóùÇçsÇ§
	}
	void WindowKeyDownProcedure(HWND hwnd, WPARAM wparam) {
		switch (wparam) {
		case VK_ESCAPE:
			DestroyWindow(hwnd);
			break;
		}
	}

	void Window::Create(const wstring& appName, UINT width, UINT height) {
		SetProcessDpiAwarenessContext(DPI_AWARENESS_CONTEXT_PER_MONITOR_AWARE_V2);
		auto result = CoInitializeEx(0, COINIT_MULTITHREADED);
		assert(SUCCEEDED(result));

		RECT rect = { 0,0,static_cast<LONG>(width),static_cast<LONG>(height) };
		AdjustWindowRect(&rect, WS_OVERLAPPEDWINDOW, FALSE);

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
		UINT dpi = GetDpiForWindow(m_Hwnd);
		printf("DPI = %u\n", dpi);
		return msg.message != WM_QUIT;
	}
}