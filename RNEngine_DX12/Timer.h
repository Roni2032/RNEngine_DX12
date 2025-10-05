#pragma once
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
		/// ‰Šú‰»
		/// </summary>
		void Init() {
			m_BeforeTime = system_clock::now();
		}
		/// <summary>
		/// XV
		/// </summary>
		void UpdateDelta() {
			m_CurrentTime = system_clock::now();
			m_DeltaTime = duration_cast<milliseconds>(m_CurrentTime - m_BeforeTime).count() * 0.001f;
			m_BeforeTime = m_CurrentTime;
		}
		float GetDeltaTime() { return m_DeltaTime; }
	};

}

