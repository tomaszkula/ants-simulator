#include "Board.h"

Board::Board(int height, int width)
{
    this->height = height;
    this->width = width;

    boardFields = std::vector<std::vector<std::unique_ptr<GameObject>>>(height);
    for(auto& row : boardFields)
        row = std::vector<std::unique_ptr<GameObject>>(width);

    moveMutexes = std::vector<std::vector<std::mutex>>(height);
    for(auto& row : moveMutexes)
        row = std::vector<std::mutex>(width);
}

Board::~Board()
{
    for(auto& t : threads)
       t.join();

    threads.clear();
}

void Board::TogglePause()
{
    pause = !pause;
}

void Board::InitBlockades(int count, int maxPerStack)
{
    for(int i = 0; i < count; i++)
    {
        int x, y;
        auto& seat = GetFreeRandomSeat(x, y);
        seat.reset(new Blockade());

        for(int j = 0; j < maxPerStack; j++)
        {
            int choice = rand() % 2;
            if(choice == 0)
                x += 1;
            else
                y += 1;

            if(x < 0 || x >= width || y < 0 || y >= height)
                break;

            boardFields[y][x].reset(new Blockade());
        }
    }
}

void Board::InitFoods(int count)
{
    for(int i = 0; i < count; i++)
    {
        int x, y;
        auto& seat = GetFreeRandomSeat(x, y);
        seat.reset(new Food());
    }
}

void Board::InitAnts(int count, float maxHealth)
{
    for(int i = 0; i < count; i++)
    {
        int x, y;
        auto& seat = GetFreeRandomSeat(x, y);
        seat.reset(new Ant(maxHealth));

        threads.push_back(std::thread(&Board::HitThread, this, boardFields[y][x].get()));
        threads.push_back(std::thread(&Board::MoveThread, this, boardFields[y][x].get(), x, y));
    }
}

void Board::InitMonsters(int count)
{
    for(int i = 0; i < count; i++)
    {
        int x, y;
        auto& seat = GetFreeRandomSeat(x, y);
        seat.reset(new Monster(1));

        threads.push_back(std::thread(&Board::MoveThread, this, boardFields[y][x].get(), x, y));
    }
}

void Board::MoveThread(GameObject* go, int x, int y)
{
    while(true)
    {
        while(!pause)
        {
            int xOld = x, yOld = y;
            if(moveMutexes[yOld][xOld].try_lock())
            {
                Creature* creature = dynamic_cast<Creature*>(go);
                if(creature && creature->GetHealthMutex().try_lock())
                {
                    if(!creature->IsAlive())
                    {
                        creature->GetHealthMutex().unlock();
                        moveMutexes[yOld][xOld].unlock();
                        return;
                    }

                    int xNew = x, yNew = y;
                    creature->Move(xNew, yNew, height, width);
                    if(moveMutexes[yNew][xNew].try_lock())
                    {
                        if(creature->CanMove(boardFields[yNew][xNew]))
                        {
                            creature->Interact(boardFields[yNew][xNew]);
                            boardFields[yNew][xNew].swap(boardFields[yOld][xOld]);
                            x = xNew;
                            y = yNew;
                        }
                        moveMutexes[yNew][xNew].unlock();
                    }
                    creature->GetHealthMutex().unlock();
                }
                moveMutexes[yOld][xOld].unlock();

                int r = rand() % 10 + 1;
                usleep(50000 * r);
            }
        }
    }
}

void Board::HitThread(GameObject* go)
{
    while(true)
    {
        while(!pause)
        {
            Creature* creature = dynamic_cast<Creature*>(go);
            float damage = rand() % (int)(creature->GetMaxHealth() + 1) / (creature->GetMaxHealth() * 2.0);
            if(creature->GetHealthMutex().try_lock())
            {
                if(!creature->IsAlive())
                {
                    creature->GetHealthMutex().unlock();
                    return;
                }
                
                creature->TakeHealth(damage);
                creature->GetHealthMutex().unlock();
                
                int r = rand() % 10 + 1;
                usleep(50000 * r);
            }
        }
    }
}

std::unique_ptr<GameObject>& Board::GetFreeRandomSeat(int& x, int& y)
{
    do
    {
        x = rand() % width;
        y = rand() % height;
    } while(boardFields[y][x]);

    return boardFields[y][x];
}

const std::vector<std::vector<std::unique_ptr<GameObject>>>& Board::GetBoardFields() const
{
    return boardFields;
}

const int Board::GetHeight() const
{
    return height;
}

const int Board::GetWidth() const
{
    return width;
}

std::vector<std::vector<std::mutex>>& Board::GetMoveMutexes()
{
    return moveMutexes;
}