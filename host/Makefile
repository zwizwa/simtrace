LDFLAGS=-lusb `pkg-config --libs libosmocore` -losmocore

all: simtrace

simtrace: main.o usb_helper.o usb.o apdu_split.o
	$(CC) -o $@ $^ $(LDFLAGS)

%.o: %.c
	$(CC) $(CFLAGS) `pkg-config --cflags libosmocore` -o $@ -c $^

install: simtrace
	install -d $(DESTDIR)/usr/bin/
	install -m 0755 simtrace $(DESTDIR)/usr/bin/
.PHONY: install

clean:
	@rm -f simtrace *.o
