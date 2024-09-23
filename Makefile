all: 
	gcc -o shell simple-shell.c
	gcc -o fib fib.c
	gcc -o helloworld helloworld.c

clean:
	rm -f shell
	rm -f fib
	rm -f helloworld

