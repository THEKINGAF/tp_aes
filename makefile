C=gcc
CFLAGS=
LDFLAGS=
EXEC=ex1q2 ex1q3 exercise2

all: $(EXEC)

ex1q2: ex1q2.o tp-utils.o aes-128_enc.o
		$(CC) -o $@ $^ $(LDFLAGS)

ex1q3: ex1q3.o tp-utils.o aes-128_enc.o
		$(CC) -o $@ $^ $(LDFLAGS)

exercise2: exercise2.o tp-utils.o aes-128_enc.o
		$(CC) -o $@ $^ $(LDFLAGS)

*.o: *.c
		$(CC) -o $@ -c $< $(CFLAGS)

.PHONY: clean
clean:
		rm -rf *.o
