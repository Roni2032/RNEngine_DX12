#pragma once
namespace RNEngine
{
	LRESULT WindowProcedure(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam);

	class Window
	{
	private:
		HWND m_hwnd;
		void Init(const TCHAR* appName, int width, int height);
	public:
		Window() :m_hwnd(NULL) {}
		Window(const TCHAR* appName, int width, int height) {
			Init(appName, width, height);
		}

		HWND GetHwnd() { return m_hwnd; }
	};
}


