#pragma once
#include "stdafx.h"
namespace RNEngine {
	class File
	{
	public:
		static vector<filesystem::directory_entry> GetDirectoryFile(const wstring& path);
		static bool IsExistFile(const wstring& path);


		//-------ƒoƒCƒiƒŠ-------
		template<class T>
		static void ReadBinary(ifstream& ifs,T* object,uint32_t size = 1,bool useClassSize = true) {
			streamsize readSize = size;
			if (useClassSize) readSize *= sizeof(T);
			ifs.read(reinterpret_cast<char*>(object), readSize);
		}
		template<class T>
		static void SaveBinary(ofstream& ofs, T* object, uint32_t size = 1, bool useClassSize = true) {
			streamsize readSize = size;
			if (useClassSize) readSize *= sizeof(T);
			ofs.write(reinterpret_cast<char*>(object), readSize);
		}
	};
}
