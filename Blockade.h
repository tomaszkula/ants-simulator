#ifndef BLOCKADE_H
#define BLOCKADE_H

#include "GameObject.h"

class Blockade : public GameObject
{
public:
    const char* GetSymbol() const final;
};

#endif // BLOCKADE_H