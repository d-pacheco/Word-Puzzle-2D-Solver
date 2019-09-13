#include <math.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#define MAXSIZE 101

int len_puzzle(FILE *puzzle_file){
 // puzzle_file: A file pointer to the file that contains the puzzle to be solved
 // Reads the first line of the puzzle to find the length of the nXn puzzle
    char puzzle_line[110];
    fgets(puzzle_line, 110, puzzle_file);
    return strlen(puzzle_line)-1;
}

void create_puzzle(int puzzle_len, char puzzle[puzzle_len][puzzle_len], FILE *puzzle_file){
 // puzzle_len: The length of the nXn puzzle
 // puzzle: A 2D array that contains the puzzle to be solved
 // puzzle_file: A file pointer to the file that contains the puzzle to be solved
 // Takes the input from the puzzle_file and stores in the 2D array puzzle
    char puzzle_line[MAXSIZE];
    
    for (int i = 0; i < puzzle_len; i++){
        fgets(puzzle_line, 110, puzzle_file);
        for (int j = 0; j < puzzle_len; j++){
            puzzle[i][j] = puzzle_line[j];
        }
    }
}

long long int hash(char word[]){
 // word: A 1D array that contains the word to be hashed
 // Hashes the word with a prime of 101 and returns a long long int
    int p = 101, x = 0, word_len;
    long long int hash_value = 0;
    for (int i = 0; word[i] != '\0' && word[i] != '\n'; i++){
        x++;
    }
    word_len = x;
    x--;
    for (int j = 0; j < word_len; j++){
        hash_value = hash_value + ((word[j] * pow(p,x)));
        x--;
    }
    return hash_value;
}

long long int update_hash(long long int substr_hash, char perv_letter, char new_letter, int word_len){
    // substr_hash: The hash to be updated
    // prev_letter: The letter to be removed from substr_hash
    // new_letter: The letter to be added to the substr_hash
    // Updates the substr_hash using a rolling hash
    int p = 101, x = word_len -1;
    return ( ( (substr_hash - (perv_letter*(pow(p,x))))*p ) + new_letter );
}

void get_hRightHashs(int puzzle_len, char puzzle[puzzle_len][puzzle_len],long long int hashTable[puzzle_len][puzzle_len][8], int word_len){
    // Get all the hashes in the Horizontal Right Direction and stores it in the hashTable with at the indecies [row][col][0]
    int col;
    long long int substr_hash;
    char substr[MAXSIZE];
    for (int row = 0; row < puzzle_len; row++){
        for (int i = 0; i < word_len; i++){
            substr[i] = puzzle[row][i];
            substr[i+1] = '\0';
        }
        col = 0;
        substr_hash = hash(substr);
        while (col < puzzle_len){
            if (col + word_len-1 >= puzzle_len){
                hashTable[row][col][0] = 0;
            }
            else{
                hashTable[row][col][0] = substr_hash;
                substr_hash = update_hash(substr_hash, puzzle[row][col], puzzle[row][col+word_len], word_len);
            }
            col++;
        }
    }
}

void get_hLeftHashs(int puzzle_len, char puzzle[puzzle_len][puzzle_len],long long int hashTable[puzzle_len][puzzle_len][8], int word_len){
    // Get all the hashes in the Horizontal Left Direction and stores it in the hashTable with at the indecies [row][col][1]
    int col;
    long long int substr_hash;
    char substr[MAXSIZE];
    for (int row = 0; row < puzzle_len; row++){
        col = puzzle_len-1;
        for (int i = 0; i < word_len; i++){
            substr[i] = puzzle[row][col];
            substr[i+1] = '\0';
            col--;
        }
        col = puzzle_len-1;
        substr_hash = hash(substr);
        while (col >= 0){
            hashTable[row][col][1] = substr_hash;
            if (col - word_len+2 <= 0){
                hashTable[row][col][1] = 0;
            }
            else{
                substr_hash = update_hash(substr_hash, puzzle[row][col], puzzle[row][col-word_len], word_len);
            }
            col--;
        }
    }
}

void get_vDownHashs(int puzzle_len, char puzzle[puzzle_len][puzzle_len],long long int hashTable[puzzle_len][puzzle_len][8], int word_len){
    // Get all the hashes in the Vertical Down Direction and stores it in the hashTable with at the indecies [row][col][2]
    int row;
    long long int substr_hash;
    char substr[MAXSIZE];
    
    for (int col = 0; col < puzzle_len; col++){
        row = 0;
        for (int i = 0; i < word_len; i++){
            substr[i] = puzzle[row][col];
            substr[i+1] = '\0';
            row++;
        }
        row = 0;
        substr_hash = hash(substr);
        while (row < puzzle_len){
            if (row + word_len > puzzle_len){
                hashTable[row][col][2] = 0;
            }
            else{
                hashTable[row][col][2] = substr_hash;
                substr_hash = update_hash(substr_hash, puzzle[row][col], puzzle[row+word_len][col], word_len);
            }
            row++;
        }
    }
}

void get_vUpHashs(int puzzle_len, char puzzle[puzzle_len][puzzle_len],long long int hashTable[puzzle_len][puzzle_len][8], int word_len){
    // Get all the hashes in the Vertical Up Direction and stores it in the hashTable with at the indecies [row][col][3]
    int row;
    long long int substr_hash;
    char substr[MAXSIZE];
    
    for (int col = 0; col < puzzle_len; col++){
        row = puzzle_len-1;
        for (int i = 0; i < word_len; i++){
            substr[i] = puzzle[row][col];
            substr[i+1] = '\0';
            row--;
        }
        row = puzzle_len-1;
        substr_hash = hash(substr);
        while(row >= 0){
            if (row - word_len+1 < 0){
                hashTable[row][col][3] = 0;
            }
            else{
                hashTable[row][col][3] = substr_hash;
                substr_hash = update_hash(substr_hash, puzzle[row][col], puzzle[row-word_len][col], word_len);
            }
            row--;
        }
    }
}

void get_TlBrDiagHashs(int puzzle_len, char puzzle[puzzle_len][puzzle_len],long long int hashTable[puzzle_len][puzzle_len][8], int word_len){
    // Get all the hashes in the Top Left Bottom Right Diagonal Direction and stores it in the hashTable with at the indecies [row][col][4]
    long long int substr_hash;
    char substr[MAXSIZE];
    
    for (int row = 0; row < puzzle_len; row++){
        int row_pointer = row, col = 0;
        for (int i = 0; i < word_len; i++){
            substr[i] = puzzle[row_pointer][col];
            substr[i+1] = '\0';
            row_pointer++;
            col++;
        }
        col = 0;
        row_pointer = row;
        substr_hash = hash(substr);
        while (row_pointer < puzzle_len){
            if (row_pointer + word_len > puzzle_len){
                hashTable[row_pointer][col][4] = 0;
            }
            else{
                hashTable[row_pointer][col][4] = substr_hash;
                substr_hash = update_hash(substr_hash, puzzle[row_pointer][col], puzzle[row_pointer+word_len][col+word_len], word_len);
            }
            col++;
            row_pointer++;
        }
    }
    
    for (int col = 1; col < puzzle_len; col++){
        int col_pointer = col, row = 0;
        for (int i = 0; i < word_len; i++){
            substr[i] = puzzle[row][col_pointer];
            substr[i+1] = '\0';
            row++;
            col_pointer++;
        }
        col_pointer = col;
        row = 0;
        substr_hash = hash(substr);
        while (col_pointer < puzzle_len){
            if (col_pointer + word_len-1 >= puzzle_len){
                hashTable[row][col_pointer][4] = 0;
            }
            else{
                hashTable[row][col_pointer][4] = substr_hash;
                substr_hash = update_hash(substr_hash, puzzle[row][col_pointer], puzzle[row+word_len][col_pointer+word_len], word_len);
            }
            col_pointer++;
            row++;
        }
    }
}

void get_BrBackTlDiagHashs(int puzzle_len, char puzzle[puzzle_len][puzzle_len],long long int hashTable[puzzle_len][puzzle_len][8], int word_len){
    // Get all the hashes in the Bottom Right Backwards Top Left Direction and stores it in the hashTable with at the indecies [row][col][5]
    long long int substr_hash;
    char substr[MAXSIZE];
    
    for (int row = puzzle_len-1; row >= 0; row--){
        int row_pointer = row, col = puzzle_len-1;
        for (int i = 0; i < word_len; i++){
            substr[i] = puzzle[row_pointer][col];
            substr[i+1] = '\0';
            row_pointer--;
            col--;
        }
        col = puzzle_len-1;
        row_pointer = row;
        substr_hash = hash(substr);
        while (row_pointer >= 0){
            if (row_pointer - word_len+1 < 0){
                hashTable[row_pointer][col][5] = 0;
            }
            else{
                hashTable[row_pointer][col][5] = substr_hash;
                substr_hash = update_hash(substr_hash, puzzle[row_pointer][col], puzzle[row_pointer-word_len][col-word_len], word_len);
            }
            col--;
            row_pointer--;
        }
    }
    
    for (int col = puzzle_len-2; col > 0; col--){
        int col_pointer = col, row = puzzle_len-1;
        for (int i = 0; i < word_len; i++){
            substr[i] = puzzle[row][col_pointer];
            substr[i+1] = '\0';
            row--;
            col_pointer--;
        }
        col_pointer = col;
        row = puzzle_len-1;
        substr_hash = hash(substr);
        while (col_pointer >= 0){
            if (col_pointer - word_len+1 < 0){
                hashTable[row][col_pointer][5] = 0;
            }
            else{
                hashTable[row][col_pointer][5] = substr_hash;
                substr_hash = update_hash(substr_hash, puzzle[row][col_pointer], puzzle[row-word_len][col_pointer-word_len], word_len);
            }
            col_pointer--;
            row--;
        }
    }
}

void get_BlTrDiagHashs(int puzzle_len, char puzzle[puzzle_len][puzzle_len],long long int hashTable[puzzle_len][puzzle_len][8], int word_len){
    // Get all the hashes in the Bottom Left Top Right Direction and stores it in the hashTable with at the indecies [row][col][6]
    long long int substr_hash;
    char substr[MAXSIZE];
    
    for (int row = puzzle_len-1; row >= 0; row--){
        int row_pointer = row, col = 0;
        for (int i = 0; i < word_len; i++){
            substr[i] = puzzle[row_pointer][col];
            substr[i+1] = '\0';
            row_pointer--;
            col++;
        }
        col = 0;
        row_pointer = row;
        substr_hash = hash(substr);
        while (row_pointer >= 0){
            if (row_pointer - word_len+1 < 0){
                hashTable[row_pointer][col][6] = 0;
            }
            else{
                hashTable[row_pointer][col][6] = substr_hash;
                substr_hash = update_hash(substr_hash, puzzle[row_pointer][col], puzzle[row_pointer-word_len][col+word_len], word_len);
            }
            col++;
            row_pointer--;
        }
    }
    
    for (int col = 1; col < puzzle_len; col++){
        int col_pointer = col, row = puzzle_len-1;
        for (int i = 0; i < word_len; i++){
            substr[i] = puzzle[row][col_pointer];
            substr[i+1] = '\0';
            row--;
            col_pointer++;
        }
        col_pointer = col;
        row = puzzle_len-1;
        substr_hash = hash(substr);
        while (col_pointer < puzzle_len){
            if (col_pointer + word_len-1 >= puzzle_len){
                hashTable[row][col_pointer][6] = 0;
            }
            else{
                hashTable[row][col_pointer][6] = substr_hash;
                substr_hash = update_hash(substr_hash, puzzle[row][col_pointer], puzzle[row-word_len][col_pointer+word_len], word_len);
            }
            col_pointer++;
            row--;
        }
    }
}

void get_TrBackBlDiagHashs(int puzzle_len, char puzzle[puzzle_len][puzzle_len],long long int hashTable[puzzle_len][puzzle_len][8], int word_len){
    // Get all the hashes in the Top Right Backwards Bottom Left Direction and stores it in the hashTable with at the indecies [row][col][7]
    long long int substr_hash;
    char substr[MAXSIZE];
    
    for (int col = puzzle_len-1; col >= 0; col--){
        int col_pointer = col, row = 0;
        for (int i = 0; i < word_len; i++){
            substr[i] = puzzle[row][col_pointer];
            substr[i+1] = '\0';
            col_pointer--;
            row++;
        }
        row = 0;
        col_pointer = col;
        substr_hash = hash(substr);
        while (col_pointer >= 0){
            if (col_pointer - word_len+1 < 0){
                hashTable[row][col_pointer][7] = 0;
            }
            else{
                hashTable[row][col_pointer][7] = substr_hash;
                substr_hash = update_hash(substr_hash, puzzle[row][col_pointer], puzzle[row+word_len][col_pointer-word_len], word_len);
            }
            col_pointer--;
            row++;
        }
    }
    
    for (int row = 1; row < puzzle_len; row++){
        int row_pointer = row, col = puzzle_len-1;
        for (int i = 0; i < word_len; i++){
            substr[i] = puzzle[row_pointer][col];
            substr[i+1] = '\0';
            row_pointer++;
            col--;
        }
        row_pointer = row;
        col = puzzle_len-1;
        substr_hash = hash(substr);
        while (row_pointer < puzzle_len){
            if (row_pointer + word_len-1 >= puzzle_len){
                hashTable[row_pointer][col][7] = 0;
            }
            else{
                hashTable[row_pointer][col][7] = substr_hash;
                substr_hash = update_hash(substr_hash, puzzle[row_pointer][col], puzzle[row_pointer+word_len][col-word_len], word_len);
            }
            row_pointer++;
            col--;
        }
    }
}

int findCoords(int puzzle_len, long long int hashTable[puzzle_len][puzzle_len][8], long long int word_hash, char word[MAXSIZE], FILE *solution_file){
    // Find the Row, Col, and direction of the given word_hash within the hashTable if it exists
    for (int direction = 0; direction < 8; direction++){
        for (int row = 0; row < puzzle_len; row++){
            for (int col = 0; col < puzzle_len; col++){
                if (hashTable[row][col][direction] == word_hash){
                    fprintf(solution_file, "%s;(%d,%d);%d\n", word, row, col, direction+1);
                    return 0;
                }
            }
        }
    }
    return 1;
}
