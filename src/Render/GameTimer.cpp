#include "GameTimer.h"

#include <iostream>

GameTimer::GameTimer()
{
}

GameTimer::~GameTimer()
{
}

void GameTimer::Start()
{
	if (m_isStopped)
	{
		auto resumeTime = std::chrono::high_resolution_clock::now();

		
		auto pauseDuration = resumeTime - m_stopTime;

		m_startTime += pauseDuration;

		m_endTime = resumeTime;
		m_isStopped = false;
	}
	else
	{
		std::cout << "Game Timer Started" << std::endl;
		m_startTime = std::chrono::high_resolution_clock::now();
		m_endTime = m_startTime;
		m_totalTime = 0.f;
		m_deltaTime = 0.f;
	}
	
}

void GameTimer::Stop()
{
	if (!m_isStopped)
	{
		m_stopTime = std::chrono::high_resolution_clock::now();
		m_isStopped = true;
	}
}

void GameTimer::Reset()
{
	Start();
}

void GameTimer::Tick()

{
	if (m_isStopped)
	{
		m_deltaTime = 0.f;
		return;
	}
	//std::cout << "Game Timer Tick" << std::endl;

	auto currTime = std::chrono::high_resolution_clock::now();

	m_totalTime = std::chrono::duration<float>(currTime - m_startTime).count();
	m_deltaTime = std::chrono::duration<float>(currTime - m_endTime).count();

	m_endTime = currTime;

	/*std::cout << "TotalTime : " << m_totalTime << std::endl;
	std::cout << "DeltaTime : " << m_deltaTime << std::endl;*/
}

float GameTimer::TotalTime() const
{
	return m_totalTime;
}

float GameTimer::DeltaTime() const
{
	return m_deltaTime;
}
