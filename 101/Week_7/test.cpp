#include <iostream>

using namespace std;

int addTwo(int,int);


int main(){

	int x;
	int y;

	cout << "Enter an Int: ";
	cin >> x;
	cout << endl << "Enter another int: ";
	cin >> y;
	cout << endl << x << " + " << y << " = " << addTwo(x,y) << endl;
	return 0;

}

int addTwo(int x,int y){

	return x + y;

}
