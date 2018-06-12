#pragma once

#include <GdiPlus.h>

// gdi_gif

class gdi_gif : public CStatic
{
	DECLARE_DYNAMIC(gdi_gif)

public:
	gdi_gif();
	virtual ~gdi_gif();

protected:
	DECLARE_MESSAGE_MAP()

public:
	BOOL create(CRect &rc, CWnd *parent, UINT nid);
	void load(LPCTSTR filename);
	void play();
	void stop();
	void round_border(Gdiplus::Color &c, int line_width = 1);
	void enable_border(bool enable);
	void set_url(LPCTSTR url);

protected:
	virtual void DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct);
	virtual void PreSubclassWindow();

	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg LRESULT OnMouseLeave(WPARAM wParam, LPARAM lParam);
	afx_msg BOOL OnEraseBkgnd(CDC *pDC);

protected:
	void DrawBorder(Gdiplus::Graphics& g);

protected:
	Gdiplus::Image *m_p_image;
	GUID *m_p_dimension_id;
	UINT m_frame_count;
	Gdiplus::PropertyItem *m_p_item;
	UINT m_current_frame;
	CString m_url;
	Gdiplus::Color m_border_color;
	int m_border_line_width;
	bool m_b_border_enable;
	HCURSOR m_h_hand_cursor;
	HCURSOR m_h_orig_cursor;
	bool m_b_track_once;
	bool m_b_is_playing;
};
