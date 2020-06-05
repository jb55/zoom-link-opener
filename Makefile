
ifeq ($(OS),Windows_NT)
	TRAY_CFLAGS := -DTRAY_WINAPI=1
	TRAY_LDFLAGS :=
else ifeq ($(shell uname -s),Linux)
	TRAY_CFLAGS := -DTRAY_APPINDICATOR=1 $(shell pkg-config --cflags appindicator3-0.1)
	TRAY_LDFLAGS := $(shell pkg-config --libs appindicator3-0.1)
else ifeq ($(shell uname -s),Darwin)
	TRAY_CFLAGS := -DTRAY_APPKIT=1
	TRAY_LDFLAGS := -framework Cocoa
endif


CFLAGS=-g -Wall -Wextra $(TRAY_CFLAGS) -DOBJC_OLD_DISPATCH_PROTOTYPES=1 $(shell pkg-config --cflags gtk+-3.0 appindicator3-0.1)
LDFLAGS=$(shell pkg-config --libs gtk+-3.0 appindicator3-0.1) $(TRAY_LDFLAGS)

DEPS=zoom.c zoom.h url_parser.c tray.h

all: zoom-link-opener

zoom-link-opener: main.c $(DEPS)
	$(CC) $(CFLAGS) $< $(LDFLAGS) -o $@

test: test.c $(DEPS)
	$(CC) $(CFLAGS) $< $(LDFLAGS) -o $@

clean:
	rm -f test zoom-link-opener

check: test
	./test
