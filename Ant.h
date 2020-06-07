#ifndef ANT_H
#define ANT_H

#include "Creature.h"

class Ant : public Creature
{
public:
    Ant(float);
    const char * GetSymbol() const final;
    void Move(int&, int&, int, int) const final;
    bool CanMove(std::unique_ptr<GameObject>&) const final;
    void Interact(std::unique_ptr<GameObject>&) final;
};

#endif // ANT_H