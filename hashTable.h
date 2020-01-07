/********************************************
* COMP15 - FA2019
* PROJECT 2
* ELIZABETH HOM
* 6 DEC 2019
* 
* hashTable.h
*
* hashTable interface - class interface for
* the hashTable class
*********************************************/

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

        struct PathLine {
            string path;                // full path name of the permutation
            int fileLine;               // fileLine the permutation appears on
        };

        struct Permutations {
            string wordPerm;            // permutation of the word
            vector<PathLine> pathLine;  // vector of PathLine structs
        };                              // associated with the permutation

        struct Lowercase {
            string wordLower;           // lowercase version of word hashed
            vector<Permutations> perm;  // vector of Permutation structs where
        };                              // a permutation is a different config.
                                        // of the word's capitalization
        int tableSize;
        vector<Lowercase*> *table;      // hash table structure: 1d array that
                                        // has pointers to Lowercase structs
    public:

        // constructor
        hashTable();

        // destructor
        ~hashTable();

        // hashes word into the hash table
        void hashWord(string lower, string orig, int fileLine, string filePath,
                      int numElem);

        // searches for word, either case sensitive or insensitive
        void findWord(string queryWord, ofstream &output);
        void findWordInsensitive(string queryWord, ofstream &output);

        // returns true if word already exists in table
        bool foundWord(string queryWord, size_t wordKey);

        // returns hash function value of word
        int getKey(string queryWord);

        // returns full line text of a specified word
        string getLineText(string path, int fileLine);

        // returns lowercase version of a string
        string lowercase(string word);

        // expands the hashTable by 2.5 original size, if loadFactor >= 0.75
        void expand();
        void checkLoadFactor(double numElem, double tableSize);

        // helper function that returns an initialized PathLine struct
        PathLine makePathLine(int fileLine, string filePath);

        // inserts new word into the table with initialized struct values
        void insertNewWord(string lower, string orig, int fileLine, string filePath, int wordKey);
};

#endif