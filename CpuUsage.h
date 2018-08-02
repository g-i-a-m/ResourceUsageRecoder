#ifndef _CPU_USAGE_H_
#define _CPU_USAGE_H_

#include <windows.h>

class CpuUsage
{
public:
	CpuUsage(DWORD dwProcessID);
	ULONGLONG GetUsageEx();
	ULONGLONG GetSystemNonIdleTimes();
	ULONGLONG GetProcessNonIdleTimes();
private:
	ULONGLONG SubtractTimes(const FILETIME& ftA, const FILETIME& ftB);
	ULONGLONG AddTimes(const FILETIME& ftA, const FILETIME& ftB);
	bool EnoughTimePassed();
	inline bool IsFirstRun() const { return (m_dwLastRun == 0); }

	//system total times
	FILETIME m_ftPrevSysKernel;
	FILETIME m_ftPrevSysUser;

	//process times
	FILETIME m_ftPrevProcKernel;
	FILETIME m_ftPrevProcUser;

	ULONGLONG m_ullPrevSysNonIdleTime;//这个变量和后面的便利记录上次获取的非idle的系统cpu时间和进程cpu时间.
	ULONGLONG m_ullPrevProcNonIdleTime;//这个类只绑定一个进程, 在构造函数里面初始化进来..

	ULONGLONG m_nCpuUsage;
	ULONGLONG m_dwLastRun;
	DWORD m_dwProcessID;
	HANDLE m_hProcess;
	volatile LONG m_lRunCount;
};

class CPUusage
{
private:
	typedef long long          int64_t;
	typedef unsigned long long uint64_t;
	HANDLE _hProcess;
	int _processor;				//cpu数量  
	int64_t _last_time;         //上一次的时间  
	int64_t _last_system_time;


	// 时间转换  
	uint64_t file_time_2_utc(const FILETIME* ftime);

	// 获得CPU的核数  
	int get_processor_number();

	//初始化
	void init()
	{
		_last_system_time = 0;
		_last_time = 0;
		_hProcess = 0;
	}

	//关闭进程句柄
	void clear()
	{
		if (_hProcess)
		{
			CloseHandle(_hProcess);
			_hProcess = 0;
		}
	}

public:
	CPUusage(DWORD ProcessID)
	{
		init();
		_processor = get_processor_number();
		setpid(ProcessID);
	}
	CPUusage()
	{
		init();
		_processor = get_processor_number();
	}
	~CPUusage()
	{
		clear();
	}

	//返回值为进程句柄，可判断OpenProcess是否成功
	HANDLE setpid(DWORD ProcessID)
	{
		clear();    //如果之前监视过另一个进程，就先关闭它的句柄
		init();
		return _hProcess = OpenProcess(PROCESS_QUERY_LIMITED_INFORMATION, false, ProcessID);
	}

	//-1 即为失败或进程已退出； 如果成功，首次调用会返回-2（中途用setpid更改了PID后首次调用也会返回-2）
	float get_cpu_usage();
};

#endif
