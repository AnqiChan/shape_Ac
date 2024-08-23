#ifndef LAYER_MOVING_H
#define LAYER_MOVING_H

#include <QGraphicsScene>
#include <QGraphicsView>
#include <vector>
#include <QWidget>
#include <QGraphicsItem>
#include <QPropertyAnimation>
#include <QParallelAnimationGroup>
#include <QThread>
#include "game_status.h"
#include "create_map.h"
#include "cell.h"


class LayerMoving : public QWidget
//manages the display of the moving objects layer
{
    Q_OBJECT
public:
    LayerMoving(int n_row, int n_col, Map *ori_map, GameStatus *status, int clk_ori, QWidget *parent = nullptr);
    ~LayerMoving();
    void display_layer_moving();
    struct MovingItem
    {
        QLabel *item;
        int x;
        int y;
        int type;
        int dir;
    };
    struct GenItem
    {
        int x;
        int y;
        int type;
        int dir;
        int gen_state = 0;
    };
    std::vector<MovingItem> moving_items;
    std::vector<GenItem> gen_items;
    int **map_moving;
    int n_row, n_col;
private:
    Map *ori_map;
    GameStatus *status;
    int clk_ori;
    bool check_free(int x,int y,int dir);
    QParallelAnimationGroup *ani_nxt = nullptr;

signals:
    void update_score();
    void update_level();

public slots:
    void add_gen_item(int x, int y, int type, int dir, int gen_state = 0);
    void remove_gen_item(int x, int y);
    void check();
    void add_moving_item(int x, int y, int type, int dir, std::vector<MovingItem>::iterator *it = nullptr);
    void remove_moving_item(int index);
    void check_if_moving(int x,int y);
};

#endif // LAYER_MOVING_H
