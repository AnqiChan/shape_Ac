#ifndef LAYER_OBJECT_H
#define LAYER_OBJECT_H

#include <QWidget>
#include <QPainter>
#include "create_map.h"

class LayerObject : public QWidget
//manages the display of the shape resource layer
{
    Q_OBJECT
public:
    LayerObject(int n_row, int n_col, Map *ori_map, QWidget *parent = nullptr);
    ~LayerObject();
    void display_layer_object();
public slots:
    void update_layer_object();

private:
    int n_row, n_col;
    Map *ori_map;
    // Cell ***map;
    QLabel ***map_object;
    QGridLayout *gridLayout;
    void paintEvent(QPaintEvent *event) override ;
};

#endif // LAYER_OBJECT_H
