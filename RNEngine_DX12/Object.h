#pragma once
//----------------------------------------------------------------
//	Object �w�b�_
// 
//  �Q�[�����Ŏg�p����I�u�W�F�N�g�̊��N���X
//  ��������GameObject��Component�Ȃǂ��h�����Ă���
//
//----------------------------------------------------------------
#include "stdafx.h"

namespace RNEngine {

	class Object
	{
	public:
		Object(){}
		virtual ~Object() {}

		/// <summary>
		/// �쐬���ꂽ�t���[���ň�x�����Ă΂��
		/// </summary>
		virtual void Start(){}

		/// <summary>
		/// ���t���[���Ă΂��
		/// </summary>
		virtual void Update(float delta) {}

		/// <summary>
		/// ���ׂĂ�Update()���I�������ɌĂ΂��
		/// </summary>
		virtual void LateUpdate(float delta) {}

		/// <summary>
		/// �`�揈���̍ۂɌĂ΂��
		/// </summary>
		virtual void Draw() {}

		/// <summary>
		/// �j���̍ۂɌĂ΂��
		/// </summary>
		virtual void Destroy() {}
	};

}
