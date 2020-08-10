/*
 * main.cpp
 * 
 * COMP15 - FALL2019
 * Elizabeth Hom (ehom01)
 * Last Modified: December 6, 2019
 * 
 * Purpose: Main driver for spotlight-search. Drives the traversal of
 *          file tree and population of hash table structure, and runs
 *          continuous user-driven query.
 */

#include <iostream>
#include <vector>
#include <fstream>
#include <sstream>

#include "FSTree.h"
#include "DirNode.h"
#include "hashTable.h"

using namespace std;

void savePaths(DirNode *node, vector<string> &paths, string dirName);
int getWords(istream &input, hashTable *table, string path, int numElem);
void getQueries(hashTable *table, ofstream &output);
bool isAlphaNum(char temp);
string stripNonAlphaNum(string inputString);

int main(int argc, char *argv[])
{
    ifstream input;
    ofstream output;
    
    // Vector of paths to all files within file tree
    vector<string> paths;
    int hashNumElem = 0;

    if (argc == 3) {
        string dirName = argv[1];
        string outputFile = argv[2];
        output.open(outputFile);
        FSTree *tree = new FSTree(dirName);
        hashTable *table = new hashTable();
        savePaths(tree->getRoot(), paths, dirName);

        // Obtains all words from each file, hashes into hash table
        for(unsigned long i = 0; i < paths.size(); i++) {
            input.open(paths[i]);
            hashNumElem = getWords(input, table, paths[i], hashNumElem);
            input.close();
        }
        
        // Continuously obtains user queries and acts accordingly
        getQueries(table, output);
        output.close();
        delete tree;
        delete table;
        paths.clear();
    } else {
        cout << "Usage: ./gerp <directory> <output_file>" << endl;
        return EXIT_FAILURE;
    }

    return 0;
}

/*
 * savePaths
 *
 * Parameters: Pointer to node (root), vector containing file tree paths, name
 *             of start directory (string)
 * Returns:    NA
 * Does:       Saves full path name of all files within file tree into paths
 *             vector
 */
void savePaths(DirNode *node, vector<string> &paths, string dirName)
{
    if (node->hasFiles()) {
        int numFiles = node->numFiles();
        for(int i = 0; i < numFiles; i++) {
            string path = "";
            path += dirName + "/" + node->getFile(i);   // gets full path name
            paths.push_back(path);    // saves into vector
        }
    }

    if (node->hasSubDir()) {    // if node has subDirs, recursively enter
        int numSubDir = node->numSubDirs();     // subDirs to obtain paths to
        for(int i = 0; i < numSubDir; i++) {    // those files
            savePaths(node->getSubDir(i), paths, dirName + "/" + 
                     (node->getSubDir(i))->getName());
        }
    }
}

/*
 * getWords
 *
 * Parameters: input stream (ifstream), hashTable, file path (string), current
 *             number of hashed elements (int)
 * Returns:    Number of elements in file
 * Does:       For a given file, reads word by word. For each word, lowercase
 *             version is hashed into table.
 */
int getWords(istream &input, hashTable *table, string path, int numElem)
{
    string currLine, lower, orig;
    int fileLine = 1;
    // Temp vector that holds all words on current line
    vector<string> temp;

    while(not input.eof()) {
        getline(input, currLine);
        temp.clear();
        istringstream ss(currLine);
        // Parses individual words on current line
        while(ss >> orig) {
            orig = stripNonAlphaNum(orig);  // Strips nonAlphaNum chars
            bool found = false;

            // Always push first word in line into temp vector, hash into table
            if (temp.size() == 0) {
               temp.push_back(orig);
               lower = table->lowercase(orig);
               numElem++;
               table->hashWord(lower, orig, fileLine, path, numElem);
            } else {
                for(unsigned long i = 0; i < temp.size(); i++) {
                    /* If line has same word multiple times, do not add into
                       temp vector */
                    if (temp[i] == orig) {
                        found = true;
                        break;
                    }
                }

                // Otherwise, add to temp vector and hash into table
                if (not found) {
                    temp.push_back(orig);
                    lower = table->lowercase(orig);
                    numElem++;
                    table->hashWord(lower, orig, fileLine, path, numElem);
                }
            }
        }
        fileLine++;
    }

    return numElem;
}

/*
 * getQueries
 *
 * Parameters: hash table, output stream (ofstream)
 * Returns:    NA
 * Does:       Continuously prompts user input and reacts accordingly
 */
void getQueries(hashTable *table, ofstream &output)
{
    string command, queryWord;
    do {
        cout << "Query? ";
        cin >> command;
        cout << endl;

        if (command == "@q" or command == "@quit") {
            cout << "Goodbye! Thank you have a nice day." << endl;
            break;
        } else if (command == "@f") {
            // Change output file
            string newOutputFile;
            cin >> newOutputFile;
            output.close();
            output.open(newOutputFile);
        } else if (command == "@i" or command == "@insensitive") {
            // Case insensitive search
            cin >> queryWord;
            table->findWordInsensitive(stripNonAlphaNum(queryWord), output);
        } else {
            // Case sensitive search
            table->findWord(stripNonAlphaNum(command), output);
        }
    } while (command != "@q" or command != "@quit");
}

/*
 * isAlphaNum
 *
 * Parameters: A character, passed in from stripNonAlphaNum()
 * Returns:    Boolean, true if character is alpha-numeric, otherwise false
 * Does:       Assesses whether passed in character is alpha-numeric using
 *             ASCII values.
 */
bool isAlphaNum(char temp)
{
    if (temp >= 48 and temp <= 57)
        return true;
    else if (temp >= 65 and temp <= 90)
        return true;
    else if (temp >= 97 and temp <= 122)
        return true;
    else
        return false;
}

/*
 * stripNonAlphaNum
 *
 * Parameters: word to be processed (string)
 * Returns:    Processed string
 * Does:       Removes all leading and trailing non-alphanumeric characters
 *             from a passed in input string
 */
string stripNonAlphaNum(string inputString)
{
    string outputString = "";
    bool foundFirst = false;
    int firstIndex = 0;
    int lastIndex = inputString.size() - 1;

    // Iterates through every character in string
    for (unsigned long i = 0; i < inputString.length(); i++) {
        char temp = inputString[i];

        if (isAlphaNum(temp)) {
            if (not foundFirst) {
                foundFirst = true;
                firstIndex = i;         // Saves index of 1st alphanumeric char
            }
            lastIndex = i;              // Saves index of last alphanumeric char
        }
    }

    // Parses out leading and trialing non-alphanumeric chars from string
    for (int i = firstIndex; i < lastIndex + 1; i++) {
        char temp = inputString[i];
        outputString = outputString + temp;
    }

    return outputString;
}
