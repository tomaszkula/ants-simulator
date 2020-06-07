#ifndef FOOD_H
#define FOOD_H

#include "GameObject.h"

class Food : public GameObject
{
public:
    Food();
    const char* GetSymbol() const final;
};

#endif // FOOD_H