#pragma once
#include "Weapon.h"
#include <string>
#include "Player.h"
#include <nlohmann/json.hpp>

namespace GameFunctions {

	//RNG Functions
	int rngInt(int min, int max);
	double rngDouble(double min, double max);

	//Damage Scaling Functions
	double scaleDamage(double baseDamage, int level);

	//Utility Functions
	std::string weaponTypeToString(Weapon::WeaponType weaponType);

	//Login / Save | Load Data
	bool savePlayerData(Player* player);

	//JSON Methods
	nlohmann::json playerToJson(Player* player);
	nlohmann::json weaponToJson(Weapon* weapon);
	std::vector<Player> loadPlayers();

	//Starting Loop of Game -> Users Create a Character / Select a Character
	Player gameStart();


}