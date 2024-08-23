#include "layer_object.h"

LayerObject::LayerObject(int n_row, int n_col, Map *ori_map, QWidget *parent) : QWidget(parent)
{
    this->n_row = n_row;
    this->n_col = n_col;
    this->ori_map = ori_map;
    this->gridLayout = nullptr;


    connect(ori_map, &Map::update_layer_object, this, &LayerObject::update_layer_object);
    map_object = new QLabel **[n_row];
    for (int i = 0; i < n_row; ++i)
    {
        map_object[i] = new QLabel *[n_col];
        for (int j = 0; j < n_col; ++j)
        {
            map_object[i][j] = new QLabel();
        }
    }
    display_layer_object();
}

LayerObject::~LayerObject()
{
    for (int i = 0; i < n_row; ++i)
    {
        for (int j = 0; j < n_col; ++j)
        {
            delete map_object[i][j];
        }
        delete[] map_object[i];
    }
    delete[] map_object;
    delete gridLayout;
}

void LayerObject::display_layer_object()
{
    this->setFixedSize(n_col * CELL_SIZE, n_row * CELL_SIZE);
    gridLayout = new QGridLayout;
    setLayout(gridLayout);
    gridLayout->setSpacing(0);
    gridLayout->setContentsMargins(0, 0, 0, 0);
    for (int i = 0; i < n_row; ++i)
    {
        for (int j = 0; j < n_col; j++)
        {
            map_object[i][j]->setFixedSize(CELL_SIZE, CELL_SIZE);

            // shapes
            if (ori_map->map[i][j]->shape_type == 1)
                map_object[i][j]->setPixmap(QPixmap(":/shapes/imgs/circle.png").scaled(CELL_SIZE, CELL_SIZE));
            else if (ori_map->map[i][j]->shape_type == 2)
                map_object[i][j]->setPixmap(QPixmap(":/shapes/imgs/rectangle.png").scaled(CELL_SIZE, CELL_SIZE));

            // conveyer_belt
            else if (ori_map->map[i][j]->tool_type == 1||ori_map->map[i][j]->tool_type == 4)
            {
                int dir = ori_map->map[i][j]->tool_dir;
                QPixmap pic;
                // special case judgement for turnings
                if ((ori_map->map[i + (dir % 2) * (2 - dir)][j + (1 - dir % 2) * (1 - dir)]->tool_type == 1||ori_map->map[i + (dir % 2) * (2 - dir)][j + (1 - dir % 2) * (1 - dir)]->tool_type == 4||ori_map->map[i + (dir % 2) * (2 - dir)][j + (1 - dir % 2) * (1 - dir)]->tool_type == 9) && (ori_map->map[i + (dir % 2) * (2 - dir)][j + (1 - dir % 2) * (1 - dir)]->tool_dir - dir + 8) % 4 == 3)
                {
                    pic = QPixmap(":/tools/imgs/conveyor_c.png").scaled(CELL_SIZE, CELL_SIZE);
                }
                else if ((ori_map->map[i - (dir % 2) * (2 - dir)][j - (1 - dir % 2) * (1 - dir)]->tool_type == 1||ori_map->map[i - (dir % 2) * (2 - dir)][j - (1 - dir % 2) * (1 - dir)]->tool_type == 4||ori_map->map[i - (dir % 2) * (2 - dir)][j - (1 - dir % 2) * (1 - dir)]->tool_type == 9) && (ori_map->map[i - (dir % 2) * (2 - dir)][j - (1 - dir % 2) * (1 - dir)]->tool_dir - dir + 8) % 4 == 1)
                {
                    pic = QPixmap(":/tools/imgs/conveyor_cc.png").scaled(CELL_SIZE, CELL_SIZE);
                }
                else
                {
                    pic = QPixmap(":/tools/imgs/conveyor.png").scaled(CELL_SIZE, CELL_SIZE);
                }
                QTransform transform;
                transform.rotate(dir * 90);
                pic = pic.transformed(transform);
                map_object[i][j]->setPixmap(pic);
            }
            else
                map_object[i][j]->clear();
            gridLayout->addWidget(map_object[i][j], i, j);
        }
    }
}

void LayerObject::update_layer_object()
{
    delete gridLayout; //
    display_layer_object();
}

void LayerObject::paintEvent(QPaintEvent *event){
    QPainter painter(this);

    // 设置网格线条的样式
    QPen pen(Qt::gray, 1, Qt::DashLine);
    painter.setPen(pen);

    // 获取窗口大小
    int width = this->width();
    int height = this->height();

    // 绘制水平线
    for (int i = 0; i <= width; i += CELL_SIZE) { // 假设网格间隔为50像素
        painter.drawLine(i, 0, i, height);
    }

    // 绘制垂直线
    for (int i = 0; i <= height; i += CELL_SIZE) {
        painter.drawLine(0, i, width, i);
    }
}
