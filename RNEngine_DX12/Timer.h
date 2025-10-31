#pragma once
#include "stdafx.h"
namespace RNEngine {

	class Timer
	{
	protected:
		steady_clock::time_point m_BeforeTime;
		steady_clock::time_point m_CurrentTime;
		float m_DeltaTime;

	public:
		Timer() : m_DeltaTime(0.0f) {}
		~Timer() {}
		/// <summary>
		/// ������
		/// </summary>
		void Init() {
			m_BeforeTime = steady_clock::now();
			timeBeginPeriod(1);
		}

		/// <summary>
		/// �X�V����
		/// </summary>
		void Update() {
			m_CurrentTime = steady_clock::now();
			m_DeltaTime = duration<float>(m_CurrentTime - m_BeforeTime).count();
			m_BeforeTime = m_CurrentTime;
		}

		/// <summary>
		/// �w�肵���b�����o�߂������`�F�b�N
		/// �Ō��Init()�����s�܂���true���Ԃ���������̎���
		/// </summary>
		/// <param name="time">�b��</param>
		/// <returns>�o�߂������̔���(true���Ԃ�ƃ��Z�b�g)</returns>
		bool CheckTime(float time) {
			m_CurrentTime = steady_clock::now();
			float delta = duration_cast<milliseconds>(m_CurrentTime - m_BeforeTime).count() * 0.001f;
			if (delta >= time) {
				m_BeforeTime = m_CurrentTime;
				return true;
			}
			return false;
		}
		float GetDeltaTime() { return m_DeltaTime; }
	};

	class FrameTimer : public Timer {
	public:
		FrameTimer() : Timer() {}
		~FrameTimer() {}

		/// <summary>
		/// �t���[������p
		/// </summary>
		/// <param name="fps"></param>
		/// <returns></returns>
		float WaitFrame(float fps) {
			const float frameTime = 1.0f / fps;
			m_CurrentTime = steady_clock::now();
			float delta = duration<float>(m_CurrentTime - m_BeforeTime).count();

			float sleepTime = frameTime - delta;
			if (sleepTime > 0) {
				this_thread::sleep_for(milliseconds((long long)(sleepTime * 1000)));
			}
			m_CurrentTime = steady_clock::now();
			m_DeltaTime = duration<float>(m_CurrentTime - m_BeforeTime).count();
			m_BeforeTime = m_CurrentTime;
			return m_DeltaTime;
		}
	};
}

