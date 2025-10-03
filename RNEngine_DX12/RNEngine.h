#pragma once
namespace RNEngine {
	LRESULT WindowProcedure(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam);

	class RnEngine
	{
	private:
		HWND InitWindow(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPWSTR lpCmdLine, int nCmdShow, const TCHAR* appName);
	public:
		RnEngine() = default;

		/// <summary>
		/// èâä˙âª
		/// </summary>
		void Init();

		/// <summary>
		/// îjä¸
		/// </summary>
		void Destroy();
	};
}

