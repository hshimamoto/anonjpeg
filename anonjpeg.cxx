// MIT License
// Copyright (c) 2015 Hiroshi Shimamoto

#include "anonjpeg.hxx"

using namespace anonjpeg;

mainwndcls::mainwndcls()
{
}

mainwnd::mainwnd(mainwndcls *c) : hWin::wnd(c)
{
}

LRESULT mainwnd::proc(HWND w, UINT m, WPARAM wp, LPARAM lp)
{
	switch (m) {
	case WM_DESTROY:
		::PostQuitMessage(0);
		return 0;
	}

	return ::DefWindowProc(w, m, wp, lp);
}

void mainapp::init(HINST inst, LPTSTR line, int show)
{
	c = new mainwndcls();
	w = new mainwnd(c);
}

int mainapp::main(void)
{
	w->create();
	w->update();
	w->show(SW_SHOW);

	msgloop();

	return 0;
}

void mainapp::exit(void)
{
}

mainapp app;

int hWinMain(HINST inst, LPTSTR line, int show)
{
	return 0;
}
