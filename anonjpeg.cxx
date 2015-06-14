// MIT License
// Copyright (c) 2015 Hiroshi Shimamoto

#include "anonjpeg.hxx"

using namespace anonjpeg;

static unsigned short sb16(unsigned short x)
{
	return ((x << 8) | ((x >> 8) & 0x00ff));
}

// EXIF
void exif::parse_app1(segment *seg)
{
	const char exifid[6] = { 0x45, 0x78, 0x69, 0x66, 0x00, 0x00 };
	char *buf;

	buf = new char[seg->size];

	fin.seekg(seg->pos);
	// skip marker
	fin.seekg(2, std::ios_base::cur);

	fin.read(buf, seg->size);

	// check ID
	for (int i = 0; i < 6; i++) {
		if (buf[2 + i] != exifid[i])
			goto out;
	}

	{
	char *tiff = &buf[8];

	// walk 0th IDF

	unsigned short num_tags;

	num_tags = sb16(*(unsigned short *)&tiff[8]);

	char *ptr = &tiff[10];

	for (int i = 0; i < num_tags; i++) {
		//unsigned short tag, type;
		unsigned int val;

		//tag = sb16(*(unsigned short *)ptr);
		ptr += 2;
		//type = sb16(*(unsigned short *)ptr);
		ptr += 2;
		// num of value
		ptr += 4;
		// values
		val = 0;
		for (int k = 0; k < 4; k++) {
			val <<= 8;
			val |= (unsigned char)ptr[k];
		}
		ptr += 4;

	}
	}
out:
	delete[] buf;
}

void exif::parse()
{
	std::list<segment *>::iterator i;

	for (i = segs.begin(); i != segs.end(); i++) {
		segment *seg = *i;

		if (seg->marker == 0xffe1) {
			// check Exif Data
			parse_app1(seg);
		}
	}
}

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
		segment *seg;
		std::ifstream::pos_type pos;

		pos = fin.tellg();

		fin.read((char *)&marker, sizeof(marker));
		marker = sb16(marker);

		if (!(0xffe0 <= marker && marker <= 0xffef))
			break;

		fin.read((char *)&size, sizeof(size));
		size = sb16(size);

		seg = new segment();
		seg->marker = marker;
		seg->size = size;
		seg->pos = pos;

		segs.push_back(seg);

		// go to next segment
		fin.seekg(size - 2, std::ios_base::cur);
	}

	// keep data pointer
	fin.seekg(-2, std::ios_base::cur);
	datap = fin.tellg();

	parse();

	valid = true;
}

exif::~exif()
{
	std::list<segment *>::iterator i;

	for (i = segs.begin(); i != segs.end(); i++) {
		delete *i;
	}

	fin.close();
}

void exif::save(LPCTSTR path)
{
	if (!valid)
		return;

	unsigned short soi;

	soi = 0xd8ff;

	std::ofstream fout(path, std::ios::binary);
	fout.write((char *)&soi, sizeof(soi));

	// move fin to data
	fin.seekg(datap);

	while (!fin.eof()) {
		unsigned short data;

		fin.read((char *)&data, sizeof(data));
		fout.write((char *)&data, sizeof(data));
	}

	fout.close();

	::MessageBox(NULL, path, "Saved", MB_OK);
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

				exif e(path);
				e.save(_T("test.jpg"));
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
