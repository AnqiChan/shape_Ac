#include "layer_machinery.h"

LayerMachinery::LayerMachinery(int n_row, int n_col, Map *ori_map, QWidget *parent) : QWidget(parent)
{
    this->n_row = n_row;
    this->n_col = n_col;
    this->ori_map = ori_map;
    connect(ori_map, &Map::update_layer_machinery, this, &LayerMachinery::update_layer_machinery);
    map_machinery = new QLabel **[n_row];
    for (int i = 0; i < n_row; ++i)
    {
        map_machinery[i] = new QLabel *[n_col];
        for (int j = 0; j < n_col; ++j)
        {
            map_machinery[i][j] = new QLabel();
        }
    }
    this->setFixedSize(n_col * CELL_SIZE, n_row * CELL_SIZE);
    gridLayout=nullptr;
    display_layer_machinery();
}

void LayerMachinery::display_layer_machinery()
//display the machinery layer
{
    gridLayout = new QGridLayout;
    setLayout(gridLayout);
    gridLayout->setSpacing(0);
    gridLayout->setContentsMargins(0, 0, 0, 0);
    for (int i = 0; i < n_row; ++i)
        for (int j = 0; j < n_col; j++)
            map_machinery[i][j]->clear();
    for (int i = 0; i < n_row; ++i)
    {
        for (int j = 0; j < n_col; j++)
        {
            map_machinery[i][j]->setFixedSize(CELL_SIZE, CELL_SIZE);
            QPixmap pic;
            QPixmap pic2;
            QTransform transform;
            switch (ori_map->map[i][j]->tool_type)
            {
            case 2:
                pic = QPixmap(":/tools/imgs/miner.png").scaled(CELL_SIZE, CELL_SIZE);
                transform.rotate(ori_map->map[i][j]->tool_dir * 90);
                pic = pic.transformed(transform);
                map_machinery[i][j]->setPixmap(pic);
                break;
            case 3:
                pic = QPixmap(":/tools/imgs/trash.png").scaled(CELL_SIZE, CELL_SIZE);
                map_machinery[i][j]->setPixmap(pic);
                break;
            case 4:
                pic = QPixmap(":/tools/imgs/cutter_main.png").scaled(CELL_SIZE, CELL_SIZE);
                transform.rotate(ori_map->map[i][j]->tool_dir * 90);
                pic = pic.transformed(transform);
                pic2 = QPixmap(":/tools/imgs/cutter_side.png").scaled(CELL_SIZE, CELL_SIZE);
                pic2 = pic2.transformed(transform);
                map_machinery[i][j]->setPixmap(pic);
                switch (ori_map->map[i][j]->tool_dir)
                {
                case 0:
                    map_machinery[i][j + 1]->setPixmap(pic2);
                    break;
                case 1:
                    map_machinery[i + 1][j]->setPixmap(pic2);
                    break;
                case 2:
                    map_machinery[i][j - 1]->setPixmap(pic2);
                    break;
                case 3:
                    map_machinery[i - 1][j]->setPixmap(pic2);
                    break;
                default:
                    break;
                }
                break;
            case 5:
                pic = QPixmap(":/tools/imgs/spinner.png").scaled(CELL_SIZE, CELL_SIZE);
                transform.rotate(ori_map->map[i][j]->tool_dir * 90);
                pic = pic.transformed(transform);
                map_machinery[i][j]->setPixmap(pic);
                break;
            case 13:
                pic = QPixmap(":/tools/imgs/receiver.png").scaled(CELL_SIZE, CELL_SIZE);
                map_machinery[i][j]->setPixmap(pic);
                break;
            default:
                break;
            }
            gridLayout->addWidget(map_machinery[i][j], i, j);
        }
    }
}

void LayerMachinery::update_layer_machinery()
//update the machinery layer
{
    delete gridLayout;
    display_layer_machinery();
}

LayerMachinery::~LayerMachinery()
{
    for (int i = 0; i < n_row; ++i)
    {
        for (int j = 0; j < n_col; ++j)
        {
            delete map_machinery[i][j];
        }
        delete[] map_machinery[i];
    }
    delete[] map_machinery;
    delete gridLayout;
}
