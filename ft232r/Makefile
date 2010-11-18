LDFLAGS=-lftdi

bitbang: bitbang.o soft_uart.o
	$(CC) $(LDFLAGS) -o $@ $^

clean:
	rm -f bitbang bitbang.o soft_uart.o

%: %.o
	$(CC) $(LDFLAGS) -o $@ $^
