#include "Ant.h"

Ant::Ant(float maxHealth) : Creature(maxHealth)
{

}

const char* Ant::GetSymbol() const
{
    return "A";
}

void Ant::Move(int& x, int& y, int height, int width) const
{
    int xOffset = rand() % 3 - 1;
    int yOffset = rand() % 3 - 1;

    int xNew = x + xOffset;
    int yNew = y + yOffset;
    if(xNew < 0 || xNew >= width || yNew < 0 || yNew >= height)
        return;

    x = xNew;
    y = yNew;
}

bool Ant::CanMove(std::unique_ptr<GameObject>& go) const
{
    if(!go || dynamic_cast<Food*>(go.get()))
        return true;

    Creature* creature = dynamic_cast<Creature*>(go.get());
    if(creature && !creature->IsAlive())
       return true;

    return false;
}

void Ant::Interact(std::unique_ptr<GameObject>& go)
{
    if(dynamic_cast<Food*>(go.get()))
    {
        health = maxHealth;
        go.reset();
    }
}