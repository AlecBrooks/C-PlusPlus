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
class NPC;
int runRNG(int min, int max);
void clearScreen();
void runRoom(Room*, Hero&);
void CombatHeader(Hero&);
void Header(Hero&, Room*);
int runCombat(Hero&, Monster&);
void createObjects();
void useItemLogic(Hero&, Item&);
void runDialogue(NPC& npc, Hero& player, Room* room);
void useRoomItem(Item& item);
void doors(Hero& player, Room* room, string targetRoom);
void endGame(int type);
void startGame();

enum MainMenu : int {
		NewGame = 1,
		Info = 2
	};

void Enter2Continue(string text, string color){
    cout << color << text << RESET;
	cin.get();
	clearScreen();
};

void printPrologue(string name){
	clearScreen();
    cout << "A " << MAGENTA << "letter " << RESET << "bearing the sigil of " << CYAN << "Tarlava" << RESET << ", your hometown arrives, bringing word from home after your long absence \nadventuring." << endl << endl;
    Enter2Continue("Press Enter to Continue.", CYAN);

	cout << name << " My Esteemed Friend and Champion," << endl << endl;
	cout << "I write to you from the Hall of the Silver Oak here in Tarlava, the very\nstreets where you first took your hesitant steps as a child. The people\nspeak of you in hushed, reverent tones, for your deeds have traveled far\nbeyond our moss-grown walls: your valor at the Black Marsh, even the sparring\nyou bested in the Frostspire. These tales are now as much a part of our village\nlore as the old oak itself.\n\nYet it is not pride alone that compels me to set quill to parchment. A\nshadow has fallen upon Tarlava. At dawn two nights past, Priestess Amara,\nwhose kindness and wisdom have guided our harvests and blessed our\nhearths, vanished from the Temple of the Gilded Flame. No footfall nor\nwhisper remains to mark her departure, but signs point to more than mere\nfoul play. As you well know, our village rests upon the long-sealed entrance\nto an ancient tomb: the Sanctum of the Ember Eye. Rumors stir that a\ncreature, stoked by magics older than memory, has broken free and carried\nher into that hallowed gloom.\n\nI beseech you, bold adventurer: return to Tarlava and journey into the Ember\nEye. Rescue Amara from whatever dread fate awaits her in those vaulted\nhalls. Should you prevail, know that your name will forever shine in our\nsongs, and I, Harlan Merrow, Mayor of Tarlava, will count you among the\ngreatest children this humble town has ever produced.\n" << endl;
	cout << "In hopeful anticipation," << endl << endl;
	cout << "Harlan Merrow, Mayor of Tarlava" << endl << endl;
    Enter2Continue("Press Enter to Continue.", CYAN);
    clearScreen();
    cout << "You journey back to your home village of Tarlava and press on to the ancient tomb's entrance. There,\nthe once sealed stone door now stands open, revealing a dark passage leading down into the crypt\nbelow.\n\n";
    Enter2Continue("Press Enter to Descend into the Darkness.", RED);
};


void printMainMenu(){
	clearScreen();
		      cout << YELLOW << R"(                   
========================================================================================================================
 __                  _                            __   _   _              __          _                   __           
/ _\ __ _ _ __   ___| |_ _   _ _ __ ___     ___  / _| | |_| |__   ___    /__\ __ ___ | |__   ___ _ __    /__\   _  ___ 
\ \ / _` | '_ \ / __| __| | | | '_ ` _ \   / _ \| |_  | __| '_ \ / _ \  /_\| '_ ` _ \| '_ \ / _ \ '__|  /_\| | | |/ _ \
_\ \ (_| | | | | (__| |_| |_| | | | | | | | (_) |  _| | |_| | | |  __/ //__| | | | | | |_) |  __/ |    //__| |_| |  __/
\__/\__,_|_| |_|\___|\__|\__,_|_| |_| |_|  \___/|_|    \__|_| |_|\___| \__/|_| |_| |_|_.__/ \___|_|    \__/ \__, |\___|
                                                                                                            |___/         
========================================================================================================================   
    )" << RESET << endl << endl;
    cout << "1) New Game" << endl;
    cout << "2) Exit" << endl;
};

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
	Room* lastLocation;

	Hero(string name, int maxHP, int hp, int st, int dex, Weapon* weapon, Room* location)
    	: name(name), maxHP(maxHP), hp(hp), st(st), dex(dex), armorClass(8 + dex), weapon(weapon), location(location) {}
	
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
    		cout << CYAN << name << RESET << " picked up a: " << MAGENTA << item.name << RESET << endl;
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
				useItemLogic(*this, inv[i]);
			return;
			};
		};
		cout << CYAN << itemName << RESET << " is not in your inventory!" << endl;
	};
	bool isAlive() {
		return hp > 0;
	};
	void move(Room* moveTo){
		this->lastLocation = this->location;
		this->location = moveTo;
	}
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

class NPC{
public:
	string name;
	string desc;
	NPC(string name, string desc)
		: name(name), desc(desc) {}
	void dialog(Hero& player, Item& item, Room* room){ 
		bool hasItem = false;
		for (int i = 0; i < player.inv.size(); ++i) {
			if (player.inv[i].name == item.name) {
				hasItem = true;
			};
		};
		if (hasItem == true) {
			cout << "Please hurry "<< CYAN << player.name << RESET << "!" << endl << endl;
		}else{
			cout << CYAN << player.name << "!\n\n" << RESET << "By the Silver Oak's grace! " << CYAN << player.name << RESET << " where have you come from?! There's sadly little time\nfor pleasantries. Tarlava teeters on the edge of annihilation: the dread lich " << YELLOW << "Zarghoul " << RESET << "has\nrisen again, his power swollen by ancient sorceries. I cannot fathom how he escaped his crypt,\nbut his unholy might now threatens us all. You must venture into his tomb and shatter his dark\nreign once and for all. I would march at your side, but fear I'd only slow you down.\nTake this vial of consecrated water its sanctified essence may turn the tide. In our most desperate hour,\nTarlava's hope lies with you. I will follow the path you've already trod and await your return at the\ntown gates. My life, my thanks, and my debt are yours, Hero of Tarlava.\n\n";
			player.addItem(item);
		};
	};
};

class Door {
public:
	string name;
	bool locked;
	bool visible;
	int ID;
	Room* connectsTo;
	Item* key;
	Door() : name(""), locked(false), visible(true), ID(999), connectsTo(nullptr), key(nullptr){}
	Door(string name, bool locked, bool visible, int ID, Room* connectsTo = nullptr, Item* key = nullptr)
		: name(name), locked(locked), visible(visible), connectsTo(connectsTo), key(key) {}
};

//create rooms (Class)
class Room {
	public:
	string name;
	string desc;
	vector<Item> Contents;
	vector<Monster> enemys;
	vector<NPC> NPCs;

	Door north, south, east, west;

	Room(string name, string desc)
		: name(name), desc(desc) {}

	void addContents(Item item) {
		Contents.push_back(item);
	};
	void addNPC(NPC npc) {
		NPCs.push_back(npc);
	};
	void addMonster(Monster enemy) {
		enemys.push_back(enemy);
	};
	void removeContents(Item item) {
    	for (vector<Item>::iterator it = Contents.begin(); it != Contents.end(); ++it) {
        	if (it->name == item.name) {
            	Contents.erase(it);
            	break;
        	};
    	};
	};
	void removeNPC(NPC npc) {
    	for (vector<NPC>::iterator it = NPCs.begin(); it != NPCs.end(); ++it) {
        	if (it->name == npc.name) {
            	NPCs.erase(it);
            	break;
        	};
    	};
	};
};
int main(){
    while (true) {
        int choice = 0;
        while (!(choice == 1 || choice == 2)) {
            clearScreen();
            printMainMenu();
            cout << "\n\nEnter your choice: ";
            if (!(cin >> choice)) {
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                choice = 0;
            }
            else if (choice < 1 || choice > 2) {
                cout << "\nInvalid choice, try again.\n\n";
            }
        };

        switch (choice) {
            case 1: {
                startGame();
                break;
            };
            case 2: {
                cout << "Goodbye!\n";
                Enter2Continue("Press Enter to Exit.", CYAN);
                return 0;
            };
        };
    };
    return 0;
};

int runRNG(int min, int max){
	uniform_int_distribution<int> dist(min, max);
    return dist(rng);
};

void runRoom(Room* here, Hero& player){
	clearScreen();
	Header(player, here);
	cout << "you enter the room and see ";
	cout << here->desc << endl << endl;
	cout << "within the room you see:" << endl;
	if (!here->enemys.empty()) {
    	cout << endl << endl << YELLOW << here->enemys[0].name << RESET << ": " << here->enemys[0].desc << endl << endl;
		Enter2Continue("Press Enter to begin combat.", RED);
		int result = runCombat(player, here->enemys[0]);
		if (result == 0) {
		if(here->enemys[0].name == "zarghoul"){
			player.hp = 0;
			endGame(1);
		}
        here->enemys.clear();
    	return;
    	}
    	else if (result == 1) {
        	player.move(player.lastLocation);
        	return;
    	}
    	else if (result == 2) {
        	endGame(0);
        	return;
    	};
	};
	if (!here->Contents.empty()) {
		cout << endl;
    	size_t n = here->Contents.size();
    	if (n == 1) {
        	cout << MAGENTA << here->Contents[0].name << RESET << ".";
    	}
    else {
        for (size_t i = 0; i < n; ++i) {
        	cout << MAGENTA << here->Contents[i].name << RESET;
        	if (i < n - 2) {
                cout << ", ";
            }
            else if (i == n - 2) {
                cout << " and ";
            }
            else {
                cout << ".";
            	};
        	};
    	};
	};
	if (!here->NPCs.empty()) {
		cout << endl;
    	size_t n = here->NPCs.size();
    	if (n == 1) {
        	cout << CYAN << here->NPCs[0].name << RESET << ".";
    	}
    else {
        for (size_t i = 0; i < n; ++i) {
        	cout << CYAN << here->NPCs[i].name << RESET;
        	if (i < n - 2) {
                cout << ", ";
            }
            else if (i == n - 2) {
                cout << " and ";
            }
            else {
                cout << ".";
            	};
        	};
    	};
	};
	cout << endl;
	if(!(here->north.connectsTo == nullptr)){
		cout << endl << "along the " << MAGENTA << "north" << RESET << " wall you see "<< here->north.name;
	};
	if(!(here->south.connectsTo == nullptr)){
		cout << endl << "along the " << MAGENTA << "south" << RESET << " wall you see "<< here->south.name;
	};
	if(!(here->east.connectsTo == nullptr)){
		cout << endl << "along the " << MAGENTA << "east" << RESET << " wall you see "<< here->east.name;
	};
	if(!(here->west.connectsTo == nullptr)){
		cout << endl << "along the " << MAGENTA << "west" << RESET << " wall you see "<< here->west.name;
	};
	cout << endl << endl;
	cout << endl << "What would you like to do?" << endl << endl;
    cout << "> go [direction] | look [item] | take [item] | use [item name] | talk to [name]" << endl << endl;
    cout << "========================================================================================" << endl << endl;
	string action;
	getline(cin,action);

	if (action.find("look ") == 0) {  //Look at an item
    	string itemName = action.substr(5);
    	bool found = false;
    	for (int i = 0; i < here->Contents.size(); ++i) {
        	if (here->Contents[i].name == itemName) {
            	cout << endl << here->Contents[i].desc<< endl << endl;
            	found = true;
       		};
    	};
    	if(found == false){
    		cout << endl << "You don't see a '" << itemName << "' here." << endl << endl;
    	};
	}else if(action.find("go ") == 0){  //move to a direction
		string targetRoom = action.substr(3);
		doors(player, here, targetRoom);
	}else if(action.find("use ") == 0){
		string itemName = action.substr(4);
		player.useItem(itemName);
    }else if(action.find("talk to ") ==0){
    	string itemName = action.substr(8);
    	for (int i = 0; i < here->NPCs.size(); ++i) {
        	if (here->NPCs[i].name == itemName) {
        		runDialogue(here->NPCs[i], player, here);
       		}else{
       			cout << "who?" << endl;
       		}
    	};
    }else if(action.find("take ") == 0){
    	string itemName = action.substr(5);
    	bool found = false;
    	for (int i = 0; i < here->Contents.size(); ++i) {
        	if (here->Contents[i].name == itemName) {
            	found = true;
            	if(here->Contents[i].canCarry == true){
            		cout << endl;
            		player.addItem(here->Contents[i]);
            		here->removeContents(here->Contents[i]);
            		cout << endl;
            	}else{
            		cout << endl << endl << "are you out of your mind?" << endl;
            	};
       		};
    	};
    	if(found == false){
    		cout << endl << "You don't see a '" << itemName << "' here." << endl << endl;
    	};
    }else{
		cout << endl << "huh?" << endl << endl;
	};
	Enter2Continue("Press Enter to Continue.", CYAN);
};

void clearScreen(){
	cout << "\033[2J\033[1;1H"; 
};
void Header(Hero& player, Room* ThisRoom) {
    cout << "===================================== " << "Player Stats" << " ===================================" << endl << endl;
    cout << BLUE << "-- Player --" << RESET << endl;
    player.showStats();
    cout << endl << "======================================= " << CYAN << ThisRoom->name << RESET <<" ========================================" << endl << endl;
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
int runCombat(Hero& player, Monster& enemy) {
    int combatRound = 1;
    while (player.isAlive() && enemy.isAlive()) {
        CombatHeader(player, enemy, combatRound);
        string action;
        getline(cin, action);
        cout << endl;

        if (action == "attack") {
            int playerDmg = player.attackRoll(enemy.armorClass);
            if (playerDmg > 0) enemy.takeDamage(playerDmg);
            if (!enemy.isAlive()) {
                CombatHeader(player, enemy, combatRound);
                cout << enemy.deathDesc << "\n\n";
                Enter2Continue("Press Enter to Continue.", GREEN);
                return 0;
            }
            int enemyDmg = enemy.attackRoll(player.armorClass);
            if (enemyDmg > 0) player.takeDamage(enemyDmg);
            if (!player.isAlive()) {
                CombatHeader(player, enemy, combatRound);
                cout << "you lose!\n\n";
                Enter2Continue("Press Enter to Continue.", RED);
                return 2;
            }
            combatRound++;
        }
        else if (action == "flee") {
            cout << "You turn to flee giving the " << YELLOW << enemy.name << RESET << " an opportunity to attack!\n";
            int enemyDmg = enemy.attackRoll(player.armorClass);
            if (enemyDmg > 0) {
                player.takeDamage(enemyDmg);
                cout << endl;
                Enter2Continue("Press Enter to Continue.", YELLOW);
                return 1;
            }
        }
        else if (action.rfind("use ", 0) == 0) {
            player.useItem(action.substr(4));
        }
        else {
            cout << "no action for this command.\n";
        }

        Enter2Continue("Press Enter to Continue.", YELLOW);
        clearScreen();
    }

    if (!enemy.isAlive()) return 0;
    if (!player.isAlive()) return 2;
    return 1;
}

void runDialogue(NPC& npc, Hero& player, Room* room){
	clearScreen();
	cout << "===================================== " << npc.name << " ===================================" << endl << endl;

	Item holyw("holy water", "A vial of holy water used to purify unholy things.", true, true);
	npc.dialog(player,holyw, room);

	cout << "============================================================================" << endl << endl;

}

void doors(Hero& player, Room* room, string targetRoom){
    if (targetRoom == "north") {
        if (room->north.connectsTo == nullptr) {
            cout << "\nYou bonk your head against the wall.\n\n";
        } else if (room->north.locked) {
            if (room->north.key != nullptr) {
                bool found = false;
                for (int i = 0; i < player.inv.size(); ++i) {
                    if (player.inv[i].name == room->north.key->name) {
                        found = true;
                        break;
                    };
                };
                if (found) {
                    room->north.locked = false;
                    if (room->north.connectsTo != nullptr) {
   						 room->north.connectsTo->south.locked = false;
					};
                    cout << "You unlock the north door using the key.\n\n";
                    player.move(room->north.connectsTo);
                } else {
                    cout << "\nThe door appears to be locked.\n\n";
                };
            } else {
                cout << "\nThis door is sealed by ancient magic no key will open it.\n\n";
            };
        } else {
            player.move(room->north.connectsTo);
            cout << "\nYou move through the north door.\n\n";
        };
    } else if (targetRoom == "south") {
        if (room->south.connectsTo == nullptr) {
            cout << "\nYou bonk your head against the wall.\n\n";
        } else if (room->south.locked) {
            if (room->south.key != nullptr) {
                bool found = false;
                for (int i = 0; i < player.inv.size(); ++i) {
                    if (player.inv[i].name == room->south.key->name) {
                        found = true;
                        break;
                    };
                };
                if (found) {
                    room->south.locked = false;
                    if (room->south.connectsTo != nullptr) {
   						 room->south.connectsTo->north.locked = false;
					};
                    cout << "You unlock the south door using the key.\n\n";
                    player.move(room->south.connectsTo);
                } else {
                    cout << "\nThe door appears to be locked.\n\n";
                };
            } else {
                cout << "\nThis door is sealed by ancient magic no key will open it.\n\n";
            };
        } else {
            player.move(room->south.connectsTo);
            cout << "\nYou move through the south door.\n\n";
        };
    } else if (targetRoom == "east") {
        if (room->east.connectsTo == nullptr) {
            cout << "\nYou bonk your head against the wall.\n\n";
        } else if (room->east.locked) {
            if (room->east.key != nullptr) {
                bool found = false;
                for (int i = 0; i < player.inv.size(); ++i) {
                    if (player.inv[i].name == room->east.key->name) {
                        found = true;
                        break;
                    };
                };
                if (found) {
                    room->east.locked = false;
                    if (room->east.connectsTo != nullptr) {
   						 room->east.connectsTo->west.locked = false;
					};
                    cout << "You unlock the east door using the key.\n\n";
                    player.move(room->east.connectsTo);
                } else {
                    cout << "\nThe door appears to be locked.\n\n";
                };
            } else {
                cout << "\nThis door is sealed by ancient magic no key will open it.\n\n";
            };
        } else {
            player.move(room->east.connectsTo);
            cout << "\nYou move through the east door.\n\n";
        };
    } else if (targetRoom == "west") {
        if (room->west.connectsTo == nullptr) {
            cout << "\nYou bonk your head against the wall.\n\n";
        } else if (room->west.locked) {
            if (room->west.key != nullptr) {
                bool found = false;
                for (int i = 0; i < player.inv.size(); ++i) {
                    if (player.inv[i].name == room->west.key->name) {
                        found = true;
                        break;
                    };
                };
                if (found) {
                    room->west.locked = false;
                    if (room->west.connectsTo != nullptr) {
   						 room->west.connectsTo->east.locked = false;
					};
                    cout << "You unlock the west door using the key.\n\n";
                    player.move(room->west.connectsTo);
                } else {
                    cout << "\nThe door appears to be locked.\n\n";
                };
            } else {
                cout << "\nThis door is sealed by ancient magic no key will open it.\n\n";
            };
        } else {
            player.move(room->west.connectsTo);
            cout << "\nYou move through the west door.\n\n";
        };
    } else {
        cout << "That's not a valid direction.\n";
    };
};

void useItemLogic(Hero& player, Item& item){
    if (item.name == "health potion") {
        int healAmount = runRNG(5, 10);
        cout << "You use the " << MAGENTA << item.name << RESET
             << " and restore " << GREEN << healAmount << RESET << " health." << endl;
        player.heal(healAmount);
        if (item.singleUse) {
            for (size_t i = 0; i < player.inv.size(); ++i) {
                if (player.inv[i].name == item.name) {
                    player.inv.erase(player.inv.begin() + i);
                    break;
                };
            };
        };
    }
    else if (item.name == "holy water") {
        if (player.location->name == "The Ember Eye") {
            cout << "You hurl the holy water at Zarghoul like a grenade. It shatters against his bony exterior,\nunleashing a purifying torrent that weakens the foul lich!\n\n";
            player.location->enemys[0].hp = 20;
            player.location->enemys[0].st = 0;
            player.location->enemys[0].dex = 1;
        	if (item.singleUse) {
            for (size_t i = 0; i < player.inv.size(); ++i) {
                if (player.inv[i].name == item.name) {
                    player.inv.erase(player.inv.begin() + i);
                    break;
                };
            };
        };
        } else {
            cout << "I wouldn’t waste that.\n";
        };
    };
};
void startGame(){
	//Create Objects
	//items
	Item hpot("health potion", "Can be used to heal 5 - 10 health.",true,true);
	Item key1("silver key", "A small silver key", false, true);
	Item key2("old key", "a dusty iron key", false, true);
	Item holyw("holy water", "A vial of holy water used to purify unholy things.", true, true);
	//weapons
	Weapon mace("Mace",1,6);
	Weapon longsword("Longsword",1,10);
	Weapon unholystaff("Unholy Staff", 1,10);
	
	//NPCs
	NPC amara("amara", "a maden kneeling on the cold stone floor, her flaxen\nhair tangled and dull from days of captivity. Her skin, pale and fair, gleams faintly in\nthe torchlight, streaked with dust and the faintest bruises at her temples. Despite her\nweakened state, her clear blue eyes meet yours with steady resolve, and the soft curve of\nher lips quivers only once before she steels herself again.");

	//monsters
	Monster skeleton("skeleton", 13, 1, 3, &mace, "A clattering mass of old bones bound by dark magic. Its hollow eye sockets glow\nfaintly with unnatural light, and it grips a rusted mace with unsettling steadiness.", "The final blow shatters the skeleton's spine. Bones fly in all directions as the magical\nforce holding it together vanishes. With a final clatter, the skull rolls to a stop, \ngrinning forever in death.");
	Monster zarghoul("zarghoul", 666, 10, 10, &unholystaff,"A towering lich in tattered robes,\nhis gaunt skull half concealed by shadow. In one empty socket he bears a jagged void;\nin the other glows a mote of amber flame, pulsing like a dying star within his brow.\n\nA chill wind stirs the dust at his feet as he turns to face you. His voice, a rasp of stone,\nechoes through the hollow vault:\n\n'So, you have come at last. Mortals dare tread where gods have fallen. Tell me, fragile one,\nwhat hope fuels your footsteps into the abyss?'\n\nHis gem eye flares bright with mocking light. You grip your sword tight, heart pounding,\nready to answer the challenge of the Ember Eye's master.\n","");

	//rooms
	Room room1("Entrance", "a vaulted stone chamber slick with damp moss and lichen.\nFaint shafts of light filter through cracks high above, casting eerie patterns on eroded\nglyphs and cobweb draped niches crumbling in silence. The air is stale and heavy, echoing\nwith the distant drip of water. Scattered across the cracked flagstones lie fragments of\nshattered pottery and bleached bone, relics of a crypt long abandoned.");
	Room room2("Crossroads", "a circular chamber hewn from smooth, ancient stone. Its walls\nare etched with concentric bands of arcane symbols, now worn and half erased by time.\nDim torchlight flickers from evenly spaced sconces, casting dancing shadows that make\nthe glyphs seem to shift and move.");
	Room room3("Bone Room", "bone-white heaps heaped against every wall, like macabre cairns\nof skulls and femurs. The flagstones beneath your feet are slick with powdery bone dust,\nand the air is thick with the rancid stench of long decayed flesh.");
		room3.addMonster(skeleton);
		room3.addContents(hpot);
		room3.addContents(key1);
	Room room4("Prison Cell", "a narrow, chamber carved from rough hewn stone. The\nair is rank with sweat and rot, and the flagstones beneath your feet are mottled with\ncrusted blood.");
		room4.addNPC(amara);
		room4.addContents(key2);
	Room room5("The Ember Eye", "a sprawling burial chamber carved from black basalt.\nFaint embers glow within recessed channels in the floor, casting the walls in a flickering scarlet\nlight. At its center rests a massive sarcophagus of obsidian, its lid etched with\nthe twisted visage of Zarghoul and crowned by a halo of ember lit runes. The air is thick\nwith the acrid tang of brimstone and scorched earth.");
		room5.addMonster(zarghoul);
	//doors
	room1.north = Door("a rotting wooden door bound with rusted iron hinges.", false, true, 1, &room2);
	room2.south = Door("a rotting wooden door bound with rusted iron hinges.", false, true, 2, &room1);
	room2.west = Door("an iron door adorned with silver embellishments.", true, true, 3, &room4, &key1); // locked
	room2.north = Door("a granite door carved with ancient runes and flecked with moss.", true, true, 4, &room5, &key2); // locked
	room2.east = Door("a sturdy oak door studded with black iron nails.", false, true, 5, &room3);
	room3.west = Door("a sturdy oak door studded with black iron nails.", false, true, 6, &room2);
	room4.east = Door("an iron door adorned with silver embellishments.", true, true, 7, &room2, &key1); // locked
	room5.south = Door("a granite door carved with ancient runes and flecked with moss.", true, true, 8, &room2, &key2); // locked
	
	//player creation
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    string playerName;
    while(playerName == ""){
    	clearScreen();
    	cout << "Name your Adventurer: ";
		getline(cin,playerName);
	};
    printPrologue(playerName);
    
    // Main Game Loop #################################################################
	clearScreen();
	//player
	Hero player(playerName, 30, 30, 3, 3, &longsword, &room1);
	player.addItem(hpot);
	while (player.hp > 0){
		runRoom(player.location,player);
	};
};

void endGame(int type){
	if(type == 0){
		cout << "Your vision blurs as wounds overwhelm you...\n"
     	"You collapse beneath the weight of battle, breath fading...\n"
     	"Darkness claims you, your journey ends here.\n" << endl;
		Enter2Continue("\n\n--- Game Over ---\n\nPress Enter to return to the main menu.", RED);
    	cin.clear();
    	clearScreen();
	}else{
		cout << "At the final strike, Zarghoul's hollow skull cracks in half with a thunderous snap.\n"
    	 "A geyser of necrotic energy erupts from the fissure, then fizzles into motes of ash\n"
     	"that drift away on stale air. The obsidian sarcophagus shudders and the runes across\n"
     	"its lid flare once in angry crimson before the entire chamber groans in protest.\n\n"
     	"Stone slabs above crack and tumble. Dust puffs up around you as the Sanctum's vaulted\n"
     	"ceiling begins to collapse. With heart pounding, you sprint past toppled pillars and\n"
     	"skitter over broken masonry as walls fracture and the floor trembles beneath your feet.\n\n"
     	"A rain of rubble chases you down the corridor; with one final burst of speed, you dive\n"
     	"through the doorway and bolt into the dying light.\n\n"
     	"Outside, you gulp sweet night air. The earth settles behind you, sealing the tomb's\n"
     	"entrance. Safe but breathless, you set off down the road back to Tarlava beneath the\n"
     	"flicker of starlight. At the gates, Mayor Harlan Merrow and Priestess Amara await.\n\n"
     	"Harlan's eyes shine with relief; Amara's hand flies to her heart. 'I knew you would\n"
     	"prevail,' Harlan whispers. Amara steps forward, eyes bright: 'You have saved us all.'\n\n"
     	"You recount the final clash, the lich's unholy scream, and the collapse, each word\n"
     	"restoring your strength. Behind you, townsfolk erupt in cheers, flaring torches into\n"
     	"the night sky. From this night forward, your name will echo in Tarlava’s songs \n"
     	"Champion of the Ember Eye.\n" << endl;
		Enter2Continue("\n\n--- You Won! ---\n\nPress Enter to return to the main menu.", GREEN); 
		cin.clear();
		clearScreen();
	}

};