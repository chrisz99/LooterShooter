#pragma once
#include "Weapon.h"
#include <string>
#include "Player.h"
#include <nlohmann/json.hpp>
#include <Windows.h>


namespace GameFunctions {

	CONST int DEFAULT_COLOR = FOREGROUND_INTENSITY | FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE;

	//Main Game Functions
	// 
		//Starting Loop of Game -> Users Create a Character / Select a Character
		Player gameStart();
		//Main Loop of Game
		bool gameLoop(Player* selectedPlayer);



	
	//Utility Functions
	std::string colorPrint(const char* format, int color, ...);

	//RNG Functions
	int rngInt(int min, int max);
	double rngDouble(double min, double max);

	//Damage Scaling Functions
	double scaleDamage(double baseDamage, int level);



	//Login / Save | Load Data
	bool savePlayerData(Player* player);

	void deletePlayerData(Player* player);

	//JSON Methods
	nlohmann::json playerToJson(Player* player);
	nlohmann::json weaponToJson(Weapon* weapon);
	nlohmann::json inventoryToJson(Player* player);
	Player::Inventory jsonToInventory(nlohmann::json inventoryJson);
	std::vector<Player> loadPlayers();



	Player createCharacterHelper();




}