#include <iostream>
#include <string>
#include <random>

using namespace std;

//startGlobal RNG engine
random_device rd;
mt19937 rng(rd());

void runRoom();
int runRNG(int max);

int main(){
	
	//create weapons (Class)

	//create player (Class)

	//create Monsters (Class)

	//create rooms (Class)

	bool gameOver = false;
	int gameOverType = 0; 

	while(gameOver == false){
		//game Loop	
		runRoom();
	}
	if(gameOverType == 0){
		//play loss to death
	}else{
		//player win 
	} 

	return 0;
}

void runRoom(){


};

int runCombat(){

return 0;
};

int runRNG(int max){
	uniform_int_distribution<int> dist(0, max);
    return dist(rng);
};