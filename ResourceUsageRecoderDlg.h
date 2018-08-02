
// Resource usage recoderDlg.h : 头文件
//

#pragma once

#include "ChartCtrl.h"
#include "ChartLineSerie.h"
#include "ChartStandardAxis.h"
#include "afxwin.h"
#include "ChartDialog.h"
#include "ConfigRecoder.h"
#include "TipsDlg.h"
#include "PictureEx.h"
#include "CpuUsage.h"

#define TIMER_SAMPLING_MSG WM_USER+1

// CResourceUsageRecoderDlg 对话框
class CResourceUsageRecoderDlg : public CDialogEx
{
// 构造
public:
	CResourceUsageRecoderDlg(CWnd* pParent = NULL);	// 标准构造函数

// 对话框数据
	enum { IDD = IDD_RESOURCEUSAGERECODER_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持

private:
	double GetCPUInfoByPid(int pid);
    double GetMemoryInfoByPid(int pid);
	double GetHandleInfoByPid(int pid);
	double GetThreadInfoByPid(int pid);

// 实现
protected:
	HICON m_hIcon;

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()

private:
    RecoderInfo m_info;

	CButton m_btn_start;
	CButton m_btn_stop;
	CPictureEx m_picGif;
	CConfigRecoder* m_pConfigRecoderDlg;//记录器配置子窗口
	CChartDialog* m_pChartCtrlCPU;		//CPU资源
	CChartDialog* m_pChartCtrlMemoy;	//内存消耗
	CChartDialog* m_pChartCtrlHandle;	//句柄消耗
	CChartDialog* m_pChartCtrlThread;   //线程个数
    CChartDialog* m_pChartCtrlIO;       //IO读写
	CChartDialog* m_pChartCtrlNonpaged; //非页面缓冲池
    UINT m_iSampRateSecond;             //TIMER间隔 / 数据采集间隔

	//CpuUsage* m_pCPUAgent;
	CPUusage* m_pCPUAgent;

public:
    afx_msg void OnClose();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	virtual BOOL DestroyWindow();
	afx_msg void OnBnClickedBtnRecode();
	afx_msg void OnBnClickedBtnStop();
};
