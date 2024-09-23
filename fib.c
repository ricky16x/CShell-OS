#include <stdio.h>
#include <stdlib.h>

unsigned long long fibonacci(int n) {
    if (n == 0) return 0;
    if (n == 1) return 1;

    unsigned long long a = 0, b = 1;

    for (int i = 2; i <= n; i++) {
        unsigned long long next = a + b;
        a = b;
        b = next;
    }
    return b;
}

int main(int argc, char *argv[]) {
    
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <positive integer>\n", argv[0]);
        return 1;
    }

   
    int n = atoi(argv[1]);
    
    
    unsigned long long result = fibonacci(n);
    printf("fib(%d) = %llu\n", n, result);

    return 0;
}
