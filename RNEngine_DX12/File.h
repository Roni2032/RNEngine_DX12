#pragma once
#include "stdafx.h"
namespace RNEngine {
	wstring ConvertStrToWstr(const string& str);
	string ConvertWstrToStr(const wstring& str);

	class File
	{
	public:
		static vector<filesystem::directory_entry> GetDirectoryFile(const wstring& path);
		static bool IsExistFile(const wstring& path);
	};
}
