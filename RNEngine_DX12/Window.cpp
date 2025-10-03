#include "stdafx.h"
#include "Window.h"

namespace RNEngine {

	//�ʓ|�����Ǐ����Ȃ�������
	LRESULT WindowProcedure(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam) {
		if (msg == WM_DESTROY) {//�E�B���h�E���j�����ꂽ��Ă΂�܂�
			PostQuitMessage(0);//OS�ɑ΂��āu�������̃A�v���͏I�����v�Ɠ`����
			return 0;
		}
		return DefWindowProc(hwnd, msg, wparam, lparam);//�K��̏������s��
	}

	void Window::Init(const TCHAR* appName, int width, int height) {

		auto result = CoInitializeEx(0, COINIT_MULTITHREADED);
		assert(SUCCEEDED(result));

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

		m_hwnd = CreateWindow(
			appName, appName,
			WS_OVERLAPPEDWINDOW,
			CW_USEDEFAULT, CW_USEDEFAULT,
			width, height,
			NULL, NULL,
			w.hInstance,
			NULL
		);
		ShowWindow(m_hwnd, SW_SHOW);
	}
}