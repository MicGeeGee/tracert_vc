#pragma once
#include "afxwin.h"


// Ctracert_abtDlg 对话框

class Ctracert_abtDlg : public CDialogEx
{
	DECLARE_DYNAMIC(Ctracert_abtDlg)

public:
	Ctracert_abtDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~Ctracert_abtDlg();

// 对话框数据
	enum { IDD = IDD_DIALOG_ABT };

protected:
	virtual BOOL OnInitDialog();
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	virtual BOOL PreTranslateMessage(MSG* p_msg);
	bool is_in_control(const CRect& rect,const CPoint& p);

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedCnfrm();

	afx_msg void OnStnClickedStaticGit();
//	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	CStatic m_git;
	CStatic m_pic;
};
