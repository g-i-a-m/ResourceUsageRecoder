// ChartDialog.cpp : 实现文件
//

#include "stdafx.h"
#include "ResourceUsageRecoder.h"
#include "ChartDialog.h"
#include "afxdialogex.h"

// CChartDialog 对话框

IMPLEMENT_DYNAMIC(CChartDialog, CDialogEx)

CChartDialog::CChartDialog(CWnd* pParent /*=NULL*/)
: CDialogEx(CChartDialog::IDD, pParent),
m_enum_unit(_SECOND),
m_iIntervalCount(1),
m_enum_charttype(_LINE),
m_yAxisMin(0.0),
m_yAxisMax(0.0)
{

}

CChartDialog::~CChartDialog()
{
}

void CChartDialog::SetInitParam(std::wstring title, ENUM_TIME_UNIT unit, const int intervalCount, ENUM_CHART_TYPE charttype)
{
	m_strTitle = title;
	m_enum_unit = unit;
	m_iIntervalCount = intervalCount;
	m_enum_charttype = charttype;
}

void CChartDialog::AddDataToChart(double Y)
{
	COleDateTime TimeVal = COleDateTime::GetCurrentTime();
	COleDateTimeSpan _cur_span = TimeVal - m_minDateTime;
	double dCurrSecond = _cur_span.GetTotalSeconds();
	double dSecond = m_span.GetTotalSeconds();
	if (dCurrSecond > dSecond*0.9)
	{
		m_span = COleDateTimeSpan(0, 0, 0, dSecond*1.2);
		m_maxDateTime = m_minDateTime + m_span;
		m_pBottomTimeAxis->SetMinMax(m_minDateTime, m_maxDateTime);
		if (ENUM_TIME_UNIT::_SECOND == m_enum_unit)
		{
			double rr = m_span.GetTotalSeconds() / 10;
			m_pBottomTimeAxis->SetTickIncrement(false, CChartDateTimeAxis::tiSecond, rr);
		}
		else if (ENUM_TIME_UNIT::_MINUTE == m_enum_unit)
		{
			double rr = m_span.GetTotalSeconds() / 10;
			m_pBottomTimeAxis->SetTickIncrement(false, CChartDateTimeAxis::tiMinute, rr);
		}
	}
	
	/*if (Y < m_yAxisMin || m_yAxisMin != 0.0)
		m_yAxisMin = Y * 0.9;*/
	if (Y >= m_yAxisMax)
	{
		m_yAxisMax = Y * 1.2;
		m_pLeftAxis->SetMinMax(m_yAxisMin, m_yAxisMax);
	}

	m_pLineSeries->AddPoint(CChartCtrl::DateToValue(TimeVal), Y);
    m_ChartCtrl.RefreshCtrl();
}

void CChartDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_CHARTCTRL, m_ChartCtrl);
}


BEGIN_MESSAGE_MAP(CChartDialog, CDialogEx)
	ON_MESSAGE(WM_USER_ADDDATA, OnAddDataToChart)
END_MESSAGE_MAP()


// CChartDialog 消息处理程序


BOOL CChartDialog::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化
	InitChartCtrl();
	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常:  OCX 属性页应返回 FALSE
}

afx_msg LRESULT CChartDialog::OnAddDataToChart(WPARAM wParam, LPARAM lParam)
{
	if (lParam!=NULL)
	{
		//PProcessInfo info = (PProcessInfo)lParam;
		AddDataToChart(lParam);
	}
	return 0L;
}

void CChartDialog::InitChartCtrl()
{
	m_ChartCtrl.EnableRefresh(false);

	// Create bottom axes
	m_pBottomTimeAxis = m_ChartCtrl.CreateDateTimeAxis(CChartCtrl::BottomAxis);
	//m_pBottomTimeAxis->SetAutoHideScrollBar(true);
	m_pBottomTimeAxis->EnableScrollBar(true);
	m_pBottomTimeAxis->SetAutomaticMode(CChartAxis::NotAutomatic);
	m_minDateTime = COleDateTime::GetCurrentTime();
	m_maxDateTime = m_minDateTime;
	if (ENUM_TIME_UNIT::_SECOND == m_enum_unit)
	{
		m_span = COleDateTimeSpan(0, 0, 0, m_iIntervalCount * 10);
		m_maxDateTime += m_span;

		m_pBottomTimeAxis->SetTickIncrement(false, CChartDateTimeAxis::tiSecond, m_iIntervalCount);
		m_pBottomTimeAxis->SetTickLabelFormat(false, _T("%H:%M"));
	}
	else if (ENUM_TIME_UNIT::_MINUTE == m_enum_unit)
	{
		m_span = COleDateTimeSpan(0, 0, m_iIntervalCount * 10, 0);
		m_maxDateTime += m_span;

		m_pBottomTimeAxis->SetTickIncrement(false, CChartDateTimeAxis::tiMinute, m_iIntervalCount);
		m_pBottomTimeAxis->SetTickLabelFormat(false, _T("%D-%H"));
	}
	m_pBottomTimeAxis->SetMinMax(CChartCtrl::DateToValue(m_minDateTime), CChartCtrl::DateToValue(m_maxDateTime));

	// Create left axes
	m_pLeftAxis = m_ChartCtrl.CreateStandardAxis(CChartCtrl::LeftAxis);
	//m_pLeftAxis->SetMinMax(m_yAxisMin, m_yAxisMax);
    m_pLeftAxis->SetAutoHideScrollBar(true);
    m_pLeftAxis->EnableScrollBar(true);
    //m_pLeftAxis->SetPanZoomEnabled(true);

	m_pLineSeries = m_ChartCtrl.CreateLineSerie();
	m_pLineSeries->SetSeriesOrdering(poNoOrdering);

	// Defines the different colors (back color, axes color, ...)
	/*COLORREF BackColor = RGB(0, 50, 0);
	COLORREF GridColor = RGB(0, 180, 0);
	COLORREF TextColor = RGB(0, 180, 0);
	COLORREF SerieColor = RGB(255, 0, 0);*/

	COLORREF BackColor = RGB(73, 125, 172);
	COLORREF GridColor = RGB(215, 215, 215);
	COLORREF TextColor = RGB(0, 180, 0);
	COLORREF SerieColor = RGB(187, 68, 53);

	m_pLineSeries->SetColor(SerieColor);

	// Specifies a sunken border for the control
	m_ChartCtrl.SetEdgeType(EDGE_SUNKEN);

	// Sets the color of the border and the back color
	m_ChartCtrl.SetBorderColor(TextColor);
	m_ChartCtrl.SetBackColor(BackColor);
	//m_ChartCtrl.SetPanEnabled(true);
	//Sets the color of the different elements of the bottom axis
	m_ChartCtrl.GetBottomAxis()->SetAxisColor(GridColor);
	m_ChartCtrl.GetBottomAxis()->SetTextColor(TextColor);
	m_ChartCtrl.GetBottomAxis()->GetGrid()->SetColor(GridColor);

	// Sets the color of the different elements of the left axis
	m_ChartCtrl.GetLeftAxis()->SetAxisColor(GridColor);
	m_ChartCtrl.GetLeftAxis()->SetTextColor(TextColor);
	m_ChartCtrl.GetLeftAxis()->GetGrid()->SetColor(GridColor);

	// Sets the color of the title, change the font to Times New Roman
	// and add a string
	m_ChartCtrl.GetTitle()->SetColor(TextColor);
	m_ChartCtrl.GetTitle()->AddString(m_strTitle.c_str());

	// Finally re-enable the refresh of the control. This will refresh the
	// control if any refresh was still 'pending'.
	m_ChartCtrl.EnableRefresh(true);
}

