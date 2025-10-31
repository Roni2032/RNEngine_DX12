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
		/// 初期化
		/// </summary>
		void Init() {
			m_BeforeTime = steady_clock::now();
			timeBeginPeriod(1);
		}

		/// <summary>
		/// 更新処理
		/// </summary>
		void Update() {
			m_CurrentTime = steady_clock::now();
			m_DeltaTime = duration<float>(m_CurrentTime - m_BeforeTime).count();
			m_BeforeTime = m_CurrentTime;
		}

		/// <summary>
		/// 指定した秒数が経過したかチェック
		/// 最後にInit()を実行またはtrueが返った時からの時間
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

	class FrameTimer : public Timer {
	public:
		FrameTimer() : Timer() {}
		~FrameTimer() {}

		/// <summary>
		/// フレーム制御用
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

