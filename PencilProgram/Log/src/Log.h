#pragma once
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
	void LogError(std::string message);
	void LogWarning(std::string message);
	void LogInfo(std::string message);
};