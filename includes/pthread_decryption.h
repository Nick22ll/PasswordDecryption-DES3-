
#ifndef PASSWORDDECRYPTION_PTHREAD_DECRYPTION_H
#define PASSWORDDECRYPTION_PTHREAD_DECRYPTION_H


#include <crypt.h>
#include <iostream>
#include <chrono>
#include <vector>
#include <string>
#include <bits/stdc++.h>
#include <pthread.h>
#include <cmath>


using namespace std;

//to pass multiple parameters to the start routine (when a pthread is created)
struct partialDecrypt_arguments{
    string encrypted_password;
    string salt;
    vector<string>* bow;
    int start_bow_idx;
    int stop_bow_idx;
    bool *found;
};


void *partialDecrypt(void *arg){
    auto *structure = new crypt_data;
    structure->initialized = 0;

    auto *arguments = (partialDecrypt_arguments*) arg;

    for(int i = arguments->start_bow_idx; i < arguments->stop_bow_idx; i++){
        if(!(*(arguments->found))) {
            if (strcmp(crypt_r(arguments->bow->at(i).c_str(), arguments->salt.c_str(), structure), arguments->encrypted_password.c_str()) == 0) {
                //password found: stop all other threads
                (*(arguments->found)) = true;
                //cout << "Password " << arguments->bow->at(i).c_str() << " found by thread: " << pthread_self() << endl;
                return (void *) (arguments->bow->at(i).c_str());
            }
        }
        else{
            i = arguments->stop_bow_idx;
        }
    }
    //cout << "Password not found! by thread: " << pthread_self() <<endl;
    return (void*)("Password Not Found!");
}


vector<double> testPThread(const string& password, string& salt, vector<string> bow, vector<int> num_threads, int executions = 100){

    auto *structure = new crypt_data;
    structure->initialized = 0;
    char *encrypted_password = crypt_r(password.c_str(), salt.c_str(), structure);


    //Create the password positions in BOW in random location
    vector<int> pass_positions;
    for(int i=0; i<executions-2; i++)
        pass_positions.push_back(int(bow.size()/executions)*i);
    pass_positions.push_back(int(bow.size()));

    vector<double> mean_values;
    std::chrono::duration<double> diff{};

    for(auto thread_num : num_threads){

        double min_exec_time= 10000 , max_exec_time = 0, mean_exec_time = 0;

        for(int execution=0; execution<executions; execution++){

            if(execution != executions-1)
                bow.insert(bow.begin() + pass_positions[execution], password);

            auto start = std::chrono::system_clock::now();

            vector<pthread_t> threads(thread_num);
            vector<partialDecrypt_arguments> arguments(thread_num);

            bool found = false;

            int chunk_length = floor(bow.size()/thread_num);
            for(int i = 0; i < thread_num - 1; i++) {
                //threads creation (assigning a chunk of bow to each thread)
                arguments[i].encrypted_password = encrypted_password;
                arguments[i].salt = salt;
                arguments[i].bow = &bow;
                arguments[i].start_bow_idx = i*chunk_length;
                arguments[i].stop_bow_idx = (i*chunk_length) + chunk_length;
                arguments[i].found = &found;
                if(pthread_create(&threads[i], NULL, partialDecrypt, (void*)&arguments[i]) != 0)
                    cout<<"Error for" <<endl;
            }
            arguments[thread_num-1].encrypted_password = encrypted_password;
            arguments[thread_num-1].salt = salt;
            arguments[thread_num-1].bow = &bow;
            arguments[thread_num-1].start_bow_idx = chunk_length * (thread_num - 1);
            arguments[thread_num-1].stop_bow_idx = bow.size();
            arguments[thread_num-1].found = &found;
            if(pthread_create(&threads[thread_num-1], NULL, partialDecrypt, (void*)&arguments[thread_num-1]) != 0){
                cout<< "Error" << endl;
            }

            for(auto thread:threads)
                pthread_join(thread, NULL);

            auto end = std::chrono::system_clock::now();
            diff = end - start;

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

        cout << "\nTEST with " << thread_num << " threads:" <<endl;
        cout << "Parallel Minimum Execution Time: " << min_exec_time << " seconds!"<<endl;
        cout << "Parallel Maximum Execution Time: " << max_exec_time << " seconds!"<<endl;
        cout << "Parallel Mean Execution Time: " << mean_exec_time<< " seconds!"<<endl;
    }

    return mean_values;
}

#endif //PASSWORDDECRYPTION_PTHREAD_DECRYPTION_H
