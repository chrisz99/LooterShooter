#pragma once
#include <string>
#include <map>
#include <vector>

class Player;

class Weapon {
public:
	//Enums for Weapon Type
	enum class WeaponType {AssaultRifle, Sniper, Pistol, Melee, SMG};

	//Declaration for Base Damage Map
	static std::map<WeaponType, std::tuple<double,int,double>> weaponBaseDamage;

	//Declaration for Weapon Names
	static std::vector<std::string> assaultRifleNames;
	static std::vector<std::string> sniperNames;
	static std::vector<std::string> meleeNames;
	static std::vector<std::string> smgNames;

	//Override Operator
	friend std::ostream& operator<<(std::ostream& os, const Weapon& obj);



private:

private:
	//Class Vars
	std::string name;
	WeaponType weaponType;
	double damage;
	int level_req;
	double attackRate;
	int accuracy;

public:
	//Constructor
	Weapon(std::string name, WeaponType weaponType, double damage, int level_req, double attackRate, int accuracy);

	//Default Constructor
	Weapon();

	//Create a Weapon
	static Weapon createWeapon(Player player, WeaponType weaponType, bool levelRequireRNG);

	//Compare Two Weapon Object -> Returns true if values are ==
	static bool compareWeapons(Weapon* weapon, Weapon* weaponCompare);

	//Utility Functions
		//Weapon Type to String
		static std::string weaponTypeToString(Weapon::WeaponType weaponType);

		//To String
		std::string toString();

		//Check if uninitialized or empty
		bool isEmpty();

	//Getters and Setters
	double getWeaponDamage();
	void setWeaponDamage(double damage);

	std::string getWeaponName();
	void setWeaponName(std::string name);

	int getWeaponAccuracy();
	void setWeaponAccuracy(int accuracy);

	double getAttackRate();
	void setAttackRate(double attackRate);

	int getWeaponLevelReq();
	void setWeaponLevelReq(int levelreq);

	WeaponType getWeaponType();
	void setWeaponType(WeaponType weaponType);




};