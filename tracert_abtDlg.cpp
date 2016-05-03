// Ctracert_abtDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "tracert_vc.h"
#include "tracert_abtDlg.h"
#include "afxdialogex.h"


// Ctracert_abtDlg 对话框

IMPLEMENT_DYNAMIC(Ctracert_abtDlg, CDialogEx)

Ctracert_abtDlg::Ctracert_abtDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(Ctracert_abtDlg::IDD, pParent)
{

}

Ctracert_abtDlg::~Ctracert_abtDlg()
{
}

void Ctracert_abtDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	//  DDX_Control(pDX, IDC_TXT_GIT, m_txt_git);
	DDX_Control(pDX, IDC_TXT_GIT, m_git);
	DDX_Control(pDX, IDC_PIC, m_pic);
}


BEGIN_MESSAGE_MAP(Ctracert_abtDlg, CDialogEx)
	ON_BN_CLICKED(IDC_CNFRM, &Ctracert_abtDlg::OnBnClickedCnfrm)
	ON_STN_CLICKED(IDC_TXT_GIT, &Ctracert_abtDlg::OnStnClickedStaticGit)
//	ON_WM_CTLCOLOR()
ON_WM_CTLCOLOR()
END_MESSAGE_MAP()


// Ctracert_abtDlg 消息处理程序


void Ctracert_abtDlg::OnBnClickedCnfrm()
{
	// TODO: 在此添加控件通知处理程序代码
	OnCancel();
}


void Ctracert_abtDlg::OnStnClickedStaticGit()
{
	// TODO: 在此添加控件通知处理程序代码
	ShellExecute(0, NULL, _T("https:///github.com//MicGeeGee//tracert_vc"), NULL, NULL, SW_NORMAL);
}

BOOL Ctracert_abtDlg::OnInitDialog()
{
	
	CStatic* p_pic=(CStatic*)GetDlgItem(IDC_PIC);

	HBITMAP bitmap; 
	bitmap = ::LoadBitmap(AfxGetInstanceHandle(),MAKEINTRESOURCE(IDB_PIC)); 
	p_pic->SetBitmap(bitmap);

	return TRUE;
}




HBRUSH Ctracert_abtDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CDialogEx::OnCtlColor(pDC, pWnd, nCtlColor);


	if(pWnd->GetDlgCtrlID()==IDC_TXT_GIT)
		pDC->SetTextColor(RGB(0,0,255));

	// TODO:  如果默认的不是所需画笔，则返回另一个画笔
	return hbr;
}


BOOL Ctracert_abtDlg::PreTranslateMessage(MSG* p_msg)
{
	
	if(p_msg->message == WM_MOUSEMOVE)    
		if(p_msg->hwnd != m_hWnd)
		{  
			CPoint p;    
			p = p_msg->pt;  
			//FromHandle(p_msg->hwnd)->ScreenToClient(&p);
			ScreenToClient(&p);

			CRect rect_git;
			
			CStatic* p_git=(CStatic*)GetDlgItem(IDC_TXT_GIT);
			p_git->GetWindowRect(&rect_git);
			ScreenToClient(&rect_git);
			
			if(is_in_control(rect_git,p))
				SetCursor(LoadCursor(NULL, IDC_HAND));
			else
				SetCursor(LoadCursor(NULL, IDC_ARROW));
				
		}
		else
			SetCursor(LoadCursor(NULL, IDC_ARROW));

	

	return CDialog::PreTranslateMessage(p_msg);
}


bool Ctracert_abtDlg::is_in_control(const CRect& rect,const CPoint& p)
{
	//return (p.x> rect.left && p.x < rect.right && p.y> rect.top && p.y <rect.bottom);
	return (p.x>= rect.left-1 && p.x < rect.right+1  && p.y> rect.top-1&& p.y <rect.bottom+1);
}
