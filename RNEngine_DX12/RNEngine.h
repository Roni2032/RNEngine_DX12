#pragma once
#include "stdafx.h"
#include "Window.h"
#include "Timer.h"
#include "Device.h"
#include "Renderer.h"
#include "GameObject.h"
#include "Component.h"

namespace RNEngine {

	class RnEngine
	{
	private:
		Window m_Window;		// �E�B���h�E
		Renderer m_Renderer;	// �`�揈��
		Device m_Device;		// �f�o�C�X
		Timer m_Timer;			// �^�C�}�[
		Timer m_FrameTimer;		// �t���[���^�C�}�[

		float m_FrameRate;		// �t���[�����[�g

		//���̃f�[�^�z��(��X�V�[���N���X������)
		vector<GameObject*> m_GameObjects; // �Q�[���I�u�W�F�N�g�̔z��
		vector<Component*> m_Components; // �R���|�[�l���g�̔z��

		void EnableDebugLayer();
	public:

		static RnEngine* g_pInstance;

		RnEngine() : m_Window(),m_FrameRate(120.0f) { g_pInstance = this; }
		~RnEngine() {}

		/// <summary>
		/// ������
		/// </summary>
		void Init();

		/// <summary>
		/// �j��
		/// </summary>
		void Destroy();

		/// <summary>
		/// �X�V
		/// </summary>
		void Update();

		Device GetDevice() { return m_Device; }
		Renderer GetRenderer() { return m_Renderer; }
		Window GetWindow() { return m_Window; }

		float GetFrameRate() {
			return m_FrameRate;
		}
		void SetFrameRate(float frame) {
			m_FrameRate = frame;
		}
	};
}

