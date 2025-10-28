#pragma once
#include "stdafx.h"
namespace RNEngine {

	class Timer
	{
	private:
		steady_clock::time_point m_BeforeTime;
		steady_clock::time_point m_CurrentTime;
		float m_DeltaTime;

	public:
		Timer() : m_DeltaTime(0.0f) {}
		~Timer() {}
		/// <summary>
		/// 初期化
		/// </summary>
		void Init() {
			m_BeforeTime = steady_clock::now();
		}
		/// <summary>
		/// 前回からの経過時間を計算
		/// CheckTimeとの同時使用は不可
		/// </summary>
		float CalcDelta() {
			m_CurrentTime = steady_clock::now();
			m_DeltaTime = duration_cast<milliseconds>(m_CurrentTime - m_BeforeTime).count() * 0.001f;
			m_BeforeTime = m_CurrentTime;
			return m_DeltaTime;
		}

		/// <summary>
		/// 指定した秒数が経過したかチェック
		/// </summary>
		/// <param name="time">秒数</param>
		/// <returns>経過したかの判定(trueが返るとリセット)</returns>
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

}

