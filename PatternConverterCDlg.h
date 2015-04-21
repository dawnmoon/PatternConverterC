
// PatternConverterCDlg.h : header file
//

#pragma once
#include <vector>
#include "PatternProcessor.h"
using namespace std;

// CPatternConverterCDlg dialog
class CPatternConverterCDlg : public CDialogEx
{
// Construction
public:
	CPatternConverterCDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	enum { IDD = IDD_PATTERNCONVERTERC_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support


// Implementation
protected:
	int m_nCurHeight;
	int m_nScrollPos;
	CRect m_rect;

	HICON m_hIcon;
	CPatternProcessor m_patProcessor;
	// Generated message map functions
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
};
