tinyc: cg.c expr.c gen.c main.c misc.c scan.c stmt.c tree.c
	cc -o tinyc -g cg.c expr.c gen.c main.c misc.c scan.c stmt.c tree.c

clean:
	rm -f tinyc *.o

test: tinyc
	./tiny input01
	cc -o out out.s
	./out
