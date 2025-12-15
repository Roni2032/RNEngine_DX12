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
}