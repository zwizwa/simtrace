LDFLAGS=-lusb -losmocore

all: simtrace

simtrace: main.o usb_helper.o usb.o apdu_split.o
	$(CC) $(LDFLAGS) -o $@ $^

%.o: %.c
	$(CC) $(CFLAGS) -o $@ -c $^


clean:
	@rm -f simtrace *.o
