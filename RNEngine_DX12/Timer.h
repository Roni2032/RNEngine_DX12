#pragma once
#include "stdafx.h"
namespace RNEngine {

	class Timer
	{
		using clock = std::chrono::high_resolution_clock;
	protected:
		clock::time_point m_BeforeTime;
		clock::time_point m_CurrentTime;
		float m_DeltaTime;

	public:
		Timer() : m_DeltaTime(0.0f) {}
		~Timer() {}
		/// <summary>
		/// 初期化
		/// </summary>
		void Init() {
			m_BeforeTime = clock::now();
			timeBeginPeriod(1);
		}

		/// <summary>
		/// 更新処理
		/// </summary>
		void Update() {
			m_CurrentTime = clock::now();
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
			m_CurrentTime = clock::now();
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
			// 現在時刻
			m_CurrentTime = clock::now();
			// 経過時間を ms 単位で取得
			double elapsed = std::chrono::duration<double, milli>(m_CurrentTime - m_BeforeTime).count();
			double delay = fps - elapsed;

			// 残余時間が1ms以上あれば Sleep
			if (delay > 1.0) {
				Sleep(static_cast<DWORD>(delay));
			}

			//待った結果をデータに保存
			m_CurrentTime = clock::now();
			m_DeltaTime = std::chrono::duration<float>(m_CurrentTime - m_BeforeTime).count();
			// 次フレーム用に lastTime を更新
			m_BeforeTime = clock::now();
			return m_DeltaTime;
		}

		float GetFps() {
			return 1.0f / m_DeltaTime;
		}
	};
}

