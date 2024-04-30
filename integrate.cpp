#include <iostream>
#include <cmath>
#include <pthread.h>
#include <vector>

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
    for (int i = 0; i < data->iterations; ++i) {
        double randNum = data->lowerBound + (static_cast<double>(rand()) / RAND_MAX) * (data->upperBound - data->lowerBound);
        double functionVal = sinx(randNum);
        totalSum += functionVal;
    }

    data->result = (data->upperBound - data->lowerBound) * totalSum / data->iterations;
    pthread_exit(NULL);
}


double parallelMonteCarloEstimate(double lowBound, double upBound, int iterations, int numThreads) {
    std::vector<pthread_t> threads(numThreads);
    std::vector<ThreadData> threadData(numThreads);

    double totalResult = 0.0;
    double range = (upBound - lowBound) / numThreads;

    for (int i = 0; i < numThreads; ++i) {
        threadData[i].lowerBound = lowBound + i * range;
        threadData[i].upperBound = (i == numThreads - 1) ? upBound : lowBound + (i + 1) * range;
        threadData[i].iterations = iterations / numThreads;

        if(pthread_create(&threads[i], NULL, monteCarloEstimate, (void*)&threadData[i])) {
            std::cerr << "Error creating thread" << std::endl;
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
    double lower = std::stod(argv[1]);
    double upper = std::stod(argv[2]);
    double iterations = std::stod(argv[3]);
    int num_threads = std::stoi(argv[4]);
    
    double estimate = parallelMonteCarloEstimate(lower, upper, iterations, num_threads);
    std::cout << "Estimate for " << lower << " -> " << upper << " is " << estimate << " (" << iterations << " iterations)" << std::endl;
    return 0;
}