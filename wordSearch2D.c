#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "puzzle2D.h"
#define maxSize 100

int main(int argc, char*argv[]){
    int argvIndex, word_len, puzzle_len, argumentsFound = 0, found, solutionGiven = 0, arguments;
    char *puzzleFileName, *wordListFileName, *solutionFileName, word[101];
    long long int word_hash;
    FILE *wordlist_file; // The file that contains the list of words to be found
    FILE *puzzle_file;   // The file that contains the puzzle to be solved
    FILE *solution_file; // The file to where the solution to the words given will be written to
    
    // Getting the required arguments denoted by their flag
    arguments = (argc-1)/2;
    argvIndex = 1;
    for (int i = 0; i<arguments; i++){
        if (strcmp(argv[argvIndex], "-p") == 0){
            puzzleFileName = argv[argvIndex+1];
            argumentsFound++;
        }
        if (strcmp(argv[argvIndex], "-l") == 0){
            word_len = atoi(argv[argvIndex+1]);
            argumentsFound++;
        }
        if (strcmp(argv[argvIndex], "-w") == 0){
            wordListFileName = argv[argvIndex+1];
            argumentsFound++;
        }
        if (strcmp(argv[argvIndex], "-o") == 0){
            solutionFileName = argv[argvIndex+1];
            solutionGiven = 1;
        }
        argvIndex = argvIndex + 2;
    }
 
    // Error Checking Files and number of arguments given to program
    // Checks that the correct number of arguments are given to the program
    if (argc != 7 && argc != 9){
        fprintf(stderr, "Usage: ./wordSearch2D -p <puzzle_file> -l <word_length> -w <wordlist_file> [-o <solution_file>]\n");
        exit(6);
    }
    // Checks that the arguments given to the program are the correct arguments
    if (argumentsFound != 3){
        fprintf(stderr, "Usage: ./wordSearch2D -p <puzzle_file> -l <word_length> -w <wordlist_file> [-o <solution_file>]\n");
        exit(6);
    }
    // Checks if the given puzzle_file exists in the given directory
    puzzle_file = fopen(puzzleFileName, "r");
    if (puzzle_file == NULL){
        fprintf(stderr, "Error: Puzzle file %s does not exist\n", puzzleFileName);
        exit(4);
    }
    // Checks if the given wordlist_file exists in the given directory
    wordlist_file = fopen(wordListFileName, "r");
    if (wordlist_file == NULL){
        fprintf(stderr, "Error: Wordlist file %s does not exist\n", wordListFileName);
        exit(5);
    }
    // If no solution file was given, set the solution_file to be by default output.txt
    if (solutionGiven == 1){
        solution_file = fopen(solutionFileName, "w");
        if (solution_file == NULL){
            fprintf(stderr, "Encountered Error\n");
            fclose(puzzle_file);
            fclose(wordlist_file);
            exit(7);
        }
    }
    else{
        solution_file = fopen("output.txt", "w");
    }
  
 
    puzzle_len = len_puzzle(puzzle_file);
    // Restart puzzle file for reading so fgets is pointing back to the first line of puzzle_file
    fclose(puzzle_file);
    puzzle_file = fopen(puzzleFileName, "r");
 
    char puzzle[puzzle_len][puzzle_len];
    long long int hashTable[puzzle_len][puzzle_len][8]; // hashTable indicies are as such hashTable[row][col][direction]
    create_puzzle(puzzle_len, puzzle, puzzle_file);
 
    // Get all possible hashes for the given word length in all directions and put it into the hash table
    get_hRightHashs(puzzle_len, puzzle, hashTable, word_len);
    get_hLeftHashs(puzzle_len, puzzle, hashTable, word_len);
    get_vDownHashs(puzzle_len, puzzle, hashTable, word_len);
    get_vUpHashs(puzzle_len, puzzle, hashTable, word_len);
    get_TlBrDiagHashs(puzzle_len, puzzle, hashTable, word_len);
    get_BrBackTlDiagHashs(puzzle_len, puzzle, hashTable, word_len);
    get_BlTrDiagHashs(puzzle_len, puzzle, hashTable, word_len);
    get_TrBackBlDiagHashs(puzzle_len, puzzle, hashTable, word_len);
    
    // Begin finding the words in the puzzle by getting their hash value and searching within the hashTable for the same value
    while (fgets(word, 101, wordlist_file) != NULL){
        if (word[word_len] == '\n'){
            word[word_len] = '\0';
        }
        if (strlen(word) != word_len){
            fprintf(stderr, "Encountered Error\n");
            fclose(puzzle_file);
            fclose(wordlist_file);
            exit(7);
        }
        else{
            word_hash = hash(word);
            found = findCoords(puzzle_len, hashTable, word_hash, word, solution_file);
            if (found == 1){
                fprintf(solution_file, "%s;(0,0);0\n", word);
            }
        }
    }
    
    return 0;
}
