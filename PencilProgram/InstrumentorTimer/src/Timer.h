#pragma once

#include <chrono>
#include <iostream>
#include <string>
#include <algorithm>
#include <fstream>

#include <thread>

//struct Timer
//{
//	std::chrono::time_point<std::chrono::steady_clock> start, end;
//	std::chrono::duration<float> duration;
//
//	Timer();
//	~Timer();
//
//	void endTimer();
//};

struct ProfileResult
{
	std::string Name;
	long long Start, End;
	uint32_t ThreadID;
};

struct InstrumentationSession
{
	std::string Name;
};

class Instrumentor
{
private:
	InstrumentationSession* m_CurrentSession;
	std::ofstream m_OutputStream;
	int m_ProfileCount;

public:
	Instrumentor();
	void BeginSession(const std::string& name, const std::string& filepath = "ProfileResults.json");
	void EndSession();
	void WriteProfile(const ProfileResult& result);
	void WriteHeader();
	void WriteFooter();
	static Instrumentor& Get();
};

class InstrumentationTimer
{
private:
	const char* m_Name;
	std::chrono::time_point<std::chrono::high_resolution_clock> m_StartTimepoint;
	bool m_Stopped;

public:
	InstrumentationTimer(const char* name);
	~InstrumentationTimer();
	void Stop();
};