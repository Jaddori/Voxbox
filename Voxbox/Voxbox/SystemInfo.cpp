#include "SystemInfo.h"

SystemInfo::SystemInfo()
{
}

SystemInfo::~SystemInfo()
{
}

void SystemInfo::poll()
{
	SYSTEM_INFO sysInfo;
	GetSystemInfo( &sysInfo );

	cores = sysInfo.dwNumberOfProcessors;

	threads = THREAD_POOL_MAX_THREADS + 2; // +2 = 1 update, 1 render

	PROCESS_MEMORY_COUNTERS memInfo;
	GetProcessMemoryInfo( GetCurrentProcess(), &memInfo, sizeof(memInfo) );

	ram = memInfo.PagefileUsage;

	// TODO: Fix this
	//vsync = ( SDL_GL_GetSwapInterval() == 1 );
	vsync = true;
}

int SystemInfo::getRam() const
{
	return ram;
}

int SystemInfo::getCores() const
{
	return cores;
}

int SystemInfo::getThreads() const
{
	return threads;
}

bool SystemInfo::getVsync() const
{
	return vsync;
}