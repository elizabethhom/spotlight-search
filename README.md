***********************************************************
* ELIZABETH HOM                                           *
* 6 DEC 2019                                              *
*                                                         *
* README - SPOTLIGHT SEARCH "GERP"                        *
***********************************************************

# Hello

*******************************
* Code Sample Important Note: *
*******************************

!!! Please refer to hashTable.cpp and main.cpp for files that best demonstrate
Elizabeth Hom's programming skills and style.

The Spotlight Search "Gerp" program serves to demonstrate Elizabeth Hom's
ability in writing clear, well-documented, modular, and accurate code, and was
her final project in COMP15: Data Structures. The program serves to showcase
her ability in designing and implementing complex data structures with multiple
levels of abstraction in working with very large directories (for example,
the Gutenberg libraries), as well as key skills such as writing insertion and
search algorithms.

****************
* Compile/run: *
****************
    - Compile program using
            "make" or "make gerp"
    - run executable with
            ./gerp directoryToIndex outputFile

            where directoryToIndex is:
                the starting directory that will be traversed and indexed, and
                is a full path name (example: /h/ehom01/comp15).
            outputFile is:
                the name of the file that the query results will be printed to.

    - example:
        the executable "./gerp /h/ehom01/comp15/hw outputfile.txt" will create
        a hash table that indexes every word within every file accessible from
        the directory /h/ehom01/comp15/hw, as well as the full file path and
        file line that the given word appears on. Once the hash table is built,
        the user is continuously prompted for queries to search for words
        within the hash table. It can perform both case sensitive & insensitive
        searches, and all the query results are printed to outputfile.txt, in
        the following format:
            PathNameToFile:FileLineNumOfWord: FullTextOfLineWordAppearsOn
        unless the output file is changed using the command "@f". 

********************
* Program Purpose: *
********************

The purpose of the program is to create a "spotlight search" mechanism that
searches for a given query word within a database. The program takes a starting
directory, and indexes every word and its relevant information (such as the word
itself, the full file path to that word, the line number of the file the word
occurs on) within all files and subdirectories within the starting directory.
This information is saved within a hash table that is created by the program,
and then responds to user queries and prints the results to an output file that
is specified by the user in the command line.

The program has a few different functionalities:
    (1) case insensitive search
    (2) case sensitive search: "@i" or "@insensitive" before the query word
    (3) change the output file: "@f" followed by the new output file name
    (4) quit the program: "@q" or "@quit"

**********
* Files: *
**********

main.cpp:
    Main driver for the program. Based on the starting directory specified by
    the user in the command line, main.cpp builds a hash table by individually
    indexing every word (and the full file path and line the word occurs on) in
    all files accessible from the starting directory. The program continuously
    prompts for user commands (modify the search or quit) and prints the query
    results to the appropriate output file.

hashTable.cpp:
    Implementation of hashTable class. Holds the hash table structure, as well
    as functions related to the hash table, such as indexing words into the
    hash table, searching for words (case sensitive and insensitive), etc.

hashTable.h:
    Interface of the hashTable class.

FSTree.h:
    Interface of the FSTree class, which was provided by instructors of COMP15.
    Uses a file-system tree to represent directories, subdirectories, and
    files. The data structure is an n-ary tree, and is used to navigate through
    folders and directories inside a computer. Contains functions related to
    the FSTree, such as constructing the FSTree, destructing it, and a function
    that returns the root of the tree (DirNode class).

DirNode.h:
    Interface of the DirNode class, and was provided by instructors of COMP15.
    Is a building block for the FSTree class, and is our representaton of
    folders. Every DirNode instance has a string name, list of files in the
    directory, and a list of subdirectories. DirNode.h contains both public and
    private functions regarding a node's name, subdirectories, files, etc.

Makefile:
    Contains the code that builds the program.

********************
* Data Structures: *
********************

The program contains multiple levels of abstraction. The overall hash table is
a one dimensional array named "table". Each slot in the array contains
information about a word hashed from a file. Each word is hashed based on its
all lowercase version (for example, the word "HeLlO" would be hashed as
"hello") to account for varying permutations of a single word. Each slot in
"table" contains a vector of pointers to "Lowercase" structs. Each Lowercase
struct contains a string of the lowercase version of the word and a vector of
"Permutation" structs, which contains information about every permutation of
the hashed lowercase word (for example, the indexed location for the hashed
word "hello" may have permutations like "Hello," "heLLo," or "hElLo" stored).
A Permutation struct contains a string that holds each unique permutation, and
a vector of PathLine structs, which contains information about the file path
and line within the file that each permutation appears on.

The program has these levels of abstraction to make case insensitive and case
sensitive search as efficient as possible. Becuase of the vector of Permutation
structs, case insensitive search prints the entire contents of the vector to
the output file, while case sensitive search searches through the vector of
unique Permutations and only prints the correct permutation's information to
the output file.

***********
* Memory: *
***********

The program contains no memory leaks, and all heap memory is deallocated by the
conclusion of the program through the destructor and main.cpp.
