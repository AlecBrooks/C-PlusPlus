#include <iostream>
#include <string>
#include <random>
#include <vector>
#include <iomanip>

using namespace std;

#define RESET   "\033[0m"
#define RED     "\033[31m" // bad values
#define GREEN   "\033[32m" // good values
#define YELLOW  "\033[33m" // enemy creatures
#define BLUE    "\033[34m" // friendly creatures
#define MAGENTA "\033[35m" // items
#define CYAN    "\033[36m" // weapons

//startGlobal RNG engine
random_device rd;
mt19937 rng(rd());

//prototypes
class Hero;
class Item;
class Monster;
class Room;
int runRNG(int min, int max);
void clearScreen();
void runRoom();
void CombatHeader(Hero&);
void Header(Hero&);
void runCombat(Hero&, Monster&);
void createObjects();
void useItemLogic(Hero&, Item&);


//Classes====================================

//create items (Class)
class Item {
	public:
    	string name;
    	string desc;
    	bool singleUse;
    	bool canCarry;
    Item(string name, string desc, bool singleUse, bool canCarry)
        : name(name), desc(desc), singleUse(singleUse), canCarry(canCarry){}
	void useMe(Hero& player) {
		useItemLogic(player, *this);
	};
    void useOn(Item& Target){
    };
};

//create weapon (Class)
class Weapon{
	public:
		string name;
		int minDmg;
		int maxDmg;
	Weapon(string name, int minDmg, int maxDmg)
		: name(name), minDmg(minDmg), maxDmg(maxDmg) {}
	int swing(int bonus) {
		int dmg = bonus + runRNG(minDmg,maxDmg);
		cout << CYAN << name << RESET << " dealing " << RED << dmg << RESET << " damage!" << endl;
		return dmg;
	};
};

//create player (Class)
class Hero{
	public:
	string name;
	int maxHP;
	int hp;
	int st;
	int dex;
	int armorClass;
	Weapon* weapon;
	vector<Item> inv;
	Room* location;
	Hero(string name, int maxHP, int hp, int st, int dex, Weapon* weapon, Room* location)
    : name(name), maxHP(maxHP), hp(hp), st(st), dex(dex), armorClass(8 + dex), weapon(weapon), location(location){}
	int attackRoll(int targetAC){
		int roll = runRNG(1,20);
		int bonus = dex;
		int attackTotal = roll + bonus;

		if(attackTotal >= targetAC){
        	cout << "You hit with your ";
        	return weapon->swing(st);
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
	int heal(int healAmount){
		hp += healAmount;
		if (hp > maxHP) hp = maxHP;
		return hp;
	};
	void showStats(){
		cout << "Name: " << CYAN << name << RESET << endl;
		cout << "Health: " << GREEN << hp << RESET << endl;

		if (weapon != nullptr) {
			cout << "Weapon: " << weapon->name << endl;
		} else {
			cout << "Weapon: (none)" << endl;
		};
		cout << YELLOW <<"Inventory:" << endl;
		if(inv.empty()){
			cout << "(empty)" << RESET << endl; 
		}else{
			for (int i = 0; i < inv.size(); ++i) {
				cout << " - " << inv[i].name << endl;
			};
			cout << RESET;
		};
	};
	void addItem(Item item) {
		if(item.canCarry == true){
    		inv.push_back(item);
    		cout << name << " picked up a: " << CYAN << item.name << RESET << endl;
    	} else {
    		cout << "You're kidding right?" << endl;
    	};
	};
	void equipWeapon(Weapon* newWeapon) {
    	weapon = newWeapon;
    	cout << name << " equipped a: " << CYAN << newWeapon->name << RESET << endl;
	};
	void examine(string itemName) {
    	for (int i = 0; i < inv.size(); ++i) {
        	if (inv[i].name == itemName) {
            	cout << CYAN << inv[i].name << RESET << ": " << inv[i].desc << endl;
            	return;
        	};
    	};
    	cout << CYAN << itemName << RESET << " you dont have that." << endl;
	};
	void useItem(string itemName) {
		for (int i = 0; i < inv.size(); ++i) {
			if (inv[i].name == itemName) {
				inv[i].useMe(*this);
				if (inv[i].singleUse) {
					inv.erase(inv.begin() + i);
				};
			return;
			};
		};
		cout << CYAN << itemName << RESET << " is not in your inventory!" << endl;
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
	string deathDesc;
	Monster(string name, int hp, int st, int dex, Weapon* weapon, string desc, string deathDesc)
		: name(name), hp(hp), st(st), dex(dex), armorClass(8+dex), weapon(weapon), desc(desc), deathDesc(deathDesc){}
	int attackRoll(int targetAC){
		int roll = runRNG(1,20);
		int bonus = dex;
		int attackTotal = roll + bonus;
		if(attackTotal >= targetAC){
        	cout << "The " << YELLOW << name << RESET << " hit with it's ";
        	return weapon->swing(st);
		}else{
        	cout << "The " << YELLOW << name << RESET << " Missed!" << endl;
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
	void showStats(){
		cout << "Name: " << YELLOW << name << RESET << endl;
		cout << "Health: " << GREEN << hp << RESET << endl;

		if (weapon != nullptr) {
			cout << "Weapon: " << weapon->name << endl;
		} else {
			cout << "Weapon: (none)" << endl;
		};
	};
};

class Door{
	public:
	string name;
	bool locked;
	bool visable;
	vector<Room>connection;
	Door(string name, bool locked, bool visable)
	: name(name), locked(locked), visable(visable) {}
};

//create rooms (Class)
class Room{
	public:
	string name;
	vector<Item>Contents;
	vector<Door>Doors;
	vector<Monster>enemys;

	Room(string name)
		: name(name) {}
};

//main loop and init
int main(){
	//Create Objects
	//rooms
	Room testRoom("testRoom");
	//items
	Item hpot("health potion", "Can be used to heal 5 - 10 health.",true,true);
	Item ppot("poison potion", "Ingestion will cause 5 - 10 damage.",true,true);
	Item map("map", "Map of the Area",false,true);
	Item BoS("boulder of sisyphus", "A fancy way of saying a big ass rock.",false,false);
	//weapons
	Weapon battleaxe("Battleaxe",1,8);
	Weapon mace("Mace",1,6);
	Weapon dagger("Dagger",1,4);
	Weapon longsword("Longsword",1,10);
	Weapon scimitar("Scimitar",1,6);
	//monsters
	Monster goblin("Goblin", 7, 0, 2, &scimitar, "A small, green creature with bloodstained claws and a cruel grin.", "The goblin screeches as your blow lands true. Blood sprays from its mouth as it stumbles backward, eyes wide in shock. With a final gasp, it crumples to the ground, twitching once before going still.");
	Monster skeleton("Skeleton", 13, 0, 3, &mace, "A clattering mass of old bones bound by dark magic. Its hollow eye sockets glow faintly with unnatural light, and it grips a rusted mace with unsettling steadiness.", "The final blow shatters the skeleton’s spine. Bones fly in all directions as the magical force holding it together vanishes. With a final clatter, the skull rolls to a stop, grinning forever in death.");
	//player
	Hero player("Aragorn", 30, 30, 2, 2, &longsword, &testRoom);
	player.addItem(hpot);
	clearScreen();
	runCombat(player,goblin);
	clearScreen();
	runCombat(player,skeleton);
	return 0;
};

int runRNG(int min, int max){
	uniform_int_distribution<int> dist(min, max);
    return dist(rng);
};
void runRoom(){

};
void clearScreen(){
	cout << "\033[2J\033[1;1H"; 
};
void Header(Hero& player, Room& ThisRoom) {
    cout << "===== Round " << ThisRoom.name << " =====" << endl << endl;
    cout << BLUE << "-- Player --" << RESET << endl;
    player.showStats();
    cout << "=====================" << endl << endl;
};
void CombatHeader(Hero& player, Monster& enemy, int combatRound) {
    cout << "===== Round " << combatRound << " =====" << endl << endl;
    cout << BLUE << "-- Player --" << RESET << endl;
    player.showStats();
    cout << endl << RED << "-- Enemy --" << RESET << endl;
    enemy.showStats();
    cout << endl << "What will you do?" << endl;
    cout << "> attack | flee | use [item name]" << endl << endl;
    cout << "=====================" << endl << endl;
};
void runCombat(Hero& player, Monster& enemy){
	int combatRound = 1;
	while (player.isAlive() && enemy.isAlive()){
		CombatHeader(player, enemy, combatRound);
		string action;
		getline(cin,action);
		cout << endl;
		if (action == "attack") {
    		int playerDmg = player.attackRoll(enemy.armorClass);
    		if (playerDmg > 0) {
        		enemy.takeDamage(playerDmg);
    		};
    		int enemyDmg = enemy.attackRoll(player.armorClass);
    		if (enemyDmg > 0) {
        		player.takeDamage(enemyDmg);
    		};
    		combatRound += 1;
		} else if (action == "flee") {
    		cout << "You turn to flee giving the " << YELLOW << enemy.name << RESET << " an opertunity to attack! " << endl;
    		int enemyDmg = enemy.attackRoll(player.armorClass);
    		if (enemyDmg > 0) {
        		player.takeDamage(enemyDmg);
    		};
    		break;
    	}else if (action.find("use ") == 0){
    		string itemName = action.substr(4);
    		player.useItem(itemName);
		} else {
    		cout << "no acction for this command." << endl;
		};
		cin.get();
		clearScreen();				
	};
	if(!enemy.isAlive()){
		CombatHeader(player, enemy, combatRound);
		cout << enemy.deathDesc << endl;
	}else if(!player.isAlive()){
		CombatHeader(player, enemy, combatRound);
		cout << "you lose!";
	}else{

	};
	cin.get();
};
void useItemLogic(Hero& player, Item& item){

	if (item.name == "health potion") {
    	int healAmount = runRNG(5, 10); 
        cout << "You use the " << MAGENTA << item.name << RESET
        << " and restore " << GREEN << healAmount << RESET << " health." << endl;
        player.heal(healAmount);
    };
    if(item.name == "poison potion") {
    	int damage = runRNG(5,10);
    	cout << "You use the " << MAGENTA << item.name << RESET
        << " and suffer " << GREEN << damage << RESET << " damage." << endl;
        player.takeDamage(damage);
    };
    if(item.name == "map"){
    	cout << "You look at the " << MAGENTA << item.name << RESET << ", nice pictures." << endl;
    };
};




