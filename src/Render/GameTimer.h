#pragma once

#include <chrono>

class GameTimer
{

private:
	std::chrono::high_resolution_clock::time_point m_startTime;
	std::chrono::high_resolution_clock::time_point m_endTime;
	std::chrono::high_resolution_clock::time_point m_stopTime;

	float m_deltaTime;
	float m_totalTime;

	bool m_isStopped = false;
public:
	GameTimer();
	~GameTimer();
	void Start();
	void Stop();
	void Reset();
	void Tick();
	float TotalTime() const;
	float DeltaTime() const;
};

