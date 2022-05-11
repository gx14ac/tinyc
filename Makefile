tinyc: cg.c expr.c gen.c interp.c main.c scan.c tree.c
	cc -o tinyc -g cg.c expr.c gen.c interp.c main.c scan.c tree.c

clean:
	rm -f tinyc *.o

test: tinyc
	./comp1 input01
	cc -o out out.s
	./out
	./comp1 input02
	cc -o out out.s
	./out
