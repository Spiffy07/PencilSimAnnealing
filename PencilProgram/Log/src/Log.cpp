
#include "Log.h"


void Log::SetLogLevel(LogLevel level)
{
	m_LogLevel = level;
}

void Log::LogError(const char* message)
{
	if (m_LogLevel >= Error)
		std::cout << message << std::endl;
}

void Log::LogWarning(const char* message)
{
	if (m_LogLevel >= Warning)
		std::cout << "  " << message << std::endl;
}

void Log::LogInfo(const char* message)
{
	if (m_LogLevel >= Info)
		std::cout << "    " << message << std::endl;
}
