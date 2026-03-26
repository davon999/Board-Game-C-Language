build: main


main: *.c
	gcc *.c -g -o main -lm

clean:
	rm main