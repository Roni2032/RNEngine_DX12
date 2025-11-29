#pragma once
namespace RNEngine {
	class Util
	{
	public:
		static wstring ConvertStrToWstr(const string& str);
		static string ConvertWstrToStr(const wstring& str);

		template<typename T>
		static T Random(T min = 0.0f, T max = 1.0f) {
			static thread_local random_device rd;
			static thread_local mt19937 gen(rd());
			
			if constexpr (is_integral_v<T>) {
				uniform_int_distribution<T> dist(min, max);
				return dist(gen);
			}
			else if constexpr (is_floating_point_v<T>) {
				uniform_real_distribution<T> dist(min, max);
				return dist(gen);
			}
		}
	};
}
