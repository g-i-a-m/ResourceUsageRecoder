
// ResourceUsageRecoderDlg.h : ͷ�ļ�
//

#ifndef _RESOURCEUSAGE_RECODER_H
#define _RESOURCEUSAGE_RECODER_H

#include "ChartCtrl.h"
#include "ChartLineSerie.h"
#include "ChartStandardAxis.h"
#include "afxwin.h"
#include "ChartDialog.h"
#include "ConfigRecoder.h"
#include "TipsDlg.h"
#include "PictureEx.h"
#include "CpuUsage.h"
#include "ProcessResourceStatistics.h"

#define TIMER_SAMPLING_MSG WM_USER+1

// CResourceUsageRecoderDlg �Ի���
class CResourceUsageRecoderDlg : public CDialogEx
{
// ����
public:
	CResourceUsageRecoderDlg(CWnd* pParent = NULL);	// ��׼���캯��

// �Ի�������
	enum { IDD = IDD_RESOURCEUSAGERECODER_DIALOG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ֧��

// ʵ��
protected:
	HICON m_hIcon;

	// ���ɵ���Ϣӳ�亯��
	virtual BOOL OnInitDialog();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()

private:
    RecoderInfo m_info;

	CButton m_btn_start;
	CButton m_btn_stop;
	CPictureEx m_picGif;
	CConfigRecoder* m_pConfigRecoderDlg;//��¼�������Ӵ���
	CChartDialog* m_pChartCtrlCPU;		//CPU��Դ
	CChartDialog* m_pChartCtrlMemoy;	//�ڴ�����
	CChartDialog* m_pChartCtrlHandle;	//�������
	CChartDialog* m_pChartCtrlThread;   //�̸߳���
    CChartDialog* m_pChartCtrlIO;       //IO��д
	CChartDialog* m_pChartCtrlNonpaged; //��ҳ�滺���
    UINT m_iSampRateSecond;             //TIMER��� / ���ݲɼ����

	ProcessResourceStatistics* m_pProcessResStatistics;//��Դͳ�ƶ���ָ��

public:
    afx_msg void OnClose();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	virtual BOOL DestroyWindow();
	afx_msg void OnBnClickedBtnRecode();
	afx_msg void OnBnClickedBtnStop();
};

#endif //_RESOURCEUSAGE_RECODER_H
