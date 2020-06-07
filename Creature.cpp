#include "Creature.h"

Creature::Creature(float maxHealth) : GameObject()
{
    this->health = maxHealth;
    this->maxHealth = maxHealth;
}

void Creature::TakeHealth(float health)
{
    this->health -= health;
}

bool Creature::IsAlive() const
{
    return health > 0;
}

float Creature::GetHealth() const
{
    return health;
}

float Creature::GetMaxHealth() const
{
    return maxHealth;
}

std::mutex& Creature::GetHealthMutex()
{
    return healthMutex;
}