// MIT License
// Copyright (c) 2015 Hiroshi Shimamoto

#include "anonjpeg.hxx"

using namespace anonjpeg;

// EXIF
exif::exif(LPTSTR path) : fin(path, std::ios::binary)
{
	valid = false;

	// check JPEG SOI
	unsigned short soi;

	fin.read((char *)&soi, sizeof(soi));
	if (soi != 0xd8ff)
		return;

	valid = true;
}

exif::~exif()
{
	fin.close();
}

mainwndcls::mainwndcls()
{
}

mainwnd::mainwnd(mainwndcls *c) : hWin::wnd(c)
{
}

LRESULT mainwnd::proc(HWND w, UINT m, WPARAM wp, LPARAM lp)
{
	switch (m) {
	case WM_DROPFILES:
		{
			HDROP d = (HDROP)wp;
			int num = ::DragQueryFile(d, -1, NULL, 0);

			for (int i = 0; i < num; i++) {
				TCHAR path[MAX_PATH];

				::DragQueryFile(d, i, path, sizeof(path));
				::MessageBox(w, path, "drop", MB_OK);
			}
			::DragFinish(d);
		}
		return 0;
	case WM_DESTROY:
		::PostQuitMessage(0);
		return 0;
	}

	return ::DefWindowProc(w, m, wp, lp);
}

void mainwnd::on_create(void)
{
	// accept drop files
	::DragAcceptFiles(get(), TRUE);
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
