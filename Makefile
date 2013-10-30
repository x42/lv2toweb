PREFIX ?= /usr/local
bindir ?= $(PREFIX)/bin
mandir ?= $(PREFIX)/share/man

CFLAGS ?=-Wall -O3
VERSION?=$(shell (git describe --tags HEAD 2>/dev/null || echo "v0.1") | sed 's/^v//')

###############################################################################

ifeq ($(shell pkg-config --exists lilv-0 || echo no), no)
	$(warning *** liblilv-0 from http://drobilla.net/software/lilv is required)
  $(error   Please install liblilv-dev)
endif

###############################################################################

override CFLAGS +=`pkg-config --cflags lilv-0` -std=c99
override CFLAGS += -DVERSION="\"$(VERSION)\""
LOADLIBES = `pkg-config --libs lilv-0` -lm
man1dir   = $(mandir)/man1

###############################################################################

default: all

lv2toweb: lv2toweb.c xhtmlcss.h

lv2toweb.1: lv2toweb
	help2man -N -n 'create html documentation for LV2 plugins' -o lv2toweb.1 ./lv2toweb

install-bin: lv2toweb
	install -d $(DESTDIR)$(bindir)
	install -m755 lv2toweb $(DESTDIR)$(bindir)

install-man: lv2toweb.1
	install -d $(DESTDIR)$(man1dir)
	install -m644 lv2toweb.1 $(DESTDIR)$(man1dir)

uninstall-bin:
	rm -f $(DESTDIR)$(bindir)/lv2toweb
	-rmdir $(DESTDIR)$(bindir)

uninstall-man:
	rm -f $(DESTDIR)$(man1dir)/lv2toweb.1
	-rmdir $(DESTDIR)$(man1dir)
	-rmdir $(DESTDIR)$(mandir)

clean:
	rm -f lv2toweb

all: lv2toweb

man: lv2toweb.1

install: install-bin install-man

uninstall: uninstall-bin uninstall-man

.PHONY:default all man clean install install-bin install-man uninstall uninstall-bin uninstall-man test

test: lv2toweb
	./lv2toweb -s scrot.png "http://gareus.org/oss/lv2/midifilter#enforcescale" > /tmp/lv2web.html
	#./lv2toweb "http://plugin.org.uk/swh-plugins/se4" > /tmp/lv2web.html
	#./lv2toweb "http://gareus.org/oss/lv2/xfade" > /tmp/lv2web.html
	#./lv2toweb "http://invadarecords.com/plugins/lv2/compressor/stereo" > /tmp/lv2web.html
	#./lv2toweb "http://gareus.org/oss/lv2/meters#EBUstereo_gtk" > /tmp/lv2web.html
	#./lv2toweb "http://gareus.org/oss/lv2/meters#EBUr128_gtk" > /tmp/lv2web.html
