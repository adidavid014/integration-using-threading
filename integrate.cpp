#include <iostream>
#include <mutex>
#include <thread>
#include <vector>
#include <functional>
using namespace std;


double sinx(double x){
    if(x == 0.0){
        return 1.0;
    }
    return sin(x)/x;
}

double trapRule(function<double(double)>& f, double a, double b, int n){
    double width = (b-a)/n;
    double sum = 0.5 * (f(a) + f(b));
    for(int i = 1; i < n; i++){
        sum += f(a + i * h);
    }
    return sum*h;
}

int main(int argc, char *argv[]){
    if(argc != 5){
        cout << "Incorrect number of arguments";
    }
    double a = stod(argv[1]);
    double b = stod(argv[2]);
    int n = stoi(argv[3]);
    int n_threads = stoi(argv[4]);

    double result = 0.0;

    vector<thread> threads; 

    for(int i = 0; i < n_threads; i++){
        threads.push_back(thread(worker, sinx, a, b, n, n_threads, i, ref(result)))
    }

}