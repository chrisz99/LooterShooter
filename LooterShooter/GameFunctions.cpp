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
#include "Weapon.h"


namespace GameFunctions {

	void invalidCommand(std::string errorText = "Invalid Command!", bool clearConsole = false) {

		if (clearConsole)
			system("cls");

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


	Player GameFunctions::createCharacterHelper() {
		//Init Player Vars
		bool creatingCharacter = true;
		std::string playerName;
		Weapon::WeaponType weaponType;

		while (creatingCharacter) {
			system("cls");
			std::cout << "What's your characters name?" << std::endl;
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
		newPlayer.equipPrimaryWeapon(starterWeapon);

		newPlayer.refreshPlayerEquipment();

		newPlayer.refreshPlayerInventory();

		//Save Player Data
		GameFunctions::savePlayerData(&newPlayer);

		//Create and Return new player object
		std::cout << "New player " << newPlayer.getPlayerName() << " successfully created!\n"
			<< newPlayer << "\n" << newPlayer.getPrimaryWeapon() << std::endl;
		return newPlayer;

	
	}

	


	Player GameFunctions::gameStart() {

		std::vector<Player> playerList;
		bool characterSelected = false;

		std::cout << "Welcome to the wasteland, Press any key to select or create a character." << std::endl;
		system("pause");
		

		//Check if there is players available
		while (!characterSelected) {

			//init player list
			 playerList = loadPlayers();

			//If Characters are found -> select one
			if (!playerList.empty()) {
				system("cls");
				std::cout << "Select a Character Survivor! Commands { DELETE 'INDEX' }" << std::endl << std::endl;
				for (int i = 0; i < playerList.size(); i++)
					std::cout << i << ".) " << playerList[i] << std::endl << std::endl;
				std::cout << playerList.size() << ".)" << " Create a character." << std::endl;
				std::string initialCommand;
				if (std::getline(std::cin, initialCommand) && !initialCommand.empty()) {
					std::istringstream iss(initialCommand);
					std::string command;
					int index;


					if (iss >> command && !command.empty()) {
						std::transform(command.begin(), command.end(), command.begin(), ::toupper);

						if (command == "DELETE") {
							if (iss >> index && index < playerList.size() && index >= 0) {
								system("cls");
								colorPrint("Player:\n%s\nSuccessfully Deleted!", FOREGROUND_GREEN, playerList[index].toString().c_str());
								deletePlayerData(&playerList[index]);
								system("pause");
							}
							else
								invalidCommand("Invalid Index Specified!", true);
						}
						else
						{
							
							if (std::stoi(command) < playerList.size() && std::stoi(command) >= 0) {
								return playerList[std::stoi(command)];
							}
							else if (std::stoi(command) == playerList.size())
								return createCharacterHelper();
							else
								invalidCommand();
							
						}
					
					}
				

				}
			}
			//No Characters Found
			else 
				return createCharacterHelper();


		}


		return playerList[0];
		
	}

	void debugSpawnWeapon(Player* player) {
		Weapon w = Weapon::createWeapon(*player, Weapon::WeaponType::AssaultRifle, false);
		bool added = player->addItem(&w);
		if (added) {
			colorPrint("Weapon\n%s\nAdded to player Inventory!", FOREGROUND_GREEN, w.toString().c_str());
			system("pause");
		}
	}

	

	bool GameFunctions::gameLoop(Player* selectedPlayer) {
		bool playerPlaying = true;

		while (playerPlaying) {
			system("cls");
			colorPrint("%s\n", GameFunctions::DEFAULT_COLOR, selectedPlayer->toString().c_str());
			colorPrint("What's the move boss?", FOREGROUND_BLUE);
			colorPrint("1. Manage Inventory\n2. Debug Spawn Weapon\n3. Log Out", FOREGROUND_GREEN);
			int selectedIndex = 0;
			if (std::cin >> selectedIndex && selectedIndex > 0 && selectedIndex < 4) {
				switch (selectedIndex) {
				case 1: selectedPlayer->managePlayerInventory();
					break;
				case 2: 
					debugSpawnWeapon(selectedPlayer);
					break;
				case 3: {
					playerPlaying = false;
					system("cls");
					break;
				}
					
				}
			}
			else
			{
				colorPrint("Invalid option!", FOREGROUND_RED);
			}
		}

		return playerPlaying;

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


	//Helper JSON Functs

	//Player to JSON
	nlohmann::json GameFunctions::playerToJson(Player* player) {
		//Create and populate the player JSON object
		nlohmann::json playerJson;
		nlohmann::json primaryWeapon = weaponToJson(&player->getPrimaryWeapon());
		nlohmann::json playerInventory = inventoryToJson(player);

		playerJson = nlohmann::json{
			{"name", player->getPlayerName()},
			{"health", player->getPlayerHealth()},
			{"exp", player->getPlayerExp()},
			{"level", player->getPlayerLevel()},
			{"inventory_space", player->getPlayerInventorySpace()},
			{"dps", player->getPlayerDPS()},
			{"armour", player->getPlayerArmour()},
			{"primary_weapon", primaryWeapon},
			{"inventory", playerInventory}
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

	nlohmann::json GameFunctions::inventoryToJson(Player* player) {
		nlohmann::json inventoryJson;

		if (player->getPlayerInventory()->weaponInventory.empty())
			return inventoryJson;

		for (auto& weapon : player->getPlayerInventory()->weaponInventory) {
			inventoryJson["weapons"].push_back(weaponToJson(&weapon));
		}

		return inventoryJson;
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

	void GameFunctions::deletePlayerData(Player* player) {

		bool playerFound = false;

		std::wstring documentsPath = getDocumentsPath();

		//Define File Paths
		std::wstring gameFolderPath = documentsPath + L"\\LooterShooter1_0";
		std::wstring playerFilePath = gameFolderPath + L"\\game_data.json";

		//Grab Previous Saves
		nlohmann::json prevSave = nlohmann::json::array();

		//Open Player File
		std::ifstream playerFileIn(playerFilePath);

		//Dump contents in Prev Save
		if (playerFileIn.is_open()) {
			playerFileIn >> prevSave;

		}

		//Iterate prev save, try and find existing player
		for (auto it = prevSave.begin(); it != prevSave.end(); ++it) {
			if ((*it)["name"] == player->getPlayerName()) {
				playerFound = true;
				prevSave.erase(it);
				break;
			}
		}

		if (playerFound) {
			// Open the file for writing
			std::ofstream playerFileOut(playerFilePath);
			if (playerFileOut.is_open()) {
				// Write the modified JSON back to the file
				playerFileOut << prevSave.dump(4); // You can format the output as needed
			}
		}

	}

	//Save Player Data 
	bool GameFunctions::savePlayerData(Player* player) {

		bool playerFound = false;

		std::wstring documentsPath = getDocumentsPath();
		
		//Define File Paths
		std::wstring gameFolderPath = documentsPath + L"\\LooterShooter1_0";
		std::wstring playerFilePath = gameFolderPath + L"\\game_data.json";

		//Create Game Save Folder -> If failed throw error
		if (!CreateDirectoryW(gameFolderPath.c_str(), NULL) && GetLastError() != ERROR_ALREADY_EXISTS) {
			std::cerr << "Failed to create game save folder!" << std::endl;
			return false;
		}

		//Convert Player Object to JSON
		nlohmann::json playerJson = playerToJson(player);

		//Grab Previous Saves
		nlohmann::json prevSave = nlohmann::json::array();

		//Open Player File
		std::ifstream playerFileIn(playerFilePath);

		//Dump contents in Prev Save
		if (playerFileIn.is_open()) {
			playerFileIn >> prevSave;

		}

		//Iterate prev save, try and find existing player
		for (auto& p : prevSave) {
			if (player->getPlayerName() == p["name"]) {
				p = playerJson;
				playerFound = true;
			 }
		}
		
		//If not found, add player
		if(!playerFound)
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

	Player::Inventory GameFunctions::jsonToInventory(nlohmann::json inventoryJson) {
		Player::Inventory loadedInventory;
		for (const auto& weapon : inventoryJson["weapons"]) {
	
			Weapon loadedWeapon(weapon["name"], weapon["weaponType"], weapon["damage"], weapon["level_req"], weapon["attack_rate"], weapon["accuracy"]);
			loadedInventory.weaponInventory.push_back(loadedWeapon);
		}
		return loadedInventory;
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
	
		try {
			playerFile >> playerArray;
		}
		catch (const nlohmann::json::exception& e) {
			playerFile.close();
			return players;
		}
	

		for (const auto& playerJson : playerArray) {
			nlohmann::json primaryWeaponJson = playerJson["primary_weapon"];
			Player::Inventory playerInventory = jsonToInventory(playerJson["inventory"]);
	

			Weapon primaryWeapon = Weapon(primaryWeaponJson["name"], primaryWeaponJson["weaponType"], primaryWeaponJson["damage"],
				primaryWeaponJson["level_req"], primaryWeaponJson["attack_rate"], primaryWeaponJson["accuracy"]);


			Player player(playerJson["name"], playerJson["health"], playerJson["exp"], playerJson["level"], playerJson["inventory_space"],
				playerJson["dps"], playerJson["armour"], primaryWeapon, playerInventory);
			
			player.refreshPlayerEquipment();
			
			player.refreshPlayerInventory();

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