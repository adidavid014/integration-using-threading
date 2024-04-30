#include <iostream>
#include <cmath>
#include <pthread.h>
#include <vector>
#include <chrono>
#include <random>

using namespace std;

struct ThreadData {
    double lowerBound;
    double upperBound;
    int iterations;
    double result;
};

double sinx(double x) {
    return sin(x);
}

void* monteCarloEstimate(void* arg){
    ThreadData* data = (ThreadData*)arg;
    double totalSum = 0.0;
    random_device rd;
    mt19937 gen(rd()); //better use of random for threads
    uniform_real_distribution<> dis(data->lowerBound, data->upperBound);

    for (int i = 0; i < data->iterations; ++i) {
        double randNum = dis(gen);
        double functionVal = sinx(randNum);
        totalSum += functionVal;
    }

    data->result = (data->upperBound - data->lowerBound) * totalSum / data->iterations;
    pthread_exit(NULL);
}

double parallelMonteCarloEstimate(double lowBound, double upBound, int iterations, int numThreads) {
    vector<pthread_t> threads(numThreads);
    vector<ThreadData> threadData(numThreads);
    double totalResult = 0.0;
    double range = (upBound - lowBound) / numThreads;

    for (int i = 0; i < numThreads; ++i) {
        threadData[i].lowerBound = lowBound + i * range;
        threadData[i].upperBound = (i == numThreads - 1) ? upBound : lowBound + (i + 1) * range;
        threadData[i].iterations = iterations / numThreads;

        if(pthread_create(&threads[i], NULL, monteCarloEstimate, (void*)&threadData[i])) {
            cerr << "Error creating thread" << endl;
            return 0.0;
        }
    }

    for (int i = 0; i < numThreads; ++i) {
        pthread_join(threads[i], NULL);
        totalResult += threadData[i].result;
    }
    return totalResult;
}

int main(int argc, char* argv[]) {
    if(argc != 5){
        cerr << "Invalid number of inputs" << endl;
    }
    double lower = stod(argv[1]);
    double upper = stod(argv[2]);
    double iterations = stod(argv[3]);
    int num_threads = stoi(argv[4]);

    auto start = std::chrono::steady_clock::now();
    double estimate = parallelMonteCarloEstimate(lower, upper, iterations, num_threads);
    cout << estimate << endl;
    auto stop = chrono::steady_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(stop - start);
    //std::cout << duration.count() << std::endl;
    return 0;
}
