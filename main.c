/*
 * Developed by team members: Logan Christianson and Marissa Gustafson
 * Logan's project contribution: 50%
 * Marissa's project contribution: 50%
**/

#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<time.h>
#include<ctype.h>
#include<Math.h>

//(True) constants
#define MAXIMUM_WORD_LENGTH 16
#define MAXIMUM_FILE_LENGTH 2000
#define SCREEN_LENGTH 80
#define SCREEN_HEIGHT 23 //24 if including the entry line

//Globals
int TotalWordNum; //Var for storing total # of words in the word bank
char PlayingBoard[SCREEN_HEIGHT][SCREEN_LENGTH]; //Playing board, printed out for the player to see
char WordBank[MAXIMUM_FILE_LENGTH][MAXIMUM_WORD_LENGTH]; //Word bank of all possible words
char ActiveWords[SCREEN_HEIGHT][MAXIMUM_WORD_LENGTH]; //Word bank of all ACTIVE words (currently in the game board) - not currently used since we're not checking
char BlankLine[SCREEN_LENGTH] = "0000000000000000000000000000000000000000000000000000000000000000000000000000000"; //What a blank line is filled with in PlayingBoard

//Function prototypes:
int addWordToGame(FILE *fp);
void addWordToPlayingBoard();
void printPlayingBoard();
void removeWord(char *word);
void SaveWordsLoop();
void PopulateStringTable(char stringArray[MAXIMUM_FILE_LENGTH][MAXIMUM_WORD_LENGTH]);
double RunTheGame();
char* advanceLine();

//Main function
int main() {
    //If our text file doesn't exist, prevent the program from running - do this here once so we don't have to check later if we run PopulateStringTable multiple times
    FILE *fp = fopen("wordList.txt", "r");
    if (fp == NULL) {
        printf("File not found, please include a text document wordList.txt in the source files.\n");
        return 0;
    }
    fclose(fp);

    //Populate our string table
    PopulateStringTable(WordBank);

    //We're ready to start running the core game loop
    printf("Welcome to Marissa's and Logan's Typing Game!\n");
    while (1) {
        //Prompt the user to add any words they want
        printf("Before the game begins, would you like to add words for the game to potentially use?\nType 'y' for yes, 'n' to continue to the game: ");
        char addWords;
        scanf(" %c", &addWords);
        if (addWords == 'y') { //Since this is an introductory course, we don't technically need to check for bad user input - this is an easy "half-attempt" at it
            SaveWordsLoop();
            PopulateStringTable(WordBank); //Since we've added more words to the file, add them to the word bank by re-creating the word bank (not very fast/efficient)
        }

        //Start running the game after list the rules and an acknowledgement the user is ready
        printf("\nHere's how the game works: words will slowly appear along the top of the console in increasing frequency as the game continues. These words will slowly move down");
        printf(" the screen until they reach the bottom. Once a word has reached the bottom of the console, the game is over. You can remove the words by typing them in. ");
        printf("They can be typed in any order, and this continues until a word has reached the bottom. Good luck!\nType 'y' to begin the game...\n");
        char start;
        while (start != 'y') {
            scanf(" %c", &start);
        }
        clock_t startTime = clock();
        RunTheGame();
        clock_t endTime = clock();
        double runTime = ((double) clock() - startTime) / CLOCKS_PER_SEC;

        //printf("\nendTime: %d \tstartTime: %d", endTime, startTime);
        //printf("\n\tendTime - startTime = %d\tclocks_per_sec = %d\n", (endTime - startTime), CLOCKS_PER_SEC);
        //printf("\trecorded time in seconds: %lf\n", runTime);

        //After the game has finished, display their stats and ask if they want to go again. If not, exit the loop and end the program
        printf("Game over! You made it %.2lf seconds.\nWould you like to play again? Type 'y' to play again, 'n' to end the game: ", runTime * 1000);
        char playAgain;
        scanf(" %c", &playAgain);
        if (playAgain != 'y')
            break;
    }

    printf("Thanks for playing!");

    return 0;
}

//Function definitions


/*
 * This recursive function is used to append single words to our wordList file
 * All necessary user prompts are handled by this function
 * This could be written more dynamically and efficiently, but we're not selling this program, so w/e
**/
void SaveWordsLoop() {
    char toAdd[MAXIMUM_WORD_LENGTH];
    printf("Please enter the word you would like to add (capitalization is ignored):\n");
    scanf("%s", toAdd);
    for(int c = 0; toAdd[c]; c++) {
        toAdd[c] = tolower(toAdd[c]);
    }

    FILE *fp = fopen("wordList.txt", "a"); //We've already checked this file is valid, game won't run without it
    fputs("\n", fp);
    fputs(toAdd, fp);
    fclose(fp);

    char addMore;
    printf("Type 'y' to add another word, type 'n' to start the game: ");
    scanf(" %c", &addMore);
    if (addMore == 'y')
        SaveWordsLoop();
}

/*
 * Populates the given array with strings. The function is self-contained.
 * Because we're being given a copy of the array pointer, we're editing the original array
**/
void PopulateStringTable(char stringArray[MAXIMUM_FILE_LENGTH][MAXIMUM_WORD_LENGTH]) {

    FILE *fp = fopen("wordList.txt", "r");
    int count = 0;
    while (!feof(fp)) {
        fscanf(fp, " %s", stringArray[count]);
		count++;
    }
	TotalWordNum = count;
	fclose(fp);
}

/*
 * This function adds one random word from the WordBank to the PlayingBoard at a random x position at the top most available y position.
 * Probably not the best nor the most efficient way to do it, but its the way it makes sense in my mind.
 * 
 * Logan's comment: we should keep track of the words added to the board.
**/
void addWordToPlayingBoard() {
    //Generate a new word, make sure it doesn't already exist in the game table
	int row = rand() % TotalWordNum;
    char toAdd[MAXIMUM_WORD_LENGTH];
    strcpy(toAdd, WordBank[row]);
	int wordLength = strlen(toAdd);

	//Randomly find the starting x position:
	int startingPosition = (rand() % (SCREEN_LENGTH - wordLength));

    //Generates the line with 0s for spaces and the characters where they should be
    int wordCount = 0;
    for (int col = 0; col < SCREEN_LENGTH - 1; col++) {
        if (col == startingPosition || (col > startingPosition && col < startingPosition + wordLength)) {
            PlayingBoard[0][col] = toAdd[wordCount++];
        } else {
            PlayingBoard[0][col] = '0';
        }
    }
}

/*
 * This function prints the PlayingBoard to the screen.
**/
void printPlayingBoard() {
	for(int i = 0; i<SCREEN_LENGTH; i++){
		printf("-");
	}
	printf("\n");
	for (int i = 0; i < SCREEN_HEIGHT; i++){
        printf("\n", i);
		for (int j = 0; j < SCREEN_LENGTH - 1; j++){
			if (PlayingBoard[i][j] == '0'){
				printf(" ");
			} else {
				printf("%c", PlayingBoard[i][j]);
			}
		}
	}
	printf("\n");
	for(int i = 0; i<SCREEN_LENGTH; i++){
		printf("-");
	}
    printf("\n");
}

void removeWord(char word[MAXIMUM_WORD_LENGTH]) {
	int wordLength = strlen(word);
	char compareTo[MAXIMUM_WORD_LENGTH];
	
	for (int i = SCREEN_HEIGHT - 1; i >= 0; i--) { //Start from the "top" (actually the bottom of the screen, where the player is most likely removing words from)
		for (int j = 0; j < SCREEN_LENGTH; j++) {
            if (PlayingBoard[i][j] != '0') { //If we run into a non-space, we know we've run into the only word in this line
                if ((PlayingBoard[i][j] == word[0])&&((j ==0)||(PlayingBoard[i][j-1] == '0'))) { //If the first letters match, grab the word and compare them to what we're given only if its the first letter of a word
                    for (int c = 0; PlayingBoard[i][j + c] != '0'; c++) {
                        compareTo[c] = PlayingBoard[i][j + c];
                    }

                    if (strcmp(word, compareTo) == 0) {
                        strcpy(PlayingBoard[i], BlankLine);
                        return;
                    }
                } else { //If they don't, it's not the word we're looking for, reset compareTo
                    for(int i = 0; i<MAXIMUM_WORD_LENGTH; i++){
						compareTo[i] = '\0';
					}
                }
            }
		}
	}
}

/*
 * Advances PlayerBoard down 1 line, returning the contents of the lost line, and adding a blank line (of '0's) along the top/0th row
**/
char* advanceLine() {
    char *toReturn = malloc((sizeof(char) * SCREEN_LENGTH));
    
    for (int row = SCREEN_HEIGHT - 1; row >= 0; row--) {
        if (row == SCREEN_HEIGHT - 1) {
            strcpy(toReturn, PlayingBoard[row]);
        }
        
        if (row - 1 >= 0) {
            strcpy(PlayingBoard[row], PlayingBoard[row - 1]);
        } else {
            strcpy(PlayingBoard[row], BlankLine);
        }
    }
    return toReturn;
}

/*
 * This function runs the game (hence the name). Not a recursive function. Calls a lot of other functions.
 * As soon as this function is called, words start appearing in the console for the player to remove.
 * Returns how long the function is ran for (which equals how long the user played the game for).
**/
double RunTheGame() {	
    char input[MAXIMUM_WORD_LENGTH];
	int clockMultiplier;

    //Fill PlayingBoard with blank lines
    for (int c = 0; c < SCREEN_HEIGHT; c++) {
        strcpy(PlayingBoard[c], BlankLine);
    }

	clock_t initialClock = clock();
	clock_t wordClock = clock();
	
	while(strcmp(advanceLine(), BlankLine) == 0){		
        
		clockMultiplier = ((clock()-initialClock)+(clock()-wordClock))/(CLOCKS_PER_SEC*15);
		//Add more words per enter based on 1. overall time, 2. individual word speed
		for(int i = 0; i<1+clockMultiplier; i++){
			advanceLine();
			addWordToPlayingBoard();
		}	
		printPlayingBoard();
		scanf(" %s", input);
		wordClock = clock();
		removeWord(input);
	}
	
	//End of game clock time:
	//clock_t finalClock = clock();
	//printf("\nfinalClock: %d \tinitialClock: %d", finalClock, initialClock);
    //printf("\n\tfinalClock - initialClock = %d\tclocks_per_sec = %d\n", (finalClock - initialClock), CLOCKS_PER_SEC);
	return 0;//((double) (finalClock - initialClock)) / (CLOCKS_PER_SEC);
}










