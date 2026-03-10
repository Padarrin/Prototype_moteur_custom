#pragma once
#include "pch.h"
class Health : public Component
{
public:
	
	int health;
	int maxHealth;
	bool isDead = false;
};

