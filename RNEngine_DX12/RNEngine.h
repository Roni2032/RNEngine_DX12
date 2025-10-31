#pragma once
#include "stdafx.h"
#include "Window.h"
#include "Timer.h"

namespace RNEngine {
#define FRAME_NONDEFINE 10000
	class Renderer;
	class Device;
	class FrameTimer;

	class RnEngine
	{
	private:
		unique_ptr<Window> m_Window;		// �E�B���h�E
		unique_ptr <Renderer> m_Renderer;	// �`�揈��
		unique_ptr<Device> m_Device;		// �f�o�C�X
		FrameTimer m_FrameTimer;		// �t���[���^�C�}�[

		float m_FrameRate;		// �t���[�����[�g

		//���̃f�[�^�z��(��X�V�[���N���X������)
		//vector<GameObject*> m_GameObjects; // �Q�[���I�u�W�F�N�g�̔z��
		//vector<Component*> m_Components; // �R���|�[�l���g�̔z��

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

		unique_ptr <Device>& GetDevice() { return m_Device; }
		unique_ptr <Renderer>& GetRenderer() { return m_Renderer; }
		unique_ptr<Window>& GetWindow() { return m_Window; }

		float GetFrameRate() {
			return m_FrameRate;
		}
		void SetFrameRate(float frame) {
			m_FrameRate = frame;
		}
	};
}

