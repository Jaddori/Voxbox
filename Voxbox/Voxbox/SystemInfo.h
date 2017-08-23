#pragma once

#ifdef _WIN32
#include <Windows.h>
#include <Psapi.h>
#endif
#include "SDL.h"
#include "ThreadPool.h"

class SystemInfo
{
public:
	SystemInfo();
	~SystemInfo();

	void poll();

	int getCores() const;
	int getThreads() const;
	int getRam() const;
	bool getVsync() const;

private:
	int cores;
	int threads;
	int ram;
	bool vsync;
};
