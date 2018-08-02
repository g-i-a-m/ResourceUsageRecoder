#include "stdafx.h"
#include "ProcessResourceStatistics.h"

#include <psapi.h>
#pragma comment(lib,"Psapi.lib")

ProcessResourceStatistics::ProcessResourceStatistics(int pid) : m_hProcess(NULL)
{
	m_hProcess = OpenProcess(PROCESS_QUERY_INFORMATION | PROCESS_VM_READ, FALSE, pid);
	if (m_hProcess!=NULL)
		m_cpu.Init(m_hProcess);
}

ProcessResourceStatistics::~ProcessResourceStatistics()
{
	if (m_hProcess != NULL)
	{
		CloseHandle(m_hProcess);
		m_hProcess = NULL;
	}
}

int ProcessResourceStatistics::IsRunning()
{
	int iRet = -1;
	if (m_hProcess != NULL)
	{
		DWORD exitcode;
		GetExitCodeProcess(m_hProcess, &exitcode);
		if (exitcode != STILL_ACTIVE)
		{
			CloseHandle(m_hProcess);
			m_hProcess = 0;
		}
		else
		{
			iRet = 0;
		}
	}
	
	return iRet;
}

double ProcessResourceStatistics::GetCPUInfo()
{
	double cpuUsaeRate = 0.0;
	if (m_hProcess != NULL && m_cpu.IsNormal())
	{
		cpuUsaeRate = m_cpu.GetCpuUsageRate();
	}
	return cpuUsaeRate;
}

double ProcessResourceStatistics::GetMemoryInfo()
{
	double memMB = 0;
	if (m_hProcess != NULL)
	{
		PROCESS_MEMORY_COUNTERS pmc;
		::GetProcessMemoryInfo(m_hProcess, &pmc, sizeof(pmc));
		memMB = pmc.WorkingSetSize / 1024.00 / 1024.00; //MByte
	}
	return memMB;
}

double ProcessResourceStatistics::GetHandleInfo()
{
	DWORD dwHandleCount = 0;
	if (m_hProcess != NULL)
	{
		GetProcessHandleCount(m_hProcess, &dwHandleCount);
	}
	return (double)dwHandleCount;
}

double ProcessResourceStatistics::GetThreadInfo()
{
	DWORD dwThreadCount = 0;
	if (m_hProcess != NULL)
	{

	}
	return (double)dwThreadCount;
}

