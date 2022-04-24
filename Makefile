tinyc: main.c scan.c
	gcc -o tinyc -g main.c scan.c

clean:
	rm -f tinyc *.o
