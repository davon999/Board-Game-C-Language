# Any modifciations to this makefile will not be reflected in the checker, only add debugging information if you want this to reflect the real environment
build: main

	
main:
	gcc src/*.c -g -o checker/main -lm

clean:
	rm -f checker/main
