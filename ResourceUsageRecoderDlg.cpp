
// Resource usage recoderDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "ResourceUsageRecoder.h"
#include "ResourceUsageRecoderDlg.h"
#include "afxdialogex.h"
#include <stdio.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CResourceUsageRecoderDlg �Ի���

double Y_AXIS_MIN = 0;
double Y_AXIS_MAX = 150;

CResourceUsageRecoderDlg::CResourceUsageRecoderDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CResourceUsageRecoderDlg::IDD, pParent),
	m_pChartCtrlCPU(NULL),
	m_pChartCtrlMemoy(NULL),
	m_pChartCtrlHandle(NULL),
	m_pChartCtrlThread(NULL),
    m_pChartCtrlIO(NULL),
	m_pChartCtrlNonpaged(NULL),
	m_pProcessResStatistics(NULL)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDI_ICO_MAIN);
}

void CResourceUsageRecoderDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_BTN_RECODE, m_btn_start);
	DDX_Control(pDX, IDC_BTN_STOP, m_btn_stop);
	DDX_Control(pDX, IDC_STATIC_GIF, m_picGif);
}

BEGIN_MESSAGE_MAP(CResourceUsageRecoderDlg, CDialogEx)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_BTN_RECODE, &CResourceUsageRecoderDlg::OnBnClickedBtnRecode)
	ON_BN_CLICKED(IDC_BTN_STOP, &CResourceUsageRecoderDlg::OnBnClickedBtnStop)
    ON_WM_CLOSE()
END_MESSAGE_MAP()

long Fibonacci(int n)
{
	if (n == 0)
		return 0;
	else if (n == 1)
		return 1;
	else
		return Fibonacci(n - 1) + Fibonacci(n - 2);
}

// CResourceUsageRecoderDlg ��Ϣ�������

BOOL CResourceUsageRecoderDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// ���ô˶Ի����ͼ�ꡣ  ��Ӧ�ó��������ڲ��ǶԻ���ʱ����ܽ��Զ�
	//  ִ�д˲���
	SetIcon(m_hIcon, TRUE);			// ���ô�ͼ��
	SetIcon(m_hIcon, FALSE);		// ����Сͼ��

	// TODO:  �ڴ���Ӷ���ĳ�ʼ������

	m_pConfigRecoderDlg = new CConfigRecoder();
	if (m_pConfigRecoderDlg != NULL)
	{
		m_pConfigRecoderDlg->Create(IDD_DLG_CONFIGRECODE, this);
		CRect rect_parent, rect;
		this->GetWindowRect(&rect_parent);
		m_pConfigRecoderDlg->GetWindowRect(&rect);
		m_pConfigRecoderDlg->MoveWindow(0, 0, rect_parent.Width(), rect.Height());
		m_pConfigRecoderDlg->ShowWindow(SW_SHOW);
	}

	//�����Դ��ʱ��Ҫ���ļ����ó�ֻ������,Ҫ��ms��ĳɾ�̬ͼƬ
	//if (m_picGif.Load(_T(".\\res\\nonono.gif")))
	if (m_picGif.Load(MAKEINTRESOURCE(IDR_GIF1), _T("gif")))
	{
		m_picGif.Draw();
	}

	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
}

BOOL CResourceUsageRecoderDlg::PreTranslateMessage(MSG* pMsg)
{
	//����ESC�رմ���
	if (pMsg->message == WM_KEYDOWN && pMsg->wParam == VK_ESCAPE)
		return TRUE;
	else
		return CDialogEx::PreTranslateMessage(pMsg);
}

// �����Ի��������С����ť������Ҫ����Ĵ���
//  �����Ƹ�ͼ�ꡣ  ����ʹ���ĵ�/��ͼģ�͵� MFC Ӧ�ó���
//  �⽫�ɿ���Զ���ɡ�

void CResourceUsageRecoderDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // ���ڻ��Ƶ��豸������

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// ʹͼ���ڹ����������о���
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;
		// ����ͼ��
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CRect rect;
		GetClientRect(rect);
		CPaintDC dc(this);
		dc.FillSolidRect(rect, RGB(255, 255, 255));
		CDialogEx::OnPaint();
	}
}

//���û��϶���С������ʱϵͳ���ô˺���ȡ�ù��
//��ʾ��
HCURSOR CResourceUsageRecoderDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

BOOL CResourceUsageRecoderDlg::DestroyWindow()
{
	// TODO:  �ڴ����ר�ô����/����û���
	KillTimer(TIMER_SAMPLING_MSG);
	return CDialogEx::DestroyWindow();
}

void CResourceUsageRecoderDlg::OnClose()
{
    // TODO: Add your message handler code here and/or call default
    CTipsDlg tips;
    tips.SetTips(_T("ȷ��Ҫ�ر���?"), BTNTYPE_OKCANCEL);
	if (tips.DoModal() == IDOK)
	{
		//�˳�����
		CDialogEx::OnClose();
	}
}

void CResourceUsageRecoderDlg::OnTimer(UINT_PTR nIDEvent)
{
	// TODO:  �ڴ������Ϣ�����������/�����Ĭ��ֵ
	switch (nIDEvent)
	{
	case TIMER_SAMPLING_MSG:
		{
			//double Y = Fibonacci(m_iCount++);
			static int g_iCount = 0;
			double Y = ((g_iCount % 5) == 0) ? (g_iCount++)*1.6 : (g_iCount++)*1.15;
			if (m_pProcessResStatistics!=NULL && (m_pProcessResStatistics->IsRunning() == -1))
			{
				KillTimer(TIMER_SAMPLING_MSG);

				/*SYSTEMTIME time;
				GetLocalTime(&time);
				TCHAR szTips[128] = {0};
				_sntprintf(szTips,127,_T("%d-%02d-%02d %02d:%02d:%02d��������ֹ"), time.wYear,time.wMonth,time.wDay,time.wHour,time.wMinute,time.wSecond);*/
				CTipsDlg tips;
				tips.SetTips(_T("��������ֹ"), BTNTYPE_ONLYOK);
				tips.DoModal();
				return;
			}

			if (m_pChartCtrlCPU != NULL)
			{
				double cpuUsageRate = m_pProcessResStatistics->GetCPUInfo();
				::PostMessage(m_pChartCtrlCPU->GetSafeHwnd(), WM_USER_ADDDATA, NULL, cpuUsageRate);
			}

			if (m_pChartCtrlMemoy != NULL)
			{
				double memNum = m_pProcessResStatistics->GetMemoryInfo();
				::PostMessage(m_pChartCtrlMemoy->GetSafeHwnd(), WM_USER_ADDDATA, NULL, memNum);
			}

			if (m_pChartCtrlHandle != NULL)
			{
				double handleNum = m_pProcessResStatistics->GetHandleInfo();
				::PostMessage(m_pChartCtrlHandle->GetSafeHwnd(), WM_USER_ADDDATA, NULL, handleNum);
			}

			if (m_pChartCtrlThread != NULL)
				::PostMessage(m_pChartCtrlThread->GetSafeHwnd(), WM_USER_ADDDATA, NULL, Y);
            if (m_pChartCtrlIO != NULL)
                ::PostMessage(m_pChartCtrlIO->GetSafeHwnd(), WM_USER_ADDDATA, NULL, Y);
			if (m_pChartCtrlNonpaged != NULL)
				::PostMessage(m_pChartCtrlNonpaged->GetSafeHwnd(), WM_USER_ADDDATA, NULL, Y);

			SetTimer(TIMER_SAMPLING_MSG, m_iSampRateSecond, NULL);
			break;
		}
		default:
			break;
	}
	CDialogEx::OnTimer(nIDEvent);
}


void CResourceUsageRecoderDlg::OnBnClickedBtnRecode()
{
    if (m_pConfigRecoderDlg != NULL)
    {
        m_pConfigRecoderDlg->GetConfig(&m_info);
        if (m_info.iPID <= 0)
        {
            CTipsDlg tips;
            tips.SetTips(_T("����ѡ��һ��Ҫ���ӵĽ���"), BTNTYPE_ONLYOK);
            tips.DoModal();
            return;
        }

        if (0x0 == m_info.iCheck)
        {
            CTipsDlg tips;
            tips.SetTips(_T("������ѡ��һ��Ҫ���ӵ���Դ"), BTNTYPE_ONLYOK);
            tips.DoModal();
            return;
        }
        
        // TODO:  �ڴ���ӿؼ�֪ͨ����������
        m_btn_start.EnableWindow(false);
        m_btn_stop.EnableWindow(true);

        if (m_pConfigRecoderDlg != NULL)
            m_pConfigRecoderDlg->ShowWindow(SW_HIDE);

		m_pProcessResStatistics = new ProcessResourceStatistics(m_info.iPID);
		if (m_pProcessResStatistics == NULL)
		{
			CTipsDlg tips;
			tips.SetTips(_T("̫���������޷�ͳ����Դ,���˳�"), BTNTYPE_ONLYOK);
			tips.DoModal();
			return;
		}

        //���������ر����Ӵ���
        int iCount = 0;
        int iWidth(0), iHeight(0);
        if (m_info.iCheck & RESTYPE_CPU)
        {
            m_pChartCtrlCPU = new CChartDialog();
            TCHAR title[64] = { 0 };
            _swprintf_p(title,63,_T("%s-%d cpu usage recoder"), m_info.strName.c_str(), m_info.iPID);
            m_pChartCtrlCPU->SetInitParam(title, _T("Cpu usage percentage"), (m_info.iSampRateUnit==0) ? _SECOND : _MINUTE, m_info.iSampRate, _LINE);
            m_pChartCtrlCPU->Create(IDD_DLG_CHARTCTRL, this);
            CRect rect_cpu;
            m_pChartCtrlCPU->GetWindowRect(&rect_cpu);
            iWidth = rect_cpu.Width();
            iHeight = rect_cpu.Height();
            m_pChartCtrlCPU->MoveWindow(iWidth * (iCount % 2), iHeight * (iCount / 2), iWidth, iHeight);
            m_pChartCtrlCPU->ShowWindow(SW_SHOW);
            iCount++;
        }

        if (m_info.iCheck & RESTYPE_MEM)
        {
            m_pChartCtrlMemoy = new CChartDialog();
            TCHAR title[64] = { 0 };
            _swprintf_p(title, 63, _T("%s-%d memory usage recoder"), m_info.strName.c_str(), m_info.iPID);
            m_pChartCtrlMemoy->SetInitParam(title, _T("Unit:MB"), (m_info.iSampRateUnit == 0) ? _SECOND : _MINUTE, m_info.iSampRate, _LINE);
            m_pChartCtrlMemoy->Create(IDD_DLG_CHARTCTRL, this);
            CRect rect_memory;
            m_pChartCtrlMemoy->GetWindowRect(&rect_memory);
            iWidth = rect_memory.Width();
            iHeight = rect_memory.Height();
            m_pChartCtrlMemoy->MoveWindow(iWidth * (iCount % 2), iHeight * (iCount / 2), iWidth, iHeight);
            m_pChartCtrlMemoy->ShowWindow(SW_SHOW);
            iCount++;
        }

        if (m_info.iCheck & RESTYPE_HANDLE)
        {
            m_pChartCtrlHandle = new CChartDialog();
            TCHAR title[64] = { 0 };
            _swprintf_p(title, 63, _T("%s-%d handle usage recoder"), m_info.strName.c_str(), m_info.iPID);
            m_pChartCtrlHandle->SetInitParam(title, _T("Number of handles"), (m_info.iSampRateUnit == 0) ? _SECOND : _MINUTE, m_info.iSampRate, _LINE);
            m_pChartCtrlHandle->Create(IDD_DLG_CHARTCTRL, this);
            CRect rect_handle;
            m_pChartCtrlHandle->GetWindowRect(&rect_handle);
            iWidth = rect_handle.Width();
            iHeight = rect_handle.Height();
            m_pChartCtrlHandle->MoveWindow(iWidth * (iCount % 2), iHeight * (iCount / 2), iWidth, iHeight);
            m_pChartCtrlHandle->ShowWindow(SW_SHOW);
            iCount++;
        }

        if (m_info.iCheck & RESTYPE_THREAD)
        {
            m_pChartCtrlThread = new CChartDialog();
            TCHAR title[64] = { 0 };
            _swprintf_p(title, 63, _T("%s-%d thread usage recoder"), m_info.strName.c_str(), m_info.iPID);
            m_pChartCtrlThread->SetInitParam(title, _T("Number of thread"), (m_info.iSampRateUnit == 0) ? _SECOND : _MINUTE, m_info.iSampRate, _LINE);
            m_pChartCtrlThread->Create(IDD_DLG_CHARTCTRL, this);
            CRect rect_thread;
            m_pChartCtrlThread->GetWindowRect(&rect_thread);
            iWidth = rect_thread.Width();
            iHeight = rect_thread.Height();
            m_pChartCtrlThread->MoveWindow(iWidth * (iCount % 2), iHeight * (iCount / 2), iWidth, iHeight);
            m_pChartCtrlThread->ShowWindow(SW_SHOW);
            iCount++;
        }

        if (m_info.iCheck & RESTYPE_IO)
        {
            m_pChartCtrlIO = new CChartDialog();
            TCHAR title[64] = { 0 };
            _swprintf_p(title, 63, _T("%s-%d I/O usage recoder"), m_info.strName.c_str(), m_info.iPID);
            m_pChartCtrlIO->SetInitParam(title, _T("Unit:MB"), (m_info.iSampRateUnit == 0) ? _SECOND : _MINUTE, m_info.iSampRate, _LINE);
            m_pChartCtrlIO->Create(IDD_DLG_CHARTCTRL, this);
            CRect rect_io;
            m_pChartCtrlIO->GetWindowRect(&rect_io);
            iWidth = rect_io.Width();
            iHeight = rect_io.Height();
            m_pChartCtrlIO->MoveWindow(iWidth * (iCount % 2), iHeight * (iCount / 2), iWidth, iHeight);
            m_pChartCtrlIO->ShowWindow(SW_SHOW);
            iCount++;
        }

        if (m_info.iCheck & RESTYPE_NONP)
        {
            m_pChartCtrlNonpaged = new CChartDialog();
            TCHAR title[64] = { 0 };
            _swprintf_p(title, 63, _T("%s-%d Nonpaged usage recoder"), m_info.strName.c_str(), m_info.iPID);
            m_pChartCtrlNonpaged->SetInitParam(title, _T("Unit:MB"), (m_info.iSampRateUnit == 0) ? _SECOND : _MINUTE, m_info.iSampRate, _LINE);
            m_pChartCtrlNonpaged->Create(IDD_DLG_CHARTCTRL, this);
            CRect rect_nonpaged;
            m_pChartCtrlNonpaged->GetWindowRect(&rect_nonpaged);
            iWidth = rect_nonpaged.Width();
            iHeight = rect_nonpaged.Height();
            m_pChartCtrlNonpaged->MoveWindow(iWidth * (iCount % 2), iHeight * (iCount / 2), iWidth, iHeight);
            m_pChartCtrlNonpaged->ShowWindow(SW_SHOW);
            iCount++;
        }

        if (iCount != 0 && iWidth != 0 && iHeight != 0)
        {
            //���������ڵĴ�С��λ��
            int nDesktopWidth = GetSystemMetrics(SM_CXSCREEN);
            int nDesktopHeight = GetSystemMetrics(SM_CYSCREEN);
            int iiWidth = (iCount > 1) ? iWidth * 2 : iWidth;
            int iiHeight = ((iCount % 2) != 0) ? (iCount / 2 + 1)*iHeight : (iCount / 2)*iHeight;
            this->MoveWindow((nDesktopWidth - iiWidth) / 2, (nDesktopHeight - iiHeight) / 2 - 40, iiWidth + 15, iiHeight + 100);

            //������ťλ��
            CRect rect_start, rect_stop;
            m_btn_start.GetWindowRect(&rect_start);
            m_btn_start.MoveWindow(50, iiHeight + 20, rect_start.Width(), rect_start.Height());
            m_btn_stop.GetWindowRect(&rect_stop);
            m_btn_stop.MoveWindow(180, iiHeight + 20, rect_stop.Width(), rect_stop.Height());

            //����GIFλ��
            CRect rect_gif;
            m_picGif.GetWindowRect(&rect_gif);
            m_picGif.MoveWindow(iiWidth - rect_gif.Width(), iiHeight + 68 - rect_gif.Height(), rect_gif.Width(), rect_gif.Height());

            //������ʱ��
            m_iSampRateSecond = 1000 * m_info.iSampRate * ((m_info.iSampRateUnit == 0) ? 1 : 60);
            SetTimer(TIMER_SAMPLING_MSG, 1000, NULL);//1���ʼ��¼��һ��, ֮��m_iSampRateSecondʱ����
        }
    }
}

void CResourceUsageRecoderDlg::OnBnClickedBtnStop()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	m_btn_stop.EnableWindow(false);
	m_btn_start.EnableWindow(true);

	//�رն�ʱ��
	KillTimer(TIMER_SAMPLING_MSG);
}

