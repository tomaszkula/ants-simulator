#ifndef GAMEOBJECT_H
#define GAMEOBJECT_H

#include <ostream>

class GameObject
{
public:
    GameObject();
    virtual const char* GetSymbol() const;

    friend std::ostream& operator<<(std::ostream& os, const GameObject& go);
};

#endif // GAMEOBJECT_H