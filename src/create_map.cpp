#include "create_map.h"

int Map::tool_chosen = 0;
int Map::tool_dir = 0;
bool Map::stick=false;

Map::Map(int n_row, int n_col, QWidget *parent)
    : QWidget(parent), n_row(n_row), n_col(n_col), map(new Cell **[n_row]), buttons(new LRbutton **[n_row])
{
    for (int i = 0; i < n_row; ++i)
    {
        map[i] = new Cell *[n_col];
        buttons[i] = new LRbutton *[n_col];
        for (int j = 0; j < n_col; ++j)
        {
            map[i][j] = new Cell();
            buttons[i][j] = new LRbutton();
            map[i][j]->shape_type = 0;
            connect(buttons[i][j], &LRbutton::leftClicked, this, [=]()
                    {
                update_map_add(i,j);
                if(!stick)
                {
                parent->setCursor(Qt::ArrowCursor);
                tool_chosen=0;
                tool_dir=0;
                }
            });
            connect(buttons[i][j], &LRbutton::rightClicked, this, [=]()
                    {
                update_map_remove(i,j);
                parent->setCursor(Qt::ArrowCursor);
                tool_chosen=0;
                tool_dir=0;
            });
        }
    }

    center_size = 3;
    // center_x = n_row / 2 - center_size/2;
    // center_y = n_col / 2 - center_size/2;
    center_x = n_row / 2;
    center_y = n_col / 2;

    for (int i = center_x; i < center_x + center_size; i++)
        for (int j = center_y; j < center_y + center_size; j++)
            map[i][j]->tool_type = 13;

    shape_gen(0);
    gridLayout=nullptr;
    display_map();
}

void Map::display_map()
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
            buttons[i][j]->setFixedSize(CELL_SIZE, CELL_SIZE);
            buttons[i][j]->setEnabled(true);
            buttons[i][j]->setStyleSheet("QPushButton { border-radius: 0px; padding: 0px; margin: 0px; }");

            buttons[i][j]->setIconSize(QSize(CELL_SIZE, CELL_SIZE));
            gridLayout->addWidget(buttons[i][j], i, j);
        }
    }
    for (int i = 0; i < n_row; i++)
    {
        buttons[i][0]->setEnabled(false);
        buttons[i][n_col - 1]->setEnabled(false);
    }
    for (int i = 0; i < n_col; i++)
    {
        buttons[0][i]->setEnabled(false);
        buttons[n_row - 1][i]->setEnabled(false);
    }
}

void Map::update_map_add(int x, int y)
{
    if(map[x][y]->tool_type!=9 && map[x][y]->tool_type!=13)
    {
        switch (tool_chosen)
        {
        case 1:
            map[x][y]->tool_type = tool_chosen;
            map[x][y]->tool_dir = tool_dir;
            emit update_layer_object();
            break;
        case 2:
            if (map[x][y]->shape_type != 0)
            {
                map[x][y]->tool_type = tool_chosen;
                map[x][y]->tool_dir = tool_dir;
                emit add_gen_item(x, y, map[x][y]->shape_type, tool_dir);
                emit update_layer_machinery();
            }
            break;
        case 3:
            map[x][y]->tool_type = tool_chosen;
            map[x][y]->tool_dir = tool_dir;
            emit update_layer_machinery();
            break;
        case 4:
            if(map[x+(tool_dir%2)*(2-tool_dir)][y+(1-tool_dir%2)*(1-tool_dir)]->tool_type==0)
            {
                map[x][y]->tool_type = tool_chosen;
                map[x][y]->tool_dir = tool_dir;
                map[x+(tool_dir%2)*(2-tool_dir)][y+(1-tool_dir%2)*(1-tool_dir)]->tool_type = 9;
                map[x+(tool_dir%2)*(2-tool_dir)][y+(1-tool_dir%2)*(1-tool_dir)]->tool_dir = tool_dir;
                emit update_layer_object();
                emit update_layer_machinery();
            }
            break;
        case 5:
            map[x][y]->tool_type = tool_chosen;
            map[x][y]->tool_dir = tool_dir;
            emit update_layer_machinery();
            break;
        default:
            break;
        }
    }
}

void Map::update_map_remove(int x, int y)
{
    if (map[x][y]->tool_type != 0 && map[x][y]->tool_type != 13)
    {
        if (map[x][y]->tool_type == 1)
        {
            map[x][y]->tool_type = 0;
            map[x][y]->tool_dir = 0;
            emit update_layer_object();
        }
        else if (map[x][y]->tool_type == 2)
        {
            map[x][y]->tool_type = 0;
            map[x][y]->tool_dir = 0;
            emit remove_gen_item(x, y);
            emit update_layer_machinery();
        }
        else if(map[x][y]->tool_type == 4 )
        {
            map[x][y]->tool_type = 0;
            map[x][y]->tool_dir = 0;
            map[x+(tool_dir%2)*(2-tool_dir)][y+(1-tool_dir%2)*(1-tool_dir)]->tool_type = 0;
            map[x+(tool_dir%2)*(2-tool_dir)][y+(1-tool_dir%2)*(1-tool_dir)]->tool_dir = 0;
            emit update_layer_object();
            emit update_layer_machinery();
        }
        else
        {
            map[x][y]->tool_type = 0;
            map[x][y]->tool_dir = 0;
            emit update_layer_machinery();
            emit update_layer_object();
        }
        emit remove_moving(x,y); //remove moving item on the cell if there is any
    }
}

void Map::chose_tool(int index)
{
    tool_chosen = index + 1;
    tool_dir = 0;
}

void Map::larger_receiver(){
    center_size+=1;
    for (int i = center_x; i < center_x + center_size; i++)
        for (int j = center_y; j < center_y + center_size; j++)
            map[i][j]->tool_type = 13;
    emit update_layer_machinery();
}

void Map::more_shapes(int i){
    shape_gen(i);
    emit update_layer_object();
}

void Map::shape_gen(int i){
    switch (i)
    {
    case 0:
        for (int i = n_row / 2-5; i < n_row / 2-3; i++)
            for (int j = n_col / 2-6; j < n_col / 2-3; j++)
                if(map[i][j]->tool_type == 0)
                    map[i][j]->shape_type = 1;
        for (int i = n_row / 2+7; i < n_row / 2+9; i++)
            for (int j = n_col / 2+6; j < n_col / 2+9; j++)
                if(map[i][j]->tool_type == 0)
                    map[i][j]->shape_type = 2;
        break;
    case 1:
        for (int i = n_row / 2-7; i < n_row / 2-3; i++)
            for (int j = n_col / 2; j < n_col / 2+1; j++)
                if(map[i][j]->tool_type == 0)
                    map[i][j]->shape_type = 2;
        for (int i = n_row / 2+9; i < n_row / 2+11; i++)
            for (int j = n_col / 2+5; j < n_col / 2+8; j++)
                if(map[i][j]->tool_type == 0)
                    map[i][j]->shape_type = 1;
        break;
    case 2:
        for (int i = n_row / 2 - 9; i < n_row / 2 - 8; i++)
            for (int j = n_col / 2 - 10; j < n_col / 2 - 7; j++)
                if (map[i][j]->tool_type == 0)
                    map[i][j]->shape_type = 2;
        for (int i = n_row / 2 + 5; i < n_row / 2 + 8; i++)
            for (int j = n_col / 2 - 5; j < n_col / 2 - 3; j++)
                if (map[i][j]->tool_type == 0)
                    map[i][j]->shape_type = 1;
        break;
    case 3:
        for (int i = n_row / 2 - 8; i < n_row / 2 - 5; i++)
            for (int j = n_col / 2 + 12; j < n_col / 2 + 14; j++)
                if (map[i][j]->tool_type == 0)
                    map[i][j]->shape_type = 1;
        for (int i = n_row / 2 + 9; i < n_row / 2 + 11; i++)
            for (int j = n_col / 2 - 12; j < n_col / 2 - 9; j++)
                if (map[i][j]->tool_type == 0)
                    map[i][j]->shape_type = 2;
        break;
    case 4:
        // for enlarged map only
        for (int i = n_row / 2 - 15; i < n_row / 2 - 13; i++)
            for (int j = n_col / 2 - 3; j < n_col / 2 + 1; j++)
                if (map[i][j]->tool_type == 0)
                    map[i][j]->shape_type = 1;
        for (int i = n_row / 2 - 2; i < n_row / 2 + 2; i++)
            for (int j = n_col / 2 - 22; j < n_col / 2 - 20; j++)
                if (map[i][j]->tool_type == 0)
                    map[i][j]->shape_type = 2;
        break;
    case 5:
        // for enlarged map only
        for (int i = n_row / 2 - 16; i < n_row / 2 -15; i++)
            for (int j = n_col / 2 - 4; j < n_col / 2; j++)
                if (map[i][j]->tool_type == 0)
                    map[i][j]->shape_type = 1;
        for (int i = n_row / 2  + 15; i < n_row / 2 + 16; i++)
            for (int j = n_col / 2 + 19; j < n_col / 2 + 23; j++)
                if (map[i][j]->tool_type == 0)
                    map[i][j]->shape_type = 2;
        break;
    default:
        break;
    }
}

Map::~Map()
{
    for (int i = 0; i < n_row; ++i)
    {
        for (int j = 0; j < n_col; ++j)
        {
            delete map[i][j];
            delete buttons[i][j];
        }
        delete[] map[i];
        delete[] buttons[i];
    }
    delete[] map;
    delete[] buttons;
    delete gridLayout;
}
