#include <iostream>
#include <string>

using namespace std;

int main(){

    string name;

    cout << "Please type your name: ";
    getline(cin, name);
    cout << endl << "your name is " << name << "!";

    cin.ignore();
    //cin.get();
    
    return 0;
}