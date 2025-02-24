#include <iostream>
#include <random>
using namespace std;

int main(){

    int seed = 123;
    cout << "======================" << endl << "enter numbers for seed: ";
    cin >> seed;

    int min = 0;
    cout << endl << "enter min: ";
    cin >> min;

    int max = 99;
    cout << endl << "enter max: ";
    cin >> max;

    mt19937 num(seed);
    uniform_int_distribution<int> distribution(min,max);

    int myRand = distribution(num);

    cout << endl << "======================" << endl << "Your random number: " << myRand << endl
    << "======================" << endl
    << "Seed: " << seed << endl
    << "Min: " << min << endl
    << "Max: " << max << endl;

    return 0;

}