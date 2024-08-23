#ifndef UPGRADE_H
#define UPGRADE_H

#include <QWidget>
#include <QLabel>
#include <QGridLayout>
#include <QPushbutton>
#include "game_status.h"

class Upgrade : public QWidget
//the upgrade window, manages the upgrade of the player
{
    Q_OBJECT
public:
    Upgrade(GameStatus *status, QWidget *parent = nullptr);
    void open_upgrade();
    void close_upgrade();
    void change_state();

private:
    GameStatus *status;
    bool state = false;
    QGridLayout *layout;
    QLabel *player_info;
    QLabel *level;
    QLabel *money;
    QLabel *local;
    QLabel *global;
    QLabel *reward;
    QLabel *map_up;
    QPushButton *locals[3];
    QPushButton *globals[4];
    QPushButton *map_ups[3];
    //int map_up_times[3]={0,0,0};
    void update_display();

private slots:
    void update_local1();
    void update_local2();
    void update_local3();
    void update_global1(int i);
    void update_global2();
    void update_global3();
    void update_global4();

signals:
    void larger_receiver();
    void more_shapes(int i);
    void larger_map();

};

#endif // UPGRADE_H
