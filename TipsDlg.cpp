// TipsDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "ResourceUsageRecoder.h"
#include "TipsDlg.h"
#include "afxdialogex.h"
#include <string>

// CTipsDlg 对话框

IMPLEMENT_DYNAMIC(CTipsDlg, CDialogEx)

CTipsDlg::CTipsDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CTipsDlg::IDD, pParent),
    m_iBtnType(BTNTYPE_ONLYOK)
{

}

CTipsDlg::~CTipsDlg()
{
}

void CTipsDlg::SetTips(const TCHAR* msg, ENUM_BTNTYPE type)
{
	size_t iSize = wcslen(msg);
	size_t index = 0;
	std::wstring str = msg;
	for (size_t i = 0; i < iSize; )
	{
		m_strTips += str.substr(index, index+6).c_str();
		if (index + 6 >= iSize)
		{
			break;
		}
		else
		{
			m_strTips += _T("\n");
			index = i += 6;
		}
	}
    m_iBtnType = type;
}

void CTipsDlg::DoDataExchange(CDataExchange* pDX)
{
    CDialogEx::DoDataExchange(pDX);
    DDX_Control(pDX, IDC_STATIC_TIPS, m_staticTipsLable);
    DDX_Control(pDX, IDC_BTN_TIPSOK, m_btnOK);
    DDX_Control(pDX, IDC_BTN_TIPCANCEL, m_btnCancel);
    DDX_Control(pDX, IDC_STATIC_PICTURE, m_static_pic1);
    
}


BEGIN_MESSAGE_MAP(CTipsDlg, CDialogEx)
    ON_WM_PAINT()
    ON_WM_CTLCOLOR()
	ON_BN_CLICKED(IDC_BTN_TIPSOK, &CTipsDlg::OnBnClickedBtnTipsok)
    ON_BN_CLICKED(IDC_BTN_TIPCANCEL, &CTipsDlg::OnBnClickedBtnTipcancel)
END_MESSAGE_MAP()


// CTipsDlg 消息处理程序

BOOL CTipsDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化
	m_font.CreatePointFont(200, _T("华文行楷"));
    m_brush.CreateSolidBrush(RGB(255, 255, 255));

	m_staticTipsLable.SetWindowTextW(m_strTips);

    if (BTNTYPE_ONLYOK == m_iBtnType)
    {
        m_btnCancel.ShowWindow(SW_HIDE);
    }

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常:  OCX 属性页应返回 FALSE
}

void CTipsDlg::OnPaint()
{
    CPaintDC dc(this); // device context for painting
                       // TODO:  在此处添加消息处理程序代码
                       // 不为绘图消息调用 CDialogEx::OnPaint()
    CRect rect;
    GetClientRect(rect);
    dc.FillSolidRect(rect, RGB(255, 255, 255)); //设置为绿色背景
}

HBRUSH CTipsDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
    HBRUSH hbr = CDialogEx::OnCtlColor(pDC, pWnd, nCtlColor);

    // TODO:  在此更改 DC 的任何特性

	if (pWnd->GetDlgCtrlID() == IDC_STATIC_TIPS)
	{
		pDC->SetBkMode(TRANSPARENT);
		pDC->SetTextColor(RGB(0, 0, 0));
		pDC->SelectObject(&m_font);
		return (HBRUSH)GetStockObject(HOLLOW_BRUSH);
	}
	
    switch (nCtlColor)
    {
    case CTLCOLOR_STATIC:
        return m_brush;
    default:
        break;
    }
    // TODO:  如果默认的不是所需画笔，则返回另一个画笔
    return hbr;
}

void CTipsDlg::OnBnClickedBtnTipsok()
{
	// TODO: Add your control notification handler code here
    return CDialogEx::OnOK();
}


void CTipsDlg::OnBnClickedBtnTipcancel()
{
    // TODO: Add your control notification handler code here
    return CDialogEx::OnCancel();
}
