#pragma once

namespace Engine
{

	typedef std::chrono::high_resolution_clock Clock;
	class ITimer
	{
	public:
		void Start();

		
		float DeltaTime();
		void TimeScale(float t = 1.0f);
		float TimeScale();
		
		float GetElapsedTime()
		{
			
			return m_ElapsedTime;
		}
		
		int GetFrameRate()
		{
			return 1 / DeltaTime();
		}

	private:
		void Reset();
		void Tick();

	private:
		
		Clock::time_point mStartTime;
		std::chrono::duration<float> mDetlaTime;
		
		float mTimeScale;
		
		float m_ElapsedTime;
	};
}

