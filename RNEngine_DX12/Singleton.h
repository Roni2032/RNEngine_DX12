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
	/// Singleton �w�b�_
	/// 
	/// �g�p���@
	/// �E�p���ɂ���ăV���O���g����
	/// �ESingletonBase���p�������N���X��GetInstance�Ŏ擾
	/// �Efriend class��SingletonBase<T>���w�肷��
	/// �E�R���X�g���N�^��default�ŗǂ�
	/// ----------------------------------------------------------------
}
