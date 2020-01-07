/********************************************
* COMP15 - FA2019
* PROJECT 2
* ELIZABETH HOM
* 6 DEC 2019
* 
* main.cpp
*
* interactive main for gerp
*********************************************/

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
    vector<string> paths;   // vector containing paths to all files from given
    int hashNumElem = 0;    // start directory

    if (argc == 3) {
        string dirName = argv[1];
        string outputFile = argv[2];
        output.open(outputFile);
        FSTree *tree = new FSTree(dirName);
        hashTable *table = new hashTable();
        savePaths(tree->getRoot(), paths, dirName);

        // gets all words from every file, hashes each one into hash table
        for(unsigned long i = 0; i < paths.size(); i++) {
            input.open(paths[i]);
            hashNumElem = getWords(input, table, paths[i], hashNumElem);
            input.close();
        }
        getQueries(table, output);  // continuously gets user queries and
        output.close();             // acts accordingly
        delete tree;
        delete table;
        paths.clear();
    } else {
        cout << "Usage:  gerp directory output_file" << endl;
        return EXIT_FAILURE;
    }

    return 0;
}

// function    savePaths
// parameters: pointer to a node (root), vector containing paths to all files
//             from the starting node, name of starting directory
// returns:    none
// does:       saves full path name of all files reachable from the starting
//             directory into paths vector
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

// function    getWords
// parameters: ifstream, hashTable, file path, number of elements hashed thus
//             far
// returns:    number of elements in the file passed in
// does:       for a given file, reads in individual words in file until reach
//             end of file. for each word, its lowercase version is hashed into
//             the table
int getWords(istream &input, hashTable *table, string path, int numElem)
{
    string currLine, lower, orig;
    int fileLine = 1;
    vector<string> temp;    // vector that holds all words on a line

    while(not input.eof()) {
        getline(input, currLine);
        temp.clear();
        istringstream ss(currLine);
        while(ss >> orig) {         // parses individual words on line
            orig = stripNonAlphaNum(orig);  // strip nonAlphaNum chars
            bool found = false;

            if (temp.size() == 0) {     // always push first word in line
               temp.push_back(orig);    // into temp vector, hash into
               lower = table->lowercase(orig);  // table
               numElem++;
               table->hashWord(lower, orig, fileLine, path, numElem);
            } else {
                for(unsigned long i = 0; i < temp.size(); i++) {
                    if (temp[i] == orig) {   // if line has the same word
                        found = true;        // multiple times, do not add into
                        break;               // temp vector
                    }
                }

                if (not found) {            // otherwise, add to temp vector
                    temp.push_back(orig);   // and hash into table
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

// function    getQueries
// parameters: hashTable ofstream
// returns:    none
// does:       continuously prompts for user input and program acts
//             accordingly (@q = quit program, @f = change file, @i = case
//             insensitive search, otherwise performs case sensitive search)
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
        } else if (command == "@f") {   // change outputFile
            string newOutputFile;
            cin >> newOutputFile;
            output.close();
            output.open(newOutputFile);
        // case insensitive search
        } else if (command == "@i" or command == "@insensitive") {
            cin >> queryWord;
            table->findWordInsensitive(stripNonAlphaNum(queryWord), output);
        // case sensitive search
        } else {
            table->findWord(stripNonAlphaNum(command), output);
        }
    } while (command != "@q" or command != "@quit");
}

// function    isAlphaNum
// parameters: a character, passed in from stripNonAlphaNum()
// returns:    true if the character is alpha-numeric, false if not
// does:       a helper function that assesses whether the passed in character
//             is alpha-numeric using ASCII values
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

// function    stripNonAlphaNum
// parameters: a string intended to be processed
// returns:    the processed string, outputString
// does:       removes all leading and trailing non-alphanumeric characters
//             from a passed in inputString
string stripNonAlphaNum(string inputString)
{
    string outputString = "";
    bool foundFirst = false;
    int firstIndex = 0;
    int lastIndex = inputString.size() - 1;

    for (unsigned long i = 0; i < inputString.length(); i++) {
        char temp = inputString[i];     // iterates thru every char in string

        if (isAlphaNum(temp)) {         // if char is alphanumeric
            if (not foundFirst) {
                foundFirst = true;
                firstIndex = i;         // saves index of 1st alphanumeric char
            }
            lastIndex = i;              // index of last alphanumeric char
        }
    }

    for (int i = firstIndex; i < lastIndex + 1; i++) {  // parses out leading &
        char temp = inputString[i];                     // trailing nonalphanum
        outputString = outputString + temp;             // characters from str
    }

    return outputString;
}