#include <iostream>
#include <string>
#include <random>
#include <vector>

using namespace std;

//startGlobal RNG engine
random_device rd;
mt19937 rng(rd());

//prototypes
void runRoom();
void runCombat();
int runRNG(int min, int max);

//Classes

//create items (Class)
class Item {
	public:
    	string name;
    	string desc;
    Item(string name, string desc)
        : name(name), desc(desc) {}
};

//create weapon (Class)
class Weapon{
	public:
		string name;
		int minDmg;
		int maxDmg;

	Weapon(string name, int minDmg, int maxDmg, string desc)
		: name(name), minDmg(minDmg), maxDmg(maxDmg) {}

	int swing() {
		int dmg = runRNG(minDmg,maxDmg);
		cout << name << " deals " << dmg << " damage!" << endl;
		return dmg;
	};
};

//create player (Class)
class Hero{
	public:
	string name;
	int hp;
	int st;
	int dex;
	int armorClass;
	Weapon* weapon;
	vector<Item> inv;

	Hero(string name, int hp, int st, int dex, Weapon* weapon)
		: name(name), hp(hp), st(st), dex(dex), armorClass(10+dex){}

	int attackRoll(int targetAC){

		int roll = runRNG(1,20);
		int bonus = st;
		int attackTotal = roll + bonus;

		if(attackTotal >= targetAC){
        	cout << "You hit!" << endl;
        	return weapon->swing();
		}else{
        	cout << "You missed!" << endl;
        	return 0; 
		};
	};

	int takeDamage(int owie){
		hp -= owie;
		if (hp < 0) hp = 0;
		return hp;
	};

	void showStats(){
		cout << "Name: " << name << endl;
		cout << "Health: " << hp << endl;
		cout << "Weapon: " << weapon -> name << endl;
		cout << "Inventory:" << endl;
		if(inv.empty()){
			cout << "(empty)" << endl; 
		}else{
			for (int i = 0; i < inv.size(); ++i) {
				cout << " - " << inv[i].name << endl;
			}
		}
	};

	void addItem(Item item) {
    	inv.push_back(item);
    	cout << name << " picked up a: " << item.name << endl;
	};


	void equipWeapon(Weapon* newWeapon) {
    	weapon = newWeapon;
    	cout << name << " equipped a: " << newWeapon->name << endl;
	};


	void examine(string itemName) {
    	for (int i = 0; i < inv.size(); ++i) {
        	if (inv[i].name == itemName) {
            	cout << inv[i].name << ": " << inv[i].desc << endl;
            	return;
        	}
    	}
    	cout << itemName << " you dont have that." << endl;
	};

	void useItem(Item item){
	
	};

	bool isAlive() {
		return hp > 0;
	};

};
	
	//create Monsters (Class)
class Monster{
	public:
	string name;
	int hp;
	int st;
	int dex;
	int armorClass;
	Weapon* weapon;
	string desc;

	Monster(string name, int hp, int st, int dex, Weapon* weapon, string desc)
		: name(name), hp(hp), st(st), dex(dex), armorClass(10+dex), desc(desc){}

	int attackRoll(int targetAC){

		int roll = runRNG(1,20);
		int bonus = st;
		int attackTotal = roll + bonus;

		if(attackTotal >= targetAC){
        	cout << "the " << name << " hit!" << endl;
        	return weapon->swing();
		}else{
        	cout << "the " << name << " Missed!" << endl;
        	return 0; 
		};
	};

	bool isAlive() {
		return hp > 0;
	};

	int takeDamage(int owie){
		hp -= owie;
		if (hp < 0) hp = 0;
		return hp;
	};

};

	//create rooms (Class)


//main loop and init
int main(){

	bool gameOver = false;
	int gameOverType = 0; 

	while(gameOver == false){
		//game Loop	

	}
	if(gameOverType == 0){
		//player loss to death
	}else{
		//player win 
	} 

	return 0;
}

int runRNG(int min, int max){
	uniform_int_distribution<int> dist(min, max);
    return dist(rng);
};

void runRoom(){

};

void runCombat(){

};