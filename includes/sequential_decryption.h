//
// Created by nicco on 04/07/22.
//

#ifndef PASSWORDDECRYPTION_SEQUENTIAL_DECRYPTION_H
#define PASSWORDDECRYPTION_SEQUENTIAL_DECRYPTION_H

#include <crypt.h>
#include <iostream>
#include <chrono>
#include <vector>
#include <string>
#include <bits/stdc++.h>

using namespace std;

string sequentialDecryption(const char *encrypted_password, const char *salt, const vector<string>& bow){
    //cout<< "SequentialDecryption started!\nBrute force hacking started!" << endl;
    auto *structure = new crypt_data;
    structure->initialized = 0;

    for(const auto& word : bow){
        if(strcmp(crypt_r(word.c_str(), salt, structure), encrypted_password) == 0){
            //cout<< "Password (" << word << ") found in: " << diff.count() << " seconds!" <<endl;
            return word;
        }
    }
    //cout << "Password Not Found!" << endl;
    //cout << "Time elapsed: " << diff.count() << "seconds" << endl;
    return "Password Not Found!";
}


double testSequential(const string& password, const string& salt, vector<string> bow, int executions=100){

    auto *structure = new crypt_data;
    structure->initialized = 0;
    const char *encrypted_password = crypt_r(password.c_str(), salt.c_str(), structure);

    //Create the password positions in BOW in random location
    vector<int> pass_positions;
    for(int i=0; i<executions-2; i++)
        pass_positions.push_back(int(bow.size()/executions)*i);
    pass_positions.push_back(int(bow.size()));
    std::chrono::duration<double> diff{};

    double min_exec_time= 10000 , max_exec_time = 0, mean_exec_time = 0, exec_time;

    for(int execution=0; execution<executions; execution++){

        if(execution != executions-1)
            bow.insert(bow.begin() + pass_positions[execution], password);

        auto start = std::chrono::system_clock::now();
        sequentialDecryption(encrypted_password, salt.c_str(), bow);
        diff = std::chrono::system_clock::now() - start;
        exec_time = diff.count();
        mean_exec_time += exec_time;

        //Check min
        if(exec_time < min_exec_time)
            min_exec_time = exec_time;

        //Check max
        if(exec_time > max_exec_time)
            max_exec_time = exec_time;

        if(execution != executions-1)
            bow.erase(bow.begin()+pass_positions[execution]);
    }

    mean_exec_time /= executions;

    cout << "Sequential Minimum Execution Time: " << min_exec_time << " seconds!"<<endl;
    cout << "Sequential Maximum Execution Time: " << max_exec_time << " seconds!"<<endl;
    cout << "Sequential Mean Execution Time: " << mean_exec_time<< " seconds!"<<endl;

    return mean_exec_time;
}



#endif //PASSWORDDECRYPTION_SEQUENTIAL_DECRYPTION_H
