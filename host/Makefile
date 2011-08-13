LDFLAGS=`pkg-config --libs libusb-1.0` `pkg-config --libs libosmocore` -losmocore

all: simtrace

simtrace: main.o apdu_split.o
	$(CC) -o $@ $^ $(LDFLAGS)

%.o: %.c
	$(CC) $(CFLAGS) `pkg-config --cflags libusb-1.0` `pkg-config --cflags libosmocore` -o $@ -c $^

install: simtrace
	install -d $(DESTDIR)/usr/bin/
	install -m 0755 simtrace $(DESTDIR)/usr/bin/
.PHONY: install

clean:
	@rm -f simtrace *.o
