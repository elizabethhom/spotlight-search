/*
 * hashTable.cpp
 * 
 * COMP15 - FALL2019
 * Elizabeth Hom (ehom01)
 * Last Modified: December 6, 2019
 * 
 * Purpose: Implementation of the hashTable class oversees the hash table
 *          structure and its relevant functions, such as indexing words
 *          into the table, searching (case-sensitive and insensitive).
 */

#include <iostream>
#include <string>
#include <functional>
#include <vector>
#include <fstream>
#include "hashTable.h"

using namespace std;

static const int TABLESIZE = 20011;

/*
 * hashFunction
 *
 * Parameters: Word to hash (string)
 * Returns:    Index (size_t) that corresponds with the passed in key
 * Does:       Returns hash index
 */
static size_t hashFunction(string key)
{
    return std::hash<string>{}(key);
}


/*
 * Constructor
 *
 * Parameters: NA
 * Returns:    NA
 * Does:       Initializes hash table, sets hash table initial size to 20011
 */
hashTable::hashTable()
{
    tableSize = TABLESIZE;
    table = new vector<Lowercase*>[tableSize];
}

/*
 * Destructor
 *
 * Parameters: NA
 * Returns:    NA
 * Does:       Destroys heap memory associated with hash table.
 */
hashTable::~hashTable()
{
    for(int i = 0; i < tableSize; i++) {
        for(int j = 0; j < (int) table[i].size(); j++) {
            delete table[i].at(j);
        }
    }

    delete [] table;
}

/*
 * hashWord
 *
 * Parameters: Lowercase word being hashed (string), permutation of word
 *             (string), line number word appears on in file (int), path
 *             to file (string), current number of hash table elements (int)
 * Returns:    NA
 * Does:       Hashes passed in lowercase word. If word does not yet exist
 *             in hash table, new node is inserted into table. Otherwise,
 *             new permutation's information is updated to existing node.
 */
void hashTable::hashWord(string lower, string orig, int fileLine, string
                         filePath, int numElem)
{
    size_t key = hashFunction(lower) % tableSize;
    bool found = false;

    // If lowercase word exists in table already
    if (foundWord(lower, key)) {
        for(int i = 0; i < (int) table[key].size(); i++) {
            if (table[key].at(i)->wordLower == lower) {
                for(int j = 0; j < (int) table[key].at(i)->perm.size(); j++) {
                    if(table[key].at(i)->perm[j].wordPerm == orig) {
                        /* If permutation of word found, then add PathLine
                           struct inside permutations */
                        PathLine temp = makePathLine(fileLine, filePath);
                        table[key].at(i)->perm[j].pathLine.push_back(temp);
                        found = true;
                        return;
                    }
                }
                if (not found) {
                    // Otherwise, add new perm struct
                    PathLine temp = makePathLine(fileLine, filePath);
                    Permutations permTemp;
                    permTemp.wordPerm = orig;
                    permTemp.pathLine.push_back(temp);
                    table[key].at(i)->perm.push_back(permTemp);
                }
            }
        }
    } else {
        // If word does not exist in table yet, insert new node
        insertNewWord(lower, orig, fileLine, filePath, key);
    }

    // Check loadFactor after each insertion
    checkLoadFactor(numElem, tableSize);
}

/*
 * makePathLine
 *
 * Parameters: line number word appears on in file (int), path
 * Returns:    Populated PathLine struct
 * Does:       Creates PathLine struct and poulates with respective
 *             fileLine and filePath values.
 */
hashTable::PathLine hashTable::makePathLine(int fileLine, string filePath)
{
    PathLine pathLine;
    pathLine.path = filePath;
    pathLine.fileLine = fileLine;

    return pathLine;
}

/*
 * insertNewWord
 *
 * Parameters: lowercase word being hashed (string), permutation of word (string),
 *             word's fileLine (int) and filePath (string), hash function key (int)
 * Returns:    NA
 * Does:       Allocates space for new Lowercase node and populates relevant
 *             values in node. Pushes back into overall hash table.
 */
void hashTable::insertNewWord(string lower, string orig, int fileLine, string
                              filePath, int key)
{
    // Creates new Lowercase node
    Lowercase *insertNode = new Lowercase;
    insertNode->wordLower = lower;
    PathLine temp = makePathLine(fileLine, filePath);

    Permutations permTemp;
    permTemp.wordPerm = orig;

    // Push back into hash table
    permTemp.pathLine.push_back(temp);
    insertNode->perm.push_back(permTemp);
    table[key].push_back(insertNode);   
}

/*
 * checkLoadFactor
 *
 * Parameters: current number of elements in table (double), total size of
 *             table (double)
 * Returns:    NA
 * Does:       Checks load factor of the hash table. If load factor > 0.75,
 *             calls expand(), which increases tableSize by 5/2.
 */
void hashTable::checkLoadFactor(double numElem, double tableSize)
{
    double loadFactor = numElem / tableSize;

    if (loadFactor >= 0.75)
        expand();
}

/*
 * findWord
 *
 * Parameters: query word (string), output stream (ofstream)
 * Returns:    NA
 * Does:       Performs case sensitive search, prints appropriate query result
 *             to output file depending on if word was found. If found, prints
 *             word's full file path, line the word is found on, and full text
 *             of the line. Otherwise, prints error message.
 */
void hashTable::findWord(string queryWord, ofstream &output)
{
    // Search for lowercase version of word
    if (foundWord(lowercase(queryWord), getKey(lowercase(queryWord)))) {
        int key = getKey(lowercase(queryWord));
        
        for(unsigned long i = 0; i < table[key].size(); i++) {
            
            // If lowercase version of word found, search for specific perm
            if (table[key].at(i)->wordLower == lowercase(queryWord)) {
                int currPerm = table[key].at(i)->perm.size();
                
                for(int j = 0; j < currPerm; j++) {
                    // If perm found, print fileLine and path to outputfile
                    if (table[key].at(i)->perm[j].wordPerm == queryWord) {
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
        }
    } else
        output << queryWord + " Not Found. Try with @insensitive or @i.\n";
}

/*
 * findWordInsensitive
 *
 * Parameters: query word (string), output stream (ofstream)
 * Returns:    NA
 * Does:       Performs case insensitive search, prints appropriate query result
 *             to output file depending on if word was found. If found, prints
 *             word's full file path, line the word is found on, and full text
 *             of the line. Otherwise, prints error message.
 */
void hashTable::findWordInsensitive(string queryWord, ofstream &output)
{
    // Search for lowercase version of word
    if (foundWord(lowercase(queryWord), getKey(lowercase(queryWord)))) {
        int key = getKey(lowercase(queryWord));

        for(unsigned long i = 0; i < table[key].size(); i++) {
            
            /* If lowercase version of word found, print path & fileLine of all
               permutations */
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

/*
 * foundWord
 *
 * Parameters: query word (string), hash table key (size_t)
 * Returns:    True if queryWord exists in table. Otherwise, false.
 * Does:       Searches table at word's hash table key index and returns boolean
 *             value depending on if word was found.
 */
bool hashTable::foundWord(string queryWord, size_t key)
{
    for(unsigned long i = 0; i < table[key].size(); i++) {
        if (table[key].at(i)->wordLower == queryWord) {
            return true;
        }
    }

    return false;
}

/*
 * getLineText
 *
 * Parameters: file path (string), line in file word found on (int)
 * Returns:    String of full text of which word appears on
 * Does:       Returns full text of line query word appears on.
 */
string hashTable::getLineText(string path, int fileLine)
{
    string fullText = "";
    ifstream input;
    input.open(path);

    for(int i = 0; i < fileLine; i++) {     // Traverses file until reaches
        getline(input, fullText);           // correct fileLine
    }
    input.close();

    return fullText;
}

/*
 * getKey
 *
 * Parameters: word being searched for (string)
 * Returns:    Integer index of word in hash table (int)
 * Does:       Uses hash function to return integer index/key of queried word.
 */
int hashTable::getKey(string queryWord)
{
    size_t key = hashFunction(queryWord);
    key = key % tableSize;

    return key;
}

/*
 * expand
 *
 * Parameters: NA
 * Returns:    NA
 * Does:       Expands size of table to 5/2 of current size and rehashes all
 *             existing contents into new table.
 */
void hashTable::expand()
{
    int newSize = tableSize * 5/2;
    // Allocates new vector
    vector<Lowercase*> *temp = new vector<Lowercase*>[newSize];

    // Rehashes old table into new table
    for (int i = 0; i < tableSize; i++) {
        for (unsigned long j = 0; j < table[i].size(); j++) {
            size_t key = hashFunction(table[i].at(j)->wordLower);
            key = key % (tableSize * 5/2);
            temp[key].push_back(table[i].at(j));
        }
    }
    delete [] table;

    table = temp;
    tableSize = tableSize * 5/2;    // Update tableSize
}

// function    lowercase
// parameters: a word string
// returns:    the lowercase version of the passed in string
// does:       converts the passed in string to all lowercase

/*
 * lowercase
 *
 * Parameters: word (string)
 * Returns:    Lowercase version of passed in word
 * Does:       Converts word to all lowercase.
 */
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
