LDFLAGS=-lusb `pkg-config --libs libosmocore` -losmocore

all: simtrace

simtrace: main.o usb_helper.o usb.o apdu_split.o
	$(CC) $(LDFLAGS) -o $@ $^

%.o: %.c
	$(CC) $(CFLAGS) `pkg-config --cflags libosmocore` -o $@ -c $^


clean:
	@rm -f simtrace *.o
