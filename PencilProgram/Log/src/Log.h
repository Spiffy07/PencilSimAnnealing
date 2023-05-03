#pragma once

#include <iostream>
#include <string>

class Log
{
	int m_LogLevel;

public:
	enum LogLevel
	{
		Error, Warning, Info
	};


	void SetLogLevel(LogLevel level);
	void LogError(const char* message);
	void LogWarning(const char* message);
	void LogInfo(const char* message);
};