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
		wstring m_AppName;
		HINSTANCE m_Instance;


		void Create(const wstring& appName, UINT width, UINT height);
	public:
		Window() :m_Hwnd(NULL),m_Height(0),m_Width(0) {}
		Window(const wstring& appName, UINT width, UINT height):m_Width(width), m_Height(height) {
			Create(appName, width, height);
		}

		void Destroy() {
			UnregisterClass(m_AppName.c_str(), m_Instance);
		}

		bool ProcessMessage();

		HWND GetHwnd()const { return m_Hwnd; }
		UINT GetWidth()const { return m_Width; }
		UINT GetHeight()const { return m_Height; }
	};
}


