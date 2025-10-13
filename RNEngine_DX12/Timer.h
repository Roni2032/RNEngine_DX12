#pragma once
#include "stdafx.h"
namespace RNEngine {

	class Timer
	{
	private:
		system_clock::time_point m_BeforeTime;
		system_clock::time_point m_CurrentTime;
		float m_DeltaTime;

	public:
		Timer() : m_DeltaTime(0.0f) {}
		~Timer() {}
		/// <summary>
		/// ������
		/// </summary>
		void Init() {
			m_BeforeTime = system_clock::now();
		}
		/// <summary>
		/// �O�񂩂�̌o�ߎ��Ԃ��v�Z
		/// CheckTime�Ƃ̓����g�p�͕s��
		/// </summary>
		void CalcDelta() {
			m_CurrentTime = system_clock::now();
			m_DeltaTime = duration_cast<milliseconds>(m_CurrentTime - m_BeforeTime).count() * 0.001f;
			m_BeforeTime = m_CurrentTime;
		}

		/// <summary>
		/// �w�肵���b�����o�߂������`�F�b�N
		/// </summary>
		/// <param name="time">�b��</param>
		/// <returns>�o�߂������̔���(true���Ԃ�ƃ��Z�b�g)</returns>
		bool CheckTime(float time) {
			m_CurrentTime = system_clock::now();
			if (duration_cast<milliseconds>(m_CurrentTime - m_BeforeTime).count() * 0.001f >= time) {
				m_BeforeTime = m_CurrentTime;
				return true;
			}
			return false;
		}
		float GetDeltaTime() { return m_DeltaTime; }
	};

}

