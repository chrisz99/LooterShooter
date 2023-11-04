#include "Player.h"
#include <iostream>
#include <string>
#include "Weapon.h"
#include "GameFunctions.h"
#include <iostream>
#include <Windows.h>
#include <sstream>
#include <algorithm>

class Weapon;

//Constructor
Player::Player(std::string name, double health, double exp, int level, int inventory_space, double dps, double armour) {
	// Initialize the player without a primary weapon
	this->name = name;
	this->health = health;
	this->exp = exp;
	this->level = level;
	this->inventory_space = inventory_space;
	this->dps = dps;
	this->armour = armour;
}


// Player Constructor with Primary Weapon
Player::Player(std::string name, double health, double exp, int level, int inventory_space, double dps, double armour, Weapon primaryWeapon) {
	// Initialize the player with a primary weapon
	this->name = name;
	this->health = health;
	this->exp = exp;
	this->level = level;
	this->inventory_space = inventory_space;
	this->dps = dps;
	this->armour = armour;
	this->primaryWeapon = primaryWeapon;
}

// Player Constructor with Primary Weapon and Player Inventory
Player::Player(std::string name, double health, double exp, int level, int inventory_space, double dps, double armour, Weapon primaryWeapon, Inventory playerInventory) {
	this->name = name;
	this->health = health;
	this->exp = exp;
	this->level = level;
	this->inventory_space = inventory_space;
	this->dps = dps;
	this->armour = armour;
	this->primaryWeapon = primaryWeapon;
	this->playerInventory = playerInventory;
}


//Player Constructory with Primary Weapon and Inventory and Equipped Armour
Player::Player(std::string name, double health, double exp, int level, int inventory_space, double dps, double armour, Weapon primaryWeapon, Inventory playerInventory, Armour equippedArmour) {
	this->name = name;
	this->health = health;
	this->exp = exp;
	this->level = level;
	this->inventory_space = inventory_space;
	this->dps = dps;
	this->armour = armour;
	this->primaryWeapon = primaryWeapon;
	this->playerInventory = playerInventory;
	this->equippedArmour = equippedArmour;
}

//Default Constructor
Player::Player() {}


// << Override Operator
std::ostream& operator<<(std::ostream& os, const Player& obj) {
	os << "Name:" << obj.name << " Health:" << obj.health << " Level:" << obj.level << " DPS:" << obj.dps << " Armour:" << obj.armour;
	return os;
}



//Equip Primary Weapon
void Player::equipPrimaryWeapon(Weapon primaryWeapon){
this->primaryWeapon = primaryWeapon;
refreshPlayerEquipment();
}

void Player::equipArmour(Armour* armour) {
	this->equippedArmour = *armour;
	refreshPlayerEquipment();
}



void invalidCommand(std::string errorText = "Invalid Command!") {
	if (errorText == "Invalid Command!") {
		GameFunctions::colorPrint("Invalid Command!", FOREGROUND_RED);
		system("pause");
		std::cin.clear();
	}
	else {
		GameFunctions::colorPrint(errorText.c_str(), FOREGROUND_RED);
		system("pause");
		std::cin.clear();

	}
}

//Adds Item to player Inventory
bool Player::addItem(Weapon* weaponItem) {
	if (playerInventory.weaponInventory.size() < inventory_space) {
		playerInventory.weaponInventory.push_back(*weaponItem);
		return true;
	}
	else {
		invalidCommand("Not enough space in player inventory!");
		return false;
	}
}

bool Player::addItem(Armour* armourItem) {
	if (playerInventory.weaponInventory.size() < inventory_space) {
		playerInventory.armourInventory.push_back(*armourItem);
		return true;
	}
	else {
		invalidCommand("Not enough space in player inventory!");
		return false;
	}
}



//Refresh Player Inventory so that Equipped Items are Added to Inv
void Player::refreshPlayerInventory() {
	bool primaryWeaponFound = false;
	bool armourFound = false;


		//Look for Primary Weapon in Inventory
		for (int i = 0; i < playerInventory.weaponInventory.size(); i++) {
			if (Weapon::compareWeapons(&playerInventory.weaponInventory[i], &primaryWeapon))
				primaryWeaponFound = true;
		}

		//Look for Equipped Armour In Inventory
		for (int i = 0; i < playerInventory.armourInventory.size(); i++) {
			if (Armour::compareArmours(&playerInventory.armourInventory[i], &equippedArmour))
				armourFound = true;
		}

		//If not found add it to player inventory
		if (!primaryWeaponFound && !primaryWeapon.getWeaponName().empty())
			playerInventory.weaponInventory.push_back(primaryWeapon);

		if (!armourFound && !equippedArmour.isEmpty())
			playerInventory.armourInventory.push_back(equippedArmour);
	
}

void Player::displayInventory() {
	int index = 0;
	GameFunctions::colorPrint("Player %s Inventory:\n", FOREGROUND_BLUE, name.c_str());
	std::cout << "Weapons:\n" << std::endl;
	for (int i = 0; i < playerInventory.weaponInventory.size(); i++) {
		if (Weapon::compareWeapons(&primaryWeapon, &playerInventory.weaponInventory[i]))
		std::cout << i + 1 << ".)" << " *EQUIPPED* " << playerInventory.weaponInventory[i] << std::endl;
		else
			std::cout << i + 1 << ".) " << playerInventory.weaponInventory[i] << std::endl;
		index = i + 1;
	}
	std::cout << "Armour:\n" << std::endl;
	index += 1;
	for (int i = 0; i < playerInventory.armourInventory.size(); i++) {
		if (Armour::compareArmours(&equippedArmour, &playerInventory.armourInventory[i]))
			std::cout << index  << ".)" << " *EQUIPPED* " << playerInventory.armourInventory[i].toString() << std::endl << std::endl;
		else
			std::cout << index  << ".) " << playerInventory.armourInventory[i].toString() << std::endl << std::endl;
		index += 1;
	}

}


void Player::managePlayerInventory() {
	bool managingInventory = true;
	while (managingInventory) {
		system("cls");
		GameFunctions::colorPrint("Commands { EQUIP 'INDEX' , DELETE 'INDEX' , DONE }\n", FOREGROUND_BLUE);
		displayInventory();
		std::string playerCommand;
		if (std::getline(std::cin, playerCommand) && !playerCommand.empty() && playerCommand != " ") {
			std::istringstream iss(playerCommand);
			std::string command;
			int index;

			//Extract Command
			if (iss >> command && !command.empty() && command != " ") {

				std::transform(command.begin(), command.end(), command.begin(), ::toupper);

				if (command == "DONE")
					managingInventory = false;
				else {
					//Extract Index if Provided
					if (iss >> index && index > 0 && index <= playerInventory.weaponInventory.size() + playerInventory.armourInventory.size()) {
					}
					else
					{
						invalidCommand();
						continue;
					}

					if (command == "EQUIP") {
						system("cls");
						if (index <= playerInventory.weaponInventory.size()) {
							primaryWeapon = playerInventory.weaponInventory[index - 1];
							GameFunctions::colorPrint("Weapon:\n%s\nSuccessfully Equipped!", FOREGROUND_GREEN, playerInventory.weaponInventory[index - 1].toString().c_str());
						}
						else {
							int armourIndex = (index - 1) - playerInventory.weaponInventory.size();
							equippedArmour = playerInventory.armourInventory[armourIndex];
							GameFunctions::colorPrint("Armour:\n%s\nSuccessfully Equipped!", FOREGROUND_GREEN, playerInventory.armourInventory[armourIndex].toString().c_str());
						}
						system("pause");

					}
					else if (command == "DELETE") {
						system("cls");
						if (Weapon::compareWeapons(&primaryWeapon, &playerInventory.weaponInventory[index - 1])) {
							primaryWeapon = Weapon();
							GameFunctions::colorPrint("Weapon:\n%s\nSuccessfully Removed!", FOREGROUND_GREEN, playerInventory.weaponInventory[index - 1].toString().c_str());
							playerInventory.weaponInventory.erase(playerInventory.weaponInventory.begin() + index);
							
						}
					
						else {
							if (index >= 0 && index <= playerInventory.weaponInventory.size()) {
							GameFunctions::colorPrint("Weapon:\n%s\nSuccessfully Removed!", FOREGROUND_GREEN, playerInventory.weaponInventory[index - 1].toString().c_str());
							playerInventory.weaponInventory.erase(playerInventory.weaponInventory.begin() + index - 1);

							}
						}
								
					
						system("pause");

					}
					else {
						invalidCommand();
						continue;
					}
				}


				
			}



		}


	}

	//Refresh Player Equipment
 	refreshPlayerEquipment();
	refreshPlayerInventory();

	//Save Player Data
	GameFunctions::savePlayerData(this);

}

//Refreshs Player Stats After Equipping Equipment
void Player::refreshPlayerEquipment() {
	if (!primaryWeapon.isEmpty())
	this->dps = primaryWeapon.getWeaponDamage() * primaryWeapon.getAttackRate();
	if (!equippedArmour.isEmpty())
		this->armour = equippedArmour.getArmourProtectionStat();
}



//Getters and Setters

std::string Player::getPlayerName() {
	return name;
}

void Player::setPlayerName(std::string name) {
	this->name = name;
}

int Player::getPlayerLevel() {
	return level;
}

void Player::setPlayerLevel(int level) {
	this->level = level;
}

Weapon Player::getPrimaryWeapon() {
	return primaryWeapon;
}

Armour Player::getEquippedArmour() {
	return equippedArmour;
}



double Player::getPlayerHealth() {
	return health;
}
void Player::setPlayerHealth(double health){
	this->health = health;
	}

double Player::getPlayerExp(){
	return exp;
	}
void Player::setPlayerExp(double exp){
	this->exp = exp;
	}

int Player::getPlayerInventorySpace(){
	return inventory_space;
	}
void Player::setPlayerInventorySpace(int inventoryspace){
	this->inventory_space = inventoryspace;
	}

double Player::getPlayerDPS(){
	return dps;
	}
void Player::setPlayerDPS(double dps){
	this->dps = dps;
	}

double Player::getPlayerArmour(){
	return armour;
	}
void Player::setPlayerArmour(double armour){
	this->armour = armour;
	}


Player::Inventory* Player::getPlayerInventory() {
	return &playerInventory;

}
void Player::setPlayerInventory(Inventory* inventory) {
	this->playerInventory = *inventory;
}

int Player::getNumOfItemsInInventory() {
	int numItems = 0;

	for (const Weapon w : playerInventory.weaponInventory) 
		numItems += 1;

	for (const Armour a : playerInventory.armourInventory)
		numItems += 1;

	return numItems;
	
}


std::string Player::toString() {
	return "Name: " + this->name + " Health: " + std::to_string(this->health) + " Exp: " + std::to_string(this->exp) + 
		" DPS: " + std::to_string(this->dps) + " Armour: " + std::to_string(this->armour) + " Inventory: " 
		+ std::to_string(getNumOfItemsInInventory()) + "/" + std::to_string(inventory_space);
}




