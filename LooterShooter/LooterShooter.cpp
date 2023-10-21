#include <windows.h>
#include <iostream>
#include <string>
#include "Player.h"
#include "GameFunctions.h"
#include "Weapon.h"

int main()
{
    Player player("Henry", 20.0, 66.6, 10, 50, 12.2, 12.10);
    Weapon weapon = Weapon::createWeapon(player, Weapon::WeaponType::AssaultRifle, false);
    player.equipPrimaryWeapon(&weapon);
    //GameFunctions::savePlayerData(&player);
    GameFunctions::gameStart();

}

