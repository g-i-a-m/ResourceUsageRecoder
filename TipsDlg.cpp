// TipsDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "ResourceUsageRecoder.h"
#include "TipsDlg.h"
#include "afxdialogex.h"
#include <string>

// CTipsDlg �Ի���

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


// CTipsDlg ��Ϣ�������

BOOL CTipsDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  �ڴ���Ӷ���ĳ�ʼ��
	m_font.CreatePointFont(200, _T("�����п�"));
    m_brush.CreateSolidBrush(RGB(255, 255, 255));

	m_staticTipsLable.SetWindowTextW(m_strTips);

    if (BTNTYPE_ONLYOK == m_iBtnType)
    {
        m_btnCancel.ShowWindow(SW_HIDE);
    }

	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣:  OCX ����ҳӦ���� FALSE
}

void CTipsDlg::OnPaint()
{
    CPaintDC dc(this); // device context for painting
                       // TODO:  �ڴ˴������Ϣ����������
                       // ��Ϊ��ͼ��Ϣ���� CDialogEx::OnPaint()
    CRect rect;
    GetClientRect(rect);
    dc.FillSolidRect(rect, RGB(255, 255, 255)); //����Ϊ��ɫ����
}

HBRUSH CTipsDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
    HBRUSH hbr = CDialogEx::OnCtlColor(pDC, pWnd, nCtlColor);

    // TODO:  �ڴ˸��� DC ���κ�����

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
    // TODO:  ���Ĭ�ϵĲ������軭�ʣ��򷵻���һ������
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
