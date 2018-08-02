// ConfigRecoder.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "ResourceUsageRecoder.h"
#include "ConfigRecoder.h"
#include "TipsDlg.h"
#include "afxdialogex.h"
#include "psapi.h"
#include <TlHelp32.h>
#pragma comment(lib,"psapi.lib")

// CConfigRecoder �Ի���

bool CConfigRecoder::fav = false;

IMPLEMENT_DYNAMIC(CConfigRecoder, CDialogEx)

CConfigRecoder::CConfigRecoder(CWnd* pParent /*=NULL*/)
	: CDialogEx(CConfigRecoder::IDD, pParent),
    m_listctrl_index(0)
{

}

CConfigRecoder::~CConfigRecoder()
{
}

void CConfigRecoder::GetConfig(RecoderInfo* info)
{
    //���̺�
    int pos = m_listctrl.GetSelectionMark();
    int pid = _ttoi(m_listctrl.GetItemText(pos, 1));
    info->iPID = pid;
    info->strName = m_listctrl.GetItemText(pos, 0).GetString();

    //���ӵ���Դ����ָ��
    info->iCheck = 0x0;
    if (m_ck_mem.GetCheck())
    {
        info->iCheck = info->iCheck | 0x1;
    }

    if (m_ck_cpu.GetCheck())
    {
        info->iCheck = info->iCheck | 0x2;
    }

    if (m_ck_handle.GetCheck())
    {
        info->iCheck = info->iCheck | 0x4;
    }

    if (m_ck_thead.GetCheck())
    {
        info->iCheck = info->iCheck | 0x8;
    }

    if (m_ck_io.GetCheck())
    {
        info->iCheck = info->iCheck | 0x10;
    }

    if (m_ck_Nonpaged.GetCheck())
    {
        info->iCheck = info->iCheck | 0x20;
    }

    //
    CString strSampRate;
    m_edit_samprate.GetWindowText(strSampRate);
    info->iSampRate = _wtoi(strSampRate.GetBuffer());
    int index = m_cmb_sampunit.GetCurSel();
    if (index == 0)//��
    {
        info->iSampRateUnit = 0;
    }
    else if (index == 1)//��
    {
        info->iSampRateUnit = 1;
    }

    //�����Ƿ���Ҫ���浽����
    if (m_ck_save.GetCheck())
    {
        info->bSave = true;
    }
    else
    {
        info->bSave = false;
    }
}

void CConfigRecoder::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_PROCESS, m_listctrl);
	DDX_Control(pDX, IDC_CK_MEM, m_ck_mem);
	DDX_Control(pDX, IDC_CK_CPU, m_ck_cpu);
	DDX_Control(pDX, IDC_CK_HANDLE, m_ck_handle);
	DDX_Control(pDX, IDC_CK_THREAD, m_ck_thead);
	DDX_Control(pDX, IDC_CK_IO, m_ck_io);
	DDX_Control(pDX, IDC_CK_NOD, m_ck_Nonpaged);
	DDX_Control(pDX, IDC_CK_SAVE, m_ck_save);
	DDX_Control(pDX, IDC_EDIT_SAMPRATE, m_edit_samprate);
	DDX_Control(pDX, IDC_CMB_SAMPUNIT, m_cmb_sampunit);
	DDX_Control(pDX, IDC_CK_NETUP, m_ck_netup);
	DDX_Control(pDX, IDC_CK_NETDOWN, m_ck_netdown);
}


BEGIN_MESSAGE_MAP(CConfigRecoder, CDialogEx)
	ON_WM_PAINT()
	ON_WM_CTLCOLOR()
    ON_NOTIFY(LVN_COLUMNCLICK, IDC_LIST_PROCESS, &CConfigRecoder::OnLvnColumnclickListProcess)
    ON_EN_CHANGE(IDC_EDIT_SAMPRATE, &CConfigRecoder::OnEnChangeEditSamprate)
    ON_CBN_SELCHANGE(IDC_CMB_SAMPUNIT, &CConfigRecoder::OnCbnSelchangeCmbSampunit)
	ON_BN_CLICKED(IDC_CK_NETUP, &CConfigRecoder::OnBnClickedCkNetup)
	ON_BN_CLICKED(IDC_CK_NETDOWN, &CConfigRecoder::OnBnClickedCkNetdown)
END_MESSAGE_MAP()


// CConfigRecoder ��Ϣ��������


BOOL CConfigRecoder::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  �ڴ����Ӷ���ĳ�ʼ��
	m_brush.CreateSolidBrush(RGB(255, 255, 255));

    //��ʼ�������б�
    m_listctrl.SetExtendedStyle(LVS_EX_FULLROWSELECT | LVS_EX_FULLROWSELECT);
    m_listctrl.SetBkColor(RGB(255, 255, 255));
    m_listctrl.InsertColumn(0, _T("Name"), LVCFMT_LEFT, 160);
    m_listctrl.InsertColumn(1, _T("PID"), LVCFMT_LEFT, 50);
    m_listctrl.InsertColumn(2, _T("File Path"), LVCFMT_LEFT, 300);
    GetAllProcess();

    //��ʼ���༭��
    m_edit_samprate.SetWindowText(_T("10"));

    //��ʼ��combobox
    m_cmb_sampunit.InsertString(0,_T("��"));
    m_cmb_sampunit.InsertString(1,_T("��"));
    m_cmb_sampunit.SetCurSel(1);

	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣:  OCX ����ҳӦ���� FALSE
}



void CConfigRecoder::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	// TODO:  �ڴ˴�������Ϣ�����������
	// ��Ϊ��ͼ��Ϣ���� CDialogEx::OnPaint()
	CRect rect;
	GetClientRect(rect);
	dc.FillSolidRect(rect, RGB(255, 255, 255));
}


HBRUSH CConfigRecoder::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CDialogEx::OnCtlColor(pDC, pWnd, nCtlColor);

	// TODO:  �ڴ˸��� DC ���κ�����

	if (pWnd->GetDlgCtrlID() == IDC_STATIC_SAMPRATE)
	{
		pDC->SetBkMode(TRANSPARENT);
		pDC->SetTextColor(RGB(0, 0, 0));
		return (HBRUSH)GetStockObject(HOLLOW_BRUSH);
	}

	switch (nCtlColor)
	{
	case CTLCOLOR_STATIC:
		return m_brush;
	default:
		break;
	}
	// TODO:  ���Ĭ�ϵĲ������軭�ʣ��򷵻���һ������
	return hbr;
}

bool CConfigRecoder::raisePrivilege()
{
    HANDLE hToken;
    bool fOk = false;
    if (OpenProcessToken(GetCurrentProcess(), TOKEN_ADJUST_PRIVILEGES | TOKEN_QUERY, &hToken))
    {
        TOKEN_PRIVILEGES tp;
        tp.PrivilegeCount = 1;
        tp.Privileges[0].Attributes = SE_PRIVILEGE_ENABLED;
        if (!LookupPrivilegeValue(NULL, SE_DEBUG_NAME, &tp.Privileges[0].Luid))
        {
            CloseHandle(hToken);
            return fOk;
        }
        if (!AdjustTokenPrivileges(hToken, FALSE, &tp, sizeof(tp), NULL, NULL))
        {
            CloseHandle(hToken);
            return fOk;
        }

        fOk = (GetLastError() == ERROR_SUCCESS);
        CloseHandle(hToken);
    }
    return fOk;
}

void CConfigRecoder::GetAllProcess()
{
	/*if (!raisePrivilege())
	{
		CTipsDlg tips;
		tips.SetTips(_T("����Ȩ��ʧ��"));
		tips.DoModal();
	}*/

    DWORD aProcesses[1024], cbNeeded;
    if (!EnumProcesses(aProcesses, sizeof(aProcesses), &cbNeeded))
        return;

    for (unsigned int i = 0; (i<cbNeeded)&&(i<1024); i++)
    {
        HMODULE hMods[512] = { 0 };
        DWORD cbNeeded = 0;
        TCHAR szModName[MAX_PATH] = { 0 };
        TCHAR szPathName[MAX_PATH] = { 0 };
        //PROCESS_ALL_ACCESS
        HANDLE hProcess = ::OpenProcess(PROCESS_ALL_ACCESS, FALSE, aProcesses[i]);
        if (hProcess != NULL)
        {
            BOOL Wow64Process;
            IsWow64Process(hProcess, &Wow64Process);//�ж���32λ����64λ����
            if (EnumProcessModulesEx(hProcess, hMods, sizeof(hMods), &cbNeeded, Wow64Process ? LIST_MODULES_32BIT : LIST_MODULES_64BIT))
                if (GetModuleBaseName(hProcess, hMods[0], szModName, _countof(szModName)) && GetModuleFileNameEx(hProcess, hMods[0], szPathName, _countof(szPathName)))
                    AddProcess2List(szModName, aProcesses[i], szPathName);
            CloseHandle(hProcess);
        }
    }
    return;
}

void CConfigRecoder::GetAllProcess_V2()
{
    // Ϊ���̵������߳��ĸ�����  
    HANDLE hSnapshort = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
    if (hSnapshort == INVALID_HANDLE_VALUE)
        return;

    // ����߳��б��������¼���̵߳���ϸ��Ϣ����ʹ��Thread32First��Thread32Next���������м�¼��ÿ���߳���Ϣ  
    PROCESSENTRY32 stcProcessInfo;
    stcProcessInfo.dwSize = sizeof(stcProcessInfo);
    BOOL bRet = Process32First(hSnapshort, &stcProcessInfo);
    while (bRet)
    {
        bRet = Process32Next(hSnapshort, &stcProcessInfo);

        HMODULE hMods[512] = { 0 };
        DWORD cbNeeded = 0;
        TCHAR szPathName[MAX_PATH] = { 0 };
        //PROCESS_ALL_ACCESS
        HANDLE hProcess = ::OpenProcess(PROCESS_ALL_ACCESS, FALSE, stcProcessInfo.th32ProcessID);
        if (hProcess != NULL)
        {
            BOOL Wow64Process;
            IsWow64Process(hProcess, &Wow64Process);//�ж���32λ����64λ����
            if (EnumProcessModulesEx(hProcess, hMods, sizeof(hMods), &cbNeeded, Wow64Process ? LIST_MODULES_32BIT : LIST_MODULES_64BIT))
                if (GetModuleFileNameEx(hProcess, hMods[0], szPathName, _countof(szPathName)))
                    AddProcess2List(stcProcessInfo.szExeFile, stcProcessInfo.th32ProcessID, szPathName);
            CloseHandle(hProcess);
        }
    }
    CloseHandle(hSnapshort);
}


void CConfigRecoder::AddProcess2List(TCHAR* name, DWORD pid, TCHAR* pathname)
{
    m_listctrl.InsertItem(m_listctrl_index, L"");
    m_listctrl.SetItemText(m_listctrl_index, 0, name);

    TCHAR tmp[12] = { 0 };
#ifdef UNICODE
    _itow_s(pid, tmp, 10);
#else
    _itoa_s(pid, tmp, 10);
#endif

    m_listctrl.SetItemText(m_listctrl_index, 1, tmp);
    if (pathname != NULL)
    {
        m_listctrl.SetItemText(m_listctrl_index, 2, pathname);
    }
    else
    {
        m_listctrl.SetItemText(m_listctrl_index, 2, _T(""));
    }
    
    ++m_listctrl_index;
}


int CConfigRecoder::listCompare(LPARAM lParam1, LPARAM lParam2, LPARAM lParamSort)
{
    DATA* pListCtrl = (DATA*)lParamSort;
    int col = pListCtrl->subitem;
    int colType = UNKNOW_TYPE;
    CListCtrl* plist = pListCtrl->plist;
    if (col == 1)
        colType = INT_TYPE;
    else if (col == 0)
        colType = TEXT_TYPE;
    else
        colType = UNKNOW_TYPE;

    //��ȡ��ͷ��Ϣ
    LVCOLUMN Vol;
    TCHAR szCol[MAX_PATH] = { 0 };
    Vol.pszText = szCol;
    Vol.mask = LVCF_TEXT;
    Vol.cchTextMax = sizeof(szCol);
    plist->GetColumn(col, &Vol);

    if (colType == INT_TYPE)//int�ͱȽ�
    {
        CString strRow1 = plist->GetItemText(lParam1, col);
        CString strRow2 = plist->GetItemText(lParam2, col);
        int n1 = _ttoi(strRow1);
        int n2 = _ttoi(strRow2);
        if (fav)
        {
            // �Ƚ������� �������-1��ʾn1����n2ǰ�棬�������1��ʾn1����n2���棬�����ȷ���0������û���ж���ȵ����
            if (n1 < n2)
                return -1;
            else if (n1 > n2)
                return 1;
            else
                return 0;
        }
        else
        {
            if (n1 < n2)
                return 1;
            else if (n1 > n2)
                return -1;
            else
                return 0;
        }
    }
    else if (colType == TEXT_TYPE)
    {
        CString strItem1 = plist->GetItemText(lParam1, col);
        CString strItem2 = plist->GetItemText(lParam2, col);
        int iRet = 0;
        if (fav)
            iRet = _tcscmp(strItem1.MakeUpper().GetBuffer(MAX_PATH), strItem2.MakeUpper().GetBuffer(MAX_PATH));
        else
            iRet = _tcscmp(strItem2.MakeUpper().GetBuffer(MAX_PATH), strItem1.MakeUpper().GetBuffer(MAX_PATH));
        return iRet;
    }
    else
    {
        return 0;
    }
}

void CConfigRecoder::SortListCtrl(int subitem, bool bNotChange)
{
    //�����е�ÿһ�м���������
    for (int i = 0; i < m_listctrl.GetItemCount(); ++i)
        m_listctrl.SetItemData(i, i);

    DATA data;
    data.subitem = subitem;
    data.plist = &m_listctrl;
    bNotChange ? fav = fav : fav = !fav;
    m_listctrl.SortItems(listCompare, (LPARAM)&data);
}

void CConfigRecoder::OnLvnColumnclickListProcess(NMHDR *pNMHDR, LRESULT *pResult)
{
    LPNMLISTVIEW pNMListView = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
    // TODO: �ڴ����ӿؼ�֪ͨ�����������

    SortListCtrl(pNMListView->iSubItem);
    *pResult = 0;
}

void CConfigRecoder::OnEnChangeEditSamprate()
{
    // TODO:  If this is a RICHEDIT control, the control will not
    // send this notification unless you override the CDialogEx::OnInitDialog()
    // function and call CRichEditCtrl().SetEventMask()
    // with the ENM_CHANGE flag ORed into the mask.

    // TODO:  Add your control notification handler code here
}

void CConfigRecoder::OnCbnSelchangeCmbSampunit()
{
    // TODO: Add your control notification handler code here
}


void CConfigRecoder::OnBnClickedCkNetup()
{
	// TODO: Add your control notification handler code here
}


void CConfigRecoder::OnBnClickedCkNetdown()
{
	// TODO: Add your control notification handler code here
}