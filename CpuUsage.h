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
private:
	typedef long long          int64_t;
	typedef unsigned long long uint64_t;
	HANDLE _hProcess;
	int _processor;				//cpu����  
	int64_t _last_time;         //��һ�ε�ʱ��  
	int64_t _last_system_time;


	// ʱ��ת��  
	uint64_t file_time_2_utc(const FILETIME* ftime);

	// ���CPU�ĺ���  
	int get_processor_number();

	//��ʼ��
	void init()
	{
		_last_system_time = 0;
		_last_time = 0;
		_hProcess = 0;
	}

	//�رս��̾��
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

	//����ֵΪ���̾�������ж�OpenProcess�Ƿ�ɹ�
	HANDLE setpid(DWORD ProcessID)
	{
		clear();    //���֮ǰ���ӹ���һ�����̣����ȹر����ľ��
		init();
		return _hProcess = OpenProcess(PROCESS_QUERY_LIMITED_INFORMATION, false, ProcessID);
	}

	//-1 ��Ϊʧ�ܻ�������˳��� ����ɹ����״ε��û᷵��-2����;��setpid������PID���״ε���Ҳ�᷵��-2��
	float get_cpu_usage();
};

#endif
