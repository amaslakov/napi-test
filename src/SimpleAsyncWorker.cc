#include "SimpleAsyncWorker.h"
#include <chrono>
#include <thread>
#include <vector>
#include <mutex>
#include <cstdlib>
#include <deque>
#include <iostream>

static std::mutex barrier;
std::deque<std::vector<double>> myDeque;

std::vector<double> get_random_bundle() {
    std::vector<double> bundle;
    for (int i = 0; i < 5; ++i) {
        bundle.push_back((double) rand() / RAND_MAX);
    }
    return bundle;
}

void simulation_thread() {
    barrier.lock();
    std::cout << std::this_thread::get_id() << " started" << std::endl;
    myDeque.push_back(get_random_bundle());
    barrier.unlock();
}

SimpleAsyncWorker::SimpleAsyncWorker(Function& callback, int runTime)
    : AsyncWorker(callback), runTime(runTime) {};

void SimpleAsyncWorker::Execute() {
    std::vector<std::thread> threads;
    int i;

    for (i = 0; i < 8; ++i) {
        threads.push_back(std::thread(&simulation_thread));
    }
    for (auto& t : threads) {
        t.join();
    }

    std::vector<double> temp_vector;
    std::cout.precision(3);
    int size = myDeque.size();
    for (i = 0; i < size; ++i) {
        temp_vector = myDeque.front();
        myDeque.pop_front();
        std::cout << "[";
        for (unsigned long j = 0; j < temp_vector.size(); ++j) {
            std::cout << std::fixed << temp_vector[j] << ", ";
        }
        std::cout << "]" << std::endl;
    }

    if (runTime == 4) {
        SetError("Oops! Failed after 'working' 4 seconds.");
    }
};

void SimpleAsyncWorker::OnOK() {
    std::string msg = "SimpleAsyncWorker returning after 'working' " + std::to_string(runTime) + " seconds.";
    Callback().Call({ Env().Null(), String::New(Env(), msg) });
};