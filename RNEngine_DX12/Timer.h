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
		/// 初期化
		/// </summary>
		void Init() {
			m_BeforeTime = system_clock::now();
		}
		/// <summary>
		/// 前回からの経過時間を計算
		/// CheckTimeとの同時使用は不可
		/// </summary>
		void CalcDelta() {
			m_CurrentTime = system_clock::now();
			m_DeltaTime = duration_cast<milliseconds>(m_CurrentTime - m_BeforeTime).count() * 0.001f;
			m_BeforeTime = m_CurrentTime;
		}

		/// <summary>
		/// 指定した秒数が経過したかチェック
		/// </summary>
		/// <param name="time">秒数</param>
		/// <returns>経過したかの判定(trueが返るとリセット)</returns>
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

