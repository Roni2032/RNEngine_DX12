#pragma once


namespace RNEngine
{

	LRESULT WindowProcedure(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam);
	void WindowKeyDownProcedure(HWND hwnd, WPARAM wparam);
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
		Window() : m_Hwnd(NULL), m_Height(0), m_Width(0), m_Instance{} {}
		Window(const wstring& appName, UINT width, UINT height):m_Width(width), m_Height(height) {
			Create(appName, width, height);
		}

		static void GetDesktopWindowSize(UINT* width, UINT* height) {
			RECT rect;
			HWND desktop = GetDesktopWindow();
			GetWindowRect(desktop, &rect);
			*width = rect.right;
			*height = rect.bottom;
		}

		void Destroy() {
			UnregisterClass(m_AppName.c_str(), m_Instance);
		}

		bool ProcessMessage();

		HWND GetHwnd()const { return m_Hwnd; }
		UINT GetWidth()const { return m_Width; }
		UINT GetHeight()const { return m_Height; }

		RECT GetClientRect()const {
			RECT rect;
			::GetClientRect(m_Hwnd, &rect);
			return rect;
		}
		void ScreenToClient(POINT* point)const {
			::ScreenToClient(m_Hwnd, point);
		}
	};
}


