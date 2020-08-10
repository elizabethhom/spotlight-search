# spotlight-search

Given a starting directory, traverses and indexes every word within the file tree into a database. Allows the user to query the database (case sensitive and insensitive) and prints query results accordingly. Written as a final project for Tufts University's Data Structures Course in Fall 2019.

### Compile & Run
* Compile program using "make"
* Run with executable: ./gerp \<directoryToIndex\> \<outputFile\>
  * directoryToIndex: starting directory that will be traversed and indexed, and is a full path name (example: /home/ehom/comp15).
  * outputFile: file name that query results will be printed to.

### Purpose
*spotlight-search* searches for a given query word within a file tree from a given starting path point. The program takes a starting directory, scans every file within the given file tree, and indexes every word. Relevant information about each indexed word (e.g. the word itself, full file path, line number the word occurs on in the file) is saved inside a hash table structure. Once the entire database has been stored successfully, the program responds to user queries and prints the query results to an output file specified in the command line.

### Commands
* Case-Insensitive Search
* Case-Sensitive Search: "@i" or "@insensitive" before query word
* Change output file: "@f \<newOutputFile>\"
* Quit program: "@q" or "@quit"

### Program Architecture
The program's overall structure a hash table composed of a one-dimensional array. Each index of the array contains information about a word found within the file tree. Each word is hashed based on its all lowercase version (for example, "HeLlO" would be hashed as "hello") to account for varying permuations of a single word. Each index of the hash table contains a vector of pointers to "Lowercase" structs, which contains the word in lowercase and a vector of "Permutations structs," which contains information about every permutation (for example: "heLlo," "HELLO," "hellO") of the current word. Specifically, each Permutation struct contains a vector of "PathLine" structs, which contains information about the file path to the word and line number that each permutation was found on.

These nesting data structures were designed to allow for efficient case insensitive and sensitive search, and to accomodate large databases. Small, medium, and very large Gutenberg libraries were used as test cases while *spotlight-search* was being built.

### Files
* main.cpp: Main driver for the program. Given a starting directory to search within on the command line, main.cpp builds a hash table data structure (see *Program Architecture*) by individually indexing and saving every word, its path, and line number within all files in the file tree. Once built, *spotlight-search* continuously prompts the user for commands to modify or quit the search and prints the query results to the output file specified on the command line.
* hashTable.cpp: Implementation of the hashTable class oversees the hash table structure and its relevant functions, such as indexing words into the table, searching (case-sensitive and insensitive).
* hashTable.h: Interface of the hashTable class.
* FSTree.h: Interface of the FSTree class, provided by COMP15 staff. Uses a file-system tree to represent directories, subdirectories, and files.
* DirNode.h: Interface of the DirNode class, provided by COMP15 staff. Building block for the FSTree class and represents folders.
* Makefile: Contains code that builds the program.





