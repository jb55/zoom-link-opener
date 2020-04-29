
CFLAGS=-g -Wall -Wextra -Werror $(shell pkg-config --cflags gtk+-3.0 appindicator3-0.1)
LDFLAGS=$(shell pkg-config --libs gtk+-3.0 appindicator3-0.1)

DEPS=zoom.c zoom.h url_parser.c tray.h

all: zoom-link-opener

zoom-link-opener: zoom-link-opener.c $(DEPS)
	$(CC) $(CFLAGS) $< $(LDFLAGS) -o $@

test: test.c $(DEPS)
	$(CC) $(CFLAGS) $< $(LDFLAGS) -o $@

clean:
	rm -f test zoom-link-opener

check: test
	./test
