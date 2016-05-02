
// tracert_vcDlg.h : header file
//

#pragma once
#include "afxcmn.h"


// Ctracert_vcDlg dialog
class Ctracert_vcDlg : public CDialogEx
{
// Construction
public:
	Ctracert_vcDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	enum { IDD = IDD_TRACERT_VC_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support


// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	
	CListCtrl m_lst_items;
	afx_msg void OnBnClickedBtnStart();
	afx_msg void OnBnClickedBtnStop();
	afx_msg void OnBnClickedBtnQuit();
};
