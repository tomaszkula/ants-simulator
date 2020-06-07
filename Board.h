#ifndef BOARD_H
#define BOARD_H

#include <algorithm>
#include <iostream>
#include <memory>
#include <mutex>
#include <thread>
#include <unistd.h>
#include <vector>

#include "GameObject.h"
#include "Creature.h"
#include "Ant.h"
#include "Monster.h"
#include "Food.h"
#include "Blockade.h"

class Board
{
private:
    int height, width;
    std::vector<std::vector<std::unique_ptr<GameObject>>> boardFields;
    bool pause;

    std::vector<std::thread> threads;
    std::vector<std::vector<std::mutex>> moveMutexes;

public:
    Board(int, int);
    ~Board();
    void TogglePause();
    void InitBlockades(int, int);
    void InitFoods(int);
    void InitAnts(int, float);
    void InitMonsters(int);
    void MoveThread(GameObject*, int, int);
    void HitThread(GameObject*);

    std::unique_ptr<GameObject>& GetFreeRandomSeat(int&, int&);
    const std::vector<std::vector<std::unique_ptr<GameObject>>>& GetBoardFields() const;
    const int GetHeight() const;
    const int GetWidth() const;
    std::vector<std::vector<std::mutex>>& GetMoveMutexes();
};

#endif // BOARD_H