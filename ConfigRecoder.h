#pragma once
#include <string>

struct RecoderInfo
{
    RecoderInfo() :iPID(-1), iCheck(0), bSave(false){}

    int iPID;               //进程PID
    std::wstring strName;   //进程名
    int iCheck;             //监视项
    int iSampRate;          //监视采样时间间隔
    int iSampRateUnit;      //监视采样时间单位
    bool bSave;             //是否保存到本地
};

enum ENUM_RESTYPE
{
    RESTYPE_MEM     = 0x1,
    RESTYPE_CPU     = 0x2,
    RESTYPE_HANDLE  = 0x4,
    RESTYPE_THREAD  = 0x8,
    RESTYPE_IO      = 0x10,
    RESTYPE_NONP    = 0x20
};

struct DATA
{
	int subitem;		//点击表头的列数
	CListCtrl* plist;	//listctrl的指针
};

enum ColumnType
{
	INT_TYPE = 0,
	TEXT_TYPE = 1,
	UNKNOW_TYPE = 100
};

// CConfigRecoder 对话框

class CConfigRecoder : public CDialogEx
{
	DECLARE_DYNAMIC(CConfigRecoder)

public:
	CConfigRecoder(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CConfigRecoder();
    void GetConfig(RecoderInfo* info);

// 对话框数据
	enum { IDD = IDD_DLG_CONFIGRECODE };

private:
    bool raisePrivilege();
    void GetAllProcess();
    void GetAllProcess_V2();
    void AddProcess2List(TCHAR* name, DWORD pid, TCHAR* pathname = NULL);
    void SortListCtrl(int subitem, bool bNotChange = false);
    static int CALLBACK listCompare(LPARAM lParam1, LPARAM lParam2, LPARAM lParamSort);

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()

public:
	CListCtrl m_listctrl;
	CButton m_ck_mem;
	CButton m_ck_cpu;
	CButton m_ck_handle;
	CButton m_ck_thread;
	CButton m_ck_io;
	CButton m_ck_Nonpaged;
	CButton m_ck_save;
    CEdit m_edit_samprate;
    CComboBox m_cmb_sampunit;

	CBrush m_brush;
    int m_listctrl_index;
	static bool fav;

	virtual BOOL OnInitDialog();
	virtual BOOL PreTranslateMessage(MSG* pMsg);

	afx_msg void OnPaint();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
    afx_msg void OnLvnColumnclickListProcess(NMHDR *pNMHDR, LRESULT *pResult);
    afx_msg void OnEnChangeEditSamprate();
    afx_msg void OnCbnSelchangeCmbSampunit();
	CButton m_ck_netup;
	afx_msg void OnBnClickedCkNetup();
	afx_msg void OnBnClickedCkNetdown();
	CButton m_ck_netdown;
};
