#include "SystemInfo.h"

SystemInfo::SystemInfo()
{
}

SystemInfo::~SystemInfo()
{
}

void SystemInfo::poll()
{
	cores = SDL_GetCPUCount();
	threads = THREAD_POOL_MAX_THREADS + 2;
	
	// TODO: Fix this
	//vsync = ( SDL_GL_GetSwapInterval() == 1 );
	vsync = true;
	
#ifdef _WIN32
	PROCESS_MEMORY_COUNTERS memInfo;
	GetProcessMemoryInfo( GetCurrentProcess(), &memInfo, sizeof(memInfo) );

	ram = memInfo.PagefileUsage;
#else
	struct rusage memInfo;
	getrusage( RUSAGE_SELF, &memInfo );
	
	ram = memInfo.ru_maxrss;
#endif
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
