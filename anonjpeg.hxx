// MIT License
// Copyright (c) 2015 Hiroshi Shimamoto

#ifndef ANONJPEG_HXX
#define ANONJPEG_HXX

#include "hwin/hwin.hxx"

namespace anonjpeg {

class mainwndcls : public hWin::cls {
public:
	mainwndcls();
};

class mainwnd : public hWin::wnd {
public:
	mainwnd(mainwndcls *c);
	LRESULT proc(HWND w, UINT m, WPARAM wp, LPARAM lp);
	void on_create(void);
};

class mainapp : public hWin::app {
	mainwndcls *c;
	mainwnd *w;
public:
	void init(HINST, LPTSTR, int);
	int main(void);
	void exit(void);
};

}; // namespace anonjpeg;

#endif // ANONJPEG_HXX
