/********************************************
* COMP15 - FA2019
* PROJECT 2
* ELIZABETH HOM
* 6 DEC 2019
* 
* hashTable.cpp
*
* hashTable implementation
*********************************************/

#include <iostream>
#include <string>
#include <functional>
#include <vector>
#include <fstream>
#include "hashTable.h"

using namespace std;

// hash function
// parameters: a string key
// does: returns a size_t index that corresponds with the passed in key
static size_t hashFunction(string key)
{
    return std::hash<string>{}(key);
}

// constructor
// parameters: none
// does:       initializes table (hash ta ble), sets initial hash tableSize
//             to 20011
hashTable::hashTable()
{
    tableSize = 20011;
    table = new vector<Lowercase*>[tableSize];
}

// destructor
// parameters: none
// does:       none
hashTable::~hashTable()
{
    for(int i = 0; i < tableSize; i++) {
        for(int j = 0; j < (int) table[i].size(); j++) {
            delete table[i].at(j);
        }
    }

    delete [] table;
}

// function    hashWord
// parameters: lowercase version of string being hashed, permutation of string,
//             fileLine and filePath of word, and number of elements in hash
//             table at that point
// returns:    none
// does:       hashes the word passed in (by its lowercase version). if the
//             word does not yet exist in the hash table, a new node will be
//             inserted into the table; if the word already exists, then
//             information about the new permutation of the word will be added
//             to the existing node
void hashTable::hashWord(string lower, string orig, int fileLine, string
                         filePath, int numElem)
{
    size_t key = hashFunction(lower) % tableSize;
    bool found = false;

    // if lowercase ver. of word exists in table already:
    if (foundWord(lower, key)) {
        for(int i = 0; i < (int) table[key].size(); i++) {
            if (table[key].at(i)->wordLower == lower) {
                for(int j = 0; j < (int) table[key].at(i)->perm.size(); j++) {
                    if(table[key].at(i)->perm[j].wordPerm == orig) {
                        PathLine temp = makePathLine(fileLine, filePath);
                        table[key].at(i)->perm[j].pathLine.push_back(temp);
                        found = true;   // if perm of word found, add
                        return;         // PathLine struct inside perms
                    }
                }
                if (not found) {        // otherwise, add new perm struct
                    PathLine temp = makePathLine(fileLine, filePath);
                    Permutations permTemp;
                    permTemp.wordPerm = orig;
                    permTemp.pathLine.push_back(temp);
                    table[key].at(i)->perm.push_back(permTemp);
                }
            }
        }
    } else // if word does not exist in table yet, insert new node
        insertNewWord(lower, orig, fileLine, filePath, key);

    checkLoadFactor(numElem, tableSize);    // check loadFactor after every
}                                           // insertion

// function    makePathLine
// parameters: fileLine word occurs on, path to file
// returns:    a filled PathLine struct
// does:       creates a PathLine struct and fill struct with respective
//             fileLine and filePath values
hashTable::PathLine hashTable::makePathLine(int fileLine, string filePath)
{
    PathLine pathLine;
    pathLine.path = filePath;
    pathLine.fileLine = fileLine;

    return pathLine;
}

// function    insertNewWord
// parameters: lowercase version of string being hashed, permutation of string,
//             fileLine and filePath of word, word key of word (from hash fxn)
// returns:    none
// does:       allocates space for a new Lowercase node. fills relevant values
//             in node, then pushes back into the overall hash table
void hashTable::insertNewWord(string lower, string orig, int fileLine, string
                              filePath, int key)
{
    Lowercase *insertNode = new Lowercase;  // creates new Lowercase node
    insertNode->wordLower = lower;
    PathLine temp = makePathLine(fileLine, filePath);

    Permutations permTemp;
    permTemp.wordPerm = orig;

    permTemp.pathLine.push_back(temp);      // pushes back into hash table
    insertNode->perm.push_back(permTemp);
    table[key].push_back(insertNode);   
}

// function    checkLoadFactor
// parameters: number of elements currently in table, current tableSize
// returns:    none
// does:       checks the load factor of the hash table. if load factor exceeds
//             0.75, calls expand() function, which increases the tableSize by
//             5/2
void hashTable::checkLoadFactor(double numElem, double tableSize)
{
    double loadFactor = numElem / tableSize;

    if (loadFactor >= 0.75)
        expand();
}

// function    findWord
// parameters: word string being searched for, ofstream
// returns:    none
// does:       performs case sensitive search, prints appropriate message to
//             output file based on whether the queryWord was found (if found,
//             also prints the word's file path, line the word occurs on, and
//             the full text of the line
void hashTable::findWord(string queryWord, ofstream &output)
{
    //bool found = false;       // search for lowercase version of word
    if (foundWord(lowercase(queryWord), getKey(lowercase(queryWord)))) {
        int key = getKey(lowercase(queryWord));
        for(unsigned long i = 0; i < table[key].size(); i++) {
            // if lowercase version of word found, search for specific perm
            if (table[key].at(i)->wordLower == lowercase(queryWord)) {
                int currPerm = table[key].at(i)->perm.size();
                for(int j = 0; j < currPerm; j++) {
                    // if the perm found, print fileLine & path to outputfile
                    if (table[key].at(i)->perm[j].wordPerm == queryWord) {
                        Permutations *temp = &table[key].at(i)->perm[j];
                        //found = true;
                        int currPL = temp->pathLine.size();
                        for(int k = 0; k < currPL; k++) {
                            string path = temp->pathLine[k].path;
                            int fileLine = temp->pathLine[k].fileLine;
                            output << path << ":" << fileLine << ": "
                                   << getLineText(path, fileLine) << "\n";
                        }
                    }
                }
            }
        }
    } else
        output << queryWord + " Not Found. Try with @insensitive or @i.\n";
}

// function    findWordInsensitive
// parameters: word string being searched for, ofstream
// returns:    none
// does:       performs case insensitive search, prints appropriate message to
//             output file based on whether the queryWord was found (if found,
//             also prints the word's file path, line the word occurs on, and
//             the full text of the line
void hashTable::findWordInsensitive(string queryWord, ofstream &output)
{
    // search for lowercase version of word
    if (foundWord(lowercase(queryWord), getKey(lowercase(queryWord)))) {
        int key = getKey(lowercase(queryWord));

        for(unsigned long i = 0; i < table[key].size(); i++) {
            // if lowercase version of word found, print path & fileLine of all
            // permutations
            if (table[key].at(i)->wordLower == lowercase(queryWord)) {
                int currPerm = table[key].at(i)->perm.size();
                for(int j = 0; j < currPerm; j++) {
                    Permutations *temp = &table[key].at(i)->perm[j];
                    int currPL = temp->pathLine.size();
                    for(int k = 0; k < currPL; k++) {
                        string path = temp->pathLine[k].path;
                        int fileLine = temp->pathLine[k].fileLine;

                        output << path << ":" << fileLine << ": "
                             << getLineText(path, fileLine) << "\n";
                    }
                }
            }
        }
    } else
        output << queryWord + " Not Found.\n";
}

// function    foundWord
// parameters: word string being searched for, the word's key index
// returns:    boolean, true if queryWord already exists in the table, false
//             if not
// does:       searches the table at the word's index (from the hash function)
//             and returns true if the word is found in the table, false if not
bool hashTable::foundWord(string queryWord, size_t key)
{
    for(unsigned long i = 0; i < table[key].size(); i++) {
        if (table[key].at(i)->wordLower == queryWord) {
            return true;
        } else {
            continue;
        }
    }

    return false;
}

// function    getLineText
// parameters: file path, line in which the word appears on
// returns:    string of full text of which the word appears on
// does:       returns the full text of which the queryWord appears on
string hashTable::getLineText(string path, int fileLine)
{
    string fullText = "";
    ifstream input;
    input.open(path);

    for(int i = 0; i < fileLine; i++) {     // traverses file until reaches
        getline(input, fullText);           // correct fileLine
    }
    input.close();

    return fullText;
}

// function    getKey
// parameters: word string being searched for
// returns:    integer index of the word
// does:       uses the hash function to return the integer index/key of the
//             word being searched for
int hashTable::getKey(string queryWord)
{
    size_t key = hashFunction(queryWord);
    key = key % tableSize;

    return key;
}

// function    expand
// parameters: none
// returns:    none
// does:       expands the size of existing table to 5/2 of its original
//             size and rehashes all the existing information in the table
//             into the new table
void hashTable::expand()
{
    int newSize = tableSize * 5/2;
    vector<Lowercase*> *temp = new vector<Lowercase*>[newSize]; // allocates new
                                                                // vector
    for (int i = 0; i < tableSize; i++) {
        for (unsigned long j = 0; j < table[i].size(); j++) {
            size_t key = hashFunction(table[i].at(j)->wordLower);  // rehashes
            key = key % (tableSize * 5/2);                         // old table
            temp[key].push_back(table[i].at(j));                   // into new
        }                                                          // table
    }
    delete [] table;

    table = temp;
    tableSize = tableSize * 5/2;    // update tableSize
}

// function    lowercase
// parameters: a word string
// returns:    the lowercase version of the passed in string
// does:       converts the passed in string to all lowercase
string hashTable::lowercase(string word)
{
    string wordLower = "";
    for(unsigned long i = 0; i < word.length(); i++) {
        char temp = word[i];
        int tempASCII = tolower(temp);
        char temp2 = tempASCII;

        wordLower = wordLower + temp2;
    }

    return wordLower;
}