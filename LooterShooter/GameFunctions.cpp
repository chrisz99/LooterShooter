#include <random>
#include "GameFunctions.h"
#include <cmath>
#include <windows.h>
#include <shlobj.h>
#include <iostream>
#include <codecvt>
#include <fstream> // Include the necessary header for file operations

namespace GameFunctions {

	Player GameFunctions::gameStart() {

		//init player list
		std::vector<Player> playerList = loadPlayers();
		bool characterSelected = false;

		std::cout << "Welcome to the wasteland, Press any key to select or create a character." << std::endl;
		system("pause");
		

		//Check if there is players available
		while (!characterSelected) {

			//If Characters are found -> select one
			if (!playerList.empty()) {
				system("cls");
				std::cout << "Select a Character Survivor!" << std::endl << std::endl;
				for (int i = 0; i < playerList.size(); i++)
					std::cout << i << ".) " << playerList[i] << std::endl << std::endl;

				int selectedIndex;
				if (std::cin >> selectedIndex) {
					if (selectedIndex < 0 || playerList.size() > selectedIndex) {
						return playerList[selectedIndex];
					}
					else
						std::cerr << "Invalid index for character selection" << std::endl;
				}
			}
			//No Characters Found
			else {
				//Init Player Vars
				std::string playerName;
				Weapon::WeaponType weaponType;

				system("cls");
				std::cout << "No character files are found, create a character!" << std::endl
					<< "What's your characters name?" << std::endl;
				std::cin >> playerName;
				std::cout << "What type of weapon do you wish to start out with?" << std::endl <<
					std::endl << "1.) Assault Rifle\n" << "2.) Sniper\n" << "3.) Pistol\n" << "4.) Melee\n" << "5.) SMG" << std::endl;
				int weaponIndex;
				if (std::cin >> weaponIndex) {
					if (weaponIndex > 0 && weaponIndex <= 5) {
						weaponType = static_cast<Weapon::WeaponType>(weaponIndex - 1);
					}
					else {
						std::cerr << "Invalid weapon type selected, try again" << std::endl;
						system("pause");
					}
				}

				//Create Player Object
				Player newPlayer(playerName, 100, 0, 0, 8, 0, 0);

				//Create Weapon Object
				Weapon starterWeapon = Weapon::createWeapon(newPlayer, weaponType, false);

				//Equip Primary Weapon
				newPlayer.equipPrimaryWeapon(&starterWeapon);
				
				//Save Player Data
				GameFunctions::savePlayerData(&newPlayer);

				//Create and Return new player object
				std::cout << "New player " << newPlayer.getPlayerName() << " successfully created!\n"
					<< newPlayer << "\n" << *newPlayer.getPrimaryWeapon() << std::endl;
				return newPlayer;

			}


		}


		return playerList[0];
		
	}


	int GameFunctions::rngInt(int min, int max) {
		std::mt19937 rng(std::random_device{}());

		std::uniform_int_distribution<int> distribution(min, max);

		return distribution(rng);
	}

	double GameFunctions::rngDouble(double min, double max) {
		std::mt19937 rng(std::random_device{}());

		std::uniform_real_distribution<double> distribution(min, max);

		double result = distribution(rng);
		return std::round(result * 10.0) / 10.0;
	}

	double GameFunctions::scaleDamage(double baseDamage, int level) {

		double scalingFactor = 1;

		if (level > 0 && level < 10)
			scalingFactor = rngDouble(1,1.2);
		else if (level > 10 && level < 20)
			scalingFactor = rngDouble(1, 1.5);
		else if (level > 20 && level < 30)
			scalingFactor = rngDouble(1, 2);


		double scaledDamage = baseDamage * scalingFactor;

		return scaledDamage;
	}

	std::string GameFunctions::weaponTypeToString(Weapon::WeaponType weaponType) {
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

	//Helper JSON Functs

	//Player to JSON
	nlohmann::json GameFunctions::playerToJson(Player* player) {
		//Create and populate the player JSON object
		nlohmann::json playerJson;

		playerJson = nlohmann::json{
			{"name", player->getPlayerName()},
			{"health", player->getPlayerHealth()},
			{"exp", player->getPlayerExp()},
			{"level", player->getPlayerLevel()},
			{"inventory_space", player->getPlayerInventorySpace()},
			{"dps", player->getPlayerDPS()},
			{"armour", player->getPlayerArmour()},
		};

		return playerJson;

	}

	//Weapon to JSON
	nlohmann::json GameFunctions::weaponToJson(Weapon* weapon){
		//Creates and Populates JSON with Weapon Data
		nlohmann::json weaponJson;

		weaponJson = nlohmann::json{
			{"name", weapon->getWeaponName()},
			{"weaponType", weapon->getWeaponType()},
			{"damage", weapon->getWeaponDamage()},
			{"level_req", weapon->getWeaponLevelReq()},
			{"attack_rate", weapon->getAttackRate()},
			{"accuracy", weapon->getWeaponAccuracy()}
		};

		return weaponJson;

		}

	//Helper Function to Grab Documents Path
	std::wstring getDocumentsPath() {

		//Grab Path To Documents -> If failed throw error message
		PWSTR documentsPointer;

		if (!SUCCEEDED(SHGetKnownFolderPath(FOLDERID_Documents, 0, NULL, &documentsPointer))) {
			std::cerr << "Failed to get documents path to save data!" << std::endl;
			return L"NULL";
		}

		//Convert PWSTR to wString
		std::wstring documentsPath(documentsPointer);

		return documentsPath;
	}

	//Save Player Data 
	bool GameFunctions::savePlayerData(Player* player) {

		std::wstring documentsPath = getDocumentsPath();
		
		//Define File Paths
		std::wstring gameFolderPath = documentsPath + L"\\LooterShooter1_0";
		std::wstring playerFilePath = gameFolderPath + L"\\game_data.json";

		//Create Game Save Folder -> If failed throw error
		if (!CreateDirectoryW(gameFolderPath.c_str(), NULL) && GetLastError() != ERROR_ALREADY_EXISTS) {
			std::cerr << "Failed to create game save folder!" << std::endl;
			return false;
		}



		//Create and Populate JSON objects
		nlohmann::json playerJson = playerToJson(player);
		nlohmann::json weaponJson = weaponToJson(player->getPrimaryWeapon());

		//Grab Previous Saves
		nlohmann::json prevSave = nlohmann::json::array();

		std::ifstream playerFileIn(playerFilePath);

		if (playerFileIn.is_open()) {
			playerFileIn >> prevSave;

		}

		//Add Primary Weapon to player JSON
		playerJson["primary_weapon"] = weaponJson;


		prevSave.push_back(playerJson);

		//Write contents of Player JSON with included Weapon JSON for primary Weapon
		std::ofstream playerFile(playerFilePath);
		if (playerFile.is_open()) {
			playerFile << prevSave.dump(4);
			playerFile.close();
		}
		else {
			std::cerr << "Failed to write save player data!" << std::endl;
			return false;
		}

		//File Was Successfully Saved!
		return true;


	}

	//Returns a list of players
	std::vector<Player> GameFunctions::loadPlayers() {
		std::wstring documentsPath = getDocumentsPath();
		std::wstring playerFilePath = documentsPath + L"\\LooterShooter1_0\\game_data.json";
		std::vector<Player> players;

		std::ifstream playerFile(playerFilePath);
		if (!playerFile.is_open()) {
			std::cerr << "Failed to read player save file!" << std::endl;
			return players;
		}

		nlohmann::json playerArray;
		playerFile >> playerArray;
		playerFile.close();


		for (const auto& playerJson : playerArray) {
			nlohmann::json primaryWeaponJson = playerJson["primary_weapon"];

			Weapon* primaryWeapon = new Weapon(primaryWeaponJson["name"], primaryWeaponJson["weaponType"], primaryWeaponJson["damage"],
				primaryWeaponJson["level_req"], primaryWeaponJson["attack_rate"], primaryWeaponJson["accuracy"]);


			Player player(playerJson["name"], playerJson["health"], playerJson["exp"], playerJson["level"], playerJson["inventory_space"],
				playerJson["dps"], playerJson["armour"], primaryWeapon);

			players.push_back(player);
		}


		return players;
	}




}