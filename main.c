/*
 * Developed by team members: Logan Christianson and Marissa Gustafson
 * Logan's project contribution: 0%
 * Marissa's project contribution: 0%
**/

/*
 * I've gone ahead and finished the program structure/backbone. I rewrote the function that adds words to the file and made it recursive, and eliminated some unnecessary code.
 * I didn't delete your code so if you want to compare what we wrote or would rather use what you wrote, let me know and we can work it out, I just noticed some syntax and
 * logic errors we would need to work through otherwise, and decided to rewrite it so we can call it recursively instead.
 * 
 * We probably won't need to touch main() much further, everything else we'll need to do will be done in RunTheGame(). Why not run the game stuff in main()? Because it's a
 * good programming habit to segmentize your program into "tasks" - where functions are self-contained and can be ran in many different scopes. Fortunately for us, RunTheGame()
 * is only gonna need to be ran in 1 instance, so we don't need to be careful with it - however, we should still segment that task into its own function. So I did.
 * 
 * So continuing on, we'll probably want to create an "entry" struct to hold some info about each word that's scrolling down the console, like a numerical ID to reference it by,
 * the actual string, its location (x & y), and maybe other info too.
 * We'll still probably need a few more functions for displaying the full "board" of words. Might want to save the board state in an array h-tall, where h is the number of horizontal
 * lines our cygwin console displays by default.
 * RunTheGame() needs to be flesh out too. We'll need a loop inside it for continually printing the board then asking for input. Is there a timer function available in C? Maybe
 * we could run the display update in a timer? IDK how feasible that is.
*/

#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<time.h>
#include<ctype.h>
#include<Math.h>

//(True) constants
#define MAXIMUM_WORD_LENGTH 16
#define MAXIMUM_FILE_LENGTH 2000
#define END_GAME_LINE 15
#define SCREEN_LENGTH 80
//Set number of total words:
	int totalWordNum;
//Set playing board
char playingBoard[END_GAME_LINE+20][SCREEN_LENGTH];

//Set current lowest line
int lowestLine = 0;	


//Struct listings:


//Function prototypes:
int addWordToGame(FILE *fp);
void addWordToPlayingBoard(char wordBank[MAXIMUM_FILE_LENGTH][MAXIMUM_WORD_LENGTH]);
void printPlayingBoard();
void removeWord(char *word);
void SaveWordsLoop();
void PopulateStringTable(char stringArray[MAXIMUM_FILE_LENGTH][MAXIMUM_WORD_LENGTH]);
double RunTheGame(char stringArray[MAXIMUM_FILE_LENGTH][MAXIMUM_WORD_LENGTH]);

//Main function
int main() {
    //Start by openiong up our text file of words
    char wordBank[MAXIMUM_FILE_LENGTH][MAXIMUM_WORD_LENGTH];
	
	srand(time(NULL));

	

    //If our text file doesn't exist, prevent the program from running - do this here once so we don't have to check later if we run PopulateStringTable multiple times
    FILE *fp = fopen("wordList.txt", "r");
    if (fp == NULL) {
        printf("File not found, please include a text document wordList.txt in the source files.\n");
        return 0;
    }
    fclose(fp);

    //Populate our string table
    PopulateStringTable(wordBank);

    //We're ready to start running the core game loop
    printf("Welcome to Marissa's and Logan's Typing Game!\n");
    while (1) {
        //Prompt the user to add any words they want
        printf("Before the game begins, would you like to add words for the game to potentially use?\nType 'y' for yes, 'n' to continue to the game: ");
        char addWords;
        scanf(" %c", &addWords);
        if (addWords == 'y') { //Since this is an introductory course, we don't technically need to check for bad user input - this is an easy "half-attempt" at it
            SaveWordsLoop();
            PopulateStringTable(wordBank); //Since we've added more words to the file, add them to the word bank by re-creating the word bank (not very fast/efficient)
        }

        //Start running the game after list the rules and an acknowledgement the user is ready
        printf("\nHere's how the game works: words will slowly appear along the top of the console in increasing frequency as the game continues. These words will slowly move down");
        printf(" the screen until they reach the bottom. Once a word has reached the bottom of the console, the game is over. You can remove the words by typing them in. ");
        printf("They can be typed in any order, and this continues until a word has reached the bottom. Good luck!\nType 'y' to begin the game...\n");
        char start;
        while (start != 'y') {
            scanf(" %c", &start);
        }
        double runTime = RunTheGame(wordBank);

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
	totalWordNum = count;
	fclose(fp);
}

/*
 * This function adds one random word from the wordBank to the playingBoard at a random x position at the top most available y position.
 * Probably not the best nor the most efficient way to do it, but its the way it makes sense in my mind.
**/

void addWordToPlayingBoard(char wordBank[MAXIMUM_FILE_LENGTH][MAXIMUM_WORD_LENGTH]){
	
	//Randomly find which row to take a word from wordBank from:
	int row = rand()%totalWordNum;
	int wordLength = 0;

	//Find the length of the actual word
	for(int i = 0; i<MAXIMUM_WORD_LENGTH; i++){
		if(wordBank[row][i]!= '\0'){
			wordLength++;
		}
	}

	//Randomly find the starting x position:
	int startingPosition = (rand()% (SCREEN_LENGTH-wordLength+1));
	
	//find the first line that has the space open for the word
	int space = 0;
	int j = -1;
	while(space != wordLength +1){
		j++;
		space = 0;
		for(int i = startingPosition; i<=startingPosition+wordLength; i++){
			if(playingBoard[j][i] == '0'){
				space++;
			}
		}
	}
	
	//Put the word into the playingBoard
	for(int i = startingPosition; i<=startingPosition+wordLength; i++){
		if(i < startingPosition+wordLength){
			playingBoard[j][i] = wordBank[row][i-startingPosition];
		}else{
			playingBoard[j][i] = '0';
		}
	}

	//Update the lowest line currently in the game
	if(j > lowestLine){
		lowestLine = j;
	}
}

/*
 * This function prints the playingBoard to the screen.
**/

void printPlayingBoard(){
	
	for(int i = 0; i<=lowestLine; i++){
		for(int j = 0; j<SCREEN_LENGTH; j++){
			if(playingBoard[i][j] == '0'){
				printf(" ");
			}else{
				printf("%c", playingBoard[i][j]);
			}
		}
		printf("\n");
	}
	
	for(int i = 0; i<END_GAME_LINE-lowestLine; i++){
		printf("\n");
	}
	
}

void removeWord(char *word){
	int wordLength = 0;
	
	//Find the actual word length
	for(int i = 0; i<MAXIMUM_WORD_LENGTH; i++){
		if(word[i] != '0'){
			wordLength++;
		}
	}
	
	//Go through playingBoard and search for the word
	int match = 0;
	int wordCount = 0;
	//The following two variables are to remember where the indexes of where the match was found
	int rowMatch = 0;
	int colMatch = 0;
	int foundIt = 0;
	
	for(int i = 0; i<=END_GAME_LINE+20; i++){
		for(int j = 0; j<SCREEN_LENGTH; j++){
			if(playingBoard[i][j] == word[wordCount]){
				match++;
				wordCount++;
				if(match +1 == wordLength){
					rowMatch = i;
					colMatch = j-wordLength+2;
					foundIt = 1;
				}
			}else{
				match = 0;
				wordCount = 0;
			}
		}
	}
	
	//If the number of matches == the length of the word, we replace all the chars w 0
	//Replace with 0 bc of how I set up printPlayingBoard
	if(foundIt == 1){
		printf("Match found at [%d][%d]\n", rowMatch, colMatch);
		for(int j = colMatch; j<wordLength+colMatch-1; j++){
			printf("Replacing %c with 0\n", playingBoard[rowMatch][j]);
			playingBoard[rowMatch][j] = '0';
		}
	}

}

/*
 * This function runs the game (hence the name). Not a recursive function. Calls a lot of other functions.
 * As soon as this function is called, words start appearing in the console for the player to remove.
 * Returns how long the function is ran for (which equals how long the user played the game for).
**/
double RunTheGame(char wordBank[MAXIMUM_FILE_LENGTH][MAXIMUM_WORD_LENGTH]) {
	
	
    char input[MAXIMUM_WORD_LENGTH];
	int startingNumWords = 60;
	
	//Initialize input as zeros
	for(int i = 0; i<MAXIMUM_WORD_LENGTH; i++){
		input[i] = '0';
	}
	
	//Initialize the playingBoard as  0, this helps with comparisons in addWordToPlayingBoard and printPlayingBoard
	for(int i = 0; i<=END_GAME_LINE; i++){
		for(int j = 0; j<=SCREEN_LENGTH; j++){
			playingBoard[i][j] = '0';
		}
	}

	//start initial clock
	clock_t  initialClock = clock();
	
	for(int i = 0; i< startingNumWords; i++){
		addWordToPlayingBoard(wordBank);
	}
	int numWordsToAdd = 5;
	
	while((lowestLine!=END_GAME_LINE)){
		
		printPlayingBoard();
		scanf(" %s", input);
		removeWord(input);
		
		
		
		for(int i = 0; i<numWordsToAdd; i++){
			addWordToPlayingBoard(wordBank);
		}
		
	}
	//End of game clock time:
	clock_t finalClock = clock();
	
	//These are not in the units of seconds, this is what I found online...not sure why its not seconds.
	return (double)(finalClock-initialClock)/CLOCKS_PER_SEC;
}










