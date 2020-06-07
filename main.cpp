#include <iostream>
#include <memory>
#include <ncurses.h>
#include <unistd.h>

#include "Board.h"

enum
{
    ANT_FULL_HEALTH = 1, ANT_MIDDLE_HEALTH, ANT_LOW_HEALTH, DEAD, FOOD, BLOCKADE, MONSTER
};

void RefreshScreen(std::unique_ptr<Board>& board);
void PauseGame(std::unique_ptr<Board>& board);

int main()
{
    srand(time(NULL));

    initscr();
    curs_set(0);

    start_color();
    init_pair(ANT_FULL_HEALTH, COLOR_GREEN, COLOR_BLACK);
    init_pair(ANT_MIDDLE_HEALTH, COLOR_YELLOW, COLOR_BLACK);
    init_pair(ANT_LOW_HEALTH, COLOR_RED, COLOR_BLACK);
    init_pair(DEAD, COLOR_BLACK, COLOR_BLACK);
    init_pair(FOOD, COLOR_WHITE, COLOR_BLACK);
    init_pair(BLOCKADE, COLOR_MAGENTA, COLOR_MAGENTA);
    init_pair(MONSTER, COLOR_CYAN, COLOR_BLACK);

    int windowHeight, windowWidth;
    getmaxyx(stdscr, windowHeight, windowWidth);

    std::unique_ptr<Board> board(new Board(windowHeight, windowWidth));
    try
    {
        board->InitBlockades(10, 15);
        board->InitFoods(20);
        board->InitAnts(10, 100.0);
        board->InitMonsters(5);
    }
    catch(const std::exception& e)
    {
        std::cerr << e.what() << '\n';
        return -1;
    }

    std::thread refreshScreenThread(RefreshScreen, std::ref(board));
    std::thread pauseGameThread(PauseGame, std::ref(board));
    
    refreshScreenThread.join();
    pauseGameThread.join();

    endwin();
    return 0;
}

void RefreshScreen(std::unique_ptr<Board>& board)
{
    const std::vector<std::vector<std::unique_ptr<GameObject>>>& boardFields = board->GetBoardFields();
    std::vector<std::vector<std::mutex>>& moveMutexes = board->GetMoveMutexes();

    while(true)
    {
        clear();
        for(int i = 0; i < board->GetHeight(); i++)
        {
            for(int j = 0; j < board->GetWidth(); j++)
            {
                moveMutexes[i][j].lock();
                if(boardFields[i][j])
                {
                    Ant* ant = dynamic_cast<Ant*>(boardFields[i][j].get());
                    if(ant)
                    {
                        float health = ant->GetHealth();
                        float maxHealth = ant->GetMaxHealth();
                        if(health >= 0.75 * maxHealth)
                            attron(COLOR_PAIR(ANT_FULL_HEALTH));
                        else if (health >= 0.25 * maxHealth)
                            attron(COLOR_PAIR(ANT_MIDDLE_HEALTH));
                        else if (health > 0)
                            attron(COLOR_PAIR(ANT_LOW_HEALTH));
                        else
                           attron(COLOR_PAIR(DEAD));
                    }

                    Food* food = dynamic_cast<Food*>(boardFields[i][j].get());
                    if(food)
                        attron(COLOR_PAIR(FOOD));

                    Blockade* blockade = dynamic_cast<Blockade*>(boardFields[i][j].get());
                    if(blockade)
                        attron(COLOR_PAIR(BLOCKADE));

                    Monster* monster = dynamic_cast<Monster*>(boardFields[i][j].get());
                    if(monster)
                    {
                        if(monster->IsAlive())
                            attron(COLOR_PAIR(MONSTER));
                        else
                            attron(COLOR_PAIR(DEAD));
                    }
                
                    mvprintw(i, j, boardFields[i][j]->GetSymbol());
                }
                moveMutexes[i][j].unlock();
            } 
        }
        refresh();
        usleep(300000);
    }
}

void PauseGame(std::unique_ptr<Board>& board)
{
    while(true) {
        getch();
        board->TogglePause();

        usleep(500000);
    }
}