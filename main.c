//Initial commit
/*
 * Developed by team members: Logan Christianson and Marissa Gustafson
 * Logan's project contribution: 0%
 * Marissa's project contribution: 0%
**/

#include<stdio.h>
#include<stdlib.h>

//(True) constants
#define MAXIMUM_WORD_LENGTH 16

//Struct listings:


//Function prototypes:


//Main function
int main() {
    //Start by openiong up our text file of words
    FILE *fp = fopen("wordList.txt", "r");
    char wordBank[1000][MAXIMUM_WORD_LENGTH];

    //If our text file doesn't exist, prevent the program from running
    if (fp == NULL) {
        printf("File not found, please include a text document wordList.txt in the source files.\n");
        return 0;
    }

    //Populate our string table
    int count = 0;
    while (!feof(fp)) {
        fscanf(fp, " %s", &wordBank[count]);
    }

    return 0;
}

//Function definitions

/*
 * TO-DO
 * Prompt users to run the game or provide additional input
 * Save any additional input INTO the file
 * Actually run the game
 */