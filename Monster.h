#ifndef MONSTER_H
#define MONSTER_H

#include "Creature.h"

class Monster : public Creature
{
public:
    Monster(float);
    const char * GetSymbol() const final;
    void Move(int&, int&, int, int) const final;
    bool CanMove(std::unique_ptr<GameObject>&) const final;
    void Interact(std::unique_ptr<GameObject>&) final;
};

#endif // MONSTER_H