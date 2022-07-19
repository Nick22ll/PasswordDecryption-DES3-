
#ifndef PASSWORDDECRYPTION_OMP_DECRYPTION_H
#define PASSWORDDECRYPTION_OMP_DECRYPTION_H

#include <crypt.h>
#include <iostream>
#include <chrono>
#include <vector>
#include <bits/stdc++.h>

using namespace std;


string openMPDecryption(const char *encrypted_password, const char *salt, vector<string> bow, int num_threads = 8) {

    //cout << "openMPDecryption started!\nBrute force hacking started!" << endl;

    struct crypt_data data;
    data.initialized = 0;

    bool found = false;
    string pwd;
    unsigned int bow_length = bow.size();

    #pragma omp parallel num_threads(num_threads) default(none) shared(bow_length, bow, found, salt, encrypted_password, pwd)
    {
        auto *structure = new crypt_data;
        structure->initialized = 0;
        #pragma omp for schedule(static)
        for (unsigned int i = 0; i < bow_length; i++) {
            if (!found) {
                if (strcmp(crypt_r(bow[i].c_str(), salt, structure), encrypted_password) == 0) {
                    pwd = bow[i];
                    found = true;
                }
            } else {
                i = bow_length + 1;
            }
        }
    }
    if(found){
        //printf("Password ( %s ) found in: %f seconds", pwd.c_str(), diff.count());
        return pwd;
    }

    //printf("\nPassword Not Found!\n");
    //printf("Time elapsed: %f seconds!\n", diff.count());
    return "Password Not Found!";
}

vector<double> testOMP(const string& password, const string& salt, vector<string> bow, vector<int> num_threads, int executions = 100){

    auto *structure = new crypt_data;
    structure->initialized = 0;
    const char *encrypted_password = crypt_r(password.c_str(), salt.c_str(), structure);

    //Create the password positions in BOW in random location
    vector<int> pass_positions;
    for(int i=0; i<executions-2; i++)
        pass_positions.push_back(int(bow.size()/executions)*i);
    pass_positions.push_back(int(bow.size()));

    vector<double> mean_values;
    std::chrono::duration<double> diff{};

    for(int thread_num = 0; thread_num < 7; thread_num++){

        double min_exec_time= 10000 , max_exec_time = 0, mean_exec_time = 0;

        for(int execution=0; execution<executions; execution++){

            if(execution != executions-1)
                bow.insert(bow.begin() + pass_positions[execution], password);
            auto start = std::chrono::system_clock::now();
            openMPDecryption(encrypted_password, salt.c_str(), bow, num_threads[thread_num]);
            diff = std::chrono::system_clock::now() - start;
            double exec_time = diff.count();

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
        mean_values.push_back(mean_exec_time);

        cout << "\nTEST with " << num_threads[thread_num] << " threads:" <<endl;
        cout << "Parallel Minimum Execution Time: " << min_exec_time << " seconds!"<<endl;
        cout << "Parallel Maximum Execution Time: " << max_exec_time << " seconds!"<<endl;
        cout << "Parallel Mean Execution Time: " << mean_exec_time<< " seconds!"<<endl;
    }

    return mean_values;
}

#endif //PASSWORDDECRYPTION_OMP_DECRYPTION_H
