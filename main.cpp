#include <crypt.h>
#include "includes/word_generator.h"
#include "includes/pthread_decryption.h"
#include "includes/sequential_decryption.h"
#include "includes/omp_decryption.h"
#include <iostream>
#include <chrono>
#include <pthread.h>

using namespace std;

vector<string> loadFromBOW(string &bow_path);


int main() {

    //string leaked_passwords = "../bag_of_words/leaked_passwords.txt";
    //extractBOW(leaked_passwords);

    string bow_path = "../bag_of_words/bag_of_words.txt";

    string password = "bertaGUI";
    string salt = "paperino";


    vector<string> bow = loadFromBOW(bow_path);
    cout<< "BOW loaded!" << endl;

    vector<int> threads_num{2, 4, 8, 12,  16, 20, 24};

    //start sequential decryption
    cout << "Starting Sequential Test" << endl;
    double mean_sequential = testSequential(password, salt, bow);

    //start omp decryption
    cout << "Starting OpenMP Test" << endl;
    vector<double> mean_parallel = testOMP(password, salt, bow, threads_num);
    for(int i=0;i<threads_num.size();i++)
        cout<< "Speedup with " << threads_num[i] << " threads is: " << mean_sequential/mean_parallel[i] << endl;

    //start pthread decryption
    cout << " Starting PThread Test" << endl;
    vector<double> mean_pthread = testPThread(password, salt, bow, threads_num);
    for(int i=0;i<threads_num.size();i++)
        cout<< "Speedup with " << threads_num[i] << " threads is: " << mean_sequential/mean_pthread[i] << endl;

    return 0;
}


vector<string> loadFromBOW(string &bow_path){
    vector<string> loaded_bow;

    ifstream bow;
    bow.open(bow_path);
    if( !bow )
        cout << "Opening bow failed!" << endl;

    string word;

    //Extract bow length
    std::getline(bow, word);

    while(std::getline(bow, word))
        loaded_bow.push_back(word);
    cout<<"Words: " << loaded_bow.size() << endl;
    bow.close();
    return loaded_bow;
}
