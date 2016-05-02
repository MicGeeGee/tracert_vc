
// tracert_vcDlg.cpp : implementation file
//

#include "stdafx.h"
#include "tracert_vc.h"
#include "tracert_vcDlg.h"
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
}

void Ctracert_vcDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LST, m_lst_items);
}

BEGIN_MESSAGE_MAP(Ctracert_vcDlg, CDialogEx)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BTN_START, &Ctracert_vcDlg::OnBnClickedBtnStart)
	ON_BN_CLICKED(IDC_BTN_STOP, &Ctracert_vcDlg::OnBnClickedBtnStop)
	ON_BN_CLICKED(IDC_BTN_QUIT, &Ctracert_vcDlg::OnBnClickedBtnQuit)
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
	m_lst_items.InsertColumn(1,_T("Interval 1"),LVCFMT_CENTER,80);
	m_lst_items.InsertColumn(2,_T("Interval 2"),LVCFMT_CENTER,80);
	m_lst_items.InsertColumn(3,_T("Interval 3"),LVCFMT_CENTER,80);
	m_lst_items.InsertColumn(4,_T("Router IP address"),LVCFMT_CENTER,300);

	
	


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
	
	/*
	int index=m_lst_items.GetItemCount();
	char index_str[100];
	sprintf(index_str,"%d",index);

	CString index_cstr(index_str);
	
	


	int row_index=m_lst_items.InsertItem(index,_T("1"));
	m_lst_items.SetItemText(row_index,0,index_cstr);
	m_lst_items.SetItemText(row_index,1,_T("30"));
	m_lst_items.SetItemText(row_index,2,_T("40"));
	m_lst_items.SetItemText(row_index,3,_T("30"));
	m_lst_items.SetItemText(row_index,4,_T("1.1.1.1"));
	*/

	//Clear the list.
	while (m_lst_items.DeleteItem(0));


	CEdit* p_edt=(CEdit*)GetDlgItem(IDC_EDT_IP);
	CString IP_addr;
	p_edt->GetWindowText(IP_addr);
	
	char addr_str[100];
	int len =WideCharToMultiByte(CP_ACP,0,IP_addr,-1,NULL,0,NULL,NULL);  
	WideCharToMultiByte(CP_ACP,0,IP_addr,-1,addr_str,len,NULL,NULL );  
	
	tracert::instance::init(addr_str,&m_lst_items);
	//tracert::instance::run();

	unsigned long tid;
	CreateThread(NULL,0,&(tracert::instance::run),NULL,NULL,&tid);
}


void Ctracert_vcDlg::OnBnClickedBtnStop()
{
	// TODO: 在此添加控件通知处理程序代码
	while (m_lst_items.DeleteItem(0));
	
}


void Ctracert_vcDlg::OnBnClickedBtnQuit()
{
	// TODO: 在此添加控件通知处理程序代码
	

}
