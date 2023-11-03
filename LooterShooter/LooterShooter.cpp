#include <windows.h>
#include <iostream>
#include <string>
#include "Player.h"
#include "GameFunctions.h"
#include "Weapon.h"
#include "Armour.h"

int main()
{

    while (true) {
        Player player = GameFunctions::gameStart();
        for (int i = 0; i < 10; i++) {
            Armour armour = Armour::createArmour(&player, true);
            std::cout  << armour.toString() << std::endl;
        }
        system("pause");
        GameFunctions::gameLoop(&player);
    }


}

