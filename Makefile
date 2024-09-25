all: 
	gcc -o shell simple-shell.c
	gcc -o fib fib.c
	gcc -o helloworld helloworld.c

clean:
	rm -f simple-shell
	rm -f shell
	rm -f fib
	rm -f helloworld
	rm -f ans.txt

