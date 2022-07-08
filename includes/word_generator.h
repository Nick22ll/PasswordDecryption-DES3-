//
// Created by Nicco on 21/06/2022.
//

#ifndef DES_SEQUENTIAL_2_WORD_GENERATOR_H
#define DES_SEQUENTIAL_2_WORD_GENERATOR_H

// C++ program to print all
// possible strings of length k
#include <bits/stdc++.h>
#include <string>
#include <iostream>
#include <random>
using namespace std;


// The main recursive method to print all possible strings of length k
void printAllKLengthRec(char set[], string prefix,
                        int n, int k, ofstream* file)
{

    // Base case: k is 0,
    // print prefix
    if (k == 0)
    {
        //cout << (prefix) << endl;
        (*file) << prefix << endl;
//      (*file).write(prefix.c_str(), prefix.length());
        return;
    }

    // One by one add all characters
    // from set and recursively
    // call for k equals to k-1
    for (int i = 0; i < n; i++)
    {
        string newPrefix;

        // Next character of input added
        newPrefix = prefix + set[i];

        // k is decreased, because
        // we have added a new character
        printAllKLengthRec(set, newPrefix, n, k - 1, file);
    }

}

void generateBagOfWords(ofstream* file)
{

    int set_dimension = 11 + 26 + 26;
    char characters[set_dimension];
    //Add numeric characters and . /
    for(int i=0; i<11; i++)
        characters[i] = char(i + 46);

    //Add uppercase letters
    for(int i=0; i<26; i++)
        characters[i + 11] = char(i + 65);

    //Add lowercase letters
    for(int i=0; i<26; i++)
        characters[i + 37] = char(i + 97);

    std::shuffle(characters, characters+set_dimension, std::mt19937(std::random_device()()));

    int k = 8;

    printAllKLengthRec(characters, "", set_dimension, k, file);
}


void extractBOW(string bow_path){
    ifstream bow;
    bow.open(bow_path);
    if( !bow )
        cout << "Opening bow failed!" << endl;

    std::vector<string> extracted_bow;

    string line;
    while(getline(bow, line))
    {
        line.pop_back();
        if(line.length() == 8){
            bool discard = false;
            for(int i =0; i<8; i++){
                char letter = line[i];
                discard =  letter < 46 || (letter > 57 && letter < 65) || (letter > 90 && letter < 97) || letter > 122;
                if(discard)
                    break;
            }
            if(!discard)
            {
                extracted_bow.push_back(line);
            }
        }
    }

    bow.close();

    cout<< "Passwords extracted: " << extracted_bow.size() << endl;

    ofstream stream;
    stream.open("../bag_of_words/bag_of_words.txt");
    if( !stream )
        cout << "Opening file failed" << endl;

    stream << "Length:" << extracted_bow.size() << endl;
    for(const auto& value: extracted_bow) {
        stream << value << endl;
    }

    stream.close();
}




#endif //DES_SEQUENTIAL_2_WORD_GENERATOR_H
