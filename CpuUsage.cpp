#include "stdafx.h"
#include <windows.h>
#include "CpuUsage.h"
#include <strsafe.h>

CpuUsage::CpuUsage(DWORD dwProcessID)
	: m_nCpuUsage(0),
	m_dwLastRun(0),
	m_lRunCount(0),
	m_dwProcessID(dwProcessID),
	m_ullPrevProcNonIdleTime(0),
	m_ullPrevSysNonIdleTime(0)
{
	HANDLE hProcess = GetCurrentProcess();
	SetPrivilege(hProcess, SE_DEBUG_NAME, TRUE);

	m_hProcess = ::OpenProcess(PROCESS_QUERY_INFORMATION, TRUE, m_dwProcessID);
	if (m_hProcess == 0)
	{
		OutputDebugString(_T("OpenProcess"));
	}
	ZeroMemory(&m_ftPrevSysKernel, sizeof(FILETIME));
	ZeroMemory(&m_ftPrevSysUser, sizeof(FILETIME));

	ZeroMemory(&m_ftPrevProcKernel, sizeof(FILETIME));
	ZeroMemory(&m_ftPrevProcUser, sizeof(FILETIME));
}

BOOL CpuUsage::SetPrivilege(HANDLE hProcess, LPCTSTR lpszPrivilege, BOOL bEnablePrivilege)
{
	HANDLE hToken;
	if (!OpenProcessToken(hProcess, TOKEN_ADJUST_PRIVILEGES, &hToken))
	{
		OutputDebugString(_T("OpenProcessToken"));
		return FALSE;
	}
	LUID luid;
	if (!LookupPrivilegeValue(NULL, lpszPrivilege, &luid))
	{
		OutputDebugString(_T("LookupPrivilegeValue"));
		return FALSE;
	}
	TOKEN_PRIVILEGES tkp;
	tkp.PrivilegeCount = 1;
	tkp.Privileges[0].Luid = luid;
	tkp.Privileges[0].Attributes = (bEnablePrivilege) ? SE_PRIVILEGE_ENABLED : FALSE;
	if (!AdjustTokenPrivileges(hToken, FALSE, &tkp, sizeof(TOKEN_PRIVILEGES), (PTOKEN_PRIVILEGES)NULL, (PDWORD)NULL))
	{
		OutputDebugString(_T("AdjustTokenPrivileges"));
		return FALSE;
	}
	return TRUE;
}

ULONGLONG CpuUsage::SubtractTimes(const FILETIME &ftA, const FILETIME &ftB)
{
	LARGE_INTEGER a, b;
	a.LowPart = ftA.dwLowDateTime;
	a.HighPart = ftA.dwHighDateTime;

	b.LowPart = ftB.dwLowDateTime;
	b.HighPart = ftB.dwHighDateTime;

	return a.QuadPart - b.QuadPart;
}

ULONGLONG CpuUsage::AddTimes(const FILETIME &ftA, const FILETIME &ftB)
{
	LARGE_INTEGER a, b;
	a.LowPart = ftA.dwLowDateTime;
	a.HighPart = ftA.dwHighDateTime;

	b.LowPart = ftB.dwLowDateTime;
	b.HighPart = ftB.dwHighDateTime;

	return a.QuadPart + b.QuadPart;
}

bool CpuUsage::EnoughTimePassed()
{
	const int minElapsedMS = 250;//milliseconds

	ULONGLONG dwCurrentTickCount = GetTickCount();
	return (dwCurrentTickCount - m_dwLastRun) > minElapsedMS;
}
#ifndef USE_DEPRECATED_FUNCS

ULONGLONG CpuUsage::GetSystemNonIdleTimes()
{
	FILETIME ftSysIdle, ftSysKernel, ftSysUser;
	if (!GetSystemTimes(&ftSysIdle, &ftSysKernel, &ftSysUser))
	{
		OutputDebugString(_T("GetSystemTimes"));
		return 0;
	}
	return AddTimes(ftSysKernel, ftSysUser);
}


ULONGLONG CpuUsage::GetProcessNonIdleTimes()
{
	FILETIME ftProcCreation, ftProcExit, ftProcKernel, ftProcUser;
	if (!GetProcessTimes(m_hProcess, &ftProcCreation, &ftProcExit, &ftProcKernel, &ftProcUser) && false)
	{
		OutputDebugString(_T("GetProcessNonIdleTimes"));
		return 0;
	}
	return AddTimes(ftProcKernel, ftProcUser);
}
#endif

ULONGLONG CpuUsage::GetUsageEx()
{
	ULONGLONG nCpuCopy = m_nCpuUsage;
	if (::InterlockedIncrement(&m_lRunCount) == 1)
	{
		if (!EnoughTimePassed())
		{
			::InterlockedDecrement(&m_lRunCount);
			return nCpuCopy;
		}
		ULONGLONG ullSysNonIdleTime = GetSystemNonIdleTimes();
		ULONGLONG ullProcNonIdleTime = GetProcessNonIdleTimes();
		if (!IsFirstRun())
		{
			ULONGLONG ullTotalSys = ullSysNonIdleTime - m_ullPrevSysNonIdleTime;
			if (ullTotalSys == 0)
			{
				::InterlockedDecrement(&m_lRunCount);
				return nCpuCopy;
			}
			m_nCpuUsage = ULONGLONG((ullProcNonIdleTime - m_ullPrevProcNonIdleTime) * 100.0 / (ullTotalSys));
			m_ullPrevSysNonIdleTime = ullSysNonIdleTime;
			m_ullPrevProcNonIdleTime = ullProcNonIdleTime;
		}
		m_dwLastRun = (ULONGLONG)GetTickCount();
		nCpuCopy = m_nCpuUsage;
	}
	::InterlockedDecrement(&m_lRunCount);
	return nCpuCopy;
}


float CPUusage::GetCpuUsageRate()
{
	FILETIME now;
	FILETIME creation_time;
	FILETIME exit_time;
	FILETIME kernel_time;
	FILETIME user_time;

	if (!_hProcess)
	{
		m_bNormal = false;
		return -1;
	}

	GetSystemTimeAsFileTime(&now);

	//判断进程是否已经退出
	DWORD exitcode;
	GetExitCodeProcess(_hProcess, &exitcode);
	if (exitcode != STILL_ACTIVE)
	{
		m_bNormal = false;
		return -1;
	}

	//计算占用CPU的百分比
	if (!GetProcessTimes(_hProcess, &creation_time, &exit_time, &kernel_time, &user_time))
	{
		m_bNormal = false;
		return -1;
	}

	__int64 system_time = (file_time_2_utc(&kernel_time) + file_time_2_utc(&user_time))	/ _processor;
	__int64 time = file_time_2_utc(&now);

	//判断是否为首次计算
	if ((_last_system_time == 0) || (_last_time == 0))
	{
		_last_system_time = system_time;
		_last_time = time;
		return -2;
	}

	__int64 system_time_delta = system_time - _last_system_time;
	__int64 time_delta = time - _last_time;

	if (time_delta == 0)
	{
		m_bNormal = false;
		return -1;
	}

	float cpu = (float)system_time_delta * 100 / (float)time_delta;
	_last_system_time = system_time;
	_last_time = time;
	return cpu;
}

unsigned __int64 CPUusage::file_time_2_utc(const FILETIME* ftime)
{
	LARGE_INTEGER li;

	li.LowPart = ftime->dwLowDateTime;
	li.HighPart = ftime->dwHighDateTime;
	return li.QuadPart;
}

int CPUusage::get_processor_number()
{
	SYSTEM_INFO info;
	GetSystemInfo(&info);
	return info.dwNumberOfProcessors;
}
