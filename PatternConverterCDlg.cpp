
// PatternConverterCDlg.cpp : implementation file
//

#include "stdafx.h"
#include "PatternConverterC.h"
#include "PatternConverterCDlg.h"
#include "afxdialogex.h"
#include "Markup.h"
#include <atlimage.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CAboutDlg dialog used for App About

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// Dialog Data
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

// Implementation
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CPatternConverterCDlg dialog

CPatternConverterCDlg::CPatternConverterCDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CPatternConverterCDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CPatternConverterCDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CPatternConverterCDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_WM_VSCROLL()
END_MESSAGE_MAP()


// CPatternConverterCDlg message handlers

BOOL CPatternConverterCDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	// TODO: Add extra initialization here
	

	if(m_patProcessor.ReadPatternXML(_T("PatternList.xml")))
	{
		CClientDC dc(this);
		m_patProcessor.Save(dc, _T("patternC++.bmp"));
		GetWindowRect(m_rect);
		m_nScrollPos = 0;
		int nScrollMax = m_rect.Height();
		SCROLLINFO si;
		si.cbSize = sizeof(SCROLLINFO);
		si.fMask = SIF_ALL; 
		si.nMin = 0;
		si.nMax = nScrollMax;
		si.nPage = m_patProcessor.GetPatternCount()>0? si.nMax/m_patProcessor.GetPatternCount():si.nMax;
		si.nPos = 0;
		SetScrollInfo(SB_VERT, &si, TRUE); 
	}

	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CPatternConverterCDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

void CPatternConverterCDlg::OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	SCROLLINFO scrollinfo; 
	int rate = 1;
	int page = 1;
	GetScrollInfo(SB_VERT,&scrollinfo,SIF_ALL); 
	switch (nSBCode) 
	{ 
	case SB_BOTTOM: 
		ScrollWindow(0,(scrollinfo.nPos-scrollinfo.nMax)*rate); 
		scrollinfo.nPos = scrollinfo.nMax; 
		SetScrollInfo(SB_VERT,&scrollinfo,SIF_ALL); 
		break; 
	case SB_TOP: 
		ScrollWindow(0,(scrollinfo.nPos-scrollinfo.nMin)*rate); 
		scrollinfo.nPos = scrollinfo.nMin; 
		SetScrollInfo(SB_VERT,&scrollinfo,SIF_ALL); 
		break; 
	case SB_LINEUP: 
		scrollinfo.nPos -= 1; 
		if (scrollinfo.nPos<scrollinfo.nMin) 
		{ 
			scrollinfo.nPos = scrollinfo.nMin; 
			break; 
		} 
		SetScrollInfo(SB_VERT,&scrollinfo,SIF_ALL); 
		ScrollWindow(0,rate); 
		break; 
	case SB_LINEDOWN: 
		scrollinfo.nPos += 1; 
		if (scrollinfo.nPos>scrollinfo.nMax) 
		{ 
			scrollinfo.nPos = scrollinfo.nMax; 
			break; 
		} 
		SetScrollInfo(SB_VERT,&scrollinfo,SIF_ALL); 
		ScrollWindow(0,-rate); 
		break; 
	case SB_PAGEUP: 
		scrollinfo.nPos -= page; 
		if (scrollinfo.nPos<scrollinfo.nMin) 
		{ 
			scrollinfo.nPos = scrollinfo.nMin; 
			break; 
		} 
		SetScrollInfo(SB_VERT,&scrollinfo,SIF_ALL); 
		ScrollWindow(0,rate*page); 
		break; 
	case SB_PAGEDOWN: 
		scrollinfo.nPos += page; 
		if (scrollinfo.nPos>scrollinfo.nMax) 
		{ 
			scrollinfo.nPos = scrollinfo.nMax; 
			break; 
		} 
		SetScrollInfo(SB_VERT,&scrollinfo,SIF_ALL); 
		ScrollWindow(0,-rate*page); 
		break; 
	case SB_ENDSCROLL: 
		// MessageBox("SB_ENDSCROLL"); 
		break; 
	case SB_THUMBPOSITION: 
		// ScrollWindow(0,(scrollinfo.nPos-nPos)*rate); 
		// scrollinfo.nPos = nPos; 
		// SetScrollInfo(SB_VERT,&scrollinfo,SIF_ALL); 
		break; 
	case SB_THUMBTRACK: 
		ScrollWindow(0,(scrollinfo.nPos-nPos)*rate); 
		scrollinfo.nPos = nPos; 
		SetScrollInfo(SB_VERT,&scrollinfo,SIF_ALL); 
		break; 
	} 
	Invalidate();
	CDialogEx::OnVScroll(nSBCode, nPos, pScrollBar);
}

void CPatternConverterCDlg::OnPaint()
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
		CPaintDC dc(this);
		CRect rtClient;
		GetClientRect(rtClient);
		CDC memDC;
		memDC.CreateCompatibleDC(&dc);
		CBitmap memBmp;
		memBmp.CreateCompatibleBitmap(&dc,m_patProcessor.GetFillWidth()*10,m_patProcessor.GetTotalHeight());
		memDC.SelectObject(memBmp);
		m_patProcessor.Draw(memDC);
		SCROLLINFO info;
		GetScrollInfo(SB_VERT,&info);
		int pos = m_patProcessor.GetTotalHeight()*info.nPos/info.nMax;
		dc.BitBlt(20,0,m_patProcessor.GetFillWidth()*10,rtClient.Height(), &memDC, 0, pos, SRCCOPY);
		CDialogEx::OnPaint();
	}
}

// The system calls this function to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CPatternConverterCDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}
