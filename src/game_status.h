#ifndef GAME_STATUS_H
#define GAME_STATUS_H

#include <vector>
#include <QObject>

class GameStatus : public QObject
//data of the game, including the level, money, tasks, upgrades etc
//enables further development of the games levels
{
    Q_OBJECT
public:
    GameStatus();
    ~GameStatus();
    int level;
    int money;

    struct task
    {
        int type;
        int goal;
        int done;
    };
    int ani_rate = 1;
    int cut_en = 2;
    int ext_en = 4;
    std::vector<std::vector<task>> Levels = {{{1, 10, 0}},
                                             {{1, 10, 0}, {2, 10, 0}},
                                             {{1, 10, 0}, {3, 20, 0}},
                                             {{1, 10, 0}, {2, 10, 0}, {3,25,0}},
                                             {{1,10,0}, {2,20,0}, {3,30,0},{4,20,0}}};
    bool enhanced[3] = {0, 0, 0}; // extractor, cutter, conveyor_belt
    int enhanceable=0;
    int shape_value[5];
    int map_up_times[3]={0,0,0};
    bool check_level();
    void check_enhance();
    void update_clk();

signals:
    void update_clk_sig(int r);
    void update_bg();

private:
};

#endif // GAME_STATUS_H
