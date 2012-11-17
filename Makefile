GCC=gcc

all:
	$(GCC) -c main.c
	$(GCC) main.o libfprint.so


clean:
	rm main.o a.out
