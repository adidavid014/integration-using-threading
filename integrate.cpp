#include <iostream>
#include <mutex>
#include <thread>
using namespace std;

int main(int argc, char *argv[]){
    if(argc != 5){
        cout << "Incorrect number of arguments";
    }
    double a = stod(argv[1]);
    double b = stod(argv[2]);
    int n = stoi(argv[3]);
    int n_threads = stoi(argv[4]]);

}