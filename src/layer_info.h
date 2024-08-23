#ifndef LAYER_INFO_H
#define LAYER_INFO_H

#include <QWidget>
#include <QThread>
#include "create_map.h"
#include "game_status.h"

class LayerInfo : public QWidget
//manages the display the score and level information
{
    Q_OBJECT
public:
    LayerInfo(int n_row, int n_col, Map *ori_map, GameStatus *status, QWidget *parent = nullptr);
    ~LayerInfo();
    void display_score();
    void display_level();
public slots:
    void update_layer_info_score();
    // void update_layer_info_level();
private:
    GameStatus *status;
    int n_row, n_col;
    Map *ori_map;
    QLabel *score_label = nullptr;
    QGridLayout *layout_score = nullptr;
    QLabel *level_label = nullptr;
};

#endif // LAYER_INFO_H
