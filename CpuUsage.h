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

	ULONGLONG m_ullPrevSysNonIdleTime;//��������ͺ���ı�����¼�ϴλ�ȡ�ķ�idle��ϵͳcpuʱ��ͽ���cpuʱ��.
	ULONGLONG m_ullPrevProcNonIdleTime;//�����ֻ��һ������, �ڹ��캯�������ʼ������..

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
		//Init()��������HANDLE,���ⲿȥ�ر�_hProcess
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

	//-1 ��Ϊʧ�ܻ�������˳��� ����ɹ����״ε��û᷵��-2
	float GetCpuUsageRate();

private:
	// ʱ��ת��
	unsigned __int64 file_time_2_utc(const FILETIME* ftime);
	// ���CPU�ĺ���
	int get_processor_number();

private:
	bool m_bNormal;
	HANDLE _hProcess;
	int _processor;				//cpu����
	__int64 _last_time;         //��һ�ε�ʱ��
	__int64 _last_system_time;

	
};

#endif
