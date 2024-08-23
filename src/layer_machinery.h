#ifndef LAYER_MACHINERY_H
#define LAYER_MACHINERY_H

#include <QWidget>
#include "create_map.h"

class LayerMachinery : public QWidget
//manages the display of the machinery layer
{
    Q_OBJECT
public:
    LayerMachinery(int n_row, int n_col, Map *ori_map, QWidget *parent = nullptr);
    ~LayerMachinery();
    void display_layer_machinery();

public slots:
    void update_layer_machinery();

private:
    int n_row, n_col;
    Map *ori_map;
    QLabel ***map_machinery;
    QGridLayout *gridLayout;
};

#endif // LAYER_MACHINERY_H
