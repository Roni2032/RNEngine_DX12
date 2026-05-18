#pragma once
#include "stdafx.h"
#include "Reflection.h"
namespace RNEngine {
	/*class Serialize
	{
		void ConvertJsonToBinary();
	public:
		void Save();
		void Load();

		void Register(shared_ptr<ReflectInterface> reflect);
	};*/

	class SerializeInterface {
	public:
		template<typename T,typename V>
		size_t GetOffset(V T::* member) {
			union {
				T obj;
				char data[sizeof(T)];
			}unionData{};

			for (size_t i = 0; i != sizeof(T); ++i) {
				if (&(unionData.obj.*member) == (V*)&unionData.data[i]) {
					return i;
				}
			}

			//メンバーが見つからない
			return -1;
		}
	};
}
