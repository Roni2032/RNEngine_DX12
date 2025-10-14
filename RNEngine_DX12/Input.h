#pragma once

#include "stdafx.h"
#include "Singleton.h"
namespace RNEngine
{
	class Input : public SingletonBase<Input>
	{
	private:
		friend class SingletonBase<Input>;
		Input() = default;
		~Input() = default;

		enum class State {
			PRESS,
			RELEASE,
			HOLD
		};

		map<wstring, State> m_KeyState; //キーの状態を保存するマップ
	public:
		/// <summary>
		/// キー入力の初期化
		/// </summary>
		void Init();
		/// <summary>
		/// キー入力の更新
		/// </summary>
		void Update();
	};

}

