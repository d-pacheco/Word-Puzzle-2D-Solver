# Word-Puzzle-2D-Solver

This program uses the Rabin-Karp algorithm to file the starting location and the direction of a given work in a 2D word-search grid. If the given word does not exist the grid then the output will be the word followed (0,0);0 indicating the word has no location or direction in the puzzle.
The program is able to solve a 100x100 puzzle with words given of of equal length.

## How to use the program

After compiling the program with the follow command:
```
Compile the program:
make

To clean after program is finish being used:
make clean
```
You can either run the program with the following commands:
```
./wordsearch2D -p <puzzle_file> -l <word_length> -w <wordlist_file> [-o <solution_file>]
```
The solution file is an optional argument to where you would like to have so solution saved to, if no solution file is given the file output.txt will be generate and have solutions saved within that file.

## What I struggled with

- Working in the horizontal directions and knowing when the end of the array has been reached and that it does not go out of bounds.

## What I learned from this project

- Working with a multi-file program from the begging of the project to better structure and organization
- Traversing large 2D and 3D arrays
- Passing files between functions
- Creating a new file from within the program and writing to the file
- Writing output to a file