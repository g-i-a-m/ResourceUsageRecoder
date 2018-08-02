#pragma once

#include "ChartCtrl.h"
#include "ChartLineSerie.h"
#include "ChartStandardAxis.h"
#include "string.h"

// CChartDialog 对话框

#define WM_USER_ADDDATA WM_USER + 1

enum ENUM_TIME_UNIT
{
	_MINUTE,
	_SECOND
};

enum ENUM_CHART_TYPE
{
	_POINT,
	_LINE,
	_SURFACE,
	_BAR,
	_CANDLESTICK,
	_GANTT
};

typedef struct ProcessInfo
{
	std::string name;
	std::string id;
	std::string cpu;
	std::string mem;
	std::string handle;
	std::string thread;
	std::string nonpaged;
} *PProcessInfo;

class CChartDialog : public CDialogEx
{
	DECLARE_DYNAMIC(CChartDialog)

public:
	CChartDialog(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CChartDialog();
	void SetInitParam(std::wstring title, ENUM_TIME_UNIT unit = ENUM_TIME_UNIT::_MINUTE, const int intervalCount = 1, \
		ENUM_CHART_TYPE charttype = ENUM_CHART_TYPE::_LINE);
	void AddDataToChart(double Y);
// 对话框数据
	enum { IDD = IDD_DLG_CHARTCTRL };

private:
	void InitChartCtrl();
	COleDateTime GetMaxOleDateTime(COleDateTime& minValue);

private:
	CChartCtrl m_ChartCtrl;
	CChartDateTimeAxis* m_pBottomTimeAxis;
	CChartStandardAxis* m_pLeftAxis;
	CChartLineSerie* m_pLineSeries;

	std::wstring m_strTitle;
	ENUM_TIME_UNIT m_enum_unit;
	int m_iIntervalCount;
	ENUM_CHART_TYPE m_enum_charttype;

	COleDateTime m_minDateTime;
	COleDateTime m_maxDateTime;
	COleDateTimeSpan m_span;
    double m_yAxisMin;
	double m_yAxisMax;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()

public:
	virtual BOOL OnInitDialog();
	afx_msg LRESULT OnAddDataToChart(WPARAM wParam, LPARAM lParam);
};
