#include "stdafx.h"
#include "RNEngine.h"

namespace RNEngine {

	//�ʓ|�����Ǐ����Ȃ�������
	LRESULT WindowProcedure(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam) {
		if (msg == WM_DESTROY) {//�E�B���h�E���j�����ꂽ��Ă΂�܂�
			PostQuitMessage(0);//OS�ɑ΂��āu�������̃A�v���͏I�����v�Ɠ`����
			return 0;
		}
		return DefWindowProc(hwnd, msg, wparam, lparam);//�K��̏������s��
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
