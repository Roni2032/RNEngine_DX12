#pragma once
#include "project.h"

namespace RNEngine
{
	LRESULT WindowProcedure(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam);

	class Window
	{
	private:
		HWND m_Hwnd;
		UINT m_Width;
		UINT m_Height;
		TCHAR* m_AppName;
		HINSTANCE m_Instance;


		void Init(const TCHAR* appName, UINT width, UINT height);
	public:
		Window() :m_Hwnd(NULL),m_Height(0),m_Width(0) {}
		Window(const TCHAR* appName, UINT width, UINT height) {
			Init(appName, width, height);
		}

		void Destroy() {
			UnregisterClass(m_AppName, m_Instance);
		}

		HWND GetHwnd() { return m_Hwnd; }
		UINT GetWidth() { return m_Width; }
		UINT GetHeight() { return m_Height; }
	};
}


