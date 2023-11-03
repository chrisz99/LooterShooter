#include "Weapon.h"
#include <string>
#include "GameFunctions.h"
#include "Player.h"

//Constructor
Weapon::Weapon(std::string name, WeaponType weaponType, double damage, int level_req, double attackRate, int accuracy) : name(name),
weaponType(weaponType), damage(damage), level_req(level_req), attackRate(attackRate), accuracy(accuracy){}


//Default Constructor
Weapon::Weapon() {}

//Override for << Operator
 std::ostream& operator<<(std::ostream& os, const Weapon& obj) {
	 os << "Name: " << obj.name << " | " << "Weapon Type: " << Weapon::weaponTypeToString(obj.weaponType) << " | "
		 << "Damage: " << obj.damage << " | " << "Level Req: " << obj.level_req << " | "
		 << "Attack-Rate: " << obj.attackRate << " | " << "Accuracy: " << obj.accuracy << std::endl;
	 return os;
}

 //Weapon Names
std::vector<std::string> Weapon::assaultRifleNames = { "AR-15", "AR-10", "MK-18", "AR-15 Short Barreled", "AK47", "AK101", "AK105" };
std::vector<std::string> Weapon::sniperNames = { "DMR", "Mosin-Nagant", "M23", "SVD", "SSG-08" };
std::vector<std::string> Weapon::meleeNames = { "Baseball Bat", "Stiletto", "Combat-Knife", "Frying-Pan", "Knife"};
std::vector<std::string> Weapon::smgNames = { "MP5", "TMP", "Uzi", "Vector", "UMP-45", "MPX" };



//Map For Base Damage Values | Accuracy | Fire-Rate
//DAMAGE || ACCURACY || FIRE RATE
 std::map<Weapon::WeaponType, std::tuple<double, int, double>> Weapon::weaponBaseDamage = {
	{ Weapon::WeaponType::AssaultRifle, std::make_tuple(35,70,1.3)},
	{ Weapon::WeaponType::Sniper, std::make_tuple(80,85,0.8) },
	{ Weapon::WeaponType::Pistol, std::make_tuple(20,45,1) },
	{ Weapon::WeaponType::Melee, std::make_tuple(60,100,2) },
	{ Weapon::WeaponType::SMG, std::make_tuple(30,55,1.6) }
};

Weapon Weapon::createWeapon(Player player, WeaponType weaponType, bool levelRequireRNG) {

	std::string name;
	double damage;
	int level_req;
	double attackRate;
	int accuracy;
	int randomint;
	

	//Level Req Roll --> Determine whether weapon will be within players level
	if (GameFunctions::rngInt(0, 1) == 0 && levelRequireRNG == true)
		level_req = player.getPlayerLevel() + GameFunctions::rngInt(0, 5);
	else
		level_req = player.getPlayerLevel();


	//Set Attack Rate
	attackRate = std::get<2>(weaponBaseDamage[weaponType]);

	if (weaponType != WeaponType::Sniper)
		attackRate = attackRate + GameFunctions::rngDouble(-0.5, 0.8);

	
	//Set Accuracy
	accuracy = std::get<1>(weaponBaseDamage[weaponType]) + GameFunctions::rngInt(-15, 10);



	//Set Damage
	damage = std::get<0>(weaponBaseDamage[weaponType]) + GameFunctions::rngDouble(-10, 10);
	


	switch (weaponType) {

	case WeaponType::AssaultRifle: {
		name = Weapon::assaultRifleNames[GameFunctions::rngInt(0, Weapon::assaultRifleNames.size() - 1)];
		break;
	}
	case WeaponType::SMG: {
		name = smgNames[GameFunctions::rngInt(0, Weapon::smgNames.size() - 1)];
		break;
	}
	case WeaponType::Melee: {
		name = meleeNames[GameFunctions::rngInt(0, Weapon::meleeNames.size() - 1)];
		break;
	}
	case WeaponType::Sniper: {
		name = sniperNames[GameFunctions::rngInt(0, Weapon::sniperNames.size() - 1)];
		break;
	}

	}

	return Weapon(name, weaponType, damage, level_req, attackRate, accuracy);

}

std::string Weapon::weaponTypeToString(Weapon::WeaponType weaponType) {
	switch (weaponType) {
	case Weapon::WeaponType::AssaultRifle:
		return "Assault Rifle";
	case Weapon::WeaponType::Melee:
		return "Melee";
	case Weapon::WeaponType::Pistol:
		return "Pistol";
	case Weapon::WeaponType::SMG:
		return "SMG";
	case Weapon::WeaponType::Sniper:
		return "Sniper";
	default:
		return "Assault Rifle";
	}
}



//To String
std::string Weapon::toString() {
	return "Name: " + name + " WeaponType: " + weaponTypeToString(weaponType) + " Damage: " + std::to_string(damage) + " Level Req: " + std::to_string(level_req) + " Attack Rate: " +
		std::to_string(attackRate) + " Accuracy: " + std::to_string(accuracy);

}

bool Weapon::compareWeapons(Weapon* weapon, Weapon* weaponCompare) {

	if (weapon->name != weaponCompare->name)
		return false;
	if (weapon->weaponType != weaponCompare->weaponType)
		return false;
	if (weapon->damage != weaponCompare->damage)
		return false;
	if (weapon->level_req != weaponCompare->level_req)
		return false;
	if (weapon->attackRate != weaponCompare->attackRate)
		return false;
	if (weapon->accuracy != weaponCompare->accuracy)
		return false;

	return true;
}


//Getters and Setters

double Weapon::getWeaponDamage() {
	if (this != nullptr)
		return damage;
	else
		return 0;
} 
void Weapon::setWeaponDamage(double damage){
	this->damage = damage;
	}
std::string Weapon::getWeaponName(){
	if (this != nullptr)
	return name;
	else
		return "";
	}
void Weapon::setWeaponName(std::string name){
	this->name = name;
	}
int Weapon::getWeaponAccuracy(){
if (this != nullptr)	
return accuracy;
else
return 0;
	}
void Weapon::setWeaponAccuracy(int accuracy){
	this->accuracy = accuracy;
	}
double Weapon::getAttackRate() {
if (this != nullptr)	
return attackRate;
else
return 0;
}

void Weapon::setAttackRate(double attackRate){
	this->attackRate = attackRate;
	}

int Weapon::getWeaponLevelReq() {
if (this != nullptr)	
return this->level_req;
else
return 0;
}
void Weapon::setWeaponLevelReq(int levelreq){
	this->level_req = levelreq;
	}

Weapon::WeaponType Weapon::getWeaponType(){
	if (this != nullptr)
		return this->weaponType;
	else
		return WeaponType::AssaultRifle;
	}
void Weapon::setWeaponType(WeaponType weaponType){
	this->weaponType = weaponType;
	}

