//Initial commit
/*
 * Developed by team members: Logan Christianson and Marissa Gustafson
 * Logan's project contribution: 0%
 * Marissa's project contribution: 0%
**/

#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<time.h>

//(True) constants
#define MAXIMUM_WORD_LENGTH 16

//Struct listings:


//Function prototypes:
void addWord(char *word);
int addWordToGame(FILE *fp);

//Main function
int main() {
    //Start by openiong up our text file of words
    FILE *fp = fopen("wordList.txt", "r");
    char wordBank[2000][MAXIMUM_WORD_LENGTH];

	srand(time(NULL));

    //If our text file doesn't exist, prevent the program from running
    if (fp == NULL) {
        printf("File not found, please include a text document wordList.txt in the source files.\n");
        return 0;
    }

    //Populate our string table
    int count = 0;
    while (!feof(fp)) {
        fscanf(fp, " %s", &wordBank[count]);
		count++;
    }
	fclose(fp);

	//Ask if ready to start game:
	char start;
	printf("Are you ready to start? (y/n)\n");
	scanf("%c", &start);

	if(start == 'n'){
		return 0;
	}

	//Ask if want to add words to game
	char input;
	printf("Would you like to add words to the game?(y/n)\n");
	scanf(" %c", &input);
	if(input == 'y'){
		addWordToGame(fp);
	}

    return 0;
}

//Function definitions

/*
 *@author Marissa
 *This function randomly chooses the starting position of each word and prints it
 *It should pick its x position, the y position should always be at the top
 *The world should always be contained in the board, i.e. have enough space for its right for its length
 *
 */
void addWord(char *word){
	int lengthOfScreen = 80;
	int wordLength = strlen(word);
	int startingPosition = (rand()% (lengthOfScreen-wordLength));
	//printf("Starting is : %d\n", startingPosition);
	
	for(int i = 0; i<startingPosition; i++){
		printf(" ");
	}
	printf("%s\n", word);
}

/*
 *@author Marissa
 *This function appends a user-given word to the end of the wordList.txt file.
 *This addition continues between games.
 *
 */
int addWordToGame(FILE *fp){
	//Reopen file to append
	fp = fopen("wordList.txt", "a");
	
	//If our text file doesn't exist, prevent the program from running
	if (fp == NULL) {
		printf("File not found, please include a text document wordList.txt in the source files.\n");
		return 0;
	}
	
	char newWord[20];
	char input = 'y';
	
	//Add words until they don't want to add more
	while (input == 'y') {
		printf("Enter the word you would like to add to game: \n");
		scanf(" %s", &newWord);
		fputs("\n", fp);
		fputs(newWord, fp);
		printf("Would you like to add another word to the game? (y/n)\n");
		scanf(" %c", &input);
	}
	fclose(fp);
	return 0;
}

/*
 * TO-DO
 * Prompt users to run the game or provide additional input
 * Save any additional input INTO the file
 * Actually run the game
 */