SUBDIRS = audio c++ core gui video extra
AM_CXXFLAGS = -fPIC

if HAVE_WINDRES
ico_stuff = gui/windows/resources.rc
endif

.rc.o:
	$(WINDRES) -I.. -o $@ $<

bin_PROGRAMS = punes

punes_SOURCES = \
	core/main.c \
	gui/resources.cpp \
	core/palette.c \
	$(ico_stuff)

punes_LDADD = \
	core/libcore.a \
	video/libvideo.a \
	c++/libcplus.a \
	gui/libgui.a \
	audio/libaudio.a \
	$(LIB7ZIP_LIBS)
