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
	BOOL SetPrivilege(HANDLE hProcess, LPCTSTR lpszPrivilege, BOOL bEnablePrivilege);
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
public:
	CPUusage() :_last_system_time(0), _last_time(0), _hProcess(NULL), m_bNormal(false)
	{
	}

	~CPUusage()
	{
		//Init()传进来的HANDLE,由外部去关闭_hProcess
		/*if (_hProcess)
		{
			CloseHandle(_hProcess);
			_hProcess = 0;
		}*/
	}

	int Init(HANDLE hProcess)
	{
		_processor = get_processor_number();
		_hProcess = hProcess;
		//_hProcess = OpenProcess(PROCESS_QUERY_LIMITED_INFORMATION, false, ProcessID);
		float ret = GetCpuUsageRate();
		if ((int)ret == -2)
		{
			m_bNormal = true;
			return 0;
		}
		else
		{
			return -1;
		}
	}

	bool IsNormal() { return m_bNormal;	}

	//-1 即为失败或进程已退出； 如果成功，首次调用会返回-2
	float GetCpuUsageRate();

private:
	// 时间转换
	unsigned __int64 file_time_2_utc(const FILETIME* ftime);
	// 获得CPU的核数
	int get_processor_number();

private:
	bool m_bNormal;
	HANDLE _hProcess;
	int _processor;				//cpu数量
	__int64 _last_time;         //上一次的时间
	__int64 _last_system_time;

	
};

#endif
