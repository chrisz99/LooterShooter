#include <random>
#include <cmath>
#include <windows.h>
#include <shlobj.h>
#include <iostream>
#include <codecvt>
#include <fstream> // Include the necessary header for file operations
#include <sstream>
#include <iomanip>
#include <string>
#include "GameFunctions.h"


namespace GameFunctions {



	Player GameFunctions::createCharacterHelper() {
		//Init Player Vars
		bool creatingCharacter = true;
		std::string playerName;
		Weapon::WeaponType weaponType;

		while (creatingCharacter) {
			system("cls");
			std::cout << "What's your characters name?" << std::endl;
			std::cin.ignore(); 
			std::getline(std::cin, playerName);
			std::cout << "What type of weapon do you wish to start out with?" << std::endl <<
				std::endl << "1.) Assault Rifle\n" << "2.) Sniper\n" << "3.) Pistol\n" << "4.) Melee\n" << "5.) SMG" << std::endl;
			int weaponIndex;
			if (std::cin >> weaponIndex) {
				if (weaponIndex > 0 && weaponIndex <= 5) {
					weaponType = static_cast<Weapon::WeaponType>(weaponIndex - 1);
					creatingCharacter = false;
				}
				else {
					std::cerr << "Invalid weapon type selected, try again" << std::endl;
					system("pause");
				}
			}


		}

		//Create Player Object
		Player newPlayer(playerName, 100, 0, 1, 8, 0, 0);

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
				std::cout << playerList.size() << ".)" << " Create a character." << std::endl;
				int selectedIndex;
				if (std::cin >> selectedIndex) {
					if (selectedIndex < 0 || playerList.size() >= selectedIndex) {
						if (selectedIndex != playerList.size())
							return playerList[selectedIndex];
						else
							return createCharacterHelper();
					}
					else {
						std::cerr << "Invalid index for character selection" << std::endl;
						system("pause");
					}
				}
			}
			//No Characters Found
			else 
				return createCharacterHelper();


		}


		return playerList[0];
		
	}

	bool GameFunctions::gameLoop(Player* selectedPlayer) {
		while (true) {
			system("cls");
			colorPrint("%s\n", GameFunctions::DEFAULT_COLOR, selectedPlayer->toString().c_str());
			std::cout << "What's the move boss?" << std::endl;
			colorPrint("1. Manage Inventory ", FOREGROUND_BLUE);
			int selectedIndex = 0;
			if (std::cin >> selectedIndex && selectedIndex < 0 && selectedIndex < 4) {
				switch (selectedIndex) {
				case 1: selectedPlayer->managePlayerInventory();
					break;
				default:
					break;
				}
			}
			system("pause");
		}

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




	std::string GameFunctions::colorPrint(const char* format,int color, ...) {
		HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
		SetConsoleTextAttribute(hConsole, color);

		va_list args;
		va_start(args, format);

		// Determine the size of the formatted string
		int size = vsnprintf(nullptr, 0, format, args);

		// Allocate a buffer for the formatted string
		char* buffer = new char[size + 1];

		va_start(args, format);
		vsnprintf(buffer, size + 1, format, args);
		va_end(args);

		std::string result(buffer);
		delete[] buffer;

		std::cout << result << std::endl;

		SetConsoleTextAttribute(hConsole, 7);

		return result;
	}



}