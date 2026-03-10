#pragma once

#include "../Engine/core.h"

////////////////////
//--GAME INCLUDE--//
///////////////////
#define RAIL_LENGHT 150.f

#include "Scenes/MainScene.h"
#include "Scenes/MainMenuScene.h"
#include "DeathMenu.h"

#define HALF_RAIL_LENGT RAIL_LENGHT/2

#include "Player.h"
#include "PlayerController.h"
#include "GameController.h"
#include "GameMap.h"
#include "Gun.h"
#include "Enemy.h"
#include "Ally.h"
#include "MobMap.h"


#define WOOD_DEPTH_SIZE 0.35f
#define OFFSET_BETWEEN_WOODS 1.f
#define WOOD_NUMBER RAIL_LENGHT / OFFSET_BETWEEN_WOODS

#define NB_MOBS 6
#define NB_ALLIES 2