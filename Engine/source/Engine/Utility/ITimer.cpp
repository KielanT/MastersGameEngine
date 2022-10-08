#include "epch.h"
#include "ITimer.h"

namespace Engine
{
	

	void ITimer::Start()
	{
		Reset();
		mTimeScale = 1.0f;
		mDetlaTime = std::chrono::duration<float>(0.0f);
	}
	
	
	void ITimer::Reset()
	{
		mStartTime = Clock::now();
		
	}
	
	float ITimer::DeltaTime()
	{
		Tick();
		Reset();
		return mDetlaTime.count();
	}
	
	void ITimer::TimeScale(float t)
	{
		mTimeScale = t;
	}
	
	float ITimer::TimeScale()
	{
		return mTimeScale;
	}
	
	void ITimer::Tick()
	{
		mDetlaTime = Clock::now() - mStartTime;
		m_ElapsedTime += mDetlaTime.count();
	}
}