#pragma once
#include "afxwin.h"


// Ctracert_abtDlg �Ի���

class Ctracert_abtDlg : public CDialogEx
{
	DECLARE_DYNAMIC(Ctracert_abtDlg)

public:
	Ctracert_abtDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~Ctracert_abtDlg();

// �Ի�������
	enum { IDD = IDD_DIALOG_ABT };

protected:
	virtual BOOL OnInitDialog();
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

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
