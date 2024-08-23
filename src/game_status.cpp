#include "game_status.h"

#include <QDebug>

GameStatus::GameStatus()
{
    level = 0;
    money = 0;
    shape_value[0] = 0;
    shape_value[1] = 1;
    shape_value[2] = 1;
    shape_value[3] = 2;
    shape_value[4] = 3;
    ani_rate=1;
    cut_en=2;
    ext_en=4;
    enhanceable=0;
    enhanced[0]=0;
    enhanced[1]=0;
    enhanced[2]=0;
    map_up_times[0]=0;
    map_up_times[1]=0;
    map_up_times[2]=0;
}

bool GameStatus::check_level()
//check if the player has finished the current level
{
    for (auto i : Levels[level])
    {
        if (i.done < i.goal)
            return false;
    }
    level = (level + 1) % Levels.size();

    //empty task done and restore default for local reinforcements
    for (auto &i : Levels[level])
    {
        i.done = 0;
    }
    ani_rate = 1;
    update_clk();
    cut_en = 2;
    ext_en = 4;
    for (int i = 0; i < 3; i++)
    {
        enhanced[i] = false;
    }
    enhanceable=0;
    emit update_bg();
    return true;
}

void GameStatus::check_enhance()
//check if local reinforcements are available
{
    enhanceable=0;
    for (auto i : Levels[level])
    {
        if (i.done == i.goal)
            enhanceable++;
    }
    for(int i=0;i<3;i++)
    {
        if(enhanced[i]==true)
            enhanceable--;
    }
}

void GameStatus::update_clk()
//update the clock signal
{
    emit update_clk_sig(ani_rate);
}

GameStatus::~GameStatus() {}
