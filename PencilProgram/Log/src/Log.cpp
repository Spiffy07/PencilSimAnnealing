
#include "Log.h"


void Log::SetLogLevel(LogLevel level)
{
	m_LogLevel = level;
}

void Log::LogError(std::string message)
{
	if (m_LogLevel >= Error)
		std::cout << message << std::endl;
}

void Log::LogWarning(std::string message)
{
	if (m_LogLevel >= Warning)
		std::cout << "  " << message << std::endl;
}

void Log::LogInfo(std::string message)
{
	if (m_LogLevel >= Info)
		std::cout << "    " << message << std::endl;
}
