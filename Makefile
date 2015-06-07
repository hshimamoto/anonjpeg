MINGW = i686-w64-mingw32-
CXX = $(MINGW)g++
LD = $(MINGW)g++
AR = $(MINGW)ar

CXXFLAGS = -g -O2 -Wall
CXXFLAGS += -mwindows
LDFLAGS = -g -O2 -Wall
LDFLAGS += -mwindows
LDFLAGS += -static

anonjpeg-objs = anonjpeg.o
hwinobjs = hwin/hwin.o

all: anonjpeg.exe

anonjpeg.exe: $(anonjpeg-objs)
	$(LD) $(LDFLAGS) -o $@ $($(basename $@)-objs) $(hwinobjs)

%.o: %.cxx
	$(CXX) $(CXXFLAGS) -c -o $@ $<

clean:
	rm -f *.o
