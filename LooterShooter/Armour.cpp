#include <string>
#include "Player.h"
#include "Armour.h"
#include <vector>
#include "GameFunctions.h"
#include <iomanip> 
#include <sstream>
//Armour List
//Tuple ----> Armour Name || Armour Base Protection || Armour Base Health
std::map<Armour::ArmourType, std::vector<std::tuple<std::string, int,double>>> Armour::armourList = {
	{ArmourType::BODY, 
	{
		{"Press-Vest",5,25},
		{"Anti-Stab Vest", 2,15},
		{"AR500-Steel Kevlar", 8,35},
		{"Homemade Plate-Carrier", 6,20}
	}},
	{ArmourType::HEAD_AND_BODY,
	{
		{"Russian PASGT", 15,50},
	{"SWAT PASGT", 15,50},
	{"US PASGT",15,50},
	{"Homemade PASGT", 10,40}

	}},
	{ArmourType::FULL_BODY,{
		{"The Tank", 25,80},
	{"Flak Fitted PASGT", 20,80},
	{"Homemade Flak PASGT", 17,60}
	}}
};


Armour::Armour() {};

Armour::Armour(std::string name, int protectionStat, int level_Req, ArmourType armourType, double armourHealth, double baseArmourHealth) {
	this->name = name;
	this->protectionStat = protectionStat;
	this->level_Req = level_Req;
	this->armourType = armourType;
	this->armourHealth = armourHealth;
	this->baseArmourHealth = baseArmourHealth;
}

std::string Armour::armourTypeToString(ArmourType armourType) {
	switch (armourType) {
	case ArmourType::BODY:
		return "Body";
		break;

	case ArmourType::HEAD_AND_BODY:
		return "Head and Body";
		break;

	case ArmourType::FULL_BODY:
		return "Full Body";
		break;
	}
}


std::string Armour::toString() {
	std::ostringstream streamArmourHealth, streamBaseArmourHealth;
	streamArmourHealth << std::fixed << std::setprecision(1) << this->armourHealth;
	streamBaseArmourHealth << std::fixed << std::setprecision(1) << this->baseArmourHealth;

	// Convert the rounded doubles to strings
	std::string formattedArmourHealth = streamArmourHealth.str();
	std::string formattedBaseArmourHealth = streamBaseArmourHealth.str();

	// Construct the string with formatted doubles
	return "Name: " + this->name + " Armour: " + std::to_string(this->protectionStat) + " Level Req: " + std::to_string(this->level_Req) +
		" ArmourType: " + armourTypeToString(this->armourType) + " Armour Health: " + formattedArmourHealth + "/" + formattedBaseArmourHealth;
}


//Create Armour --> Player
Armour Armour::createArmour(Player* player, bool levelRequireRng) {

	//Init Vars
	int level_req;
	int protectionStat;
	double armourHealth;
	double baseArmourHealth;
	std::string name;


	//Roll an Armour Type
	double armourTypeRoll = GameFunctions::rngDouble(0, 1);
	ArmourType armourType = ArmourType::BODY;
	if (armourTypeRoll >= .6)
		armourType = ArmourType::BODY;
	else if (armourTypeRoll >= .3)
		armourType = ArmourType::HEAD_AND_BODY;
	else if (armourTypeRoll >= .1)
		armourType = ArmourType::FULL_BODY;


	//Roll Base Armour from Armour List
	int armourListRoll = GameFunctions::rngInt(0, armourList[armourType].size() - 1);

	//Set Name
	name = std::get<0>(armourList[armourType][armourListRoll]);

	//Roll Level Req
	if (levelRequireRng)
		level_req = GameFunctions::rngInt(player->getPlayerLevel(), player->getPlayerLevel() + 5);
	else
		level_req = player->getPlayerLevel();

	//Roll Health of Found Armour
		//50 percent Chance of Armour being used
	if (GameFunctions::rngInt(0, 1) == 0)
		armourHealth = std::get<2>(armourList[armourType][armourListRoll]) + GameFunctions::rngDouble(-(std::get<2>(armourList[armourType][armourListRoll]) / 2), 0);
	else
		armourHealth = std::get<2>(armourList[armourType][armourListRoll]);

	//Set Base
	baseArmourHealth = std::get<2>(armourList[armourType][armourListRoll]);

	//Roll Protection Stat
	protectionStat = std::get<1>(armourList[armourType][armourListRoll]) + GameFunctions::rngInt(-(std::get<1>(armourList[armourType][armourListRoll]) / 2), 5);

	//Return Armour
	return Armour(name, protectionStat, level_req, armourType, armourHealth,baseArmourHealth);

}

//Setters and Getters

std::string Armour::getArmourName(){
	return this->name;
	}
void Armour::setArmourName(std::string armourName){
	this->name = armourName;
	}

int Armour::getArmourProtectionStat(){
	return this->protectionStat;
	}
void Armour::setArmourProtectionStat(int protectionStat){
	this->protectionStat = protectionStat;
	}

int Armour::getArmourLevelReq(){
	return this->level_Req;
	}
void Armour::setArmourLevelReq(int levelreq){
	this->level_Req = levelreq;
	}

Armour::ArmourType Armour::getArmourType(){
	return this->armourType;
	}
void Armour::setArmourType(ArmourType armourType){
	this->armourType = armourType;
	}

double Armour::getArmourHealth(){
	return this->armourHealth;
	}
void Armour::setArmourHealth(double armourHealth){
	this->armourHealth = armourHealth;
	}


double Armour::getBaseArmourHealth() {
	return this->baseArmourHealth;
}
void Armour::setBaseArmourHealth(double baseArmourHealth){
	this->baseArmourHealth = baseArmourHealth;
	}