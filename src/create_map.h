#ifndef CREATE_MAP_H
#define CREATE_MAP_H
#include "cell.h"
#include "lr_button.h"

#include <QWidget>
#include <QGridLayout>
#include <QPushButton>
#include <QLabel>
#include <QPixmap>

#define ROW 24
#define COL 32

class Map : public QWidget
//manages the events happening to the map
{
    Q_OBJECT
public:
    Cell ***map;
    int n_row, n_col;
    Map(int n_row, int n_col, QWidget *parent = nullptr);
    ~Map();
    void display_map();
    int center_x, center_y, center_size;
    void static chose_tool(int index);
    int static tool_chosen;
    int static tool_dir;
    bool static stick;
signals:
    void update_layer_object();
    void update_layer_machinery();
    void add_gen_item(int x, int y, int type, int dir,int state = 0);
    void remove_gen_item(int x, int y);
    void remove_moving(int x,int y);
private slots:
    void update_map_add(int i, int j);
    void update_map_remove(int i, int j);
public slots:
    void larger_receiver();
    void more_shapes(int i);
private:
    LRbutton ***buttons;
    QGridLayout *gridLayout;
    void shape_gen(int i);
};

#endif // CREATE_MAP_H
