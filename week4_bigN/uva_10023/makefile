output_diff: my.out 10023.out
	diff my.out 10023.out
my.out: run
	./run < 10023.in > my.out
run: 10023.c
	gcc -Wall 10023.c -o run
clean:
	rm -f run my.out