#include "layer_info.h"

LayerInfo::LayerInfo(int n_row, int n_col, Map *ori_map, GameStatus *status, QWidget *parent)
{
    this->n_row = n_row;
    this->n_col = n_col;
    this->ori_map = ori_map;
    this->status = status;
    this->setFixedSize(n_col * CELL_SIZE, n_row * CELL_SIZE);
    score_label=nullptr;
    layout_score=nullptr;
    level_label=nullptr;
    display_score();
}

void LayerInfo::display_score()
//display the number of shapes the receiver has received
{
    score_label = new QLabel(this);
    score_label->setGeometry(ori_map->center_y * CELL_SIZE, ori_map->center_x * CELL_SIZE, ori_map->center_size * CELL_SIZE, ori_map->center_size * CELL_SIZE);

    layout_score = new QGridLayout;
    layout_score->setColumnStretch(0, 1);
    layout_score->setColumnStretch(1, 2);
    score_label->setLayout(layout_score);

    auto task_list = status->Levels[status->level];
    int task_num = task_list.size();
    QLabel **task = new QLabel *[task_num];
    QLabel **task_pic = new QLabel *[task_num];

    level_label = new QLabel(this);
    level_label->setText("Level " + QString::number(status->level + 1));
    layout_score->addWidget(level_label, 0, 0);

    for (int i = 0; i < task_num; i++)
    {
        task_pic[i] = new QLabel(score_label);
        switch (task_list[i].type)
        {
        case 1:
            task_pic[i]->setPixmap(QPixmap(":/shapes/imgs/circle.png").scaled(CELL_SIZE * 0.7, CELL_SIZE * 0.7));
            break;
        case 2:
            task_pic[i]->setPixmap(QPixmap(":/shapes/imgs/rectangle.png").scaled(CELL_SIZE * 0.7, CELL_SIZE * 0.7));
            break;
        case 3:
            task_pic[i]->setPixmap(QPixmap(":/shapes/imgs/rectangle_half.png").scaled(CELL_SIZE * 0.7, CELL_SIZE * 0.7));
            break;
        case 4:
            task_pic[i]->setPixmap(QPixmap(":/shapes/imgs/rectangle_quarter.png").scaled(CELL_SIZE * 0.7, CELL_SIZE * 0.7));
            break;
        default:
            break;
        }
        layout_score->addWidget(task_pic[i], i + 1, 0);

        task[i] = new QLabel(score_label);
        task[i]->setText(QString::number(task_list[i].done) + "/" + QString::number(task_list[i].goal));
        layout_score->addWidget(task[i], i + 1, 1);
    }

    score_label->setStyleSheet("QLabel {  color : black; }");
    score_label->show();
}

LayerInfo::~LayerInfo()
{
    if (level_label != nullptr)
    {
        delete level_label;
        level_label = nullptr;
    }
    if (layout_score != nullptr)
    {
        delete layout_score;
        layout_score = nullptr;
    }
    if (score_label != nullptr)
    {
        delete score_label;
        score_label = nullptr;
    }
}

void LayerInfo::update_layer_info_score()
//update the number of shapes the receiver has received
{
    if (level_label != nullptr)
    {
        delete level_label;
        level_label = nullptr;
    }
    if (layout_score != nullptr)
    {
        delete layout_score;
        layout_score = nullptr;
    }
    if (score_label != nullptr)
    {
        delete score_label;
        score_label = nullptr;
    }
    display_score();
}
