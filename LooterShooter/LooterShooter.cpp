#include <windows.h>
#include <iostream>
#include <string>
#include "Player.h"
#include "GameFunctions.h"
#include "Weapon.h"

int main()
{


    Player player = GameFunctions::gameStart();
    GameFunctions::gameLoop(&player);

}

