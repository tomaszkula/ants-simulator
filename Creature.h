#ifndef CREATURE_H
#define CREATURE_H

#include <memory>
#include <mutex>

#include "GameObject.h"
#include "Food.h"
#include "Blockade.h"

class Creature : public GameObject
{
protected:
    float health, maxHealth;
    std::mutex healthMutex;

public:
    Creature(float);
    virtual void Move(int&, int&, int, int) const = 0;
    virtual bool CanMove(std::unique_ptr<GameObject>&) const = 0;
    virtual void Interact(std::unique_ptr<GameObject>&) = 0;
    void TakeHealth(float);

    bool IsAlive() const;
    float GetHealth() const;
    float GetMaxHealth() const;
    std::mutex& GetHealthMutex();
};

#endif // CREATURE_H