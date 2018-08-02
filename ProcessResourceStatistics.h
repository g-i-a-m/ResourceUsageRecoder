#ifndef _PROCESSRESOUCE_STATISTICS_H
#define _PROCESSRESOUCE_STATISTICS_H
#include "CpuUsage.h"

class ProcessResourceStatistics
{
public:
	ProcessResourceStatistics(int pid);
	~ProcessResourceStatistics();

	int IsRunning();
	double GetCPUInfo();
	double GetMemoryInfo();
	double GetHandleInfo();
	double GetThreadInfo();
	
private:
	HANDLE m_hProcess;
	CPUusage m_cpu;
};

#endif //_PROCESSRESOUCE_STATISTICS_H
