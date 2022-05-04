tinyc: expr.c interp.c main.c scan.c tree.c
	cc -o tinyc -g main.c scan.c expr.c interp.c tree.c

clean:
	rm -f tinyc *.o
