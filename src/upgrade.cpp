#include "upgrade.h"

Upgrade::Upgrade(GameStatus *status, QWidget *parent)
    : QWidget(parent)
// set the upgrade window
{
    this->status = status;
    this->setStyleSheet("QPushButton {"
                        "  color: black;"
                        "}"
                        "QPushButton:disabled {"
                        "  color: #808080;"
                        "}");
    setWindowTitle("Upgrade Center");
    setFixedSize(300, 400);
    setGeometry(828, 295, 300, 400);
    setWindowFlags(Qt::FramelessWindowHint);
    layout = new QGridLayout(this);
    setLayout(layout);
    player_info = new QLabel("Player Info", this);
    player_info->setStyleSheet("font-size: 12pt; font-family: Arial; font-weight: bold;");
    layout->addWidget(player_info, 0, 0, 1, 2);

    level = new QLabel("Level: " + QString::number(status->level + 1), this);
    layout->addWidget(level, 1, 0, 1, 2);
    money = new QLabel("Money: " + QString::number(status->money), this);
    layout->addWidget(money, 2, 0, 1, 2);

    local = new QLabel("Local Reinforcement", this);
    local->setStyleSheet("font-size: 12pt; font-family: Arial; font-weight: bold;");
    layout->addWidget(local, 3, 0, 1, 2);

    locals[0] = new QPushButton("Upgrade Extractor", this);
    locals[0]->setStyleSheet("QPushButton { text-align:left; }");
    connect(locals[0], SIGNAL(clicked()), this, SLOT(update_local1()));
    layout->addWidget(locals[0], 4, 0, 1, 2);

    locals[1] = new QPushButton("Upgrade Cutter", this);
    locals[1]->setStyleSheet("QPushButton { text-align:left; }");
    connect(locals[1], SIGNAL(clicked()), this, SLOT(update_local2()));
    layout->addWidget(locals[1], 5, 0, 1, 2);

    locals[2] = new QPushButton("Upgrade Conveyor_Belt", this);
    locals[2]->setStyleSheet("QPushButton { text-align:left; }");
    connect(locals[2], SIGNAL(clicked()), this, SLOT(update_local3()));
    layout->addWidget(locals[2], 6, 0, 1, 2);

    global = new QLabel("Global Reinforcement", this);
    global->setStyleSheet("font-size: 12pt; font-family: Arial; font-weight: bold;");
    layout->addWidget(global, 7, 0, 1, 2);

    reward = new QLabel("Delivery Reward ", this);
    layout->addWidget(reward, 8, 0, 1, 2);

    globals[0] = new QPushButton(this);
    globals[0]->setIcon(QIcon(":/shapes/imgs/circle.png"));
    globals[0]->setText(QString::number(status->shape_value[1]) + " to " + QString::number(status->shape_value[1]*2)+"  at  "+QString::number(status->shape_value[1]*10));
    connect(globals[0] ,&QPushButton::clicked,this,[=](){update_global1(1);});
    layout->addWidget(globals[0] , 9, 0, 1, 1);

    globals[1]  = new QPushButton(this);
    globals[1]  ->setIcon(QIcon(":/shapes/imgs/rectangle.png"));
    globals[1]  ->setText(QString::number(status->shape_value[2]) + " to " + QString::number(status->shape_value[2]*2)+"  at  "+QString::number(status->shape_value[2]*10));
    connect(globals[1] ,&QPushButton::clicked,this,[=](){update_global1(2);});
    layout->addWidget(globals[1]  , 9, 1, 1, 1);

    globals[2]   = new QPushButton(this);
    globals[2]   ->setIcon(QIcon(":/shapes/imgs/rectangle_half.png"));
    globals[2]   ->setText(QString::number(status->shape_value[3]) + " to " + QString::number(status->shape_value[3]*2)+"  at  "+QString::number(status->shape_value[3]*10));
    connect(globals[2] ,&QPushButton::clicked,this,[=](){update_global1(3);});
    layout->addWidget(globals[2]  , 10, 0, 1, 1);

    globals[3]=new QPushButton(this);
    globals[3]->setIcon(QIcon(":/shapes/imgs/rectangle_quarter.png"));
    globals[3]->setText(QString::number(status->shape_value[4]) + " to " + QString::number(status->shape_value[4]*2)+"  at  "+QString::number(status->shape_value[4]*10));
    connect(globals[3] ,&QPushButton::clicked,this,[=](){update_global1(4);});
    layout->addWidget(globals[3], 10, 1, 1, 1);

    map_up=new QLabel("Map Upgrades",this);
    layout->addWidget(map_up,11,0,1,2);

    map_ups[0]=new QPushButton(this);
    map_ups[0]->setText("Lager Receiving Center  at "+QString::number(50*(status->map_up_times[0]+1)));
    map_ups[0]->setStyleSheet("QPushButton { text-align:left; }");
    connect(map_ups[0],&QPushButton::clicked,this,&Upgrade::update_global2);
    layout->addWidget(map_ups[0],12,0,1,2);

    map_ups[1]=new QPushButton(this);
    map_ups[1]->setText("More Shpae Resource  at "+QString::number(50*(status->map_up_times[1]+1)));
    map_ups[1]->setStyleSheet("QPushButton { text-align:left; }");
    connect(map_ups[1],&QPushButton::clicked,this,&Upgrade::update_global3);
    layout->addWidget(map_ups[1],13,0,1,2);

    map_ups[2]=new QPushButton(this);
    map_ups[2]->setText("Larger Map  at "+QString::number(50*(status->map_up_times[2]+1)));
    map_ups[2]->setStyleSheet("QPushButton { text-align:left; }");
    connect(map_ups[2],&QPushButton::clicked,this,&Upgrade::update_global4);
    layout->addWidget(map_ups[2],14,0,1,2);

}

void Upgrade::open_upgrade()
//update and open the upgrade window
{
    update_display();
    show();
}

void Upgrade::close_upgrade()
//close the upgrade window
{
    close();
}

void Upgrade::change_state()
//open and close the upgrade window
{
    if (state)
    {
        state = false;
        close_upgrade();
    }
    else
    {
        state = true;
        open_upgrade();
    }
}

void Upgrade::update_local1()
//faster extractor
{
    status->enhanced[0] = true;
    locals[0]->setEnabled(false);
    status->ext_en /= 2;
    update_display();
}

void Upgrade::update_local2()
//faster cutter
{
    status->enhanced[1] = true;
    locals[1]->setEnabled(false);
    status->cut_en /= 2;
    update_display();
}

void Upgrade::update_local3()
//faster conveyor belt
{
    status->enhanced[2] = true;
    locals[2]->setEnabled(false);
    status->ani_rate *= 2;
    status->cut_en *= 2;
    status->ext_en *= 2;
    status->update_clk();
    update_display();
}

void Upgrade::update_global1(int i)
//more reward
{
    status->money-=status->shape_value[i]*10;
    status->shape_value[i] *= 2;
    update_display();
}

void Upgrade::update_global2()
//larger receiver
{
    status->money-=50*(status->map_up_times[0]+1);
    emit larger_receiver();
    status->map_up_times[0]++;
    update_display();
}

void Upgrade::update_global3()
//more shapes
{
    status->money-=100*(status->map_up_times[1]+1);
    status->map_up_times[1]++;
    emit more_shapes(status->map_up_times[1]);
    update_display();
}

void Upgrade::update_global4()
//larger map
{
    status->money-=150*(status->map_up_times[2]+1);
    status->map_up_times[2]++;
    emit larger_map();
    update_display();
}

void Upgrade::update_display()
//update the display of the upgrade window
{
    level->setText("Level: " + QString::number(status->level + 1));
    money->setText("Money: " + QString::number(status->money));
    status->check_enhance();
    for (int i = 0; i < 3; i++)
    {
        if (status->enhanceable>0 && status->enhanced[i] == false)
            locals[i]->setEnabled(true);
        else
            locals[i]->setEnabled(false);
    }
    for(int i=0;i<4;i++){
        globals[i]->setText(QString::number(status->shape_value[i+1]) + " to " + QString::number(status->shape_value[i+1]*2)+"  at  "+QString::number(status->shape_value[i+1]*10));
        if(status->money>=status->shape_value[i+1]*10)
            globals[i]->setEnabled(true);
        else
            globals[i]->setEnabled(false);
    }
    map_ups[0]->setText("Lager Receiving Center  at "+QString::number(50*(status->map_up_times[0]+1)));
    map_ups[1]->setText("More Shpae Resource  at "+QString::number(100*(status->map_up_times[1]+1)));
    map_ups[2]->setText("Larger Map  at "+QString::number(150*(status->map_up_times[2]+1)));

    if(status->money>=50*(status->map_up_times[0]+1))
        map_ups[0]->setEnabled(true);
    else
        map_ups[0]->setEnabled(false);

    if(status->money>=100*(status->map_up_times[1]+1)&& status->map_up_times[1]<6 &&!(status->map_up_times[1]>3 &&status-> map_up_times[2]==0))
        map_ups[1]->setEnabled(true);
    else
        map_ups[1]->setEnabled(false);

    if(status->money>=150*(status->map_up_times[2]+1))
        map_ups[2]->setEnabled(true);
    else
        map_ups[2]->setEnabled(false);
}
