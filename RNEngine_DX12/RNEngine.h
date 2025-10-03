#pragma once
namespace RNEngine {
	class RnEngine
	{
	private:
		HRESULT InitWindow(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPWSTR lpCmdLine, int nCmdShow, const TCHAR* appName);
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

