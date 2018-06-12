// gdi_gif.cpp : implementation file
//

#include "stdafx.h"
#include "mfc_gdi_gif0.h"
#include "gdi_gif.h"

#define TIMER_ID 1

// gdi_gif

IMPLEMENT_DYNAMIC(gdi_gif, CStatic)

gdi_gif::gdi_gif()
{
	m_p_image = NULL;
	m_p_dimension_id = NULL;
	m_p_item = NULL;
	m_b_border_enable = false;
	m_b_track_once = false;
	m_b_is_playing = false;
	m_h_hand_cursor = LoadCursor(NULL, MAKEINTRESOURCE(IDC_HAND));
}

gdi_gif::~gdi_gif()
{
	if (m_p_image)
	{
		delete m_p_image;
	}

	if (m_p_dimension_id)
	{
		delete []m_p_dimension_id;
	}

	if (m_p_item)
	{
		free(m_p_item);
	}
}

BOOL gdi_gif::create(CRect &rc, CWnd *parent, UINT nid)
{
	return CStatic::Create(NULL, WS_CHILD | WS_VISIBLE | SS_OWNERDRAW | SS_NOTIFY, rc, parent, nid);
}

void gdi_gif::load(LPCTSTR filename)
{
	m_p_image = Gdiplus::Image::FromFile(filename);

	UINT count = m_p_image->GetFrameDimensionsCount();
	m_p_dimension_id = new GUID[count];
	m_p_image->GetFrameDimensionsList(m_p_dimension_id, count);

	TCHAR tc_guid[39] = {0};
	StringFromGUID2(m_p_dimension_id[0], tc_guid, 39);

	m_frame_count = m_p_image->GetFrameCount(&m_p_dimension_id[0]);

	UINT total_buffer = m_p_image->GetPropertyItemSize(PropertyTagFrameDelay);
	m_p_item = (Gdiplus::PropertyItem *)malloc(total_buffer);
	m_p_image->GetPropertyItem(PropertyTagFrameDelay, total_buffer, m_p_item);
}

void gdi_gif::play()
{
	if (m_b_is_playing)
		return;

	m_current_frame = 0;
	GUID guid = Gdiplus::FrameDimensionTime;
	m_p_image->SelectActiveFrame(&guid, m_current_frame);

	SetTimer(TIMER_ID, ((UINT *)m_p_item[0].value)[m_current_frame] * 10, NULL);
	++m_current_frame;
	Invalidate(FALSE);

	m_b_is_playing = true;
}

void gdi_gif::stop()
{
	if (!m_b_is_playing)
		return;

	KillTimer(TIMER_ID);
	m_b_is_playing = false;
}

void gdi_gif::round_border(Gdiplus::Color &c, int line_width /*= 1*/)
{
	m_border_color = c;
	m_border_line_width = line_width;

	CRect client;
	GetClientRect(&client);
	ClientToScreen(&client);

	client.left -= line_width;
	client.top -= line_width;
	client.right += line_width;
	client.bottom += line_width;

	SetWindowPos(
		CWnd::FromHandle(HWND_BOTTOM),
		client.left,
		client.top,
		client.Width(),
		client.Height(),
		SWP_NOZORDER
		);
}

void gdi_gif::enable_border(bool enable)
{
	m_b_border_enable = enable;
}

void gdi_gif::set_url(LPCTSTR url)
{
	m_url = url;
}

void gdi_gif::DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct)
{
	Gdiplus::Graphics g(lpDrawItemStruct->hDC);
	DrawBorder(g);

	CRect client;
	GetClientRect(&client);

	if (m_b_border_enable)
	{
		client.DeflateRect(m_border_line_width, m_border_line_width,
			m_border_line_width, m_border_line_width);
	}

	g.DrawImage(m_p_image, client.left, client.top, client.Width(), client.Height());
}

void gdi_gif::PreSubclassWindow()
{
	ModifyStyle(0, SS_OWNERDRAW | SS_NOTIFY);
	CStatic::PreSubclassWindow();
}

void gdi_gif::OnTimer(UINT_PTR nIDEvent)
{
	KillTimer(nIDEvent);

	GUID guid = Gdiplus::FrameDimensionTime;
	m_p_image->SelectActiveFrame(&guid, m_current_frame);

	SetTimer(TIMER_ID,((UINT *)m_p_item[0].value)[m_current_frame] * 10, NULL);
	m_current_frame = (++m_current_frame) % m_frame_count;
	Invalidate(FALSE);
}

void gdi_gif::OnLButtonDown(UINT nFlags, CPoint point)
{
	if (m_url.IsEmpty())
		return;

	ShellExecute(NULL, L"Open", L"IEXPLORE", m_url, NULL, SW_SHOW);
}

void gdi_gif::OnMouseMove(UINT nFlags, CPoint point)
{
	if (!m_b_track_once)
	{
		TRACKMOUSEEVENT	csTME;
		csTME.cbSize = sizeof(csTME);
		csTME.dwFlags = TME_LEAVE;
		csTME.hwndTrack = m_hWnd;
		::_TrackMouseEvent(&csTME);

		m_b_track_once = true;
	}

	if (m_url.IsEmpty())
		return;

	m_h_orig_cursor = GetCursor();
	SetCursor(m_h_hand_cursor);
}

LRESULT gdi_gif::OnMouseLeave(WPARAM wParam, LPARAM lParam)
{
	m_b_track_once = true;
	SetCursor(m_h_orig_cursor);

	return TRUE;
}

BOOL gdi_gif::OnEraseBkgnd(CDC *pDC)
{
	return CStatic::OnEraseBkgnd(pDC);
}

void gdi_gif::DrawBorder(Gdiplus::Graphics& g)
{
	if (!m_b_border_enable)
		return;

	CRect client;
	GetClientRect(&client);

	Gdiplus::Rect gdi_rect(client.left, client.top, client.Width(), client.Height());
	Gdiplus::SolidBrush gdi_brush(m_border_color);

	g.FillRectangle(&gdi_brush, gdi_rect);
}

BEGIN_MESSAGE_MAP(gdi_gif, CStatic)
	ON_WM_TIMER()
	ON_WM_MOUSEMOVE()
	ON_MESSAGE(WM_MOUSELEAVE, OnMouseLeave)
	ON_WM_LBUTTONDOWN()
	ON_WM_ERASEBKGND()
END_MESSAGE_MAP()
