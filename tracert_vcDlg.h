
// tracert_vcDlg.h : header file
//

#pragma once
#include "afxcmn.h"
#include "afxwin.h"


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

	virtual BOOL PreTranslateMessage(MSG* p_msg);
	
	bool is_in_control(const CRect& rect,const CPoint& p);

	CString def_cap;

	HANDLE p_tracert;

public:
	
	CListCtrl m_lst_items;
	afx_msg void OnBnClickedBtnStart();
	afx_msg void OnBnClickedBtnStop();
	afx_msg void OnBnClickedBtnQuit();
	CStatic m_cap;
	CProgressCtrl m_prgrs;
	afx_msg void OnMouseHover(UINT nFlags, CPoint point);
	CButton m_btn_start;
	CButton m_btn_stop;
	CButton m_btn_quit;
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	CEdit m_edt;
	afx_msg void OnBnClickedBtnAbt();
};
