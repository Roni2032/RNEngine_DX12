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
	public:
		/// <summary>
		/// �L�[���͂̏�����
		/// </summary>
		void Init() {
		}
		/// <summary>
		/// �L�[���͂̍X�V
		/// </summary>
		void Update() {

		}
	};

}

