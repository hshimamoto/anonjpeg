// MIT License
// Copyright (c) 2015 Hiroshi Shimamoto

#include "anonjpeg.hxx"

using namespace anonjpeg;

static unsigned short sb16(unsigned short x)
{
	return ((x << 8) | ((x >> 8) & 0x00ff));
}

// EXIF
exif::exif(LPTSTR path) : fin(path, std::ios::binary)
{
	valid = false;

	// check JPEG SOI
	unsigned short soi;

	fin.read((char *)&soi, sizeof(soi));
	if (soi != 0xd8ff)
		return;

	// check APPx
	for (;;) {
		unsigned short marker, size;
		std::ifstream::pos_type pos;

		pos = fin.tellg();

		fin.read((char *)&marker, sizeof(marker));
		marker = sb16(marker);

		if (!(0xffe0 <= marker && marker <= 0xffef))
			break;

		segs.push_back(pos);

		fin.read((char *)&size, sizeof(size));
		size = sb16(size);

		// go to next segment
		fin.seekg(size - 2, std::ios_base::cur);
	}

	unsigned short sos;

	fin.read((char *)&sos, sizeof(soi));
	if (sos != 0xdaff)
		return;

	datap = fin.tellg();

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
