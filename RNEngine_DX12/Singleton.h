#pragma once
#include "stdafx.h"

namespace RNEngine{
	template<class T>
	class SingletonBase {
	protected:
		SingletonBase() = default;
	public:
		~SingletonBase(){}

		static T& GetInstance() {
			static T instance;
			return instance;
		}

	private:
		SingletonBase(const SingletonBase&) = delete;
		SingletonBase& operator=(const SingletonBase&) = delete;

	};

	///----------------------------------------------------------------
	/// Singleton ヘッダ
	/// 
	/// 使用方法
	/// ・継承によってシングルトン化
	/// ・SingletonBaseを継承したクラスをGetInstanceで取得
	/// ・friend classでSingletonBase<T>を指定する
	/// ・コンストラクタはdefaultで良い
	/// ----------------------------------------------------------------
}
