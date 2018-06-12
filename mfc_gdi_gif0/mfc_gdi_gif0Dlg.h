
// mfc_gdi_gif0Dlg.h : header file
//

#pragma once


// Cmfc_gdi_gif0Dlg dialog
class Cmfc_gdi_gif0Dlg : public CDialogEx
{
// Construction
public:
	Cmfc_gdi_gif0Dlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	enum { IDD = IDD_MFC_GDI_GIF0_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support


// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
};
