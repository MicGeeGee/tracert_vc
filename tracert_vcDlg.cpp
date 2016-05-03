
// tracert_vcDlg.cpp : implementation file
//

#include "stdafx.h"
#include "tracert_vc.h"
#include "tracert_vcDlg.h"
#include "tracert_abtDlg.h"
#include "afxdialogex.h"
#include "tracert.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// Ctracert_vcDlg dialog



Ctracert_vcDlg::Ctracert_vcDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(Ctracert_vcDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	p_tracert=NULL;
}

void Ctracert_vcDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LST, m_lst_items);
	DDX_Control(pDX, IDC_CAP, m_cap);
	DDX_Control(pDX, IDC_PRGRS, m_prgrs);
	DDX_Control(pDX, IDC_BTN_START, m_btn_start);
	DDX_Control(pDX, IDC_BTN_STOP, m_btn_stop);
	DDX_Control(pDX, IDC_BTN_QUIT, m_btn_quit);
	DDX_Control(pDX, IDC_EDT_IP, m_edt);
}

BEGIN_MESSAGE_MAP(Ctracert_vcDlg, CDialogEx)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BTN_START, &Ctracert_vcDlg::OnBnClickedBtnStart)
	ON_BN_CLICKED(IDC_BTN_STOP, &Ctracert_vcDlg::OnBnClickedBtnStop)
	ON_BN_CLICKED(IDC_BTN_QUIT, &Ctracert_vcDlg::OnBnClickedBtnQuit)
	ON_WM_MOUSEHOVER()
	ON_WM_MOUSEMOVE()
	ON_BN_CLICKED(IDC_BTN_ABT, &Ctracert_vcDlg::OnBnClickedBtnAbt)
END_MESSAGE_MAP()


// Ctracert_vcDlg message handlers

BOOL Ctracert_vcDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	// TODO: Add extra initialization here
	DWORD grid_style = m_lst_items.GetExtendedStyle();
	grid_style|= LVS_EX_FULLROWSELECT;
	grid_style|= LVS_EX_GRIDLINES;
	m_lst_items.SetExtendedStyle(grid_style);



	m_lst_items.InsertColumn(0,_T("#"),LVCFMT_CENTER,20);
	m_lst_items.InsertColumn(1,_T("Interval 1(ms)"),LVCFMT_CENTER,80);
	m_lst_items.InsertColumn(2,_T("Interval 2(ms)"),LVCFMT_CENTER,80);
	m_lst_items.InsertColumn(3,_T("Interval 3(ms)"),LVCFMT_CENTER,80);
	m_lst_items.InsertColumn(4,_T("Router IP address"),LVCFMT_CENTER,300);

	CHeaderCtrl* p_header_ctrl=(CHeaderCtrl* )m_lst_items.GetHeaderCtrl();
	p_header_ctrl->EnableWindow(FALSE);
		
	m_prgrs.SetRange(0,30);
	m_prgrs.SetStep(1);

	
	return TRUE;  // return TRUE  unless you set the focus to a control
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void Ctracert_vcDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

// The system calls this function to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR Ctracert_vcDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}




void Ctracert_vcDlg::OnBnClickedBtnStart()
{
	// TODO: 在此添加控件通知处理程序代码
	//Clear the list.
	while (m_lst_items.DeleteItem(0));
	//Clear progress bar.
	m_prgrs.SetPos(0);
	//Clear default caption.
	def_cap=CString("");

	CEdit* p_edt=(CEdit*)GetDlgItem(IDC_EDT_IP);
	CString IP_addr;
	p_edt->GetWindowText(IP_addr);
	
	char addr_str[100];
	int len =WideCharToMultiByte(CP_ACP,0,IP_addr,-1,NULL,0,NULL,NULL);  
	WideCharToMultiByte(CP_ACP,0,IP_addr,-1,addr_str,len,NULL,NULL );  
	
	tracert::instance::init(addr_str,&m_lst_items,&m_prgrs,&def_cap);
	//tracert::instance::run();

	unsigned long tid;
	p_tracert=CreateThread(NULL,0,&(tracert::instance::run),NULL,NULL,&tid);
}


void Ctracert_vcDlg::OnBnClickedBtnStop()
{
	// TODO: 在此添加控件通知处理程序代码
	while (m_lst_items.DeleteItem(0));
	if(p_tracert)
	{
		TerminateThread(p_tracert,0);
		tracert::instance::terminate();
		p_tracert=NULL;
	}
	m_prgrs.SetPos(0);
	def_cap=CString("");
}


void Ctracert_vcDlg::OnBnClickedBtnQuit()
{
	// TODO: 在此添加控件通知处理程序代码
	CDialog::OnOK();
}

BOOL Ctracert_vcDlg::PreTranslateMessage(MSG* p_msg)
{
	if(p_msg->message==WM_KEYDOWN)
	{
		//Capture the 'ENTER' and 'ESC'.
		if(p_msg->wParam==VK_RETURN)
		{
			OnBnClickedBtnStart();
			return TRUE;
		}
		if(p_msg->wParam==VK_ESCAPE)
		{
			OnOK();
			return TRUE;
		}
	}
	if(p_msg->message == WM_MOUSEMOVE)    
		if(p_msg->hwnd != m_hWnd)
		{  
			CPoint p;    
			p = p_msg->pt;  
			//FromHandle(p_msg->hwnd)->ScreenToClient(&p);
			ScreenToClient(&p);

			CRect rect_start;
			CRect rect_stop;
			CRect rect_quit;
			CRect rect_edt;
		
			m_btn_start.GetWindowRect(&rect_start);
			m_btn_stop.GetWindowRect(&rect_stop);
			m_btn_quit.GetWindowRect(&rect_quit);
			m_edt.GetWindowRect(&rect_edt);

			ScreenToClient(&rect_start);
			ScreenToClient(&rect_stop);
			ScreenToClient(&rect_quit);
			ScreenToClient(&rect_edt);


			if(is_in_control(rect_start,p))
				m_cap.SetWindowText(_T("Start traceroute.(Or press 'Enter')"));
			else if(is_in_control(rect_stop,p))
				m_cap.SetWindowText(_T("Terminate traceroute."));
			else if(is_in_control(rect_quit,p))
				m_cap.SetWindowText(_T("Close the window.(Or press 'Esc')"));
			else if(is_in_control(rect_edt,p))
				m_cap.SetWindowText(_T("Destination IP address for test, press 'Enter' to start tracerouting."));
			else
				m_cap.SetWindowText(def_cap);
		}
		else
				m_cap.SetWindowText(def_cap);

	return CDialog::PreTranslateMessage(p_msg);
}

void Ctracert_vcDlg::OnMouseHover(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值

	CDialogEx::OnMouseHover(nFlags, point);
}

bool Ctracert_vcDlg::is_in_control(const CRect& rect,const CPoint& p)
{
	//return (p.x> rect.left && p.x < rect.right && p.y> rect.top && p.y <rect.bottom);
	return (p.x>= rect.left-1 && p.x < rect.right+1  && p.y> rect.top-1&& p.y <rect.bottom+1);
}

void Ctracert_vcDlg::OnMouseMove(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	
	
	CDialogEx::OnMouseMove(nFlags, point);
}


void Ctracert_vcDlg::OnBnClickedBtnAbt()
{
	// TODO: 在此添加控件通知处理程序代码
	Ctracert_abtDlg* abt_dlg=new Ctracert_abtDlg();
	abt_dlg->DoModal();
	
}
