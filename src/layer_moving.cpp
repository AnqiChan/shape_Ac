#include "layer_moving.h"

LayerMoving::LayerMoving(int n_row, int n_col, Map *ori_map, GameStatus *status, int clk_ori, QWidget *parent) : QWidget(parent)
{
    this->status = status;
    this->clk_ori = clk_ori;
    this->n_row = n_row;
    this->n_col = n_col;
    map_moving = new int *[n_row];
    for (int i = 0; i < n_row; i++)
    {
        map_moving[i] = new int[n_col];
        for (int j = 0; j < n_col; j++)
        {
            map_moving[i][j] = 0;
        }
    }
    ani_nxt = new QParallelAnimationGroup;
    this->ori_map = ori_map;
    this->setFixedSize(n_col * CELL_SIZE, n_row * CELL_SIZE);
}

LayerMoving::~LayerMoving()
{
    for (int i = 0; i < n_row; i++)
    {
        delete[] map_moving[i];
    }
    delete[] map_moving;
    for (int i = 0; i < moving_items.size(); i++)
    {
        delete moving_items[i].item;
    }
}

void LayerMoving::add_gen_item(int x, int y, int type, int dir, int gen_state)
//add extractor to the moving item generator vector
{
    GenItem gen_item;
    gen_item.x = x;
    gen_item.y = y;
    gen_item.type = type;
    gen_item.dir = dir;
    gen_item.gen_state = 0;
    gen_items.push_back(gen_item);
}

void LayerMoving::remove_gen_item(int x, int y)
//remove extractor from the moving item generator vector
{
    for (int i = 0; i < gen_items.size(); i++)
    {
        if (gen_items[i].x == x && gen_items[i].y == y)
        {
            gen_items.erase(gen_items.begin() + i);
            return;
        }
    }
}

void LayerMoving::add_moving_item(int x, int y, int type, int dir, std::vector<MovingItem>::iterator *it)
//add moving item to the moving item vector
//if it is not nullptr, insert the item to the position pointed by it, else append it to the end
{
    MovingItem moving_item;
    moving_item.x = x;
    moving_item.y = y;
    moving_item.type = type;
    moving_item.dir = dir;
    QPixmap pic;
    QTransform transform;
    switch (type)
    {
    case 1:
        // circle
        moving_item.item = new QLabel(this);
        moving_item.item->setFixedSize(CELL_SIZE, CELL_SIZE);
        pic = (QPixmap(":/shapes/imgs/circle.png").scaled(CELL_SIZE, CELL_SIZE));
        moving_item.item->setPixmap(pic);
        moving_item.item->setGeometry(y * CELL_SIZE, x * CELL_SIZE, CELL_SIZE, CELL_SIZE);
        moving_item.item->show();
        break;
    case 2:
        // rectangle
        moving_item.item = new QLabel(this);
        moving_item.item->setFixedSize(CELL_SIZE, CELL_SIZE);
        pic = (QPixmap(":/shapes/imgs/rectangle.png").scaled(CELL_SIZE, CELL_SIZE));
        moving_item.item->setPixmap(pic);
        moving_item.item->setGeometry(y * CELL_SIZE, x * CELL_SIZE, CELL_SIZE, CELL_SIZE);
        moving_item.item->show();
        break;
    case 3:
        // half ractangle
        moving_item.item = new QLabel(this);
        moving_item.item->setFixedSize(CELL_SIZE, CELL_SIZE);
        pic = (QPixmap(":/shapes/imgs/rectangle_half.png").scaled(CELL_SIZE, CELL_SIZE));
        transform.rotate(90 * dir);
        pic = pic.transformed(transform);
        moving_item.item->setPixmap(pic);
        moving_item.item->setGeometry(y * CELL_SIZE, x * CELL_SIZE, CELL_SIZE, CELL_SIZE);
        moving_item.item->show();
        break;
    case 4:
        // quarter ractangle
        moving_item.item = new QLabel(this);
        moving_item.item->setFixedSize(CELL_SIZE, CELL_SIZE);
        pic = (QPixmap(":/shapes/imgs/rectangle_quarter.png").scaled(CELL_SIZE, CELL_SIZE));
        transform.rotate(90 * dir);
        pic = pic.transformed(transform);
        moving_item.item->setPixmap(pic);
        moving_item.item->setGeometry(y * CELL_SIZE, x * CELL_SIZE, CELL_SIZE, CELL_SIZE);
        moving_item.item->show();
        break;
    default:
        break;
    }
    map_moving[x][y] = 1;
    if (it == nullptr)
        moving_items.push_back(moving_item);
    else
        *it = moving_items.insert(*it + 1, moving_item);
}

void LayerMoving::remove_moving_item(int index)
//remove moving item from the moving item vector
{
    auto it=moving_items.begin()+index;
    moving_items.erase(it);
}

void LayerMoving::check_if_moving(int x,int y)
//check if there is a moving item on the cell, if there is, remove it
{
    for (auto it = moving_items.begin(); it != moving_items.end(); it++){
        if(it->x==x&&it->y==y){
            map_moving[x][y] = 0;
            delete it->item;
            it->item = nullptr;
            moving_items.erase(it);
            return;
        }
    }
}

void LayerMoving::check()
//get the animation group
{
    int ani_time = clk_ori / status->ani_rate - 20;

    // remove trash and receiver,spin through spinner
    for (auto it = moving_items.begin(); it != moving_items.end(); /* no increment here */)
    {
        int x = it->x;
        int y = it->y;
        if (ori_map->map[x][y]->tool_type == 3)
        {
            map_moving[x][y] = 0;
            delete it->item;
            it->item = nullptr;
            it = moving_items.erase(it);
        }
        else if (ori_map->map[x][y]->tool_type == 13)
        {
            for (auto &req : status->Levels[status->level])
            {
                if (it->type == req.type && req.goal > req.done)
                {
                    status->money += status->shape_value[it->type];
                    req.done++;
                }
            }
            map_moving[x][y] = 0;
            delete it->item;
            it->item = nullptr;
            it = moving_items.erase(it);
        }
        else
        {
            if (ori_map->map[x][y]->tool_type == 5)
            {
                QTransform transform;
                transform.rotate(90);
                it->dir = (it->dir + 1) % 4;
                it->item->setPixmap(it->item->pixmap().transformed(transform));
            }
            ++it;
        }
    }

    // update score
    emit update_score();

    // check if level goal is meet
    if (status->check_level())
    {
        emit update_score();
    }

    // new from extracter
    for (auto &newitem : gen_items)
    {
        int x = newitem.x;
        int y = newitem.y;
        if (newitem.gen_state != 0)
        {
            newitem.gen_state = (newitem.gen_state + 1) % status->ext_en;
            continue;
        }
        if(ori_map->map[x-(1-newitem.dir%2)*(1-newitem.dir)][y-(newitem.dir%2)*(newitem.dir-2)]->tool_type == 1 &&
            ori_map->map[x-(1-newitem.dir%2)*(1-newitem.dir)][y-(newitem.dir%2)*(newitem.dir-2)]->tool_dir == newitem.dir &&
            map_moving[x-(1-newitem.dir%2)*(1-newitem.dir)][y-(newitem.dir%2)*(newitem.dir-2)] == 0)
        {
            add_moving_item(x, y, newitem.type, newitem.dir);
            newitem.gen_state = (newitem.gen_state + 1) % status->ext_en;
        }
    }

    ani_nxt->clear();

    //add move items animations
    for (auto moveitem = moving_items.begin(); moveitem < moving_items.end(); ++moveitem)
    {
        int x = moveitem->x;
        int y = moveitem->y;
        QTransform transform;
        int dir=ori_map->map[x][y]->tool_dir;
        int x1=x-(1-dir%2)*(1-dir),y1=y-(dir%2)*(dir-2);
        int x2=x+(dir%3==0?-1:1), y2=y+(dir>1?-1:1);
        int x3=x+(dir%2)*(2-dir), y3=y+(1-dir%2)*(1-dir);

        //normal conveyor belt
        if (ori_map->map[x][y]->tool_type != 4)
        {
            int dir=ori_map->map[x][y]->tool_dir;
            if (check_free(x1,y1,dir))
            {
                QPropertyAnimation *animation = new QPropertyAnimation(moveitem->item, "pos");
                animation->setDuration(ani_time);
                animation->setStartValue(QPoint(y * CELL_SIZE, x * CELL_SIZE));
                animation->setEndValue(QPoint(y1* CELL_SIZE, x1 * CELL_SIZE));
                map_moving[x1][y1] = 1;
                map_moving[x][y] = 0;
                moveitem->x = x1;
                moveitem->y = y1;
                ani_nxt->addAnimation(animation);
            }
        }

        //cutter
        else if (ori_map->map[x][y]->tool_type == 4 && (moveitem->type == 2 || moveitem->type == 3 || moveitem->type == 4))
        {
            if (ori_map->map[x][y]->tool_state != 0)
            {
                ori_map->map[x][y]->tool_state = (ori_map->map[x][y]->tool_state + 1) % status->cut_en;
                continue;
            }
            //full to half
            if (moveitem->type == 2)
            {
                if (check_free(x1,y1,dir)&&check_free(x2,y2,dir))
                {
                    map_moving[x1][y1] = 1;
                    map_moving[x][y] = 0;
                    moveitem->x=x1;
                    moveitem->y=y1;
                    moveitem->type++;
                    moveitem->dir=dir;
                    QPixmap pic = QPixmap(":/shapes/imgs/rectangle_half.png").scaled(CELL_SIZE, CELL_SIZE);
                    transform.rotate(90*dir);
                    pic = pic.transformed(transform);
                    moveitem->item->setPixmap(pic);

                    QPropertyAnimation *animation = new QPropertyAnimation(moveitem->item, "pos");
                    animation->setDuration(ani_time);
                    animation->setStartValue(QPoint(y * CELL_SIZE, x * CELL_SIZE));
                    animation->setEndValue(QPoint(y1 * CELL_SIZE, x1 * CELL_SIZE));
                    ani_nxt->addAnimation(animation);

                    add_moving_item(x3, y3, 3, (dir+2)%4, &moveitem);
                    map_moving[x2][y2] = 1;
                    map_moving[x3][y3] = 0;
                    moveitem->x=x2;
                    moveitem->y=y2;
                    QPropertyAnimation *animation2 = new QPropertyAnimation(moveitem->item, "pos");
                    animation2->setDuration(ani_time);
                    animation2->setStartValue(QPoint(y3 * CELL_SIZE, x3 * CELL_SIZE));
                    animation2->setEndValue(QPoint(y2 * CELL_SIZE, x2 * CELL_SIZE));
                    ani_nxt->addAnimation(animation2);
                }
            }
            //half to quarter
            else if(moveitem->type == 3&& (dir+moveitem->dir)%2==1)
            {
                if (check_free(x1,y1,dir)&&check_free(x2,y2,dir))
                {
                    int main_dir,side_dir;
                    if((moveitem->dir-dir+8)%4==1){
                        main_dir=dir;
                        side_dir=(dir+1)%4;
                    }
                    else
                    {
                        main_dir=(dir-1+8)%4;
                        side_dir=(dir-2+8)%4;
                    }
                    map_moving[x1][y1] = 1;
                    map_moving[x][y] = 0;
                    moveitem->x=x1;
                    moveitem->y=y1;
                    moveitem->type++;
                    moveitem->dir=main_dir;
                    QPixmap pic = QPixmap(":/shapes/imgs/rectangle_quarter.png").scaled(CELL_SIZE, CELL_SIZE);
                    transform.rotate(90*main_dir);
                    pic = pic.transformed(transform);
                    moveitem->item->setPixmap(pic);

                    QPropertyAnimation *animation = new QPropertyAnimation(moveitem->item, "pos");
                    animation->setDuration(ani_time);
                    animation->setStartValue(QPoint(y * CELL_SIZE, x * CELL_SIZE));
                    animation->setEndValue(QPoint(y1 * CELL_SIZE, x1 * CELL_SIZE));
                    ani_nxt->addAnimation(animation);

                    add_moving_item(x3, y3, 4, side_dir, &moveitem);
                    map_moving[x2][y2] = 1;
                    map_moving[x3][y3] = 0;
                    moveitem->x=x2;
                    moveitem->y=y2;
                    QPropertyAnimation *animation2 = new QPropertyAnimation(moveitem->item, "pos");
                    animation2->setDuration(ani_time);
                    animation2->setStartValue(QPoint(y3 * CELL_SIZE, x3 * CELL_SIZE));
                    animation2->setEndValue(QPoint(y2 * CELL_SIZE, x2 * CELL_SIZE));
                    ani_nxt->addAnimation(animation2);
                }
            }
            //half to half
            else if(moveitem->type == 3&& dir==moveitem->dir)
            {
                if(check_free(x1,y1,dir))
                {
                    QPropertyAnimation *animation = new QPropertyAnimation(moveitem->item, "pos");
                    animation->setDuration(ani_time);
                    animation->setStartValue(QPoint(y * CELL_SIZE, x * CELL_SIZE));
                    animation->setEndValue(QPoint(y1* CELL_SIZE, x1 * CELL_SIZE));
                    map_moving[x1][y1] = 1;
                    map_moving[x][y] = 0;
                    moveitem->x = x1;
                    moveitem->y = y1;
                    ani_nxt->addAnimation(animation);
                }
            }
            else if(moveitem->type == 3&& (dir-moveitem->dir+8)%4==2)
            {
                if(check_free(x2,y2,dir))
                {
                    QPropertyAnimation *animation = new QPropertyAnimation(moveitem->item, "pos");
                    animation->setDuration(ani_time);
                    animation->setStartValue(QPoint(y3 * CELL_SIZE, x3 * CELL_SIZE));
                    animation->setEndValue(QPoint(y2 * CELL_SIZE, x2 * CELL_SIZE));
                    ani_nxt->addAnimation(animation);
                    moveitem->x=x2;
                    moveitem->y=y2;
                    map_moving[x2][y2] = 1;
                    map_moving[x][y] = 0;
                }
            }
            //quarter to quarter
            else if(moveitem->type==4&&((moveitem->dir-dir)==0||(dir-moveitem->dir+8)%4==1))
            {
                if(check_free(x1,y1,dir))
                {
                    QPropertyAnimation *animation = new QPropertyAnimation(moveitem->item, "pos");
                    animation->setDuration(ani_time);
                    animation->setStartValue(QPoint(y * CELL_SIZE, x * CELL_SIZE));
                    animation->setEndValue(QPoint(y1 * CELL_SIZE, x1 * CELL_SIZE));
                    ani_nxt->addAnimation(animation);
                    moveitem->x=x1;
                    moveitem->y=y1;
                    map_moving[x1][y1] = 1;
                    map_moving[x][y] = 0;
                }
            }
            else if(moveitem->type==4&&((dir-moveitem->dir+8)%4==3||(dir-moveitem->dir+8)%4==2))
            {
                if(check_free(x2,y2,dir))
                {
                    QPropertyAnimation *animation = new QPropertyAnimation(moveitem->item, "pos");
                    animation->setDuration(ani_time);
                    animation->setStartValue(QPoint(y3 * CELL_SIZE, x3 * CELL_SIZE));
                    animation->setEndValue(QPoint(y2 * CELL_SIZE, x2 * CELL_SIZE));
                    ani_nxt->addAnimation(animation);
                    moveitem->x=x2;
                    moveitem->y=y2;
                    map_moving[x2][y2] = 1;
                    map_moving[x][y] = 0;
                }
            }
            ori_map->map[x][y]->tool_state = (ori_map->map[x][y]->tool_state + 1) % status->cut_en;
        }
    }

    ani_nxt->start();
}

bool LayerMoving::check_free(int x,int y,int dir){
    if(map_moving[x][y] == 0 &&
        ((ori_map->map[x][y] ->tool_type == 1 && ori_map->map[x][y]->tool_dir != (dir+2)%4) ||
         (ori_map->map[x][y] ->tool_type == 3) ||
         (ori_map->map[x][y] ->tool_type == 13) ||
         (ori_map->map[x][y]->tool_type == 4 && ori_map->map[x][y] ->tool_dir == dir) ||
         (ori_map->map[x][y]->tool_type == 5 && ori_map->map[x][y] ->tool_dir == dir)))
        return true;
    else return false;
}
