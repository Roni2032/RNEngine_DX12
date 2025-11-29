#include "stdafx.h"
#include "File.h"

namespace RNEngine {
	wstring ConvertStrToWstr(const string& str) {
		int sizeNeeded = MultiByteToWideChar(
			CP_UTF8,
			0,
			str.c_str(),
			(int)str.size(),
			nullptr,
			0
		);

		std::wstring result(sizeNeeded, 0);

		MultiByteToWideChar(
			CP_UTF8,
			0,
			str.c_str(),
			(int)str.size(),
			&result[0],
			sizeNeeded
		);
		return result;
	}
	string ConvertWstrToStr(const wstring& str) {
        int sizeNeeded = WideCharToMultiByte(
            CP_UTF8,
            0,
            str.c_str(),
            (int)str.size(),
            nullptr,
            0,
            nullptr,
            nullptr
        );

        // 結果用のバッファを作成
        std::string result(sizeNeeded, 0);

        // 実際に変換
        WideCharToMultiByte(
            CP_UTF8,
            0,
            str.c_str(),
            (int)str.size(),
            &result[0],
            sizeNeeded,
            nullptr,
            nullptr
        );

        return result;
	}

	
    vector<filesystem::directory_entry> File::GetDirectoryFile(const wstring& path) {
        vector<filesystem::directory_entry> directories;
        for (const std::filesystem::directory_entry& directory : std::filesystem::directory_iterator(path)) {
            directories.push_back(directory);
        }
        return directories;
	}

    bool File::IsExistFile(const wstring& path) {
        return filesystem::exists(path);
    }
}