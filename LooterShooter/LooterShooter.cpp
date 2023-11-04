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

        GameFunctions::gameLoop(&player);
    }


}

