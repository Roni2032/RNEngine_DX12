#include "stdafx.h"
#include "File.h"

namespace RNEngine {
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
    wstring File::SplitFilename(const wstring& filePath) {
        string path = Util::ConvertWstrToStr(filePath);
        size_t pos = path.find_last_of("/");
        if (pos != string::npos) {
            path = path.substr(pos, path.size());
        }
        return Util::ConvertStrToWstr(path);
    }
    string File::SplitFilename(const string& filePath) {
        string path = filePath;
        size_t pos = path.find_last_of("/");
        if (pos != string::npos) {
            path = path.substr(pos, path.size());
        }
        return path;
    }

    wstring File::SplitFilePath(const wstring& filePath) {
        string path = Util::ConvertWstrToStr(filePath);
        size_t pos = path.find_last_of("/");
        if (pos != string::npos) {
            path = path.substr(0, pos + 1);
        }
        return Util::ConvertStrToWstr(path);
    }
    string File::SplitFilePath(const string& filePath) {
        string path = filePath;
        size_t pos = path.find_last_of("/");
        if (pos != string::npos) {
            path = path.substr(0, pos + 1);
        }
        return path;
    }
}