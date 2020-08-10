/*
 * hashTable.h
 * 
 * COMP15 - FALL2019
 * Elizabeth Hom (ehom01)
 * Last Modified: December 6, 2019
 * 
 * Purpose: Interface of the hashTable class, which oversees the hash table
 *          structure and its relevant functions.
 */

#include <iostream>
#include <string>
#include <vector>
#include <functional>

using namespace std;

#ifndef HASHTABLE_H_
#define HASHTABLE_H_

class hashTable
{
    private:

       /*
        * PathLine
        *
        * Contains full path name to file containin the permutation and
        * line number the permutation appears on within the file.
        */
        struct PathLine {
            string path;
            int fileLine;
        };

       /*
        * Permutations
        *
        * Contains permutation of a word and a vector of PathLine structs
        * associated with the permutation.
        */
        struct Permutations {
            string wordPerm;
            vector<PathLine> pathLine;
        };

       /*
        * Lowercase
        *
        * Contains lowercase version of word being hashed and vector of
        * Permutation structs where each permutation is a different
        * configuration of the word's capitalization.
        */
        struct Lowercase {
            string wordLower;
            vector<Permutations> perm;
        };

        int tableSize;
        
       /* Hash table structure: 1D array that contains pointers to
        * Lowercase structs
        */
        vector<Lowercase*> *table;

    public:

        // Constructor
        hashTable();

        // Destructor
        ~hashTable();

        // Hashes word into the hash table
        void hashWord(string lower, string orig, int fileLine, string filePath,
                      int numElem);

        // Searches for word (Case sensitive & insensitive)
        void findWord(string queryWord, ofstream &output);
        void findWordInsensitive(string queryWord, ofstream &output);

        // Returns true if word already exists in table
        bool foundWord(string queryWord, size_t wordKey);

        // Returns hash function value of word
        int getKey(string queryWord);

        // Returns full line text of a specified word
        string getLineText(string path, int fileLine);

        // Returns lowercase version of a string
        string lowercase(string word);

        // Expands the hashTable by 2.5 original size, if loadFactor >= 0.75
        void expand();
        void checkLoadFactor(double numElem, double tableSize);

        // Returns an initialized PathLine struct
        PathLine makePathLine(int fileLine, string filePath);

        // Inserts new word into the table with initialized struct values
        void insertNewWord(string lower, string orig, int fileLine, string filePath, int wordKey);
};

#endif
