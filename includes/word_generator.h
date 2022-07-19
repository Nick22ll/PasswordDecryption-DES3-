
#ifndef DES_SEQUENTIAL_2_WORD_GENERATOR_H
#define DES_SEQUENTIAL_2_WORD_GENERATOR_H

#include <bits/stdc++.h>
#include <string>
#include <iostream>
#include <random>
using namespace std;

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
