#pragma once
#include "afxwin.h"

enum ENUM_BTNTYPE
{
    BTNTYPE_ONLYOK,
    BTNTYPE_OKCANCEL,
};

// CTipsDlg �Ի���

class CTipsDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CTipsDlg)

public:
	CTipsDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CTipsDlg();
    void SetTips(const TCHAR* msg, ENUM_BTNTYPE type = BTNTYPE_ONLYOK);
// �Ի�������
	enum { IDD = IDD_DLG_TIPS };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
private:
	CStatic m_staticTipsLable;
	CButton m_btnOK;
    CButton m_btnCancel;
    CStatic m_static_pic1;
    CBrush m_brush;
	CFont m_font;

    CString m_strTips;
    ENUM_BTNTYPE m_iBtnType;

public:
	virtual BOOL OnInitDialog();
    afx_msg void OnPaint();
    afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnBnClickedBtnTipsok();
    
    afx_msg void OnBnClickedBtnTipcancel();
};
