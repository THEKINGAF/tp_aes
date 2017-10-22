C=gcc
CFLAGS=
LDFLAGS=
EXEC=exercise1 exercise2

all: $(EXEC)

exercise1: exercise1.o tp-utils.o aes-128_enc.o
		$(CC) -o $@ $^ $(LDFLAGS)

exercise1.o: exercise1.c
		$(CC) -o $@ -c $< $(CFLAGS)

exercise2: exercise2.o tp-utils.o aes-128_enc.o
		$(CC) -o $@ $^ $(LDFLAGS)

exercise2.o: exercise2.c
		$(CC) -o $@ -c $< $(CFLAGS)

tp-utils.o: tp-utils.c
		$(CC) -o $@ -c $< $(CFLAGS)

aes-128_enc.o: aes-128_enc.c
		$(CC) -o $@ -c $< $(CFLAGS)

clean:
		rm -rf *.o
