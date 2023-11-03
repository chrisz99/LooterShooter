#pragma once
#include <string>
#include <iostream>
#include <vector>
#include "Weapon.h"

class Weapon;

class Player {

public: 
	struct Inventory {
		std::vector<Weapon> weaponInventory;

	};

private:
	std::string name;
	double health;
	double exp;
	int level;
	int inventory_space;
	double dps;
	double armour;
	Weapon primaryWeapon;
	Inventory playerInventory;


public:
	//Player Constructor
	Player(std::string name, double health, double exp, int level, int inventory_space, double dps, double armour);

	//Player Constructor with Primary Weapon
	Player(std::string name, double health, double exp, int level, int inventory_space, double dps, double armour, Weapon primaryWeapon);

	//Player Constructory with Primary Weapon and Inventory
	Player(std::string name, double health, double exp, int level, int inventory_space, double dps, double armour, Weapon primaryWeapon, Inventory playerInventory);


	//Default Constructor
	Player();


	//Override << operator
	friend std::ostream& operator<<(std::ostream& os, const Player& obj);

	bool addItem(Weapon* weaponItem);


	//Equip Primary Weapon
	void equipPrimaryWeapon(Weapon primaryWeapon);

	//Refresh Player Equipment so Player Stats can Update
	void refreshPlayerEquipment();

	//Refresh Player Inventory so that Equipped Items are Added to Inv
	void refreshPlayerInventory();
	
	//Display Inventory
	void displayInventory();

	//Manage Inventory
	void managePlayerInventory();


	//Setters and Getters
	std::string getPlayerName();
	void setPlayerName(std::string name);

	Weapon getPrimaryWeapon();

	double getPlayerHealth();
	void setPlayerHealth(double health);

	double getPlayerExp();
	void setPlayerExp(double exp);

	int getPlayerLevel();
	void setPlayerLevel(int level);

	int getPlayerInventorySpace();
	void setPlayerInventorySpace(int inventoryspace);

	double getPlayerDPS();
	void setPlayerDPS(double dps);

	double getPlayerArmour();
	void setPlayerArmour(double armour);

	Inventory* getPlayerInventory();
	void setPlayerInventory(Inventory* inventory);

	int getNumOfItemsInInventory();

	std::string toString();


	


};