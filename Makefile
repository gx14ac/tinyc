tinyc: expr.c interp.c main.c scan.c tree.c
	cc -o tinyc -g main.c scan.c expr.c interp.c tree.c

tinyc2: expr.c interp.c main.c scan.c tree.c
	cc -o tinyc2 -g main.c scan.c precedence_expr.c interp.c tree.c

clean:
	rm -f tinyc *.o

test: tinyc
	-(./tinyc input01; \
	 ./tinyc input02; \
	 ./tinyc input03; \
	 ./tinyc input04; \
	 ./tinyc input05)

test2: tinyc2
	-(./tinyc2 input01; \
	 ./tinyc2 input02; \
	 ./tinyc2 input03; \
	 ./tinyc2 input04; \
	 ./tinyc2 input05)
