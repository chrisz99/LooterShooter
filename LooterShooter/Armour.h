#pragma once
#include <string>
#include "Player.h";



class Armour {

public:
	enum class ArmourType {BODY,HEAD_AND_BODY,FULL_BODY};

	static std::map<ArmourType, std::vector<std::tuple<std::string,int, double>>> armourList;

private:
	std::string name;
	int protectionStat;
	int level_Req;
	ArmourType armourType;
	double armourHealth;
	double baseArmourHealth;

public:

	//Default Constructor
	Armour();

	//Reg Constructor
	Armour(std::string name, int protectionStat, int level_Req, ArmourType armourType, double armourHealth, double baseArmourHealth);

	//Create Armour --> From Player
	static Armour createArmour(Player* player, bool levelRequireRng);

	//Utility
		//To String
	std::string toString();
		//Return Armour Type as String
	static std::string armourTypeToString(ArmourType armourType);

	//Setters and Getters

	std::string getArmourName();
	void setArmourName(std::string armourName);

	int getArmourProtectionStat();
	void setArmourProtectionStat(int protectionStat);

	int getArmourLevelReq();
	void setArmourLevelReq(int levelreq);

	ArmourType getArmourType();
	void setArmourType(ArmourType armourType);

	double getArmourHealth();
	void setArmourHealth(double armourHealth);

	double getBaseArmourHealth();
	void setBaseArmourHealth(double baseArmourHealth);

};