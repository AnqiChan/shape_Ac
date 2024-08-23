#include "guide.h"

Guide::Guide( QWidget *parent)  : QWidget(parent)
//set the window of the user guide
{
    setWindowTitle("Guide");
    setFixedSize(300, 400);
    setGeometry(828, 295, 300, 400);
    setWindowFlags(Qt::FramelessWindowHint);
    layout=new QVBoxLayout(this);
    setLayout(layout);

    title = new QLabel("A Guide for the Explorer",this);
    title->setStyleSheet("font-size: 18pt; font-family: Arial; font-weight: bold;");
    layout->addWidget(title);

    basic=new QLabel("Basics",this);
    basic->setStyleSheet("font-size: 12pt; font-family: Arial; font-weight: bold;");
    layout->addWidget(basic);

    basics[0]=new QLabel("Left click to place",this);
    layout->addWidget(basics[0]);
    basics[1]=new QLabel("Right click to remove",this);
    layout->addWidget(basics[1]);
    basics[2]=new QLabel("Press R to rotate",this);
    layout->addWidget(basics[2]);
    basics[3]=new QLabel("Press S to enable stick",this);
    layout->addWidget(basics[3]);

    obj_info=new QLabel("About Resources",this);
    obj_info->setStyleSheet("font-size: 12pt; font-family: Arial; font-weight: bold;");
    layout->addWidget(obj_info);

    obj_infos[0]=new QLabel("Those shaped square can be cut",this);
    layout->addWidget(obj_infos[0]);
    obj_infos[1]=new QLabel("Those shaped circle can not be cut",this);
    layout->addWidget(obj_infos[1]);

    up=new QLabel("About Upgrades",this);
    up->setStyleSheet("font-size: 12pt; font-family: Arial; font-weight: bold;");
    layout->addWidget(up);

    ups[0]=new QLabel("Go to Upgrade Center to get upgrades",this);
    layout->addWidget(ups[0]);
    ups[1]=new QLabel("Local reinforcements will be removed each level",this);
    layout->addWidget(ups[1]);
    ups[2]=new QLabel("Global reinforcements stays",this);
    layout->addWidget(ups[2]);
}


void Guide::change_state()
//open and close window on click
{
    if (state)
    {
        state = false;
        close();
    }
    else
    {
        state = true;
        show();
    }
}
