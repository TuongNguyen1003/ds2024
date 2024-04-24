#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Define constants for maximum length of path and maximum length of a line
#define MAX_PATH_LENGTH 1000
#define MAX_LINE_LENGTH 2000

// Define structures for key-value pairs
struct KeyValue {
    int key;
    char value[MAX_PATH_LENGTH];
};

// Define Mapper function
void map(char *line, struct KeyValue *output) {
    int pathLength = strlen(line);
    strncpy(output->value, line, MAX_PATH_LENGTH);
    output->key = pathLength;
}

// Define Reducer function
void reduce(struct KeyValue *inputs, int numInputs) {
    int maxLength = 0;
    char longestPath[MAX_PATH_LENGTH];
    
    // Find the longest path among all inputs
    for (int i = 0; i < numInputs; i++) {
        if (inputs[i].key > maxLength) {
            maxLength = inputs[i].key;
            strncpy(longestPath, inputs[i].value, MAX_PATH_LENGTH);
        }
    }
    
    // Print the longest path
    printf("Longest Path: %s\n", longestPath);
}

int main(int argc, char *argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <input file>\n", argv[0]);
        return EXIT_FAILURE;
    }
    
    FILE *inputFile = fopen(argv[1], "r");
    if (inputFile == NULL) {
        perror("Error opening input file");
        return EXIT_FAILURE;
    }
    
    // Define an array to store key-value pairs
    struct KeyValue *output = malloc(sizeof(struct KeyValue));
    
    char line[MAX_LINE_LENGTH];
    while (fgets(line, MAX_LINE_LENGTH, inputFile) != NULL) {
        // Map each line to a key-value pair
        map(line, output);
        
        // Reduce the key-value pair
        reduce(output, 1);
    }
    
    fclose(inputFile);
    free(output);
    
    return EXIT_SUCCESS;
}
