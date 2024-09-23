#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[]) {
    // Check for the correct number of arguments
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <name>\n", argv[0]);
        return 1;
    }

    // Get the name from the command line argument
    char *name = argv[1];

    // Print the greeting
    printf("Hello, World, %s!\n", name);

    return 0;
}
