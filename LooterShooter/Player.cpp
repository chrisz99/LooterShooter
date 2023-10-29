#include "Player.h"
#include <iostream>
#include <string>
#include "Weapon.h"
#include "GameFunctions.h"
#include <iostream>
#include <Windows.h>


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
	this->primaryWeapon = nullptr; // No primary weapon in this constructor
}


// Player Constructor with Primary Weapon
Player::Player(std::string name, double health, double exp, int level, int inventory_space, double dps, double armour, Weapon* primaryWeapon) {
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

//Default Constructor
Player::Player() {}


// << Override Operator
std::ostream& operator<<(std::ostream& os, const Player& obj) {
	os << "Name:" << obj.name << " Health:" << obj.health << " Level:" << obj.level << " DPS:" << obj.dps << " Armour:" << obj.armour;
	return os;
}

//Equip Primary Weapon
void Player::equipPrimaryWeapon(Weapon* primaryWeapon){
this->primaryWeapon = primaryWeapon;
refreshPlayerEquipment();
}


void Player::managePlayerInventory() {
	system("cls");
	GameFunctions::colorPrint("Player %s Inventory:", FOREGROUND_BLUE, name);
}

//Refreshs Player Stats After Equipping Equipment
void Player::refreshPlayerEquipment() {
	this->dps = this->getPrimaryWeapon()->getWeaponDamage() * this->getPrimaryWeapon()->getAttackRate();
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

Weapon* Player::getPrimaryWeapon() {
	return this->primaryWeapon;
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

	return numItems;
	
}


std::string Player::toString() {
	return "Name: " + this->name + " Health: " + std::to_string(this->health) + " Exp: " + std::to_string(this->exp) + 
		" DPS: " + std::to_string(this->dps) + " Armour: " + std::to_string(this->armour) + " Inventory: " 
		+ std::to_string(getNumOfItemsInInventory()) + "/" + std::to_string(playerInventory.weaponInventory.size());
}




