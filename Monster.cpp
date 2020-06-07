#include "Monster.h"

Monster::Monster(float maxHealth) : Creature(maxHealth)
{

}

const char* Monster::GetSymbol() const
{
    return "M";
}

void Monster::Move(int& x, int& y, int height, int width) const
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

bool Monster::CanMove(std::unique_ptr<GameObject>& go) const
{
    // if(dynamic_cast<Blockade*>(go.get()) || dynamic_cast<Monster*>(go.get()))
    //     return false;

    // return true;
    if(!go || dynamic_cast<Food*>(go.get()))
        return true;

    Creature* creature = dynamic_cast<Creature*>(go.get());
    if(creature)
       return true;

    return false;
}

void Monster::Interact(std::unique_ptr<GameObject>& go)
{
    if(dynamic_cast<Food*>(go.get()))
        go.reset();

    Creature* creature = dynamic_cast<Creature*>(go.get());
    if(creature && creature->IsAlive())
        creature->TakeHealth(creature->GetMaxHealth());
}